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
  YYSYMBOL_make_struct_single = 464,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 465,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 466,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 467, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 468, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 469, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 470, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 471, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_use_initializer = 472,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 473,         /* make_struct_decl  */
  YYSYMBOL_474_88 = 474,                   /* $@88  */
  YYSYMBOL_475_89 = 475,                   /* $@89  */
  YYSYMBOL_476_90 = 476,                   /* $@90  */
  YYSYMBOL_477_91 = 477,                   /* $@91  */
  YYSYMBOL_478_92 = 478,                   /* $@92  */
  YYSYMBOL_479_93 = 479,                   /* $@93  */
  YYSYMBOL_480_94 = 480,                   /* $@94  */
  YYSYMBOL_481_95 = 481,                   /* $@95  */
  YYSYMBOL_make_tuple = 482,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 483,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 484,          /* make_tuple_call  */
  YYSYMBOL_485_96 = 485,                   /* $@96  */
  YYSYMBOL_486_97 = 486,                   /* $@97  */
  YYSYMBOL_make_dim = 487,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 488,            /* make_dim_decl  */
  YYSYMBOL_489_98 = 489,                   /* $@98  */
  YYSYMBOL_490_99 = 490,                   /* $@99  */
  YYSYMBOL_491_100 = 491,                  /* $@100  */
  YYSYMBOL_492_101 = 492,                  /* $@101  */
  YYSYMBOL_493_102 = 493,                  /* $@102  */
  YYSYMBOL_494_103 = 494,                  /* $@103  */
  YYSYMBOL_495_104 = 495,                  /* $@104  */
  YYSYMBOL_496_105 = 496,                  /* $@105  */
  YYSYMBOL_497_106 = 497,                  /* $@106  */
  YYSYMBOL_498_107 = 498,                  /* $@107  */
  YYSYMBOL_make_table = 499,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 500,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 501,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 502, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 503,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 504       /* array_comprehension  */
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
#define YYLAST   14587

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  217
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  288
/* YYNRULES -- Number of rules.  */
#define YYNRULES  866
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1631

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
       0,   534,   534,   535,   540,   541,   542,   543,   544,   545,
     546,   547,   548,   549,   550,   551,   552,   556,   562,   563,
     564,   568,   569,   573,   591,   592,   593,   594,   598,   602,
     607,   616,   624,   640,   645,   653,   653,   692,   722,   726,
     727,   728,   732,   735,   739,   745,   754,   757,   763,   764,
     768,   772,   773,   777,   780,   786,   792,   795,   801,   802,
     806,   807,   808,   817,   818,   822,   823,   823,   829,   830,
     831,   832,   833,   837,   843,   843,   849,   849,   855,   863,
     873,   882,   882,   889,   890,   891,   892,   893,   894,   898,
     903,   911,   912,   913,   917,   918,   919,   920,   921,   922,
     923,   924,   930,   933,   939,   942,   948,   949,   950,   954,
     967,   985,   988,   996,  1007,  1018,  1029,  1032,  1039,  1043,
    1050,  1051,  1055,  1056,  1057,  1061,  1064,  1071,  1075,  1076,
    1077,  1078,  1079,  1080,  1081,  1082,  1083,  1084,  1085,  1086,
    1087,  1088,  1089,  1090,  1091,  1092,  1093,  1094,  1095,  1096,
    1097,  1098,  1099,  1100,  1101,  1102,  1103,  1104,  1105,  1106,
    1107,  1108,  1109,  1110,  1111,  1112,  1113,  1114,  1115,  1116,
    1117,  1118,  1119,  1120,  1121,  1122,  1123,  1124,  1125,  1126,
    1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,  1135,  1136,
    1137,  1138,  1139,  1140,  1141,  1142,  1143,  1144,  1145,  1146,
    1147,  1148,  1149,  1150,  1151,  1152,  1153,  1154,  1155,  1156,
    1157,  1158,  1163,  1181,  1182,  1183,  1187,  1193,  1193,  1210,
    1214,  1225,  1234,  1246,  1247,  1248,  1249,  1250,  1251,  1252,
    1253,  1254,  1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,
    1263,  1264,  1265,  1266,  1270,  1275,  1281,  1287,  1298,  1299,
    1303,  1304,  1308,  1312,  1319,  1319,  1319,  1325,  1325,  1325,
    1334,  1368,  1371,  1374,  1377,  1383,  1384,  1395,  1399,  1402,
    1410,  1410,  1410,  1413,  1419,  1422,  1426,  1430,  1437,  1443,
    1447,  1451,  1454,  1457,  1465,  1468,  1471,  1479,  1482,  1490,
    1493,  1496,  1504,  1510,  1511,  1512,  1516,  1517,  1521,  1522,
    1526,  1531,  1539,  1545,  1551,  1560,  1572,  1575,  1581,  1581,
    1581,  1584,  1584,  1584,  1589,  1589,  1589,  1597,  1597,  1597,
    1603,  1613,  1624,  1639,  1642,  1648,  1649,  1656,  1667,  1668,
    1669,  1673,  1674,  1675,  1676,  1680,  1685,  1693,  1694,  1698,
    1703,  1710,  1717,  1717,  1726,  1727,  1728,  1729,  1730,  1731,
    1732,  1736,  1737,  1738,  1739,  1740,  1741,  1742,  1743,  1744,
    1745,  1746,  1747,  1748,  1749,  1750,  1751,  1752,  1753,  1754,
    1758,  1759,  1760,  1761,  1766,  1767,  1768,  1769,  1770,  1771,
    1772,  1773,  1774,  1775,  1776,  1777,  1778,  1779,  1780,  1781,
    1782,  1787,  1794,  1806,  1811,  1821,  1825,  1832,  1835,  1835,
    1835,  1840,  1840,  1840,  1853,  1857,  1861,  1866,  1873,  1873,
    1873,  1880,  1884,  1893,  1897,  1900,  1906,  1907,  1908,  1909,
    1910,  1911,  1912,  1913,  1914,  1915,  1916,  1917,  1918,  1919,
    1920,  1921,  1922,  1923,  1924,  1925,  1926,  1927,  1928,  1929,
    1930,  1931,  1932,  1933,  1934,  1935,  1936,  1937,  1938,  1939,
    1940,  1941,  1947,  1948,  1949,  1950,  1951,  1964,  1965,  1966,
    1967,  1968,  1969,  1970,  1971,  1972,  1973,  1974,  1975,  1978,
    1981,  1982,  1985,  1985,  1985,  1988,  1993,  1997,  2001,  2001,
    2001,  2006,  2009,  2013,  2013,  2013,  2018,  2021,  2022,  2023,
    2024,  2025,  2026,  2027,  2028,  2029,  2031,  2035,  2036,  2041,
    2045,  2046,  2047,  2048,  2049,  2050,  2051,  2055,  2059,  2063,
    2067,  2071,  2075,  2079,  2083,  2087,  2094,  2095,  2096,  2100,
    2101,  2102,  2106,  2107,  2111,  2112,  2113,  2117,  2118,  2122,
    2133,  2136,  2136,  2155,  2154,  2169,  2168,  2181,  2190,  2199,
    2209,  2210,  2214,  2217,  2226,  2227,  2231,  2234,  2237,  2253,
    2262,  2263,  2267,  2270,  2273,  2287,  2288,  2292,  2298,  2304,
    2307,  2311,  2317,  2326,  2327,  2328,  2332,  2333,  2337,  2344,
    2349,  2358,  2364,  2375,  2378,  2383,  2388,  2396,  2407,  2410,
    2410,  2430,  2431,  2435,  2436,  2437,  2441,  2444,  2444,  2463,
    2466,  2469,  2484,  2503,  2504,  2505,  2510,  2510,  2536,  2537,
    2541,  2542,  2542,  2546,  2547,  2548,  2552,  2562,  2567,  2562,
    2579,  2584,  2579,  2599,  2600,  2604,  2605,  2609,  2615,  2616,
    2620,  2621,  2622,  2626,  2629,  2635,  2640,  2635,  2654,  2661,
    2666,  2675,  2681,  2692,  2693,  2694,  2695,  2696,  2697,  2698,
    2699,  2700,  2701,  2702,  2703,  2704,  2705,  2706,  2707,  2708,
    2709,  2710,  2711,  2712,  2713,  2714,  2715,  2716,  2717,  2718,
    2722,  2723,  2724,  2725,  2726,  2727,  2728,  2729,  2733,  2744,
    2748,  2755,  2767,  2774,  2783,  2788,  2791,  2804,  2804,  2804,
    2817,  2821,  2828,  2832,  2839,  2840,  2841,  2842,  2843,  2858,
    2864,  2864,  2864,  2868,  2873,  2880,  2880,  2887,  2891,  2895,
    2900,  2905,  2910,  2915,  2919,  2923,  2928,  2932,  2936,  2941,
    2941,  2941,  2947,  2954,  2954,  2954,  2959,  2959,  2959,  2965,
    2965,  2965,  2970,  2974,  2974,  2974,  2979,  2979,  2979,  2988,
    2992,  2992,  2992,  2997,  2997,  2997,  3006,  3010,  3010,  3010,
    3015,  3015,  3015,  3024,  3024,  3024,  3030,  3030,  3030,  3039,
    3042,  3053,  3069,  3069,  3074,  3079,  3069,  3104,  3104,  3109,
    3115,  3104,  3140,  3140,  3145,  3150,  3140,  3180,  3181,  3182,
    3183,  3184,  3188,  3195,  3202,  3208,  3214,  3221,  3228,  3234,
    3244,  3252,  3257,  3264,  3265,  3270,  3271,  3275,  3276,  3280,
    3281,  3285,  3286,  3287,  3291,  3292,  3293,  3297,  3298,  3302,
    3308,  3315,  3323,  3330,  3338,  3347,  3347,  3347,  3355,  3355,
    3355,  3362,  3362,  3362,  3369,  3369,  3369,  3380,  3383,  3389,
    3403,  3409,  3415,  3421,  3421,  3421,  3431,  3436,  3443,  3451,
    3456,  3463,  3463,  3463,  3473,  3473,  3473,  3483,  3483,  3483,
    3493,  3501,  3501,  3501,  3509,  3516,  3516,  3516,  3526,  3531,
    3538,  3541,  3547,  3555,  3564,  3572,  3580,  3593,  3594,  3598,
    3599,  3604,  3607,  3610,  3613,  3616,  3619
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
  "make_decl", "make_struct_fields", "make_struct_single",
  "make_struct_dim", "optional_block",
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

