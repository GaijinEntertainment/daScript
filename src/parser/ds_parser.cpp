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
  YYSYMBOL_type_declaration_no_options_list = 282, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 283,       /* expression_keyword  */
  YYSYMBOL_284_7 = 284,                    /* $@7  */
  YYSYMBOL_285_8 = 285,                    /* $@8  */
  YYSYMBOL_286_9 = 286,                    /* $@9  */
  YYSYMBOL_287_10 = 287,                   /* $@10  */
  YYSYMBOL_expr_pipe = 288,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 289,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 290,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 291,     /* new_type_declaration  */
  YYSYMBOL_292_11 = 292,                   /* $@11  */
  YYSYMBOL_293_12 = 293,                   /* $@12  */
  YYSYMBOL_expr_new = 294,                 /* expr_new  */
  YYSYMBOL_expression_break = 295,         /* expression_break  */
  YYSYMBOL_expression_continue = 296,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 297, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 298,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 299, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 300,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 301,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 302,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 303,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 304,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 305,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 306, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 307,           /* expression_let  */
  YYSYMBOL_expr_cast = 308,                /* expr_cast  */
  YYSYMBOL_309_13 = 309,                   /* $@13  */
  YYSYMBOL_310_14 = 310,                   /* $@14  */
  YYSYMBOL_311_15 = 311,                   /* $@15  */
  YYSYMBOL_312_16 = 312,                   /* $@16  */
  YYSYMBOL_313_17 = 313,                   /* $@17  */
  YYSYMBOL_314_18 = 314,                   /* $@18  */
  YYSYMBOL_expr_type_decl = 315,           /* expr_type_decl  */
  YYSYMBOL_316_19 = 316,                   /* $@19  */
  YYSYMBOL_317_20 = 317,                   /* $@20  */
  YYSYMBOL_expr_type_info = 318,           /* expr_type_info  */
  YYSYMBOL_expr_list = 319,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 320,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 321,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 322,            /* capture_entry  */
  YYSYMBOL_capture_list = 323,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 324,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 325,               /* expr_block  */
  YYSYMBOL_expr_full_block = 326,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 327, /* expr_full_block_assumed_piped  */
  YYSYMBOL_328_21 = 328,                   /* $@21  */
  YYSYMBOL_expr_numeric_const = 329,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 330,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 331,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 332,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 333,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 334,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 335,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 336,           /* func_addr_expr  */
  YYSYMBOL_337_22 = 337,                   /* $@22  */
  YYSYMBOL_338_23 = 338,                   /* $@23  */
  YYSYMBOL_339_24 = 339,                   /* $@24  */
  YYSYMBOL_340_25 = 340,                   /* $@25  */
  YYSYMBOL_expr_field = 341,               /* expr_field  */
  YYSYMBOL_342_26 = 342,                   /* $@26  */
  YYSYMBOL_343_27 = 343,                   /* $@27  */
  YYSYMBOL_expr_call = 344,                /* expr_call  */
  YYSYMBOL_expr = 345,                     /* expr  */
  YYSYMBOL_346_28 = 346,                   /* $@28  */
  YYSYMBOL_347_29 = 347,                   /* $@29  */
  YYSYMBOL_348_30 = 348,                   /* $@30  */
  YYSYMBOL_349_31 = 349,                   /* $@31  */
  YYSYMBOL_350_32 = 350,                   /* $@32  */
  YYSYMBOL_351_33 = 351,                   /* $@33  */
  YYSYMBOL_expr_mtag = 352,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 353, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 354,        /* optional_override  */
  YYSYMBOL_optional_constant = 355,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 356, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 357, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 358, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 359, /* struct_variable_declaration_list  */
  YYSYMBOL_360_34 = 360,                   /* $@34  */
  YYSYMBOL_361_35 = 361,                   /* $@35  */
  YYSYMBOL_362_36 = 362,                   /* $@36  */
  YYSYMBOL_function_argument_declaration = 363, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 364,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 365,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 366,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 367,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 368,             /* variant_type  */
  YYSYMBOL_variant_type_list = 369,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 370,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 371,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 372,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 373,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 374,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 375, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 376, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 377, /* global_variable_declaration_list  */
  YYSYMBOL_378_37 = 378,                   /* $@37  */
  YYSYMBOL_optional_shared = 379,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 380, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 381,               /* global_let  */
  YYSYMBOL_382_38 = 382,                   /* $@38  */
  YYSYMBOL_enum_list = 383,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 384, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 385,             /* single_alias  */
  YYSYMBOL_386_39 = 386,                   /* $@39  */
  YYSYMBOL_alias_list = 387,               /* alias_list  */
  YYSYMBOL_alias_declaration = 388,        /* alias_declaration  */
  YYSYMBOL_389_40 = 389,                   /* $@40  */
  YYSYMBOL_optional_public_or_private_enum = 390, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 391,                /* enum_name  */
  YYSYMBOL_enum_declaration = 392,         /* enum_declaration  */
  YYSYMBOL_393_41 = 393,                   /* $@41  */
  YYSYMBOL_394_42 = 394,                   /* $@42  */
  YYSYMBOL_395_43 = 395,                   /* $@43  */
  YYSYMBOL_396_44 = 396,                   /* $@44  */
  YYSYMBOL_optional_structure_parent = 397, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 398,          /* optional_sealed  */
  YYSYMBOL_structure_name = 399,           /* structure_name  */
  YYSYMBOL_class_or_struct = 400,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 401, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 402, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 403,    /* structure_declaration  */
  YYSYMBOL_404_45 = 404,                   /* $@45  */
  YYSYMBOL_405_46 = 405,                   /* $@46  */
  YYSYMBOL_variable_name_with_pos_list = 406, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 407,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 408, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 409, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 410,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 411,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 412,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 413, /* bitfield_type_declaration  */
  YYSYMBOL_414_47 = 414,                   /* $@47  */
  YYSYMBOL_415_48 = 415,                   /* $@48  */
  YYSYMBOL_table_type_pair = 416,          /* table_type_pair  */
  YYSYMBOL_dim_list = 417,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 418, /* type_declaration_no_options  */
  YYSYMBOL_419_49 = 419,                   /* $@49  */
  YYSYMBOL_420_50 = 420,                   /* $@50  */
  YYSYMBOL_421_51 = 421,                   /* $@51  */
  YYSYMBOL_422_52 = 422,                   /* $@52  */
  YYSYMBOL_423_53 = 423,                   /* $@53  */
  YYSYMBOL_424_54 = 424,                   /* $@54  */
  YYSYMBOL_425_55 = 425,                   /* $@55  */
  YYSYMBOL_426_56 = 426,                   /* $@56  */
  YYSYMBOL_427_57 = 427,                   /* $@57  */
  YYSYMBOL_428_58 = 428,                   /* $@58  */
  YYSYMBOL_429_59 = 429,                   /* $@59  */
  YYSYMBOL_430_60 = 430,                   /* $@60  */
  YYSYMBOL_431_61 = 431,                   /* $@61  */
  YYSYMBOL_432_62 = 432,                   /* $@62  */
  YYSYMBOL_433_63 = 433,                   /* $@63  */
  YYSYMBOL_434_64 = 434,                   /* $@64  */
  YYSYMBOL_435_65 = 435,                   /* $@65  */
  YYSYMBOL_436_66 = 436,                   /* $@66  */
  YYSYMBOL_437_67 = 437,                   /* $@67  */
  YYSYMBOL_438_68 = 438,                   /* $@68  */
  YYSYMBOL_439_69 = 439,                   /* $@69  */
  YYSYMBOL_440_70 = 440,                   /* $@70  */
  YYSYMBOL_441_71 = 441,                   /* $@71  */
  YYSYMBOL_442_72 = 442,                   /* $@72  */
  YYSYMBOL_443_73 = 443,                   /* $@73  */
  YYSYMBOL_444_74 = 444,                   /* $@74  */
  YYSYMBOL_445_75 = 445,                   /* $@75  */
  YYSYMBOL_type_declaration = 446,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 447,  /* tuple_alias_declaration  */
  YYSYMBOL_448_76 = 448,                   /* $@76  */
  YYSYMBOL_449_77 = 449,                   /* $@77  */
  YYSYMBOL_450_78 = 450,                   /* $@78  */
  YYSYMBOL_451_79 = 451,                   /* $@79  */
  YYSYMBOL_variant_alias_declaration = 452, /* variant_alias_declaration  */
  YYSYMBOL_453_80 = 453,                   /* $@80  */
  YYSYMBOL_454_81 = 454,                   /* $@81  */
  YYSYMBOL_455_82 = 455,                   /* $@82  */
  YYSYMBOL_456_83 = 456,                   /* $@83  */
  YYSYMBOL_bitfield_alias_declaration = 457, /* bitfield_alias_declaration  */
  YYSYMBOL_458_84 = 458,                   /* $@84  */
  YYSYMBOL_459_85 = 459,                   /* $@85  */
  YYSYMBOL_460_86 = 460,                   /* $@86  */
  YYSYMBOL_461_87 = 461,                   /* $@87  */
  YYSYMBOL_make_decl = 462,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 463,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 464,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 465,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 466,          /* make_struct_dim  */
  YYSYMBOL_make_struct_dim_list = 467,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 468,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_block = 469,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 470, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 471, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 472, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 473, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 474, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_use_initializer = 475,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 476,         /* make_struct_decl  */
  YYSYMBOL_477_88 = 477,                   /* $@88  */
  YYSYMBOL_478_89 = 478,                   /* $@89  */
  YYSYMBOL_479_90 = 479,                   /* $@90  */
  YYSYMBOL_480_91 = 480,                   /* $@91  */
  YYSYMBOL_481_92 = 481,                   /* $@92  */
  YYSYMBOL_482_93 = 482,                   /* $@93  */
  YYSYMBOL_483_94 = 483,                   /* $@94  */
  YYSYMBOL_484_95 = 484,                   /* $@95  */
  YYSYMBOL_make_tuple = 485,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 486,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 487,          /* make_tuple_call  */
  YYSYMBOL_488_96 = 488,                   /* $@96  */
  YYSYMBOL_489_97 = 489,                   /* $@97  */
  YYSYMBOL_make_dim = 490,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 491,            /* make_dim_decl  */
  YYSYMBOL_492_98 = 492,                   /* $@98  */
  YYSYMBOL_493_99 = 493,                   /* $@99  */
  YYSYMBOL_494_100 = 494,                  /* $@100  */
  YYSYMBOL_495_101 = 495,                  /* $@101  */
  YYSYMBOL_496_102 = 496,                  /* $@102  */
  YYSYMBOL_497_103 = 497,                  /* $@103  */
  YYSYMBOL_498_104 = 498,                  /* $@104  */
  YYSYMBOL_499_105 = 499,                  /* $@105  */
  YYSYMBOL_500_106 = 500,                  /* $@106  */
  YYSYMBOL_501_107 = 501,                  /* $@107  */
  YYSYMBOL_make_table = 502,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 503,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 504,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 505, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 506,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 507       /* array_comprehension  */
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
#define YYLAST   14870

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  217
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  291
/* YYNRULES -- Number of rules.  */
#define YYNRULES  875
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1661

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
       0,   540,   540,   541,   546,   547,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,   562,   568,   569,
     570,   574,   575,   579,   597,   598,   599,   600,   604,   608,
     613,   622,   630,   646,   651,   659,   659,   698,   728,   732,
     733,   734,   738,   741,   745,   751,   760,   763,   769,   770,
     774,   778,   779,   783,   786,   792,   798,   801,   807,   808,
     812,   813,   814,   823,   824,   828,   829,   829,   835,   836,
     837,   838,   839,   843,   849,   849,   855,   855,   861,   869,
     879,   888,   888,   895,   896,   897,   898,   899,   900,   904,
     909,   917,   918,   919,   923,   924,   925,   926,   927,   928,
     929,   930,   936,   939,   945,   948,   954,   955,   956,   960,
     973,   991,   994,  1002,  1013,  1024,  1035,  1038,  1045,  1049,
    1056,  1057,  1061,  1062,  1063,  1067,  1070,  1077,  1081,  1082,
    1083,  1084,  1085,  1086,  1087,  1088,  1089,  1090,  1091,  1092,
    1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,  1101,  1102,
    1103,  1104,  1105,  1106,  1107,  1108,  1109,  1110,  1111,  1112,
    1113,  1114,  1115,  1116,  1117,  1118,  1119,  1120,  1121,  1122,
    1123,  1124,  1125,  1126,  1127,  1128,  1129,  1130,  1131,  1132,
    1133,  1134,  1135,  1136,  1137,  1138,  1139,  1140,  1141,  1142,
    1143,  1144,  1145,  1146,  1147,  1148,  1149,  1150,  1151,  1152,
    1153,  1154,  1155,  1156,  1157,  1158,  1159,  1160,  1161,  1162,
    1163,  1164,  1169,  1187,  1188,  1189,  1193,  1199,  1199,  1216,
    1220,  1231,  1240,  1252,  1253,  1254,  1255,  1256,  1257,  1258,
    1259,  1260,  1261,  1262,  1263,  1264,  1265,  1266,  1267,  1268,
    1269,  1270,  1271,  1272,  1276,  1281,  1287,  1293,  1304,  1305,
    1309,  1310,  1314,  1318,  1325,  1325,  1325,  1331,  1331,  1331,
    1340,  1374,  1377,  1380,  1383,  1389,  1390,  1401,  1405,  1408,
    1416,  1416,  1416,  1419,  1425,  1428,  1432,  1436,  1443,  1450,
    1456,  1460,  1464,  1467,  1470,  1478,  1481,  1484,  1492,  1495,
    1503,  1506,  1509,  1517,  1523,  1524,  1525,  1529,  1530,  1534,
    1535,  1539,  1544,  1552,  1558,  1564,  1573,  1585,  1588,  1594,
    1594,  1594,  1597,  1597,  1597,  1602,  1602,  1602,  1610,  1610,
    1610,  1616,  1626,  1637,  1650,  1660,  1671,  1686,  1689,  1695,
    1696,  1703,  1714,  1715,  1716,  1720,  1721,  1722,  1723,  1727,
    1732,  1740,  1741,  1745,  1750,  1757,  1764,  1764,  1773,  1774,
    1775,  1776,  1777,  1778,  1779,  1783,  1784,  1785,  1786,  1787,
    1788,  1789,  1790,  1791,  1792,  1793,  1794,  1795,  1796,  1797,
    1798,  1799,  1800,  1801,  1805,  1806,  1807,  1808,  1813,  1814,
    1815,  1816,  1817,  1818,  1819,  1820,  1821,  1822,  1823,  1824,
    1825,  1826,  1827,  1828,  1829,  1834,  1841,  1853,  1858,  1868,
    1872,  1879,  1882,  1882,  1882,  1887,  1887,  1887,  1900,  1904,
    1908,  1913,  1920,  1920,  1920,  1927,  1931,  1940,  1944,  1947,
    1953,  1954,  1955,  1956,  1957,  1958,  1959,  1960,  1961,  1962,
    1963,  1964,  1965,  1966,  1967,  1968,  1969,  1970,  1971,  1972,
    1973,  1974,  1975,  1976,  1977,  1978,  1979,  1980,  1981,  1982,
    1983,  1984,  1985,  1986,  1987,  1988,  1994,  1995,  1996,  1997,
    1998,  2011,  2012,  2013,  2014,  2015,  2016,  2017,  2018,  2019,
    2020,  2021,  2022,  2025,  2028,  2029,  2032,  2032,  2032,  2035,
    2040,  2044,  2048,  2048,  2048,  2053,  2056,  2060,  2060,  2060,
    2065,  2068,  2069,  2070,  2071,  2072,  2073,  2074,  2075,  2076,
    2078,  2082,  2083,  2088,  2092,  2093,  2094,  2095,  2096,  2097,
    2098,  2102,  2106,  2110,  2114,  2118,  2122,  2126,  2130,  2134,
    2141,  2142,  2143,  2147,  2148,  2149,  2153,  2154,  2158,  2159,
    2160,  2164,  2165,  2169,  2180,  2183,  2183,  2202,  2201,  2216,
    2215,  2228,  2237,  2246,  2256,  2257,  2261,  2264,  2273,  2274,
    2278,  2281,  2284,  2300,  2309,  2310,  2314,  2317,  2320,  2334,
    2335,  2339,  2345,  2351,  2354,  2358,  2364,  2373,  2374,  2375,
    2379,  2380,  2384,  2391,  2396,  2405,  2411,  2422,  2425,  2430,
    2435,  2443,  2454,  2457,  2457,  2477,  2478,  2482,  2483,  2484,
    2488,  2491,  2491,  2510,  2513,  2516,  2531,  2550,  2551,  2552,
    2557,  2557,  2583,  2584,  2588,  2589,  2589,  2593,  2594,  2595,
    2599,  2609,  2614,  2609,  2626,  2631,  2626,  2646,  2647,  2651,
    2652,  2656,  2662,  2663,  2667,  2668,  2669,  2673,  2676,  2682,
    2687,  2682,  2701,  2708,  2713,  2722,  2728,  2739,  2740,  2741,
    2742,  2743,  2744,  2745,  2746,  2747,  2748,  2749,  2750,  2751,
    2752,  2753,  2754,  2755,  2756,  2757,  2758,  2759,  2760,  2761,
    2762,  2763,  2764,  2765,  2769,  2770,  2771,  2772,  2773,  2774,
    2775,  2776,  2780,  2791,  2795,  2802,  2814,  2821,  2830,  2835,
    2838,  2851,  2851,  2851,  2864,  2868,  2875,  2879,  2886,  2887,
    2888,  2889,  2890,  2905,  2911,  2911,  2911,  2915,  2920,  2927,
    2927,  2934,  2938,  2942,  2947,  2952,  2957,  2962,  2966,  2970,
    2975,  2979,  2983,  2988,  2988,  2988,  2994,  3001,  3001,  3001,
    3006,  3006,  3006,  3012,  3012,  3012,  3017,  3021,  3021,  3021,
    3026,  3026,  3026,  3035,  3039,  3039,  3039,  3044,  3044,  3044,
    3053,  3057,  3057,  3057,  3062,  3062,  3062,  3071,  3071,  3071,
    3077,  3077,  3077,  3086,  3089,  3100,  3116,  3116,  3121,  3126,
    3116,  3151,  3151,  3156,  3162,  3151,  3187,  3187,  3192,  3197,
    3187,  3227,  3228,  3229,  3230,  3231,  3235,  3242,  3249,  3255,
    3261,  3268,  3275,  3281,  3290,  3296,  3304,  3309,  3316,  3321,
    3328,  3333,  3339,  3340,  3345,  3346,  3350,  3351,  3355,  3356,
    3360,  3361,  3362,  3366,  3367,  3368,  3372,  3373,  3377,  3383,
    3390,  3398,  3405,  3413,  3422,  3422,  3422,  3430,  3430,  3430,
    3437,  3437,  3437,  3444,  3444,  3444,  3455,  3458,  3464,  3478,
    3484,  3490,  3496,  3496,  3496,  3506,  3511,  3518,  3526,  3531,
    3538,  3538,  3538,  3548,  3548,  3548,  3558,  3558,  3558,  3568,
    3576,  3576,  3576,  3584,  3591,  3591,  3591,  3601,  3606,  3613,
    3616,  3622,  3630,  3639,  3647,  3655,  3668,  3669,  3673,  3674,
    3679,  3682,  3685,  3688,  3691,  3694
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
  "optional_expr_list_in_braces", "type_declaration_no_options_list",
  "expression_keyword", "$@7", "$@8", "$@9", "$@10", "expr_pipe",
  "name_in_namespace", "expression_delete", "new_type_declaration", "$@11",
  "$@12", "expr_new", "expression_break", "expression_continue",
  "expression_return_no_pipe", "expression_return",
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
  "bitfield_type_declaration", "$@47", "$@48", "table_type_pair",
  "dim_list", "type_declaration_no_options", "$@49", "$@50", "$@51",
  "$@52", "$@53", "$@54", "$@55", "$@56", "$@57", "$@58", "$@59", "$@60",
  "$@61", "$@62", "$@63", "$@64", "$@65", "$@66", "$@67", "$@68", "$@69",
  "$@70", "$@71", "$@72", "$@73", "$@74", "$@75", "type_declaration",
  "tuple_alias_declaration", "$@76", "$@77", "$@78", "$@79",
  "variant_alias_declaration", "$@80", "$@81", "$@82", "$@83",
  "bitfield_alias_declaration", "$@84", "$@85", "$@86", "$@87",
  "make_decl", "make_struct_fields", "make_variant_dim",
  "make_struct_single", "make_struct_dim", "make_struct_dim_list",
  "make_struct_dim_decl", "optional_block",
  "optional_trailing_semicolon_cur_cur",
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