#define YYPACT_NINF (-1431)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-741)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1431,    66, -1431, -1431,    38,    41,   242,   -91, -1431,  -101,
     442,   442,   442, -1431,    98,    67, -1431, -1431,   -33, -1431,
   -1431, -1431,   479, -1431,   113, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431,   115, -1431,    45,   122,   194,
   -1431, -1431, -1431, -1431,   242, -1431,    27, -1431,   442,   442,
   -1431, -1431,   113, -1431, -1431, -1431, -1431, -1431,   223,   323,
   -1431, -1431, -1431,    67,    67,    67,   344, -1431,   638,   212,
   -1431, -1431, -1431, -1431,   578,   594,   613, -1431,   616,    56,
      38,   425,    41,   410,   534, -1431,   521,   521, -1431,   584,
     485,     6,   576,   624,   604,   607,   615, -1431,   617,   475,
   -1431, -1431,   -69,    38,    67,    67,    67,    67, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431,   620, -1431, -1431, -1431, -1431,
   -1431,   612, -1431, -1431, -1431, -1431, -1431,   590,    89, -1431,
   -1431, -1431, -1431,   736, -1431, -1431, -1431, -1431, -1431,   625,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,   621,
   -1431,   182, -1431,   575,   654,   638, 14419, -1431,   -13,   702,
   -1431,   -73, -1431, -1431,   628, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431,   208, -1431,   634, -1431,   630,   647,   649, -1431,
   -1431, 12914, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431,   797,   798, -1431,
     622,   650, -1431,   455, -1431,   667, -1431,   658,    38,    38,
     637,    50, -1431, -1431, -1431,    89, -1431,  9858, -1431, -1431,
   -1431,   671,   675, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431,   678,   642, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431,   833, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
     687,   653, -1431, -1431,   124,   669, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431,   688,   676,   690,
   -1431,   -73,   267, -1431,    38,   659,   831,   462, -1431, -1431,
     682,   684,   692,   674,   697,   698, -1431, -1431, -1431,   680,
   -1431, -1431, -1431, -1431, -1431,   700, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431,   704, -1431, -1431,
   -1431,   705,   707, -1431, -1431, -1431, -1431,   708,   709,   683,
      98, -1431, -1431, -1431, -1431, -1431,   976,   713, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431,   737,   769, -1431,   695, -1431,
     261, -1431,   -30,  9858, -1431,  2363,   202, -1431,    98, -1431,
   -1431, -1431,    50,   699, -1431,  9053,   740,   741,  9858, -1431,
     -39, -1431, -1431, -1431,  9053, -1431, -1431,   743, -1431,   356,
     370,   378, -1431, -1431,  9053,   332, -1431, -1431, -1431,     2,
   -1431, -1431, -1431,    13,  5211, -1431,   703,  9614,   415,  9713,
     417, -1431, -1431,  9053, -1431, -1431,   188,   -28, -1431,   713,
   -1431,   719,   721,  9053, -1431, -1431, -1431, -1431, -1431,   131,
     710,   726,    26,  3183, -1431, -1431,   650,   337,  5413,   714,
    9053,   752,   728,   732,   715, -1431,   748,   742,   768,  5615,
      71,   364,   746, -1431,   381,   747,   751,  3385,  9053,  9053,
     108,   108,   108,   724,   729,   734,   735,   738,   744, -1431,
    9416,  9515,  5819, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
    6021,   756, -1431,  6225,   916, -1431,  9053,  9053,  9053,  9053,
    9053,  4807,  9053, -1431,   745, -1431, -1431,   773,   777,  9053,
     949, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
     551, -1431,   -16,   779, -1431,   781,   782,   783, -1431,   786,
   -1431, -1431,   903, -1431, -1431, -1431, -1431,   760, -1431, -1431,
     -66, -1431, -1431, -1431, -1431, -1431, -1431,  1931, -1431,   759,
   -1431, -1431, -1431, -1431, -1431, -1431,   390, -1431,   796, -1431,
   -1431,    31, -1431, -1431,   761,   784, -1431, 10297, -1431,   934,
     599, -1431, -1431, -1431,  3789,  9858,  9858,  9858, 10407,  9858,
    9858,   765,   811,  9858,   622,  9858,   622,  9858,   622,  9957,
     812, 10442, -1431,  9053, -1431, -1431, -1431, -1431,   770, -1431,
   -1431, 12437,  9053, -1431,   976,   802, -1431,   804,   -25, -1431,
     805,   713,   814,   795, -1431,   807,   815, 10552,   790,   953,
   -1431,   -49, -1431, -1431, -1431, 12949,   188, -1431,   791, -1431,
   -1431,    98, -1431,   803,   813,   824, -1431,  9053,  3789, -1431,
     818,   861, 10040,   985,  9858,  9053,  9053, 13690,  9053, 12949,
     820, -1431, -1431,  9053, -1431, -1431,   816,   857, 13690,  9053,
   -1431, -1431,  9053, -1431, -1431,  9053, -1431,  9858,  3789, -1431,
   10040,   322,   322,   806, -1431,   760, -1431, -1431, -1431,  9053,
    9053,  9053,  9053,  9053,  9053,   188,  1708,   188,  2777,   188,
   13048, -1431,   632, -1431, 12949, -1431,   586,   188, -1431,   838,
     847,   322,   322,   120,   322,   322,   188,  1014,   843, 13690,
     843,   287, -1431, -1431, 12949, -1431, -1431, -1431, -1431,  5009,
   -1431, -1431, -1431, -1431, -1431, -1431,    -9,   872,   108, -1431,
   14216, 14311,  3991,  3991,  3991,  3991,  3991,  3991,  3991,  3991,
    9053,  9053, -1431, -1431,  9053,  3991,  3991,  9053,  9053,  9053,
     865,  3991,  9053,   431,  9053,  9053,  9053,  9053,  9053,  9053,
    3991,  3991,  9053,  9053,  9053,  3991,  3991,  3991,  9053,  3991,
    6427,  9053,  9053,  9053,  9053,  9053,  9053,  9053,  9053,  9053,
    9053,   125,  9053, -1431,  6629, -1431,  9053, -1431,   202, -1431,
      67,  1025,   -73, -1431,   866, -1431,  3789, -1431, 10152,  -106,
     154,   845,   414, -1431,   253,   346, -1431, -1431,    -8,   411,
     669,   533,   669,   546,   669, -1431,   152, -1431,   233, -1431,
    9858,   828,   818, -1431, -1431, 12472, -1431, -1431,  9858, -1431,
   -1431,  9858, -1431, -1431, -1431,  9053,   877, -1431,   879, -1431,
    9858, -1431,  3789,  9858,  9858, -1431,    18,  9858,  6831,  7033,
    9858, -1431, -1431, -1431,  9858,   843, -1431,   818,  9053,  9053,
    9053,  9053,  9053,  9053,  9053,  9053,  9053,  9053,  9053,  9053,
    9053,  9053,  9053,  9053,  9053,  9053,   650,   519,   838, 13690,
   10587, -1431, -1431,  9858,  9858, 10697,  9858, -1431, -1431, 10732,
    9858,   843,  9858,  9858,   843,  9858,   579, -1431, 10040, -1431,
     872, 10842, 10877, 10987, 11022, 11132, 11167,    21,   108,   836,
     139,  2980,  4195,  7235, 13147,   863,   -10,   167,   871,   252,
      28,  7437,   -10,   497,    34,  9053,   882,  9053, -1431, -1431,
    9858, -1431,  9858, -1431,  9053,   597,    35,  9053,   884, -1431,
      42,   188,  9053,   839,   868,   858,   873,   395, -1431, -1431,
     569,  9053,   760,  4399, -1431,   231,   863,   870,   908,   908,
   -1431, -1431,    -3,   622, -1431,   885,   876, -1431, -1431,   891,
     889, -1431, -1431,   108,   108,   108, -1431, -1431,  2130, -1431,
    2130, -1431,  2130, -1431,  2130, -1431,  2130, -1431,  2130, -1431,
    2130, -1431,  2130,   486,   486,   191, -1431,  2130, -1431,  2130,
     191,  1978,  1978,   892, -1431,  2130,   280,   893, -1431, 12571,
     -18,   -18,   759, 13690,   486,   486, -1431,  2130, -1431,  2130,
   14030, 13905, 13940, -1431,  2130, -1431,  2130, -1431,  2130, 13780,
   -1431,  2130, 14342, 13182, 14058, 14148,  1301,   191,   191,   733,
     733,   280,   280,   280,   432,  9053,   894,   895,   440,  9053,
    1076, 12670, -1431,   266, 13272,   910,   271,   631,  1028,   923,
   -1431, -1431, 10262, -1431, -1431, -1431, -1431,  9858, -1431, -1431,
     928, -1431, -1431,   914, -1431,   915, -1431,   917,  9957, -1431,
     812, -1431,   235,   713, -1431, -1431,   713,   713, 11277, -1431,
    1068,   -56, -1431, 10040,   864,   905,  9053,   547, -1431,   282,
     -74,   944, 11312,   548,  9858,  9858,  9858,   960,   900, 13690,
   13690, 13690, 13690, 13690, 13690, 13690, 13690, 13690, 13690, 13690,
   13690, 13690, 13690, 13690, 13690, 13690, 13690, -1431,  9858,   904,
     907, -1431,  9053,  1008,  1063, -1431,  1188, -1431,  1786,   911,
    2283,  2697,   913,  2899,   872,   622, -1431, -1431, -1431, -1431,
   -1431,   906,  9053, -1431,  9053,  9053,  9053,  4603, 12437,     7,
    9053,   467,   481,   167, -1431, -1431,   918, -1431,  9053,  9053,
   -1431,   920, -1431,  9053,   481,   478,   922, -1431, -1431,  9053,
   13690, -1431, -1431,   293,   348, 13307,  9053, -1431, -1431,  2575,
    9053,    44, -1431, -1431,  9053,  9053,  9858,   622,   650, -1431,
   -1431,  9053, -1431,  1482,   872,   289,  9255, -1431, -1431, -1431,
     307,   283,   969,   970,   974,   975, -1431,   315,   669, -1431,
    9053, -1431,  9053, -1431, -1431, -1431,  7639,  9053, -1431,   924,
     932, -1431, -1431,  9053,   933, -1431, 12705,  9053,  7841,   935,
   -1431, 12804, -1431, -1431, -1431, -1431, -1431,   973, -1431, -1431,
     196, -1431,    15, -1431, -1431, -1431, -1431, -1431,   713, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431,  9858,   937, -1431,   989,  9053, -1431, -1431,
     357, -1431, -1431,   109, -1431,   945,   366, -1431, -1431,  3102,
    3305,  3507, -1431, -1431,  3709,  9053, -1431, 13690, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431,   948,   669,  8043,
     508, 11422, 13690, 13690,   -10,   167, 13690,   950,   153,   863,
   -1431, -1431, 13690,   871, -1431,   512,   -10,   952, -1431, -1431,
   -1431, -1431,   520, -1431, -1431, -1431,   529, -1431,   532,  9053,
   11457, 11567,  3911,   669, -1431, 12949, -1431,   981,   622,   954,
    4399,   997,   955,   437, -1431, -1431, -1431, -1431,    -3,   961,
     135,  9858, 11602,  9858, 11712, -1431,   302, 11747, -1431,  9053,
   13815,  9053, -1431, 11857, -1431,   308,  9053, -1431, -1431, -1431,
    1141,    15, -1431, -1431,   631, -1431, -1431, -1431,   713,  9053,
   -1431, 13690,   962,   963, -1431, -1431, -1431,  1009,  9053, -1431,
   -1431, -1431, -1431,   966,   968,   838,  9053,  9053,  9053,   971,
    1112,   972,   978,  8245,   135, -1431,   311,  9053,   160,   167,
   -1431,  9053,  9053,  9053,  9053,   478, -1431,  9053,  9053,   979,
    9053,  9053,   536, -1431, -1431, -1431,   993,   569,  3587, -1431,
     669, -1431,   330, -1431,   558,  9858,   -39, -1431, -1431,  8447,
   -1431, -1431,  3988, -1431,   549, -1431, -1431, -1431,  9858, 11892,
   12002, -1431, -1431, 12037, -1431, -1431,  1141,   188,   980,  1112,
    1112,  1000, 12147,   983,   987,   992,  9053,  9053,   984,   191,
     191,   191,  9053, -1431,  1112,   481, -1431, 12182, -1431, -1431,
   13397,  9053,  9053, -1431, 12292, 13690, 13690, 13397, -1431,  1021,
     191,  9053, -1431,  1021, 13397,  9053,   324, -1431, -1431,  8649,
    8851, -1431, -1431, -1431, -1431, -1431, 13690,   650,   994,  9858,
     -39,   940,  9053,  9053, 13780, -1431, -1431,   555, -1431, -1431,
   -1431, 14419, -1431, -1431, -1431,   481,   481,  9053, -1431,  1112,
    1112,   481,   843,   838, -1431,   843,   481,    74, -1431,  1154,
     998, 13690, 13690,   179,  1031,  1032,   999,  1034,  1003, 13397,
   -1431,   324,  9053,  9053, 13690, -1431, -1431,   940,  9053,  9053,
   13436, 13815, -1431, -1431, -1431,  1035, 14419,   118,   132, 12327,
     481,   481,   136,  1006,  1013,  1015,   172, -1431,  9053,  1005,
    9053,  9053,  1010,  1043, -1431, -1431, -1431,  1018, -1431, 13690,
    9053, 13526, 13561, -1431, -1431, -1431,   650, -1431, -1431, -1431,
     180,   189, -1431, -1431, -1431, -1431, -1431, 13690, -1431, 13690,
   13690, -1431, -1431, -1431, 13651, -1431, -1431, -1431, -1431, -1431,
   -1431
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   120,     1,   296,     0,     0,     0,   601,   297,     0,
     593,   593,   593,    16,     0,     0,    15,     3,     0,    10,
       9,     8,     0,     7,   581,     6,    11,     5,     4,    13,
      12,    14,    92,    93,    91,   100,   102,    37,    53,    50,
      51,    39,    40,    41,     0,    42,    48,    38,   593,   593,
      22,    21,   581,   595,   594,   762,   752,   757,     0,   265,
      35,   107,   108,     0,     0,     0,   109,   111,   118,     0,
     106,    17,   619,   618,   213,   603,   620,   582,   583,     0,
       0,     0,     0,    43,     0,    49,     0,     0,    46,     0,
       0,   593,     0,    18,     0,     0,     0,   267,     0,     0,
     117,   112,     0,     0,     0,     0,     0,     0,   121,   215,
     214,   217,   212,   605,   604,     0,   622,   621,   625,   585,
     584,   587,    98,    99,    96,    97,    95,     0,     0,    94,
     103,    54,    52,    48,    45,    44,   596,   598,   600,     0,
     602,    19,    20,    23,   763,   753,   758,   266,    33,    36,
     116,     0,   113,   114,   115,   119,     0,   606,     0,   615,
     578,   516,    24,    25,     0,    87,    88,    85,    86,    84,
      83,    89,     0,    47,     0,   599,     0,     0,     0,    34,
     110,     0,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,     0,     0,   127,
     122,     0,   607,     0,   616,     0,   626,   579,     0,     0,
     518,     0,    26,    27,    28,     0,   101,     0,   764,   754,
     759,   181,   182,   179,   130,   131,   133,   132,   134,   135,
     136,   137,   163,   164,   161,   162,   154,   165,   166,   155,
     152,   153,   180,   174,     0,   178,   167,   168,   169,   170,
     141,   142,   143,   138,   139,   140,   151,     0,   157,   158,
     156,   149,   150,   145,   144,   146,   147,   148,   129,   128,
     173,     0,   159,   160,   516,   125,   244,   218,   589,   660,
     663,   666,   667,   661,   664,   662,   665,     0,   613,   623,
     586,   516,     0,   104,     0,     0,   568,   566,   588,    90,
       0,     0,     0,     0,     0,     0,   633,   653,   634,   669,
     635,   639,   640,   641,   642,   659,   646,   647,   648,   649,
     650,   651,   652,   654,   655,   656,   657,   722,   638,   645,
     658,   729,   736,   636,   643,   637,   644,     0,     0,     0,
       0,   668,   684,   687,   685,   686,   749,   597,   674,   546,
     552,   183,   184,   177,   172,   185,   175,   171,     0,   123,
     295,   540,     0,     0,   216,     0,   608,   610,     0,   617,
     530,   627,     0,     0,   105,     0,     0,     0,     0,   567,
       0,   690,   713,   716,     0,   719,   709,     0,   677,   723,
     730,   737,   743,   746,     0,     0,   699,   704,   698,     0,
     712,   708,   701,     0,     0,   703,   688,     0,   765,   755,
     760,   186,   176,     0,   293,   294,     0,   516,   124,   126,
     246,     0,     0,     0,    63,    64,    76,   422,   423,     0,
       0,     0,     0,   281,   416,   279,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   280,     0,     0,     0,     0,
       0,     0,     0,   659,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   505,
       0,     0,     0,   344,   346,   345,   347,   348,   349,   350,
       0,     0,    29,     0,   219,   223,     0,     0,     0,     0,
       0,     0,     0,   328,   329,   420,   419,     0,     0,     0,
       0,   239,   234,   231,   230,   232,   233,   264,   245,   225,
     499,   224,   417,     0,   490,    71,    72,    69,   237,    70,
     238,   240,   299,   229,   489,   488,   487,   120,   493,   418,
       0,   226,   492,   491,   463,   424,   464,   351,   425,     0,
     421,   767,   771,   768,   769,   770,     0,   590,     0,   589,
     614,   531,   580,   517,     0,     0,   499,     0,   570,   571,
       0,   564,   565,   563,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   122,     0,   122,     0,   122,     0,
       0,     0,   695,   248,   706,   707,   700,   702,     0,   705,
     689,     0,     0,   751,   750,     0,   675,     0,   265,   547,
       0,   542,     0,     0,   553,     0,     0,     0,     0,   628,
     538,   557,   541,   805,   808,     0,     0,   270,   274,   273,
     278,     0,   317,     0,     0,     0,   841,     0,     0,   285,
     282,     0,   323,     0,     0,     0,     0,   268,     0,     0,
       0,   308,   311,     0,   243,   314,     0,     0,    57,     0,
      78,   845,     0,   814,   823,     0,   811,     0,     0,   290,
     287,   452,   453,   329,   339,   120,   263,   261,   262,     0,
       0,     0,     0,     0,     0,     0,   783,     0,     0,     0,
     821,   848,     0,   254,     0,   257,     0,     0,   850,   859,
       0,   429,   428,   465,   427,   426,     0,     0,   859,   323,
     859,   330,   241,   242,     0,    74,   342,   222,   497,     0,
     228,   235,   236,   284,   289,   298,     0,   337,     0,   227,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   454,   455,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   408,     0,   221,     0,   591,     0,   609,   611,   624,
       0,     0,   516,   569,     0,   573,     0,   577,   351,     0,
       0,     0,   680,   693,     0,     0,   670,   672,     0,     0,
     125,     0,   125,     0,   125,   544,     0,   550,     0,   671,
       0,     0,   249,   697,   682,     0,   676,   766,     0,   548,
     756,     0,   554,   761,   539,     0,     0,   556,     0,   555,
       0,   558,     0,     0,     0,    79,     0,     0,   797,     0,
       0,   831,   834,   837,     0,   859,   286,   283,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   859,   269,
       0,    80,    81,     0,     0,     0,     0,    55,    56,     0,
       0,   859,     0,     0,   859,     0,     0,   291,   288,   330,
     337,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     265,     0,     0,     0,   817,   781,   783,     0,   826,     0,
       0,     0,   783,     0,     0,     0,     0,     0,   786,   853,
       0,   247,     0,    32,     0,    30,     0,   860,     0,   244,
       0,     0,   860,     0,     0,     0,     0,   398,   395,   397,
      60,     0,   120,     0,   411,     0,   780,     0,     0,     0,
     307,   306,     0,   122,   260,     0,     0,   476,   475,     0,
       0,   477,   481,     0,     0,     0,   373,   382,   361,   383,
     362,   385,   364,   384,   363,   386,   365,   376,   355,   377,
     356,   378,   357,   430,   431,   443,   387,   366,   388,   367,
     444,   441,   442,     0,   375,   353,   470,     0,   461,     0,
     494,   495,   496,   354,   432,   433,   389,   368,   390,   369,
     448,   449,   450,   379,   358,   380,   359,   381,   360,   451,
     374,   352,     0,     0,   446,   447,   445,   439,   440,   435,
     434,   436,   437,   438,     0,     0,     0,   404,     0,     0,
       0,     0,   414,     0,     0,     0,     0,   524,   527,     0,
     572,   575,   351,   576,   691,   714,   717,     0,   720,   710,
       0,   678,   724,     0,   731,     0,   738,     0,     0,   744,
       0,   747,     0,   252,   694,   683,   543,   549,     0,   630,
     631,   559,   562,   561,     0,     0,     0,     0,   798,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   324,
     361,   362,   364,   363,   365,   355,   356,   357,   366,   367,
     353,   368,   369,   358,   359,   360,   352,   292,     0,     0,
       0,   466,     0,     0,     0,   467,     0,   498,     0,     0,
       0,     0,     0,     0,   337,   122,   500,   501,   502,   503,
     504,     0,     0,   784,     0,     0,     0,     0,   323,   783,
       0,     0,     0,     0,   792,   793,     0,   800,     0,     0,
     790,     0,   829,     0,     0,     0,     0,   788,   830,     0,
     820,   785,   849,     0,     0,     0,     0,   851,   852,     0,
       0,     0,   828,   456,     0,     0,     0,   122,     0,    58,
      59,     0,    73,    65,   337,     0,     0,   413,   412,   300,
       0,     0,     0,     0,     0,     0,   335,     0,   125,   472,
       0,   478,     0,   372,   370,   371,     0,     0,   459,     0,
       0,   482,   486,     0,     0,   462,     0,     0,     0,     0,
     405,     0,   409,   457,   415,   592,   612,   121,   525,   526,
     527,   528,   519,   532,   574,   692,   715,   718,   681,   721,
     711,   673,   679,   725,   727,   732,   734,   739,   741,   545,
     745,   551,   748,     0,     0,   629,     0,     0,   806,   809,
       0,   271,   276,     0,   275,     0,     0,   320,   318,     0,
       0,     0,   842,   840,     0,     0,   854,    82,   309,   312,
     315,   846,   844,   815,   824,   822,   812,     0,   125,     0,
       0,     0,   773,   772,   783,     0,   818,     0,     0,   782,
     799,   791,   819,   827,   789,     0,   783,     0,   795,   796,
     803,   787,     0,   255,   258,    31,     0,   220,     0,     0,
       0,     0,     0,   125,    61,     0,    66,     0,   122,     0,
       0,     0,     0,   566,   333,   334,   332,   331,     0,     0,
       0,     0,     0,     0,     0,   393,     0,     0,   483,     0,
     471,     0,   460,     0,   406,     0,     0,   458,   410,   537,
     522,   519,   520,   521,   524,   728,   735,   742,   253,   248,
     632,   560,     0,     0,    77,   272,   277,     0,     0,   319,
     832,   835,   838,     0,     0,   859,     0,     0,     0,     0,
     797,     0,     0,     0,     0,   506,     0,     0,     0,     0,
     801,     0,     0,     0,     0,     0,   794,     0,     0,   250,
       0,     0,     0,   396,   515,   399,     0,    60,     0,    75,
     125,   391,     0,   301,   566,     0,     0,   336,   338,     0,
     325,   341,     0,   514,     0,   512,   394,   509,     0,     0,
       0,   508,   407,     0,   523,   533,   522,     0,     0,   797,
     797,     0,     0,     0,     0,     0,     0,     0,     0,   310,
     313,   316,     0,   816,   797,     0,   468,     0,   340,   507,
     857,     0,     0,   802,     0,   775,   774,   857,   804,   857,
     256,   248,   259,   857,   857,     0,     0,   402,    62,   281,
       0,    67,    71,    72,    69,    70,    68,     0,     0,     0,
       0,     0,     0,     0,   326,   473,   479,     0,   513,   511,
     510,     0,   535,   529,   696,     0,     0,     0,   321,   797,
     797,     0,   859,   859,   855,   859,     0,     0,   469,     0,
       0,   777,   776,     0,     0,     0,     0,     0,     0,   857,
     400,     0,     0,     0,   287,   343,   392,     0,     0,     0,
       0,   327,   474,   480,   484,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   813,     0,     0,
       0,     0,     0,     0,   251,   865,   861,     0,   403,   288,
       0,     0,     0,   305,   485,   534,     0,   807,   810,   322,
       0,     0,   839,   843,   856,   847,   825,   858,   863,   779,
     778,   864,   866,   862,     0,   304,   303,   536,   833,   836,
     302
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1431, -1431, -1431, -1431, -1431, -1431,   535,  1151, -1431, -1431,
   -1431,  1231, -1431, -1431, -1431,   691,  1189, -1431,  1101, -1431,
   -1431,  1156, -1431, -1431, -1431,  -198, -1431, -1431, -1431,  -197,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,  1020,
   -1431, -1431,   -41,   -45, -1431, -1431, -1431,   355,   450,  -518,
    -564,  -785, -1431, -1431, -1431, -1430, -1431, -1431,  -209,  -365,
   -1431,   304, -1431, -1343, -1431,  -184,   277, -1431, -1431, -1431,
   -1431,  -412,   -14, -1431, -1431, -1431, -1431, -1431,  -192,  -189,
    -188, -1431,  -186, -1431, -1431, -1431,  1255, -1431,   301, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431,  -422,  -150,   327,   -93, -1431,  -883,  -436, -1431,
     749, -1431, -1431,  -367,   626, -1431, -1431, -1431, -1373, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,   794, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431,  -148,  -115,  -199,  -116,
      20, -1431, -1431, -1431, -1431, -1431,   844, -1431,  -575, -1431,
   -1431,  -578, -1431, -1431,  -615,  -195,  -566, -1288, -1431,  -349,
   -1431, -1431,  1222, -1431, -1431, -1431,   716,   808,    69, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,  -608,
    -222, -1431,   841, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431,  -373, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,  -204,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431,   842,  -708,   177,  -647,  -890,
   -1431, -1431, -1431, -1135,  -147, -1358, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431,   114,  -479, -1431, -1431, -1431,
     598, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431,  -629, -1431, -1380,  -701, -1431
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    16,   143,    52,    17,   164,   170,   696,   505,
     149,   506,    99,    19,    20,    45,    46,    47,    88,    21,
      39,    40,   507,   508,  1201,  1202,   509,  1347,  1438,   510,
     511,   951,   512,   626,   513,   514,   515,   516,  1132,   171,
     172,    35,    36,    37,   220,    66,    67,    68,    69,    22,
     285,   374,   210,    23,   111,   211,   112,   156,   674,   976,
     518,   375,   519,   821,  1502,  1082,   566,   930,  1428,   932,
    1429,   521,   522,   523,   628,   847,  1395,   524,   525,   526,
     527,   528,   529,   530,   531,   426,   532,   726,  1210,   960,
     533,   534,   883,  1406,   884,  1407,   886,  1408,   535,   850,
    1399,   536,   708,  1451,   537,  1216,  1217,   963,   676,   538,
     783,   952,   539,   641,   977,   541,   542,   543,   949,   544,
    1196,  1506,  1197,  1561,   545,  1050,  1378,   546,   709,  1361,
    1572,  1363,  1573,  1458,  1604,   548,   370,  1384,  1465,  1250,
    1252,  1059,   561,   792,  1531,  1576,   371,   372,   610,   816,
     419,   615,   818,   420,  1156,   620,   574,   390,   307,   308,
     217,   301,    78,   121,    25,   161,   376,    89,    90,   174,
      91,    26,    49,   115,   158,    27,   288,   558,   559,  1055,
     379,   215,   216,    76,   118,   381,    28,   159,   299,   621,
     549,   297,   353,   354,   808,   418,   355,   582,  1262,   801,
     416,   356,   575,  1255,   820,   580,  1260,   576,  1256,   577,
    1257,   579,  1259,   583,  1263,   584,  1385,   585,  1265,   586,
    1386,   587,  1267,   588,  1387,   589,  1270,   590,  1272,   611,
      29,    95,   177,   359,   612,    30,    96,   178,   360,   616,
      31,    94,   176,   358,   607,   550,   915,   957,   916,   917,
     929,  1178,  1172,  1167,  1330,  1100,   551,   843,  1392,   844,
    1393,   895,  1412,   892,  1410,   918,   698,   552,   893,  1411,
     919,   553,  1104,  1473,  1105,  1474,  1106,  1475,   854,  1403,
     890,  1409,   692,   699,   554,  1550,   938,   555
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,    70,   287,   691,   796,   352,   842,   943,   540,   944,
     517,   956,   817,   221,   815,   570,   878,  1145,   846,   727,
     810,   640,   812,   357,   814,  1073,  1163,  1075,  1320,  1077,
     633,   639,  1175,   562,   594,   677,   678,  1096,  -120,   130,
    1152,   922,   908,   791,   604,   596,  1468,  1173,    84,    70,
      70,    70,  1483,  1179,  1186,   669,    32,    33,   151,   908,
    1382,  1190,    53,  1339,   728,  1446,     2,    50,    54,   837,
     122,   123,     3,   104,   105,   106,   837,   907,   517,   920,
     710,   924,   909,    85,   417,   218,   571,    48,  1064,   936,
      70,    70,    70,    70,  1283,     4,   572,     5,   940,     6,
      61,  1575,   517,   165,   166,     7,   750,   686,   688,   753,
     754,  1535,  1536,    51,   729,     8,    98,  1554,    92,  1555,
    1383,     9,  1212,  1557,  1558,   368,  1546,   634,   635,    62,
     218,   839,  1213,  1560,   417,   431,   432,   838,   839,  1284,
     840,   305,   219,   841,   150,    10,  1606,    71,   573,   958,
     427,   352,   718,   382,  1108,   442,  1520,   900,  1556,   306,
     139,   447,   797,   909,   828,   212,   352,    11,    12,   429,
    1162,   822,  1070,   302,    77,  1283,   213,  1130,   303,  1597,
    1420,  1580,  1581,   428,  1214,   138,  1071,   219,  1598,  1215,
    1139,   781,   782,  1142,   595,   352,   719,   352,   461,   462,
     305,    63,    38,   959,   838,   597,    34,   838,    58,   517,
     789,   730,   731,   351,   838,   855,   857,   124,   306,   636,
     838,   838,   125,   598,   126,    86,   856,   127,   838,   599,
     838,    80,   464,   465,   837,    59,    87,   643,   637,    58,
     891,   790,  1380,   894,  1155,  1205,    13,   753,   754,   286,
     167,   660,  1044,  1045,  1162,   168,   897,   169,   352,   352,
     127,  1307,  1251,   384,   837,    14,    59,   424,   128,  1315,
      64,   877,    58,   517,  1155,  1449,    15,   368,   837,    65,
      98,  1046,   218,   659,  1493,   837,   286,  1587,  1422,   480,
     481,   482,   964,  1047,   896,  1491,   839,   955,  1162,    59,
     730,   731,    79,   517,   837,   740,   741,   742,   743,   493,
     425,    81,  1162,   286,  1590,   750,  1162,   752,   753,   754,
     755,  1348,   503,   673,   627,   756,   839,   758,   759,   781,
     782,  1607,  1078,  1191,  1048,  1049,   405,   369,   618,   219,
     839,   501,   730,   731,   417,  1608,  1079,   839,  1065,  1612,
    1164,  1165,  1162,   352,   352,   352,   619,   352,   352,   351,
    1162,   352,  1053,   352,   560,   352,   839,   352,    80,  1162,
     556,   799,   800,   802,   351,   804,   805,   961,  1166,   809,
      82,   811,   557,   813,  1061,  1616,   776,   777,   778,   779,
     780,    97,   956,  1628,   225,   180,   742,   743,   107,  1218,
     781,   782,  1629,   351,   750,   351,   752,   753,   754,   755,
      41,    42,    43,  1080,   756,  1273,   845,  1206,   100,   101,
     102,   226,   352,   108,  1419,   351,  1099,  1081,    58,  1274,
    1092,   517,  1169,  1360,  1204,  1170,  1425,   945,   742,   743,
     881,    44,   946,   417,  1207,   352,   750,  1068,  1182,   753,
     754,   755,   858,    80,  1319,    59,   756,   107,  1187,   152,
     153,   154,   155,  1171,    98,    58,   351,   351,   858,  1351,
    1094,  1095,  1153,  1273,   945,  1161,  1277,   517,   383,  1244,
     947,  1107,  1247,    72,    73,   931,    74,  1333,   858,   781,
     782,   710,    59,  1351,   858,  1282,  1353,   858,    53,   710,
    1349,  1358,  1271,  1269,    54,   950,   730,   731,   968,   972,
    1133,  1134,  1314,  1136,    75,  1456,  1161,  1138,  1352,  1140,
    1141,  1462,  1143,  1414,  1489,   592,  1359,  1326,  1273,   289,
     644,   781,   782,   290,  1012,   286,   417,  1223,  1224,  1225,
    1069,  1518,  1334,   858,   593,  -726,  1397,   291,   292,   645,
    -726,   406,   293,   294,   295,   296,   103,   661,  1436,  -733,
    1398,   351,   351,   351,  -733,   351,   351,  -740,  -726,   351,
     785,   351,  -740,   351,   664,   351,   662,   786,   407,   408,
    1198,  1308,  -733,   605,  -401,   613,   131,  1007,  1234,  -401,
    -740,  1199,  1200,   665,  1067,   606,  1239,   614,   352,  1008,
    1235,   417,   742,   743,   417,  1072,   352,  -401,  1240,   352,
     750,   406,   752,   753,   754,   755,  1083,   849,   352,    87,
     756,   352,   352,  1317,  1086,   352,  1445,  1087,   352,   389,
     351,   406,   352,  1343,   109,  1318,  1091,   909,   407,   408,
     110,   409,  1442,  1097,  1058,   410,  1103,   148,   387,  1283,
     113,   388,   520,   351,   389,  1517,   114,  1327,   407,   408,
    1328,   352,   352,  1329,   352,   137,  1405,  1127,   352,   116,
     352,   352,   119,   352,  1280,   117,  1176,  1169,   120,  1177,
     141,   776,   777,   778,   779,   780,   142,  1248,  1417,    41,
      42,    43,  1424,  1249,   858,   781,   782,   948,   858,  1128,
    1427,   409,   133,  1423,  1478,   410,   858,   411,   352,  1430,
     352,   412,  1431,  1129,   413,   858,  1505,   104,   858,   106,
     520,   409,   858,   417,   571,   410,  1083,  1074,  1083,   414,
    1310,  1289,  1290,  1291,   572,   415,   417,   417,   417,   417,
    1076,  1281,  1288,  1526,   520,   417,  1183,  1519,  1184,  1574,
     389,  1325,   136,   730,   731,  1294,   140,  1332,   162,   163,
     933,   934,   162,   163,  1336,   503,   673,   411,  1338,   222,
     223,   412,   144,  1144,   413,   145,    70,   134,   135,   795,
     104,   105,   106,   146,  1440,   147,   573,   411,   157,   414,
     160,   412,    85,   179,   413,   415,   104,   675,   675,   675,
     222,   223,   224,  1492,  1366,   175,   351,   214,   228,   414,
    1232,   926,   927,   928,   351,   415,  1375,   351,    55,    56,
      57,   227,   540,  1342,   517,   229,   351,   230,   286,   351,
     351,   282,   283,   351,   284,   298,   351,   300,  1547,   361,
     351,  1583,  1584,   362,  1585,   352,   363,   716,  1543,   742,
     743,   520,   304,   364,   365,   366,   352,   750,   373,   752,
     753,   754,   755,  1258,   367,   378,   377,   756,   380,   351,
     351,   385,   351,   386,   716,   391,   351,   392,   351,   351,
    1522,   351,   352,   352,   352,   393,   394,  1416,  1577,  1578,
     395,   396,   397,   398,  1582,   404,   406,   399,   400,  1586,
     401,   402,   403,   417,   422,   421,   352,   423,   568,   569,
     563,   581,   623,   602,   624,   520,   351,  1432,   351,   632,
     650,   651,   631,   407,   408,   652,   648,   653,   654,   656,
     778,   779,   780,  1610,  1611,   655,   679,   406,  1591,   663,
     666,   680,   781,   782,   667,   520,   681,   682,  1499,   695,
     683,  1503,   700,   712,  1568,  1569,   684,   713,   715,   720,
     711,   721,   722,   723,   407,   408,   724,   822,   725,   716,
      15,   784,   406,   659,   352,   787,   794,   693,   806,   807,
     613,   823,   826,   827,   831,   829,   409,   832,  1452,  1344,
     410,   728,   406,   830,   833,   836,   851,   716,   876,   407,
     408,  1600,   835,   848,   858,   887,   852,   882,   406,   520,
     520,   520,   520,   520,   520,   520,   520,   853,   888,   407,
     408,   899,   520,   520,   937,   939,   941,   409,   520,   942,
     962,   410,  1057,  1003,  1060,   407,   408,   520,   520,  1066,
     406,  1084,   520,   520,   520,  1089,   520,  1090,  1154,  1161,
    1192,   352,   411,   351,  1542,   675,   412,  1168,  1278,   413,
    1545,  1181,   409,  1188,   351,   571,   410,   407,   408,  1388,
    1194,  1394,  1521,   520,   414,   572,  1209,  1242,  1219,   822,
     415,  1193,   409,  1208,  1221,  1195,   410,   640,  1220,  1246,
     351,   351,   351,   411,  1251,   406,  1261,   412,   409,  1279,
     413,  1222,   410,  1253,  1226,  1227,  1237,  1238,  1264,  1266,
    1276,  1268,  1286,  1293,   351,   414,  1295,  1368,  1309,   520,
    1296,   415,   407,   408,  1302,   716,  1305,   573,   411,  1321,
     409,  1324,   412,  1331,   410,   413,  1437,  1354,  1355,   352,
     857,   352,  1356,  1357,  1369,  1371,  1567,  1376,   411,  1389,
     414,  1450,   412,  1379,  1292,   413,   415,  1390,  1396,  1454,
    1413,  1439,  1421,  1426,   411,  1443,  1444,  1441,   412,   547,
     414,   413,  1448,  1464,  1469,  1470,   415,  1471,  1476,   567,
    1477,  1098,   351,  1482,  1484,   409,   414,  1507,   578,   410,
    1485,  1501,   415,  1534,  1537,  1539,   411,  1544,   591,  1540,
     412,  1549,  1298,   413,  1541,  1450,  1588,  1566,   601,  1589,
    1592,  1593,  1594,  1595,  1596,  1605,  1618,   617,   414,  1613,
     406,  1621,  1622,   352,   415,   716,  1614,   625,  1615,  1623,
     129,   935,    18,    83,   173,   675,   352,   642,   132,  1508,
    1056,  1511,   647,  1189,   649,   309,  1512,   407,   408,  1513,
    1514,   411,  1515,   658,  1527,   412,    24,  1299,   413,   351,
    1211,   670,   671,   672,  1488,  1447,  1466,  1532,  1467,   717,
    1381,   622,  1533,   414,    93,   788,   690,  1285,  1498,   415,
     629,   630,     0,  1323,   694,     0,   923,   690,     0,     0,
     701,   702,   703,   704,   705,     0,     0,   352,     0,     0,
     675,   675,   675,   714,     0,   716,     0,   716,  1565,   716,
     409,   716,     0,   716,   410,   716,     0,   716,     0,   716,
       0,   730,   731,     0,   716,     0,   716,     0,     0,     0,
       0,     0,   716,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   716,     0,   716,   351,     0,   351,
       0,   716,     0,   716,     0,   716,     0,     0,   716,   979,
     981,   983,   985,   987,   989,   991,     0,     0,   798,     0,
       0,   996,   998,     0,     0,     0,   411,  1004,     0,     0,
     412,     0,  1300,   413,     0,     0,  1016,  1018,     0,   716,
       0,  1023,  1025,  1027,     0,  1030,   825,  1627,   414,     0,
       0,     0,     0,     0,   415,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   740,   741,   742,   743,   744,
     716,     0,   747,   748,   749,   750,     0,   752,   753,   754,
     755,   351,   642,     0,     0,   756,     0,   758,   759,   690,
     879,     0,   880,     0,   351,     0,     0,   885,     0,     0,
       0,     0,     0,   889,     0,     0,     0,     0,     0,     0,
       0,     0,   898,     0,     0,     0,   520,     0,     0,     0,
       0,     0,     0,   901,   902,   903,   904,   905,   906,     0,
     914,     0,   914,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   948,  1346,   774,   775,   776,   777,   778,   779,
     780,     0,   730,   731,     0,   351,     0,     0,     0,     0,
     781,   782,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   978,   980,   982,   984,
     986,   988,   990,   992,   993,   994,     0,     0,   995,   997,
     999,  1000,  1001,  1002,     0,  1005,  1006,   948,  1009,  1010,
    1011,  1013,  1014,  1015,  1017,  1019,  1020,  1021,  1022,  1024,
    1026,  1028,  1029,  1031,  1033,  1034,  1035,  1036,  1037,  1038,
    1039,  1040,  1041,  1042,  1043,     0,  1051,     0,     0,     0,
    1054,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1062,     0,     0,     0,     0,     0,   740,   741,   742,   743,
     744,     0,     0,   747,   748,   749,   750,     0,   752,   753,
     754,   755,     0,     0,     0,     0,   756,     0,   758,   759,
       0,     0,     0,     0,   762,   763,   764,     0,     0,  1088,
     768,     0,     0,     0,     0,     0,  1093,     0,     0,     0,
       0,     0,     0,  1102,     0,     0,     0,     0,     0,     0,
       0,     0,  1109,  1110,  1111,  1112,  1113,  1114,  1115,  1116,
    1117,  1118,  1119,  1120,  1121,  1122,  1123,  1124,  1125,  1126,
     770,     0,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   781,   782,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   701,  1158,     0,     0,     0,
       0,     0,   431,   432,     0,     0,     0,     0,     0,  1180,
       0,   690,   437,   438,   439,   440,   441,     0,  1185,     0,
       0,   690,   442,     0,   444,     0,  1109,     0,   447,     0,
     406,     0,     0,     0,     0,  1203,   449,     0,     0,     0,
       0,     0,   452,     0,     0,   453,     0,     0,   454,     0,
     908,     0,   457,     0,     0,     0,     0,   407,   408,     0,
       0,     0,   564,     0,     0,   461,   462,     0,   316,   317,
     318,     0,   320,   321,   322,   323,   324,   463,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,     0,
     338,   339,   340,     0,     0,   343,   344,   345,   346,   464,
     465,   466,     0,     0,     0,     0,     0,     0,   406,     0,
       0,     0,     0,     0,   468,   469,     0,     0,     0,     0,
     409,     0,     0,     0,   410,     0,     0,     0,     0,  1236,
       0,     0,     0,  1241,     0,   407,   408,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   473,   474,   475,
     476,   477,     0,   478,   909,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   910,   565,   491,   492,
       0,     0,     0,     0,     0,     0,   493,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   411,     0,     0,     0,
     412,     0,     0,   911,   497,   498,     0,    14,   409,     0,
     499,   500,   410,     0,     0,     0,     0,     0,   912,     0,
     913,     0,   503,   504,   415,     0,  1297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     -68,     0,     0,     0,     0,     0,     0,     0,  1311,  1312,
    1313,   730,   731,     0,  1316,     0,     0,     0,     0,     0,
       0,     0,  1322,   914,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   411,     0,     0,     0,   412,     0,
    1301,   413,     0,   547,     0,     0,     0,     0,  1340,  1341,
       0,     0,     0,     0,     0,  1345,   414,     0,   730,   731,
    1109,     0,   415,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1362,     0,  1364,     0,     0,     0,
       0,  1367,     0,     0,     0,     0,     0,  1370,     0,     0,
       0,  1373,     0,     0,     0,     0,     0,   732,   733,   734,
     735,   736,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   749,   750,   751,   752,   753,   754,
     755,     0,     0,     0,     0,   756,   757,   758,   759,   760,
     761,  1391,     0,   762,   763,   764,   765,   766,   767,   768,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   690,
       0,     0,   740,   741,   742,   743,   744,     0,     0,   747,
       0,     0,   750,     0,   752,   753,   754,   755,     0,     0,
       0,     0,   756,     0,   758,   759,     0,     0,   769,   770,
       0,   771,   772,   773,   774,   775,   776,   777,   778,   779,
     780,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     781,   782,     0,     0,     0,   503,   673,     0,     0,     0,
     730,   731,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1459,     0,  1460,     0,     0,     0,     0,
    1463,   774,   775,   776,   777,   778,   779,   780,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   781,   782,     0,
       0,     0,  1472,     0,     0,     0,     0,     0,     0,     0,
    1479,  1480,  1481,     0,     0,     0,     0,  1487,     0,     0,
       0,  1490,     0,     0,     0,  1494,  1495,  1496,  1497,     0,
       0,   690,  1500,     0,   690,  1504,     0,     0,     0,     0,
       0,     0,  1516,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1524,   740,   741,   742,   743,   744,     0,
       0,   747,   748,   749,   750,     0,   752,   753,   754,   755,
       0,     0,     0,     0,   756,     0,   758,   759,     0,     0,
       0,   690,   762,   763,   764,     0,     0,     0,   768,     0,
       0,     0,     0,     0,     0,  1551,  1552,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1559,
       0,     0,     0,     0,  1564,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   406,  1570,  1571,   770,     0,
     771,   772,   773,   774,   775,   776,   777,   778,   779,   780,
       0,  1579,     0,     0,     0,     0,     0,     0,     0,   781,
     782,     0,   407,   408,   503,   673,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1599,     0,     0,
       0,     0,  1601,  1602,   430,     0,     0,   431,   432,     3,
       0,   433,   434,   435,     0,   436,     0,   437,   438,   439,
     440,   441,  1617,     0,  1619,  1620,     0,   442,   443,   444,
     445,   446,     0,   447,  1624,     0,     0,     0,     0,     0,
     448,   449,     0,     0,   450,   409,   451,   452,     0,   410,
     453,     0,     8,   454,   455,     0,   456,   457,     0,     0,
     458,   459,     0,     0,     0,     0,     0,   460,     0,     0,
     461,   462,     0,   316,   317,   318,     0,   320,   321,   322,
     323,   324,   463,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,     0,   338,   339,   340,     0,     0,
     343,   344,   345,   346,   464,   465,   466,   467,     0,     0,
       0,   411,     0,     0,     0,   412,     0,  1303,   413,   468,
     469,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   414,   470,   471,   472,     0,     0,   415,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   473,   474,   475,   476,   477,     0,   478,     0,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,    59,   490,   491,   492,     0,     0,     0,     0,     0,
       0,   493,   494,   495,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   496,   497,
     498,     0,    14,     0,     0,   499,   500,     0,     0,     0,
       0,     0,     0,   501,     0,   502,   430,   503,   504,   431,
     432,     3,     0,   433,   434,   435,     0,   436,     0,   437,
     438,   439,   440,   441,     0,     0,     0,     0,     0,   442,
     443,   444,   445,   446,     0,   447,     0,     0,     0,     0,
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
       0,     0,     0,     0,   473,   474,   475,   476,   477,   406,
     478,     0,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,    59,   490,   491,   492,     0,     0,     0,
       0,     0,     0,   493,  1337,   495,   407,   408,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   498,     0,    14,     0,     0,   499,   500,     0,
       0,   431,   432,     0,     0,   501,     0,   502,     0,   503,
     504,   437,   438,   439,   440,   441,     0,     0,     0,     0,
       0,   442,     0,   444,     0,     0,     0,   447,     0,   406,
       0,     0,     0,     0,     0,   449,     0,     0,     0,   409,
       0,   452,     0,   410,   453,     0,     0,   454,     0,     0,
       0,   457,     0,     0,     0,     0,   407,   408,     0,     0,
       0,   564,     0,     0,   461,   462,     0,   316,   317,   318,
       0,   320,   321,   322,   323,   324,   463,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,     0,   338,
     339,   340,     0,     0,   343,   344,   345,   346,   464,   465,
     466,     0,     0,     0,     0,   411,     0,     0,     0,   412,
       0,  1304,   413,   468,   469,     0,     0,     0,     0,   409,
       0,     0,     0,   410,     0,     0,     0,   414,     0,     0,
       0,     0,     0,   415,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   473,   474,   475,   476,
     477,   406,   478,   909,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   910,   565,   491,   492,     0,
       0,     0,     0,     0,     0,   493,     0,     0,   407,   408,
       0,     0,     0,     0,     0,   411,     0,     0,     0,   412,
       0,     0,   911,   497,   498,     0,    14,     0,     0,   499,
     500,     0,     0,     0,   431,   432,     0,   912,     0,   921,
       0,   503,   504,   415,   437,   438,   439,   440,   441,     0,
       0,     0,     0,     0,   442,     0,   444,     0,     0,     0,
     447,     0,   596,     0,     0,     0,     0,     0,   449,     0,
       0,   409,     0,     0,   452,   410,     0,   453,     0,     0,
     454,     0,     0,     0,   457,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   564,     0,     0,   461,   462,     0,
     316,   317,   318,     0,   320,   321,   322,   323,   324,   463,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,     0,   338,   339,   340,     0,     0,   343,   344,   345,
     346,   464,   465,   466,     0,     0,     0,   411,     0,     0,
       0,   412,     0,  1306,   413,     0,   468,   469,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   414,
       0,     0,     0,     0,     0,   415,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   473,
     474,   475,   476,   477,   406,   478,     0,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,    59,   565,
     491,   492,     0,     0,     0,     0,     0,     0,   493,     0,
       0,   407,   408,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   597,     0,     0,   496,   497,   498,     0,    14,
       0,     0,   499,   500,     0,     0,     0,   431,   432,     0,
    1157,     0,   502,     0,   503,   504,   599,   437,   438,   439,
     440,   441,     0,     0,     0,     0,     0,   442,     0,   444,
       0,     0,     0,   447,     0,     0,     0,     0,     0,     0,
       0,   449,     0,     0,   409,     0,     0,   452,   410,     0,
     453,     0,     0,   454,     0,     0,     0,   457,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   564,     0,     0,
     461,   462,     0,   316,   317,   318,     0,   320,   321,   322,
     323,   324,   463,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,     0,   338,   339,   340,     0,     0,
     343,   344,   345,   346,   464,   465,   466,     0,     0,     0,
     411,     0,     0,     0,   412,     0,  1400,   413,     0,   468,
     469,     0,     0,     0,     0,     0,     0,     0,   638,     0,
       0,     0,   414,     0,   470,   471,   472,     0,   415,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   473,   474,   475,   476,   477,   406,   478,     0,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,    59,   565,   491,   492,     0,     0,     0,     0,     0,
       0,   493,     0,     0,   407,   408,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   496,   497,
     498,     0,    14,     0,     0,   499,   500,     0,     0,   431,
     432,     0,     0,   501,     0,   502,     0,   503,   504,   437,
     438,   439,   440,   441,     0,     0,     0,     0,     0,   442,
       0,   444,     0,     0,     0,   447,     0,     0,     0,     0,
       0,     0,     0,   449,     0,     0,     0,   409,     0,   452,
       0,   410,   453,     0,     0,   454,     0,     0,     0,   457,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   564,
       0,     0,   461,   462,     0,   316,   317,   318,     0,   320,
     321,   322,   323,   324,   463,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,     0,   338,   339,   340,
       0,     0,   343,   344,   345,   346,   464,   465,   466,     0,
       0,     0,     0,   411,     0,     0,     0,   412,     0,  1401,
     413,   468,   469,     0,     0,     0,     0,     0,     0,     0,
     668,     0,     0,     0,     0,   414,   470,   471,   472,     0,
       0,   415,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   473,   474,   475,   476,   477,   406,
     478,     0,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,    59,   565,   491,   492,     0,     0,     0,
       0,     0,     0,   493,     0,     0,   407,   408,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   498,     0,    14,     0,     0,   499,   500,     0,
       0,   431,   432,     0,     0,   501,     0,   502,     0,   503,
     504,   437,   438,   439,   440,   441,     0,     0,     0,     0,
       0,   442,  1509,   444,   445,     0,     0,   447,     0,     0,
       0,     0,     0,     0,     0,   449,     0,     0,     0,   409,
       0,   452,     0,   410,   453,     0,     0,   454,   455,     0,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   564,     0,     0,   461,   462,     0,   316,   317,   318,
       0,   320,   321,   322,   323,   324,   463,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,     0,   338,
     339,   340,     0,     0,   343,   344,   345,   346,   464,   465,
     466,  1510,     0,     0,     0,   411,     0,     0,     0,   412,
       0,  1402,   413,   468,   469,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   414,     0,     0,
       0,     0,     0,   415,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   473,   474,   475,   476,
     477,   406,   478,     0,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,    59,   565,   491,   492,     0,
       0,     0,     0,     0,     0,   493,     0,     0,   407,   408,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   496,   497,   498,     0,    14,     0,     0,   499,
     500,     0,     0,   431,   432,     0,     0,   501,     0,   502,
       0,   503,   504,   437,   438,   439,   440,   441,     0,     0,
       0,     0,     0,   442,     0,   444,     0,     0,     0,   447,
       0,     0,     0,     0,     0,     0,     0,   449,     0,     0,
       0,   409,     0,   452,     0,   410,   453,     0,     0,   454,
       0,     0,     0,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   564,     0,     0,   461,   462,     0,   316,
     317,   318,     0,   320,   321,   322,   323,   324,   463,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
       0,   338,   339,   340,     0,     0,   343,   344,   345,   346,
     464,   465,   466,     0,     0,     0,     0,   411,     0,     0,
       0,   412,     0,  1404,   413,   468,   469,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   414,
     470,   471,   472,     0,     0,   415,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   473,   474,
     475,   476,   477,   406,   478,     0,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,    59,   565,   491,
     492,     0,     0,     0,     0,     0,     0,   493,     0,     0,
     407,   408,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   496,   497,   498,     0,    14,     0,
       0,   499,   500,     0,     0,   431,   432,     0,     0,   501,
       0,   502,     0,   503,   504,   437,   438,   439,   440,   441,
       0,     0,     0,     0,     0,   442,     0,   444,     0,     0,
     406,   447,     0,     0,     0,     0,     0,     0,     0,   449,
       0,     0,     0,   409,     0,   452,     0,   410,   453,     0,
       0,   454,     0,     0,     0,   457,     0,   407,   408,     0,
       0,     0,     0,     0,     0,   564,     0,     0,   461,   462,
       0,   316,   317,   318,     0,   320,   321,   322,   323,   324,
     463,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,     0,   338,   339,   340,     0,     0,   343,   344,
     345,   346,   464,   465,   466,     0,     0,     0,     0,   411,
       0,     0,     0,   412,     0,  1435,   413,   468,   469,     0,
     409,     0,     0,     0,   410,     0,     0,     0,     0,     0,
       0,   414,   973,   974,   975,     0,     0,   415,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     473,   474,   475,   476,   477,     0,   478,     0,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,    59,
     565,   491,   492,     0,     0,     0,     0,     0,     0,   493,
       0,     0,     0,     0,     0,     0,   411,     0,     0,     0,
     412,     0,  1525,   413,     0,     0,   496,   497,   498,     0,
      14,     0,     0,   499,   500,     0,     0,     0,   414,   431,
     432,   501,     0,   502,   415,   503,   504,   706,     0,   437,
     438,   439,   440,   441,     0,     0,     0,     0,     0,   442,
       0,   444,     0,     0,     0,   447,     0,     0,     0,     0,
       0,     0,     0,   449,     0,     0,     0,     0,     0,   452,
       0,     0,   453,   707,     0,   454,     0,     0,     0,   457,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   564,
       0,     0,   461,   462,     0,   316,   317,   318,     0,   320,
     321,   322,   323,   324,   463,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,     0,   338,   339,   340,
       0,     0,   343,   344,   345,   346,   464,   465,   466,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   468,   469,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   473,   474,   475,   476,   477,     0,
     478,     0,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,    59,   565,   491,   492,     0,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   498,     0,    14,     0,     0,   499,   500,     0,
       0,     0,     0,   431,   432,   501,   600,   502,     0,   503,
     504,   706,     0,   437,   438,   439,   440,   441,     0,     0,
       0,     0,     0,   442,     0,   444,     0,     0,     0,   447,
       0,     0,     0,     0,     0,     0,     0,   449,     0,     0,
       0,     0,     0,   452,     0,     0,   453,   707,     0,   454,
       0,     0,     0,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   564,     0,     0,   461,   462,     0,   316,
     317,   318,     0,   320,   321,   322,   323,   324,   463,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
       0,   338,   339,   340,     0,     0,   343,   344,   345,   346,
     464,   465,   466,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,   469,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   473,   474,
     475,   476,   477,     0,   478,   909,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   910,   565,   491,
     492,     0,     0,     0,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   496,   497,   498,     0,    14,     0,
       0,   499,   500,     0,     0,     0,     0,   431,   432,   501,
       0,   502,     0,   503,   504,   706,     0,   437,   438,   439,
     440,   441,     0,     0,     0,     0,     0,   442,     0,   444,
       0,     0,     0,   447,     0,     0,     0,     0,     0,     0,
       0,   449,     0,     0,     0,     0,     0,   452,     0,     0,
     453,   707,     0,   454,     0,     0,     0,   457,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   564,     0,     0,
     461,   462,     0,   316,   317,   318,     0,   320,   321,   322,
     323,   324,   463,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,     0,   338,   339,   340,     0,     0,
     343,   344,   345,   346,   464,   465,   466,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   468,
     469,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   473,   474,   475,   476,   477,     0,   478,     0,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,    59,   565,   491,   492,     0,     0,     0,     0,     0,
       0,   493,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   496,   497,
     498,     0,    14,     0,     0,   499,   500,     0,     0,     0,
       0,   431,   432,   501,   823,   502,     0,   503,   504,   706,
       0,   437,   438,   439,   440,   441,     0,     0,     0,     0,
       0,   442,     0,   444,     0,     0,     0,   447,     0,     0,
       0,     0,     0,     0,     0,   449,     0,     0,     0,     0,
       0,   452,     0,     0,   453,   707,     0,   454,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   564,     0,     0,   461,   462,     0,   316,   317,   318,
       0,   320,   321,   322,   323,   324,   463,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,     0,   338,
     339,   340,     0,     0,   343,   344,   345,   346,   464,   465,
     466,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   473,   474,   475,   476,
     477,     0,   478,     0,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,    59,   565,   491,   492,     0,
       0,     0,     0,     0,     0,   493,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   496,   497,   498,     0,    14,     0,     0,   499,
     500,     0,     0,   431,   432,     0,     0,   501,     0,   502,
       0,   503,   504,   437,   438,   439,   440,   441,     0,     0,
       0,     0,     0,   442,     0,   444,     0,     0,     0,   447,
       0,     0,     0,     0,     0,     0,     0,   449,     0,     0,
       0,     0,     0,   452,     0,     0,   453,     0,     0,   454,
       0,     0,     0,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   564,     0,     0,   461,   462,     0,   316,
     317,   318,     0,   320,   321,   322,   323,   324,   463,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
       0,   338,   339,   340,     0,     0,   343,   344,   345,   346,
     464,   465,   466,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,   469,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   473,   474,
     475,   476,   477,     0,   478,   909,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   910,   565,   491,
     492,     0,     0,     0,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   496,   497,   498,     0,    14,     0,
       0,   499,   500,     0,     0,   431,   432,     0,     0,   953,
       0,   502,   954,   503,   504,   437,   438,   439,   440,   441,
       0,     0,     0,     0,     0,   442,     0,   444,     0,     0,
       0,   447,     0,     0,     0,     0,     0,     0,     0,   449,
       0,     0,     0,     0,     0,   452,     0,     0,   453,     0,
       0,   454,     0,     0,     0,   457,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   564,     0,     0,   461,   462,
       0,   316,   317,   318,     0,   320,   321,   322,   323,   324,
     463,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,     0,   338,   339,   340,     0,     0,   343,   344,
     345,   346,   464,   465,   466,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   468,   469,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     473,   474,   475,   476,   477,     0,   478,     0,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,    59,
     565,   491,   492,     0,     0,     0,     0,     0,     0,   493,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   496,   497,   498,     0,
      14,     0,     0,   499,   500,     0,     0,   431,   432,     0,
       0,   501,   600,   502,     0,   503,   504,   437,   438,   439,
     440,   441,     0,     0,     0,     0,     0,   442,     0,   444,
       0,     0,     0,   447,     0,     0,     0,     0,     0,     0,
       0,   449,     0,     0,     0,     0,     0,   452,     0,     0,
     453,     0,     0,   454,     0,     0,     0,   457,     0,     0,
       0,     0,     0,   646,     0,     0,     0,   564,     0,     0,
     461,   462,     0,   316,   317,   318,     0,   320,   321,   322,
     323,   324,   463,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,     0,   338,   339,   340,     0,     0,
     343,   344,   345,   346,   464,   465,   466,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   468,
     469,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   473,   474,   475,   476,   477,     0,   478,     0,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,    59,   565,   491,   492,     0,     0,     0,     0,     0,
       0,   493,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   496,   497,
     498,     0,    14,     0,     0,   499,   500,     0,     0,   431,
     432,     0,     0,   501,     0,   502,     0,   503,   504,   437,
     438,   439,   440,   441,     0,     0,     0,     0,     0,   442,
       0,   444,     0,     0,     0,   447,     0,     0,     0,     0,
       0,     0,     0,   449,     0,     0,     0,     0,     0,   452,
       0,     0,   453,     0,     0,   454,     0,     0,     0,   457,
       0,     0,   657,     0,     0,     0,     0,     0,     0,   564,
       0,     0,   461,   462,     0,   316,   317,   318,     0,   320,
     321,   322,   323,   324,   463,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,     0,   338,   339,   340,
       0,     0,   343,   344,   345,   346,   464,   465,   466,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   468,   469,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   473,   474,   475,   476,   477,     0,
     478,     0,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,    59,   565,   491,   492,     0,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   498,     0,    14,     0,     0,   499,   500,     0,
       0,     0,     0,   431,   432,   501,     0,   502,     0,   503,
     504,   689,     0,   437,   438,   439,   440,   441,     0,     0,
       0,     0,     0,   442,     0,   444,     0,     0,     0,   447,
       0,     0,     0,     0,     0,     0,     0,   449,     0,     0,
       0,     0,     0,   452,     0,     0,   453,     0,     0,   454,
       0,     0,     0,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   564,     0,     0,   461,   462,     0,   316,
     317,   318,     0,   320,   321,   322,   323,   324,   463,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
       0,   338,   339,   340,     0,     0,   343,   344,   345,   346,
     464,   465,   466,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,   469,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   473,   474,
     475,   476,   477,     0,   478,     0,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,    59,   565,   491,
     492,     0,     0,     0,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   496,   497,   498,     0,    14,     0,
       0,   499,   500,     0,     0,   431,   432,     0,     0,   501,
       0,   502,     0,   503,   504,   437,   438,   439,   440,   441,
       0,     0,     0,     0,     0,   442,     0,   444,     0,     0,
       0,   447,     0,     0,     0,     0,     0,     0,     0,   449,
       0,     0,     0,     0,     0,   452,     0,     0,   453,     0,
       0,   454,     0,     0,     0,   457,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   564,     0,     0,   461,   462,
       0,   316,   317,   318,     0,   320,   321,   322,   323,   324,
     463,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,     0,   338,   339,   340,     0,     0,   343,   344,
     345,   346,   464,   465,   466,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   468,   469,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     473,   474,   475,   476,   477,     0,   478,     0,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,    59,
     565,   491,   492,     0,     0,     0,     0,     0,     0,   493,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   693,     0,   496,   497,   498,     0,
      14,     0,     0,   499,   500,     0,     0,     0,     0,   431,
     432,   501,     0,   502,     0,   503,   504,   697,     0,   437,
     438,   439,   440,   441,     0,     0,     0,     0,     0,   442,
       0,   444,     0,     0,     0,   447,     0,     0,     0,     0,
       0,     0,     0,   449,     0,     0,     0,     0,     0,   452,
       0,     0,   453,     0,     0,   454,     0,     0,     0,   457,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   564,
       0,     0,   461,   462,     0,   316,   317,   318,     0,   320,
     321,   322,   323,   324,   463,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,     0,   338,   339,   340,
       0,     0,   343,   344,   345,   346,   464,   465,   466,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   468,   469,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   473,   474,   475,   476,   477,     0,
     478,     0,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,    59,   565,   491,   492,     0,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   498,     0,    14,     0,     0,   499,   500,     0,
       0,   431,   432,     0,     0,   501,     0,   502,     0,   503,
     504,   437,   438,   439,   440,   441,     0,     0,  1032,     0,
       0,   442,     0,   444,     0,     0,     0,   447,     0,     0,
       0,     0,     0,     0,     0,   449,     0,     0,     0,     0,
       0,   452,     0,     0,   453,     0,     0,   454,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   564,     0,     0,   461,   462,     0,   316,   317,   318,
       0,   320,   321,   322,   323,   324,   463,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,     0,   338,
     339,   340,     0,     0,   343,   344,   345,   346,   464,   465,
     466,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   473,   474,   475,   476,
     477,     0,   478,     0,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,    59,   565,   491,   492,     0,
       0,     0,     0,     0,     0,   493,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   496,   497,   498,     0,    14,     0,     0,   499,
     500,     0,     0,   431,   432,     0,     0,   501,     0,   502,
       0,   503,   504,   437,   438,   439,   440,   441,     0,     0,
       0,     0,     0,   442,     0,   444,     0,     0,     0,   447,
       0,     0,     0,     0,     0,     0,     0,   449,     0,     0,
       0,     0,     0,   452,     0,     0,   453,     0,     0,   454,
       0,     0,     0,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   564,     0,     0,   461,   462,     0,   316,
     317,   318,     0,   320,   321,   322,   323,   324,   463,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
       0,   338,   339,   340,     0,     0,   343,   344,   345,   346,
     464,   465,   466,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,   469,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   473,   474,
     475,   476,   477,     0,   478,     0,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,    59,   565,   491,
     492,     0,     0,     0,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   496,   497,   498,     0,    14,     0,
       0,   499,   500,     0,     0,   431,   432,     0,     0,   501,
       0,   502,  1052,   503,   504,   437,   438,   439,   440,   441,
       0,     0,     0,     0,     0,   442,     0,   444,     0,     0,
       0,   447,     0,     0,     0,     0,     0,     0,     0,   449,
       0,     0,     0,     0,     0,   452,     0,     0,   453,     0,
       0,   454,     0,     0,     0,   457,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   564,     0,     0,   461,   462,
    1098,   316,   317,   318,     0,   320,   321,   322,   323,   324,
     463,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,     0,   338,   339,   340,     0,     0,   343,   344,
     345,   346,   464,   465,   466,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   468,   469,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     473,   474,   475,   476,   477,     0,   478,     0,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,    59,
     565,   491,   492,     0,     0,     0,     0,     0,     0,   493,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   496,   497,   498,     0,
      14,     0,     0,   499,   500,     0,     0,   431,   432,     0,
       0,   501,     0,   502,     0,   503,   504,   437,   438,   439,
     440,   441,     0,     0,     0,     0,     0,   442,     0,   444,
       0,     0,     0,   447,     0,     0,     0,     0,     0,     0,
       0,   449,     0,     0,     0,     0,     0,   452,     0,     0,
     453,     0,     0,   454,     0,     0,     0,   457,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   564,     0,     0,
     461,   462,     0,   316,   317,   318,     0,   320,   321,   322,
     323,   324,   463,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,     0,   338,   339,   340,     0,     0,
     343,   344,   345,   346,   464,   465,   466,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   468,
     469,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   473,   474,   475,   476,   477,     0,   478,     0,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,    59,   565,   491,   492,     0,     0,     0,     0,     0,
       0,   493,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1101,     0,   496,   497,
     498,     0,    14,     0,     0,   499,   500,     0,     0,   431,
     432,     0,     0,   501,     0,   502,     0,   503,   504,   437,
     438,   439,   440,   441,     0,     0,     0,     0,     0,   442,
       0,   444,     0,     0,     0,   447,     0,     0,     0,     0,
       0,     0,     0,   449,     0,     0,     0,     0,     0,   452,
       0,     0,   453,     0,     0,   454,     0,     0,     0,   457,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   564,
       0,     0,   461,   462,     0,   316,   317,   318,     0,   320,
     321,   322,   323,   324,   463,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,     0,   338,   339,   340,
       0,     0,   343,   344,   345,   346,   464,   465,   466,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   468,   469,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   473,   474,   475,   476,   477,     0,
     478,     0,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,    59,   565,   491,   492,     0,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   498,     0,    14,     0,     0,   499,   500,     0,
       0,   431,   432,     0,     0,   501,     0,   502,  1159,   503,
     504,   437,   438,   439,   440,   441,     0,     0,     0,     0,
       0,   442,     0,   444,     0,     0,     0,   447,     0,     0,
       0,     0,     0,     0,     0,   449,     0,     0,     0,     0,
       0,   452,     0,     0,   453,     0,     0,   454,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   564,     0,     0,   461,   462,     0,   316,   317,   318,
       0,   320,   321,   322,   323,   324,   463,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,     0,   338,
     339,   340,     0,     0,   343,   344,   345,   346,   464,   465,
     466,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   473,   474,   475,   476,
     477,     0,   478,     0,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,    59,   565,   491,   492,     0,
       0,     0,     0,     0,     0,   493,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   496,   497,   498,     0,    14,     0,     0,   499,
     500,     0,     0,   431,   432,     0,     0,   501,     0,   502,
    1174,   503,   504,   437,   438,   439,   440,   441,     0,     0,
       0,     0,     0,   442,     0,   444,     0,     0,     0,   447,
       0,     0,     0,     0,     0,     0,     0,   449,     0,     0,
       0,     0,     0,   452,     0,     0,   453,     0,     0,   454,
       0,     0,     0,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   564,     0,     0,   461,   462,     0,   316,
     317,   318,     0,   320,   321,   322,   323,   324,   463,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
       0,   338,   339,   340,     0,     0,   343,   344,   345,   346,
     464,   465,   466,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,   469,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   473,   474,
     475,   476,   477,     0,   478,     0,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,    59,   565,   491,
     492,     0,     0,     0,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   496,   497,   498,     0,    14,     0,
       0,   499,   500,     0,     0,   431,   432,     0,     0,   501,
       0,   502,  1365,   503,   504,   437,   438,   439,   440,   441,
       0,     0,     0,     0,     0,   442,     0,   444,     0,     0,
       0,   447,     0,     0,     0,     0,     0,     0,     0,   449,
       0,     0,     0,     0,     0,   452,     0,     0,   453,     0,
       0,   454,     0,     0,     0,   457,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   564,     0,     0,   461,   462,
       0,   316,   317,   318,     0,   320,   321,   322,   323,   324,
     463,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,     0,   338,   339,   340,     0,     0,   343,   344,
     345,   346,   464,   465,   466,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   468,   469,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     473,   474,   475,   476,   477,     0,   478,     0,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,    59,
     565,   491,   492,     0,     0,     0,     0,     0,     0,   493,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   496,   497,   498,     0,
      14,     0,     0,   499,   500,     0,     0,   431,   432,     0,
       0,   501,     0,   502,  1374,   503,   504,   437,   438,   439,
     440,   441,     0,     0,     0,     0,     0,   442,     0,   444,
       0,     0,     0,   447,     0,     0,     0,     0,     0,     0,
       0,   449,     0,     0,     0,     0,     0,   452,     0,     0,
     453,     0,     0,   454,     0,     0,     0,   457,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   564,     0,     0,
     461,   462,     0,   316,   317,   318,     0,   320,   321,   322,
     323,   324,   463,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,     0,   338,   339,   340,     0,     0,
     343,   344,   345,   346,   464,   465,   466,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   468,
     469,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   473,   474,   475,   476,   477,     0,   478,     0,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,    59,   565,   491,   492,     0,     0,     0,     0,     0,
       0,   493,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   496,   497,
     498,     0,    14,     0,     0,   499,   500,     0,     0,   431,
     432,     0,     0,   501,     0,   502,  1415,   503,   504,   437,
     438,   439,   440,   441,     0,     0,     0,     0,     0,   442,
       0,   444,     0,     0,     0,   447,     0,     0,     0,     0,
       0,     0,     0,   449,     0,     0,     0,     0,     0,   452,
       0,     0,   453,     0,     0,   454,     0,     0,     0,   457,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   564,
       0,     0,   461,   462,     0,   316,   317,   318,     0,   320,
     321,   322,   323,   324,   463,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,     0,   338,   339,   340,
       0,     0,   343,   344,   345,   346,   464,   465,   466,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   468,   469,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   473,   474,   475,   476,   477,     0,
     478,     0,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,    59,   565,   491,   492,     0,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   498,     0,    14,     0,     0,   499,   500,     0,
       0,   431,   432,     0,     0,   501,     0,   502,  1486,   503,
     504,   437,   438,   439,   440,   441,     0,     0,     0,     0,
       0,   442,     0,   444,     0,     0,     0,   447,     0,     0,
       0,     0,     0,     0,     0,   449,     0,     0,     0,     0,
       0,   452,     0,     0,   453,     0,     0,   454,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   564,     0,     0,   461,   462,     0,   316,   317,   318,
       0,   320,   321,   322,   323,   324,   463,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,     0,   338,
     339,   340,     0,     0,   343,   344,   345,   346,   464,   465,
     466,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
       0,     0,  1523,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   473,   474,   475,   476,
     477,     0,   478,     0,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,    59,   565,   491,   492,     0,
       0,     0,     0,     0,     0,   493,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   496,   497,   498,     0,    14,     0,     0,   499,
     500,     0,     0,   431,   432,     0,     0,   501,     0,   502,
       0,   503,   504,   437,   438,   439,   440,   441,     0,     0,
       0,     0,     0,   442,     0,   444,     0,     0,     0,   447,
       0,     0,     0,     0,     0,     0,     0,   449,     0,     0,
       0,     0,     0,   452,     0,     0,   453,     0,     0,   454,
       0,     0,     0,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   564,     0,     0,   461,   462,     0,   316,
     317,   318,     0,   320,   321,   322,   323,   324,   463,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
       0,   338,   339,   340,     0,     0,   343,   344,   345,   346,
     464,   465,   466,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,   469,     0,     0,     0,
       0,     0,     0,     0,  1562,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   473,   474,
     475,   476,   477,     0,   478,     0,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,    59,   565,   491,
     492,     0,     0,     0,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   496,   497,   498,     0,    14,     0,
       0,   499,   500,     0,     0,   431,   432,     0,     0,   501,
       0,   502,     0,   503,   504,   437,   438,   439,   440,   441,
       0,     0,     0,     0,     0,   442,     0,   444,     0,     0,
       0,   447,     0,     0,     0,     0,     0,     0,     0,   449,
       0,     0,     0,     0,     0,   452,     0,     0,   453,     0,
       0,   454,     0,     0,     0,   457,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   564,     0,     0,   461,   462,
       0,   316,   317,   318,     0,   320,   321,   322,   323,   324,
     463,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,     0,   338,   339,   340,     0,     0,   343,   344,
     345,   346,   464,   465,   466,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   468,   469,     0,
       0,     0,     0,     0,     0,     0,  1563,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     473,   474,   475,   476,   477,     0,   478,     0,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,    59,
     565,   491,   492,     0,     0,     0,     0,     0,     0,   493,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   496,   497,   498,     0,
      14,     0,     0,   499,   500,     0,     0,   431,   432,     0,
       0,   501,     0,   502,     0,   503,   504,   437,   438,   439,
     440,   441,     0,     0,     0,     0,     0,   442,     0,   444,
       0,     0,     0,   447,     0,     0,     0,     0,     0,     0,
       0,   449,     0,     0,     0,     0,     0,   452,     0,     0,
     453,     0,     0,   454,     0,     0,     0,   457,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   564,     0,     0,
     461,   462,     0,   316,   317,   318,     0,   320,   321,   322,
     323,   324,   463,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,     0,   338,   339,   340,     0,     0,
     343,   344,   345,   346,   464,   465,   466,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   468,
     469,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   473,   474,   475,   476,   477,     0,   478,     0,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,    59,   565,   491,   492,     0,     0,     0,     0,     0,
       0,   493,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   496,   497,
     498,     0,    14,     0,     0,   499,   500,     0,     0,   431,
     432,     0,     0,   501,     0,   502,     0,   503,   504,   437,
     438,   439,   440,   441,     0,     0,     0,     0,     0,   442,
       0,   444,     0,     0,     0,   447,     0,     0,     0,     0,
       0,     0,     0,   449,     0,     0,     0,     0,     0,   452,
       0,     0,   453,     0,     0,   454,     0,     0,     0,   457,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   564,
       0,     0,   461,   462,     0,   316,   317,   318,     0,   320,
     321,   322,   323,   324,   463,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,     0,   338,   339,   340,
       0,     0,   343,   344,   345,   346,   464,   465,   466,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   468,   469,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   473,   474,   475,   476,   477,     0,
     478,     0,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,    59,   565,   491,   492,     0,   685,     0,
       0,     0,     0,   493,   310,     0,     0,     0,     0,     0,
     311,     0,     0,     0,     0,     0,   312,     0,     0,     0,
     496,   497,   498,     0,    14,     0,   313,   499,   500,     0,
       0,     0,     0,     0,   314,  1350,     0,   502,     0,   503,
     504,     0,     0,     0,     0,     0,     0,     0,     0,   315,
       0,     0,     0,     0,     0,     0,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,     0,
       0,     0,     0,     0,     0,     0,     0,   687,     0,     0,
       0,     0,     0,   310,     0,     0,     0,     0,     0,   311,
       0,     0,     0,     0,     0,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   313,     0,    58,     0,     0,
       0,     0,     0,   314,     0,     0,     0,     0,     0,     0,
     349,     0,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,     0,     0,    59,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     350,     0,   310,     0,     0,     0,     0,     0,   311,     0,
       0,     0,     0,     0,   312,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   313,     0,    58,     0,     0,     0,
       0,     0,   314,     0,     0,     0,     0,     0,     0,   349,
       0,     0,     0,     0,     0,     0,     0,   315,     0,     0,
       0,     0,     0,    59,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   350,
       0,   310,     0,     0,     0,     0,     0,   311,     0,     0,
       0,     0,     0,   312,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,     0,    58,     0,     0,     0,     0,
       0,   314,     0,     0,     0,     0,     0,     0,   349,     0,
       0,     0,     0,     0,     0,     0,   315,     0,     0,     0,
       0,     0,    59,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   350,     0,
     603,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   349,     0,     0,
       0,     0,     0,     0,     0,     0,   310,     0,     0,     0,
       0,   608,   311,     0,     0,     0,     0,     0,   312,     0,
       0,     0,     0,   609,     0,     0,     0,     0,   313,     0,
       0,     0,     0,     0,     0,     0,   314,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,     0,   350,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   310,     0,     0,     0,     0,
       0,   311,     0,     0,     0,     0,     0,   312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,     0,    58,
       0,     0,     0,     0,     0,   314,     0,     0,     0,     0,
       0,     0,   349,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,    59,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     730,   731,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   350,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   349,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   608,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   859,   860,   861,   862,
     863,   864,   865,   866,   740,   741,   742,   743,   744,   867,
     868,   747,   748,   749,   750,   869,   752,   753,   754,   755,
    -351,   350,   730,   731,   756,   757,   758,   759,   870,   871,
       0,     0,   762,   763,   764,   872,   873,   874,   768,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   875,   770,     0,
     771,   772,   773,   774,   775,   776,   777,   778,   779,   780,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   781,
     782,     0,     0,     0,   503,   673,     0,     0,   859,   860,
     861,   862,   863,   864,   865,   866,   740,   741,   742,   743,
     744,   867,   868,   747,   748,   749,   750,   869,   752,   753,
     754,   755,   730,   731,     0,     0,   756,   757,   758,   759,
     870,   871,     0,     0,   762,   763,   764,   872,   873,   874,
     768,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   730,   731,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1063,     0,     0,     0,     0,     0,     0,   875,
     770,     0,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   781,   782,     0,     0,     0,   503,   673,   859,   860,
     861,   862,   863,   864,   865,   866,   740,   741,   742,   743,
     744,   867,   868,   747,   748,   749,   750,   869,   752,   753,
     754,   755,     0,     0,     0,     0,   756,   757,   758,   759,
     870,   871,     0,     0,   762,   763,   764,   872,   873,   874,
     768,   740,   741,   742,   743,   744,     0,     0,   747,   748,
     749,   750,     0,   752,   753,   754,   755,   730,   731,     0,
       0,   756,     0,   758,   759,     0,     0,     0,     0,   762,
     763,   764,  1254,     0,     0,   768,     0,     0,     0,   875,
     770,     0,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   730,   731,     0,     0,     0,     0,     0,     0,
       0,   781,   782,     0,     0,     0,   503,   673,     0,     0,
       0,     0,     0,     0,     0,   770,     0,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   781,   782,     0,     0,
     793,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   740,   741,   742,   743,   744,     0,     0,   747,   748,
     749,   750,     0,   752,   753,   754,   755,     0,     0,     0,
       0,   756,     0,   758,   759,     0,     0,     0,     0,   762,
     763,   764,     0,     0,     0,   768,   740,   741,   742,   743,
     744,     0,     0,   747,   748,   749,   750,     0,   752,   753,
     754,   755,   730,   731,     0,     0,   756,     0,   758,   759,
       0,     0,     0,     0,   762,   763,   764,     0,     0,     0,
     768,     0,     0,     0,     0,   770,     0,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   730,   731,     0,
       0,     0,     0,     0,     0,     0,   781,   782,     0,     0,
     803,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     770,     0,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   781,   782,     0,     0,   819,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   740,   741,   742,   743,
     744,     0,     0,   747,   748,   749,   750,     0,   752,   753,
     754,   755,     0,     0,     0,     0,   756,     0,   758,   759,
       0,     0,     0,     0,   762,   763,   764,     0,     0,     0,
     768,   740,   741,   742,   743,   744,     0,     0,   747,   748,
     749,   750,     0,   752,   753,   754,   755,   730,   731,     0,
       0,   756,     0,   758,   759,     0,     0,     0,     0,   762,
     763,   764,     0,     0,     0,   768,     0,     0,     0,     0,
     770,     0,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   730,   731,     0,     0,     0,     0,     0,     0,
       0,   781,   782,     0,     0,   834,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   770,     0,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   781,   782,     0,     0,
    1131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   740,   741,   742,   743,   744,     0,     0,   747,   748,
     749,   750,     0,   752,   753,   754,   755,     0,     0,     0,
       0,   756,     0,   758,   759,     0,     0,     0,     0,   762,
     763,   764,     0,     0,     0,   768,   740,   741,   742,   743,
     744,     0,     0,   747,   748,   749,   750,     0,   752,   753,
     754,   755,   730,   731,     0,     0,   756,     0,   758,   759,
       0,     0,     0,     0,   762,   763,   764,     0,     0,     0,
     768,     0,     0,     0,     0,   770,     0,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   730,   731,     0,
       0,     0,     0,     0,     0,     0,   781,   782,     0,     0,
    1135,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     770,     0,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   781,   782,     0,     0,  1137,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   740,   741,   742,   743,
     744,     0,     0,   747,   748,   749,   750,     0,   752,   753,
     754,   755,     0,     0,     0,     0,   756,     0,   758,   759,
       0,     0,     0,     0,   762,   763,   764,     0,     0,     0,
     768,   740,   741,   742,   743,   744,     0,     0,   747,   748,
     749,   750,     0,   752,   753,   754,   755,   730,   731,     0,
       0,   756,     0,   758,   759,     0,     0,     0,     0,   762,
     763,   764,     0,     0,     0,   768,     0,     0,     0,     0,
     770,     0,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   730,   731,     0,     0,     0,     0,     0,     0,
       0,   781,   782,     0,     0,  1146,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   770,     0,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   781,   782,     0,     0,
    1147,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   740,   741,   742,   743,   744,     0,     0,   747,   748,
     749,   750,     0,   752,   753,   754,   755,     0,     0,     0,
       0,   756,     0,   758,   759,     0,     0,     0,     0,   762,
     763,   764,     0,     0,     0,   768,   740,   741,   742,   743,
     744,     0,     0,   747,   748,   749,   750,     0,   752,   753,
     754,   755,   730,   731,     0,     0,   756,     0,   758,   759,
       0,     0,     0,     0,   762,   763,   764,     0,     0,     0,
     768,     0,     0,     0,     0,   770,     0,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   730,   731,     0,
       0,     0,     0,     0,     0,     0,   781,   782,     0,     0,
    1148,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     770,     0,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   781,   782,     0,     0,  1149,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   740,   741,   742,   743,
     744,     0,     0,   747,   748,   749,   750,     0,   752,   753,
     754,   755,     0,     0,     0,     0,   756,     0,   758,   759,
       0,     0,     0,     0,   762,   763,   764,     0,     0,     0,
     768,   740,   741,   742,   743,   744,     0,     0,   747,   748,
     749,   750,     0,   752,   753,   754,   755,   730,   731,     0,
       0,   756,     0,   758,   759,     0,     0,     0,     0,   762,
     763,   764,     0,     0,     0,   768,     0,     0,     0,     0,
     770,     0,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   730,   731,     0,     0,     0,     0,     0,     0,
       0,   781,   782,     0,     0,  1150,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   770,     0,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   781,   782,     0,     0,
    1151,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   740,   741,   742,   743,   744,     0,     0,   747,   748,
     749,   750,     0,   752,   753,   754,   755,     0,     0,     0,
       0,   756,     0,   758,   759,     0,     0,     0,     0,   762,
     763,   764,     0,     0,     0,   768,   740,   741,   742,   743,
     744,     0,     0,   747,   748,   749,   750,     0,   752,   753,
     754,   755,   730,   731,     0,     0,   756,     0,   758,   759,
       0,     0,     0,     0,   762,   763,   764,     0,     0,     0,
     768,     0,     0,     0,     0,   770,     0,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   730,   731,     0,
       0,     0,     0,     0,     0,     0,   781,   782,     0,     0,
    1275,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     770,     0,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   781,   782,     0,     0,  1287,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   740,   741,   742,   743,
     744,     0,     0,   747,   748,   749,   750,     0,   752,   753,
     754,   755,     0,     0,     0,     0,   756,     0,   758,   759,
       0,     0,     0,     0,   762,   763,   764,     0,     0,     0,
     768,   740,   741,   742,   743,   744,     0,     0,   747,   748,
     749,   750,     0,   752,   753,   754,   755,   730,   731,     0,
       0,   756,     0,   758,   759,     0,     0,     0,     0,   762,
     763,   764,     0,     0,     0,   768,     0,     0,     0,     0,
     770,     0,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   730,   731,     0,     0,     0,     0,     0,     0,
       0,   781,   782,     0,     0,  1418,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   770,     0,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   781,   782,     0,     0,
    1433,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   740,   741,   742,   743,   744,     0,     0,   747,   748,
     749,   750,     0,   752,   753,   754,   755,     0,     0,     0,
       0,   756,     0,   758,   759,     0,     0,     0,     0,   762,
     763,   764,     0,     0,     0,   768,   740,   741,   742,   743,
     744,     0,     0,   747,   748,   749,   750,     0,   752,   753,
     754,   755,   730,   731,     0,     0,   756,     0,   758,   759,
       0,     0,     0,     0,   762,   763,   764,     0,     0,     0,
     768,     0,     0,     0,     0,   770,     0,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   730,   731,     0,
       0,     0,     0,     0,     0,     0,   781,   782,     0,     0,
    1434,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     770,     0,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   781,   782,     0,     0,  1453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   740,   741,   742,   743,
     744,     0,     0,   747,   748,   749,   750,     0,   752,   753,
     754,   755,     0,     0,     0,     0,   756,     0,   758,   759,
       0,     0,     0,     0,   762,   763,   764,     0,     0,     0,
     768,   740,   741,   742,   743,   744,     0,     0,   747,   748,
     749,   750,     0,   752,   753,   754,   755,   730,   731,     0,
       0,   756,     0,   758,   759,     0,     0,     0,     0,   762,
     763,   764,     0,     0,     0,   768,     0,     0,     0,     0,
     770,     0,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   730,   731,     0,     0,     0,     0,     0,     0,
       0,   781,   782,     0,     0,  1455,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   770,     0,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   781,   782,     0,     0,
    1457,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   740,   741,   742,   743,   744,     0,     0,   747,   748,
     749,   750,     0,   752,   753,   754,   755,     0,     0,     0,
       0,   756,     0,   758,   759,     0,     0,     0,     0,   762,
     763,   764,     0,     0,     0,   768,   740,   741,   742,   743,
     744,     0,     0,   747,   748,   749,   750,     0,   752,   753,
     754,   755,   730,   731,     0,     0,   756,     0,   758,   759,
       0,     0,     0,     0,   762,   763,   764,     0,     0,     0,
     768,     0,     0,     0,     0,   770,     0,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   730,   731,     0,
       0,     0,     0,     0,     0,     0,   781,   782,     0,     0,
    1461,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     770,     0,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   781,   782,     0,     0,  1528,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   740,   741,   742,   743,
     744,     0,     0,   747,   748,   749,   750,     0,   752,   753,
     754,   755,     0,     0,     0,     0,   756,     0,   758,   759,
       0,     0,     0,     0,   762,   763,   764,     0,     0,     0,
     768,   740,   741,   742,   743,   744,     0,     0,   747,   748,
     749,   750,     0,   752,   753,   754,   755,   730,   731,     0,
       0,   756,     0,   758,   759,     0,     0,     0,     0,   762,
     763,   764,     0,     0,     0,   768,     0,     0,     0,     0,
     770,     0,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   730,   731,     0,     0,     0,     0,     0,     0,
       0,   781,   782,     0,     0,  1529,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   770,     0,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   781,   782,     0,     0,
    1530,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   740,   741,   742,   743,   744,     0,     0,   747,   748,
     749,   750,     0,   752,   753,   754,   755,     0,     0,     0,
       0,   756,     0,   758,   759,     0,     0,     0,     0,   762,
     763,   764,     0,     0,     0,   768,   740,   741,   742,   743,
     744,     0,     0,   747,   748,   749,   750,     0,   752,   753,
     754,   755,   730,   731,     0,     0,   756,     0,   758,   759,
       0,     0,     0,     0,   762,   763,   764,     0,     0,     0,
     768,     0,     0,     0,     0,   770,     0,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   730,   731,     0,
       0,     0,     0,     0,     0,     0,   781,   782,     0,     0,
    1538,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     770,     0,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   781,   782,     0,     0,  1548,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   740,   741,   742,   743,
     744,     0,     0,   747,   748,   749,   750,     0,   752,   753,
     754,   755,     0,     0,     0,     0,   756,     0,   758,   759,
       0,     0,     0,     0,   762,   763,   764,     0,     0,     0,
     768,   740,   741,   742,   743,   744,     0,     0,   747,   748,
     749,   750,     0,   752,   753,   754,   755,   730,   731,     0,
       0,   756,     0,   758,   759,     0,     0,     0,     0,   762,
     763,   764,     0,     0,     0,   768,     0,     0,     0,     0,
     770,     0,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   730,   731,     0,     0,     0,     0,     0,     0,
       0,   781,   782,     0,     0,  1553,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   770,     0,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   781,   782,     0,     0,
    1609,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   740,   741,   742,   743,   744,     0,     0,   747,   748,
     749,   750,     0,   752,   753,   754,   755,     0,     0,     0,
       0,   756,     0,   758,   759,     0,     0,     0,     0,   762,
     763,   764,     0,     0,     0,   768,   740,   741,   742,   743,
     744,   730,   731,   747,   748,   749,   750,     0,   752,   753,
     754,   755,     0,     0,     0,     0,   756,     0,   758,   759,
       0,     0,     0,     0,   762,   763,   764,     0,     0,     0,
     768,     0,     0,     0,     0,   770,     0,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   781,   782,   824,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     770,     0,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   781,   782,  1085,     0,   740,   741,   742,   743,   744,
     730,   731,   747,   748,   749,   750,     0,   752,   753,   754,
     755,     0,     0,     0,     0,   756,     0,   758,   759,     0,
       0,     0,     0,   762,   763,   764,     0,     0,     0,   768,
       0,     0,     0,     0,     0,   730,   731,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   770,
       0,   771,   772,   773,   774,   775,   776,   777,   778,   779,
     780,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     781,   782,  1228,     0,   740,   741,   742,   743,   744,     0,
       0,   747,   748,   749,   750,     0,   752,   753,   754,   755,
       0,     0,     0,     0,   756,     0,   758,   759,     0,     0,
       0,     0,   762,   763,   764,     0,     0,     0,   768,   740,
     741,   742,   743,   744,   730,   731,   747,   748,   749,   750,
       0,   752,   753,   754,   755,     0,     0,     0,     0,   756,
       0,   758,   759,     0,     0,     0,     0,   762,   763,   764,
       0,     0,     0,   768,     0,     0,     0,     0,   770,     0,
     771,   772,   773,   774,   775,   776,   777,   778,   779,   780,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   781,
     782,  1243,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   770,     0,   771,   772,   773,   774,   775,
     776,   777,   778,   779,   780,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   781,   782,  1372,     0,   740,   741,
     742,   743,   744,     0,     0,   747,   748,   749,   750,     0,
     752,   753,   754,   755,   231,   232,     0,     0,   756,     0,
     758,   759,     0,     0,     0,     0,   762,   763,   764,     0,
       0,   233,   768,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   730,
     731,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   770,     0,   771,   772,   773,   774,   775,   776,
     777,   778,   779,   780,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   781,   782,  1377,     0,     0,     0,     0,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,     0,     0,
     252,   253,   254,     0,     0,     0,     0,     0,     0,   255,
     256,   257,   258,   259,     0,     0,   260,   261,   262,   263,
     264,   265,   266,   740,   741,   742,   743,   744,   730,   731,
     747,   748,   749,   750,     0,   752,   753,   754,   755,     0,
       0,     0,     0,   756,     0,   758,   759,     0,     0,     0,
       0,   762,   763,   764,     0,     0,     0,   768,     0,     0,
       0,     0,   267,     0,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,     0,     0,   278,   279,     0,     0,
       0,     0,     0,   280,   281,     0,     0,   286,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   770,     0,   771,
     772,   773,   774,   775,   776,   777,   778,   779,   780,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   781,   782,
       0,     0,   740,   741,   742,   743,   744,   730,   731,   747,
     748,   749,   750,     0,   752,   753,   754,   755,     0,     0,
       0,     0,   756,     0,   758,   759,     0,     0,   925,     0,
     762,   763,   764,     0,     0,     0,   768,     0,     0,     0,
       0,     0,   730,   731,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   770,     0,   771,   772,
     773,   774,   775,   776,   777,   778,   779,   780,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   781,   782,     0,
       0,   740,   741,   742,   743,   744,     0,     0,   747,   748,
     749,   750,     0,   752,   753,   754,   755,     0,     0,     0,
       0,   756,     0,   758,   759,     0,     0,  1160,     0,   762,
     763,   764,   730,   731,     0,   768,   740,   741,   742,   743,
     744,     0,     0,   747,   748,   749,   750,     0,   752,   753,
     754,   755,     0,     0,     0,     0,   756,     0,   758,   759,
       0,     0,     0,     0,   762,   763,   764,   730,   731,     0,
     768,     0,     0,     0,     0,   770,     0,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   781,   782,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     770,  1233,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,     0,     0,     0,     0,   740,   741,   742,   743,
     744,   781,   782,   747,   748,   749,   750,     0,   752,   753,
     754,   755,     0,     0,     0,     0,   756,     0,   758,   759,
       0,     0,     0,     0,   762,   763,   764,   730,   731,     0,
     768,   740,   741,   742,   743,   744,     0,     0,   747,   748,
     749,   750,     0,   752,   753,   754,   755,     0,     0,     0,
       0,   756,     0,   758,   759,     0,     0,     0,     0,   762,
     763,   764,  1245,     0,     0,   768,   730,   731,     0,     0,
     770,     0,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   781,   782,  1335,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   770,     0,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,     0,     0,     0,
       0,   740,   741,   742,   743,   744,   781,   782,   747,   748,
     749,   750,     0,   752,   753,   754,   755,     0,     0,     0,
       0,   756,     0,   758,   759,     0,     0,     0,     0,   762,
     763,   764,     0,     0,     0,   768,   730,   731,     0,     0,
     740,   741,   742,   743,   744,     0,     0,   747,   748,   749,
     750,     0,   752,   753,   754,   755,     0,     0,     0,     0,
     756,     0,   758,   759,     0,     0,     0,  1549,   762,   763,
     764,   730,   731,     0,   768,   770,     0,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   781,   782,     0,     0,
       0,     0,     0,     0,     0,     0,  1603,     0,     0,     0,
       0,     0,     0,     0,   770,     0,   771,   772,   773,   774,
     775,   776,   777,   778,   779,   780,     0,     0,     0,     0,
     740,   741,   742,   743,   744,   781,   782,   747,   748,   749,
     750,     0,   752,   753,   754,   755,     0,     0,     0,     0,
     756,     0,   758,   759,     0,     0,     0,     0,   762,   763,
     764,   730,   731,     0,   768,   740,   741,   742,   743,   744,
       0,     0,   747,   748,   749,   750,     0,   752,   753,   754,
     755,     0,     0,     0,     0,   756,     0,   758,   759,     0,
       0,     0,     0,   762,   763,   764,  1625,     0,     0,   768,
     730,   731,     0,     0,   770,     0,   771,   772,   773,   774,
     775,   776,   777,   778,   779,   780,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   781,   782,     0,     0,     0,
       0,  1626,     0,     0,     0,     0,     0,     0,     0,   770,
       0,   771,   772,   773,   774,   775,   776,   777,   778,   779,
     780,     0,     0,     0,     0,   740,   741,   742,   743,   744,
     781,   782,   747,   748,   749,   750,     0,   752,   753,   754,
     755,     0,     0,     0,     0,   756,     0,   758,   759,     0,
       0,     0,     0,   762,   763,   764,     0,     0,     0,   768,
     730,   731,     0,     0,   740,   741,   742,   743,   744,     0,
       0,   747,   748,   749,   750,     0,   752,   753,   754,   755,
       0,     0,     0,     0,   756,     0,   758,   759,     0,     0,
       0,  1630,   762,   763,   764,   730,   731,     0,   768,   770,
       0,   771,   772,   773,   774,   775,   776,   777,   778,   779,
     780,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     781,   782,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   770,     0,
     771,   772,   773,   774,   775,   776,   777,   778,   779,   780,
       0,     0,     0,     0,   740,   741,   742,   743,   744,   781,
     782,   747,   748,   749,   750,     0,   752,   753,   754,   755,
       0,     0,     0,     0,   756,     0,   758,   759,     0,     0,
       0,     0,   762,   763,   764,   730,   731,     0,  -741,   740,
     741,   742,   743,   744,     0,     0,   747,   748,   749,   750,
       0,   752,   753,   754,   755,     0,     0,     0,     0,   756,
       0,   758,   759,     0,     0,     0,     0,   762,   763,   764,
     730,   731,     0,     0,     0,     0,     0,     0,   770,     0,
     771,   772,   773,   774,   775,   776,   777,   778,   779,   780,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   781,
     782,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   770,     0,   771,   772,   773,   774,   775,
     776,   777,   778,   779,   780,     0,     0,     0,     0,   740,
     741,   742,   743,   744,   781,   782,   747,   748,   749,   750,
       0,   752,   753,   754,   755,     0,     0,     0,     0,   756,
       0,   758,   759,     0,     0,     0,     0,   762,     0,   764,
     730,   731,     0,     0,   740,   741,   742,   743,   744,     0,
       0,   747,   748,   749,   750,     0,   752,   753,   754,   755,
       0,     0,     0,     0,   756,     0,   758,   759,   730,   731,
       0,     0,   762,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   771,   772,   773,   774,   775,
     776,   777,   778,   779,   780,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   781,   782,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     771,   772,   773,   774,   775,   776,   777,   778,   779,   780,
       0,     0,     0,     0,   740,   741,   742,   743,   744,   781,
     782,   747,   748,   749,   750,     0,   752,   753,   754,   755,
       0,     0,     0,     0,   756,     0,   758,   759,   730,   731,
       0,     0,   740,   741,   742,   743,   744,     0,     0,   747,
     748,   749,   750,     0,   752,   753,   754,   755,     0,     0,
       0,     0,   756,     0,   758,   759,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     771,   772,   773,   774,   775,   776,   777,   778,   779,   780,
       0,     0,     0,     0,   965,     0,     0,     0,     0,   781,
     782,     0,     0,     0,     0,     0,     0,     0,     0,   772,
     773,   774,   775,   776,   777,   778,   779,   780,     0,     0,
       0,     0,   740,   741,   742,   743,   744,   781,   782,   747,
     748,   749,   750,     0,   752,   753,   754,   755,     0,     0,
       0,     0,   756,     0,   758,   759,   316,   317,   318,     0,
     320,   321,   322,   323,   324,   463,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,     0,   338,   339,
     340,     0,     0,   343,   344,   345,   346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   969,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     773,   774,   775,   776,   777,   778,   779,   780,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   781,   782,     0,
    1229,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   966,     0,     0,     0,     0,     0,     0,     0,
       0,   316,   317,   318,   967,   320,   321,   322,   323,   324,
     463,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,     0,   338,   339,   340,     0,     0,   343,   344,
     345,   346,   316,   317,   318,     0,   320,   321,   322,   323,
     324,   463,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,     0,   338,   339,   340,     0,     0,   343,
     344,   345,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   181,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   970,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   971,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   182,
       0,   183,     0,   184,   185,   186,   187,   188,  1230,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
    1231,   200,   201,   202,     0,     0,   203,   204,   205,   206,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   207,   208,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   209
};