#define YYPACT_NINF (-1464)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-745)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1464,   124, -1464, -1464,    39,   -44,   222,   -22, -1464,   -92,
     355,   355,   355, -1464,   164,   334, -1464, -1464,    13, -1464,
   -1464, -1464,   402, -1464,   155, -1464, -1464, -1464, -1464, -1464,
   -1464, -1464, -1464, -1464, -1464,   112, -1464,   133,   290,   332,
   -1464, -1464, -1464, -1464,   222, -1464,    21, -1464,   355,   355,
   -1464, -1464,   155, -1464, -1464, -1464, -1464, -1464,   367,   187,
   -1464, -1464, -1464,   334,   334,   334,   328, -1464,   645,    72,
   -1464, -1464, -1464, -1464,   482,   581,   595, -1464,   609,    30,
      39,   452,   -44,   427,   470, -1464,   565,   565, -1464,   494,
     483,    -2,   487,   619,   517,   524,   537, -1464,   542,   500,
   -1464, -1464,   -62,    39,   334,   334,   334,   334, -1464, -1464,
   -1464, -1464, -1464, -1464, -1464,   545, -1464, -1464, -1464, -1464,
   -1464,   540, -1464, -1464, -1464, -1464, -1464,   415,   156, -1464,
   -1464, -1464, -1464,   664, -1464, -1464, -1464, -1464, -1464,   570,
   -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464,   586,
   -1464,   215, -1464,   346,   611,   645, 14702, -1464,    10,   646,
   -1464,   -35, -1464, -1464,   623, -1464, -1464, -1464, -1464, -1464,
   -1464, -1464,   247, -1464,   577, -1464,   589,   613,   615, -1464,
   -1464, 13260, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464,
   -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464,
   -1464, -1464, -1464, -1464, -1464, -1464, -1464,   722,   766, -1464,
     597,   633, -1464,   458, -1464,   644, -1464,   622,    39,    39,
     599,    94, -1464, -1464, -1464,   156, -1464,  9914, -1464, -1464,
   -1464,   648,   651, -1464, -1464, -1464, -1464, -1464, -1464, -1464,
   -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464,
   -1464, -1464, -1464,   652,   618, -1464, -1464, -1464, -1464, -1464,
   -1464, -1464, -1464, -1464, -1464, -1464, -1464,   811, -1464, -1464,
   -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464,
     665,   627, -1464, -1464,   101,   653, -1464, -1464, -1464, -1464,
   -1464, -1464, -1464, -1464, -1464, -1464, -1464,   662,   655,   667,
   -1464,   -35,   139, -1464,    39,   639,   810,   490, -1464, -1464,
     660,   661,   670,   643,   673,   674, -1464, -1464, -1464,   658,
   -1464, -1464, -1464, -1464, -1464,   675, -1464, -1464, -1464, -1464,
   -1464, -1464, -1464, -1464, -1464, -1464, -1464,   678, -1464, -1464,
   -1464,   683,   684, -1464, -1464, -1464, -1464,   685,   687,   676,
     164, -1464, -1464, -1464, -1464, -1464,  4270,   668, -1464, -1464,
   -1464, -1464, -1464, -1464, -1464,   717,   751, -1464,   677, -1464,
      66, -1464,   -77,  9914, -1464,  2315,   169, -1464,   164, -1464,
   -1464, -1464,    94,   679, -1464,  9208,   719,   723,  9914, -1464,
      65, -1464, -1464, -1464,  9208, -1464, -1464,   724, -1464,   331,
     365,   387, -1464, -1464,  9208,   160, -1464, -1464, -1464,    -3,
   -1464, -1464, -1464,    29,  5568, -1464,   686,  9670,   399,  9769,
     434, -1464, -1464,  9208, -1464, -1464,   129,   -37, -1464,   668,
   -1464,   700,   707,  9208, -1464, -1464, -1464, -1464, -1464,   219,
     -74,   708,    24,  3338, -1464, -1464,   633,   298,  5770,   690,
    9208,   735,   712,   714,   702, -1464,   737,   727,   754,  5972,
      78,   304,   729, -1464,   305,   730,   731,  3540,  9208,  9208,
     -73,   -73,   -73,   706,   713,   716,   718,   720,   721, -1464,
    1962,  9571,  6176, -1464, -1464, -1464, -1464, -1464, -1464, -1464,
    6378,   742, -1464,  6582,   900, -1464,  9208,  9208,  9208,  9208,
    9208,  4962,  9208, -1464,   728, -1464, -1464,   757,   758,  9208,
     930, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464,
    -119, -1464,    60,   762, -1464,   765,   767,   768, -1464,   770,
   -1464, -1464,   881, -1464, -1464, -1464, -1464,   743, -1464, -1464,
     -43, -1464, -1464, -1464, -1464, -1464, -1464,  1714, -1464,   740,
   -1464, -1464, -1464, -1464, -1464, -1464,   314, -1464,   777, -1464,
   -1464,    23, -1464, -1464,   745,   769, -1464, 10463, -1464,   916,
     552, -1464, -1464, -1464,  3944,  9914,  9914,  9914, 10498,  9914,
    9914,   746,   793,  9914,   597,  9914,   597,  9914,   597, 10013,
     796, 10608, -1464,  9208, -1464, -1464, -1464, -1464,   755, -1464,
   -1464, 12783,  9208, -1464,  4270,   785, -1464,   788,   107, -1464,
     802,   668,   789,   787, -1464,   803,   805, 10643,   773,   944,
   -1464,   -42, -1464, -1464, -1464, 13295,   129, -1464,   775, -1464,
   -1464,   164,   330, -1464,   795,   797,   801, -1464,  9208,  3944,
   -1464,   812,   859, 10096,   983,  9914,  9208,  9208, 14036,  9208,
   13295,   813, -1464, -1464,  9208, -1464, -1464,   814,   838, 14036,
    9208, -1464, -1464,  9208, -1464, -1464,  9208, -1464,  9914,  3944,
   -1464, 10096,   214,   214,   790, -1464,   743, -1464, -1464, -1464,
    9208,  9208,  9208,  9208,  9208,  9208,   129,  2729,   129,  2932,
     129, 13394, -1464,   656, -1464, 13295, -1464,   650,   129, -1464,
     816,   826,   214,   214,   142,   214,   214,   129,   999,   831,
   14036,   831,   105, -1464, -1464, 13295, -1464, -1464, -1464, -1464,
    5164, -1464, -1464, -1464, -1464, -1464, -1464,   218,   854,   -73,
   -1464, 14545, 14576,  4146,  4146,  4146,  4146,  4146,  4146,  4146,
    4146,  9208,  9208, -1464, -1464,  9208,  4146,  4146,  9208,  9208,
    9208,   855,  4146,  9208,   453,  9208,  9208,  9208,  9208,  9208,
    9208,  4146,  4146,  9208,  9208,  9208,  4146,  4146,  4146,  9208,
    4146,  6784,  9208,  9208,  9208,  9208,  9208,  9208,  9208,  9208,
    9208,  9208,   179,  9208, -1464,  6986, -1464,  9208, -1464,   169,
   -1464,   334,  1018,   -35, -1464,   862, -1464,  3944, -1464, 10208,
      90,   569,   833,   380, -1464,   571,   576, -1464, -1464,   224,
     579,   653,   582,   653,   584,   653, -1464,   301, -1464,   351,
   -1464,  9914,   818,   812, -1464, -1464, 12818, -1464, -1464,  9914,
   -1464, -1464,  9914, -1464, -1464, -1464,  9208,   864, -1464,   865,
   -1464,  9914, -1464,  3944,  9914,  9914, -1464,    20,  9914,  5366,
    7188,   866,  9208,  9914, -1464, -1464, -1464,  9914,   831, -1464,
     812,  9208,  9208,  9208,  9208,  9208,  9208,  9208,  9208,  9208,
    9208,  9208,  9208,  9208,  9208,  9208,  9208,  9208,  9208,   633,
    1140,   816, 14036, 10753, -1464, -1464,  9914,  9914, 10788,  9914,
   -1464, -1464, 10898,  9914,   831,  9914,  9914,   831,  9914,  1094,
   -1464, 10096, -1464,   854, 10933, 11043, 11078, 11188, 11223, 11333,
      22,   -73,   823,   -21,  3135,  4350,  7390, 13493,   850,   -12,
     216,   851,   239,    43,  7592,   -12,   626,    45,  9208,   860,
    9208, -1464, -1464,  9914, -1464,  9914, -1464,  9208,   450,    52,
    9208,   861, -1464,    54,   129,  9208,   827,   828,   830,   832,
     391, -1464, -1464,   578,  9208,   743,  4554, -1464,   263,   850,
     834,   875,   875, -1464, -1464,   -23,   597, -1464,   852,   836,
   -1464, -1464,   853,   839, -1464, -1464,   -73,   -73,   -73, -1464,
   -1464, 10353, -1464, 10353, -1464, 10353, -1464, 10353, -1464, 10353,
   -1464, 10353, -1464, 10353, -1464, 10353,   782,   782,  9699, -1464,
   10353, -1464, 10353,  9699,   532,   532,   840, -1464, 10353,    58,
     841, -1464, 12917,     5,     5,   740, 14036,   782,   782, -1464,
   10353, -1464, 10353, 14376, 14251, 14286, -1464, 10353, -1464, 10353,
   -1464, 10353, 14126, -1464, 10353, 14671, 13528, 14404,   892,  1411,
    9699,  9699,   184,   184,    58,    58,    58,   473,  9208,   842,
     843,   474,  9208,  1048, 13016, -1464,   267, 13618,   871,   143,
     628,   991,   878, -1464, -1464, 10318, -1464, -1464, -1464, -1464,
    9914, -1464, -1464,   891, -1464, -1464,   868, -1464,   869, -1464,
     870, 10013, -1464,   796, -1464,   353,   668, -1464, -1464,   668,
     668, 11368, -1464,  1024,   -41, -1464, 10096,  1241,  1520,  9208,
     585,   476,   272,   847,   856,   902, 11478,   354, 11513,   590,
    9914,  9914,  9914,  1598,   858, 14036, 14036, 14036, 14036, 14036,
   14036, 14036, 14036, 14036, 14036, 14036, 14036, 14036, 14036, 14036,
   14036, 14036, 14036, -1464,  9914,   863,   867, -1464,  9208,  1746,
    1751, -1464,  1889, -1464,  1946,   879,  2649,  2851,   880,  3054,
     854,   597, -1464, -1464, -1464, -1464, -1464,   882,  9208, -1464,
    9208,  9208,  9208,  4758, 12783,     4,  9208,   477,   476,   216,
   -1464, -1464,   857, -1464,  9208,  9208, -1464,   884, -1464,  9208,
     476,   504,   888, -1464, -1464,  9208, 14036, -1464, -1464,   384,
     388, 13653,  9208, -1464, -1464,  2527,  9208,    55, -1464, -1464,
    9208,  9208,  9914,   597,   633, -1464, -1464,  9208, -1464,  2076,
     854,   147,  9410, -1464, -1464, -1464,   209,   279,   904,   905,
     906,   908, -1464,   273,   653, -1464,  9208, -1464,  9208, -1464,
   -1464, -1464,  7794,  9208, -1464,   885,   893, -1464, -1464,  9208,
     894, -1464, 13051,  9208,  7996,   895, -1464, 13150, -1464, -1464,
   -1464, -1464, -1464,   899, -1464, -1464,   229, -1464,     3, -1464,
   -1464, -1464, -1464, -1464,   668, -1464, -1464, -1464, -1464, -1464,
   -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464,  9914,
     896, -1464,   913,  9208, -1464, -1464,   -67, -1464,    87,   890,
   -1464, -1464, -1464,   392, -1464,   936,   897, -1464, -1464,  3257,
    3460,  3662, -1464, -1464,  3864,  9208, -1464, 14036, -1464, -1464,
   -1464, -1464, -1464, -1464, -1464, -1464, -1464,   901,   653,  8198,
     513, 11623, 14036, 14036,   -12,   216, 14036,   910,   157,   850,
   -1464, -1464, 14036,   851, -1464,   515,   -12,   903, -1464, -1464,
   -1464, -1464,   516, -1464, -1464, -1464,   528, -1464,   529,  9208,
   11658, 11768,  4066,   653, -1464, 13295, -1464,   931,   597,   911,
    4554,   948,   907,   463, -1464, -1464, -1464, -1464,   -23,   912,
     125,  9914, 11803,  9914, 11913, -1464,   283, 11948, -1464,  9208,
   14161,  9208, -1464, 12058, -1464,   286,  9208, -1464, -1464, -1464,
    1080,     3, -1464, -1464,   628, -1464, -1464, -1464,   668,  9208,
   -1464, 14036,   915,   918, -1464, -1464, -1464,   949,  9208,   934,
    9208, -1464, -1464, -1464, -1464,   919,   920,   816,  9208,  9208,
    9208,   921,  1056,   922,   923,  8400,   125, -1464,   287,  9208,
     221,   216, -1464,  9208,  9208,  9208,  9208,   504, -1464,  9208,
    9208,   924,  9208,  9208,   563, -1464, -1464, -1464,   943,   578,
    3742, -1464,   653, -1464,   276, -1464,   505,  9914,    65, -1464,
   -1464,  8602, -1464, -1464,  4143, -1464,   591, -1464, -1464, -1464,
    9914, 12093, 12203, -1464, -1464, 12238, -1464, -1464,  1080,   129,
     927,  1056,  1056,   950, 12348,   933, 12383,   938,   939,   940,
    9208,  9208,   929,  9699,  9699,  9699,  9208, -1464, -1464,  1056,
     476, -1464, 12493, -1464, -1464, 13743,  9208,  9208, -1464, 12528,
   14036, 14036, 13743, -1464,   963,  9699,  9208, -1464,   963, 13743,
    9208,   234, -1464, -1464,  8804,  9006, -1464, -1464, -1464, -1464,
   -1464, 14036,   633,   942,  9914,    65,   739,  9208,  9208, 14126,
   -1464, -1464,   592, -1464, -1464, -1464, 14702, -1464, -1464, -1464,
     -58,   -58,  9208, -1464,  9208, -1464,  1056,  1056,   476,   831,
     816, -1464,   831,   -58,   850,   951, -1464,  1104,   946, 14036,
   14036,   245,   984,   986,   953,   988,   958, 13743, -1464,   234,
    9208,  9208, 14036, -1464, -1464,   739,  9208,  9208, 13782, 14161,
   -1464, -1464, -1464,   982, 14702,   476,   850,   985,   957,   961,
   12638, 12673,   -58,   -58,   965,   966,   967,   972,   973, -1464,
    9208,   976,  9208,  9208,   978,   997, -1464, -1464, -1464,   979,
   -1464, 14036,  9208, 13872, 13907, -1464, -1464, -1464,   633,   292,
     989, -1464, -1464, -1464, -1464, -1464,   990,   993, -1464, -1464,
   -1464, -1464, -1464, 14036, -1464, 14036, 14036, -1464, -1464, -1464,
   13997, -1464, -1464, -1464, -1464,   476, -1464, -1464, -1464,   296,
   -1464
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   120,     1,   297,     0,     0,     0,   605,   298,     0,
     597,   597,   597,    16,     0,     0,    15,     3,     0,    10,
       9,     8,     0,     7,   585,     6,    11,     5,     4,    13,
      12,    14,    92,    93,    91,   100,   102,    37,    53,    50,
      51,    39,    40,    41,     0,    42,    48,    38,   597,   597,
      22,    21,   585,   599,   598,   766,   756,   761,     0,   265,
      35,   107,   108,     0,     0,     0,   109,   111,   118,     0,
     106,    17,   623,   622,   213,   607,   624,   586,   587,     0,
       0,     0,     0,    43,     0,    49,     0,     0,    46,     0,
       0,   597,     0,    18,     0,     0,     0,   267,     0,     0,
     117,   112,     0,     0,     0,     0,     0,     0,   121,   215,
     214,   217,   212,   609,   608,     0,   626,   625,   629,   589,
     588,   591,    98,    99,    96,    97,    95,     0,     0,    94,
     103,    54,    52,    48,    45,    44,   600,   602,   604,     0,
     606,    19,    20,    23,   767,   757,   762,   266,    33,    36,
     116,     0,   113,   114,   115,   119,     0,   610,     0,   619,
     582,   520,    24,    25,     0,    87,    88,    85,    86,    84,
      83,    89,     0,    47,     0,   603,     0,     0,     0,    34,
     110,     0,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,     0,     0,   127,
     122,     0,   611,     0,   620,     0,   630,   583,     0,     0,
     522,     0,    26,    27,    28,     0,   101,     0,   768,   758,
     763,   181,   182,   179,   130,   131,   133,   132,   134,   135,
     136,   137,   163,   164,   161,   162,   154,   165,   166,   155,
     152,   153,   180,   174,     0,   178,   167,   168,   169,   170,
     141,   142,   143,   138,   139,   140,   151,     0,   157,   158,
     156,   149,   150,   145,   144,   146,   147,   148,   129,   128,
     173,     0,   159,   160,   520,   125,   244,   218,   593,   664,
     667,   670,   671,   665,   668,   666,   669,     0,   617,   627,
     590,   520,     0,   104,     0,     0,   572,   570,   592,    90,
       0,     0,     0,     0,     0,     0,   637,   657,   638,   673,
     639,   643,   644,   645,   646,   663,   650,   651,   652,   653,
     654,   655,   656,   658,   659,   660,   661,   726,   642,   649,
     662,   733,   740,   640,   647,   641,   648,     0,     0,     0,
       0,   672,   688,   691,   689,   690,   753,   601,   678,   550,
     556,   183,   184,   177,   172,   185,   175,   171,     0,   123,
     296,   544,     0,     0,   216,     0,   612,   614,     0,   621,
     534,   631,     0,     0,   105,     0,     0,     0,     0,   571,
       0,   694,   717,   720,     0,   723,   713,     0,   681,   727,
     734,   741,   747,   750,     0,     0,   703,   708,   702,     0,
     716,   712,   705,     0,     0,   707,   692,     0,   769,   759,
     764,   186,   176,     0,   294,   295,     0,   520,   124,   126,
     246,     0,     0,     0,    63,    64,    76,   426,   427,     0,
       0,     0,     0,   282,   420,   280,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   281,     0,     0,     0,     0,
       0,     0,     0,   663,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   509,
       0,     0,     0,   348,   350,   349,   351,   352,   353,   354,
       0,     0,    29,     0,   219,   223,     0,     0,     0,     0,
       0,     0,     0,   332,   333,   424,   423,     0,     0,     0,
       0,   239,   234,   231,   230,   232,   233,   264,   245,   225,
     503,   224,   421,     0,   494,    71,    72,    69,   237,    70,
     238,   240,   300,   229,   493,   492,   491,   120,   497,   422,
       0,   226,   496,   495,   467,   428,   468,   355,   429,     0,
     425,   771,   775,   772,   773,   774,     0,   594,     0,   593,
     618,   535,   584,   521,     0,     0,   503,     0,   574,   575,
       0,   568,   569,   567,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   122,     0,   122,     0,   122,     0,
       0,     0,   699,   248,   710,   711,   704,   706,     0,   709,
     693,     0,     0,   755,   754,     0,   679,     0,   265,   551,
       0,   546,     0,     0,   557,     0,     0,     0,     0,   632,
     542,   561,   545,   814,   817,     0,     0,   270,   274,   273,
     279,     0,     0,   318,     0,     0,     0,   850,     0,     0,
     286,   283,     0,   327,     0,     0,     0,     0,   268,     0,
       0,     0,   309,   312,     0,   243,   315,     0,     0,    57,
       0,    78,   854,     0,   823,   832,     0,   820,     0,     0,
     291,   288,   456,   457,   333,   343,   120,   263,   261,   262,
       0,     0,     0,     0,     0,     0,     0,   792,     0,     0,
       0,   830,   857,     0,   254,     0,   257,     0,     0,   859,
     868,     0,   433,   432,   469,   431,   430,     0,     0,   868,
     327,   868,   334,   241,   242,     0,    74,   346,   222,   501,
       0,   228,   235,   236,   285,   290,   299,     0,   341,     0,
     227,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   458,   459,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   412,     0,   221,     0,   595,     0,   613,   615,
     628,     0,     0,   520,   573,     0,   577,     0,   581,   355,
       0,     0,     0,   684,   697,     0,     0,   674,   676,     0,
       0,   125,     0,   125,     0,   125,   548,     0,   554,     0,
     675,     0,     0,   249,   701,   686,     0,   680,   770,     0,
     552,   760,     0,   558,   765,   543,     0,     0,   560,     0,
     559,     0,   562,     0,     0,     0,    79,     0,     0,   806,
       0,     0,     0,     0,   840,   843,   846,     0,   868,   287,
     284,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   868,   269,     0,    80,    81,     0,     0,     0,     0,
      55,    56,     0,     0,   868,     0,     0,   868,     0,     0,
     292,   289,   334,   341,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   265,     0,     0,     0,   826,   786,   792,
       0,   835,     0,     0,     0,   792,     0,     0,     0,     0,
       0,   795,   862,     0,   247,     0,    32,     0,    30,     0,
     869,     0,   244,     0,     0,   869,     0,     0,     0,     0,
     402,   399,   401,    60,     0,   120,     0,   415,     0,   785,
       0,     0,     0,   308,   307,     0,   122,   260,     0,     0,
     480,   479,     0,     0,   481,   485,     0,     0,     0,   377,
     386,   365,   387,   366,   389,   368,   388,   367,   390,   369,
     380,   359,   381,   360,   382,   361,   434,   435,   447,   391,
     370,   392,   371,   448,   445,   446,     0,   379,   357,   474,
       0,   465,     0,   498,   499,   500,   358,   436,   437,   393,
     372,   394,   373,   452,   453,   454,   383,   362,   384,   363,
     385,   364,   455,   378,   356,     0,     0,   450,   451,   449,
     443,   444,   439,   438,   440,   441,   442,     0,     0,     0,
     408,     0,     0,     0,     0,   418,     0,     0,     0,     0,
     528,   531,     0,   576,   579,   355,   580,   695,   718,   721,
       0,   724,   714,     0,   682,   728,     0,   735,     0,   742,
       0,     0,   748,     0,   751,     0,   252,   698,   687,   547,
     553,     0,   634,   635,   563,   566,   565,     0,     0,     0,
       0,   807,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   328,   365,   366,   368,   367,
     369,   359,   360,   361,   370,   371,   357,   372,   373,   362,
     363,   364,   356,   293,     0,     0,     0,   470,     0,     0,
       0,   471,     0,   502,     0,     0,     0,     0,     0,     0,
     341,   122,   504,   505,   506,   507,   508,     0,     0,   793,
       0,     0,     0,     0,   327,   792,     0,     0,     0,     0,
     801,   802,     0,   809,     0,     0,   799,     0,   838,     0,
       0,     0,     0,   797,   839,     0,   829,   794,   858,     0,
       0,     0,     0,   860,   861,     0,     0,     0,   837,   460,
       0,     0,     0,   122,     0,    58,    59,     0,    73,    65,
     341,     0,     0,   417,   416,   301,     0,     0,     0,     0,
       0,     0,   339,     0,   125,   476,     0,   482,     0,   376,
     374,   375,     0,     0,   463,     0,     0,   486,   490,     0,
       0,   466,     0,     0,     0,     0,   409,     0,   413,   461,
     419,   596,   616,   121,   529,   530,   531,   532,   523,   536,
     578,   696,   719,   722,   685,   725,   715,   677,   683,   729,
     731,   736,   738,   743,   745,   549,   749,   555,   752,     0,
       0,   633,     0,     0,   815,   818,     0,   271,     0,     0,
     276,   277,   275,     0,   321,     0,     0,   324,   319,     0,
       0,     0,   851,   849,     0,     0,   863,    82,   310,   313,
     316,   855,   853,   824,   833,   831,   821,     0,   125,     0,
       0,     0,   777,   776,   792,     0,   827,     0,     0,   787,
     808,   800,   828,   836,   798,     0,   792,     0,   804,   805,
     812,   796,     0,   255,   258,    31,     0,   220,     0,     0,
       0,     0,     0,   125,    61,     0,    66,     0,   122,     0,
       0,     0,     0,   570,   337,   338,   336,   335,     0,     0,
       0,     0,     0,     0,     0,   397,     0,     0,   487,     0,
     475,     0,   464,     0,   410,     0,     0,   462,   414,   541,
     526,   523,   524,   525,   528,   732,   739,   746,   253,   248,
     636,   564,     0,     0,    77,   272,   278,     0,     0,     0,
       0,   320,   841,   844,   847,     0,     0,   868,     0,     0,
       0,     0,   806,     0,     0,     0,     0,   510,     0,     0,
       0,     0,   810,     0,     0,     0,     0,     0,   803,     0,
       0,   250,     0,     0,     0,   400,   519,   403,     0,    60,
       0,    75,   125,   395,     0,   302,   570,     0,     0,   340,
     342,     0,   329,   345,     0,   518,     0,   516,   398,   513,
       0,     0,     0,   512,   411,     0,   527,   537,   526,     0,
       0,   806,   806,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   311,   314,   317,     0,   807,   825,   806,
       0,   472,     0,   344,   511,   866,     0,     0,   811,     0,
     779,   778,   866,   813,   866,   256,   248,   259,   866,   866,
       0,     0,   406,    62,   282,     0,    67,    71,    72,    69,
      70,    68,     0,     0,     0,     0,     0,     0,     0,   330,
     477,   483,     0,   517,   515,   514,     0,   539,   533,   700,
       0,     0,     0,   322,     0,   325,   806,   806,     0,   868,
     868,   864,   868,     0,   784,     0,   473,     0,     0,   781,
     780,     0,     0,     0,     0,     0,     0,   866,   404,     0,
       0,     0,   288,   347,   396,     0,     0,     0,     0,   331,
     478,   484,   488,     0,     0,     0,   790,   868,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   822,
       0,     0,     0,     0,     0,     0,   251,   874,   870,     0,
     407,   289,     0,     0,     0,   306,   489,   538,     0,     0,
     869,   791,   816,   819,   323,   326,     0,     0,   848,   852,
     865,   856,   834,   867,   872,   783,   782,   873,   875,   871,
       0,   305,   304,   540,   788,     0,   842,   845,   303,     0,
     789
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1464, -1464, -1464, -1464, -1464, -1464,   498,  1112, -1464, -1464,
   -1464,  1195, -1464, -1464, -1464,   636,  1153, -1464,  1065, -1464,
   -1464,  1120, -1464, -1464, -1464,  -245, -1464, -1464, -1464,  -242,
   -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464,   987,
   -1464, -1464,   -54,   -51, -1464, -1464, -1464,   350,   418,  -515,
    -568,  -779, -1464, -1464, -1464, -1285, -1464, -1464,  -209,  -354,
   -1464,   268, -1464, -1340, -1464,  -327,   508, -1464, -1464, -1464,
   -1464,  -412,   -14, -1464, -1464, -1464, -1464, -1464,  -239,  -237,
    -236, -1464,  -231, -1464, -1464, -1464,  1225, -1464,   265, -1464,
   -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464,
   -1464, -1464,  -375,  -198,   711,  -139, -1464,  -884,  -448, -1464,
     715, -1464, -1464,  -369,  -142, -1464, -1464, -1464, -1436, -1464,
   -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464,   725, -1464,
   -1464, -1464, -1464, -1464, -1464, -1464,  -153,  -158,  -241,  -156,
     -20, -1464, -1464, -1464, -1464, -1464,   822, -1464,  -574, -1464,
   -1464,  -580, -1464, -1464,  -617,  -240,  -563, -1325, -1464,  -370,
   -1464, -1464,  1188, -1464, -1464, -1464,   691,   837,   162, -1464,
   -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464,
   -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464,  -589,
    -141, -1464,   820, -1464, -1464, -1464, -1464, -1464, -1464, -1464,
   -1464,  -374, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464,
   -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464,
   -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464,  -214,
   -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464,
   -1464, -1464, -1464, -1464, -1464,   824,  -654,  -307,  -824,  -672,
   -1464, -1463,  -872, -1464, -1464, -1464, -1142,  -185, -1347, -1464,
   -1464, -1464, -1464, -1464, -1464, -1464, -1464, -1464,    82,  -479,
   -1464, -1464, -1464,   564, -1464, -1464, -1464, -1464, -1464, -1464,
   -1464, -1464, -1464, -1464, -1464, -1464,  -641, -1464, -1449,  -700,
   -1464
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    16,   143,    52,    17,   164,   170,   697,   505,
     149,   506,    99,    19,    20,    45,    46,    47,    88,    21,
      39,    40,   507,   508,  1207,  1208,   509,  1357,  1450,   510,
     511,   954,   512,   626,   513,   514,   515,   516,  1138,   171,
     172,    35,    36,    37,   220,    66,    67,    68,    69,    22,
     285,   374,   210,    23,   111,   211,   112,   156,   675,   979,
     518,   375,   519,   822,  1517,  1085,   566,   933,  1440,   935,
    1441,   521,   522,   523,   628,   848,  1405,   524,   525,   526,
     527,   528,   529,   530,   531,   426,   532,   727,  1216,   963,
     533,   534,   886,  1418,   887,  1419,   889,  1420,   535,   853,
    1411,   536,   709,  1463,   537,  1222,  1223,   966,   677,   538,
     784,   955,   539,   642,   980,   541,   542,   543,   952,   544,
    1202,  1521,  1203,  1579,   545,  1053,  1388,   546,   710,  1371,
    1590,  1373,  1591,  1470,  1626,   548,   370,  1394,  1477,  1256,
    1258,  1062,   561,   793,  1546,  1594,   371,   372,   610,   817,
     419,   615,   819,   420,  1162,   620,   574,   390,   307,   308,
     217,   301,    78,   121,    25,   161,   376,    89,    90,   174,
      91,    26,    49,   115,   158,    27,   288,   558,   559,  1058,
     379,   215,   216,    76,   118,   381,    28,   159,   299,   621,
     549,   297,   353,   354,   809,   418,   355,   582,  1268,   802,
     416,   356,   575,  1261,   821,   580,  1266,   576,  1262,   577,
    1263,   579,  1265,   583,  1269,   584,  1395,   585,  1271,   586,
    1396,   587,  1273,   588,  1397,   589,  1276,   590,  1278,   611,
      29,    95,   177,   359,   612,    30,    96,   178,   360,   616,
      31,    94,   176,   358,   607,   550,  1596,  1565,   960,   919,
    1597,  1598,   920,   932,  1184,  1178,  1173,  1340,  1104,   551,
     844,  1402,   845,  1403,   898,  1424,   895,  1422,   921,   699,
     552,   896,  1423,   922,   553,  1110,  1487,  1111,  1488,  1112,
    1489,   857,  1415,   893,  1421,   693,   700,   554,  1568,   941,
     555
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,    70,   287,   692,   843,   881,   540,   797,   221,   946,
     818,   947,   562,   357,   570,   816,   811,   925,   813,  1151,
     815,   517,   728,   678,   679,  1103,   130,  1330,   634,   594,
    -120,   640,  1076,   918,  1078,   918,  1080,   847,  1458,  1099,
     911,  1158,    84,   604,   122,   123,   792,  1169,  1392,    70,
      70,    70,   151,  1181,    53,   670,   911,    32,    33,  1480,
      54,   596,  1179,  1572,  1185,  1573,   959,    58,   641,  1575,
    1576,  1192,   424,  1196,  1349,  1498,    50,    85,   731,   732,
     104,   105,   106,   838,   838,  1578,   352,   729,  1599,   517,
      70,    70,    70,    70,    59,   503,   674,   910,   912,   923,
    1608,   927,  1218,   427,   838,   286,   687,   689,  1393,   939,
    1288,   286,  1219,   517,  1161,   425,   368,    38,   943,   861,
      98,   218,    51,   218,     2,   635,   636,   711,  1619,   751,
       3,  1535,   754,   755,  1550,  1551,   428,   730,   631,  1636,
    1637,   503,   674,  1620,   839,   840,   840,   841,   382,   417,
     842,   150,  1563,     4,  1595,     5,    48,     6,  1114,   429,
     912,   903,   798,     7,  1220,   303,   840,   302,  1168,  1221,
     165,   166,  1288,     8,   743,   744,  1574,   138,   219,     9,
     219,  1136,   751,  1432,   753,   754,   755,   756,   212,   595,
     571,   124,   757,    71,  1145,   959,   125,  1148,   126,   213,
     572,   127,   790,    10,   731,   732,   839,    34,   839,  1602,
    1603,    92,   838,   351,   782,   783,    77,   637,   823,    86,
     517,   597,  1161,   431,   432,    11,    12,   859,   719,   839,
      87,   839,   352,   791,   731,   732,   638,   644,   839,   598,
     839,   839,   128,   442,   305,   599,    58,   352,    98,   447,
     384,   661,   573,   139,   368,   948,   286,   900,   107,   218,
     949,  1593,   306,   858,   860,  1461,  1317,   782,   783,   754,
     755,   880,   720,    59,   840,  1390,   352,  1289,   352,   618,
     417,   967,   838,   108,  1067,   517,   461,   462,   894,  1508,
     660,   897,  1434,  1325,   899,  1257,   829,   619,   950,    79,
     743,   744,  1211,   286,    13,    58,  1047,  1048,   751,  1628,
     753,   754,   755,   756,   369,   517,   219,   167,   757,    80,
     464,   465,   168,    14,   169,    80,  1358,   127,    98,   107,
     743,   744,    59,  1167,    15,  1049,   405,   556,   751,   352,
     352,   754,   755,   756,   840,   958,   838,  1050,   757,   557,
     383,   782,   783,   592,  1253,  1197,  1506,   964,  1359,   351,
      58,   800,   801,   803,   560,   805,   806,    61,   305,   810,
     838,   812,   593,   814,   351,    58,   961,   480,   481,   482,
    1612,   779,   780,   781,   948,  1064,   306,    59,  1051,  1052,
      41,    42,    43,   782,   783,  1361,    62,   493,  1224,  1170,
    1171,    80,    59,   351,  1073,   351,    72,    73,   840,    74,
    1056,    53,   627,   100,   101,   102,   846,    54,  1074,  1175,
    1362,    44,  1176,   782,   783,   351,   632,  1172,   180,   501,
     962,  1095,   840,   225,   352,   352,   352,    75,   352,   352,
    1210,   884,   352,   517,   352,  1370,   352,   959,   352,  1212,
    1177,  1188,  1431,   861,   152,   153,   154,   155,   861,  1368,
     226,  1193,  1167,  1159,  1437,  1361,   351,   351,    63,   861,
    1097,  1098,   861,   861,  1102,    58,  1213,  1283,  1167,    81,
    1250,  1081,  1167,  1113,  1369,  1290,   934,  1533,   104,   517,
     106,   645,  1363,  1324,   786,  1082,  1468,   662,   665,  1474,
    1504,   787,    59,  1277,   352,  1654,   953,  1275,  1336,  1660,
     646,   918,  1139,  1140,  1329,  1142,   663,   666,    82,  1144,
    -730,  1146,  1147,   851,  1149,  -730,   918,   352,  1229,  1230,
    1231,  1083,   289,  1279,  1295,    97,   290,    64,   109,  1426,
     103,   711,   852,  -730,   110,  1084,    65,  1280,  1296,   711,
     291,   292,   731,   732,  -737,   293,   294,   295,   296,  -737,
    1070,   351,   351,   351,  1279,   351,   351,   605,  1279,   351,
     417,   351,  1407,   351,  1448,   351,  -744,  -737,  1343,   606,
    -405,  -744,  1344,  1318,   406,  -405,  1408,   162,   163,  1204,
     971,   975,   982,   984,   986,   988,   990,   992,   994,  -744,
    1205,  1206,   613,  -405,   999,  1001,  1189,  1086,  1190,  1010,
    1007,   407,   408,   131,   614,  1089,  1015,   850,  1090,  1019,
    1021,  1011,   222,   223,  1026,  1028,  1030,  1094,  1033,  1240,
    1245,   351,   912,  1327,  1100,  1353,    87,   113,   133,  1109,
    1061,  1241,  1246,   114,  1288,  1328,   741,   742,   743,   744,
     745,   116,  1457,   748,   351,   389,   751,   117,   753,   754,
     755,   756,   136,   137,  1417,   119,   757,   140,   759,   760,
    1133,   120,   148,  1532,   409,   141,   387,   571,   410,   388,
     352,   142,   389,  1337,  1254,   144,  1338,   572,   352,  1339,
    1255,   352,   145,  1429,  1534,  1436,  1439,   389,   951,   861,
     352,   861,   861,   352,   352,   146,  1454,   352,  1442,  1443,
     147,  1435,   352,   157,   861,   861,   352,  1492,   160,  1086,
      85,  1086,   134,   135,  1286,   775,   776,   777,   778,   779,
     780,   781,   796,    41,    42,    43,  1299,  1300,  1301,   573,
     411,   782,   783,  1520,   412,   352,   352,   413,   352,   861,
     175,   214,   352,   104,   352,   352,   282,   352,   179,   417,
    1304,   417,   414,  1068,   227,  1071,   417,   228,   415,   417,
    1072,   406,   417,  1075,   417,   417,  1077,    70,  1079,  1287,
     417,   417,   417,  1320,  1298,  1541,  1592,   104,   105,   106,
    1452,   229,   352,   230,   352,   222,   223,   224,   407,   408,
     283,   300,   731,   732,  1335,  1182,  1175,   351,  1183,   284,
    1342,   286,   298,  1507,   304,   351,   361,  1346,   351,   362,
     363,  1348,   162,   163,   936,   937,   540,   351,  1352,   364,
     351,   351,   365,   366,   351,   929,   930,   931,   367,   351,
     377,   517,   373,   351,   378,   380,  1564,    55,    56,    57,
    1560,   385,   386,   391,   392,   394,  1264,  1376,   417,  1605,
    1606,   409,  1607,   393,   571,   410,   395,   396,   398,  1385,
     397,   399,   351,   351,   572,   351,   400,   401,   402,   351,
     403,   351,   351,   520,   351,   421,   422,   568,   404,   423,
     563,   569,   581,   623,  1238,  1537,   602,  1631,   743,   744,
     624,   633,   649,   651,  1564,   652,   751,   653,   753,   754,
     755,   756,   731,   732,   654,   657,   757,   655,   680,   351,
     656,   351,   664,   667,   668,   681,   573,   411,   682,   352,
     683,   412,   684,   685,   413,   696,   701,   713,   714,   716,
     352,  1629,   721,   712,  1428,   722,   726,   723,   724,   414,
     725,   520,   785,    15,  1613,   415,   788,   660,   795,   807,
    1514,   808,   694,  1518,   613,   827,   824,   828,   831,   352,
     352,   352,  1586,  1587,  1444,   520,   832,   777,   778,   779,
     780,   781,   830,   833,   834,   836,   837,   849,   854,   729,
     855,   782,   783,   352,   856,  1354,   879,  1464,   861,   891,
     885,  1659,   940,   890,   942,   902,   741,   742,   743,   744,
     745,   944,   965,   748,   749,   750,   751,   945,   753,   754,
     755,   756,  1622,  1006,   823,  1060,   757,  1069,   759,   760,
    1063,  1087,  1092,  1093,  1107,  1160,  1167,  1174,  1198,  1187,
    1194,  1199,  1200,  1215,  1201,  1225,  1227,  1214,  1226,  1248,
    1252,  1228,  1232,  1233,  1243,  1244,   351,  1257,  1259,  1267,
    1291,   352,  1270,  1272,  1274,  1398,  1282,   351,  1331,  1292,
    1293,  1303,  1364,  1365,  1366,  1305,  1367,  1404,  1378,  1389,
    1306,  1400,   520,  1536,   774,   775,   776,   777,   778,   779,
     780,   781,  1312,  1315,  1319,  1334,   351,   351,   351,  1341,
     547,   782,   783,  1406,  1409,  1379,  1381,  1386,  1399,  1410,
     567,  1451,  1476,  1425,  1438,  1559,  1455,  1483,  1456,   578,
     351,  1562,  1433,  1460,  1453,  1497,   406,  1481,  1485,   591,
    1482,  1490,  1491,  1496,  1499,  1500,  1516,  1522,   352,   601,
    1549,   823,  1561,  1567,  1552,  1554,  1449,   520,   617,   641,
    1556,  1557,  1558,   407,   408,  1584,  1610,  1611,   625,  1466,
    1585,  1462,  1627,  1614,  1609,  1615,  1616,  1617,   643,  1618,
    1632,  1630,   406,   648,  1633,   650,  1648,   520,  1638,  1639,
    1640,   676,   676,   676,   659,  1641,  1642,  1644,   351,  1647,
    1649,   129,   671,   672,   673,   938,    18,    83,   173,   407,
     408,  1655,   132,  1656,  1523,   860,  1657,   691,  1526,  1059,
    1195,  1527,   309,  1528,  1529,   695,   409,  1462,   691,  1530,
     410,   702,   703,   704,   705,   706,    24,  1217,  1503,  1459,
     352,   717,   352,  1478,   715,   718,  1391,  1547,  1479,  1548,
      93,   520,   520,   520,   520,   520,   520,   520,   520,   622,
     789,  1604,  1513,   926,   520,   520,  1542,  1333,   717,   629,
     520,     0,   409,   630,     0,   351,   410,     0,     0,   520,
     520,     0,     0,   406,   520,   520,   520,     0,   520,     0,
       0,     0,   411,     0,     0,     0,   412,     0,  1150,   413,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   799,
     407,   408,     0,     0,   414,   520,     0,     0,     0,     0,
     415,     0,     0,     0,     0,     0,   352,     0,     0,     0,
    1134,     0,     0,  1583,     0,     0,     0,   826,   411,   352,
       0,     0,   412,     0,  1135,   413,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     414,   520,     0,     0,   717,     0,   415,   351,     0,   351,
       0,     0,     0,   409,   643,     0,     0,   410,     0,     0,
       0,   691,   882,     0,   883,     0,     0,     0,     0,   888,
       0,     0,   717,     0,     0,   892,     0,     0,     0,     0,
       0,     0,     0,   352,   901,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   904,   905,   906,   907,   908,
     909,     0,   917,     0,   917,     0,     0,     0,     0,  1653,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   411,
       0,   731,   732,   412,     0,  1284,   413,     0,     0,     0,
     676,     0,     0,   351,     0,     0,     0,     0,     0,     0,
       0,   414,     0,     0,     0,     0,   351,   415,   981,   983,
     985,   987,   989,   991,   993,   995,   996,   997,     0,     0,
     998,  1000,  1002,  1003,  1004,  1005,     0,  1008,  1009,     0,
    1012,  1013,  1014,  1016,  1017,  1018,  1020,  1022,  1023,  1024,
    1025,  1027,  1029,  1031,  1032,  1034,  1036,  1037,  1038,  1039,
    1040,  1041,  1042,  1043,  1044,  1045,  1046,   951,  1054,     0,
     717,     0,  1057,     0,     0,     0,     0,     0,     0,     0,
     351,     0,  1065,     0,     0,   741,   742,   743,   744,   745,
       0,     0,   748,   749,   750,   751,     0,   753,   754,   755,
     756,     0,     0,     0,     0,   757,     0,   759,   760,     0,
       0,     0,   406,     0,     0,     0,     0,     0,     0,     0,
       0,  1091,     0,     0,     0,   951,     0,     0,  1096,     0,
       0,     0,     0,     0,     0,  1106,     0,  1108,     0,   407,
     408,     0,     0,     0,     0,     0,  1115,  1116,  1117,  1118,
    1119,  1120,  1121,  1122,  1123,  1124,  1125,  1126,  1127,  1128,
    1129,  1130,  1131,  1132,   775,   776,   777,   778,   779,   780,
     781,     0,   717,     0,     0,     0,     0,     0,     0,     0,
     782,   783,   676,     0,     0,     0,     0,     0,     0,     0,
     406,     0,     0,     0,     0,     0,     0,     0,     0,   702,
    1164,     0,   409,     0,     0,     0,   410,     0,     0,     0,
       0,     0,     0,  1186,     0,   691,     0,   407,   408,     0,
       0,     0,  1191,     0,     0,   691,     0,     0,     0,     0,
    1115,     0,     0,     0,     0,     0,     0,     0,     0,  1209,
       0,     0,     0,     0,     0,     0,     0,   676,   676,   676,
       0,     0,   717,     0,   717,     0,   717,     0,   717,     0,
     717,     0,   717,   520,   717,     0,   717,     0,   411,     0,
       0,   717,   412,   717,  1285,   413,     0,     0,     0,   717,
     409,     0,     0,   -68,   410,     0,     0,     0,     0,     0,
     414,   717,     0,   717,   731,   732,   415,     0,   717,     0,
     717,     0,   717,     0,     0,   717,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1242,     0,     0,   717,  1247,   406,     0,
       0,     0,     0,   406,     0,     0,   411,     0,     0,     0,
     412,     0,  1302,   413,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   407,   408,   717,   414,     0,
     407,   408,     0,     0,   415,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,   750,   751,   752,
     753,   754,   755,   756,     0,     0,     0,     0,   757,   758,
     759,   760,   761,   762,     0,     0,   763,   764,   765,   766,
     767,   768,   769,  1307,     0,     0,     0,     0,   409,     0,
       0,     0,   410,   409,     0,     0,     0,   410,     0,     0,
       0,     0,     0,     0,     0,  1321,  1322,  1323,     0,     0,
       0,  1326,     0,     0,     0,     0,     0,     0,     0,  1332,
     917,   770,   771,     0,   772,   773,   774,   775,   776,   777,
     778,   779,   780,   781,     0,     0,     0,     0,     0,     0,
     547,   406,     0,   782,   783,  1350,  1351,     0,   503,   674,
       0,     0,  1355,     0,   411,     0,     0,  1115,   412,   411,
    1308,   413,     0,   412,     0,  1309,   413,     0,   407,   408,
       0,  1372,     0,  1374,     0,     0,   414,     0,  1377,     0,
       0,   414,   415,     0,  1380,     0,     0,   415,  1383,     0,
       0,     0,     0,     0,   686,     0,     0,     0,   406,     0,
     310,     0,     0,     0,     0,     0,   311,     0,     0,     0,
       0,     0,   312,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   313,     0,     0,   407,   408,     0,  1401,     0,
     314,   409,     0,     0,     0,   410,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,     0,
     691,     0,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,     0,     0,     0,   409,     0,
       0,     0,   410,     0,     0,     0,     0,   411,     0,     0,
       0,   412,     0,  1310,   413,     0,     0,  1356,     0,     0,
       0,     0,     0,     0,     0,     0,   731,   732,     0,   414,
       0,     0,     0,    58,  1471,   415,  1472,     0,     0,     0,
       0,  1475,     0,     0,     0,     0,   349,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      59,     0,     0,  1484,   411,  1486,     0,     0,   412,     0,
    1311,   413,     0,  1493,  1494,  1495,     0,     0,     0,     0,
    1502,     0,     0,     0,  1505,     0,   414,     0,  1509,  1510,
    1511,  1512,   415,     0,   691,  1515,     0,   691,  1519,     0,
       0,     0,     0,     0,     0,  1531,   350,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1539,     0,     0,     0,
     741,   742,   743,   744,   745,     0,     0,   748,   749,   750,
     751,     0,   753,   754,   755,   756,     0,     0,     0,     0,
     757,     0,   759,   760,     0,     0,   691,     0,   763,   764,
     765,     0,     0,     0,   769,     0,     0,     0,     0,     0,
       0,  1569,  1570,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1577,     0,     0,     0,     0,
    1582,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1588,  1589,   771,     0,   772,   773,   774,   775,
     776,   777,   778,   779,   780,   781,     0,  1600,     0,  1601,
       0,     0,     0,     0,     0,   782,   783,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1621,     0,     0,     0,
       0,  1623,  1624,     0,     0,     0,   430,     0,     0,   431,
     432,     3,     0,   433,   434,   435,     0,   436,     0,   437,
     438,   439,   440,   441,     0,  1643,     0,  1645,  1646,   442,
     443,   444,   445,   446,     0,   447,     0,  1650,     0,     0,
       0,     0,   448,   449,     0,     0,   450,     0,   451,   452,
       0,     0,   453,     0,     8,   454,   455,     0,   456,   457,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   460,
       0,     0,   461,   462,     0,   316,   317,   318,     0,   320,
     321,   322,   323,   324,   463,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,     0,   338,   339,   340,
       0,     0,   343,   344,   345,   346,   464,   465,   466,   467,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   468,   469,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   470,   471,   472,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   473,   474,   475,   476,   477,     0,
     478,     0,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,    59,   490,   491,   492,     0,     0,     0,
       0,     0,     0,   493,   494,   495,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   498,     0,    14,     0,     0,   499,   500,     0,
       0,     0,     0,     0,     0,   501,     0,   502,   430,   503,
     504,   431,   432,     3,     0,   433,   434,   435,     0,   436,
       0,   437,   438,   439,   440,   441,     0,     0,     0,     0,
       0,   442,   443,   444,   445,   446,     0,   447,     0,     0,
       0,     0,     0,     0,   448,   449,     0,     0,   450,     0,
     451,   452,     0,     0,   453,     0,     8,   454,   455,     0,
     456,   457,     0,     0,   458,   459,     0,     0,     0,     0,
       0,   460,     0,     0,   461,   462,     0,   316,   317,   318,
       0,   320,   321,   322,   323,   324,   463,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,     0,   338,
     339,   340,     0,     0,   343,   344,   345,   346,   464,   465,
     466,   467,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   470,   471,
     472,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   473,   474,   475,   476,
     477,   406,   478,     0,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,    59,   490,   491,   492,     0,
       0,     0,     0,     0,     0,   493,  1347,   495,   407,   408,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   496,   497,   498,     0,    14,     0,     0,   499,
     500,     0,     0,   431,   432,     0,     0,   501,     0,   502,
       0,   503,   504,   437,   438,   439,   440,   441,     0,     0,
       0,     0,     0,   442,     0,   444,     0,     0,     0,   447,
       0,   406,     0,     0,     0,     0,     0,   449,     0,     0,
       0,   409,     0,   452,     0,   410,   453,     0,     0,   454,
       0,   911,     0,   457,     0,     0,     0,     0,   407,   408,
       0,     0,     0,   564,     0,     0,   461,   462,     0,   316,
     317,   318,     0,   320,   321,   322,   323,   324,   463,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
       0,   338,   339,   340,     0,     0,   343,   344,   345,   346,
     464,   465,   466,     0,     0,     0,     0,   411,     0,     0,
       0,   412,     0,  1313,   413,   468,   469,     0,     0,     0,
       0,   409,     0,     0,     0,   410,     0,     0,     0,   414,
       0,     0,     0,     0,     0,   415,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   473,   474,
     475,   476,   477,   406,   478,   912,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   913,   565,   491,
     492,     0,     0,     0,     0,     0,     0,   493,     0,     0,
     407,   408,     0,     0,     0,     0,     0,   411,     0,     0,
       0,   412,     0,     0,   914,   497,   498,     0,    14,     0,
       0,   499,   500,     0,     0,     0,   431,   432,     0,   915,
       0,   916,     0,   503,   504,   415,   437,   438,   439,   440,
     441,     0,     0,     0,     0,     0,   442,     0,   444,     0,
       0,     0,   447,     0,   406,     0,     0,     0,     0,     0,
     449,     0,     0,   409,     0,     0,   452,   410,     0,   453,
       0,     0,   454,     0,     0,     0,   457,     0,     0,     0,
       0,   407,   408,     0,     0,     0,   564,     0,     0,   461,
     462,     0,   316,   317,   318,     0,   320,   321,   322,   323,
     324,   463,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,     0,   338,   339,   340,     0,     0,   343,
     344,   345,   346,   464,   465,   466,     0,     0,     0,   411,
       0,     0,     0,   412,     0,  1314,   413,     0,   468,   469,
       0,     0,     0,     0,   409,     0,     0,     0,   410,     0,
       0,   414,     0,     0,     0,     0,     0,   415,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   473,   474,   475,   476,   477,   406,   478,   912,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     913,   565,   491,   492,     0,     0,     0,     0,     0,     0,
     493,     0,     0,   407,   408,     0,     0,     0,     0,     0,
     411,     0,     0,     0,   412,     0,     0,   914,   497,   498,
       0,    14,     0,     0,   499,   500,     0,     0,     0,   431,
     432,     0,   915,     0,   924,     0,   503,   504,   415,   437,
     438,   439,   440,   441,     0,     0,     0,     0,     0,   442,
       0,   444,     0,     0,     0,   447,     0,   596,     0,     0,
       0,     0,     0,   449,     0,     0,   409,     0,     0,   452,
     410,     0,   453,     0,     0,   454,     0,     0,     0,   457,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   564,
       0,     0,   461,   462,     0,   316,   317,   318,     0,   320,
     321,   322,   323,   324,   463,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,     0,   338,   339,   340,
       0,     0,   343,   344,   345,   346,   464,   465,   466,     0,
       0,     0,   411,     0,     0,     0,   412,     0,  1316,   413,
       0,   468,   469,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   414,     0,     0,     0,     0,     0,
     415,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   473,   474,   475,   476,   477,   406,
     478,     0,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,    59,   565,   491,   492,     0,     0,     0,
       0,     0,     0,   493,     0,     0,   407,   408,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   597,     0,     0,
     496,   497,   498,     0,    14,     0,     0,   499,   500,     0,
       0,     0,   431,   432,     0,  1163,     0,   502,     0,   503,
     504,   599,   437,   438,   439,   440,   441,     0,     0,     0,
       0,     0,   442,     0,   444,     0,     0,     0,   447,     0,
       0,     0,     0,     0,     0,     0,   449,     0,     0,   409,
       0,     0,   452,   410,     0,   453,     0,     0,   454,     0,
       0,     0,   457,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   564,     0,     0,   461,   462,     0,   316,   317,
     318,     0,   320,   321,   322,   323,   324,   463,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,     0,
     338,   339,   340,     0,     0,   343,   344,   345,   346,   464,
     465,   466,     0,     0,     0,   411,     0,     0,     0,   412,
       0,  1412,   413,     0,   468,   469,     0,     0,     0,     0,
       0,     0,     0,   639,     0,     0,     0,   414,     0,   470,
     471,   472,     0,   415,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   473,   474,   475,
     476,   477,   406,   478,     0,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,    59,   565,   491,   492,
       0,     0,     0,     0,     0,     0,   493,     0,     0,   407,
     408,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   496,   497,   498,     0,    14,     0,     0,
     499,   500,     0,     0,   431,   432,     0,     0,   501,     0,
     502,     0,   503,   504,   437,   438,   439,   440,   441,     0,
       0,     0,     0,     0,   442,     0,   444,     0,     0,     0,
     447,     0,     0,     0,     0,     0,     0,     0,   449,     0,
       0,     0,   409,     0,   452,     0,   410,   453,     0,     0,
     454,     0,     0,     0,   457,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   564,     0,     0,   461,   462,     0,
     316,   317,   318,     0,   320,   321,   322,   323,   324,   463,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,     0,   338,   339,   340,     0,     0,   343,   344,   345,
     346,   464,   465,   466,     0,     0,     0,     0,   411,     0,
       0,     0,   412,     0,  1413,   413,   468,   469,     0,     0,
       0,     0,     0,     0,     0,   669,     0,     0,     0,     0,
     414,   470,   471,   472,     0,     0,   415,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   473,
     474,   475,   476,   477,   406,   478,     0,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,    59,   565,
     491,   492,     0,     0,     0,     0,     0,     0,   493,     0,
       0,   407,   408,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   496,   497,   498,     0,    14,
       0,     0,   499,   500,     0,     0,   431,   432,     0,     0,
     501,     0,   502,     0,   503,   504,   437,   438,   439,   440,
     441,     0,     0,     0,     0,     0,   442,  1524,   444,   445,
       0,     0,   447,     0,     0,     0,     0,     0,     0,     0,
     449,     0,     0,     0,   409,     0,   452,     0,   410,   453,
       0,     0,   454,   455,     0,     0,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   564,     0,     0,   461,
     462,     0,   316,   317,   318,     0,   320,   321,   322,   323,
     324,   463,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,     0,   338,   339,   340,     0,     0,   343,
     344,   345,   346,   464,   465,   466,  1525,     0,     0,     0,
     411,     0,     0,     0,   412,     0,  1414,   413,   468,   469,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   414,     0,     0,     0,     0,     0,   415,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   473,   474,   475,   476,   477,   406,   478,     0,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
      59,   565,   491,   492,     0,     0,     0,     0,     0,     0,
     493,     0,     0,   407,   408,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   496,   497,   498,
       0,    14,     0,     0,   499,   500,     0,     0,   431,   432,
       0,     0,   501,     0,   502,     0,   503,   504,   437,   438,
     439,   440,   441,     0,     0,     0,     0,     0,   442,     0,
     444,     0,     0,     0,   447,     0,     0,     0,     0,     0,
       0,     0,   449,     0,     0,     0,   409,     0,   452,     0,
     410,   453,     0,     0,   454,     0,     0,     0,   457,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   564,     0,
       0,   461,   462,     0,   316,   317,   318,     0,   320,   321,
     322,   323,   324,   463,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,     0,   338,   339,   340,     0,
       0,   343,   344,   345,   346,   464,   465,   466,     0,     0,
       0,     0,   411,     0,     0,     0,   412,     0,  1416,   413,
     468,   469,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   414,   470,   471,   472,     0,     0,
     415,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   473,   474,   475,   476,   477,   406,   478,
       0,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,    59,   565,   491,   492,     0,     0,     0,     0,
       0,     0,   493,     0,     0,   407,   408,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   496,
     497,   498,     0,    14,     0,     0,   499,   500,     0,     0,
     431,   432,     0,     0,   501,     0,   502,     0,   503,   504,
     437,   438,   439,   440,   441,     0,     0,     0,     0,     0,
     442,     0,   444,     0,     0,   406,   447,     0,     0,     0,
       0,     0,     0,     0,   449,     0,     0,     0,   409,     0,
     452,     0,   410,   453,     0,     0,   454,     0,     0,     0,
     457,     0,   407,   408,     0,     0,     0,     0,     0,     0,
     564,     0,     0,   461,   462,     0,   316,   317,   318,     0,
     320,   321,   322,   323,   324,   463,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,     0,   338,   339,
     340,     0,     0,   343,   344,   345,   346,   464,   465,   466,
       0,     0,     0,     0,   411,     0,     0,     0,   412,     0,
    1447,   413,   468,   469,     0,   409,     0,     0,     0,   410,
       0,     0,     0,     0,     0,     0,   414,   976,   977,   978,
       0,     0,   415,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   473,   474,   475,   476,   477,
       0,   478,   406,   479,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,    59,   565,   491,   492,     0,     0,
       0,     0,     0,     0,   493,     0,     0,     0,     0,   407,
     408,   411,     0,     0,     0,   412,     0,  1540,   413,     0,
       0,   496,   497,   498,     0,    14,     0,     0,   499,   500,
       0,     0,     0,   414,   431,   432,   501,     0,   502,   415,
     503,   504,   707,     0,   437,   438,   439,   440,   441,     0,
       0,     0,     0,     0,   442,     0,   444,     0,     0,     0,
     447,     0,     0,     0,     0,     0,     0,     0,   449,     0,
       0,     0,   409,     0,   452,     0,   410,   453,   708,     0,
     454,     0,     0,     0,   457,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   564,     0,     0,   461,   462,     0,
     316,   317,   318,     0,   320,   321,   322,   323,   324,   463,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,     0,   338,   339,   340,     0,     0,   343,   344,   345,
     346,   464,   465,   466,     0,     0,     0,     0,   411,     0,
       0,     0,   412,     0,     0,   413,   468,   469,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     414,     0,     0,     0,     0,     0,   415,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   473,
     474,   475,   476,   477,     0,   478,     0,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,    59,   565,
     491,   492,     0,     0,     0,     0,     0,     0,   493,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   496,   497,   498,     0,    14,
       0,     0,   499,   500,     0,     0,     0,     0,   431,   432,
     501,   600,   502,     0,   503,   504,   707,     0,   437,   438,
     439,   440,   441,     0,     0,     0,     0,     0,   442,     0,
     444,     0,     0,     0,   447,     0,     0,     0,     0,     0,
       0,     0,   449,     0,     0,     0,     0,     0,   452,     0,
       0,   453,   708,     0,   454,     0,     0,     0,   457,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   564,     0,
       0,   461,   462,     0,   316,   317,   318,     0,   320,   321,
     322,   323,   324,   463,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,     0,   338,   339,   340,     0,
       0,   343,   344,   345,   346,   464,   465,   466,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,   469,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   473,   474,   475,   476,   477,     0,   478,
     912,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   913,   565,   491,   492,     0,     0,     0,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   496,
     497,   498,     0,    14,     0,     0,   499,   500,     0,     0,
       0,     0,   431,   432,   501,     0,   502,     0,   503,   504,
     707,     0,   437,   438,   439,   440,   441,     0,     0,     0,
       0,     0,   442,     0,   444,     0,     0,     0,   447,     0,
       0,     0,     0,     0,     0,     0,   449,     0,     0,     0,
       0,     0,   452,     0,     0,   453,   708,     0,   454,     0,
       0,     0,   457,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   564,     0,     0,   461,   462,     0,   316,   317,
     318,     0,   320,   321,   322,   323,   324,   463,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,     0,
     338,   339,   340,     0,     0,   343,   344,   345,   346,   464,
     465,   466,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   468,   469,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   473,   474,   475,
     476,   477,     0,   478,     0,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,    59,   565,   491,   492,
       0,     0,     0,     0,     0,     0,   493,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   496,   497,   498,     0,    14,     0,     0,
     499,   500,     0,     0,     0,     0,   431,   432,   501,   824,
     502,     0,   503,   504,   707,     0,   437,   438,   439,   440,
     441,     0,     0,     0,     0,     0,   442,     0,   444,     0,
       0,     0,   447,     0,     0,     0,     0,     0,     0,     0,
     449,     0,     0,     0,     0,     0,   452,     0,     0,   453,
     708,     0,   454,     0,     0,     0,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   564,     0,     0,   461,
     462,     0,   316,   317,   318,     0,   320,   321,   322,   323,
     324,   463,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,     0,   338,   339,   340,     0,     0,   343,
     344,   345,   346,   464,   465,   466,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   468,   469,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   473,   474,   475,   476,   477,     0,   478,     0,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
      59,   565,   491,   492,     0,     0,     0,     0,     0,     0,
     493,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   496,   497,   498,
       0,    14,     0,     0,   499,   500,     0,     0,   431,   432,
       0,     0,   501,     0,   502,     0,   503,   504,   437,   438,
     439,   440,   441,     0,     0,     0,     0,     0,   442,     0,
     444,     0,     0,     0,   447,     0,     0,     0,     0,     0,
       0,     0,   449,     0,     0,     0,     0,     0,   452,     0,
       0,   453,     0,     0,   454,     0,     0,     0,   457,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   564,     0,
       0,   461,   462,     0,   316,   317,   318,     0,   320,   321,
     322,   323,   324,   463,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,     0,   338,   339,   340,     0,
       0,   343,   344,   345,   346,   464,   465,   466,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,   469,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   473,   474,   475,   476,   477,     0,   478,
     912,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   913,   565,   491,   492,     0,     0,     0,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   496,
     497,   498,     0,    14,     0,     0,   499,   500,     0,     0,
     431,   432,     0,     0,   956,     0,   502,   957,   503,   504,
     437,   438,   439,   440,   441,     0,     0,     0,     0,     0,
     442,     0,   444,     0,     0,     0,   447,     0,     0,     0,
       0,     0,     0,     0,   449,     0,     0,     0,     0,     0,
     452,     0,     0,   453,     0,     0,   454,     0,     0,     0,
     457,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     564,     0,     0,   461,   462,  1101,   316,   317,   318,     0,
     320,   321,   322,   323,   324,   463,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,     0,   338,   339,
     340,     0,     0,   343,   344,   345,   346,   464,   465,   466,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   468,   469,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   473,   474,   475,   476,   477,
       0,   478,   912,   479,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   913,   565,   491,   492,     0,     0,
       0,     0,     0,     0,   493,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   496,   497,   498,     0,    14,     0,     0,   499,   500,
       0,     0,   431,   432,     0,     0,   501,     0,   502,     0,
     503,   504,   437,   438,   439,   440,   441,     0,     0,     0,
       0,     0,   442,     0,   444,     0,     0,     0,   447,     0,
       0,     0,     0,     0,     0,     0,   449,     0,     0,     0,
       0,     0,   452,     0,     0,   453,     0,     0,   454,     0,
       0,     0,   457,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   564,     0,     0,   461,   462,     0,   316,   317,
     318,     0,   320,   321,   322,   323,   324,   463,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,     0,
     338,   339,   340,     0,     0,   343,   344,   345,   346,   464,
     465,   466,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   468,   469,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   473,   474,   475,
     476,   477,     0,   478,     0,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,    59,   565,   491,   492,
       0,     0,     0,     0,     0,     0,   493,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   496,   497,   498,     0,    14,     0,     0,
     499,   500,     0,     0,   431,   432,     0,     0,   501,   600,
     502,     0,   503,   504,   437,   438,   439,   440,   441,     0,
       0,     0,     0,     0,   442,     0,   444,     0,     0,     0,
     447,     0,     0,     0,     0,     0,     0,     0,   449,     0,
       0,     0,     0,     0,   452,     0,     0,   453,     0,     0,
     454,     0,     0,     0,   457,     0,     0,     0,     0,     0,
     647,     0,     0,     0,   564,     0,     0,   461,   462,     0,
     316,   317,   318,     0,   320,   321,   322,   323,   324,   463,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,     0,   338,   339,   340,     0,     0,   343,   344,   345,
     346,   464,   465,   466,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   468,   469,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   473,
     474,   475,   476,   477,     0,   478,     0,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,    59,   565,
     491,   492,     0,     0,     0,     0,     0,     0,   493,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   496,   497,   498,     0,    14,
       0,     0,   499,   500,     0,     0,   431,   432,     0,     0,
     501,     0,   502,     0,   503,   504,   437,   438,   439,   440,
     441,     0,     0,     0,     0,     0,   442,     0,   444,     0,
       0,     0,   447,     0,     0,     0,     0,     0,     0,     0,
     449,     0,     0,     0,     0,     0,   452,     0,     0,   453,
       0,     0,   454,     0,     0,     0,   457,     0,     0,   658,
       0,     0,     0,     0,     0,     0,   564,     0,     0,   461,
     462,     0,   316,   317,   318,     0,   320,   321,   322,   323,
     324,   463,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,     0,   338,   339,   340,     0,     0,   343,
     344,   345,   346,   464,   465,   466,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   468,   469,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   473,   474,   475,   476,   477,     0,   478,     0,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
      59,   565,   491,   492,     0,     0,     0,     0,     0,     0,
     493,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   496,   497,   498,
       0,    14,     0,     0,   499,   500,     0,     0,     0,     0,
     431,   432,   501,     0,   502,     0,   503,   504,   690,     0,
     437,   438,   439,   440,   441,     0,     0,     0,     0,     0,
     442,     0,   444,     0,     0,     0,   447,     0,     0,     0,
       0,     0,     0,     0,   449,     0,     0,     0,     0,     0,
     452,     0,     0,   453,     0,     0,   454,     0,     0,     0,
     457,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     564,     0,     0,   461,   462,     0,   316,   317,   318,     0,
     320,   321,   322,   323,   324,   463,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,     0,   338,   339,
     340,     0,     0,   343,   344,   345,   346,   464,   465,   466,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   468,   469,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   473,   474,   475,   476,   477,
       0,   478,     0,   479,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,    59,   565,   491,   492,     0,     0,
       0,     0,     0,     0,   493,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   496,   497,   498,     0,    14,     0,     0,   499,   500,
       0,     0,   431,   432,     0,     0,   501,     0,   502,     0,
     503,   504,   437,   438,   439,   440,   441,     0,     0,     0,
       0,     0,   442,     0,   444,     0,     0,     0,   447,     0,
       0,     0,     0,     0,     0,     0,   449,     0,     0,     0,
       0,     0,   452,     0,     0,   453,     0,     0,   454,     0,
       0,     0,   457,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   564,     0,     0,   461,   462,     0,   316,   317,
     318,     0,   320,   321,   322,   323,   324,   463,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,     0,
     338,   339,   340,     0,     0,   343,   344,   345,   346,   464,
     465,   466,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   468,   469,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   473,   474,   475,
     476,   477,     0,   478,     0,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,    59,   565,   491,   492,
       0,     0,     0,     0,     0,     0,   493,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   694,     0,   496,   497,   498,     0,    14,     0,     0,
     499,   500,     0,     0,     0,     0,   431,   432,   501,     0,
     502,     0,   503,   504,   698,     0,   437,   438,   439,   440,
     441,     0,     0,     0,     0,     0,   442,     0,   444,     0,
       0,     0,   447,     0,     0,     0,     0,     0,     0,     0,
     449,     0,     0,     0,     0,     0,   452,     0,     0,   453,
       0,     0,   454,     0,     0,     0,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   564,     0,     0,   461,
     462,     0,   316,   317,   318,     0,   320,   321,   322,   323,
     324,   463,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,     0,   338,   339,   340,     0,     0,   343,
     344,   345,   346,   464,   465,   466,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   468,   469,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   473,   474,   475,   476,   477,     0,   478,     0,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
      59,   565,   491,   492,     0,     0,     0,     0,     0,     0,
     493,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   496,   497,   498,
       0,    14,     0,     0,   499,   500,     0,     0,   431,   432,
       0,     0,   501,     0,   502,     0,   503,   504,   437,   438,
     439,   440,   441,     0,     0,  1035,     0,     0,   442,     0,
     444,     0,     0,     0,   447,     0,     0,     0,     0,     0,
       0,     0,   449,     0,     0,     0,     0,     0,   452,     0,
       0,   453,     0,     0,   454,     0,     0,     0,   457,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   564,     0,
       0,   461,   462,     0,   316,   317,   318,     0,   320,   321,
     322,   323,   324,   463,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,     0,   338,   339,   340,     0,
       0,   343,   344,   345,   346,   464,   465,   466,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,   469,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   473,   474,   475,   476,   477,     0,   478,
       0,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,    59,   565,   491,   492,     0,     0,     0,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   496,
     497,   498,     0,    14,     0,     0,   499,   500,     0,     0,
     431,   432,     0,     0,   501,     0,   502,     0,   503,   504,
     437,   438,   439,   440,   441,     0,     0,     0,     0,     0,
     442,     0,   444,     0,     0,     0,   447,     0,     0,     0,
       0,     0,     0,     0,   449,     0,     0,     0,     0,     0,
     452,     0,     0,   453,     0,     0,   454,     0,     0,     0,
     457,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     564,     0,     0,   461,   462,     0,   316,   317,   318,     0,
     320,   321,   322,   323,   324,   463,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,     0,   338,   339,
     340,     0,     0,   343,   344,   345,   346,   464,   465,   466,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   468,   469,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   473,   474,   475,   476,   477,
       0,   478,     0,   479,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,    59,   565,   491,   492,     0,     0,
       0,     0,     0,     0,   493,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   496,   497,   498,     0,    14,     0,     0,   499,   500,
       0,     0,   431,   432,     0,     0,   501,     0,   502,  1055,
     503,   504,   437,   438,   439,   440,   441,     0,     0,     0,
       0,     0,   442,     0,   444,     0,     0,     0,   447,     0,
       0,     0,     0,     0,     0,     0,   449,     0,     0,     0,
       0,     0,   452,     0,     0,   453,     0,     0,   454,     0,
       0,     0,   457,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   564,     0,     0,   461,   462,     0,   316,   317,
     318,     0,   320,   321,   322,   323,   324,   463,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,     0,
     338,   339,   340,     0,     0,   343,   344,   345,   346,   464,
     465,   466,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   468,   469,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   473,   474,   475,
     476,   477,     0,   478,     0,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,    59,   565,   491,   492,
       0,     0,     0,     0,     0,     0,   493,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1105,     0,   496,   497,   498,     0,    14,     0,     0,
     499,   500,     0,     0,   431,   432,     0,     0,   501,     0,
     502,     0,   503,   504,   437,   438,   439,   440,   441,     0,
       0,     0,     0,     0,   442,     0,   444,     0,     0,     0,
     447,     0,     0,     0,     0,     0,     0,     0,   449,     0,
       0,     0,     0,     0,   452,     0,     0,   453,     0,     0,
     454,     0,     0,     0,   457,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   564,     0,     0,   461,   462,     0,
     316,   317,   318,     0,   320,   321,   322,   323,   324,   463,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,     0,   338,   339,   340,     0,     0,   343,   344,   345,
     346,   464,   465,   466,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   468,   469,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   473,
     474,   475,   476,   477,     0,   478,     0,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,    59,   565,
     491,   492,     0,     0,     0,     0,     0,     0,   493,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   496,   497,   498,     0,    14,
       0,     0,   499,   500,     0,     0,   431,   432,     0,     0,
     501,     0,   502,  1165,   503,   504,   437,   438,   439,   440,
     441,     0,     0,     0,     0,     0,   442,     0,   444,     0,
       0,     0,   447,     0,     0,     0,     0,     0,     0,     0,
     449,     0,     0,     0,     0,     0,   452,     0,     0,   453,
       0,     0,   454,     0,     0,     0,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   564,     0,     0,   461,
     462,     0,   316,   317,   318,     0,   320,   321,   322,   323,
     324,   463,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,     0,   338,   339,   340,     0,     0,   343,
     344,   345,   346,   464,   465,   466,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   468,   469,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   473,   474,   475,   476,   477,     0,   478,     0,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
      59,   565,   491,   492,     0,     0,     0,     0,     0,     0,
     493,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   496,   497,   498,
       0,    14,     0,     0,   499,   500,     0,     0,   431,   432,
       0,     0,   501,     0,   502,  1180,   503,   504,   437,   438,
     439,   440,   441,     0,     0,     0,     0,     0,   442,     0,
     444,     0,     0,     0,   447,     0,     0,     0,     0,     0,
       0,     0,   449,     0,     0,     0,     0,     0,   452,     0,
       0,   453,     0,     0,   454,     0,     0,     0,   457,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   564,     0,
       0,   461,   462,     0,   316,   317,   318,     0,   320,   321,
     322,   323,   324,   463,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,     0,   338,   339,   340,     0,
       0,   343,   344,   345,   346,   464,   465,   466,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,   469,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   473,   474,   475,   476,   477,     0,   478,
       0,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,    59,   565,   491,   492,     0,     0,     0,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   496,
     497,   498,     0,    14,     0,     0,   499,   500,     0,     0,
     431,   432,     0,     0,   501,     0,   502,  1375,   503,   504,
     437,   438,   439,   440,   441,     0,     0,     0,     0,     0,
     442,     0,   444,     0,     0,     0,   447,     0,     0,     0,
       0,     0,     0,     0,   449,     0,     0,     0,     0,     0,
     452,     0,     0,   453,     0,     0,   454,     0,     0,     0,
     457,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     564,     0,     0,   461,   462,     0,   316,   317,   318,     0,
     320,   321,   322,   323,   324,   463,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,     0,   338,   339,
     340,     0,     0,   343,   344,   345,   346,   464,   465,   466,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   468,   469,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   473,   474,   475,   476,   477,
       0,   478,     0,   479,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,    59,   565,   491,   492,     0,     0,
       0,     0,     0,     0,   493,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   496,   497,   498,     0,    14,     0,     0,   499,   500,
       0,     0,   431,   432,     0,     0,   501,     0,   502,  1384,
     503,   504,   437,   438,   439,   440,   441,     0,     0,     0,
       0,     0,   442,     0,   444,     0,     0,     0,   447,     0,
       0,     0,     0,     0,     0,     0,   449,     0,     0,     0,
       0,     0,   452,     0,     0,   453,     0,     0,   454,     0,
       0,     0,   457,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   564,     0,     0,   461,   462,     0,   316,   317,
     318,     0,   320,   321,   322,   323,   324,   463,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,     0,
     338,   339,   340,     0,     0,   343,   344,   345,   346,   464,
     465,   466,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   468,   469,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   473,   474,   475,
     476,   477,     0,   478,     0,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,    59,   565,   491,   492,
       0,     0,     0,     0,     0,     0,   493,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   496,   497,   498,     0,    14,     0,     0,
     499,   500,     0,     0,   431,   432,     0,     0,   501,     0,
     502,  1427,   503,   504,   437,   438,   439,   440,   441,     0,
       0,     0,     0,     0,   442,     0,   444,     0,     0,     0,
     447,     0,     0,     0,     0,     0,     0,     0,   449,     0,
       0,     0,     0,     0,   452,     0,     0,   453,     0,     0,
     454,     0,     0,     0,   457,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   564,     0,     0,   461,   462,     0,
     316,   317,   318,     0,   320,   321,   322,   323,   324,   463,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,     0,   338,   339,   340,     0,     0,   343,   344,   345,
     346,   464,   465,   466,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   468,   469,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   473,
     474,   475,   476,   477,     0,   478,     0,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,    59,   565,
     491,   492,     0,     0,     0,     0,     0,     0,   493,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   496,   497,   498,     0,    14,
       0,     0,   499,   500,     0,     0,   431,   432,     0,     0,
     501,     0,   502,  1501,   503,   504,   437,   438,   439,   440,
     441,     0,     0,     0,     0,     0,   442,     0,   444,     0,
       0,     0,   447,     0,     0,     0,     0,     0,     0,     0,
     449,     0,     0,     0,     0,     0,   452,     0,     0,   453,
       0,     0,   454,     0,     0,     0,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   564,     0,     0,   461,
     462,     0,   316,   317,   318,     0,   320,   321,   322,   323,
     324,   463,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,     0,   338,   339,   340,     0,     0,   343,
     344,   345,   346,   464,   465,   466,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   468,   469,
       0,     0,     0,     0,     0,     0,     0,  1538,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   473,   474,   475,   476,   477,     0,   478,     0,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
      59,   565,   491,   492,     0,     0,     0,     0,     0,     0,
     493,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   496,   497,   498,
       0,    14,     0,     0,   499,   500,     0,     0,   431,   432,
       0,     0,   501,     0,   502,     0,   503,   504,   437,   438,
     439,   440,   441,     0,     0,     0,     0,     0,   442,     0,
     444,     0,     0,     0,   447,     0,     0,     0,     0,     0,
       0,     0,   449,     0,     0,     0,     0,     0,   452,     0,
       0,   453,     0,     0,   454,     0,     0,     0,   457,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   564,     0,
       0,   461,   462,     0,   316,   317,   318,     0,   320,   321,
     322,   323,   324,   463,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,     0,   338,   339,   340,     0,
       0,   343,   344,   345,   346,   464,   465,   466,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,   469,     0,     0,     0,     0,     0,     0,     0,  1580,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   473,   474,   475,   476,   477,     0,   478,
       0,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,    59,   565,   491,   492,     0,     0,     0,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   496,
     497,   498,     0,    14,     0,     0,   499,   500,     0,     0,
     431,   432,     0,     0,   501,     0,   502,     0,   503,   504,
     437,   438,   439,   440,   441,     0,     0,     0,     0,     0,
     442,     0,   444,     0,     0,     0,   447,     0,     0,     0,
       0,     0,     0,     0,   449,     0,     0,     0,     0,     0,
     452,     0,     0,   453,     0,     0,   454,     0,     0,     0,
     457,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     564,     0,     0,   461,   462,     0,   316,   317,   318,     0,
     320,   321,   322,   323,   324,   463,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,     0,   338,   339,
     340,     0,     0,   343,   344,   345,   346,   464,   465,   466,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   468,   469,     0,     0,     0,     0,     0,     0,
       0,  1581,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   473,   474,   475,   476,   477,
       0,   478,     0,   479,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,    59,   565,   491,   492,     0,     0,
       0,     0,     0,     0,   493,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   496,   497,   498,     0,    14,     0,     0,   499,   500,
       0,     0,   431,   432,     0,     0,   501,     0,   502,     0,
     503,   504,   437,   438,   439,   440,   441,     0,     0,     0,
       0,     0,   442,     0,   444,     0,     0,     0,   447,     0,
       0,     0,     0,     0,     0,     0,   449,     0,     0,     0,
       0,     0,   452,     0,     0,   453,     0,     0,   454,     0,
       0,     0,   457,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   564,     0,     0,   461,   462,     0,   316,   317,
     318,     0,   320,   321,   322,   323,   324,   463,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,     0,
     338,   339,   340,     0,     0,   343,   344,   345,   346,   464,
     465,   466,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   468,   469,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   473,   474,   475,
     476,   477,     0,   478,     0,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,    59,   565,   491,   492,
       0,     0,     0,     0,     0,     0,   493,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   496,   497,   498,     0,    14,     0,     0,
     499,   500,     0,     0,   431,   432,     0,     0,   501,     0,
     502,     0,   503,   504,   437,   438,   439,   440,   441,     0,
       0,     0,     0,     0,   442,     0,   444,     0,     0,     0,
     447,     0,     0,     0,     0,     0,     0,     0,   449,     0,
       0,     0,     0,     0,   452,     0,     0,   453,     0,     0,
     454,     0,     0,     0,   457,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   564,     0,     0,   461,   462,     0,
     316,   317,   318,     0,   320,   321,   322,   323,   324,   463,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,     0,   338,   339,   340,     0,     0,   343,   344,   345,
     346,   464,   465,   466,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   468,   469,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   473,
     474,   475,   476,   477,     0,   478,     0,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,    59,   565,
     491,   492,     0,   688,     0,     0,     0,     0,   493,   310,
       0,     0,     0,     0,     0,   311,     0,     0,     0,     0,
       0,   312,     0,     0,     0,   496,   497,   498,     0,    14,
       0,   313,   499,   500,     0,     0,     0,     0,     0,   314,
    1360,     0,   502,     0,   503,   504,     0,     0,     0,     0,
       0,     0,     0,     0,   315,     0,     0,     0,     0,     0,
       0,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   310,     0,
       0,     0,     0,     0,   311,     0,     0,     0,     0,     0,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,     0,    58,     0,     0,     0,     0,     0,   314,   731,
     732,     0,     0,     0,     0,   349,     0,     0,     0,     0,
       0,     0,     0,   315,     0,     0,     0,     0,     0,    59,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   350,     0,   310,     0,     0,
       0,     0,     0,   311,     0,     0,     0,     0,     0,   312,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   313,
       0,    58,     0,   741,   742,   743,   744,   314,     0,     0,
       0,     0,     0,   751,   349,   753,   754,   755,   756,     0,
       0,     0,   315,   757,     0,   759,   760,     0,    59,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   350,     0,   603,     0,     0,     0,
       0,     0,     0,     0,   777,   778,   779,   780,   781,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   782,   783,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   349,     0,     0,     0,     0,     0,     0,
       0,     0,   310,     0,     0,     0,     0,   608,   311,     0,
       0,     0,     0,     0,   312,     0,     0,     0,     0,   609,
       0,     0,     0,     0,   313,     0,     0,     0,     0,     0,
       0,     0,   314,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   315,     0,     0,
       0,     0,     0,   350,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   310,     0,     0,     0,     0,     0,   311,     0,     0,
       0,     0,     0,   312,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,     0,    58,     0,     0,     0,     0,
       0,   314,     0,     0,     0,     0,     0,     0,   349,     0,
       0,     0,     0,     0,     0,     0,   315,     0,     0,     0,
       0,     0,    59,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   731,   732,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   350,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   349,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   608,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   862,   863,   864,   865,   866,   867,   868,   869,
     741,   742,   743,   744,   745,   870,   871,   748,   749,   750,
     751,   872,   753,   754,   755,   756,  -355,   350,   731,   732,
     757,   758,   759,   760,   873,   874,     0,     0,   763,   764,
     765,   875,   876,   877,   769,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   878,   771,     0,   772,   773,   774,   775,
     776,   777,   778,   779,   780,   781,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   782,   783,     0,     0,     0,
     503,   674,     0,     0,   862,   863,   864,   865,   866,   867,
     868,   869,   741,   742,   743,   744,   745,   870,   871,   748,
     749,   750,   751,   872,   753,   754,   755,   756,   731,   732,
       0,     0,   757,   758,   759,   760,   873,   874,     0,     0,
     763,   764,   765,   875,   876,   877,   769,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   731,   732,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1066,     0,
       0,     0,     0,     0,     0,   878,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   782,   783,     0,
       0,     0,   503,   674,   862,   863,   864,   865,   866,   867,
     868,   869,   741,   742,   743,   744,   745,   870,   871,   748,
     749,   750,   751,   872,   753,   754,   755,   756,     0,     0,
       0,     0,   757,   758,   759,   760,   873,   874,     0,     0,
     763,   764,   765,   875,   876,   877,   769,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,   731,   732,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,  1260,     0,
       0,   769,     0,     0,     0,   878,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   731,   732,
       0,     0,     0,     0,     0,     0,     0,   782,   783,     0,
       0,     0,   503,   674,     0,     0,     0,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   782,   783,     0,     0,     0,   503,   674,     0,
       0,     0,     0,     0,     0,     0,     0,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,     0,     0,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,     0,     0,
       0,   769,   741,   742,   743,   744,   745,     0,     0,   748,
     749,   750,   751,     0,   753,   754,   755,   756,   731,   732,
       0,     0,   757,     0,   759,   760,     0,     0,     0,     0,
     763,   764,   765,     0,     0,     0,   769,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   731,   732,     0,     0,     0,     0,     0,
       0,     0,   782,   783,     0,     0,   794,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   782,   783,     0,
       0,   804,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   741,   742,   743,   744,   745,     0,     0,   748,
     749,   750,   751,     0,   753,   754,   755,   756,     0,     0,
       0,     0,   757,     0,   759,   760,     0,     0,     0,     0,
     763,   764,   765,     0,     0,     0,   769,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,   731,   732,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,     0,     0,
       0,   769,     0,     0,     0,     0,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   731,   732,
       0,     0,     0,     0,     0,     0,     0,   782,   783,     0,
       0,   820,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   782,   783,     0,     0,   835,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,     0,     0,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,     0,     0,
       0,   769,   741,   742,   743,   744,   745,     0,     0,   748,
     749,   750,   751,     0,   753,   754,   755,   756,   731,   732,
       0,     0,   757,     0,   759,   760,     0,     0,     0,     0,
     763,   764,   765,     0,     0,     0,   769,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   731,   732,     0,     0,     0,     0,     0,
       0,     0,   782,   783,     0,     0,  1137,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   782,   783,     0,
       0,  1141,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   741,   742,   743,   744,   745,     0,     0,   748,
     749,   750,   751,     0,   753,   754,   755,   756,     0,     0,
       0,     0,   757,     0,   759,   760,     0,     0,     0,     0,
     763,   764,   765,     0,     0,     0,   769,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,   731,   732,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,     0,     0,
       0,   769,     0,     0,     0,     0,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   731,   732,
       0,     0,     0,     0,     0,     0,     0,   782,   783,     0,
       0,  1143,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   782,   783,     0,     0,  1152,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,     0,     0,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,     0,     0,
       0,   769,   741,   742,   743,   744,   745,     0,     0,   748,
     749,   750,   751,     0,   753,   754,   755,   756,   731,   732,
       0,     0,   757,     0,   759,   760,     0,     0,     0,     0,
     763,   764,   765,     0,     0,     0,   769,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   731,   732,     0,     0,     0,     0,     0,
       0,     0,   782,   783,     0,     0,  1153,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   782,   783,     0,
       0,  1154,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   741,   742,   743,   744,   745,     0,     0,   748,
     749,   750,   751,     0,   753,   754,   755,   756,     0,     0,
       0,     0,   757,     0,   759,   760,     0,     0,     0,     0,
     763,   764,   765,     0,     0,     0,   769,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,   731,   732,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,     0,     0,
       0,   769,     0,     0,     0,     0,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   731,   732,
       0,     0,     0,     0,     0,     0,     0,   782,   783,     0,
       0,  1155,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   782,   783,     0,     0,  1156,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,     0,     0,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,     0,     0,
       0,   769,   741,   742,   743,   744,   745,     0,     0,   748,
     749,   750,   751,     0,   753,   754,   755,   756,   731,   732,
       0,     0,   757,     0,   759,   760,     0,     0,     0,     0,
     763,   764,   765,     0,     0,     0,   769,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   731,   732,     0,     0,     0,     0,     0,
       0,     0,   782,   783,     0,     0,  1157,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   782,   783,     0,
       0,  1281,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   741,   742,   743,   744,   745,     0,     0,   748,
     749,   750,   751,     0,   753,   754,   755,   756,     0,     0,
       0,     0,   757,     0,   759,   760,     0,     0,     0,     0,
     763,   764,   765,     0,     0,     0,   769,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,   731,   732,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,     0,     0,
       0,   769,     0,     0,     0,     0,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   731,   732,
       0,     0,     0,     0,     0,     0,     0,   782,   783,     0,
       0,  1294,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   782,   783,     0,     0,  1297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,     0,     0,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,     0,     0,
       0,   769,   741,   742,   743,   744,   745,     0,     0,   748,
     749,   750,   751,     0,   753,   754,   755,   756,   731,   732,
       0,     0,   757,     0,   759,   760,     0,     0,     0,     0,
     763,   764,   765,     0,     0,     0,   769,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   731,   732,     0,     0,     0,     0,     0,
       0,     0,   782,   783,     0,     0,  1430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   782,   783,     0,
       0,  1445,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   741,   742,   743,   744,   745,     0,     0,   748,
     749,   750,   751,     0,   753,   754,   755,   756,     0,     0,
       0,     0,   757,     0,   759,   760,     0,     0,     0,     0,
     763,   764,   765,     0,     0,     0,   769,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,   731,   732,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,     0,     0,
       0,   769,     0,     0,     0,     0,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   731,   732,
       0,     0,     0,     0,     0,     0,     0,   782,   783,     0,
       0,  1446,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   782,   783,     0,     0,  1465,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,     0,     0,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,     0,     0,
       0,   769,   741,   742,   743,   744,   745,     0,     0,   748,
     749,   750,   751,     0,   753,   754,   755,   756,   731,   732,
       0,     0,   757,     0,   759,   760,     0,     0,     0,     0,
     763,   764,   765,     0,     0,     0,   769,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   731,   732,     0,     0,     0,     0,     0,
       0,     0,   782,   783,     0,     0,  1467,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   782,   783,     0,
       0,  1469,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   741,   742,   743,   744,   745,     0,     0,   748,
     749,   750,   751,     0,   753,   754,   755,   756,     0,     0,
       0,     0,   757,     0,   759,   760,     0,     0,     0,     0,
     763,   764,   765,     0,     0,     0,   769,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,   731,   732,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,     0,     0,
       0,   769,     0,     0,     0,     0,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   731,   732,
       0,     0,     0,     0,     0,     0,     0,   782,   783,     0,
       0,  1473,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   782,   783,     0,     0,  1543,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,     0,     0,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,     0,     0,
       0,   769,   741,   742,   743,   744,   745,     0,     0,   748,
     749,   750,   751,     0,   753,   754,   755,   756,   731,   732,
       0,     0,   757,     0,   759,   760,     0,     0,     0,     0,
     763,   764,   765,     0,     0,     0,   769,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   731,   732,     0,     0,     0,     0,     0,
       0,     0,   782,   783,     0,     0,  1544,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   782,   783,     0,
       0,  1545,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   741,   742,   743,   744,   745,     0,     0,   748,
     749,   750,   751,     0,   753,   754,   755,   756,     0,     0,
       0,     0,   757,     0,   759,   760,     0,     0,     0,     0,
     763,   764,   765,     0,     0,     0,   769,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,   731,   732,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,     0,     0,
       0,   769,     0,     0,     0,     0,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   731,   732,
       0,     0,     0,     0,     0,     0,     0,   782,   783,     0,
       0,  1553,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   782,   783,     0,     0,  1555,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,     0,     0,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,     0,     0,
       0,   769,   741,   742,   743,   744,   745,     0,     0,   748,
     749,   750,   751,     0,   753,   754,   755,   756,   731,   732,
       0,     0,   757,     0,   759,   760,     0,     0,     0,     0,
     763,   764,   765,     0,     0,     0,   769,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   731,   732,     0,     0,     0,     0,     0,
       0,     0,   782,   783,     0,     0,  1566,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   782,   783,     0,
       0,  1571,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   741,   742,   743,   744,   745,     0,     0,   748,
     749,   750,   751,     0,   753,   754,   755,   756,     0,     0,
       0,     0,   757,     0,   759,   760,     0,     0,     0,     0,
     763,   764,   765,     0,     0,     0,   769,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,   731,   732,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,     0,     0,
       0,   769,     0,     0,     0,     0,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   731,   732,
       0,     0,     0,     0,     0,     0,     0,   782,   783,     0,
       0,  1634,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   782,   783,     0,     0,  1635,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,     0,     0,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,     0,     0,
       0,   769,   741,   742,   743,   744,   745,   731,   732,   748,
     749,   750,   751,     0,   753,   754,   755,   756,     0,     0,
       0,     0,   757,     0,   759,   760,     0,     0,     0,     0,
     763,   764,   765,     0,     0,     0,   769,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   782,   783,   825,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   782,   783,  1088,
       0,   741,   742,   743,   744,   745,   731,   732,   748,   749,
     750,   751,     0,   753,   754,   755,   756,     0,     0,     0,
       0,   757,     0,   759,   760,     0,     0,     0,     0,   763,
     764,   765,     0,     0,     0,   769,     0,     0,     0,     0,
       0,   731,   732,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   771,     0,   772,   773,   774,
     775,   776,   777,   778,   779,   780,   781,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   782,   783,  1234,     0,
     741,   742,   743,   744,   745,     0,     0,   748,   749,   750,
     751,     0,   753,   754,   755,   756,     0,     0,     0,     0,
     757,     0,   759,   760,     0,     0,     0,     0,   763,   764,
     765,     0,     0,     0,   769,   741,   742,   743,   744,   745,
     731,   732,   748,   749,   750,   751,     0,   753,   754,   755,
     756,     0,     0,     0,     0,   757,     0,   759,   760,     0,
       0,     0,     0,   763,   764,   765,     0,     0,     0,   769,
       0,     0,     0,     0,   771,     0,   772,   773,   774,   775,
     776,   777,   778,   779,   780,   781,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   782,   783,  1249,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   771,
       0,   772,   773,   774,   775,   776,   777,   778,   779,   780,
     781,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     782,   783,  1382,     0,   741,   742,   743,   744,   745,     0,
       0,   748,   749,   750,   751,     0,   753,   754,   755,   756,
     231,   232,     0,     0,   757,     0,   759,   760,     0,     0,
       0,     0,   763,   764,   765,     0,     0,   233,   769,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   731,   732,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   771,     0,
     772,   773,   774,   775,   776,   777,   778,   779,   780,   781,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   782,
     783,  1387,     0,     0,     0,     0,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,     0,     0,   252,   253,   254,     0,
       0,     0,     0,     0,     0,   255,   256,   257,   258,   259,
       0,     0,   260,   261,   262,   263,   264,   265,   266,   741,
     742,   743,   744,   745,   731,   732,   748,   749,   750,   751,
       0,   753,   754,   755,   756,     0,     0,     0,     0,   757,
       0,   759,   760,     0,     0,     0,     0,   763,   764,   765,
       0,     0,     0,   769,     0,     0,     0,     0,   267,     0,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
       0,     0,   278,   279,     0,     0,     0,     0,     0,   280,
     281,     0,     0,   286,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   771,     0,   772,   773,   774,   775,   776,
     777,   778,   779,   780,   781,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   782,   783,     0,     0,   741,   742,
     743,   744,   745,   731,   732,   748,   749,   750,   751,     0,
     753,   754,   755,   756,     0,     0,     0,     0,   757,     0,
     759,   760,     0,     0,   928,     0,   763,   764,   765,     0,
       0,     0,   769,     0,     0,     0,     0,     0,   731,   732,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   771,     0,   772,   773,   774,   775,   776,   777,
     778,   779,   780,   781,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   782,   783,     0,     0,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,     0,     0,     0,     0,   757,     0,   759,
     760,     0,     0,  1166,     0,   763,   764,   765,   731,   732,
       0,   769,   741,   742,   743,   744,   745,     0,     0,   748,
     749,   750,   751,     0,   753,   754,   755,   756,     0,     0,
       0,     0,   757,     0,   759,   760,     0,     0,     0,     0,
     763,   764,   765,   731,   732,     0,   769,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   782,   783,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   771,  1239,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,     0,     0,
       0,     0,   741,   742,   743,   744,   745,   782,   783,   748,
     749,   750,   751,     0,   753,   754,   755,   756,     0,     0,
       0,     0,   757,     0,   759,   760,     0,     0,     0,     0,
     763,   764,   765,   731,   732,     0,   769,   741,   742,   743,
     744,   745,     0,     0,   748,   749,   750,   751,     0,   753,
     754,   755,   756,     0,     0,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,  1251,     0,
       0,   769,   731,   732,     0,     0,   771,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   782,   783,  1345,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,     0,     0,     0,     0,   741,   742,   743,
     744,   745,   782,   783,   748,   749,   750,   751,     0,   753,
     754,   755,   756,     0,     0,     0,     0,   757,     0,   759,
     760,     0,     0,     0,     0,   763,   764,   765,     0,     0,
       0,   769,   731,   732,     0,     0,   741,   742,   743,   744,
     745,     0,     0,   748,   749,   750,   751,     0,   753,   754,
     755,   756,     0,     0,     0,     0,   757,     0,   759,   760,
       0,     0,     0,  1567,   763,   764,   765,   731,   732,     0,
     769,   771,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   782,   783,     0,     0,     0,     0,     0,     0,
       0,     0,  1625,     0,     0,     0,     0,     0,     0,     0,
     771,     0,   772,   773,   774,   775,   776,   777,   778,   779,
     780,   781,     0,     0,     0,     0,   741,   742,   743,   744,
     745,   782,   783,   748,   749,   750,   751,     0,   753,   754,
     755,   756,     0,     0,     0,     0,   757,     0,   759,   760,
       0,     0,     0,     0,   763,   764,   765,   731,   732,     0,
     769,   741,   742,   743,   744,   745,     0,     0,   748,   749,
     750,   751,     0,   753,   754,   755,   756,     0,     0,     0,
       0,   757,     0,   759,   760,     0,     0,     0,     0,   763,
     764,   765,  1651,     0,     0,   769,   731,   732,     0,     0,
     771,     0,   772,   773,   774,   775,   776,   777,   778,   779,
     780,   781,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   782,   783,     0,     0,     0,     0,  1652,     0,     0,
       0,     0,     0,     0,     0,   771,     0,   772,   773,   774,
     775,   776,   777,   778,   779,   780,   781,     0,     0,     0,
       0,   741,   742,   743,   744,   745,   782,   783,   748,   749,
     750,   751,     0,   753,   754,   755,   756,     0,     0,     0,
       0,   757,     0,   759,   760,     0,     0,     0,     0,   763,
     764,   765,     0,     0,     0,   769,   731,   732,     0,     0,
     741,   742,   743,   744,   745,     0,     0,   748,   749,   750,
     751,     0,   753,   754,   755,   756,     0,     0,     0,     0,
     757,     0,   759,   760,     0,     0,     0,  1658,   763,   764,
     765,   731,   732,     0,   769,   771,     0,   772,   773,   774,
     775,   776,   777,   778,   779,   780,   781,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   782,   783,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   771,     0,   772,   773,   774,   775,
     776,   777,   778,   779,   780,   781,     0,     0,     0,     0,
     741,   742,   743,   744,   745,   782,   783,   748,   749,   750,
     751,     0,   753,   754,   755,   756,     0,     0,     0,     0,
     757,     0,   759,   760,     0,     0,     0,     0,   763,   764,
     765,   731,   732,     0,  -745,   741,   742,   743,   744,   745,
       0,     0,   748,   749,   750,   751,     0,   753,   754,   755,
     756,     0,     0,     0,     0,   757,     0,   759,   760,     0,
       0,     0,     0,   763,   764,   765,   731,   732,     0,     0,
       0,     0,     0,     0,   771,     0,   772,   773,   774,   775,
     776,   777,   778,   779,   780,   781,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   782,   783,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   771,
       0,   772,   773,   774,   775,   776,   777,   778,   779,   780,
     781,     0,     0,     0,     0,   741,   742,   743,   744,   745,
     782,   783,   748,   749,   750,   751,     0,   753,   754,   755,
     756,     0,     0,     0,     0,   757,     0,   759,   760,     0,
       0,     0,     0,   763,     0,   765,   731,   732,     0,     0,
     741,   742,   743,   744,   745,     0,     0,   748,   749,   750,
     751,     0,   753,   754,   755,   756,     0,     0,     0,     0,
     757,     0,   759,   760,   731,   732,     0,     0,   763,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   772,   773,   774,   775,   776,   777,   778,   779,   780,
     781,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     782,   783,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   772,   773,   774,   775,
     776,   777,   778,   779,   780,   781,     0,     0,     0,     0,
     741,   742,   743,   744,   745,   782,   783,   748,   749,   750,
     751,     0,   753,   754,   755,   756,     0,     0,     0,     0,
     757,     0,   759,   760,     0,     0,     0,     0,   741,   742,
     743,   744,   745,     0,     0,   748,   749,   750,   751,     0,
     753,   754,   755,   756,     0,     0,     0,     0,   757,     0,
     759,   760,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   968,     0,     0,   772,   773,   774,   775,
     776,   777,   778,   779,   780,   781,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   782,   783,     0,     0,     0,
       0,     0,     0,     0,   972,   773,   774,   775,   776,   777,
     778,   779,   780,   781,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   782,   783,   316,   317,   318,     0,   320,
     321,   322,   323,   324,   463,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,     0,   338,   339,   340,
       0,     0,   343,   344,   345,   346,   316,   317,   318,     0,
     320,   321,   322,   323,   324,   463,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,     0,   338,   339,
     340,     0,     0,   343,   344,   345,   346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1235,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   969,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   970,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   973,     0,     0,     0,   181,     0,     0,     0,
       0,   316,   317,   318,   974,   320,   321,   322,   323,   324,
     463,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,     0,   338,   339,   340,     0,     0,   343,   344,
     345,   346,   182,     0,   183,     0,   184,   185,   186,   187,
     188,     0,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,     0,   200,   201,   202,     0,     0,   203,
     204,   205,   206,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   207,   208,
       0,     0,     0,     0,     0,     0,     0,  1236,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     209
};