static const yytype_int16 yycheck[] =
{
      14,    15,   211,   482,   570,   227,   621,   708,   375,   710,
     375,   719,   590,   161,   589,   388,   645,   900,   626,   537,
     584,   443,   586,   227,   588,   810,   916,   812,  1163,   814,
       4,   443,   922,   382,    32,   471,   472,    19,     7,    80,
      19,   688,    52,   561,   417,    32,  1389,    19,    21,    63,
      64,    65,  1410,    19,    19,   467,    18,    19,   103,    52,
      45,    19,    56,    19,   130,  1353,     0,   168,    62,   125,
      14,    15,     6,   142,   143,   144,   125,   685,   443,   687,
     502,   689,   156,    56,   190,   158,   125,   178,   194,   697,
     104,   105,   106,   107,   168,    29,   135,    31,   706,    33,
      33,  1531,   467,    14,    15,    39,   124,   480,   481,   127,
     128,  1469,  1470,   214,   180,    49,   141,  1497,    49,  1499,
     105,    55,   125,  1503,  1504,   153,  1484,   101,   102,    62,
     158,   187,   135,  1506,   190,     4,     5,   186,   187,   213,
     189,   150,   215,   192,   213,    79,  1576,   180,   187,   158,
     180,   373,   168,   301,   855,    24,  1444,   675,  1501,   168,
      91,    30,   574,   156,   189,   178,   388,   101,   102,   373,
     180,   593,   180,   218,    61,   168,   189,   878,   219,  1559,
    1315,  1539,  1540,   213,   187,   179,   194,   215,  1561,   192,
     891,   209,   210,   894,   192,   417,   212,   419,    67,    68,
     150,   134,   161,   212,   186,   192,   168,   186,   141,   574,
     179,    20,    21,   227,   186,   637,   638,   161,   168,   193,
     186,   186,   166,   210,   168,   198,   638,   171,   186,   216,
     186,   186,   101,   102,   125,   168,   209,   446,   212,   141,
     662,   210,    46,   665,   135,   953,   180,   127,   128,   178,
     161,   460,   127,   128,   180,   166,   668,   168,   480,   481,
     171,  1144,    66,   304,   125,   199,   168,     6,   212,  1159,
     203,   644,   141,   638,   135,   140,   210,   153,   125,   212,
     141,   156,   158,   212,  1419,   125,   178,   213,   135,   158,
     159,   160,   728,   168,   667,   135,   187,   719,   180,   168,
      20,    21,   187,   668,   125,   114,   115,   116,   117,   178,
      49,   189,   180,   178,   135,   124,   180,   126,   127,   128,
     129,  1204,   214,   215,   193,   134,   187,   136,   137,   209,
     210,   213,   180,   941,   209,   210,   350,   213,   150,   215,
     187,   210,    20,    21,   190,   213,   194,   187,   194,   213,
     183,   184,   180,   575,   576,   577,   168,   579,   580,   373,
     180,   583,   784,   585,   378,   587,   187,   589,   186,   180,
     168,   575,   576,   577,   388,   579,   580,   726,   211,   583,
     186,   585,   180,   587,   796,   213,   195,   196,   197,   198,
     199,   168,  1100,   213,   186,   213,   116,   117,   186,   963,
     209,   210,   213,   417,   124,   419,   126,   127,   128,   129,
     168,   169,   170,   180,   134,   180,   625,   186,    63,    64,
      65,   213,   644,   211,  1314,   439,   848,   194,   141,   194,
     842,   796,   180,  1218,   952,   183,  1326,   150,   116,   117,
     649,   199,   155,   190,   213,   667,   124,   194,   927,   127,
     128,   129,   186,   186,  1162,   168,   134,   186,   937,   104,
     105,   106,   107,   211,   141,   141,   480,   481,   186,   186,
     843,   844,   908,   180,   150,   186,  1091,   842,   211,   213,
     193,   854,   211,     4,     5,   694,     7,   194,   186,   209,
     210,   913,   168,   186,   186,   213,   213,   186,    56,   921,
     211,   186,  1080,  1078,    62,   714,    20,    21,   730,   731,
     883,   884,  1159,   886,    35,   213,   186,   890,   211,   892,
     893,   213,   895,  1308,   213,   193,   211,  1174,   180,    74,
     193,   209,   210,    78,   756,   178,   190,   973,   974,   975,
     194,   211,   194,   186,   212,   189,   180,    92,    93,   212,
     194,    32,    97,    98,    99,   100,   212,   193,  1343,   189,
     194,   575,   576,   577,   194,   579,   580,   189,   212,   583,
     180,   585,   194,   587,   193,   589,   212,   187,    59,    60,
      11,  1145,   212,   168,   189,   168,   161,   156,   156,   194,
     212,    22,    23,   212,   180,   180,   156,   180,   820,   168,
     168,   190,   116,   117,   190,   194,   828,   212,   168,   831,
     124,    32,   126,   127,   128,   129,   820,   631,   840,   209,
     134,   843,   844,   156,   828,   847,   189,   831,   850,   192,
     644,    32,   854,  1197,    56,   168,   840,   156,    59,    60,
      62,   122,  1350,   847,   792,   126,   850,   172,   186,   168,
      56,   189,   375,   667,   192,  1440,    62,   179,    59,    60,
     182,   883,   884,   185,   886,   180,  1295,   876,   890,    56,
     892,   893,    56,   895,  1096,    62,   179,   180,    62,   182,
      56,   195,   196,   197,   198,   199,    62,    56,   180,   168,
     169,   170,   180,    62,   186,   209,   210,   711,   186,   180,
     180,   122,   168,  1318,  1405,   126,   186,   188,   930,   180,
     932,   192,   180,   194,   195,   186,   180,   142,   186,   144,
     443,   122,   186,   190,   125,   126,   930,   194,   932,   210,
    1152,  1104,  1105,  1106,   135,   216,   190,   190,   190,   190,
     194,   194,   194,   194,   467,   190,   930,   189,   932,   194,
     192,  1173,   168,    20,    21,  1128,   180,  1179,   172,   173,
     174,   175,   172,   173,  1186,   214,   215,   188,  1190,   172,
     173,   192,   168,   194,   195,   168,   790,    86,    87,   180,
     142,   143,   144,   168,  1348,   168,   187,   188,   168,   210,
     178,   192,    56,   172,   195,   216,   142,   470,   471,   472,
     172,   173,   174,  1418,  1226,   180,   820,   105,   178,   210,
    1032,   179,   180,   181,   828,   216,  1238,   831,    10,    11,
      12,   187,  1189,  1196,  1189,   178,   840,   178,   178,   843,
     844,    34,    34,   847,   212,   168,   850,   179,  1485,   168,
     854,  1542,  1543,   168,  1545,  1067,   168,   520,  1477,   116,
     117,   574,   215,   211,    21,   168,  1078,   124,   189,   126,
     127,   128,   129,  1067,   211,   189,   178,   134,   178,   883,
     884,   212,   886,    42,   547,   193,   890,   193,   892,   893,
    1446,   895,  1104,  1105,  1106,   193,   212,  1309,  1535,  1536,
     193,   193,   212,   193,  1541,   212,    32,   193,   193,  1546,
     193,   193,   193,   190,   135,   168,  1128,   212,   168,   168,
     211,   168,   193,   210,   193,   638,   930,  1339,   932,   193,
     168,   193,   212,    59,    60,   193,   212,   212,   180,   161,
     197,   198,   199,  1580,  1581,   193,   212,    32,  1553,   193,
     193,   212,   209,   210,   193,   668,   212,   212,  1427,   193,
     212,  1430,    36,   180,  1520,  1521,   212,   180,     9,   180,
     215,   180,   180,   180,    59,    60,   180,  1389,    65,   642,
     210,   212,    32,   212,  1196,   179,    42,   193,   213,   168,
     168,   211,   180,   179,   189,   180,   122,   180,  1361,  1198,
     126,   130,    32,   179,   179,    42,   193,   670,    13,    59,
      60,  1567,   212,   212,   186,   189,   193,   187,    32,   732,
     733,   734,   735,   736,   737,   738,   739,   193,   161,    59,
      60,   215,   745,   746,   186,   178,    12,   122,   751,   186,
     158,   126,     7,   168,   168,    59,    60,   760,   761,   194,
      32,   213,   765,   766,   767,   168,   769,   168,   212,   186,
     211,  1273,   188,  1067,  1476,   728,   192,   186,   194,   195,
    1482,   179,   122,   179,  1078,   125,   126,    59,    60,  1273,
     212,  1280,  1445,   796,   210,   135,   168,     1,   193,  1501,
     216,   213,   122,   213,   193,   212,   126,  1509,   212,   179,
    1104,  1105,  1106,   188,    66,    32,   168,   192,   122,   194,
     195,   212,   126,   180,   212,   212,   212,   212,   194,   194,
      42,   194,   168,   213,  1128,   210,   212,   193,   212,   842,
     213,   216,    59,    60,   213,   798,   213,   187,   188,   211,
     122,   211,   192,   211,   126,   195,  1345,   168,   168,  1361,
    1562,  1363,   168,   168,   212,   212,  1519,   212,   188,   212,
     210,  1360,   192,   180,   194,   195,   216,   168,   213,  1363,
     212,   180,   212,   211,   188,   168,   211,   213,   192,   375,
     210,   195,   211,    32,   212,   212,   216,   168,   212,   385,
     212,    69,  1196,   212,   212,   122,   210,   194,   394,   126,
     212,   212,   216,   213,   194,   212,   188,   213,   404,   212,
     192,   180,   194,   195,   212,  1414,    52,   213,   414,   211,
     179,   179,   213,   179,   211,   180,   211,   423,   210,   213,
      32,   211,   179,  1445,   216,   898,   213,   433,   213,   211,
      79,   696,     1,    44,   133,   908,  1458,   443,    82,  1437,
     790,  1438,   448,   939,   450,   225,  1438,    59,    60,  1438,
    1438,   188,  1438,   459,  1458,   192,     1,   194,   195,  1273,
     959,   467,   468,   469,  1414,  1358,  1381,  1466,  1384,   520,
    1250,   427,  1467,   210,    52,   559,   482,  1100,  1425,   216,
     439,   439,    -1,  1169,   490,    -1,   688,   493,    -1,    -1,
     496,   497,   498,   499,   500,    -1,    -1,  1519,    -1,    -1,
     973,   974,   975,   509,    -1,   978,    -1,   980,  1517,   982,
     122,   984,    -1,   986,   126,   988,    -1,   990,    -1,   992,
      -1,    20,    21,    -1,   997,    -1,   999,    -1,    -1,    -1,
      -1,    -1,  1005,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1017,    -1,  1019,  1361,    -1,  1363,
      -1,  1024,    -1,  1026,    -1,  1028,    -1,    -1,  1031,   733,
     734,   735,   736,   737,   738,   739,    -1,    -1,   574,    -1,
      -1,   745,   746,    -1,    -1,    -1,   188,   751,    -1,    -1,
     192,    -1,   194,   195,    -1,    -1,   760,   761,    -1,  1062,
      -1,   765,   766,   767,    -1,   769,   602,  1606,   210,    -1,
      -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,
    1093,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,  1445,   638,    -1,    -1,   134,    -1,   136,   137,   645,
     646,    -1,   648,    -1,  1458,    -1,    -1,   653,    -1,    -1,
      -1,    -1,    -1,   659,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   668,    -1,    -1,    -1,  1189,    -1,    -1,    -1,
      -1,    -1,    -1,   679,   680,   681,   682,   683,   684,    -1,
     686,    -1,   688,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1506,    11,   193,   194,   195,   196,   197,   198,
     199,    -1,    20,    21,    -1,  1519,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   732,   733,   734,   735,
     736,   737,   738,   739,   740,   741,    -1,    -1,   744,   745,
     746,   747,   748,   749,    -1,   751,   752,  1561,   754,   755,
     756,   757,   758,   759,   760,   761,   762,   763,   764,   765,
     766,   767,   768,   769,   770,   771,   772,   773,   774,   775,
     776,   777,   778,   779,   780,    -1,   782,    -1,    -1,    -1,
     786,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     796,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,   835,
     148,    -1,    -1,    -1,    -1,    -1,   842,    -1,    -1,    -1,
      -1,    -1,    -1,   849,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   858,   859,   860,   861,   862,   863,   864,   865,
     866,   867,   868,   869,   870,   871,   872,   873,   874,   875,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   911,   912,    -1,    -1,    -1,
      -1,    -1,     4,     5,    -1,    -1,    -1,    -1,    -1,   925,
      -1,   927,    14,    15,    16,    17,    18,    -1,   934,    -1,
      -1,   937,    24,    -1,    26,    -1,   942,    -1,    30,    -1,
      32,    -1,    -1,    -1,    -1,   951,    38,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      52,    -1,    54,    -1,    -1,    -1,    -1,    59,    60,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,  1045,
      -1,    -1,    -1,  1049,    -1,    59,    60,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,
     192,    -1,    -1,   195,   196,   197,    -1,   199,   122,    -1,
     202,   203,   126,    -1,    -1,    -1,    -1,    -1,   210,    -1,
     212,    -1,   214,   215,   216,    -1,  1132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1154,  1155,
    1156,    20,    21,    -1,  1160,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1168,  1169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,   192,    -1,
     194,   195,    -1,  1189,    -1,    -1,    -1,    -1,  1194,  1195,
      -1,    -1,    -1,    -1,    -1,  1201,   210,    -1,    20,    21,
    1206,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1220,    -1,  1222,    -1,    -1,    -1,
      -1,  1227,    -1,    -1,    -1,    -1,    -1,  1233,    -1,    -1,
      -1,  1237,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
     139,  1277,    -1,   142,   143,   144,   145,   146,   147,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1295,
      -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,   121,
      -1,    -1,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,   187,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1369,    -1,  1371,    -1,    -1,    -1,    -1,
    1376,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,    -1,  1398,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1406,  1407,  1408,    -1,    -1,    -1,    -1,  1413,    -1,    -1,
      -1,  1417,    -1,    -1,    -1,  1421,  1422,  1423,  1424,    -1,
      -1,  1427,  1428,    -1,  1430,  1431,    -1,    -1,    -1,    -1,
      -1,    -1,  1438,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1449,   114,   115,   116,   117,   118,    -1,
      -1,   121,   122,   123,   124,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,
      -1,  1477,   142,   143,   144,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,  1491,  1492,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1505,
      -1,    -1,    -1,    -1,  1510,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,  1522,  1523,   188,    -1,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
      -1,  1537,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,
     210,    -1,    59,    60,   214,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1563,    -1,    -1,
      -1,    -1,  1568,  1569,     1,    -1,    -1,     4,     5,     6,
      -1,     8,     9,    10,    -1,    12,    -1,    14,    15,    16,
      17,    18,  1588,    -1,  1590,  1591,    -1,    24,    25,    26,
      27,    28,    -1,    30,  1600,    -1,    -1,    -1,    -1,    -1,
      37,    38,    -1,    -1,    41,   122,    43,    44,    -1,   126,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    -1,
      57,    58,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    -1,    70,    71,    72,    -1,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    92,    93,    94,    -1,    -1,
      97,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,   188,    -1,    -1,    -1,   192,    -1,   194,   195,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   210,   131,   132,   133,    -1,    -1,   216,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,   150,   151,   152,   153,    -1,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,    -1,   199,    -1,    -1,   202,   203,    -1,    -1,    -1,
      -1,    -1,    -1,   210,    -1,   212,     1,   214,   215,     4,
       5,     6,    -1,     8,     9,    10,    -1,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      25,    26,    27,    28,    -1,    30,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,    32,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,    -1,    -1,    -1,
      -1,    -1,    -1,   178,   179,   180,    59,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,    -1,   199,    -1,    -1,   202,   203,    -1,
      -1,     4,     5,    -1,    -1,   210,    -1,   212,    -1,   214,
     215,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   122,
      -1,    44,    -1,   126,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    59,    60,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,    72,
      -1,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,   192,
      -1,   194,   195,   116,   117,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,   126,    -1,    -1,    -1,   210,    -1,    -1,
      -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,
     153,    32,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    59,    60,
      -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,   192,
      -1,    -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,
     203,    -1,    -1,    -1,     4,     5,    -1,   210,    -1,   212,
      -1,   214,   215,   216,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,   122,    -1,    -1,    44,   126,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    -1,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,   188,    -1,    -1,
      -1,   192,    -1,   194,   195,    -1,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,
      -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,   151,   152,   153,    32,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   192,    -1,    -1,   195,   196,   197,    -1,   199,
      -1,    -1,   202,   203,    -1,    -1,    -1,     4,     5,    -1,
     210,    -1,   212,    -1,   214,   215,   216,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,   122,    -1,    -1,    44,   126,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    -1,    70,    71,    72,    -1,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    92,    93,    94,    -1,    -1,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
     188,    -1,    -1,    -1,   192,    -1,   194,   195,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,   210,    -1,   131,   132,   133,    -1,   216,    -1,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,   150,   151,   152,   153,    32,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,    -1,   199,    -1,    -1,   202,   203,    -1,    -1,     4,
       5,    -1,    -1,   210,    -1,   212,    -1,   214,   215,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,   122,    -1,    44,
      -1,   126,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    -1,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,   188,    -1,    -1,    -1,   192,    -1,   194,
     195,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,   210,   131,   132,   133,    -1,
      -1,   216,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,    32,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    59,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,    -1,   199,    -1,    -1,   202,   203,    -1,
      -1,     4,     5,    -1,    -1,   210,    -1,   212,    -1,   214,
     215,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    25,    26,    27,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   122,
      -1,    44,    -1,   126,    47,    -1,    -1,    50,    51,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,    72,
      -1,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    97,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,   188,    -1,    -1,    -1,   192,
      -1,   194,   195,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,    -1,    -1,
      -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,
     153,    32,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    59,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,
     203,    -1,    -1,     4,     5,    -1,    -1,   210,    -1,   212,
      -1,   214,   215,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,   122,    -1,    44,    -1,   126,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    94,    -1,    -1,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,   188,    -1,    -1,
      -1,   192,    -1,   194,   195,   116,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,
     131,   132,   133,    -1,    -1,   216,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
     151,   152,   153,    32,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      59,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,    -1,   199,    -1,
      -1,   202,   203,    -1,    -1,     4,     5,    -1,    -1,   210,
      -1,   212,    -1,   214,   215,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      32,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,   122,    -1,    44,    -1,   126,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    59,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,   188,
      -1,    -1,    -1,   192,    -1,   194,   195,   116,   117,    -1,
     122,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,   210,   131,   132,   133,    -1,    -1,   216,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,   151,   152,   153,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,
     192,    -1,   194,   195,    -1,    -1,   195,   196,   197,    -1,
     199,    -1,    -1,   202,   203,    -1,    -1,    -1,   210,     4,
       5,   210,    -1,   212,   216,   214,   215,    12,    -1,    14,
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
      -1,    -1,    -1,     4,     5,   210,   211,   212,    -1,   214,
     215,    12,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    48,    -1,    50,
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
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,    -1,   199,    -1,
      -1,   202,   203,    -1,    -1,    -1,    -1,     4,     5,   210,
      -1,   212,    -1,   214,   215,    12,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    48,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
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
     197,    -1,   199,    -1,    -1,   202,   203,    -1,    -1,    -1,
      -1,     4,     5,   210,   211,   212,    -1,   214,   215,    12,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    47,    48,    -1,    50,    -1,    -1,
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
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
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
      69,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
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
     167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,    -1,   199,    -1,    -1,   202,   203,    -1,    -1,     4,
       5,    -1,    -1,   210,    -1,   212,    -1,   214,   215,    14,
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
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
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
     167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,    -1,   199,    -1,    -1,   202,   203,    -1,    -1,     4,
       5,    -1,    -1,   210,    -1,   212,    -1,   214,   215,    14,
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
     165,   166,   167,   168,   169,   170,   171,    -1,    12,    -1,
      -1,    -1,    -1,   178,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
     195,   196,   197,    -1,   199,    -1,    40,   202,   203,    -1,
      -1,    -1,    -1,    -1,    48,   210,    -1,   212,    -1,   214,
     215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    40,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
     154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,   168,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,    -1,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    40,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,   154,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,   168,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
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
     216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,
      -1,   168,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,   214,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,   168,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   214,    20,    21,   134,   135,   136,   137,   138,   139,
      -1,    -1,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,    -1,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,
     210,    -1,    -1,    -1,   214,   215,    -1,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,    20,    21,    -1,    -1,   134,   135,   136,   137,
     138,   139,    -1,    -1,   142,   143,   144,   145,   146,   147,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,   187,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,    -1,   214,   215,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,   135,   136,   137,
     138,   139,    -1,    -1,   142,   143,   144,   145,   146,   147,
     148,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    20,    21,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,   180,    -1,    -1,   148,    -1,    -1,    -1,   187,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,    -1,   214,   215,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    20,    21,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    20,    21,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    20,    21,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    20,    21,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    20,    21,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    20,    21,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    20,    21,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    20,    21,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    20,    21,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    20,    21,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    20,    21,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    20,    21,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    20,    21,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    20,    21,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,   114,   115,   116,   117,
     118,    20,    21,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,   211,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,   211,    -1,   114,   115,   116,   117,   118,
      20,    21,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,   211,    -1,   114,   115,   116,   117,   118,    -1,
      -1,   121,   122,   123,   124,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,
      -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,   114,
     115,   116,   117,   118,    20,    21,   121,   122,   123,   124,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,   188,    -1,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,
     210,   211,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   209,   210,   211,    -1,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    20,    21,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    37,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,   211,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    -1,    -1,
     126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,   135,
     136,   137,   138,   139,    -1,    -1,   142,   143,   144,   145,
     146,   147,   148,   114,   115,   116,   117,   118,    20,    21,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,   202,   203,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,   114,   115,   116,   117,   118,    20,    21,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,   140,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,   140,    -1,   142,
     143,   144,    20,    21,    -1,   148,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    20,    21,    -1,
     148,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
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
      -1,   209,   210,   176,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,   118,   209,   210,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    20,    21,    -1,   148,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,   180,    -1,    -1,   148,
      20,    21,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,    -1,
      -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,
     209,   210,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
      20,    21,    -1,    -1,   114,   115,   116,   117,   118,    -1,
      -1,   121,   122,   123,   124,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,
      -1,   180,   142,   143,   144,    20,    21,    -1,   148,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
      -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,   209,
     210,   121,   122,   123,   124,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,
      -1,    -1,   142,   143,   144,    20,    21,    -1,   148,   114,
     115,   116,   117,   118,    -1,    -1,   121,   122,   123,   124,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,
     210,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,   114,
     115,   116,   117,   118,   209,   210,   121,   122,   123,   124,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,   144,
      20,    21,    -1,    -1,   114,   115,   116,   117,   118,    -1,
      -1,   121,   122,   123,   124,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,   136,   137,    20,    21,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   209,   210,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
      -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,   209,
     210,   121,   122,   123,   124,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,   136,   137,    20,    21,
      -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
      -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,   209,
     210,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,   209,   210,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    70,    71,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    -1,    97,    98,    99,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    72,   168,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    -1,    97,
      98,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    72,    -1,    74,    75,    76,    77,    78,   156,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
     168,    92,    93,    94,    -1,    -1,    97,    98,    99,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   168
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
     462,   473,   484,   488,   501,   504,   168,   180,   394,   395,
     289,   359,   376,   211,    64,   169,   283,   345,   168,   168,
     418,   125,   135,   187,   373,   419,   424,   426,   345,   428,
     422,   168,   414,   430,   432,   434,   436,   438,   440,   442,
     444,   345,   193,   212,    32,   192,    32,   192,   210,   216,
     211,   345,   210,   216,   418,   168,   180,   461,   168,   180,
     365,   446,   451,   168,   180,   368,   456,   345,   150,   168,
     372,   406,   363,   193,   193,   345,   250,   193,   291,   409,
     462,   212,   193,     4,   101,   102,   193,   212,   125,   288,
     319,   330,   345,   275,   193,   212,    60,   345,   212,   345,
     168,   193,   193,   212,   180,   193,   161,    57,   345,   212,
     275,   193,   212,   193,   193,   212,   193,   193,   125,   288,
     345,   345,   345,   215,   275,   321,   325,   325,   325,   212,
     212,   212,   212,   212,   212,    12,   418,    12,   418,    12,
     345,   483,   499,   193,   345,   193,   225,    12,   483,   500,
      36,   345,   345,   345,   345,   345,    12,    48,   319,   345,
     319,   215,   180,   180,   345,     9,   321,   327,   168,   212,
     180,   180,   180,   180,   180,    65,   304,   266,   130,   180,
      20,    21,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   134,   135,   136,   137,
     138,   139,   142,   143,   144,   145,   146,   147,   148,   187,
     188,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   209,   210,   327,   212,   180,   187,   179,   383,   179,
     210,   266,   360,   213,    42,   180,   373,   288,   345,   446,
     446,   416,   446,   213,   446,   446,   213,   168,   411,   446,
     267,   446,   267,   446,   267,   365,   366,   368,   369,   213,
     421,   280,   319,   211,   211,   345,   180,   179,   189,   180,
     179,   189,   180,   179,   213,   212,    42,   125,   186,   187,
     189,   192,   371,   474,   476,   275,   406,   292,   212,   289,
     316,   193,   193,   193,   495,   319,   288,   319,   186,   106,
     107,   108,   109,   110,   111,   112,   113,   119,   120,   125,
     138,   139,   145,   146,   147,   187,    13,   418,   500,   345,
     345,   275,   187,   309,   311,   345,   313,   189,   161,   345,
     497,   319,   480,   485,   319,   478,   418,   288,   345,   215,
     266,   345,   345,   345,   345,   345,   345,   406,    52,   156,
     168,   195,   210,   212,   345,   463,   465,   466,   482,   487,
     406,   212,   465,   487,   406,   140,   179,   180,   181,   467,
     284,   275,   286,   174,   175,   223,   406,   186,   503,   178,
     406,    12,   186,   503,   503,   150,   155,   193,   289,   335,
     275,   248,   328,   210,   213,   319,   463,   464,   158,   212,
     306,   376,   158,   324,   325,    18,   156,   168,   407,    18,
     156,   168,   407,   131,   132,   133,   276,   331,   345,   331,
     345,   331,   345,   331,   345,   331,   345,   331,   345,   331,
     345,   331,   345,   345,   345,   345,   331,   345,   331,   345,
     345,   345,   345,   168,   331,   345,   345,   156,   168,   345,
     345,   345,   407,   345,   345,   345,   331,   345,   331,   345,
     345,   345,   345,   331,   345,   331,   345,   331,   345,   345,
     331,   345,    21,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   127,   128,   156,   168,   209,   210,
     342,   345,   213,   319,   345,   396,   265,     7,   353,   358,
     168,   288,   345,   180,   194,   194,   194,   180,   194,   194,
     180,   194,   194,   268,   194,   268,   194,   268,   180,   194,
     180,   194,   282,   446,   213,   211,   446,   446,   345,   168,
     168,   446,   288,   345,   418,   418,    19,   446,    69,   319,
     472,   193,   345,   446,   489,   491,   493,   418,   503,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   275,   180,   194,
     503,   213,   255,   418,   418,   213,   418,   213,   418,   503,
     418,   418,   503,   418,   194,   324,   213,   213,   213,   213,
     213,   213,    19,   325,   212,   135,   371,   210,   345,   213,
     140,   186,   180,   466,   183,   184,   211,   470,   186,   180,
     183,   211,   469,    19,   213,   466,   179,   182,   468,    19,
     345,   179,   483,   282,   282,   345,    19,   483,   179,   278,
      19,   406,   211,   213,   212,   212,   337,   339,    11,    22,
      23,   241,   242,   345,   266,   463,   186,   213,   213,   168,
     305,   305,   125,   135,   187,   192,   322,   323,   267,   193,
     212,   193,   212,   325,   325,   325,   212,   212,   211,    18,
     156,   168,   407,   189,   156,   168,   345,   212,   212,   156,
     168,   345,     1,   211,   213,   180,   179,   211,    56,    62,
     356,    66,   357,   180,   180,   420,   425,   427,   446,   429,
     423,   168,   415,   431,   194,   435,   194,   439,   194,   365,
     443,   368,   445,   180,   194,   213,    42,   371,   194,   194,
     319,   194,   213,   168,   213,   464,   168,   213,   194,   418,
     418,   418,   194,   213,   418,   212,   213,   345,   194,   194,
     194,   194,   213,   194,   194,   213,   194,   324,   267,   212,
     319,   345,   345,   345,   465,   466,   345,   156,   168,   463,
     470,   211,   345,   482,   211,   319,   465,   179,   182,   185,
     471,   211,   319,   194,   194,   176,   319,   179,   319,    19,
     345,   345,   418,   267,   275,   345,    11,   244,   324,   211,
     210,   186,   211,   213,   168,   168,   168,   168,   186,   211,
     268,   346,   345,   348,   345,   213,   319,   345,   193,   212,
     345,   212,   211,   345,   213,   319,   212,   211,   343,   180,
      46,   357,    45,   105,   354,   433,   437,   441,   446,   212,
     168,   345,   475,   477,   275,   293,   213,   180,   194,   317,
     194,   194,   194,   496,   194,   500,   310,   312,   314,   498,
     481,   486,   479,   212,   268,   213,   319,   180,   213,   466,
     470,   212,   135,   371,   180,   466,   211,   180,   285,   287,
     180,   180,   319,   213,   213,   194,   268,   275,   245,   180,
     267,   213,   463,   168,   211,   189,   374,   322,   211,   140,
     275,   320,   418,   213,   446,   213,   213,   213,   350,   345,
     345,   213,   213,   345,    32,   355,   354,   356,   280,   212,
     212,   168,   345,   490,   492,   494,   212,   212,   503,   345,
     345,   345,   212,   472,   212,   212,   213,   345,   320,   213,
     345,   135,   371,   470,   345,   345,   345,   345,   471,   483,
     345,   212,   281,   483,   345,   180,   338,   194,   242,    25,
     104,   246,   295,   296,   297,   299,   345,   268,   211,   189,
     374,   418,   373,   125,   345,   194,   194,   446,   213,   213,
     213,   361,   355,   372,   213,   472,   472,   194,   213,   212,
     212,   212,   319,   500,   213,   319,   472,   465,   213,   180,
     502,   345,   345,   213,   502,   502,   280,   502,   502,   345,
     335,   340,   125,   125,   345,   275,   213,   418,   373,   373,
     345,   345,   347,   349,   194,   272,   362,   465,   465,   345,
     472,   472,   465,   503,   503,   503,   465,   213,    52,   211,
     135,   371,   179,   179,   213,   179,   211,   502,   335,   345,
     373,   345,   345,   180,   351,   180,   272,   213,   213,   213,
     465,   465,   213,   213,   213,   213,   213,   345,   211,   345,
     345,   211,   179,   211,   345,   180,   180,   275,   213,   213,
     180
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
     292,   293,   291,   291,   294,   294,   294,   294,   294,   295,
     296,   297,   297,   297,   298,   298,   298,   299,   299,   300,
     300,   300,   301,   302,   302,   302,   303,   303,   304,   304,
     305,   305,   306,   306,   306,   306,   307,   307,   309,   310,
     308,   311,   312,   308,   313,   314,   308,   316,   317,   315,
     318,   318,   318,   319,   319,   320,   320,   320,   321,   321,
     321,   322,   322,   322,   322,   323,   323,   324,   324,   325,
     325,   326,   328,   327,   329,   329,   329,   329,   329,   329,
     329,   330,   330,   330,   330,   330,   330,   330,   330,   330,
     330,   330,   330,   330,   330,   330,   330,   330,   330,   330,
     331,   331,   331,   331,   332,   332,   332,   332,   332,   332,
     332,   332,   332,   332,   332,   332,   332,   332,   332,   332,
     332,   333,   333,   334,   334,   335,   335,   336,   337,   338,
     336,   339,   340,   336,   341,   341,   341,   341,   342,   343,
     341,   344,   344,   344,   344,   344,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   346,   347,   345,   345,   345,   345,   348,   349,
     345,   345,   345,   350,   351,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   353,   353,   353,   354,
     354,   354,   355,   355,   356,   356,   356,   357,   357,   358,
     359,   360,   359,   361,   359,   362,   359,   359,   363,   363,
     364,   364,   365,   365,   366,   366,   367,   367,   367,   368,
     369,   369,   370,   370,   370,   371,   371,   372,   372,   372,
     372,   372,   372,   373,   373,   373,   374,   374,   375,   375,
     375,   375,   375,   376,   376,   376,   376,   376,   377,   378,
     377,   379,   379,   380,   380,   380,   381,   382,   381,   383,
     383,   383,   383,   384,   384,   384,   386,   385,   387,   387,
     388,   389,   388,   390,   390,   390,   391,   393,   394,   392,
     395,   396,   392,   397,   397,   398,   398,   399,   400,   400,
     401,   401,   401,   402,   402,   404,   405,   403,   406,   406,
     406,   406,   406,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   407,   407,   407,   407,   407,   407,   407,
     408,   408,   408,   408,   408,   408,   408,   408,   409,   410,
     410,   410,   411,   411,   412,   412,   412,   414,   415,   413,
     416,   416,   417,   417,   418,   418,   418,   418,   418,   418,
     419,   420,   418,   418,   418,   421,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   422,
     423,   418,   418,   424,   425,   418,   426,   427,   418,   428,
     429,   418,   418,   430,   431,   418,   432,   433,   418,   418,
     434,   435,   418,   436,   437,   418,   418,   438,   439,   418,
     440,   441,   418,   442,   443,   418,   444,   445,   418,   446,
     446,   446,   448,   449,   450,   451,   447,   453,   454,   455,
     456,   452,   458,   459,   460,   461,   457,   462,   462,   462,
     462,   462,   463,   463,   463,   463,   463,   463,   463,   463,
     464,   465,   465,   466,   466,   467,   467,   468,   468,   469,
     469,   470,   470,   470,   471,   471,   471,   472,   472,   473,
     473,   473,   473,   473,   473,   474,   475,   473,   476,   477,
     473,   478,   479,   473,   480,   481,   473,   482,   482,   482,
     483,   483,   484,   485,   486,   484,   487,   487,   488,   488,
     488,   489,   490,   488,   491,   492,   488,   493,   494,   488,
     488,   495,   496,   488,   488,   497,   498,   488,   499,   499,
     500,   500,   501,   501,   501,   501,   501,   502,   502,   503,
     503,   504,   504,   504,   504,   504,   504
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
       0,     0,     5,     1,     2,     5,     5,     6,     2,     1,
       1,     1,     2,     3,     2,     2,     3,     2,     3,     2,
       2,     3,     4,     1,     1,     0,     1,     1,     1,     0,
       1,     3,     9,     8,     8,     7,     3,     3,     0,     0,
       7,     0,     0,     7,     0,     0,     7,     0,     0,     6,
       5,     8,    10,     1,     3,     1,     2,     3,     1,     1,
       2,     2,     2,     2,     2,     1,     3,     0,     4,     1,
       6,     6,     0,     7,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     6,     8,     5,     6,     1,     4,     3,     0,     0,
       8,     0,     0,     9,     3,     4,     5,     6,     0,     0,
       5,     3,     4,     4,     3,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     2,     4,     4,     5,     4,
       5,     3,     4,     1,     1,     2,     4,     4,     7,     8,
       3,     5,     0,     0,     8,     3,     3,     3,     0,     0,
       8,     3,     4,     0,     0,     9,     4,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     2,     4,     1,
       4,     4,     4,     4,     4,     1,     6,     7,     6,     6,
       7,     7,     6,     7,     6,     6,     0,     4,     1,     0,
       1,     1,     0,     1,     0,     1,     1,     0,     1,     5,
       0,     0,     4,     0,     9,     0,    10,     5,     3,     4,
       1,     3,     1,     3,     1,     3,     0,     2,     3,     3,
       1,     3,     0,     2,     3,     1,     1,     1,     2,     3,
       5,     3,     3,     1,     1,     1,     0,     1,     1,     4,
       3,     3,     5,     4,     6,     5,     5,     4,     0,     0,
       4,     0,     1,     0,     1,     1,     6,     0,     6,     0,
       2,     3,     5,     0,     1,     1,     0,     5,     2,     3,
       4,     0,     4,     0,     1,     1,     1,     0,     0,     9,
       0,     0,    11,     0,     2,     0,     1,     3,     1,     1,
       0,     1,     1,     0,     3,     0,     0,     7,     1,     4,
       3,     3,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     1,     3,     0,     2,     3,     0,     0,     6,
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
       1,     1,     3,     0,     2,     2,     1,     2,     1,     2,
       1,     2,     1,     1,     2,     1,     1,     0,     1,     5,
       4,     6,     7,     5,     7,     0,     0,    10,     0,     0,
      10,     0,     0,     9,     0,     0,     7,     1,     3,     3,
       3,     1,     5,     0,     0,    10,     1,     3,     4,     4,
       4,     0,     0,    11,     0,     0,    11,     0,     0,    10,
       5,     0,     0,    10,     5,     0,     0,    10,     1,     3,
       1,     3,     4,     3,     5,     8,    10,     0,     3,     0,
       1,     9,    10,    10,    10,     9,    10
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

    case YYSYMBOL_make_struct_single: /* make_struct_single  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_struct_dim: /* make_struct_dim  */
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

  case 277: /* expr_new: "new" new_type_declaration '(' use_initializer make_struct_single ')'  */
                                                                                                            {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 278: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 279: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 280: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 281: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 282: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 283: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 284: /* expression_return: expression_return_no_pipe "end of expression"  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 285: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 286: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 287: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 288: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 289: /* expression_yield: expression_yield_no_pipe "end of expression"  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 290: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 291: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 292: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 293: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 294: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 295: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 296: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 297: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 298: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 299: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 300: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 301: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 302: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 303: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 304: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 305: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 306: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 307: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 308: /* $@13: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 309: /* $@14: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 310: /* expr_cast: "cast" '<' $@13 type_declaration_no_options '>' $@14 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 311: /* $@15: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 312: /* $@16: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 313: /* expr_cast: "upcast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 314: /* $@17: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 315: /* $@18: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 316: /* expr_cast: "reinterpret" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 317: /* $@19: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 318: /* $@20: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 319: /* expr_type_decl: "type" '<' $@19 type_declaration '>' $@20  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 320: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 321: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 322: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" "end of expression" "name" '>' expr ')'  */
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

  case 323: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 324: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 325: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 326: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 327: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 328: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 329: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 330: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 331: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 332: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 333: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 334: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 335: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 336: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 337: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 338: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 339: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 340: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 341: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 342: /* $@21: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 343: /* expr_full_block_assumed_piped: block_or_lambda $@21 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 344: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 345: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 346: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 347: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 348: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 349: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 350: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 351: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 352: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 353: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 354: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 355: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 356: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 357: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 358: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 359: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 360: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 361: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 362: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 363: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 364: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 365: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 366: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 367: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 368: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 369: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 370: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 371: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 372: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 373: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 374: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 392: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 393: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 394: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 395: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 396: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 397: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 398: /* $@22: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 399: /* $@23: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 400: /* func_addr_expr: '@' '@' '<' $@22 type_declaration_no_options '>' $@23 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 401: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 402: /* $@25: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 403: /* func_addr_expr: '@' '@' '<' $@24 optional_function_argument_list optional_function_type '>' $@25 func_addr_name  */
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

  case 404: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 405: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 406: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 407: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 408: /* $@26: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 409: /* $@27: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 410: /* expr_field: expr '.' $@26 error $@27  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 411: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 412: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 413: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 414: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 415: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 416: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 417: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 418: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 419: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 420: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 421: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 422: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 423: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 424: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 425: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 426: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 427: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 428: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 429: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 430: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 431: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 432: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 433: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 434: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 435: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 436: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 437: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 438: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 439: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 440: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 441: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 442: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 443: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 444: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 445: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 446: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 447: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 448: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 449: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 450: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 451: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 452: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 453: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 454: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 455: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 456: /* expr: '(' expr_list optional_comma ')'  */
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

  case 457: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 458: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 459: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 460: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 461: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 462: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 463: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 464: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 465: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 466: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 467: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 468: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 469: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 470: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 471: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 472: /* $@28: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 473: /* $@29: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 474: /* expr: expr "is" "type" '<' $@28 type_declaration_no_options '>' $@29  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 475: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 476: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 477: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 478: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 479: /* $@31: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 480: /* expr: expr "as" "type" '<' $@30 type_declaration '>' $@31  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 481: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 482: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 483: /* $@32: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 484: /* $@33: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 485: /* expr: expr '?' "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 486: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 487: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 488: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 489: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 490: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 491: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 492: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 493: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 494: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 495: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 496: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 497: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 498: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 499: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 500: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 501: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 502: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 503: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 504: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 505: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 506: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 507: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 508: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 509: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 510: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 511: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 512: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 513: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 514: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 515: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 516: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 517: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); /*this one is gone when BRABRA is disabled*/ }
    break;

  case 518: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 519: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 520: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 521: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 522: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 523: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 524: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 525: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 526: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 527: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 528: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 529: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 530: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 531: /* $@34: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 532: /* struct_variable_declaration_list: struct_variable_declaration_list $@34 structure_variable_declaration "end of expression"  */
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

  case 533: /* $@35: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 534: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@35 function_declaration_header "end of expression"  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 535: /* $@36: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 536: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@36 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 537: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' "end of expression"  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 538: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 539: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 540: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 541: /* function_argument_list: function_argument_list "end of expression" function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 542: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 543: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 544: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 545: /* tuple_type_list: tuple_type_list "end of expression" tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 546: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 547: /* tuple_alias_type_list: tuple_alias_type_list "end of expression"  */
                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 548: /* tuple_alias_type_list: tuple_alias_type_list tuple_type "end of expression"  */
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

  case 549: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 550: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 551: /* variant_type_list: variant_type_list "end of expression" variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 552: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 553: /* variant_alias_type_list: variant_alias_type_list "end of expression"  */
                                        {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 554: /* variant_alias_type_list: variant_alias_type_list variant_type "end of expression"  */
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

  case 555: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 556: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 557: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 558: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 559: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 560: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 561: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 562: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 563: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 564: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 565: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 566: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 567: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 568: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 569: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 570: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 571: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 572: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 573: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options "end of expression"  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 574: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 575: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 576: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 577: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 578: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 579: /* $@37: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 580: /* global_variable_declaration_list: global_variable_declaration_list $@37 optional_field_annotation let_variable_declaration  */
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

  case 581: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 582: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 583: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 584: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 585: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 586: /* global_let: kwd_let optional_shared optional_public_or_private_variable "begin of code block" global_variable_declaration_list "end of code block"  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 587: /* $@38: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 588: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@38 optional_field_annotation let_variable_declaration  */
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

  case 589: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 590: /* enum_list: enum_list "end of expression"  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 591: /* enum_list: enum_list "name" "end of expression"  */
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

  case 592: /* enum_list: enum_list "name" '=' expr "end of expression"  */
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

  case 593: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 594: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 595: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 596: /* $@39: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 597: /* single_alias: optional_public_or_private_alias "name" $@39 '=' type_declaration  */
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

  case 601: /* $@40: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 603: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 604: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 605: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 606: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 607: /* $@41: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 608: /* $@42: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 609: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name "begin of code block" $@41 enum_list $@42 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 610: /* $@43: %empty  */
                                                                                                                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 611: /* $@44: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 612: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration "begin of code block" $@43 enum_list $@44 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 613: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 614: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 615: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 616: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 617: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 618: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 619: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 620: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 621: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 622: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 623: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 624: /* optional_struct_variable_declaration_list: "begin of code block" struct_variable_declaration_list "end of code block"  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 625: /* $@45: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 626: /* $@46: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 627: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@45 structure_name $@46 optional_struct_variable_declaration_list  */
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

  case 628: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 629: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 630: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 631: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 632: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 633: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 634: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 635: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 636: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 637: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 638: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 639: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 640: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 641: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 642: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 643: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 644: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 645: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 646: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 647: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 648: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 649: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 650: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 651: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 652: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 653: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 654: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 655: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 656: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 657: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 658: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 659: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 660: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 661: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 662: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 663: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 664: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 665: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 666: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 667: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 668: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 669: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 670: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 671: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 672: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 673: /* bitfield_bits: bitfield_bits "end of expression" "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 674: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 675: /* bitfield_alias_bits: bitfield_alias_bits "end of expression"  */
                                      {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 676: /* bitfield_alias_bits: bitfield_alias_bits "name" "end of expression"  */
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

  case 677: /* $@47: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 678: /* $@48: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 679: /* bitfield_type_declaration: "bitfield" '<' $@47 bitfield_bits '>' $@48  */
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

  case 680: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 681: /* table_type_pair: type_declaration "end of expression" type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 682: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 683: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 684: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 685: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 686: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 687: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 688: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 689: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 690: /* $@49: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 691: /* $@50: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 692: /* type_declaration_no_options: "type" '<' $@49 type_declaration '>' $@50  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 693: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 694: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 695: /* $@51: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 696: /* type_declaration_no_options: '$' name_in_namespace '<' $@51 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 697: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 698: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 699: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 700: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 701: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 702: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 703: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 704: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 705: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 706: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 707: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 708: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 709: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 710: /* $@53: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 711: /* type_declaration_no_options: "smart_ptr" '<' $@52 type_declaration '>' $@53  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 712: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 713: /* $@54: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 714: /* $@55: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 715: /* type_declaration_no_options: "array" '<' $@54 type_declaration '>' $@55  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 716: /* $@56: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 717: /* $@57: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 718: /* type_declaration_no_options: "table" '<' $@56 table_type_pair '>' $@57  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 719: /* $@58: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 720: /* $@59: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 721: /* type_declaration_no_options: "iterator" '<' $@58 type_declaration '>' $@59  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 722: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 723: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 724: /* $@61: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 725: /* type_declaration_no_options: "block" '<' $@60 type_declaration '>' $@61  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 726: /* $@62: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 727: /* $@63: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 728: /* type_declaration_no_options: "block" '<' $@62 optional_function_argument_list optional_function_type '>' $@63  */
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

  case 729: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 730: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 731: /* $@65: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 732: /* type_declaration_no_options: "function" '<' $@64 type_declaration '>' $@65  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 733: /* $@66: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 734: /* $@67: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 735: /* type_declaration_no_options: "function" '<' $@66 optional_function_argument_list optional_function_type '>' $@67  */
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

  case 736: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 737: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 738: /* $@69: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 739: /* type_declaration_no_options: "lambda" '<' $@68 type_declaration '>' $@69  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 740: /* $@70: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 741: /* $@71: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 742: /* type_declaration_no_options: "lambda" '<' $@70 optional_function_argument_list optional_function_type '>' $@71  */
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

  case 743: /* $@72: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 744: /* $@73: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 745: /* type_declaration_no_options: "tuple" '<' $@72 tuple_type_list '>' $@73  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 746: /* $@74: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 747: /* $@75: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 748: /* type_declaration_no_options: "variant" '<' $@74 variant_type_list '>' $@75  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 749: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 750: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 751: /* type_declaration: type_declaration '|' '#'  */
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

  case 752: /* $@76: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 753: /* $@77: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 754: /* $@78: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 755: /* $@79: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 756: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@76 "name" $@77 "begin of code block" $@78 tuple_alias_type_list $@79 "end of code block"  */
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

  case 757: /* $@80: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 758: /* $@81: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 759: /* $@82: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 760: /* $@83: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 761: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@80 "name" $@81 "begin of code block" $@82 variant_alias_type_list $@83 "end of code block"  */
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

  case 762: /* $@84: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 763: /* $@85: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 764: /* $@86: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 765: /* $@87: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 766: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@84 "name" $@85 "begin of code block" $@86 bitfield_alias_bits $@87 "end of code block"  */
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

  case 767: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 768: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 769: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 770: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 771: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 772: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 773: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 774: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 775: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 776: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 777: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 778: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 779: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 780: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 781: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 782: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 783: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 784: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 797: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 798: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 799: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 800: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 801: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 802: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 803: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 804: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 805: /* $@88: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 806: /* $@89: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 807: /* make_struct_decl: "struct" '<' $@88 type_declaration_no_options '>' $@89 '(' use_initializer make_struct_dim ')'  */
                                                                                                                                                                                                                              {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 808: /* $@90: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 809: /* $@91: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 810: /* make_struct_decl: "class" '<' $@90 type_declaration_no_options '>' $@91 '(' use_initializer make_struct_dim ')'  */
                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 811: /* $@92: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 812: /* $@93: %empty  */
                                                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 813: /* make_struct_decl: "variant" '<' $@92 type_declaration_no_options '>' $@93 '(' make_struct_dim ')'  */
                                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 814: /* $@94: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 815: /* $@95: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 816: /* make_struct_decl: "default" '<' $@94 type_declaration_no_options '>' $@95 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 817: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 818: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 819: /* make_tuple: make_tuple ',' expr  */
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

  case 820: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 821: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 822: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 823: /* $@96: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 824: /* $@97: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 825: /* make_tuple_call: "tuple" '<' $@96 type_declaration_no_options '>' $@97 '(' use_initializer make_struct_dim ')'  */
                                                                                                                                                                                                                              {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 826: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 827: /* make_dim: make_dim "end of expression" make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 828: /* make_dim_decl: '[' expr_list optional_comma ']'  */
                                                          {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 829: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 830: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 831: /* $@98: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 832: /* $@99: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 833: /* make_dim_decl: "array" "struct" '<' $@98 type_declaration_no_options '>' $@99 '(' use_initializer make_struct_dim ')'  */
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

  case 834: /* $@100: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 835: /* $@101: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 836: /* make_dim_decl: "array" "tuple" '<' $@100 type_declaration_no_options '>' $@101 '(' use_initializer make_struct_dim ')'  */
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

  case 837: /* $@102: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 838: /* $@103: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 839: /* make_dim_decl: "array" "variant" '<' $@102 type_declaration_no_options '>' $@103 '(' make_struct_dim ')'  */
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

  case 840: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 841: /* $@104: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 842: /* $@105: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 843: /* make_dim_decl: "array" '<' $@104 type_declaration_no_options '>' $@105 '(' expr_list optional_comma ')'  */
                                                                                                                                                                              {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-9])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 844: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 845: /* $@106: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 846: /* $@107: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 847: /* make_dim_decl: "fixed_array" '<' $@106 type_declaration_no_options '>' $@107 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        if ( !mka->makeType->dim.size() ) mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 848: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 849: /* make_table: make_table "end of expression" make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 850: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 851: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 852: /* make_table_decl: "begin of code block" expr_map_tuple_list optional_comma "end of code block"  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 853: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 854: /* make_table_decl: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 855: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
                                                                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-7])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-7])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 856: /* make_table_decl: "table" '<' type_declaration_no_options "end of expression" type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
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

  case 857: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 858: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 859: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 860: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 861: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 862: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 863: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 864: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 865: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 866: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block" "end of code block"  */
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