static const yytype_int16 yycheck[] =
{
      14,    15,   211,   482,   621,   646,   375,   570,   161,   709,
     590,   711,   382,   227,   388,   589,   584,   689,   586,   903,
     588,   375,   537,   471,   472,   849,    80,  1169,     4,    32,
       7,   443,   811,   687,   813,   689,   815,   626,  1363,    19,
      52,    19,    21,   417,    14,    15,   561,   919,    45,    63,
      64,    65,   103,   925,    56,   467,    52,    18,    19,  1399,
      62,    32,    19,  1512,    19,  1514,   720,   141,   443,  1518,
    1519,    19,     6,    19,    19,  1422,   168,    56,    20,    21,
     142,   143,   144,   125,   125,  1521,   227,   130,  1551,   443,
     104,   105,   106,   107,   168,   214,   215,   686,   156,   688,
    1563,   690,   125,   180,   125,   178,   480,   481,   105,   698,
     168,   178,   135,   467,   135,    49,   153,   161,   707,   186,
     141,   158,   214,   158,     0,   101,   102,   502,  1577,   124,
       6,  1456,   127,   128,  1481,  1482,   213,   180,   212,  1602,
    1603,   214,   215,  1579,   186,   187,   187,   189,   301,   190,
     192,   213,  1499,    29,   212,    31,   178,    33,   858,   373,
     156,   676,   574,    39,   187,   219,   187,   218,   180,   192,
      14,    15,   168,    49,   116,   117,  1516,   179,   215,    55,
     215,   881,   124,  1325,   126,   127,   128,   129,   178,   192,
     125,   161,   134,   180,   894,   849,   166,   897,   168,   189,
     135,   171,   179,    79,    20,    21,   186,   168,   186,  1556,
    1557,    49,   125,   227,   209,   210,    61,   193,   593,   198,
     574,   192,   135,     4,     5,   101,   102,   639,   168,   186,
     209,   186,   373,   210,    20,    21,   212,   446,   186,   210,
     186,   186,   212,    24,   150,   216,   141,   388,   141,    30,
     304,   460,   187,    91,   153,   150,   178,   669,   186,   158,
     155,  1546,   168,   638,   639,   140,  1150,   209,   210,   127,
     128,   645,   212,   168,   187,    46,   417,  1101,   419,   150,
     190,   729,   125,   211,   194,   639,    67,    68,   663,  1431,
     212,   666,   135,  1165,   668,    66,   189,   168,   193,   187,
     116,   117,   956,   178,   180,   141,   127,   128,   124,  1594,
     126,   127,   128,   129,   213,   669,   215,   161,   134,   186,
     101,   102,   166,   199,   168,   186,  1210,   171,   141,   186,
     116,   117,   168,   186,   210,   156,   350,   168,   124,   480,
     481,   127,   128,   129,   187,   720,   125,   168,   134,   180,
     211,   209,   210,   193,   211,   944,   135,   727,   211,   373,
     141,   575,   576,   577,   378,   579,   580,    33,   150,   583,
     125,   585,   212,   587,   388,   141,   158,   158,   159,   160,
     135,   197,   198,   199,   150,   797,   168,   168,   209,   210,
     168,   169,   170,   209,   210,   186,    62,   178,   966,   183,
     184,   186,   168,   417,   180,   419,     4,     5,   187,     7,
     785,    56,   193,    63,    64,    65,   625,    62,   194,   180,
     211,   199,   183,   209,   210,   439,   440,   211,   213,   210,
     212,   843,   187,   186,   575,   576,   577,    35,   579,   580,
     955,   650,   583,   797,   585,  1224,   587,  1101,   589,   186,
     211,   930,  1324,   186,   104,   105,   106,   107,   186,   186,
     213,   940,   186,   911,  1336,   186,   480,   481,   134,   186,
     844,   845,   186,   186,   849,   141,   213,  1094,   186,   189,
     213,   180,   186,   857,   211,   213,   695,   211,   142,   843,
     144,   193,   213,  1165,   180,   194,   213,   193,   193,   213,
     213,   187,   168,  1083,   645,   213,   715,  1081,  1180,   213,
     212,  1165,   886,   887,  1168,   889,   212,   212,   186,   893,
     189,   895,   896,   193,   898,   194,  1180,   668,   976,   977,
     978,   180,    74,   180,   180,   168,    78,   203,    56,  1318,
     212,   916,   212,   212,    62,   194,   212,   194,   194,   924,
      92,    93,    20,    21,   189,    97,    98,    99,   100,   194,
     180,   575,   576,   577,   180,   579,   580,   168,   180,   583,
     190,   585,   180,   587,  1353,   589,   189,   212,   194,   180,
     189,   194,   194,  1151,    32,   194,   194,   172,   173,    11,
     731,   732,   734,   735,   736,   737,   738,   739,   740,   212,
      22,    23,   168,   212,   746,   747,   933,   821,   935,   156,
     752,    59,    60,   161,   180,   829,   757,   631,   832,   761,
     762,   168,   172,   173,   766,   767,   768,   841,   770,   156,
     156,   645,   156,   156,   848,  1203,   209,    56,   168,   853,
     793,   168,   168,    62,   168,   168,   114,   115,   116,   117,
     118,    56,   189,   121,   668,   192,   124,    62,   126,   127,
     128,   129,   168,   180,  1305,    56,   134,   180,   136,   137,
     879,    62,   172,  1452,   122,    56,   186,   125,   126,   189,
     821,    62,   192,   179,    56,   168,   182,   135,   829,   185,
      62,   832,   168,   180,   189,   180,   180,   192,   712,   186,
     841,   186,   186,   844,   845,   168,  1360,   848,   180,   180,
     168,  1328,   853,   168,   186,   186,   857,  1417,   178,   933,
      56,   935,    86,    87,  1099,   193,   194,   195,   196,   197,
     198,   199,   180,   168,   169,   170,  1110,  1111,  1112,   187,
     188,   209,   210,   180,   192,   886,   887,   195,   889,   186,
     180,   105,   893,   142,   895,   896,    34,   898,   172,   190,
    1134,   190,   210,   194,   187,   194,   190,   178,   216,   190,
     194,    32,   190,   194,   190,   190,   194,   791,   194,   194,
     190,   190,   190,  1158,   194,   194,   194,   142,   143,   144,
    1358,   178,   933,   178,   935,   172,   173,   174,    59,    60,
      34,   179,    20,    21,  1179,   179,   180,   821,   182,   212,
    1185,   178,   168,  1430,   215,   829,   168,  1192,   832,   168,
     168,  1196,   172,   173,   174,   175,  1195,   841,  1202,   211,
     844,   845,    21,   168,   848,   179,   180,   181,   211,   853,
     178,  1195,   189,   857,   189,   178,  1500,    10,    11,    12,
    1491,   212,    42,   193,   193,   212,  1070,  1232,   190,  1559,
    1560,   122,  1562,   193,   125,   126,   193,   193,   193,  1244,
     212,   193,   886,   887,   135,   889,   193,   193,   193,   893,
     193,   895,   896,   375,   898,   168,   135,   168,   212,   212,
     211,   168,   168,   193,  1035,  1458,   210,  1597,   116,   117,
     193,   193,   212,   168,  1558,   193,   124,   193,   126,   127,
     128,   129,    20,    21,   212,   161,   134,   180,   212,   933,
     193,   935,   193,   193,   193,   212,   187,   188,   212,  1070,
     212,   192,   212,   212,   195,   193,    36,   180,   180,     9,
    1081,  1595,   180,   215,  1319,   180,    65,   180,   180,   210,
     180,   443,   212,   210,  1571,   216,   179,   212,    42,   213,
    1439,   168,   193,  1442,   168,   180,   211,   179,   179,  1110,
    1111,  1112,  1535,  1536,  1349,   467,   189,   195,   196,   197,
     198,   199,   180,   180,   179,   212,    42,   212,   193,   130,
     193,   209,   210,  1134,   193,  1204,    13,  1371,   186,   161,
     187,  1655,   186,   189,   178,   215,   114,   115,   116,   117,
     118,    12,   158,   121,   122,   123,   124,   186,   126,   127,
     128,   129,  1585,   168,  1399,     7,   134,   194,   136,   137,
     168,   213,   168,   168,   168,   212,   186,   186,   211,   179,
     179,   213,   212,   168,   212,   193,   193,   213,   212,     1,
     179,   212,   212,   212,   212,   212,  1070,    66,   180,   168,
     213,  1202,   194,   194,   194,  1279,    42,  1081,   211,   213,
     168,   213,   168,   168,   168,   212,   168,  1286,   193,   180,
     213,   168,   574,  1457,   192,   193,   194,   195,   196,   197,
     198,   199,   213,   213,   212,   211,  1110,  1111,  1112,   211,
     375,   209,   210,   213,   168,   212,   212,   212,   212,   212,
     385,   180,    32,   212,   211,  1490,   168,   168,   211,   394,
    1134,  1496,   212,   211,   213,    69,    32,   212,   194,   404,
     212,   212,   212,   212,   212,   212,   212,   194,  1279,   414,
     213,  1516,   213,   180,   194,   212,  1355,   639,   423,  1524,
     212,   212,   212,    59,    60,   213,    52,   211,   433,  1373,
    1534,  1370,   180,   179,   213,   179,   213,   179,   443,   211,
     213,   186,    32,   448,   213,   450,   179,   669,   213,   213,
     213,   470,   471,   472,   459,   213,   213,   211,  1202,   211,
     211,    79,   467,   468,   469,   697,     1,    44,   133,    59,
      60,   212,    82,   213,  1449,  1580,   213,   482,  1450,   791,
     942,  1450,   225,  1450,  1450,   490,   122,  1426,   493,  1450,
     126,   496,   497,   498,   499,   500,     1,   962,  1426,  1368,
    1371,   520,  1373,  1391,   509,   520,  1256,  1478,  1394,  1479,
      52,   733,   734,   735,   736,   737,   738,   739,   740,   427,
     559,  1558,  1437,   689,   746,   747,  1470,  1175,   547,   439,
     752,    -1,   122,   439,    -1,  1279,   126,    -1,    -1,   761,
     762,    -1,    -1,    32,   766,   767,   768,    -1,   770,    -1,
      -1,    -1,   188,    -1,    -1,    -1,   192,    -1,   194,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   574,
      59,    60,    -1,    -1,   210,   797,    -1,    -1,    -1,    -1,
     216,    -1,    -1,    -1,    -1,    -1,  1457,    -1,    -1,    -1,
     180,    -1,    -1,  1532,    -1,    -1,    -1,   602,   188,  1470,
      -1,    -1,   192,    -1,   194,   195,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     210,   843,    -1,    -1,   643,    -1,   216,  1371,    -1,  1373,
      -1,    -1,    -1,   122,   639,    -1,    -1,   126,    -1,    -1,
      -1,   646,   647,    -1,   649,    -1,    -1,    -1,    -1,   654,
      -1,    -1,   671,    -1,    -1,   660,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1534,   669,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   680,   681,   682,   683,   684,
     685,    -1,   687,    -1,   689,    -1,    -1,    -1,    -1,  1628,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,    20,    21,   192,    -1,   194,   195,    -1,    -1,    -1,
     729,    -1,    -1,  1457,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   210,    -1,    -1,    -1,    -1,  1470,   216,   733,   734,
     735,   736,   737,   738,   739,   740,   741,   742,    -1,    -1,
     745,   746,   747,   748,   749,   750,    -1,   752,   753,    -1,
     755,   756,   757,   758,   759,   760,   761,   762,   763,   764,
     765,   766,   767,   768,   769,   770,   771,   772,   773,   774,
     775,   776,   777,   778,   779,   780,   781,  1521,   783,    -1,
     799,    -1,   787,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1534,    -1,   797,    -1,    -1,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   836,    -1,    -1,    -1,  1579,    -1,    -1,   843,    -1,
      -1,    -1,    -1,    -1,    -1,   850,    -1,   852,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,   861,   862,   863,   864,
     865,   866,   867,   868,   869,   870,   871,   872,   873,   874,
     875,   876,   877,   878,   193,   194,   195,   196,   197,   198,
     199,    -1,   901,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,   911,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   914,
     915,    -1,   122,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,   928,    -1,   930,    -1,    59,    60,    -1,
      -1,    -1,   937,    -1,    -1,   940,    -1,    -1,    -1,    -1,
     945,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   954,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   976,   977,   978,
      -1,    -1,   981,    -1,   983,    -1,   985,    -1,   987,    -1,
     989,    -1,   991,  1195,   993,    -1,   995,    -1,   188,    -1,
      -1,  1000,   192,  1002,   194,   195,    -1,    -1,    -1,  1008,
     122,    -1,    -1,     9,   126,    -1,    -1,    -1,    -1,    -1,
     210,  1020,    -1,  1022,    20,    21,   216,    -1,  1027,    -1,
    1029,    -1,  1031,    -1,    -1,  1034,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1048,    -1,    -1,  1065,  1052,    32,    -1,
      -1,    -1,    -1,    32,    -1,    -1,   188,    -1,    -1,    -1,
     192,    -1,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    59,    60,  1096,   210,    -1,
      59,    60,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,   139,    -1,    -1,   142,   143,   144,   145,
     146,   147,   148,  1138,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,   126,   122,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1160,  1161,  1162,    -1,    -1,
      -1,  1166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1174,
    1175,   187,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
    1195,    32,    -1,   209,   210,  1200,  1201,    -1,   214,   215,
      -1,    -1,  1207,    -1,   188,    -1,    -1,  1212,   192,   188,
     194,   195,    -1,   192,    -1,   194,   195,    -1,    59,    60,
      -1,  1226,    -1,  1228,    -1,    -1,   210,    -1,  1233,    -1,
      -1,   210,   216,    -1,  1239,    -1,    -1,   216,  1243,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    32,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    59,    60,    -1,  1283,    -1,
      48,   122,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
    1305,    -1,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,   122,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,   188,    -1,    -1,
      -1,   192,    -1,   194,   195,    -1,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,   210,
      -1,    -1,    -1,   141,  1379,   216,  1381,    -1,    -1,    -1,
      -1,  1386,    -1,    -1,    -1,    -1,   154,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     168,    -1,    -1,  1408,   188,  1410,    -1,    -1,   192,    -1,
     194,   195,    -1,  1418,  1419,  1420,    -1,    -1,    -1,    -1,
    1425,    -1,    -1,    -1,  1429,    -1,   210,    -1,  1433,  1434,
    1435,  1436,   216,    -1,  1439,  1440,    -1,  1442,  1443,    -1,
      -1,    -1,    -1,    -1,    -1,  1450,   214,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1461,    -1,    -1,    -1,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,  1491,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,  1506,  1507,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1520,    -1,    -1,    -1,    -1,
    1525,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1537,  1538,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,  1552,    -1,  1554,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1581,    -1,    -1,    -1,
      -1,  1586,  1587,    -1,    -1,    -1,     1,    -1,    -1,     4,
       5,     6,    -1,     8,     9,    10,    -1,    12,    -1,    14,
      15,    16,    17,    18,    -1,  1610,    -1,  1612,  1613,    24,
      25,    26,    27,    28,    -1,    30,    -1,  1622,    -1,    -1,
      -1,    -1,    37,    38,    -1,    -1,    41,    -1,    43,    44,
      -1,    -1,    47,    -1,    49,    50,    51,    -1,    53,    54,
      -1,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    -1,    97,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,    -1,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,    -1,    -1,    -1,
      -1,    -1,    -1,   178,   179,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,    -1,   199,    -1,    -1,   202,   203,    -1,
      -1,    -1,    -1,    -1,    -1,   210,    -1,   212,     1,   214,
     215,     4,     5,     6,    -1,     8,     9,    10,    -1,    12,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    25,    26,    27,    28,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    38,    -1,    -1,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    -1,    57,    58,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,    72,
      -1,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    97,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,
     153,    32,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,    -1,
      -1,    -1,    -1,    -1,    -1,   178,   179,   180,    59,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,
     203,    -1,    -1,     4,     5,    -1,    -1,   210,    -1,   212,
      -1,   214,   215,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,   122,    -1,    44,    -1,   126,    47,    -1,    -1,    50,
      -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    59,    60,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    94,    -1,    -1,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,   188,    -1,    -1,
      -1,   192,    -1,   194,   195,   116,   117,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,   126,    -1,    -1,    -1,   210,
      -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
     151,   152,   153,    32,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      59,    60,    -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,
      -1,   192,    -1,    -1,   195,   196,   197,    -1,   199,    -1,
      -1,   202,   203,    -1,    -1,    -1,     4,     5,    -1,   210,
      -1,   212,    -1,   214,   215,   216,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,   122,    -1,    -1,    44,   126,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    59,    60,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,   188,
      -1,    -1,    -1,   192,    -1,   194,   195,    -1,   116,   117,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,   126,    -1,
      -1,   210,    -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    -1,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,   188,    -1,    -1,    -1,   192,    -1,   194,   195,
      -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   210,    -1,    -1,    -1,    -1,    -1,
     216,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,    32,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    59,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,    -1,    -1,
     195,   196,   197,    -1,   199,    -1,    -1,   202,   203,    -1,
      -1,    -1,     4,     5,    -1,   210,    -1,   212,    -1,   214,
     215,   216,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,   122,
      -1,    -1,    44,   126,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,   188,    -1,    -1,    -1,   192,
      -1,   194,   195,    -1,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,   210,    -1,   131,
     132,   133,    -1,   216,    -1,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,
     152,   153,    32,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,    -1,   199,    -1,    -1,
     202,   203,    -1,    -1,     4,     5,    -1,    -1,   210,    -1,
     212,    -1,   214,   215,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,   122,    -1,    44,    -1,   126,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    -1,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,   188,    -1,
      -1,    -1,   192,    -1,   194,   195,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
     210,   131,   132,   133,    -1,    -1,   216,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,   151,   152,   153,    32,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,    -1,   199,
      -1,    -1,   202,   203,    -1,    -1,     4,     5,    -1,    -1,
     210,    -1,   212,    -1,   214,   215,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    25,    26,    27,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,   122,    -1,    44,    -1,   126,    47,
      -1,    -1,    50,    51,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    -1,    97,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
     188,    -1,    -1,    -1,   192,    -1,   194,   195,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,    -1,    -1,    -1,    -1,    -1,   216,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,   151,   152,   153,    32,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
      -1,   199,    -1,    -1,   202,   203,    -1,    -1,     4,     5,
      -1,    -1,   210,    -1,   212,    -1,   214,   215,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,   122,    -1,    44,    -1,
     126,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    -1,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    94,    -1,
      -1,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,   188,    -1,    -1,    -1,   192,    -1,   194,   195,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   210,   131,   132,   133,    -1,    -1,
     216,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,   150,   151,   152,   153,    32,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    59,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,    -1,   199,    -1,    -1,   202,   203,    -1,    -1,
       4,     5,    -1,    -1,   210,    -1,   212,    -1,   214,   215,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    26,    -1,    -1,    32,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   122,    -1,
      44,    -1,   126,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    -1,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,   192,    -1,
     194,   195,   116,   117,    -1,   122,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,   210,   131,   132,   133,
      -1,    -1,   216,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,
      -1,   155,    32,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    59,
      60,   188,    -1,    -1,    -1,   192,    -1,   194,   195,    -1,
      -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,   203,
      -1,    -1,    -1,   210,     4,     5,   210,    -1,   212,   216,
     214,   215,    12,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,   122,    -1,    44,    -1,   126,    47,    48,    -1,
      50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    -1,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,   188,    -1,
      -1,    -1,   192,    -1,    -1,   195,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     210,    -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,   151,   152,   153,    -1,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,    -1,   199,
      -1,    -1,   202,   203,    -1,    -1,    -1,    -1,     4,     5,
     210,   211,   212,    -1,   214,   215,    12,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    47,    48,    -1,    50,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    -1,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    94,    -1,
      -1,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,    -1,   199,    -1,    -1,   202,   203,    -1,    -1,
      -1,    -1,     4,     5,   210,    -1,   212,    -1,   214,   215,
      12,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    47,    48,    -1,    50,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,
     152,   153,    -1,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,    -1,   199,    -1,    -1,
     202,   203,    -1,    -1,    -1,    -1,     4,     5,   210,   211,
     212,    -1,   214,   215,    12,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      48,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,   151,   152,   153,    -1,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
      -1,   199,    -1,    -1,   202,   203,    -1,    -1,     4,     5,
      -1,    -1,   210,    -1,   212,    -1,   214,   215,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    -1,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    94,    -1,
      -1,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,    -1,   199,    -1,    -1,   202,   203,    -1,    -1,
       4,     5,    -1,    -1,   210,    -1,   212,   213,   214,   215,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    69,    70,    71,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    -1,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,   203,
      -1,    -1,     4,     5,    -1,    -1,   210,    -1,   212,    -1,
     214,   215,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,
     152,   153,    -1,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,    -1,   199,    -1,    -1,
     202,   203,    -1,    -1,     4,     5,    -1,    -1,   210,   211,
     212,    -1,   214,   215,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      60,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    -1,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,   151,   152,   153,    -1,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,    -1,   199,
      -1,    -1,   202,   203,    -1,    -1,     4,     5,    -1,    -1,
     210,    -1,   212,    -1,   214,   215,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,   151,   152,   153,    -1,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
      -1,   199,    -1,    -1,   202,   203,    -1,    -1,    -1,    -1,
       4,     5,   210,    -1,   212,    -1,   214,   215,    12,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    -1,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,
      -1,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,   203,
      -1,    -1,     4,     5,    -1,    -1,   210,    -1,   212,    -1,
     214,   215,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,
     152,   153,    -1,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,    -1,   199,    -1,    -1,
     202,   203,    -1,    -1,    -1,    -1,     4,     5,   210,    -1,
     212,    -1,   214,   215,    12,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,   151,   152,   153,    -1,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
      -1,   199,    -1,    -1,   202,   203,    -1,    -1,     4,     5,
      -1,    -1,   210,    -1,   212,    -1,   214,   215,    14,    15,
      16,    17,    18,    -1,    -1,    21,    -1,    -1,    24,    -1,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    -1,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    94,    -1,
      -1,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,   150,   151,   152,   153,    -1,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,    -1,   199,    -1,    -1,   202,   203,    -1,    -1,
       4,     5,    -1,    -1,   210,    -1,   212,    -1,   214,   215,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    -1,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,
      -1,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,   203,
      -1,    -1,     4,     5,    -1,    -1,   210,    -1,   212,   213,
     214,   215,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,
     152,   153,    -1,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,    -1,   199,    -1,    -1,
     202,   203,    -1,    -1,     4,     5,    -1,    -1,   210,    -1,
     212,    -1,   214,   215,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    -1,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,   151,   152,   153,    -1,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,    -1,   199,
      -1,    -1,   202,   203,    -1,    -1,     4,     5,    -1,    -1,
     210,    -1,   212,   213,   214,   215,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,   151,   152,   153,    -1,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
      -1,   199,    -1,    -1,   202,   203,    -1,    -1,     4,     5,
      -1,    -1,   210,    -1,   212,   213,   214,   215,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    -1,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    94,    -1,
      -1,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,   150,   151,   152,   153,    -1,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,    -1,   199,    -1,    -1,   202,   203,    -1,    -1,
       4,     5,    -1,    -1,   210,    -1,   212,   213,   214,   215,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    -1,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,
      -1,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,   203,
      -1,    -1,     4,     5,    -1,    -1,   210,    -1,   212,   213,
     214,   215,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,
     152,   153,    -1,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,    -1,   199,    -1,    -1,
     202,   203,    -1,    -1,     4,     5,    -1,    -1,   210,    -1,
     212,   213,   214,   215,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    -1,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,   151,   152,   153,    -1,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,    -1,   199,
      -1,    -1,   202,   203,    -1,    -1,     4,     5,    -1,    -1,
     210,    -1,   212,   213,   214,   215,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,   151,   152,   153,    -1,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
      -1,   199,    -1,    -1,   202,   203,    -1,    -1,     4,     5,
      -1,    -1,   210,    -1,   212,    -1,   214,   215,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    -1,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    94,    -1,
      -1,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,   150,   151,   152,   153,    -1,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,    -1,   199,    -1,    -1,   202,   203,    -1,    -1,
       4,     5,    -1,    -1,   210,    -1,   212,    -1,   214,   215,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    -1,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,
      -1,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,   203,
      -1,    -1,     4,     5,    -1,    -1,   210,    -1,   212,    -1,
     214,   215,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,
     152,   153,    -1,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,    -1,   199,    -1,    -1,
     202,   203,    -1,    -1,     4,     5,    -1,    -1,   210,    -1,
     212,    -1,   214,   215,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    -1,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,   151,   152,   153,    -1,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,    -1,    12,    -1,    -1,    -1,    -1,   178,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,   195,   196,   197,    -1,   199,
      -1,    40,   202,   203,    -1,    -1,    -1,    -1,    -1,    48,
     210,    -1,   212,    -1,   214,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      40,    -1,   141,    -1,    -1,    -1,    -1,    -1,    48,    20,
      21,    -1,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,   168,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,    -1,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      -1,   141,    -1,   114,   115,   116,   117,    48,    -1,    -1,
      -1,    -1,    -1,   124,   154,   126,   127,   128,   129,    -1,
      -1,    -1,    63,   134,    -1,   136,   137,    -1,   168,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,   168,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,   180,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,   214,    70,    71,    72,    73,    74,    75,
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
      97,    98,    99,   100,   101,   102,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   214,    20,    21,
     134,   135,   136,   137,   138,   139,    -1,    -1,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,    -1,
     214,   215,    -1,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,    20,    21,
      -1,    -1,   134,   135,   136,   137,   138,   139,    -1,    -1,
     142,   143,   144,   145,   146,   147,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,
      -1,    -1,    -1,    -1,    -1,   187,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,    -1,   214,   215,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,   139,    -1,    -1,
     142,   143,   144,   145,   146,   147,   148,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    20,    21,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,   180,    -1,
      -1,   148,    -1,    -1,    -1,   187,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,    -1,   214,   215,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    20,    21,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    20,    21,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    20,    21,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    20,    21,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    20,    21,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    20,    21,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    20,    21,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    20,    21,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    20,    21,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    20,    21,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    20,    21,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    20,    21,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    20,    21,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    20,    21,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    20,    21,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    20,    21,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,   114,   115,   116,   117,   118,    20,    21,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,   211,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,   211,
      -1,   114,   115,   116,   117,   118,    20,    21,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,   211,    -1,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,   114,   115,   116,   117,   118,
      20,    21,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,   211,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,   211,    -1,   114,   115,   116,   117,   118,    -1,
      -1,   121,   122,   123,   124,    -1,   126,   127,   128,   129,
      20,    21,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,
      -1,    -1,   142,   143,   144,    -1,    -1,    37,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,
     210,   211,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,    -1,
      -1,    -1,    -1,    -1,    -1,   135,   136,   137,   138,   139,
      -1,    -1,   142,   143,   144,   145,   146,   147,   148,   114,
     115,   116,   117,   118,    20,    21,   121,   122,   123,   124,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,   188,    -1,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
      -1,    -1,   202,   203,    -1,    -1,    -1,    -1,    -1,   209,
     210,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   114,   115,
     116,   117,   118,    20,    21,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,   140,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,   140,    -1,   142,   143,   144,    20,    21,
      -1,   148,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    20,    21,    -1,   148,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,   209,   210,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    20,    21,    -1,   148,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,   180,    -1,
      -1,   148,    20,    21,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,   176,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,   209,   210,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,    20,    21,    -1,    -1,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,   180,   142,   143,   144,    20,    21,    -1,
     148,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,    -1,   114,   115,   116,   117,
     118,   209,   210,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    20,    21,    -1,
     148,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,   180,    -1,    -1,   148,    20,    21,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,    -1,    -1,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
      -1,   114,   115,   116,   117,   118,   209,   210,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,    20,    21,    -1,    -1,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,   180,   142,   143,
     144,    20,    21,    -1,   148,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,   118,   209,   210,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    20,    21,    -1,   148,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,
     209,   210,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,   144,    20,    21,    -1,    -1,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    20,    21,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,   118,   209,   210,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    18,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    -1,    97,    98,    99,   100,    70,    71,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    -1,    97,    98,    99,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    70,    71,    72,   168,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,    70,    -1,    72,    -1,    74,    75,    76,    77,
      78,    -1,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    -1,    97,
      98,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     168
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   218,     0,     6,    29,    31,    33,    39,    49,    55,
      79,   101,   102,   180,   199,   210,   219,   222,   228,   230,
     231,   236,   266,   270,   303,   381,   388,   392,   403,   447,
     452,   457,    18,    19,   168,   258,   259,   260,   161,   237,
     238,   168,   169,   170,   199,   232,   233,   234,   178,   389,
     168,   214,   221,    56,    62,   384,   384,   384,   141,   168,
     289,    33,    62,   134,   203,   212,   262,   263,   264,   265,
     289,   180,     4,     5,     7,    35,   400,    61,   379,   187,
     186,   189,   186,   233,    21,    56,   198,   209,   235,   384,
     385,   387,   385,   379,   458,   448,   453,   168,   141,   229,
     264,   264,   264,   212,   142,   143,   144,   186,   211,    56,
      62,   271,   273,    56,    62,   390,    56,    62,   401,    56,
      62,   380,    14,    15,   161,   166,   168,   171,   212,   224,
     259,   161,   238,   168,   232,   232,   168,   180,   179,   385,
     180,    56,    62,   220,   168,   168,   168,   168,   172,   227,
     213,   260,   264,   264,   264,   264,   274,   168,   391,   404,
     178,   382,   172,   173,   223,    14,    15,   161,   166,   168,
     224,   256,   257,   235,   386,   180,   459,   449,   454,   172,
     213,    34,    70,    72,    74,    75,    76,    77,    78,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      92,    93,    94,    97,    98,    99,   100,   116,   117,   168,
     269,   272,   178,   189,   105,   398,   399,   377,   158,   215,
     261,   353,   172,   173,   174,   186,   213,   187,   178,   178,
     178,    20,    21,    37,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   126,   127,   128,   135,   136,   137,   138,   139,
     142,   143,   144,   145,   146,   147,   148,   188,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   202,   203,
     209,   210,    34,    34,   212,   267,   178,   275,   393,    74,
      78,    92,    93,    97,    98,    99,   100,   408,   168,   405,
     179,   378,   260,   259,   215,   150,   168,   375,   376,   256,
      18,    24,    30,    40,    48,    63,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   154,
     214,   289,   407,   409,   410,   413,   418,   446,   460,   450,
     455,   168,   168,   168,   211,    21,   168,   211,   153,   213,
     353,   363,   364,   189,   268,   278,   383,   178,   189,   397,
     178,   402,   353,   211,   259,   212,    42,   186,   189,   192,
     374,   193,   193,   193,   212,   193,   193,   212,   193,   193,
     193,   193,   193,   193,   212,   289,    32,    59,    60,   122,
     126,   188,   192,   195,   210,   216,   417,   190,   412,   367,
     370,   168,   135,   212,     6,    49,   302,   180,   213,   446,
       1,     4,     5,     8,     9,    10,    12,    14,    15,    16,
      17,    18,    24,    25,    26,    27,    28,    30,    37,    38,
      41,    43,    44,    47,    50,    51,    53,    54,    57,    58,
      64,    67,    68,    79,   101,   102,   103,   104,   116,   117,
     131,   132,   133,   149,   150,   151,   152,   153,   155,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     169,   170,   171,   178,   179,   180,   195,   196,   197,   202,
     203,   210,   212,   214,   215,   226,   228,   239,   240,   243,
     246,   247,   249,   251,   252,   253,   254,   276,   277,   279,
     283,   288,   289,   290,   294,   295,   296,   297,   298,   299,
     300,   301,   303,   307,   308,   315,   318,   321,   326,   329,
     330,   332,   333,   334,   336,   341,   344,   345,   352,   407,
     462,   476,   487,   491,   504,   507,   168,   180,   394,   395,
     289,   359,   376,   211,    64,   169,   283,   345,   168,   168,
     418,   125,   135,   187,   373,   419,   424,   426,   345,   428,
     422,   168,   414,   430,   432,   434,   436,   438,   440,   442,
     444,   345,   193,   212,    32,   192,    32,   192,   210,   216,
     211,   345,   210,   216,   418,   168,   180,   461,   168,   180,
     365,   446,   451,   168,   180,   368,   456,   345,   150,   168,
     372,   406,   363,   193,   193,   345,   250,   193,   291,   409,
     462,   212,   289,   193,     4,   101,   102,   193,   212,   125,
     288,   319,   330,   345,   275,   193,   212,    60,   345,   212,
     345,   168,   193,   193,   212,   180,   193,   161,    57,   345,
     212,   275,   193,   212,   193,   193,   212,   193,   193,   125,
     288,   345,   345,   345,   215,   275,   321,   325,   325,   325,
     212,   212,   212,   212,   212,   212,    12,   418,    12,   418,
      12,   345,   486,   502,   193,   345,   193,   225,    12,   486,
     503,    36,   345,   345,   345,   345,   345,    12,    48,   319,
     345,   319,   215,   180,   180,   345,     9,   321,   327,   168,
     212,   180,   180,   180,   180,   180,    65,   304,   266,   130,
     180,    20,    21,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   134,   135,   136,
     137,   138,   139,   142,   143,   144,   145,   146,   147,   148,
     187,   188,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   209,   210,   327,   212,   180,   187,   179,   383,
     179,   210,   266,   360,   213,    42,   180,   373,   288,   345,
     446,   446,   416,   446,   213,   446,   446,   213,   168,   411,
     446,   267,   446,   267,   446,   267,   365,   366,   368,   369,
     213,   421,   280,   319,   211,   211,   345,   180,   179,   189,
     180,   179,   189,   180,   179,   213,   212,    42,   125,   186,
     187,   189,   192,   371,   477,   479,   275,   406,   292,   212,
     289,   193,   212,   316,   193,   193,   193,   498,   319,   288,
     319,   186,   106,   107,   108,   109,   110,   111,   112,   113,
     119,   120,   125,   138,   139,   145,   146,   147,   187,    13,
     418,   503,   345,   345,   275,   187,   309,   311,   345,   313,
     189,   161,   345,   500,   319,   483,   488,   319,   481,   418,
     288,   345,   215,   266,   345,   345,   345,   345,   345,   345,
     406,    52,   156,   168,   195,   210,   212,   345,   463,   466,
     469,   485,   490,   406,   212,   466,   490,   406,   140,   179,
     180,   181,   470,   284,   275,   286,   174,   175,   223,   406,
     186,   506,   178,   406,    12,   186,   506,   506,   150,   155,
     193,   289,   335,   275,   248,   328,   210,   213,   319,   463,
     465,   158,   212,   306,   376,   158,   324,   325,    18,   156,
     168,   407,    18,   156,   168,   407,   131,   132,   133,   276,
     331,   345,   331,   345,   331,   345,   331,   345,   331,   345,
     331,   345,   331,   345,   331,   345,   345,   345,   345,   331,
     345,   331,   345,   345,   345,   345,   168,   331,   345,   345,
     156,   168,   345,   345,   345,   407,   345,   345,   345,   331,
     345,   331,   345,   345,   345,   345,   331,   345,   331,   345,
     331,   345,   345,   331,   345,    21,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   127,   128,   156,
     168,   209,   210,   342,   345,   213,   319,   345,   396,   265,
       7,   353,   358,   168,   288,   345,   180,   194,   194,   194,
     180,   194,   194,   180,   194,   194,   268,   194,   268,   194,
     268,   180,   194,   180,   194,   282,   446,   213,   211,   446,
     446,   345,   168,   168,   446,   288,   345,   418,   418,    19,
     446,    69,   319,   465,   475,   193,   345,   168,   345,   446,
     492,   494,   496,   418,   506,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   275,   180,   194,   506,   213,   255,   418,
     418,   213,   418,   213,   418,   506,   418,   418,   506,   418,
     194,   324,   213,   213,   213,   213,   213,   213,    19,   325,
     212,   135,   371,   210,   345,   213,   140,   186,   180,   469,
     183,   184,   211,   473,   186,   180,   183,   211,   472,    19,
     213,   469,   179,   182,   471,    19,   345,   179,   486,   282,
     282,   345,    19,   486,   179,   278,    19,   406,   211,   213,
     212,   212,   337,   339,    11,    22,    23,   241,   242,   345,
     266,   463,   186,   213,   213,   168,   305,   305,   125,   135,
     187,   192,   322,   323,   267,   193,   212,   193,   212,   325,
     325,   325,   212,   212,   211,    18,   156,   168,   407,   189,
     156,   168,   345,   212,   212,   156,   168,   345,     1,   211,
     213,   180,   179,   211,    56,    62,   356,    66,   357,   180,
     180,   420,   425,   427,   446,   429,   423,   168,   415,   431,
     194,   435,   194,   439,   194,   365,   443,   368,   445,   180,
     194,   213,    42,   371,   194,   194,   319,   194,   168,   465,
     213,   213,   213,   168,   213,   180,   194,   213,   194,   418,
     418,   418,   194,   213,   418,   212,   213,   345,   194,   194,
     194,   194,   213,   194,   194,   213,   194,   324,   267,   212,
     319,   345,   345,   345,   466,   469,   345,   156,   168,   463,
     473,   211,   345,   485,   211,   319,   466,   179,   182,   185,
     474,   211,   319,   194,   194,   176,   319,   179,   319,    19,
     345,   345,   418,   267,   275,   345,    11,   244,   324,   211,
     210,   186,   211,   213,   168,   168,   168,   168,   186,   211,
     268,   346,   345,   348,   345,   213,   319,   345,   193,   212,
     345,   212,   211,   345,   213,   319,   212,   211,   343,   180,
      46,   357,    45,   105,   354,   433,   437,   441,   446,   212,
     168,   345,   478,   480,   275,   293,   213,   180,   194,   168,
     212,   317,   194,   194,   194,   499,   194,   503,   310,   312,
     314,   501,   484,   489,   482,   212,   268,   213,   319,   180,
     213,   469,   473,   212,   135,   371,   180,   469,   211,   180,
     285,   287,   180,   180,   319,   213,   213,   194,   268,   275,
     245,   180,   267,   213,   463,   168,   211,   189,   374,   322,
     211,   140,   275,   320,   418,   213,   446,   213,   213,   213,
     350,   345,   345,   213,   213,   345,    32,   355,   354,   356,
     280,   212,   212,   168,   345,   194,   345,   493,   495,   497,
     212,   212,   506,   345,   345,   345,   212,    69,   475,   212,
     212,   213,   345,   320,   213,   345,   135,   371,   473,   345,
     345,   345,   345,   474,   486,   345,   212,   281,   486,   345,
     180,   338,   194,   242,    25,   104,   246,   295,   296,   297,
     299,   345,   268,   211,   189,   374,   418,   373,   125,   345,
     194,   194,   446,   213,   213,   213,   361,   355,   372,   213,
     475,   475,   194,   213,   212,   213,   212,   212,   212,   319,
     503,   213,   319,   475,   463,   464,   213,   180,   505,   345,
     345,   213,   505,   505,   280,   505,   505,   345,   335,   340,
     125,   125,   345,   275,   213,   418,   373,   373,   345,   345,
     347,   349,   194,   272,   362,   212,   463,   467,   468,   468,
     345,   345,   475,   475,   464,   506,   506,   506,   468,   213,
      52,   211,   135,   371,   179,   179,   213,   179,   211,   505,
     335,   345,   373,   345,   345,   180,   351,   180,   272,   463,
     186,   506,   213,   213,   213,   213,   468,   468,   213,   213,
     213,   213,   213,   345,   211,   345,   345,   211,   179,   211,
     345,   180,   180,   275,   213,   212,   213,   213,   180,   463,
     213
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
     259,   259,   260,   260,   261,   261,   262,   262,   262,   263,
     263,   264,   264,   264,   264,   264,   264,   264,   265,   265,
     266,   266,   267,   267,   267,   268,   268,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   270,   271,   271,   271,   272,   274,   273,   275,
     275,   276,   276,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   278,   278,   278,   279,   280,   280,
     281,   281,   282,   282,   284,   285,   283,   286,   287,   283,
     288,   288,   288,   288,   288,   289,   289,   289,   290,   290,
     292,   293,   291,   291,   294,   294,   294,   294,   294,   294,
     295,   296,   297,   297,   297,   298,   298,   298,   299,   299,
     300,   300,   300,   301,   302,   302,   302,   303,   303,   304,
     304,   305,   305,   306,   306,   306,   306,   307,   307,   309,
     310,   308,   311,   312,   308,   313,   314,   308,   316,   317,
     315,   318,   318,   318,   318,   318,   318,   319,   319,   320,
     320,   320,   321,   321,   321,   322,   322,   322,   322,   323,
     323,   324,   324,   325,   325,   326,   328,   327,   329,   329,
     329,   329,   329,   329,   329,   330,   330,   330,   330,   330,
     330,   330,   330,   330,   330,   330,   330,   330,   330,   330,
     330,   330,   330,   330,   331,   331,   331,   331,   332,   332,
     332,   332,   332,   332,   332,   332,   332,   332,   332,   332,
     332,   332,   332,   332,   332,   333,   333,   334,   334,   335,
     335,   336,   337,   338,   336,   339,   340,   336,   341,   341,
     341,   341,   342,   343,   341,   344,   344,   344,   344,   344,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   346,   347,   345,   345,
     345,   345,   348,   349,   345,   345,   345,   350,   351,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     353,   353,   353,   354,   354,   354,   355,   355,   356,   356,
     356,   357,   357,   358,   359,   360,   359,   361,   359,   362,
     359,   359,   363,   363,   364,   364,   365,   365,   366,   366,
     367,   367,   367,   368,   369,   369,   370,   370,   370,   371,
     371,   372,   372,   372,   372,   372,   372,   373,   373,   373,
     374,   374,   375,   375,   375,   375,   375,   376,   376,   376,
     376,   376,   377,   378,   377,   379,   379,   380,   380,   380,
     381,   382,   381,   383,   383,   383,   383,   384,   384,   384,
     386,   385,   387,   387,   388,   389,   388,   390,   390,   390,
     391,   393,   394,   392,   395,   396,   392,   397,   397,   398,
     398,   399,   400,   400,   401,   401,   401,   402,   402,   404,
     405,   403,   406,   406,   406,   406,   406,   407,   407,   407,
     407,   407,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   407,   408,   408,   408,   408,   408,   408,
     408,   408,   409,   410,   410,   410,   411,   411,   412,   412,
     412,   414,   415,   413,   416,   416,   417,   417,   418,   418,
     418,   418,   418,   418,   419,   420,   418,   418,   418,   421,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   422,   423,   418,   418,   424,   425,   418,
     426,   427,   418,   428,   429,   418,   418,   430,   431,   418,
     432,   433,   418,   418,   434,   435,   418,   436,   437,   418,
     418,   438,   439,   418,   440,   441,   418,   442,   443,   418,
     444,   445,   418,   446,   446,   446,   448,   449,   450,   451,
     447,   453,   454,   455,   456,   452,   458,   459,   460,   461,
     457,   462,   462,   462,   462,   462,   463,   463,   463,   463,
     463,   463,   463,   463,   464,   465,   466,   466,   467,   467,
     468,   468,   469,   469,   470,   470,   471,   471,   472,   472,
     473,   473,   473,   474,   474,   474,   475,   475,   476,   476,
     476,   476,   476,   476,   477,   478,   476,   479,   480,   476,
     481,   482,   476,   483,   484,   476,   485,   485,   485,   486,
     486,   487,   488,   489,   487,   490,   490,   491,   491,   491,
     492,   493,   491,   494,   495,   491,   496,   497,   491,   491,
     498,   499,   491,   491,   500,   501,   491,   502,   502,   503,
     503,   504,   504,   504,   504,   504,   505,   505,   506,   506,
     507,   507,   507,   507,   507,   507
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
       1,     5,     1,     3,     2,     3,     1,     1,     1,     1,
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
       1,     1,     3,     0,     1,     1,     3,     0,     4,     3,
       7,     2,     2,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     2,     2,     2,     0,     2,     2,     3,     0,     1,
       0,     3,     1,     3,     0,     0,     7,     0,     0,     7,
       3,     2,     2,     2,     1,     1,     3,     2,     2,     3,
       0,     0,     5,     1,     2,     5,     5,     5,     6,     2,
       1,     1,     1,     2,     3,     2,     2,     3,     2,     3,
       2,     2,     3,     4,     1,     1,     0,     1,     1,     1,
       0,     1,     3,     9,     8,     8,     7,     3,     3,     0,
       0,     7,     0,     0,     7,     0,     0,     7,     0,     0,
       6,     5,     8,    10,     5,     8,    10,     1,     3,     1,
       2,     3,     1,     1,     2,     2,     2,     2,     2,     1,
       3,     0,     4,     1,     6,     6,     0,     7,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     6,     8,     5,     6,     1,
       4,     3,     0,     0,     8,     0,     0,     9,     3,     4,
       5,     6,     0,     0,     5,     3,     4,     4,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       4,     4,     5,     4,     5,     3,     4,     1,     1,     2,
       4,     4,     7,     8,     3,     5,     0,     0,     8,     3,
       3,     3,     0,     0,     8,     3,     4,     0,     0,     9,
       4,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     2,     4,     1,     4,     4,     4,     4,     4,     1,
       6,     7,     6,     6,     7,     7,     6,     7,     6,     6,
       0,     4,     1,     0,     1,     1,     0,     1,     0,     1,
       1,     0,     1,     5,     0,     0,     4,     0,     9,     0,
      10,     5,     3,     4,     1,     3,     1,     3,     1,     3,
       0,     2,     3,     3,     1,     3,     0,     2,     3,     1,
       1,     1,     2,     3,     5,     3,     3,     1,     1,     1,
       0,     1,     1,     4,     3,     3,     5,     4,     6,     5,
       5,     4,     0,     0,     4,     0,     1,     0,     1,     1,
       6,     0,     6,     0,     2,     3,     5,     0,     1,     1,
       0,     5,     2,     3,     4,     0,     4,     0,     1,     1,
       1,     0,     0,     9,     0,     0,    11,     0,     2,     0,
       1,     3,     1,     1,     0,     1,     1,     0,     3,     0,
       0,     7,     1,     4,     3,     3,     5,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     1,     3,     0,     2,
       3,     0,     0,     6,     1,     3,     3,     4,     1,     1,
       1,     1,     2,     3,     0,     0,     6,     4,     5,     0,
       9,     4,     2,     2,     3,     2,     3,     2,     2,     3,
       3,     3,     2,     0,     0,     6,     2,     0,     0,     6,
       0,     0,     6,     0,     0,     6,     1,     0,     0,     6,
       0,     0,     7,     1,     0,     0,     6,     0,     0,     7,
       1,     0,     0,     6,     0,     0,     7,     0,     0,     6,
       0,     0,     6,     1,     3,     3,     0,     0,     0,     0,
      10,     0,     0,     0,     0,    10,     0,     0,     0,     0,
      10,     1,     1,     1,     1,     1,     3,     3,     5,     5,
       6,     6,     8,     8,     1,     1,     1,     3,     3,     5,
       1,     2,     0,     2,     2,     1,     2,     1,     2,     1,
       2,     1,     1,     2,     1,     1,     0,     1,     5,     4,
       6,     7,     5,     7,     0,     0,    10,     0,     0,    10,
       0,     0,     9,     0,     0,     7,     1,     3,     3,     3,
       1,     5,     0,     0,    10,     1,     3,     4,     4,     4,
       0,     0,    11,     0,     0,    11,     0,     0,    10,     5,
       0,     0,    10,     5,     0,     0,    10,     1,     3,     1,
       3,     4,     3,     5,     8,    10,     0,     3,     0,     1,
       9,    10,    10,    10,     9,    10
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

  case 106: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 107: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 108: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 109: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 110: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 111: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 112: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 113: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 114: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 115: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 116: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 117: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 118: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 119: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 120: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 121: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 122: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 123: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 124: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 125: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 126: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 127: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 128: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 129: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 130: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 131: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 132: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 133: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 134: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 135: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 136: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 137: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 138: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 139: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 140: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 141: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 142: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 143: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 144: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 145: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 146: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 147: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 148: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 149: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 150: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 151: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 152: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 153: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 154: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 155: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 156: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 157: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 158: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 159: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 160: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 161: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 162: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 163: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 164: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 165: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 166: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 167: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 168: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 169: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 170: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 171: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 172: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 173: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 174: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 175: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 176: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 177: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 178: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 179: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 180: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 181: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 182: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 183: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 184: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 185: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 186: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 187: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 188: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 189: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 190: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 191: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 192: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 193: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 194: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 195: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 196: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 197: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 198: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 199: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 200: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 201: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 202: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 203: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 204: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 205: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 206: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 207: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 208: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 209: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 210: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 211: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 212: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 213: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 214: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 215: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 216: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 217: /* $@6: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 218: /* function_declaration: optional_public_or_private_function $@6 function_declaration_header expression_block  */
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

  case 219: /* expression_block: "begin of code block" expressions "end of code block"  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 220: /* expression_block: "begin of code block" expressions "end of code block" "finally" "begin of code block" expressions "end of code block"  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 221: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 222: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 223: /* expression_any: "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 224: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 225: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 226: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 227: /* expression_any: expr_assign "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 228: /* expression_any: expression_delete "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 229: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 230: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 231: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 232: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 233: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 234: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 235: /* expression_any: expression_break "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 236: /* expression_any: expression_continue "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 237: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 238: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 239: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 240: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 241: /* expression_any: expression_label "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 242: /* expression_any: expression_goto "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 243: /* expression_any: "pass" "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 244: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 245: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 246: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 247: /* expr_keyword: "keyword" expr expression_block  */
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

  case 248: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 249: /* optional_expr_list: expr_list  */
                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 250: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 251: /* optional_expr_list_in_braces: '(' optional_expr_list ')'  */
                                              { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 252: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 253: /* type_declaration_no_options_list: type_declaration_no_options_list "end of expression" type_declaration  */
                                                                           {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 254: /* $@7: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 255: /* $@8: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 256: /* expression_keyword: "keyword" '<' $@7 type_declaration_no_options_list '>' $@8 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 257: /* $@9: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 258: /* $@10: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 259: /* expression_keyword: "type function" '<' $@9 type_declaration_no_options_list '>' $@10 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 260: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 261: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 262: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 263: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 264: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 265: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 266: /* name_in_namespace: "name" "::" "name"  */
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

  case 267: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 268: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 269: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 270: /* $@11: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 271: /* $@12: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 272: /* new_type_declaration: '<' $@11 type_declaration '>' $@12  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 273: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 274: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 275: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 276: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 277: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 278: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 279: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 280: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 281: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 282: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 283: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 284: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 285: /* expression_return: expression_return_no_pipe "end of expression"  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 286: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 287: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 288: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 289: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 290: /* expression_yield: expression_yield_no_pipe "end of expression"  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 291: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 292: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 293: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 294: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 295: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 296: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 297: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 298: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 299: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 300: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 301: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 302: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 303: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 304: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 305: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 306: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 307: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 308: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 309: /* $@13: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 310: /* $@14: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 311: /* expr_cast: "cast" '<' $@13 type_declaration_no_options '>' $@14 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 312: /* $@15: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 313: /* $@16: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 314: /* expr_cast: "upcast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 315: /* $@17: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 316: /* $@18: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 317: /* expr_cast: "reinterpret" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 318: /* $@19: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 319: /* $@20: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 320: /* expr_type_decl: "type" '<' $@19 type_declaration '>' $@20  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 321: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 322: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 323: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" "end of expression" "name" '>' expr ')'  */
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

  case 324: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 325: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 326: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" "end of expression" "name" '>' '(' expr ')'  */
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

  case 327: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 328: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 329: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 330: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 331: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 332: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 333: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 334: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 335: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 336: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 337: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 338: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 339: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 340: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 341: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 342: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 343: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 344: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 345: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 346: /* $@21: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 347: /* expr_full_block_assumed_piped: block_or_lambda $@21 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 348: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 349: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 350: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 351: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 352: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 353: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 354: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 355: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 356: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 357: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 358: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 359: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 360: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 361: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 362: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 363: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 364: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 365: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 366: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 367: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 368: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 369: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 370: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 371: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 372: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 375: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 376: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 377: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 378: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 392: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 393: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 394: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 395: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 396: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 397: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 398: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 399: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 400: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 401: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 402: /* $@22: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 403: /* $@23: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 404: /* func_addr_expr: '@' '@' '<' $@22 type_declaration_no_options '>' $@23 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 405: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 406: /* $@25: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 407: /* func_addr_expr: '@' '@' '<' $@24 optional_function_argument_list optional_function_type '>' $@25 func_addr_name  */
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

  case 408: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 409: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 410: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 411: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 412: /* $@26: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 413: /* $@27: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 414: /* expr_field: expr '.' $@26 error $@27  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 415: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 416: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 417: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 418: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 419: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 420: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 421: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 422: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 423: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 424: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 425: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 426: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 427: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 428: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 429: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 430: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 431: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 432: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 433: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 434: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 435: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 436: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 437: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 438: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 439: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 440: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 441: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 442: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 443: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 444: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 445: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 446: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 447: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 448: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 449: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 450: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 451: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 452: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 453: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 454: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 455: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 456: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 457: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 458: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 459: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 460: /* expr: '(' expr_list optional_comma ')'  */
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

  case 461: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 462: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 463: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 464: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 465: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 466: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 467: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 468: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 469: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 470: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 471: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 472: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 473: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 474: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 475: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 476: /* $@28: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 477: /* $@29: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 478: /* expr: expr "is" "type" '<' $@28 type_declaration_no_options '>' $@29  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 479: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 480: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 481: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 482: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 483: /* $@31: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 484: /* expr: expr "as" "type" '<' $@30 type_declaration '>' $@31  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 485: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 486: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 487: /* $@32: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 488: /* $@33: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 489: /* expr: expr '?' "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 490: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 491: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 492: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 493: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 494: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 495: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 496: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 497: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 498: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 499: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 500: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 501: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 502: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 503: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 504: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 505: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 506: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 507: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 508: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 509: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 510: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 511: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 512: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 513: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 514: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 515: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 516: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 517: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 518: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 519: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 520: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 521: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); /*this one is gone when BRABRA is disabled*/ }
    break;

  case 522: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 523: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 524: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 525: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 526: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 527: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 528: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 529: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 530: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 531: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 532: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 533: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 534: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 535: /* $@34: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 536: /* struct_variable_declaration_list: struct_variable_declaration_list $@34 structure_variable_declaration "end of expression"  */
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

  case 537: /* $@35: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 538: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@35 function_declaration_header "end of expression"  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 539: /* $@36: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 540: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@36 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 541: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' "end of expression"  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 542: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 543: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 544: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 545: /* function_argument_list: function_argument_list "end of expression" function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 546: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 547: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 548: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 549: /* tuple_type_list: tuple_type_list "end of expression" tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 550: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 551: /* tuple_alias_type_list: tuple_alias_type_list "end of expression"  */
                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 552: /* tuple_alias_type_list: tuple_alias_type_list tuple_type "end of expression"  */
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

  case 553: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 554: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 555: /* variant_type_list: variant_type_list "end of expression" variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 556: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 557: /* variant_alias_type_list: variant_alias_type_list "end of expression"  */
                                        {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 558: /* variant_alias_type_list: variant_alias_type_list variant_type "end of expression"  */
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

  case 559: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 560: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 561: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 562: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 563: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 564: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 565: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 566: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 567: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 568: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 569: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 570: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 571: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 572: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 573: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 574: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 575: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 576: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 577: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options "end of expression"  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 578: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 579: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 580: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 581: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 582: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 583: /* $@37: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 584: /* global_variable_declaration_list: global_variable_declaration_list $@37 optional_field_annotation let_variable_declaration  */
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

  case 585: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 586: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 587: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 588: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 589: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 590: /* global_let: kwd_let optional_shared optional_public_or_private_variable "begin of code block" global_variable_declaration_list "end of code block"  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 591: /* $@38: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 592: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@38 optional_field_annotation let_variable_declaration  */
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

  case 593: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 594: /* enum_list: enum_list "end of expression"  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 595: /* enum_list: enum_list "name" "end of expression"  */
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

  case 596: /* enum_list: enum_list "name" '=' expr "end of expression"  */
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

  case 597: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 598: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 599: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 600: /* $@39: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 601: /* single_alias: optional_public_or_private_alias "name" $@39 '=' type_declaration  */
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

  case 605: /* $@40: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 607: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 608: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 609: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 610: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 611: /* $@41: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 612: /* $@42: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 613: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name "begin of code block" $@41 enum_list $@42 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 614: /* $@43: %empty  */
                                                                                                                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 615: /* $@44: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 616: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration "begin of code block" $@43 enum_list $@44 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 617: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 618: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 619: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 620: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 621: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 622: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 623: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 624: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 625: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 626: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 627: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 628: /* optional_struct_variable_declaration_list: "begin of code block" struct_variable_declaration_list "end of code block"  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 629: /* $@45: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 630: /* $@46: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 631: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@45 structure_name $@46 optional_struct_variable_declaration_list  */
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

  case 632: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 633: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 634: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 635: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 636: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 637: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 638: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 639: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 640: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 641: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 642: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 643: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 644: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 645: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 646: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 647: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 648: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 649: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 650: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 651: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 652: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 653: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 654: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 655: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 656: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 657: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 658: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 659: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 660: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 661: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 662: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 663: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 664: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 665: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 666: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 667: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 668: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 669: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 670: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 671: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 672: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 673: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 674: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 675: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 676: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 677: /* bitfield_bits: bitfield_bits "end of expression" "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 678: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 679: /* bitfield_alias_bits: bitfield_alias_bits "end of expression"  */
                                      {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 680: /* bitfield_alias_bits: bitfield_alias_bits "name" "end of expression"  */
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

  case 681: /* $@47: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 682: /* $@48: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 683: /* bitfield_type_declaration: "bitfield" '<' $@47 bitfield_bits '>' $@48  */
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

  case 684: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 685: /* table_type_pair: type_declaration "end of expression" type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 686: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 687: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 688: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 689: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 690: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 691: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 692: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 693: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 694: /* $@49: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 695: /* $@50: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 696: /* type_declaration_no_options: "type" '<' $@49 type_declaration '>' $@50  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 697: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 698: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 699: /* $@51: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 700: /* type_declaration_no_options: '$' name_in_namespace '<' $@51 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 701: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 702: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 703: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 704: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 705: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 706: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 707: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 708: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 709: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 710: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 711: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 712: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 713: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 714: /* $@53: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 715: /* type_declaration_no_options: "smart_ptr" '<' $@52 type_declaration '>' $@53  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 716: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 717: /* $@54: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 718: /* $@55: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 719: /* type_declaration_no_options: "array" '<' $@54 type_declaration '>' $@55  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 720: /* $@56: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 721: /* $@57: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 722: /* type_declaration_no_options: "table" '<' $@56 table_type_pair '>' $@57  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 723: /* $@58: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 724: /* $@59: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 725: /* type_declaration_no_options: "iterator" '<' $@58 type_declaration '>' $@59  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 726: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 727: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 728: /* $@61: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 729: /* type_declaration_no_options: "block" '<' $@60 type_declaration '>' $@61  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 730: /* $@62: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 731: /* $@63: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 732: /* type_declaration_no_options: "block" '<' $@62 optional_function_argument_list optional_function_type '>' $@63  */
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

  case 733: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 734: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 735: /* $@65: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 736: /* type_declaration_no_options: "function" '<' $@64 type_declaration '>' $@65  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 737: /* $@66: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 738: /* $@67: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 739: /* type_declaration_no_options: "function" '<' $@66 optional_function_argument_list optional_function_type '>' $@67  */
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

  case 740: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 741: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 742: /* $@69: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 743: /* type_declaration_no_options: "lambda" '<' $@68 type_declaration '>' $@69  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 744: /* $@70: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 745: /* $@71: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 746: /* type_declaration_no_options: "lambda" '<' $@70 optional_function_argument_list optional_function_type '>' $@71  */
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

  case 747: /* $@72: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 748: /* $@73: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 749: /* type_declaration_no_options: "tuple" '<' $@72 tuple_type_list '>' $@73  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 750: /* $@74: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 751: /* $@75: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 752: /* type_declaration_no_options: "variant" '<' $@74 variant_type_list '>' $@75  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 753: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 754: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 755: /* type_declaration: type_declaration '|' '#'  */
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

  case 756: /* $@76: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 757: /* $@77: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 758: /* $@78: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 759: /* $@79: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 760: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@76 "name" $@77 "begin of code block" $@78 tuple_alias_type_list $@79 "end of code block"  */
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

  case 761: /* $@80: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 762: /* $@81: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 763: /* $@82: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 764: /* $@83: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 765: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@80 "name" $@81 "begin of code block" $@82 variant_alias_type_list $@83 "end of code block"  */
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

  case 766: /* $@84: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 767: /* $@85: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 768: /* $@86: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 769: /* $@87: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 770: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@84 "name" $@85 "begin of code block" $@86 bitfield_alias_bits $@87 "end of code block"  */
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

  case 771: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 772: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 773: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 774: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 775: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 776: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 777: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 778: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 779: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 780: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 781: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 782: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 783: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 784: /* make_variant_dim: make_struct_fields  */
                                {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 785: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 786: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 787: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 788: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 789: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 790: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 791: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 792: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 793: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 806: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 807: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 808: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 809: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 810: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 811: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 812: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 813: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 814: /* $@88: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 815: /* $@89: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 816: /* make_struct_decl: "struct" '<' $@88 type_declaration_no_options '>' $@89 '(' use_initializer make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                   {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 817: /* $@90: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 818: /* $@91: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 819: /* make_struct_decl: "class" '<' $@90 type_declaration_no_options '>' $@91 '(' use_initializer make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                 {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 820: /* $@92: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 821: /* $@93: %empty  */
                                                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 822: /* make_struct_decl: "variant" '<' $@92 type_declaration_no_options '>' $@93 '(' make_variant_dim ')'  */
                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 823: /* $@94: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 824: /* $@95: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 825: /* make_struct_decl: "default" '<' $@94 type_declaration_no_options '>' $@95 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 826: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 827: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 828: /* make_tuple: make_tuple ',' expr  */
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

  case 829: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 830: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 831: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 832: /* $@96: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 833: /* $@97: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 834: /* make_tuple_call: "tuple" '<' $@96 type_declaration_no_options '>' $@97 '(' use_initializer make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                   {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 835: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 836: /* make_dim: make_dim "end of expression" make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 837: /* make_dim_decl: '[' expr_list optional_comma ']'  */
                                                          {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 838: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 839: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 840: /* $@98: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 841: /* $@99: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 842: /* make_dim_decl: "array" "struct" '<' $@98 type_declaration_no_options '>' $@99 '(' use_initializer make_struct_dim_decl ')'  */
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

  case 843: /* $@100: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 844: /* $@101: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 845: /* make_dim_decl: "array" "tuple" '<' $@100 type_declaration_no_options '>' $@101 '(' use_initializer make_struct_dim_decl ')'  */
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

  case 846: /* $@102: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 847: /* $@103: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 848: /* make_dim_decl: "array" "variant" '<' $@102 type_declaration_no_options '>' $@103 '(' make_variant_dim ')'  */
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

  case 849: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 850: /* $@104: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 851: /* $@105: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 852: /* make_dim_decl: "array" '<' $@104 type_declaration_no_options '>' $@105 '(' expr_list optional_comma ')'  */
                                                                                                                                                                              {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-9])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 853: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 854: /* $@106: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 855: /* $@107: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 856: /* make_dim_decl: "fixed_array" '<' $@106 type_declaration_no_options '>' $@107 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        if ( !mka->makeType->dim.size() ) mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 857: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 858: /* make_table: make_table "end of expression" make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 859: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 860: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 861: /* make_table_decl: "begin of code block" expr_map_tuple_list optional_comma "end of code block"  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 862: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 863: /* make_table_decl: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 864: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
                                                                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-7])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-7])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 865: /* make_table_decl: "table" '<' type_declaration_no_options "end of expression" type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
                                                                                                                                                                {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::tTuple);
        mka->makeType->argTypes.push_back((yyvsp[-7].pTypeDecl));
        mka->makeType->argTypes.push_back((yyvsp[-5].pTypeDecl));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-9])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 866: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 867: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 868: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 869: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 870: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 871: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 872: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 873: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 874: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 875: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block" "end of code block"  */
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


