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
#define YYLAST   14746

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  217
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  291
/* YYNRULES -- Number of rules.  */
#define YYNRULES  876
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1669

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
    1220,  1231,  1240,  1249,  1255,  1256,  1257,  1258,  1259,  1260,
    1261,  1262,  1263,  1264,  1265,  1266,  1267,  1268,  1269,  1270,
    1271,  1272,  1273,  1274,  1275,  1279,  1284,  1290,  1296,  1307,
    1308,  1312,  1313,  1317,  1321,  1328,  1328,  1328,  1334,  1334,
    1334,  1343,  1377,  1380,  1383,  1386,  1392,  1393,  1404,  1408,
    1411,  1419,  1419,  1419,  1422,  1428,  1431,  1435,  1439,  1446,
    1453,  1459,  1463,  1467,  1470,  1473,  1481,  1484,  1487,  1495,
    1498,  1506,  1509,  1512,  1520,  1526,  1527,  1528,  1532,  1533,
    1537,  1538,  1542,  1547,  1555,  1561,  1567,  1576,  1588,  1591,
    1597,  1597,  1597,  1600,  1600,  1600,  1605,  1605,  1605,  1613,
    1613,  1613,  1619,  1629,  1640,  1653,  1663,  1674,  1689,  1692,
    1698,  1699,  1706,  1717,  1718,  1719,  1723,  1724,  1725,  1726,
    1730,  1735,  1743,  1744,  1748,  1753,  1760,  1767,  1767,  1776,
    1777,  1778,  1779,  1780,  1781,  1782,  1786,  1787,  1788,  1789,
    1790,  1791,  1792,  1793,  1794,  1795,  1796,  1797,  1798,  1799,
    1800,  1801,  1802,  1803,  1804,  1808,  1809,  1810,  1811,  1816,
    1817,  1818,  1819,  1820,  1821,  1822,  1823,  1824,  1825,  1826,
    1827,  1828,  1829,  1830,  1831,  1832,  1837,  1844,  1856,  1861,
    1871,  1875,  1882,  1885,  1885,  1885,  1890,  1890,  1890,  1903,
    1907,  1911,  1916,  1923,  1923,  1923,  1930,  1934,  1943,  1947,
    1950,  1956,  1957,  1958,  1959,  1960,  1961,  1962,  1963,  1964,
    1965,  1966,  1967,  1968,  1969,  1970,  1971,  1972,  1973,  1974,
    1975,  1976,  1977,  1978,  1979,  1980,  1981,  1982,  1983,  1984,
    1985,  1986,  1987,  1988,  1989,  1990,  1991,  1997,  1998,  1999,
    2000,  2001,  2014,  2015,  2016,  2017,  2018,  2019,  2020,  2021,
    2022,  2023,  2024,  2025,  2028,  2031,  2032,  2035,  2035,  2035,
    2038,  2043,  2047,  2051,  2051,  2051,  2056,  2059,  2063,  2063,
    2063,  2068,  2071,  2072,  2073,  2074,  2075,  2076,  2077,  2078,
    2079,  2081,  2085,  2086,  2091,  2095,  2096,  2097,  2098,  2099,
    2100,  2101,  2105,  2109,  2113,  2117,  2121,  2125,  2129,  2133,
    2137,  2144,  2145,  2146,  2150,  2151,  2152,  2156,  2157,  2161,
    2162,  2163,  2167,  2168,  2172,  2183,  2186,  2186,  2205,  2204,
    2219,  2218,  2231,  2240,  2249,  2259,  2260,  2264,  2267,  2276,
    2277,  2281,  2284,  2287,  2303,  2312,  2313,  2317,  2320,  2323,
    2337,  2338,  2342,  2348,  2354,  2357,  2361,  2367,  2376,  2377,
    2378,  2382,  2383,  2387,  2394,  2399,  2408,  2414,  2425,  2428,
    2433,  2438,  2446,  2457,  2460,  2460,  2480,  2481,  2485,  2486,
    2487,  2491,  2494,  2494,  2513,  2516,  2519,  2534,  2553,  2554,
    2555,  2560,  2560,  2586,  2587,  2591,  2592,  2592,  2596,  2597,
    2598,  2602,  2612,  2617,  2612,  2629,  2634,  2629,  2649,  2650,
    2654,  2655,  2659,  2665,  2666,  2670,  2671,  2672,  2676,  2679,
    2685,  2690,  2685,  2704,  2711,  2716,  2725,  2731,  2742,  2743,
    2744,  2745,  2746,  2747,  2748,  2749,  2750,  2751,  2752,  2753,
    2754,  2755,  2756,  2757,  2758,  2759,  2760,  2761,  2762,  2763,
    2764,  2765,  2766,  2767,  2768,  2772,  2773,  2774,  2775,  2776,
    2777,  2778,  2779,  2783,  2794,  2798,  2805,  2817,  2824,  2833,
    2838,  2841,  2854,  2854,  2854,  2867,  2871,  2878,  2882,  2889,
    2890,  2891,  2892,  2893,  2908,  2914,  2914,  2914,  2918,  2923,
    2930,  2930,  2937,  2941,  2945,  2950,  2955,  2960,  2965,  2969,
    2973,  2978,  2982,  2986,  2991,  2991,  2991,  2997,  3004,  3004,
    3004,  3009,  3009,  3009,  3015,  3015,  3015,  3020,  3024,  3024,
    3024,  3029,  3029,  3029,  3038,  3042,  3042,  3042,  3047,  3047,
    3047,  3056,  3060,  3060,  3060,  3065,  3065,  3065,  3074,  3074,
    3074,  3080,  3080,  3080,  3089,  3092,  3103,  3119,  3119,  3124,
    3129,  3119,  3154,  3154,  3159,  3165,  3154,  3190,  3190,  3195,
    3200,  3190,  3230,  3231,  3232,  3233,  3234,  3238,  3245,  3252,
    3258,  3264,  3271,  3278,  3284,  3293,  3299,  3307,  3312,  3319,
    3324,  3331,  3336,  3342,  3343,  3348,  3349,  3353,  3354,  3358,
    3359,  3363,  3364,  3365,  3369,  3370,  3371,  3375,  3376,  3380,
    3386,  3393,  3401,  3408,  3416,  3425,  3425,  3425,  3433,  3433,
    3433,  3440,  3440,  3440,  3447,  3447,  3447,  3458,  3461,  3467,
    3481,  3487,  3493,  3499,  3499,  3499,  3509,  3514,  3521,  3529,
    3534,  3541,  3541,  3541,  3551,  3551,  3551,  3561,  3561,  3561,
    3571,  3579,  3579,  3579,  3598,  3605,  3605,  3605,  3615,  3620,
    3627,  3630,  3636,  3644,  3653,  3661,  3669,  3682,  3683,  3687,
    3688,  3693,  3696,  3699,  3702,  3705,  3708
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

#define YYPACT_NINF (-1449)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-746)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1449,    78, -1449, -1449,    81,    22,   230,    21, -1449,   -40,
     423,   423,   423, -1449,   185,   111, -1449, -1449,    96, -1449,
   -1449, -1449,   404, -1449,   241, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449,   146, -1449,   155,   225,   252,
   -1449, -1449, -1449, -1449,   230, -1449,    31, -1449,   423,   423,
   -1449, -1449,   241, -1449, -1449, -1449, -1449, -1449,   290,   398,
   -1449, -1449, -1449,   111,   111,   111,   342, -1449,   538,   121,
   -1449, -1449, -1449, -1449,   519,   574,   582, -1449,   583,    50,
      81,   395,    22,   362,   411, -1449,   607,   607, -1449,   414,
     431,    15,   440,   603,   435,   475,   479, -1449,   488,   491,
   -1449, -1449,   -12,    81,   111,   111,   111,   111, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449,   500, -1449, -1449, -1449, -1449,
   -1449,   515, -1449, -1449, -1449, -1449, -1449,   581,   132, -1449,
   -1449, -1449, -1449,   639, -1449, -1449, -1449, -1449, -1449,   517,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,   554,
   -1449,   -20, -1449,   346,   565,   538, 14578, -1449,   284,   643,
   -1449,   -91, -1449, -1449,   618, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449,   157, -1449,   522, -1449,   595,   601,   625, -1449,
   -1449, 13261, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449,   700,   748, -1449,
     573,   627, -1449,   453, -1449,   628, -1449,   634,    81,    81,
     593,    97, -1449, -1449, -1449,   132, -1449,  9915, -1449, -1449,
   -1449,   646,   647, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449,   650,   610, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449,   801, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
     656,   615, -1449, -1449,    95,   636, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449,   649,   641,   657,
   -1449,   -91,   202, -1449,    81,   616,   792,   468, -1449, -1449,
     645,   651,   653,   635,   658,   659, -1449, -1449, -1449,   637,
   -1449, -1449, -1449, -1449, -1449,   660, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449,   661, -1449, -1449,
   -1449,   662,   663, -1449, -1449, -1449, -1449,   666,   667,   654,
     185, -1449, -1449, -1449, -1449, -1449,  4756,   671, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449,   694,   704, -1449,   655, -1449,
     192, -1449,   -11,  9915, -1449,  2316,   101, -1449,   185, -1449,
   -1449, -1449,    97,   652, -1449,  9209,   697,   701,  9915, -1449,
     150, -1449, -1449, -1449,  9209, -1449, -1449,   702, -1449,   280,
     329,   335, -1449, -1449,  9209,    75, -1449, -1449, -1449,    25,
   -1449, -1449, -1449,    14,  5569, -1449,   668,  9671,   380,  9770,
     410, -1449, -1449,  9209, -1449, -1449,   266,   167, -1449,   671,
   -1449,   675,   679,  9209, -1449, -1449, -1449, -1449, -1449,   137,
     -62,   686,    38,  3339, -1449, -1449,   627,   285,  5771,   670,
    9209,   712,   695,   696,   678, -1449,   711,   703,   732,  5973,
    -102,   328,   705, -1449,   345,   706,   707,  3541,  9209,  9209,
     114,   114,   114,   682,   683,   689,   692,   693,   699, -1449,
    1897,  9572,  6177, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
    6379,   714, -1449,  6583,   873, -1449,  9209,  9209,  9209,  9209,
    9209,  4963,  9209, -1449,   709, -1449, -1449,   736,   745,  9209,
     917, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
     552, -1449,    92,   747, -1449,   749,   750,   751, -1449,   754,
   -1449, -1449,   863, -1449, -1449, -1449, -1449,   726, -1449, -1449,
     -10, -1449, -1449, -1449, -1449, -1449, -1449,  1672, -1449,   723,
   -1449, -1449, -1449, -1449, -1449, -1449,   144, -1449,   758, -1449,
   -1449,    36, -1449, -1449,   728,   752,   759, -1449, 10464, -1449,
     900,  1019, -1449, -1449, -1449,  3945,  9915,  9915,  9915, 10499,
    9915,  9915,   731,   779,  9915,   573,  9915,   573,  9915,   573,
   10014,   785, 10609, -1449,  9209, -1449, -1449, -1449, -1449,   743,
   -1449, -1449, 12784,  9209, -1449,  4756,   775, -1449,   778,   -66,
   -1449,   782,   671,   780,   769, -1449,   783,   787, 10644,   753,
     922, -1449,   130, -1449, -1449, -1449, 13296,   266, -1449,   755,
   -1449, -1449,   185,   356, -1449,   776,   777,   784, -1449,  9209,
    3945, -1449,   790,   838, 10097,   958,  9915,  9209,  9209, 14037,
    9209, 13296,   791, -1449, -1449,  9209, -1449, -1449,   797,   811,
   14037,  9209, -1449, -1449,  9209, -1449, -1449,  9209, -1449,  9915,
    3945, -1449, 10097,    61,    61,   765, -1449,   726, -1449, -1449,
   -1449,  9209,  9209,  9209,  9209,  9209,  9209,   266,  2730,   266,
    2933,   266, 13395, -1449,   614, -1449, 13296, -1449,   597,   266,
   -1449,   795,   809,    61,    61,   -13,    61,    61,   266,   976,
     804, 14037,   804,   260, -1449, -1449, 13296, -1449, -1449, -1449,
   -1449,  5165, -1449, -1449, -1449, -1449, -1449, -1449,   106,   833,
     114, -1449, 14421, 14452,  4147,  4147,  4147,  4147,  4147,  4147,
    4147,  4147,  9209,  9209, -1449, -1449,  9209,  4147,  4147,  9209,
    9209,  9209,   824,  4147,  9209,   441,  9209,  9209,  9209,  9209,
    9209,  9209,  4147,  4147,  9209,  9209,  9209,  4147,  4147,  4147,
    9209,  4147,  6785,  9209,  9209,  9209,  9209,  9209,  9209,  9209,
    9209,  9209,  9209,   -73,  9209, -1449,  6987, -1449,  9209, -1449,
     101, -1449,   111,   987,   -91,  9915, -1449,   829, -1449,  3945,
   -1449, 10209,   390,   437,   806,   -78, -1449,   518,   534, -1449,
   -1449,    79,   535,   636,   545,   636,   547,   636, -1449,   196,
   -1449,   242, -1449,  9915,   789,   804, -1449, -1449, 12819, -1449,
   -1449,  9915, -1449, -1449,  9915, -1449, -1449, -1449,  9209,   830,
   -1449,   835, -1449,  9915, -1449,  3945,  9915,  9915, -1449,    26,
    9915,  5367,  7189,   837,  9209,  9915, -1449, -1449, -1449,  9915,
     804, -1449,   790,  9209,  9209,  9209,  9209,  9209,  9209,  9209,
    9209,  9209,  9209,  9209,  9209,  9209,  9209,  9209,  9209,  9209,
    9209,   627,   564,   795, 14037, 10754, -1449, -1449,  9915,  9915,
   10789,  9915, -1449, -1449, 10899,  9915,   804,  9915,  9915,   804,
    9915,  1071, -1449, 10097, -1449,   833, 10934, 11044, 11079, 11189,
   11224, 11334,    28,   114,   794,   217,  3136,  4351,  7391, 13494,
     815,     1,   -95,   821,   129,    37,  7593,     1,   604,    39,
    9209,   831,  9209, -1449, -1449,  9915, -1449,  9915, -1449,  9209,
     629,    41,  9209,   832, -1449,    42,   266,  9209,   802,   799,
     796,   803,   343, -1449, -1449,   577,  9209,   726,  4555, -1449,
     208,   815,   810,   846,   846, -1449, -1449,   119,   573, -1449,
     823,   813, -1449, -1449,   844,   840, -1449, -1449,   114,   114,
     114, -1449, -1449, 10354, -1449, 10354, -1449, 10354, -1449, 10354,
   -1449, 10354, -1449, 10354, -1449, 10354, -1449, 10354,   822,   822,
     478, -1449, 10354, -1449, 10354,   478,  1943,  1943,   841, -1449,
   10354,   222,   842, -1449, 12918,   -55,   -55,   723, 14037,   822,
     822, -1449, 10354, -1449, 10354, 14162,  2092,  1634, -1449, 10354,
   -1449, 10354, -1449, 10354, 14127, -1449, 10354, 14547, 13529, 14252,
   14280,   912,   478,   478,   786,   786,   222,   222,   222,   454,
    9209,   843,   845,   457,  9209,  1021, 13017, -1449,   234, 13619,
     864,   237,   622,   978,   862,  1163, -1449, -1449, 10319, -1449,
   -1449, -1449, -1449,  9915, -1449, -1449,   877, -1449, -1449,   856,
   -1449,   866, -1449,   867, 10014, -1449,   785, -1449,   339,   671,
   -1449, -1449, -1449,   671,   671, 11369, -1449,  1014,   -39, -1449,
   10097,  1503,  1814,  9209,   553,   461,   238,   849,   850,   890,
   11479,   379, 11514,   555,  9915,  9915,  9915,  2236,   851, 14037,
   14037, 14037, 14037, 14037, 14037, 14037, 14037, 14037, 14037, 14037,
   14037, 14037, 14037, 14037, 14037, 14037, 14037, -1449,  9915,   854,
     855, -1449,  9209,  2650,  2852, -1449,  3055, -1449,  3258,   859,
    3461,  3663,   860,  3865,   833,   573, -1449, -1449, -1449, -1449,
   -1449,   865,  9209, -1449,  9209,  9209,  9209,  4759, 12784,     7,
    9209,   465,   461,   -95, -1449, -1449,   869, -1449,  9209,  9209,
   -1449,   872, -1449,  9209,   461,   485,   874, -1449, -1449,  9209,
   14037, -1449, -1449,   389,   397, 13654,  9209, -1449, -1449,  2528,
    9209,    47, -1449, -1449,  9209,  9209,  9915,   573,   627, -1449,
   -1449,  9209, -1449,  1899,   833,   282,  9411, -1449, -1449, -1449,
     305,   257,   899,   906,   907,   908, -1449,   309,   636, -1449,
    9209, -1449,  9209, -1449, -1449, -1449,  7795,  9209, -1449,   891,
     876, -1449, -1449,  9209,   878, -1449, 13052,  9209,  7997,   880,
   -1449, 13151, -1449, -1449, -1449, -1449, -1449,   909, -1449, -1449,
     326, -1449,    40, -1449,   833, -1449, -1449, -1449, -1449,   671,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449,  9915,   881, -1449,   918,  9209, -1449,
   -1449,   -82, -1449,   209,   882, -1449, -1449, -1449,   407, -1449,
     928,   885, -1449, -1449,  4067,  4144,  4271, -1449, -1449,  4348,
    9209, -1449, 14037, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449,   548,   636,  8199,   499, 11624, 14037, 14037,     1,
     -95, 14037,   886,   210,   815, -1449, -1449, 14037,   821, -1449,
     503,     1,   901, -1449, -1449, -1449, -1449,   516, -1449, -1449,
   -1449,   524, -1449,   525,  9209, 11659, 11769,  4475,   636, -1449,
   13296, -1449,   934,   573,   902,  4555,   948,   914,   -24, -1449,
   -1449, -1449, -1449,   119,   915,    94,  9915, 11804,  9915, 11914,
   -1449,   258, 11949, -1449,  9209,  1424,  9209, -1449, 12059, -1449,
     268,  9209, -1449, -1449, -1449,  1085,    40, -1449, -1449,   622,
     916, -1449, -1449, -1449,   671,  9209, -1449, 14037,   920,   923,
   -1449, -1449, -1449,   951,  9209,   926,  9209, -1449, -1449, -1449,
   -1449,   924,   925,   795,  9209,  9209,  9209,   927,  1058,   931,
     935,  8401, -1449,    94, -1449,   269,  9209,   243,   -95, -1449,
    9209,  9209,  9209,  9209,   485, -1449,  9209,  9209,   936,  9209,
    9209,   537, -1449, -1449, -1449,   940,   577,  3743, -1449,   636,
   -1449,   323, -1449,   527,  9915,   150, -1449, -1449,  8603, -1449,
   -1449,  4552, -1449,   556, -1449, -1449, -1449,  9915, 12094, 12204,
   -1449, -1449, 12239, -1449, -1449,  1085,   266,   933,  1058,  1058,
     944, 12349,   937, 12384,   938,   941,   945,  9209,  9209,   943,
     478,   478,   478,  9209, -1449, -1449,  1058,   461, -1449, 12494,
   -1449, -1449, 13744,  9209,  9209, -1449, 12529, 14037, 14037, 13744,
   -1449,   949,   478,  9209, -1449,   949, 13744,  9209,   277, -1449,
   -1449,  8805,  9007, -1449, -1449, -1449, -1449, -1449, 14037,   627,
     946,  9915,   150,   526,  9209,  9209, 14127, -1449, -1449,   571,
   -1449, -1449, -1449, 14578, -1449, -1449, -1449,   145,   145,  9209,
   -1449,  9209, -1449,  1058,  1058,   461,   947,   795, -1449,   804,
     145,   815,   952, -1449,  1088,   950, 14037, 14037,   250,   985,
     989,   984,   992,   961, 13744, -1449,   277,  9209,  9209, 14037,
   -1449, -1449,   526,  9209,  9209, 13783,  1424, -1449, -1449, -1449,
     993, 14578,   461,   815,   988,   962,   963, 12639, 12674,   145,
     145,   964, -1449,   967,   969,   970, -1449,  9209,   973,  9209,
    9209,   974,  1010, -1449,   977, -1449, -1449,   980, -1449, 14037,
    9209, 13873, 13908, -1449, -1449, -1449,   627,   270,   982, -1449,
   -1449, -1449, -1449, -1449,   983,   990, -1449, -1449, -1449, -1449,
   14037, -1449, 14037, 14037, -1449, -1449, -1449, -1449, 13998, -1449,
   -1449, -1449, -1449,   461, -1449, -1449, -1449,   271, -1449
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   120,     1,   298,     0,     0,     0,   606,   299,     0,
     598,   598,   598,    16,     0,     0,    15,     3,     0,    10,
       9,     8,     0,     7,   586,     6,    11,     5,     4,    13,
      12,    14,    92,    93,    91,   100,   102,    37,    53,    50,
      51,    39,    40,    41,     0,    42,    48,    38,   598,   598,
      22,    21,   586,   600,   599,   767,   757,   762,     0,   266,
      35,   107,   108,     0,     0,     0,   109,   111,   118,     0,
     106,    17,   624,   623,   213,   608,   625,   587,   588,     0,
       0,     0,     0,    43,     0,    49,     0,     0,    46,     0,
       0,   598,     0,    18,     0,     0,     0,   268,     0,     0,
     117,   112,     0,     0,     0,     0,     0,     0,   121,   215,
     214,   217,   212,   610,   609,     0,   627,   626,   630,   590,
     589,   592,    98,    99,    96,    97,    95,     0,     0,    94,
     103,    54,    52,    48,    45,    44,   601,   603,   605,     0,
     607,    19,    20,    23,   768,   758,   763,   267,    33,    36,
     116,     0,   113,   114,   115,   119,     0,   611,     0,   620,
     583,   521,    24,    25,     0,    87,    88,    85,    86,    84,
      83,    89,     0,    47,     0,   604,     0,     0,     0,    34,
     110,     0,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,     0,     0,   127,
     122,     0,   612,     0,   621,     0,   631,   584,     0,     0,
     523,     0,    26,    27,    28,     0,   101,     0,   769,   759,
     764,   181,   182,   179,   130,   131,   133,   132,   134,   135,
     136,   137,   163,   164,   161,   162,   154,   165,   166,   155,
     152,   153,   180,   174,     0,   178,   167,   168,   169,   170,
     141,   142,   143,   138,   139,   140,   151,     0,   157,   158,
     156,   149,   150,   145,   144,   146,   147,   148,   129,   128,
     173,     0,   159,   160,   521,   125,   245,   218,   594,   665,
     668,   671,   672,   666,   669,   667,   670,     0,   618,   628,
     591,   521,     0,   104,     0,     0,   573,   571,   593,    90,
       0,     0,     0,     0,     0,     0,   638,   658,   639,   674,
     640,   644,   645,   646,   647,   664,   651,   652,   653,   654,
     655,   656,   657,   659,   660,   661,   662,   727,   643,   650,
     663,   734,   741,   641,   648,   642,   649,     0,     0,     0,
       0,   673,   689,   692,   690,   691,   754,   602,   679,   551,
     557,   183,   184,   177,   172,   185,   175,   171,     0,   123,
     297,   545,     0,     0,   216,     0,   613,   615,     0,   622,
     535,   632,     0,     0,   105,     0,     0,     0,     0,   572,
       0,   695,   718,   721,     0,   724,   714,     0,   682,   728,
     735,   742,   748,   751,     0,     0,   704,   709,   703,     0,
     717,   713,   706,     0,     0,   708,   693,     0,   770,   760,
     765,   186,   176,     0,   295,   296,     0,   521,   124,   126,
     247,     0,     0,     0,    63,    64,    76,   427,   428,     0,
       0,     0,     0,   283,   421,   281,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   282,     0,     0,     0,     0,
       0,     0,     0,   664,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   510,
       0,     0,     0,   349,   351,   350,   352,   353,   354,   355,
       0,     0,    29,     0,   219,   224,     0,     0,     0,     0,
       0,     0,     0,   333,   334,   425,   424,     0,     0,     0,
       0,   240,   235,   232,   231,   233,   234,   265,   246,   226,
     504,   225,   422,     0,   495,    71,    72,    69,   238,    70,
     239,   241,   301,   230,   494,   493,   492,   120,   498,   423,
       0,   227,   497,   496,   468,   429,   469,   356,   430,     0,
     426,   772,   776,   773,   774,   775,     0,   595,     0,   594,
     619,   536,   585,   522,     0,     0,     0,   504,     0,   575,
     576,     0,   569,   570,   568,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   122,     0,   122,     0,   122,
       0,     0,     0,   700,   249,   711,   712,   705,   707,     0,
     710,   694,     0,     0,   756,   755,     0,   680,     0,   266,
     552,     0,   547,     0,     0,   558,     0,     0,     0,     0,
     633,   543,   562,   546,   815,   818,     0,     0,   271,   275,
     274,   280,     0,     0,   319,     0,     0,     0,   851,     0,
       0,   287,   284,     0,   328,     0,     0,     0,     0,   269,
       0,     0,     0,   310,   313,     0,   244,   316,     0,     0,
      57,     0,    78,   855,     0,   824,   833,     0,   821,     0,
       0,   292,   289,   457,   458,   334,   344,   120,   264,   262,
     263,     0,     0,     0,     0,     0,     0,     0,   793,     0,
       0,     0,   831,   858,     0,   255,     0,   258,     0,     0,
     860,   869,     0,   434,   433,   470,   432,   431,     0,     0,
     869,   328,   869,   335,   242,   243,     0,    74,   347,   222,
     502,     0,   229,   236,   237,   286,   291,   300,     0,   342,
       0,   228,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   459,   460,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   413,     0,   221,     0,   596,     0,   614,
     616,   629,     0,     0,   521,     0,   574,     0,   578,     0,
     582,   356,     0,     0,     0,   685,   698,     0,     0,   675,
     677,     0,     0,   125,     0,   125,     0,   125,   549,     0,
     555,     0,   676,     0,     0,   869,   702,   687,     0,   681,
     771,     0,   553,   761,     0,   559,   766,   544,     0,     0,
     561,     0,   560,     0,   563,     0,     0,     0,    79,     0,
       0,   807,     0,     0,     0,     0,   841,   844,   847,     0,
     869,   288,   285,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   869,   270,     0,    80,    81,     0,     0,
       0,     0,    55,    56,     0,     0,   869,     0,     0,   869,
       0,     0,   293,   290,   335,   342,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   266,     0,     0,     0,   827,
     787,   793,     0,   836,     0,     0,     0,   793,     0,     0,
       0,     0,     0,   796,   863,     0,   248,     0,    32,     0,
      30,     0,   870,     0,   245,     0,     0,   870,     0,     0,
       0,     0,   403,   400,   402,    60,     0,   120,     0,   416,
       0,   786,     0,     0,     0,   309,   308,     0,   122,   261,
       0,     0,   481,   480,     0,     0,   482,   486,     0,     0,
       0,   378,   387,   366,   388,   367,   390,   369,   389,   368,
     391,   370,   381,   360,   382,   361,   383,   362,   435,   436,
     448,   392,   371,   393,   372,   449,   446,   447,     0,   380,
     358,   475,     0,   466,     0,   499,   500,   501,   359,   437,
     438,   394,   373,   395,   374,   453,   454,   455,   384,   363,
     385,   364,   386,   365,   456,   379,   357,     0,     0,   451,
     452,   450,   444,   445,   440,   439,   441,   442,   443,     0,
       0,     0,   409,     0,     0,     0,     0,   419,     0,     0,
       0,     0,   529,   532,     0,     0,   577,   580,   356,   581,
     696,   719,   722,     0,   725,   715,     0,   683,   729,     0,
     736,     0,   743,     0,     0,   749,     0,   752,     0,   253,
     699,   250,   688,   548,   554,     0,   635,   636,   564,   567,
     566,     0,     0,     0,     0,   808,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   329,
     366,   367,   369,   368,   370,   360,   361,   362,   371,   372,
     358,   373,   374,   363,   364,   365,   357,   294,     0,     0,
       0,   471,     0,     0,     0,   472,     0,   503,     0,     0,
       0,     0,     0,     0,   342,   122,   505,   506,   507,   508,
     509,     0,     0,   794,     0,     0,     0,     0,   328,   793,
       0,     0,     0,     0,   802,   803,     0,   810,     0,     0,
     800,     0,   839,     0,     0,     0,     0,   798,   840,     0,
     830,   795,   859,     0,     0,     0,     0,   861,   862,     0,
       0,     0,   838,   461,     0,     0,     0,   122,     0,    58,
      59,     0,    73,    65,   342,     0,     0,   418,   417,   302,
       0,     0,     0,     0,     0,     0,   340,     0,   125,   477,
       0,   483,     0,   377,   375,   376,     0,     0,   464,     0,
       0,   487,   491,     0,     0,   467,     0,     0,     0,     0,
     410,     0,   414,   462,   420,   597,   617,   121,   530,   531,
     532,   533,   524,   537,   342,   579,   697,   720,   723,   686,
     726,   716,   678,   684,   730,   732,   737,   739,   744,   746,
     550,   750,   556,   753,     0,     0,   634,     0,     0,   816,
     819,     0,   272,     0,     0,   277,   278,   276,     0,   322,
       0,     0,   325,   320,     0,     0,     0,   852,   850,     0,
       0,   864,    82,   311,   314,   317,   856,   854,   825,   834,
     832,   822,     0,   125,     0,     0,     0,   778,   777,   793,
       0,   828,     0,     0,   788,   809,   801,   829,   837,   799,
       0,   793,     0,   805,   806,   813,   797,     0,   256,   259,
      31,     0,   220,     0,     0,     0,     0,     0,   125,    61,
       0,    66,     0,   122,     0,     0,     0,     0,   571,   338,
     339,   337,   336,     0,     0,     0,     0,     0,     0,     0,
     398,     0,     0,   488,     0,   476,     0,   465,     0,   411,
       0,     0,   463,   415,   542,   527,   524,   525,   526,   529,
       0,   733,   740,   747,   254,   249,   637,   565,     0,     0,
      77,   273,   279,     0,     0,     0,     0,   321,   842,   845,
     848,     0,     0,   869,     0,     0,     0,     0,   807,     0,
       0,     0,   223,     0,   511,     0,     0,     0,     0,   811,
       0,     0,     0,     0,     0,   804,     0,     0,   251,     0,
       0,     0,   401,   520,   404,     0,    60,     0,    75,   125,
     396,     0,   303,   571,     0,     0,   341,   343,     0,   330,
     346,     0,   519,     0,   517,   399,   514,     0,     0,     0,
     513,   412,     0,   528,   538,   527,     0,     0,   807,   807,
       0,     0,     0,     0,     0,     0,     0,   249,     0,     0,
     312,   315,   318,     0,   808,   826,   807,     0,   473,     0,
     345,   512,   867,     0,     0,   812,     0,   780,   779,   867,
     814,   867,   257,   249,   260,   867,   867,     0,     0,   407,
      62,   283,     0,    67,    71,    72,    69,    70,    68,     0,
       0,     0,     0,     0,     0,     0,   331,   478,   484,     0,
     518,   516,   515,     0,   540,   534,   701,     0,     0,     0,
     323,     0,   326,   807,   807,     0,     0,   869,   865,   869,
       0,   785,     0,   474,     0,     0,   782,   781,     0,     0,
       0,   869,     0,     0,   867,   405,     0,     0,     0,   289,
     348,   397,     0,     0,     0,     0,   332,   479,   485,   489,
       0,     0,     0,   791,   869,     0,     0,     0,     0,     0,
       0,     0,   853,     0,     0,     0,   823,     0,     0,     0,
       0,     0,     0,   870,     0,   875,   871,     0,   408,   290,
       0,     0,     0,   307,   490,   539,     0,     0,   870,   792,
     817,   820,   324,   327,     0,     0,   849,   866,   857,   835,
     868,   873,   784,   783,   874,   876,   252,   872,     0,   306,
     305,   541,   789,     0,   843,   846,   304,     0,   790
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1449, -1449, -1449, -1449, -1449, -1449,   502,  1119, -1449, -1449,
   -1449,  1201, -1449, -1449, -1449,   725,  1160, -1449,  1072, -1449,
   -1449,  1126, -1449, -1449, -1449,  -246, -1449, -1449, -1449,  -244,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,   996,
   -1449, -1449,   -47,   -59, -1449, -1449, -1449,   396,   433,  -524,
    -566,  -793, -1449, -1449, -1449, -1445, -1449, -1449,  -209,  -349,
   -1449,   283, -1449, -1337, -1449,  -409,   968, -1449, -1449, -1449,
   -1449,  -404,   -14, -1449, -1449, -1449, -1449, -1449,  -231,  -226,
    -225, -1449,  -224, -1449, -1449, -1449,  1233, -1449,   272, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449,  -432,  -196,   317,  -135, -1449,  -887,  -436, -1449,
    -512, -1449, -1449,  -368,  4334, -1449, -1449, -1449, -1448, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,   719, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449,  -152,  -157,  -245,  -158,
     -18, -1449, -1449, -1449, -1449, -1449,   816, -1449,  -573, -1449,
   -1449,  -564, -1449, -1449,  -618,  -242,  -568, -1320, -1449,  -342,
   -1449, -1449,  1193, -1449, -1449, -1449,   687,   788,   170, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,  -586,
    -197, -1449,   808, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449,  -383, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,  -215,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449,   812,  -659,  -317,  -823,  -680,
   -1449, -1203,  -889, -1449, -1449, -1449, -1148,  -195, -1354, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,    71,  -467,
   -1449, -1449, -1449,   562, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449,  -641, -1449, -1400,  -696,
   -1449
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    16,   143,    52,    17,   164,   170,   698,   505,
     149,   506,    99,    19,    20,    45,    46,    47,    88,    21,
      39,    40,   507,   508,  1211,  1212,   509,  1362,  1457,   510,
     511,   956,   512,   627,   513,   514,   515,   516,  1142,   171,
     172,    35,    36,    37,   220,    66,    67,    68,    69,    22,
     285,   374,   210,    23,   111,   211,   112,   156,   676,   981,
     518,   375,   519,   824,  1524,  1088,   567,   935,  1447,   937,
    1448,   521,   522,   523,   629,   850,  1411,   524,   525,   526,
     527,   528,   529,   530,   531,   426,   532,   728,  1220,   965,
     533,   534,   888,  1424,   889,  1425,   891,  1426,   535,   855,
    1417,   536,   710,  1470,   537,  1226,  1227,   968,   678,   538,
     785,   957,   539,   643,   982,   541,   542,   543,   954,   544,
    1206,  1528,  1207,  1586,   545,  1055,  1393,   546,   711,  1376,
    1597,  1378,  1598,  1477,  1634,   548,   370,  1399,  1484,  1260,
    1262,  1064,   561,   794,  1553,  1601,   371,   372,   611,   819,
     419,   616,   821,   420,  1166,   621,   575,   390,   307,   308,
     217,   301,    78,   121,    25,   161,   376,    89,    90,   174,
      91,    26,    49,   115,   158,    27,   288,   558,   559,  1060,
     379,   215,   216,    76,   118,   381,    28,   159,   299,   622,
     549,   297,   353,   354,   811,   418,   355,   583,  1273,   804,
     416,   356,   576,  1266,   823,   581,  1271,   577,  1267,   578,
    1268,   580,  1270,   584,  1274,   585,  1401,   586,  1276,   587,
    1402,   588,  1278,   589,  1403,   590,  1281,   591,  1283,   612,
      29,    95,   177,   359,   613,    30,    96,   178,   360,   617,
      31,    94,   176,   358,   608,   550,  1603,  1572,   962,   921,
    1604,  1605,   922,   934,  1188,  1182,  1177,  1345,  1108,   551,
     846,  1408,   847,  1409,   900,  1430,   897,  1428,   923,   700,
     552,   898,  1429,   924,   553,  1114,  1494,  1115,  1495,  1116,
    1496,   859,  1421,   895,  1427,   694,   701,   554,  1575,   943,
     555
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,    70,   287,   799,   845,   571,   883,   540,   719,   221,
     927,   642,   357,   729,   948,   693,   949,   818,  1155,   813,
    1079,   815,  1081,   817,  1083,  1335,   517,   820,  1107,   920,
     352,   920,  1173,   130,   605,   679,   680,   793,  1185,   641,
     562,   849,   635,  -120,   151,  1103,   597,  1162,  1465,    70,
      70,    70,    84,   913,  1049,  1050,  1183,   595,  1189,   913,
    1196,  1200,   961,   671,   122,   123,  1354,   218,  1487,   752,
     712,    53,   755,   756,  1505,    98,   286,    54,     2,    58,
    1585,   732,   733,  1051,     3,  1397,   840,    85,  1174,  1175,
      70,    70,    70,    70,   517,  1052,   286,   688,   690,    32,
      33,   912,  1073,   925,   863,   929,    59,     4,  1600,     5,
     661,     6,   417,   941,   755,   756,  1176,     7,   517,  1579,
     730,  1580,   945,   831,   219,  1582,  1583,     8,    50,  1091,
     104,   105,   106,     9,  1557,  1558,  1053,  1054,  1628,   636,
     637,   431,   432,  1542,    61,  1398,   165,   166,   842,   382,
     632,   417,  1570,   905,   783,   784,  1636,    10,   429,   302,
    1566,   442,   825,   914,  1118,  1464,    80,   447,   389,   427,
     731,   800,   303,    62,    51,  1293,   352,   744,   745,    11,
      12,  1172,  1439,    38,  1627,   752,  1581,  1140,   755,   756,
     757,   352,   961,   180,   138,   758,   783,   784,   424,    48,
    1149,   150,   428,  1152,   461,   462,   598,   860,   862,  1609,
    1610,   124,   841,   351,   841,   791,   125,   596,   126,    92,
     352,   127,   352,   841,   599,   841,   517,   841,   841,    86,
     600,   638,   896,   841,  1468,   899,   861,   645,   464,   465,
      87,   425,   732,   733,  1222,    63,   792,   305,   368,    34,
     639,   662,    58,   218,  1223,   840,   305,   384,    13,  1076,
     720,   139,   128,   882,   963,   306,   902,  1322,   593,   556,
     783,   784,   286,  1077,   306,   572,    71,    14,    58,    59,
    1330,   557,  1294,   352,   352,   573,   901,   594,    15,   960,
    1515,   517,   286,   167,   969,   480,   481,   482,   168,  1215,
     169,   914,    77,   127,   721,    59,  1224,   107,   369,  1179,
     219,  1225,  1180,  1293,    64,   493,   841,   842,   964,   843,
     368,   517,   844,    65,   787,   218,    58,  1363,   503,   675,
     628,   788,   108,    79,   840,   840,   405,   574,   744,   745,
    1181,    80,   840,   225,  1165,  1441,   752,   501,   754,   755,
     756,   757,  1165,    59,  1058,  1606,   758,  1602,    98,   351,
    1201,   802,   803,   805,   560,   807,   808,  1615,   840,   812,
     226,   814,  1395,   816,   351,   840,  1084,  1400,  1513,   352,
     352,   352,   219,   352,   352,  1619,   966,   352,    80,   352,
    1085,   352,  1261,   352,  1216,  1067,   842,   842,    41,    42,
      43,    58,  1228,   351,   842,   351,  1644,  1645,    72,    73,
     950,    74,  1065,   383,    81,   951,   619,   848,    58,  1106,
     863,  1217,  1086,   107,   863,   351,   633,   950,    59,    44,
     842,   783,   784,  1214,   620,  1375,  1087,   842,    82,    75,
    1438,  1099,   886,  1366,   863,    59,   961,  1254,  1257,   352,
     517,  1295,  1444,   952,   863,   863,  1171,  1171,    97,   100,
     101,   102,   212,  1101,  1102,  1192,   351,   351,  1171,  -731,
    1368,  1475,   352,   213,  -731,  1197,  1117,  1163,   646,    53,
    1288,  1481,  1511,  1662,  1668,    54,   712,   936,   104,  1329,
     106,  1366,  -731,  1364,   712,  1373,   517,   647,   732,   733,
     152,   153,   154,   155,  1341,  1143,  1144,   955,  1146,  1171,
     920,  1280,  1148,  1334,  1150,  1151,  1367,  1153,  -738,  1284,
    1374,   663,  1282,  -738,  -745,   920,  1193,   289,  1194,  -745,
    1433,   290,  -406,  1285,  1540,   973,   977,  -406,   666,    98,
     664,  -738,  1233,  1234,  1235,   291,   292,  -745,   606,   853,
     293,   294,   295,   296,   103,  -406,   131,   667,   406,  1300,
     607,  1017,   351,   351,   351,  1455,   351,   351,   854,  1284,
     351,    87,   351,  1301,   351,   109,   351,  1284,   614,   133,
     417,   110,   136,  1348,  1070,   407,   408,  1413,  1208,  1323,
     615,  1349,   742,   743,   744,   745,   406,  1012,   352,  1209,
    1210,  1414,   752,   144,   754,   755,   756,   757,  1089,  1013,
    1244,   137,   758,  1249,   760,   761,  1093,   914,   852,  1094,
     140,  1332,  1245,   407,   408,  1250,   352,   417,  1098,  1293,
     113,  1071,   351,  1333,   352,  1104,   114,   352,   116,   119,
    1113,  1358,  1063,   145,   117,   120,   352,   146,   409,   352,
     352,   572,   410,   352,   387,   351,   147,   388,   352,   141,
     389,   573,   352,   148,  1342,   142,  1539,  1343,   157,  1423,
    1344,  1291,  1137,   778,   779,   780,   781,   782,  1258,  1436,
     104,   105,   106,  1443,  1259,   863,   409,   783,   784,   863,
     410,   352,   352,   160,   352,    85,  1446,   175,   352,   953,
     352,   352,   863,   352,  1449,  1450,  1461,   104,   417,   227,
     863,   863,  1074,   574,   411,  1442,  1541,  1527,   412,   389,
    1089,   413,  1089,   863,   417,   417,   179,  1499,  1075,  1078,
    1325,  1304,  1305,  1306,   282,   417,   414,   417,   352,  1080,
     352,  1082,   415,   417,  1138,   417,   417,  1292,   214,  1303,
    1548,  1340,   411,   162,   163,  1309,   412,  1347,  1139,   413,
    1431,   417,   503,   675,  1351,  1599,   503,   675,  1353,   162,
     163,   938,   939,   228,   414,    41,    42,    43,    70,   229,
     415,   351,   283,  1186,  1179,   284,  1187,   677,   677,   677,
     222,   223,   224,   931,   932,   933,   298,  1459,    55,    56,
      57,   222,   223,   230,  1381,   286,   732,   733,   304,   351,
    1432,   134,   135,   300,   361,   362,  1390,   351,   363,  1514,
     351,   364,   365,  1357,   366,   373,   367,   377,   385,   351,
     378,   540,   351,   351,   386,   380,   351,   718,   391,   422,
    1242,   351,   732,   733,   392,   351,   393,   394,  1571,   397,
     517,   395,   396,   398,   399,   400,   401,  1567,  1269,   402,
     403,   417,   421,   563,   718,   569,   404,   423,   624,   570,
     582,  1613,   625,  1614,   351,   351,   352,   351,   603,   634,
     652,   351,   650,   351,   351,  1624,   351,   352,   653,   654,
     655,   656,  1435,   658,   681,   682,   657,  1544,   665,   668,
     669,   683,   744,   745,   684,   685,  1571,   697,  1639,   702,
     752,   686,   754,   755,   756,   757,   714,   352,   352,   352,
     758,   351,  1451,   351,   713,   715,   717,   722,   727,   723,
     724,   725,   732,   733,   726,   786,    15,   789,   744,   745,
     661,   352,   797,  1637,   809,   795,   752,   810,   754,   755,
     756,   757,   695,   614,   826,   829,   758,   830,   834,   833,
    1620,   718,   832,   835,   839,   838,   836,   851,   730,   856,
     857,   881,   893,   825,  1593,  1594,   863,   858,   887,  1521,
     904,   942,  1525,   780,   781,   782,   892,   944,   946,   718,
     947,   967,  1008,  1471,  1062,   783,   784,  1066,  1096,  1359,
    1072,  1171,  1090,  1097,  1667,  1111,  1164,  1178,  1204,   352,
    1191,  1198,  1203,  1202,  1219,  1205,  1229,   778,   779,   780,
     781,   782,  1252,  1218,  1630,  1230,   742,   743,   744,   745,
     746,   783,   784,   749,   750,   751,   752,  1231,   754,   755,
     756,   757,  1263,  1256,  1261,  1272,   758,   677,   760,   761,
    1275,   406,  1232,  1236,  1237,  1247,  1287,  1248,  1298,   351,
    1277,  1279,  1296,  1297,  1308,   825,  1310,  1369,  1311,  1404,
     351,  1569,  1317,  1320,  1370,  1371,  1372,  1324,   407,   408,
    1336,  1543,  1410,  1339,  1383,  1346,  1406,   352,  1384,  1394,
    1386,   825,  1391,  1405,   547,  1412,  1415,  1416,  1440,   642,
     351,   351,   351,   406,   568,   776,   777,   778,   779,   780,
     781,   782,  1445,   579,  1458,  1460,  1462,  1483,   718,  1490,
    1492,   783,   784,   592,   351,  1463,  1467,  1504,  1431,  1574,
     407,   408,  1488,   602,  1529,  1489,  1497,  1498,  1559,  1503,
    1617,   409,   618,  1506,   572,   410,  1556,  1507,  1523,  1561,
    1563,  1456,   626,  1564,   573,   862,  1568,  1565,  1592,  1591,
    1612,  1618,   644,  1473,  1621,  1616,  1469,   649,  1622,   651,
    1623,  1625,  1626,  1635,  1638,  1640,  1641,  1646,   660,   352,
    1647,   352,  1648,  1649,  1651,  1654,   672,   673,   674,  1655,
    1656,  1657,   351,   409,  1663,   406,  1664,   410,   129,   798,
     940,   692,    18,  1665,    83,   173,   574,   411,   132,   696,
    1530,   412,   692,  1533,   413,   703,   704,   705,   706,   707,
     718,   309,   407,   408,  1469,  1061,  1534,  1199,   716,   414,
     677,  1535,  1536,  1537,    24,   415,  1221,  1510,  1466,  1485,
    1554,  1486,  1396,   623,  1555,    93,   790,   630,  1611,  1520,
    1338,   631,   928,     0,     0,     0,     0,     0,     0,   411,
       0,     0,  1549,   412,     0,  1154,   413,   352,     0,     0,
     351,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     352,   414,     0,     0,     0,   409,     0,   415,     0,   410,
       0,     0,     0,     0,   801,   677,   677,   677,     0,     0,
     718,     0,   718,     0,   718,     0,   718,     0,   718,     0,
     718,     0,   718,     0,   718,     0,     0,     0,     0,   718,
       0,   718,   828,     0,     0,     0,     0,   718,     0,     0,
    1590,     0,     0,     0,     0,     0,     0,     0,     0,   718,
       0,   718,     0,   520,   352,     0,   718,     0,   718,     0,
     718,   411,     0,   718,     0,   412,     0,  1264,   413,   644,
       0,     0,   351,     0,   351,     0,   692,   884,     0,   885,
       0,     0,     0,   414,   890,     0,     0,     0,     0,   415,
     894,     0,     0,     0,     0,   718,     0,     0,     0,   903,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     906,   907,   908,   909,   910,   911,     0,   919,     0,   919,
       0,   520,     0,     0,     0,     0,     0,   718,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1661,     0,     0,
       0,     0,     0,     0,     0,   520,     0,     0,     0,     0,
       0,     0,     0,     0,   732,   733,     0,     0,     0,     0,
     351,     0,     0,   983,   985,   987,   989,   991,   993,   995,
     997,   998,   999,   351,     0,  1000,  1002,  1004,  1005,  1006,
    1007,     0,  1010,  1011,     0,  1014,  1015,  1016,  1018,  1019,
    1020,  1022,  1024,  1025,  1026,  1027,  1029,  1031,  1033,  1034,
    1036,  1038,  1039,  1040,  1041,  1042,  1043,  1044,  1045,  1046,
    1047,  1048,     0,  1056,     0,     0,     0,  1059,     0,     0,
       0,     0,     0,     0,   953,     0,     0,     0,  1068,     0,
       0,     0,     0,     0,     0,     0,     0,   351,     0,     0,
       0,     0,     0,     0,     0,   406,     0,     0,   742,   743,
     744,   745,   746,   520,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,     0,     0,     0,  1095,   758,     0,
     760,   761,   407,   408,  1100,     0,   764,   765,   766,     0,
       0,  1110,   953,  1112,     0,     0,     0,     0,     0,     0,
       0,     0,  1119,  1120,  1121,  1122,  1123,  1124,  1125,  1126,
    1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,  1135,  1136,
       0,     0,     0,     0,     0,     0,     0,     0,   520,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,     0,   409,     0,     0,     0,   410,
       0,     0,     0,   783,   784,   703,  1168,     0,   520,   718,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1190,
       0,   692,     0,     0,   732,   733,     0,     0,  1195,     0,
       0,   692,     0,     0,     0,     0,  1119,     0,     0,     0,
       0,     0,     0,     0,     0,  1213,     0,     0,     0,     0,
       0,   -68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   411,   732,   733,     0,   412,     0,  1289,   413,     0,
       0,     0,   520,   520,   520,   520,   520,   520,   520,   520,
       0,     0,     0,   414,     0,   520,   520,     0,     0,   415,
       0,   520,     0,     0,     0,     0,     0,     0,     0,     0,
     520,   520,     0,     0,     0,   520,   520,   520,     0,   520,
       0,     0,     0,     0,     0,     0,     0,     0,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,     0,     0,     0,   520,   758,  1246,
     760,   761,     0,  1251,     0,     0,   764,     0,   734,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,   750,   751,   752,   753,   754,   755,
     756,   757,     0,     0,     0,     0,   758,   759,   760,   761,
     762,   763,     0,   520,   764,   765,   766,   767,   768,   769,
     770,     0,     0,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,   406,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   771,
     772,  1312,   773,   774,   775,   776,   777,   778,   779,   780,
     781,   782,     0,   407,   408,     0,     0,     0,     0,     0,
       0,   783,   784,  1326,  1327,  1328,   503,   675,     0,  1331,
       0,     0,     0,     0,     0,     0,     0,  1337,   919,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   687,
    1361,     0,     0,     0,     0,   310,     0,     0,   547,   732,
     733,   311,     0,  1355,  1356,     0,     0,   312,     0,     0,
    1360,     0,     0,     0,     0,  1119,   409,   313,     0,     0,
     410,     0,     0,     0,     0,   314,     0,     0,     0,  1377,
       0,  1379,     0,     0,     0,     0,  1382,     0,     0,     0,
     315,     0,  1385,   732,   733,     0,  1388,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
       0,     0,   411,     0,     0,     0,   412,  1407,  1290,   413,
       0,     0,     0,   742,   743,   744,   745,   746,     0,     0,
     749,   750,   751,   752,   414,   754,   755,   756,   757,   692,
     415,     0,     0,   758,     0,   760,   761,     0,    58,     0,
       0,   764,   765,   766,     0,     0,     0,   770,     0,     0,
       0,   349,     0,     0,     0,     0,     0,   742,   743,   744,
     745,   746,     0,     0,   749,    59,     0,   752,     0,   754,
     755,   756,   757,     0,     0,     0,     0,   758,     0,   760,
     761,     0,     0,     0,     0,     0,     0,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,     0,
       0,     0,     0,  1478,     0,  1479,     0,     0,   783,   784,
    1482,   350,   732,   733,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1491,     0,  1493,   776,   777,   778,   779,
     780,   781,   782,  1500,  1501,  1502,     0,     0,     0,     0,
    1509,     0,   783,   784,     0,  1512,     0,     0,     0,  1516,
    1517,  1518,  1519,     0,     0,   692,  1522,   520,   692,  1526,
       0,     0,     0,     0,     0,     0,  1538,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1546,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   742,   743,   744,   745,
     746,     0,     0,   749,   750,   751,   752,   692,   754,   755,
     756,   757,     0,     0,     0,     0,   758,     0,   760,   761,
       0,     0,  1576,  1577,   764,     0,   766,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1584,     0,     0,     0,
       0,  1589,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1595,  1596,     0,     0,     0,   406,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1607,     0,
    1608,     0,   773,   774,   775,   776,   777,   778,   779,   780,
     781,   782,     0,     0,     0,   407,   408,     0,     0,     0,
       0,   783,   784,     0,     0,     0,     0,  1629,     0,     0,
       0,     0,  1631,  1632,     0,     0,     0,   430,     0,     0,
     431,   432,     3,     0,   433,   434,   435,     0,   436,     0,
     437,   438,   439,   440,   441,     0,  1650,     0,  1652,  1653,
     442,   443,   444,   445,   446,     0,   447,     0,     0,  1658,
       0,     0,     0,   448,   449,     0,     0,   450,   409,   451,
     452,     0,   410,   453,     0,     8,   454,   455,     0,   456,
     457,     0,     0,   458,   459,     0,     0,     0,     0,     0,
     460,     0,     0,   461,   462,     0,   316,   317,   318,     0,
     320,   321,   322,   323,   324,   463,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,     0,   338,   339,
     340,     0,     0,   343,   344,   345,   346,   464,   465,   466,
     467,     0,     0,     0,   411,     0,     0,     0,   412,     0,
    1307,   413,   468,   469,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   414,   470,   471,   472,
       0,     0,   415,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   473,   474,   475,   476,   477,
       0,   478,     0,   479,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,    59,   490,   491,   492,     0,     0,
       0,     0,     0,     0,   493,   494,   495,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   496,   497,   498,     0,    14,     0,     0,   499,   500,
       0,     0,     0,     0,     0,     0,   501,     0,   502,   430,
     503,   504,   431,   432,     3,     0,   433,   434,   435,     0,
     436,     0,   437,   438,   439,   440,   441,     0,     0,     0,
       0,     0,   442,   443,   444,   445,   446,     0,   447,     0,
       0,     0,     0,     0,     0,   448,   449,     0,     0,   450,
       0,   451,   452,     0,     0,   453,     0,     8,   454,   455,
       0,   456,   457,     0,     0,   458,   459,     0,     0,     0,
       0,     0,   460,     0,     0,   461,   462,     0,   316,   317,
     318,     0,   320,   321,   322,   323,   324,   463,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,     0,
     338,   339,   340,     0,     0,   343,   344,   345,   346,   464,
     465,   466,   467,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   468,   469,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   470,
     471,   472,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   473,   474,   475,
     476,   477,   406,   478,     0,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,    59,   490,   491,   492,
       0,     0,     0,     0,     0,     0,   493,  1352,   495,   407,
     408,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   496,   497,   498,     0,    14,     0,     0,
     499,   500,     0,     0,   431,   432,     0,     0,   501,     0,
     502,     0,   503,   504,   437,   438,   439,   440,   441,     0,
       0,     0,     0,     0,   442,     0,   444,     0,     0,     0,
     447,     0,   406,     0,     0,     0,     0,     0,   449,     0,
       0,     0,   409,     0,   452,     0,   410,   453,     0,     0,
     454,     0,   913,     0,   457,     0,     0,     0,     0,   407,
     408,     0,     0,     0,   564,     0,     0,   461,   462,     0,
     316,   317,   318,     0,   320,   321,   322,   323,   324,   463,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,     0,   338,   339,   340,     0,     0,   343,   344,   345,
     346,   464,   465,   565,     0,     0,     0,     0,   411,     0,
       0,     0,   412,     0,  1313,   413,   468,   469,     0,     0,
       0,     0,   409,     0,     0,     0,   410,     0,     0,     0,
     414,     0,     0,     0,     0,     0,   415,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   473,
     474,   475,   476,   477,   406,   478,   914,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   915,   566,
     491,   492,     0,     0,     0,     0,     0,     0,   493,     0,
       0,   407,   408,     0,     0,     0,     0,     0,   411,     0,
       0,     0,   412,     0,     0,   916,   497,   498,     0,    14,
       0,     0,   499,   500,     0,     0,     0,   431,   432,     0,
     917,     0,   918,     0,   503,   504,   415,   437,   438,   439,
     440,   441,     0,     0,     0,     0,     0,   442,     0,   444,
       0,     0,     0,   447,     0,   406,     0,     0,     0,     0,
       0,   449,     0,     0,   409,     0,     0,   452,   410,     0,
     453,     0,     0,   454,     0,     0,     0,   457,     0,     0,
       0,     0,   407,   408,     0,     0,     0,   564,     0,     0,
     461,   462,     0,   316,   317,   318,     0,   320,   321,   322,
     323,   324,   463,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,     0,   338,   339,   340,     0,     0,
     343,   344,   345,   346,   464,   465,   565,     0,     0,     0,
     411,     0,     0,     0,   412,     0,  1314,   413,     0,   468,
     469,     0,     0,     0,     0,   409,     0,     0,     0,   410,
       0,     0,   414,     0,     0,     0,     0,     0,   415,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   473,   474,   475,   476,   477,   406,   478,   914,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,   915,   566,   491,   492,     0,     0,     0,     0,     0,
       0,   493,     0,     0,   407,   408,     0,     0,     0,     0,
       0,   411,     0,     0,     0,   412,     0,     0,   916,   497,
     498,     0,    14,     0,     0,   499,   500,     0,     0,     0,
     431,   432,     0,   917,     0,   926,     0,   503,   504,   415,
     437,   438,   439,   440,   441,     0,     0,     0,     0,     0,
     442,     0,   444,     0,     0,     0,   447,     0,   597,     0,
       0,     0,     0,     0,   449,     0,     0,   409,     0,     0,
     452,   410,     0,   453,     0,     0,   454,     0,     0,     0,
     457,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     564,     0,     0,   461,   462,     0,   316,   317,   318,     0,
     320,   321,   322,   323,   324,   463,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,     0,   338,   339,
     340,     0,     0,   343,   344,   345,   346,   464,   465,   565,
       0,     0,     0,   411,     0,     0,     0,   412,     0,  1315,
     413,     0,   468,   469,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   414,     0,     0,     0,     0,
       0,   415,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   473,   474,   475,   476,   477,
     406,   478,     0,   479,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,    59,   566,   491,   492,     0,     0,
       0,     0,     0,     0,   493,     0,     0,   407,   408,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   598,     0,
       0,   496,   497,   498,     0,    14,     0,     0,   499,   500,
       0,     0,     0,   431,   432,     0,  1167,     0,   502,     0,
     503,   504,   600,   437,   438,   439,   440,   441,     0,     0,
       0,     0,     0,   442,     0,   444,     0,     0,     0,   447,
       0,     0,     0,     0,     0,     0,     0,   449,     0,     0,
     409,     0,     0,   452,   410,     0,   453,     0,     0,   454,
       0,     0,     0,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   564,     0,     0,   461,   462,     0,   316,
     317,   318,     0,   320,   321,   322,   323,   324,   463,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
       0,   338,   339,   340,     0,     0,   343,   344,   345,   346,
     464,   465,   466,     0,     0,     0,   411,     0,     0,     0,
     412,     0,  1316,   413,     0,   468,   469,     0,     0,     0,
       0,     0,     0,     0,   640,     0,     0,     0,   414,     0,
     470,   471,   472,     0,   415,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   473,   474,
     475,   476,   477,   406,   478,     0,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,    59,   566,   491,
     492,     0,     0,     0,     0,     0,     0,   493,     0,     0,
     407,   408,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   496,   497,   498,     0,    14,     0,
       0,   499,   500,     0,     0,   431,   432,     0,     0,   501,
       0,   502,     0,   503,   504,   437,   438,   439,   440,   441,
       0,     0,     0,     0,     0,   442,     0,   444,     0,     0,
       0,   447,     0,     0,     0,     0,     0,     0,     0,   449,
       0,     0,     0,   409,     0,   452,     0,   410,   453,     0,
       0,   454,     0,     0,     0,   457,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   564,     0,     0,   461,   462,
       0,   316,   317,   318,     0,   320,   321,   322,   323,   324,
     463,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,     0,   338,   339,   340,     0,     0,   343,   344,
     345,   346,   464,   465,   466,     0,     0,     0,     0,   411,
       0,     0,     0,   412,     0,  1318,   413,   468,   469,     0,
       0,     0,     0,     0,     0,     0,   670,     0,     0,     0,
       0,   414,   470,   471,   472,     0,     0,   415,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     473,   474,   475,   476,   477,   406,   478,     0,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,    59,
     566,   491,   492,     0,     0,     0,     0,     0,     0,   493,
       0,     0,   407,   408,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   496,   497,   498,     0,
      14,     0,     0,   499,   500,     0,     0,   431,   432,     0,
       0,   501,     0,   502,     0,   503,   504,   437,   438,   439,
     440,   441,     0,     0,     0,     0,     0,   442,  1531,   444,
     445,     0,     0,   447,     0,     0,     0,     0,     0,     0,
       0,   449,     0,     0,     0,   409,     0,   452,     0,   410,
     453,     0,     0,   454,   455,     0,     0,   457,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   564,     0,     0,
     461,   462,     0,   316,   317,   318,     0,   320,   321,   322,
     323,   324,   463,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,     0,   338,   339,   340,     0,     0,
     343,   344,   345,   346,   464,   465,   565,  1532,     0,     0,
       0,   411,     0,     0,     0,   412,     0,  1319,   413,   468,
     469,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   414,     0,     0,     0,     0,     0,   415,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   473,   474,   475,   476,   477,   406,   478,     0,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,    59,   566,   491,   492,     0,     0,     0,     0,     0,
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
       0,     0,     0,   411,     0,     0,     0,   412,     0,  1321,
     413,   468,   469,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   414,   470,   471,   472,     0,
       0,   415,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   473,   474,   475,   476,   477,   406,
     478,     0,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,    59,   566,   491,   492,     0,     0,     0,
       0,     0,     0,   493,     0,     0,   407,   408,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   498,     0,    14,     0,     0,   499,   500,     0,
       0,   431,   432,     0,     0,   501,     0,   502,     0,   503,
     504,   437,   438,   439,   440,   441,     0,     0,     0,     0,
       0,   442,     0,   444,     0,     0,   406,   447,     0,     0,
       0,     0,     0,     0,     0,   449,     0,     0,     0,   409,
       0,   452,     0,   410,   453,     0,     0,   454,     0,     0,
       0,   457,     0,   407,   408,     0,     0,     0,     0,     0,
       0,   564,     0,     0,   461,   462,     0,   316,   317,   318,
       0,   320,   321,   322,   323,   324,   463,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,     0,   338,
     339,   340,     0,     0,   343,   344,   345,   346,   464,   465,
     466,     0,     0,     0,     0,   411,     0,     0,     0,   412,
       0,  1418,   413,   468,   469,     0,   409,     0,     0,     0,
     410,     0,     0,     0,     0,     0,     0,   414,   978,   979,
     980,     0,     0,   415,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   473,   474,   475,   476,
     477,     0,   478,   406,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,    59,   566,   491,   492,     0,
       0,     0,     0,     0,     0,   493,     0,     0,     0,     0,
     407,   408,   411,     0,     0,     0,   412,     0,  1419,   413,
       0,     0,   496,   497,   498,     0,    14,     0,     0,   499,
     500,     0,     0,     0,   414,   431,   432,   501,     0,   502,
     415,   503,   504,   708,     0,   437,   438,   439,   440,   441,
       0,     0,     0,     0,     0,   442,     0,   444,     0,     0,
     406,   447,     0,     0,     0,     0,     0,     0,     0,   449,
       0,     0,     0,   409,     0,   452,     0,   410,   453,   709,
       0,   454,     0,     0,     0,   457,     0,   407,   408,     0,
       0,     0,     0,     0,     0,   564,     0,     0,   461,   462,
       0,   316,   317,   318,     0,   320,   321,   322,   323,   324,
     463,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,     0,   338,   339,   340,     0,     0,   343,   344,
     345,   346,   464,   465,   565,     0,     0,     0,     0,   411,
       0,     0,     0,   412,     0,  1420,   413,   468,   469,     0,
     409,     0,     0,     0,   410,     0,     0,     0,     0,     0,
       0,   414,     0,     0,     0,     0,     0,   415,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     473,   474,   475,   476,   477,     0,   478,   406,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,    59,
     566,   491,   492,     0,     0,     0,     0,     0,     0,   493,
       0,     0,     0,     0,   407,   408,   411,     0,     0,     0,
     412,     0,  1422,   413,     0,     0,   496,   497,   498,     0,
      14,     0,     0,   499,   500,     0,     0,     0,   414,   431,
     432,   501,   601,   502,   415,   503,   504,   708,     0,   437,
     438,   439,   440,   441,     0,     0,     0,     0,     0,   442,
       0,   444,     0,     0,   406,   447,     0,     0,     0,     0,
       0,     0,     0,   449,     0,     0,     0,   409,     0,   452,
       0,   410,   453,   709,     0,   454,     0,     0,     0,   457,
       0,   407,   408,     0,     0,     0,     0,     0,     0,   564,
       0,     0,   461,   462,     0,   316,   317,   318,     0,   320,
     321,   322,   323,   324,   463,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,     0,   338,   339,   340,
       0,     0,   343,   344,   345,   346,   464,   465,   565,     0,
       0,     0,     0,   411,     0,     0,     0,   412,     0,  1454,
     413,   468,   469,     0,   409,     0,     0,     0,   410,     0,
       0,     0,     0,     0,     0,   414,     0,     0,     0,     0,
       0,   415,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   473,   474,   475,   476,   477,     0,
     478,   914,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   915,   566,   491,   492,     0,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
     411,     0,     0,     0,   412,     0,  1547,   413,     0,     0,
     496,   497,   498,     0,    14,     0,     0,   499,   500,     0,
       0,     0,   414,   431,   432,   501,     0,   502,   415,   503,
     504,   708,     0,   437,   438,   439,   440,   441,     0,     0,
       0,     0,     0,   442,     0,   444,     0,     0,   406,   447,
       0,     0,     0,     0,     0,     0,     0,   449,     0,     0,
       0,     0,     0,   452,     0,     0,   453,   709,     0,   454,
       0,     0,     0,   457,     0,   407,   408,     0,     0,     0,
       0,     0,     0,   564,     0,     0,   461,   462,     0,   316,
     317,   318,     0,   320,   321,   322,   323,   324,   463,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
       0,   338,   339,   340,     0,     0,   343,   344,   345,   346,
     464,   465,   565,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,   469,     0,   409,     0,
       0,     0,   410,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   473,   474,
     475,   476,   477,     0,   478,     0,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,    59,   566,   491,
     492,     0,     0,     0,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,   411,     0,     0,     0,   412,     0,
       0,   413,     0,     0,   496,   497,   498,     0,    14,     0,
       0,   499,   500,     0,     0,     0,   414,   431,   432,   501,
     826,   502,   415,   503,   504,   708,     0,   437,   438,   439,
     440,   441,     0,     0,     0,     0,     0,   442,     0,   444,
       0,     0,     0,   447,     0,     0,     0,     0,     0,     0,
       0,   449,     0,     0,     0,     0,     0,   452,     0,     0,
     453,   709,     0,   454,     0,     0,     0,   457,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   564,     0,     0,
     461,   462,     0,   316,   317,   318,     0,   320,   321,   322,
     323,   324,   463,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,     0,   338,   339,   340,     0,     0,
     343,   344,   345,   346,   464,   465,   565,     0,     0,   984,
     986,   988,   990,   992,   994,   996,     0,     0,     0,   468,
     469,  1001,  1003,     0,     0,     0,     0,  1009,     0,     0,
       0,     0,     0,     0,     0,     0,  1021,  1023,     0,     0,
       0,  1028,  1030,  1032,    58,  1035,     0,     0,     0,     0,
       0,     0,   473,   474,   475,   476,   477,     0,   478,     0,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,    59,   566,   491,   492,     0,     0,     0,     0,     0,
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
       0,     0,   343,   344,   345,   346,   464,   465,   565,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   468,   469,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   473,   474,   475,   476,   477,     0,
     478,   914,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   915,   566,   491,   492,     0,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   498,     0,    14,     0,     0,   499,   500,     0,
       0,   431,   432,     0,     0,   958,     0,   502,   959,   503,
     504,   437,   438,   439,   440,   441,     0,     0,     0,     0,
       0,   442,     0,   444,     0,     0,     0,   447,     0,     0,
       0,     0,     0,     0,     0,   449,     0,     0,     0,     0,
       0,   452,     0,     0,   453,     0,     0,   454,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   564,     0,     0,   461,   462,  1105,   316,   317,   318,
       0,   320,   321,   322,   323,   324,   463,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,     0,   338,
     339,   340,     0,     0,   343,   344,   345,   346,   464,   465,
     565,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   473,   474,   475,   476,
     477,     0,   478,   914,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   915,   566,   491,   492,     0,
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
     464,   465,   565,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,   469,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   473,   474,
     475,   476,   477,     0,   478,     0,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,    59,   566,   491,
     492,     0,     0,     0,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   496,   497,   498,     0,    14,     0,
       0,   499,   500,     0,     0,   431,   432,     0,     0,   501,
     601,   502,     0,   503,   504,   437,   438,   439,   440,   441,
       0,     0,     0,     0,     0,   442,     0,   444,     0,     0,
       0,   447,     0,     0,     0,     0,     0,     0,     0,   449,
       0,     0,     0,     0,     0,   452,     0,     0,   453,     0,
       0,   454,     0,     0,     0,   457,     0,     0,     0,     0,
       0,   648,     0,     0,     0,   564,     0,     0,   461,   462,
       0,   316,   317,   318,     0,   320,   321,   322,   323,   324,
     463,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,     0,   338,   339,   340,     0,     0,   343,   344,
     345,   346,   464,   465,   565,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   468,   469,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     473,   474,   475,   476,   477,     0,   478,     0,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,    59,
     566,   491,   492,     0,     0,     0,     0,     0,     0,   493,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   496,   497,   498,     0,
      14,     0,     0,   499,   500,     0,     0,   431,   432,     0,
       0,   501,     0,   502,     0,   503,   504,   437,   438,   439,
     440,   441,     0,     0,     0,     0,     0,   442,     0,   444,
       0,     0,     0,   447,     0,     0,     0,     0,     0,     0,
       0,   449,     0,     0,     0,     0,     0,   452,     0,     0,
     453,     0,     0,   454,     0,     0,     0,   457,     0,     0,
     659,     0,     0,     0,     0,     0,     0,   564,     0,     0,
     461,   462,     0,   316,   317,   318,     0,   320,   321,   322,
     323,   324,   463,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,     0,   338,   339,   340,     0,     0,
     343,   344,   345,   346,   464,   465,   565,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   468,
     469,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   473,   474,   475,   476,   477,     0,   478,     0,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,    59,   566,   491,   492,     0,     0,     0,     0,     0,
       0,   493,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   496,   497,
     498,     0,    14,     0,     0,   499,   500,     0,     0,     0,
       0,   431,   432,   501,     0,   502,     0,   503,   504,   691,
       0,   437,   438,   439,   440,   441,     0,     0,     0,     0,
       0,   442,     0,   444,     0,     0,     0,   447,     0,     0,
       0,     0,     0,     0,     0,   449,     0,     0,     0,     0,
       0,   452,     0,     0,   453,     0,     0,   454,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   564,     0,     0,   461,   462,     0,   316,   317,   318,
       0,   320,   321,   322,   323,   324,   463,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,     0,   338,
     339,   340,     0,     0,   343,   344,   345,   346,   464,   465,
     565,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   473,   474,   475,   476,
     477,     0,   478,     0,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,    59,   566,   491,   492,     0,
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
     464,   465,   565,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,   469,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   473,   474,
     475,   476,   477,     0,   478,     0,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,    59,   566,   491,
     492,     0,     0,     0,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   695,     0,   496,   497,   498,     0,    14,     0,
       0,   499,   500,     0,     0,     0,     0,   431,   432,   501,
       0,   502,     0,   503,   504,   699,     0,   437,   438,   439,
     440,   441,     0,     0,     0,     0,     0,   442,     0,   444,
       0,     0,     0,   447,     0,     0,     0,     0,     0,     0,
       0,   449,     0,     0,     0,     0,     0,   452,     0,     0,
     453,     0,     0,   454,     0,     0,     0,   457,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   564,     0,     0,
     461,   462,     0,   316,   317,   318,     0,   320,   321,   322,
     323,   324,   463,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,     0,   338,   339,   340,     0,     0,
     343,   344,   345,   346,   464,   465,   565,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   468,
     469,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   473,   474,   475,   476,   477,     0,   478,     0,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,    59,   566,   491,   492,     0,     0,     0,     0,     0,
       0,   493,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   496,   497,
     498,     0,    14,     0,     0,   499,   500,     0,     0,   431,
     432,     0,     0,   501,     0,   502,     0,   503,   504,   437,
     438,   439,   440,   441,     0,     0,  1037,     0,     0,   442,
       0,   444,     0,     0,     0,   447,     0,     0,     0,     0,
       0,     0,     0,   449,     0,     0,     0,     0,     0,   452,
       0,     0,   453,     0,     0,   454,     0,     0,     0,   457,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   564,
       0,     0,   461,   462,     0,   316,   317,   318,     0,   320,
     321,   322,   323,   324,   463,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,     0,   338,   339,   340,
       0,     0,   343,   344,   345,   346,   464,   465,   565,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   468,   469,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   473,   474,   475,   476,   477,     0,
     478,     0,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,    59,   566,   491,   492,     0,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   498,     0,    14,     0,     0,   499,   500,     0,
       0,   431,   432,     0,     0,   501,     0,   502,     0,   503,
     504,   437,   438,   439,   440,   441,     0,     0,     0,     0,
       0,   442,     0,   444,     0,     0,     0,   447,     0,     0,
       0,     0,     0,     0,     0,   449,     0,     0,     0,     0,
       0,   452,     0,     0,   453,     0,     0,   454,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   564,     0,     0,   461,   462,     0,   316,   317,   318,
       0,   320,   321,   322,   323,   324,   463,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,     0,   338,
     339,   340,     0,     0,   343,   344,   345,   346,   464,   465,
     565,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   473,   474,   475,   476,
     477,     0,   478,     0,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,    59,   566,   491,   492,     0,
       0,     0,     0,     0,     0,   493,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   496,   497,   498,     0,    14,     0,     0,   499,
     500,     0,     0,   431,   432,     0,     0,   501,     0,   502,
    1057,   503,   504,   437,   438,   439,   440,   441,     0,     0,
       0,     0,     0,   442,     0,   444,     0,     0,     0,   447,
       0,     0,     0,     0,     0,     0,     0,   449,     0,     0,
       0,     0,     0,   452,     0,     0,   453,     0,     0,   454,
       0,     0,     0,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   564,     0,     0,   461,   462,     0,   316,
     317,   318,     0,   320,   321,   322,   323,   324,   463,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
       0,   338,   339,   340,     0,     0,   343,   344,   345,   346,
     464,   465,   565,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,   469,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   473,   474,
     475,   476,   477,     0,   478,     0,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,    59,   566,   491,
     492,     0,     0,     0,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1109,     0,   496,   497,   498,     0,    14,     0,
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
     345,   346,   464,   465,   565,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   468,   469,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     473,   474,   475,   476,   477,     0,   478,     0,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,    59,
     566,   491,   492,     0,     0,     0,     0,     0,     0,   493,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   496,   497,   498,     0,
      14,     0,     0,   499,   500,     0,     0,   431,   432,     0,
       0,   501,     0,   502,  1169,   503,   504,   437,   438,   439,
     440,   441,     0,     0,     0,     0,     0,   442,     0,   444,
       0,     0,     0,   447,     0,     0,     0,     0,     0,     0,
       0,   449,     0,     0,     0,     0,     0,   452,     0,     0,
     453,     0,     0,   454,     0,     0,     0,   457,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   564,     0,     0,
     461,   462,     0,   316,   317,   318,     0,   320,   321,   322,
     323,   324,   463,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,     0,   338,   339,   340,     0,     0,
     343,   344,   345,   346,   464,   465,   565,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   468,
     469,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   473,   474,   475,   476,   477,     0,   478,     0,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,    59,   566,   491,   492,     0,     0,     0,     0,     0,
       0,   493,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   496,   497,
     498,     0,    14,     0,     0,   499,   500,     0,     0,   431,
     432,     0,     0,   501,     0,   502,  1184,   503,   504,   437,
     438,   439,   440,   441,     0,     0,     0,     0,     0,   442,
       0,   444,     0,     0,     0,   447,     0,     0,     0,     0,
       0,     0,     0,   449,     0,     0,     0,     0,     0,   452,
       0,     0,   453,     0,     0,   454,     0,     0,     0,   457,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   564,
       0,     0,   461,   462,     0,   316,   317,   318,     0,   320,
     321,   322,   323,   324,   463,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,     0,   338,   339,   340,
       0,     0,   343,   344,   345,   346,   464,   465,   565,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   468,   469,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   473,   474,   475,   476,   477,     0,
     478,     0,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,    59,   566,   491,   492,     0,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   498,     0,    14,     0,     0,   499,   500,     0,
       0,   431,   432,     0,     0,   501,     0,   502,  1380,   503,
     504,   437,   438,   439,   440,   441,     0,     0,     0,     0,
       0,   442,     0,   444,     0,     0,     0,   447,     0,     0,
       0,     0,     0,     0,     0,   449,     0,     0,     0,     0,
       0,   452,     0,     0,   453,     0,     0,   454,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   564,     0,     0,   461,   462,     0,   316,   317,   318,
       0,   320,   321,   322,   323,   324,   463,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,     0,   338,
     339,   340,     0,     0,   343,   344,   345,   346,   464,   465,
     565,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   473,   474,   475,   476,
     477,     0,   478,     0,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,    59,   566,   491,   492,     0,
       0,     0,     0,     0,     0,   493,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   496,   497,   498,     0,    14,     0,     0,   499,
     500,     0,     0,   431,   432,     0,     0,   501,     0,   502,
    1389,   503,   504,   437,   438,   439,   440,   441,     0,     0,
       0,     0,     0,   442,     0,   444,     0,     0,     0,   447,
       0,     0,     0,     0,     0,     0,     0,   449,     0,     0,
       0,     0,     0,   452,     0,     0,   453,     0,     0,   454,
       0,     0,     0,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   564,     0,     0,   461,   462,     0,   316,
     317,   318,     0,   320,   321,   322,   323,   324,   463,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
       0,   338,   339,   340,     0,     0,   343,   344,   345,   346,
     464,   465,   565,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,   469,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   473,   474,
     475,   476,   477,     0,   478,     0,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,    59,   566,   491,
     492,     0,     0,     0,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   496,   497,   498,     0,    14,     0,
       0,   499,   500,     0,     0,   431,   432,     0,     0,   501,
       0,   502,  1434,   503,   504,   437,   438,   439,   440,   441,
       0,     0,     0,     0,     0,   442,     0,   444,     0,     0,
       0,   447,     0,     0,     0,     0,     0,     0,     0,   449,
       0,     0,     0,     0,     0,   452,     0,     0,   453,     0,
       0,   454,     0,     0,     0,   457,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   564,     0,     0,   461,   462,
       0,   316,   317,   318,     0,   320,   321,   322,   323,   324,
     463,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,     0,   338,   339,   340,     0,     0,   343,   344,
     345,   346,   464,   465,   565,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   468,   469,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     473,   474,   475,   476,   477,     0,   478,     0,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,    59,
     566,   491,   492,     0,     0,     0,     0,     0,     0,   493,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   496,   497,   498,     0,
      14,     0,     0,   499,   500,     0,     0,   431,   432,     0,
       0,   501,     0,   502,  1508,   503,   504,   437,   438,   439,
     440,   441,     0,     0,     0,     0,     0,   442,     0,   444,
       0,     0,     0,   447,     0,     0,     0,     0,     0,     0,
       0,   449,     0,     0,     0,     0,     0,   452,     0,     0,
     453,     0,     0,   454,     0,     0,     0,   457,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   564,     0,     0,
     461,   462,     0,   316,   317,   318,     0,   320,   321,   322,
     323,   324,   463,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,     0,   338,   339,   340,     0,     0,
     343,   344,   345,   346,   464,   465,   565,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   468,
     469,     0,     0,     0,     0,     0,     0,     0,  1545,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   473,   474,   475,   476,   477,     0,   478,     0,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,    59,   566,   491,   492,     0,     0,     0,     0,     0,
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
       0,     0,   343,   344,   345,   346,   464,   465,   565,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   468,   469,     0,     0,     0,     0,     0,     0,     0,
    1587,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   473,   474,   475,   476,   477,     0,
     478,     0,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,    59,   566,   491,   492,     0,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   498,     0,    14,     0,     0,   499,   500,     0,
       0,   431,   432,     0,     0,   501,     0,   502,     0,   503,
     504,   437,   438,   439,   440,   441,     0,     0,     0,     0,
       0,   442,     0,   444,     0,     0,     0,   447,     0,     0,
       0,     0,     0,     0,     0,   449,     0,     0,     0,     0,
       0,   452,     0,     0,   453,     0,     0,   454,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   564,     0,     0,   461,   462,     0,   316,   317,   318,
       0,   320,   321,   322,   323,   324,   463,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,     0,   338,
     339,   340,     0,     0,   343,   344,   345,   346,   464,   465,
     565,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
       0,     0,  1588,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   473,   474,   475,   476,
     477,     0,   478,     0,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,    59,   566,   491,   492,     0,
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
     464,   465,   565,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,   469,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   473,   474,
     475,   476,   477,     0,   478,     0,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,    59,   566,   491,
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
     345,   346,   464,   465,   565,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   468,   469,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     473,   474,   475,   476,   477,     0,   478,     0,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,    59,
     566,   491,   492,     0,   689,     0,     0,     0,     0,   493,
     310,     0,     0,     0,     0,     0,   311,     0,     0,     0,
       0,     0,   312,     0,     0,     0,   496,   497,   498,     0,
      14,     0,   313,   499,   500,     0,     0,     0,     0,     0,
     314,  1365,     0,   502,     0,   503,   504,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,     0,
       0,     0,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   310,
       0,     0,     0,     0,     0,   311,     0,     0,     0,     0,
       0,   312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   313,     0,    58,     0,     0,     0,     0,     0,   314,
       0,     0,     0,     0,     0,     0,   349,     0,     0,     0,
       0,     0,     0,     0,   315,     0,     0,     0,     0,     0,
      59,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   350,     0,   310,     0,
       0,     0,     0,     0,   311,     0,     0,     0,     0,     0,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,     0,    58,     0,     0,     0,     0,     0,   314,     0,
       0,     0,     0,     0,     0,   349,     0,     0,     0,     0,
       0,     0,     0,   315,     0,     0,     0,     0,     0,    59,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   350,     0,   604,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   349,     0,     0,     0,     0,     0,
       0,     0,     0,   310,     0,     0,     0,     0,   609,   311,
       0,     0,     0,     0,     0,   312,     0,     0,     0,     0,
     610,     0,     0,     0,     0,   313,     0,     0,     0,     0,
       0,     0,     0,   314,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,     0,     0,   350,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   310,     0,     0,     0,     0,     0,   311,     0,
       0,     0,     0,     0,   312,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   313,     0,    58,     0,     0,     0,
       0,     0,   314,     0,     0,     0,     0,     0,     0,   349,
       0,     0,     0,     0,     0,     0,     0,   315,     0,     0,
       0,     0,     0,    59,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   732,   733,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   350,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   349,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   609,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   864,   865,   866,   867,   868,   869,   870,
     871,   742,   743,   744,   745,   746,   872,   873,   749,   750,
     751,   752,   874,   754,   755,   756,   757,  -356,   350,   732,
     733,   758,   759,   760,   761,   875,   876,     0,     0,   764,
     765,   766,   877,   878,   879,   770,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   880,   772,     0,   773,   774,   775,
     776,   777,   778,   779,   780,   781,   782,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   783,   784,     0,     0,
       0,   503,   675,     0,     0,   864,   865,   866,   867,   868,
     869,   870,   871,   742,   743,   744,   745,   746,   872,   873,
     749,   750,   751,   752,   874,   754,   755,   756,   757,   732,
     733,     0,     0,   758,   759,   760,   761,   875,   876,     0,
       0,   764,   765,   766,   877,   878,   879,   770,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   732,   733,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1069,
       0,     0,     0,     0,     0,     0,   880,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   783,   784,
       0,     0,     0,   503,   675,   864,   865,   866,   867,   868,
     869,   870,   871,   742,   743,   744,   745,   746,   872,   873,
     749,   750,   751,   752,   874,   754,   755,   756,   757,     0,
       0,     0,     0,   758,   759,   760,   761,   875,   876,     0,
       0,   764,   765,   766,   877,   878,   879,   770,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,   732,   733,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,  1265,
       0,     0,   770,     0,     0,     0,   880,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,   732,
     733,     0,     0,     0,     0,     0,     0,     0,   783,   784,
       0,     0,     0,   503,   675,     0,     0,     0,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,     0,     0,   503,   675,
       0,     0,     0,     0,     0,     0,     0,     0,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,     0,     0,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,     0,
       0,     0,   770,   742,   743,   744,   745,   746,     0,     0,
     749,   750,   751,   752,     0,   754,   755,   756,   757,   732,
     733,     0,     0,   758,     0,   760,   761,     0,     0,     0,
       0,   764,   765,   766,     0,     0,     0,   770,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,   732,   733,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,     0,   796,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   783,   784,
       0,     0,   806,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   742,   743,   744,   745,   746,     0,     0,
     749,   750,   751,   752,     0,   754,   755,   756,   757,     0,
       0,     0,     0,   758,     0,   760,   761,     0,     0,     0,
       0,   764,   765,   766,     0,     0,     0,   770,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,   732,   733,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,     0,
       0,     0,   770,     0,     0,     0,     0,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,   732,
     733,     0,     0,     0,     0,     0,     0,     0,   783,   784,
       0,     0,   822,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,     0,   837,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,     0,     0,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,     0,
       0,     0,   770,   742,   743,   744,   745,   746,     0,     0,
     749,   750,   751,   752,     0,   754,   755,   756,   757,   732,
     733,     0,     0,   758,     0,   760,   761,     0,     0,     0,
       0,   764,   765,   766,     0,     0,     0,   770,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,   732,   733,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,     0,  1141,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   783,   784,
       0,     0,  1145,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   742,   743,   744,   745,   746,     0,     0,
     749,   750,   751,   752,     0,   754,   755,   756,   757,     0,
       0,     0,     0,   758,     0,   760,   761,     0,     0,     0,
       0,   764,   765,   766,     0,     0,     0,   770,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,   732,   733,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,     0,
       0,     0,   770,     0,     0,     0,     0,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,   732,
     733,     0,     0,     0,     0,     0,     0,     0,   783,   784,
       0,     0,  1147,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,     0,  1156,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,     0,     0,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,     0,
       0,     0,   770,   742,   743,   744,   745,   746,     0,     0,
     749,   750,   751,   752,     0,   754,   755,   756,   757,   732,
     733,     0,     0,   758,     0,   760,   761,     0,     0,     0,
       0,   764,   765,   766,     0,     0,     0,   770,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,   732,   733,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,     0,  1157,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   783,   784,
       0,     0,  1158,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   742,   743,   744,   745,   746,     0,     0,
     749,   750,   751,   752,     0,   754,   755,   756,   757,     0,
       0,     0,     0,   758,     0,   760,   761,     0,     0,     0,
       0,   764,   765,   766,     0,     0,     0,   770,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,   732,   733,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,     0,
       0,     0,   770,     0,     0,     0,     0,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,   732,
     733,     0,     0,     0,     0,     0,     0,     0,   783,   784,
       0,     0,  1159,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,     0,  1160,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,     0,     0,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,     0,
       0,     0,   770,   742,   743,   744,   745,   746,     0,     0,
     749,   750,   751,   752,     0,   754,   755,   756,   757,   732,
     733,     0,     0,   758,     0,   760,   761,     0,     0,     0,
       0,   764,   765,   766,     0,     0,     0,   770,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,   732,   733,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,     0,  1161,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   783,   784,
       0,     0,  1286,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   742,   743,   744,   745,   746,     0,     0,
     749,   750,   751,   752,     0,   754,   755,   756,   757,     0,
       0,     0,     0,   758,     0,   760,   761,     0,     0,     0,
       0,   764,   765,   766,     0,     0,     0,   770,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,   732,   733,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,     0,
       0,     0,   770,     0,     0,     0,     0,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,   732,
     733,     0,     0,     0,     0,     0,     0,     0,   783,   784,
       0,     0,  1299,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,     0,  1302,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,     0,     0,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,     0,
       0,     0,   770,   742,   743,   744,   745,   746,     0,     0,
     749,   750,   751,   752,     0,   754,   755,   756,   757,   732,
     733,     0,     0,   758,     0,   760,   761,     0,     0,     0,
       0,   764,   765,   766,     0,     0,     0,   770,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,   732,   733,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,     0,  1437,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   783,   784,
       0,     0,  1452,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   742,   743,   744,   745,   746,     0,     0,
     749,   750,   751,   752,     0,   754,   755,   756,   757,     0,
       0,     0,     0,   758,     0,   760,   761,     0,     0,     0,
       0,   764,   765,   766,     0,     0,     0,   770,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,   732,   733,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,     0,
       0,     0,   770,     0,     0,     0,     0,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,   732,
     733,     0,     0,     0,     0,     0,     0,     0,   783,   784,
       0,     0,  1453,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,     0,  1472,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,     0,     0,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,     0,
       0,     0,   770,   742,   743,   744,   745,   746,     0,     0,
     749,   750,   751,   752,     0,   754,   755,   756,   757,   732,
     733,     0,     0,   758,     0,   760,   761,     0,     0,     0,
       0,   764,   765,   766,     0,     0,     0,   770,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,   732,   733,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,     0,  1474,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   783,   784,
       0,     0,  1476,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   742,   743,   744,   745,   746,     0,     0,
     749,   750,   751,   752,     0,   754,   755,   756,   757,     0,
       0,     0,     0,   758,     0,   760,   761,     0,     0,     0,
       0,   764,   765,   766,     0,     0,     0,   770,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,   732,   733,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,     0,
       0,     0,   770,     0,     0,     0,     0,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,   732,
     733,     0,     0,     0,     0,     0,     0,     0,   783,   784,
       0,     0,  1480,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,     0,  1550,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,     0,     0,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,     0,
       0,     0,   770,   742,   743,   744,   745,   746,     0,     0,
     749,   750,   751,   752,     0,   754,   755,   756,   757,   732,
     733,     0,     0,   758,     0,   760,   761,     0,     0,     0,
       0,   764,   765,   766,     0,     0,     0,   770,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,   732,   733,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,     0,  1551,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   783,   784,
       0,     0,  1552,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   742,   743,   744,   745,   746,     0,     0,
     749,   750,   751,   752,     0,   754,   755,   756,   757,     0,
       0,     0,     0,   758,     0,   760,   761,     0,     0,     0,
       0,   764,   765,   766,     0,     0,     0,   770,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,   732,   733,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,     0,
       0,     0,   770,     0,     0,     0,     0,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,   732,
     733,     0,     0,     0,     0,     0,     0,     0,   783,   784,
       0,     0,  1560,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,     0,  1562,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,     0,     0,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,     0,
       0,     0,   770,   742,   743,   744,   745,   746,     0,     0,
     749,   750,   751,   752,     0,   754,   755,   756,   757,   732,
     733,     0,     0,   758,     0,   760,   761,     0,     0,     0,
       0,   764,   765,   766,     0,     0,     0,   770,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,   732,   733,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,     0,  1573,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   783,   784,
       0,     0,  1578,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   742,   743,   744,   745,   746,     0,     0,
     749,   750,   751,   752,     0,   754,   755,   756,   757,     0,
       0,     0,     0,   758,     0,   760,   761,     0,     0,     0,
       0,   764,   765,   766,     0,     0,     0,   770,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,   732,   733,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,     0,
       0,     0,   770,     0,     0,     0,     0,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,   732,
     733,     0,     0,     0,     0,     0,     0,     0,   783,   784,
       0,     0,  1642,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,     0,  1643,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,     0,     0,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,     0,
       0,     0,   770,   742,   743,   744,   745,   746,   732,   733,
     749,   750,   751,   752,     0,   754,   755,   756,   757,     0,
       0,     0,     0,   758,     0,   760,   761,     0,     0,     0,
       0,   764,   765,   766,     0,     0,     0,   770,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   783,   784,   827,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   783,   784,
    1092,     0,   742,   743,   744,   745,   746,   732,   733,   749,
     750,   751,   752,     0,   754,   755,   756,   757,     0,     0,
       0,     0,   758,     0,   760,   761,     0,     0,     0,     0,
     764,   765,   766,     0,     0,     0,   770,     0,     0,     0,
       0,     0,   732,   733,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   772,     0,   773,   774,
     775,   776,   777,   778,   779,   780,   781,   782,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   783,   784,  1238,
       0,   742,   743,   744,   745,   746,     0,     0,   749,   750,
     751,   752,     0,   754,   755,   756,   757,     0,     0,     0,
       0,   758,     0,   760,   761,     0,     0,     0,     0,   764,
     765,   766,     0,     0,     0,   770,   742,   743,   744,   745,
     746,   732,   733,   749,   750,   751,   752,     0,   754,   755,
     756,   757,     0,     0,     0,     0,   758,     0,   760,   761,
       0,     0,     0,     0,   764,   765,   766,     0,     0,     0,
     770,     0,     0,     0,     0,   772,     0,   773,   774,   775,
     776,   777,   778,   779,   780,   781,   782,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   783,   784,  1253,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     772,     0,   773,   774,   775,   776,   777,   778,   779,   780,
     781,   782,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   783,   784,  1387,     0,   742,   743,   744,   745,   746,
       0,     0,   749,   750,   751,   752,     0,   754,   755,   756,
     757,   231,   232,     0,     0,   758,     0,   760,   761,     0,
       0,     0,     0,   764,   765,   766,     0,     0,   233,   770,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   732,   733,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   772,
       0,   773,   774,   775,   776,   777,   778,   779,   780,   781,
     782,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     783,   784,  1392,     0,     0,     0,     0,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,     0,     0,   252,   253,   254,
       0,     0,     0,     0,     0,     0,   255,   256,   257,   258,
     259,     0,     0,   260,   261,   262,   263,   264,   265,   266,
     742,   743,   744,   745,   746,   732,   733,   749,   750,   751,
     752,     0,   754,   755,   756,   757,     0,     0,     0,     0,
     758,     0,   760,   761,     0,     0,     0,     0,   764,   765,
     766,     0,     0,     0,   770,     0,     0,     0,     0,   267,
       0,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,     0,     0,   278,   279,     0,     0,     0,     0,     0,
     280,   281,     0,     0,   286,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   772,     0,   773,   774,   775,   776,
     777,   778,   779,   780,   781,   782,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   783,   784,     0,     0,   742,
     743,   744,   745,   746,   732,   733,   749,   750,   751,   752,
       0,   754,   755,   756,   757,     0,     0,     0,     0,   758,
       0,   760,   761,     0,     0,   930,     0,   764,   765,   766,
       0,     0,     0,   770,     0,     0,     0,     0,     0,   732,
     733,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   772,     0,   773,   774,   775,   776,   777,
     778,   779,   780,   781,   782,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   783,   784,     0,     0,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,     0,     0,     0,     0,   758,     0,
     760,   761,     0,     0,  1170,     0,   764,   765,   766,   732,
     733,     0,   770,   742,   743,   744,   745,   746,     0,     0,
     749,   750,   751,   752,     0,   754,   755,   756,   757,     0,
       0,     0,     0,   758,     0,   760,   761,     0,     0,     0,
       0,   764,   765,   766,   732,   733,     0,   770,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   772,  1243,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,     0,
       0,     0,     0,   742,   743,   744,   745,   746,   783,   784,
     749,   750,   751,   752,     0,   754,   755,   756,   757,     0,
       0,     0,     0,   758,     0,   760,   761,     0,     0,     0,
       0,   764,   765,   766,   732,   733,     0,   770,   742,   743,
     744,   745,   746,     0,     0,   749,   750,   751,   752,     0,
     754,   755,   756,   757,     0,     0,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,  1255,
       0,     0,   770,   732,   733,     0,     0,   772,     0,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   783,   784,
    1350,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,     0,     0,     0,     0,   742,   743,
     744,   745,   746,   783,   784,   749,   750,   751,   752,     0,
     754,   755,   756,   757,     0,     0,     0,     0,   758,     0,
     760,   761,     0,     0,     0,     0,   764,   765,   766,     0,
       0,     0,   770,   732,   733,     0,     0,   742,   743,   744,
     745,   746,     0,     0,   749,   750,   751,   752,     0,   754,
     755,   756,   757,     0,     0,     0,     0,   758,     0,   760,
     761,     0,     0,     0,  1574,   764,   765,   766,   732,   733,
       0,   770,   772,     0,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   783,   784,     0,     0,     0,     0,     0,
       0,     0,     0,  1633,     0,     0,     0,     0,     0,     0,
       0,   772,     0,   773,   774,   775,   776,   777,   778,   779,
     780,   781,   782,     0,     0,     0,     0,   742,   743,   744,
     745,   746,   783,   784,   749,   750,   751,   752,     0,   754,
     755,   756,   757,     0,     0,     0,     0,   758,     0,   760,
     761,     0,     0,     0,     0,   764,   765,   766,   732,   733,
       0,   770,   742,   743,   744,   745,   746,     0,     0,   749,
     750,   751,   752,     0,   754,   755,   756,   757,     0,     0,
       0,     0,   758,     0,   760,   761,     0,     0,     0,     0,
     764,   765,   766,  1659,     0,     0,   770,   732,   733,     0,
       0,   772,     0,   773,   774,   775,   776,   777,   778,   779,
     780,   781,   782,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   783,   784,     0,     0,     0,     0,  1660,     0,
       0,     0,     0,     0,     0,     0,   772,     0,   773,   774,
     775,   776,   777,   778,   779,   780,   781,   782,     0,     0,
       0,     0,   742,   743,   744,   745,   746,   783,   784,   749,
     750,   751,   752,     0,   754,   755,   756,   757,     0,     0,
       0,     0,   758,     0,   760,   761,     0,     0,     0,     0,
     764,   765,   766,     0,     0,     0,   770,   732,   733,     0,
       0,   742,   743,   744,   745,   746,     0,     0,   749,   750,
     751,   752,     0,   754,   755,   756,   757,     0,     0,     0,
       0,   758,     0,   760,   761,     0,     0,     0,  1666,   764,
     765,   766,   732,   733,     0,   770,   772,     0,   773,   774,
     775,   776,   777,   778,   779,   780,   781,   782,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   783,   784,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   772,     0,   773,   774,   775,
     776,   777,   778,   779,   780,   781,   782,     0,     0,     0,
       0,   742,   743,   744,   745,   746,   783,   784,   749,   750,
     751,   752,     0,   754,   755,   756,   757,     0,     0,     0,
       0,   758,     0,   760,   761,     0,     0,     0,     0,   764,
     765,   766,   732,   733,     0,  -746,   742,   743,   744,   745,
     746,     0,     0,   749,   750,   751,   752,     0,   754,   755,
     756,   757,     0,     0,     0,     0,   758,     0,   760,   761,
     732,   733,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   772,     0,   773,   774,   775,
     776,   777,   778,   779,   780,   781,   782,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   783,   784,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   773,   774,   775,   776,   777,   778,   779,   780,
     781,   782,     0,     0,     0,     0,   742,   743,   744,   745,
     746,   783,   784,   749,   750,   751,   752,     0,   754,   755,
     756,   757,     0,     0,     0,     0,   758,     0,   760,   761,
       0,     0,     0,     0,   742,   743,   744,   745,   746,     0,
       0,   749,   750,   751,   752,     0,   754,   755,   756,   757,
       0,     0,     0,     0,   758,     0,   760,   761,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   970,
       0,     0,     0,   774,   775,   776,   777,   778,   779,   780,
     781,   782,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   783,   784,     0,     0,     0,     0,     0,     0,     0,
     974,     0,   775,   776,   777,   778,   779,   780,   781,   782,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   783,
     784,   316,   317,   318,     0,   320,   321,   322,   323,   324,
     463,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,     0,   338,   339,   340,     0,     0,   343,   344,
     345,   346,   316,   317,   318,     0,   320,   321,   322,   323,
     324,   463,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,     0,   338,   339,   340,     0,     0,   343,
     344,   345,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1239,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   971,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   972,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   975,     0,
       0,     0,   181,     0,     0,     0,     0,   316,   317,   318,
     976,   320,   321,   322,   323,   324,   463,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,     0,   338,
     339,   340,     0,     0,   343,   344,   345,   346,   182,     0,
     183,     0,   184,   185,   186,   187,   188,     0,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,     0,
     200,   201,   202,     0,     0,   203,   204,   205,   206,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   207,   208,     0,     0,     0,     0,
       0,     0,     0,  1240,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   209
};

static const yytype_int16 yycheck[] =
{
      14,    15,   211,   571,   622,   388,   647,   375,   520,   161,
     690,   443,   227,   537,   710,   482,   712,   590,   905,   585,
     813,   587,   815,   589,   817,  1173,   375,   591,   851,   688,
     227,   690,   921,    80,   417,   471,   472,   561,   927,   443,
     382,   627,     4,     7,   103,    19,    32,    19,  1368,    63,
      64,    65,    21,    52,   127,   128,    19,    32,    19,    52,
      19,    19,   721,   467,    14,    15,    19,   158,  1405,   124,
     502,    56,   127,   128,  1428,   141,   178,    62,     0,   141,
    1528,    20,    21,   156,     6,    45,   125,    56,   183,   184,
     104,   105,   106,   107,   443,   168,   178,   480,   481,    18,
      19,   687,   180,   689,   186,   691,   168,    29,  1553,    31,
     212,    33,   190,   699,   127,   128,   211,    39,   467,  1519,
     130,  1521,   708,   189,   215,  1525,  1526,    49,   168,   825,
     142,   143,   144,    55,  1488,  1489,   209,   210,  1586,   101,
     102,     4,     5,  1463,    33,   105,    14,    15,   187,   301,
     212,   190,  1506,   677,   209,   210,  1601,    79,   373,   218,
    1497,    24,   594,   156,   860,   189,   186,    30,   192,   180,
     180,   575,   219,    62,   214,   168,   373,   116,   117,   101,
     102,   180,  1330,   161,  1584,   124,  1523,   883,   127,   128,
     129,   388,   851,   213,   179,   134,   209,   210,     6,   178,
     896,   213,   213,   899,    67,    68,   192,   639,   640,  1563,
    1564,   161,   186,   227,   186,   179,   166,   192,   168,    49,
     417,   171,   419,   186,   210,   186,   575,   186,   186,   198,
     216,   193,   664,   186,   140,   667,   640,   446,   101,   102,
     209,    49,    20,    21,   125,   134,   210,   150,   153,   168,
     212,   460,   141,   158,   135,   125,   150,   304,   180,   180,
     168,    91,   212,   646,   158,   168,   670,  1154,   193,   168,
     209,   210,   178,   194,   168,   125,   180,   199,   141,   168,
    1169,   180,  1105,   480,   481,   135,   669,   212,   210,   721,
    1438,   640,   178,   161,   730,   158,   159,   160,   166,   958,
     168,   156,    61,   171,   212,   168,   187,   186,   213,   180,
     215,   192,   183,   168,   203,   178,   186,   187,   212,   189,
     153,   670,   192,   212,   180,   158,   141,  1214,   214,   215,
     193,   187,   211,   187,   125,   125,   350,   187,   116,   117,
     211,   186,   125,   186,   135,   135,   124,   210,   126,   127,
     128,   129,   135,   168,   786,  1558,   134,   212,   141,   373,
     946,   576,   577,   578,   378,   580,   581,  1570,   125,   584,
     213,   586,    46,   588,   388,   125,   180,  1264,   135,   576,
     577,   578,   215,   580,   581,   135,   728,   584,   186,   586,
     194,   588,    66,   590,   186,   799,   187,   187,   168,   169,
     170,   141,   968,   417,   187,   419,  1609,  1610,     4,     5,
     150,     7,   795,   211,   189,   155,   150,   626,   141,   851,
     186,   213,   180,   186,   186,   439,   440,   150,   168,   199,
     187,   209,   210,   957,   168,  1228,   194,   187,   186,    35,
    1329,   845,   651,   186,   186,   168,  1105,   213,   211,   646,
     799,   213,  1341,   193,   186,   186,   186,   186,   168,    63,
      64,    65,   178,   846,   847,   932,   480,   481,   186,   189,
     213,   213,   669,   189,   194,   942,   859,   913,   193,    56,
    1098,   213,   213,   213,   213,    62,   918,   696,   142,  1169,
     144,   186,   212,   211,   926,   186,   845,   212,    20,    21,
     104,   105,   106,   107,  1184,   888,   889,   716,   891,   186,
    1169,  1084,   895,  1172,   897,   898,   211,   900,   189,   180,
     211,   193,  1086,   194,   189,  1184,   935,    74,   937,   194,
    1323,    78,   189,   194,   211,   732,   733,   194,   193,   141,
     212,   212,   978,   979,   980,    92,    93,   212,   168,   193,
      97,    98,    99,   100,   212,   212,   161,   212,    32,   180,
     180,   758,   576,   577,   578,  1358,   580,   581,   212,   180,
     584,   209,   586,   194,   588,    56,   590,   180,   168,   168,
     190,    62,   168,   194,   194,    59,    60,   180,    11,  1155,
     180,   194,   114,   115,   116,   117,    32,   156,   795,    22,
      23,   194,   124,   168,   126,   127,   128,   129,   823,   168,
     156,   180,   134,   156,   136,   137,   831,   156,   632,   834,
     180,   156,   168,    59,    60,   168,   823,   190,   843,   168,
      56,   194,   646,   168,   831,   850,    62,   834,    56,    56,
     855,  1207,   794,   168,    62,    62,   843,   168,   122,   846,
     847,   125,   126,   850,   186,   669,   168,   189,   855,    56,
     192,   135,   859,   172,   179,    62,  1459,   182,   168,  1310,
     185,  1103,   881,   195,   196,   197,   198,   199,    56,   180,
     142,   143,   144,   180,    62,   186,   122,   209,   210,   186,
     126,   888,   889,   178,   891,    56,   180,   180,   895,   713,
     897,   898,   186,   900,   180,   180,  1365,   142,   190,   187,
     186,   186,   194,   187,   188,  1333,   189,   180,   192,   192,
     935,   195,   937,   186,   190,   190,   172,  1423,   194,   194,
    1162,  1114,  1115,  1116,    34,   190,   210,   190,   935,   194,
     937,   194,   216,   190,   180,   190,   190,   194,   105,   194,
     194,  1183,   188,   172,   173,  1138,   192,  1189,   194,   195,
     212,   190,   214,   215,  1196,   194,   214,   215,  1200,   172,
     173,   174,   175,   178,   210,   168,   169,   170,   792,   178,
     216,   795,    34,   179,   180,   212,   182,   470,   471,   472,
     172,   173,   174,   179,   180,   181,   168,  1363,    10,    11,
      12,   172,   173,   178,  1236,   178,    20,    21,   215,   823,
    1322,    86,    87,   179,   168,   168,  1248,   831,   168,  1437,
     834,   211,    21,  1206,   168,   189,   211,   178,   212,   843,
     189,  1199,   846,   847,    42,   178,   850,   520,   193,   135,
    1037,   855,    20,    21,   193,   859,   193,   212,  1507,   212,
    1199,   193,   193,   193,   193,   193,   193,  1498,  1073,   193,
     193,   190,   168,   211,   547,   168,   212,   212,   193,   168,
     168,  1567,   193,  1569,   888,   889,  1073,   891,   210,   193,
     168,   895,   212,   897,   898,  1581,   900,  1084,   193,   193,
     212,   180,  1324,   161,   212,   212,   193,  1465,   193,   193,
     193,   212,   116,   117,   212,   212,  1565,   193,  1604,    36,
     124,   212,   126,   127,   128,   129,   180,  1114,  1115,  1116,
     134,   935,  1354,   937,   215,   180,     9,   180,    65,   180,
     180,   180,    20,    21,   180,   212,   210,   179,   116,   117,
     212,  1138,    42,  1602,   213,   193,   124,   168,   126,   127,
     128,   129,   193,   168,   211,   180,   134,   179,   189,   179,
    1578,   644,   180,   180,    42,   212,   179,   212,   130,   193,
     193,    13,   161,  1405,  1542,  1543,   186,   193,   187,  1446,
     215,   186,  1449,   197,   198,   199,   189,   178,    12,   672,
     186,   158,   168,  1376,     7,   209,   210,   168,   168,  1208,
     194,   186,   213,   168,  1663,   168,   212,   186,   212,  1206,
     179,   179,   213,   211,   168,   212,   193,   195,   196,   197,
     198,   199,     1,   213,  1592,   212,   114,   115,   116,   117,
     118,   209,   210,   121,   122,   123,   124,   193,   126,   127,
     128,   129,   180,   179,    66,   168,   134,   730,   136,   137,
     194,    32,   212,   212,   212,   212,    42,   212,   168,  1073,
     194,   194,   213,   213,   213,  1497,   212,   168,   213,  1284,
    1084,  1503,   213,   213,   168,   168,   168,   212,    59,    60,
     211,  1464,  1291,   211,   193,   211,   168,  1284,   212,   180,
     212,  1523,   212,   212,   375,   213,   168,   212,   212,  1531,
    1114,  1115,  1116,    32,   385,   193,   194,   195,   196,   197,
     198,   199,   211,   394,   180,   213,   168,    32,   801,   168,
     194,   209,   210,   404,  1138,   211,   211,    69,   212,   180,
      59,    60,   212,   414,   194,   212,   212,   212,   194,   212,
      52,   122,   423,   212,   125,   126,   213,   212,   212,   212,
     212,  1360,   433,   212,   135,  1587,   213,   212,  1541,   213,
     213,   211,   443,  1378,   179,   213,  1375,   448,   179,   450,
     186,   179,   211,   180,   186,   213,   213,   213,   459,  1376,
     213,  1378,   213,   213,   211,   211,   467,   468,   469,   179,
     213,   211,  1206,   122,   212,    32,   213,   126,    79,   180,
     698,   482,     1,   213,    44,   133,   187,   188,    82,   490,
    1456,   192,   493,  1457,   195,   496,   497,   498,   499,   500,
     903,   225,    59,    60,  1433,   792,  1457,   944,   509,   210,
     913,  1457,  1457,  1457,     1,   216,   964,  1433,  1373,  1396,
    1485,  1399,  1260,   427,  1486,    52,   559,   439,  1565,  1444,
    1179,   439,   690,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,    -1,  1477,   192,    -1,   194,   195,  1464,    -1,    -1,
    1284,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1477,   210,    -1,    -1,    -1,   122,    -1,   216,    -1,   126,
      -1,    -1,    -1,    -1,   575,   978,   979,   980,    -1,    -1,
     983,    -1,   985,    -1,   987,    -1,   989,    -1,   991,    -1,
     993,    -1,   995,    -1,   997,    -1,    -1,    -1,    -1,  1002,
      -1,  1004,   603,    -1,    -1,    -1,    -1,  1010,    -1,    -1,
    1539,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1022,
      -1,  1024,    -1,   375,  1541,    -1,  1029,    -1,  1031,    -1,
    1033,   188,    -1,  1036,    -1,   192,    -1,   194,   195,   640,
      -1,    -1,  1376,    -1,  1378,    -1,   647,   648,    -1,   650,
      -1,    -1,    -1,   210,   655,    -1,    -1,    -1,    -1,   216,
     661,    -1,    -1,    -1,    -1,  1068,    -1,    -1,    -1,   670,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     681,   682,   683,   684,   685,   686,    -1,   688,    -1,   690,
      -1,   443,    -1,    -1,    -1,    -1,    -1,  1100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1636,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   467,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
    1464,    -1,    -1,   734,   735,   736,   737,   738,   739,   740,
     741,   742,   743,  1477,    -1,   746,   747,   748,   749,   750,
     751,    -1,   753,   754,    -1,   756,   757,   758,   759,   760,
     761,   762,   763,   764,   765,   766,   767,   768,   769,   770,
     771,   772,   773,   774,   775,   776,   777,   778,   779,   780,
     781,   782,    -1,   784,    -1,    -1,    -1,   788,    -1,    -1,
      -1,    -1,    -1,    -1,  1528,    -1,    -1,    -1,   799,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1541,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,   114,   115,
     116,   117,   118,   575,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,   838,   134,    -1,
     136,   137,    59,    60,   845,    -1,   142,   143,   144,    -1,
      -1,   852,  1586,   854,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   863,   864,   865,   866,   867,   868,   869,   870,
     871,   872,   873,   874,   875,   876,   877,   878,   879,   880,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   640,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,   122,    -1,    -1,    -1,   126,
      -1,    -1,    -1,   209,   210,   916,   917,    -1,   670,  1322,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   930,
      -1,   932,    -1,    -1,    20,    21,    -1,    -1,   939,    -1,
      -1,   942,    -1,    -1,    -1,    -1,   947,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   956,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,    20,    21,    -1,   192,    -1,   194,   195,    -1,
      -1,    -1,   734,   735,   736,   737,   738,   739,   740,   741,
      -1,    -1,    -1,   210,    -1,   747,   748,    -1,    -1,   216,
      -1,   753,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     762,   763,    -1,    -1,    -1,   767,   768,   769,    -1,   771,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,   799,   134,  1050,
     136,   137,    -1,  1054,    -1,    -1,   142,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,   135,   136,   137,
     138,   139,    -1,   845,   142,   143,   144,   145,   146,   147,
     148,    -1,    -1,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
     188,  1142,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,  1164,  1165,  1166,   214,   215,    -1,  1170,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1178,  1179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      11,    -1,    -1,    -1,    -1,    18,    -1,    -1,  1199,    20,
      21,    24,    -1,  1204,  1205,    -1,    -1,    30,    -1,    -1,
    1211,    -1,    -1,    -1,    -1,  1216,   122,    40,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,  1230,
      -1,  1232,    -1,    -1,    -1,    -1,  1237,    -1,    -1,    -1,
      63,    -1,  1243,    20,    21,    -1,  1247,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
      -1,    -1,   188,    -1,    -1,    -1,   192,  1288,   194,   195,
      -1,    -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,   210,   126,   127,   128,   129,  1310,
     216,    -1,    -1,   134,    -1,   136,   137,    -1,   141,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,
      -1,   154,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,    -1,    -1,   121,   168,    -1,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    -1,
      -1,    -1,    -1,  1384,    -1,  1386,    -1,    -1,   209,   210,
    1391,   214,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1414,    -1,  1416,   193,   194,   195,   196,
     197,   198,   199,  1424,  1425,  1426,    -1,    -1,    -1,    -1,
    1431,    -1,   209,   210,    -1,  1436,    -1,    -1,    -1,  1440,
    1441,  1442,  1443,    -1,    -1,  1446,  1447,  1199,  1449,  1450,
      -1,    -1,    -1,    -1,    -1,    -1,  1457,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1468,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,  1498,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,  1513,  1514,   142,    -1,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1527,    -1,    -1,    -1,
      -1,  1532,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1544,  1545,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1559,    -1,
    1561,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,    59,    60,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,    -1,    -1,  1588,    -1,    -1,
      -1,    -1,  1593,  1594,    -1,    -1,    -1,     1,    -1,    -1,
       4,     5,     6,    -1,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    18,    -1,  1617,    -1,  1619,  1620,
      24,    25,    26,    27,    28,    -1,    30,    -1,    -1,  1630,
      -1,    -1,    -1,    37,    38,    -1,    -1,    41,   122,    43,
      44,    -1,   126,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    -1,    97,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,   188,    -1,    -1,    -1,   192,    -1,
     194,   195,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   210,   131,   132,   133,
      -1,    -1,   216,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,
      -1,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,    -1,    -1,
      -1,    -1,    -1,    -1,   178,   179,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,   203,
      -1,    -1,    -1,    -1,    -1,    -1,   210,    -1,   212,     1,
     214,   215,     4,     5,     6,    -1,     8,     9,    10,    -1,
      12,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    25,    26,    27,    28,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    -1,    41,
      -1,    43,    44,    -1,    -1,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    -1,    57,    58,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,
     152,   153,    32,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,   178,   179,   180,    59,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,    -1,   199,    -1,    -1,
     202,   203,    -1,    -1,     4,     5,    -1,    -1,   210,    -1,
     212,    -1,   214,   215,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,   122,    -1,    44,    -1,   126,    47,    -1,    -1,
      50,    -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    59,
      60,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    -1,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,   188,    -1,
      -1,    -1,   192,    -1,   194,   195,   116,   117,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,   126,    -1,    -1,    -1,
     210,    -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,   151,   152,   153,    32,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    59,    60,    -1,    -1,    -1,    -1,    -1,   188,    -1,
      -1,    -1,   192,    -1,    -1,   195,   196,   197,    -1,   199,
      -1,    -1,   202,   203,    -1,    -1,    -1,     4,     5,    -1,
     210,    -1,   212,    -1,   214,   215,   216,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,   122,    -1,    -1,    44,   126,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    59,    60,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    -1,    70,    71,    72,    -1,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    92,    93,    94,    -1,    -1,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
     188,    -1,    -1,    -1,   192,    -1,   194,   195,    -1,   116,
     117,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,   126,
      -1,    -1,   210,    -1,    -1,    -1,    -1,    -1,   216,    -1,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,   150,   151,   152,   153,    32,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,
      -1,   188,    -1,    -1,    -1,   192,    -1,    -1,   195,   196,
     197,    -1,   199,    -1,    -1,   202,   203,    -1,    -1,    -1,
       4,     5,    -1,   210,    -1,   212,    -1,   214,   215,   216,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    26,    -1,    -1,    -1,    30,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,   122,    -1,    -1,
      44,   126,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    -1,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,   188,    -1,    -1,    -1,   192,    -1,   194,
     195,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   210,    -1,    -1,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,
      32,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    59,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,    -1,
      -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,   203,
      -1,    -1,    -1,     4,     5,    -1,   210,    -1,   212,    -1,
     214,   215,   216,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
     122,    -1,    -1,    44,   126,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    94,    -1,    -1,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,   188,    -1,    -1,    -1,
     192,    -1,   194,   195,    -1,   116,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,   210,    -1,
     131,   132,   133,    -1,   216,    -1,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
     151,   152,   153,    32,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      59,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,    -1,   199,    -1,
      -1,   202,   203,    -1,    -1,     4,     5,    -1,    -1,   210,
      -1,   212,    -1,   214,   215,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,   122,    -1,    44,    -1,   126,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,   188,
      -1,    -1,    -1,   192,    -1,   194,   195,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,   210,   131,   132,   133,    -1,    -1,   216,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,   151,   152,   153,    32,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,    -1,
     199,    -1,    -1,   202,   203,    -1,    -1,     4,     5,    -1,
      -1,   210,    -1,   212,    -1,   214,   215,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    25,    26,
      27,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,   122,    -1,    44,    -1,   126,
      47,    -1,    -1,    50,    51,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    -1,    70,    71,    72,    -1,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    92,    93,    94,    -1,    -1,
      97,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,   188,    -1,    -1,    -1,   192,    -1,   194,   195,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   210,    -1,    -1,    -1,    -1,    -1,   216,
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
      -1,    -1,    -1,    -1,    -1,   210,   131,   132,   133,    -1,
      -1,   216,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,    32,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    59,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,    -1,   199,    -1,    -1,   202,   203,    -1,
      -1,     4,     5,    -1,    -1,   210,    -1,   212,    -1,   214,
     215,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    32,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   122,
      -1,    44,    -1,   126,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,    72,
      -1,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,   192,
      -1,   194,   195,   116,   117,    -1,   122,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,   210,   131,   132,
     133,    -1,    -1,   216,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,
     153,    -1,   155,    32,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      59,    60,   188,    -1,    -1,    -1,   192,    -1,   194,   195,
      -1,    -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,
     203,    -1,    -1,    -1,   210,     4,     5,   210,    -1,   212,
     216,   214,   215,    12,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      32,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,   122,    -1,    44,    -1,   126,    47,    48,
      -1,    50,    -1,    -1,    -1,    54,    -1,    59,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,   188,
      -1,    -1,    -1,   192,    -1,   194,   195,   116,   117,    -1,
     122,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,   210,    -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,   151,   152,   153,    -1,   155,    32,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    59,    60,   188,    -1,    -1,    -1,
     192,    -1,   194,   195,    -1,    -1,   195,   196,   197,    -1,
     199,    -1,    -1,   202,   203,    -1,    -1,    -1,   210,     4,
       5,   210,   211,   212,   216,   214,   215,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    32,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,   122,    -1,    44,
      -1,   126,    47,    48,    -1,    50,    -1,    -1,    -1,    54,
      -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    -1,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,   188,    -1,    -1,    -1,   192,    -1,   194,
     195,   116,   117,    -1,   122,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,   210,    -1,    -1,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    -1,    -1,    -1,   192,    -1,   194,   195,    -1,    -1,
     195,   196,   197,    -1,   199,    -1,    -1,   202,   203,    -1,
      -1,    -1,   210,     4,     5,   210,    -1,   212,   216,   214,
     215,    12,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    32,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    48,    -1,    50,
      -1,    -1,    -1,    54,    -1,    59,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    94,    -1,    -1,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,    -1,   122,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
     151,   152,   153,    -1,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,   192,    -1,
      -1,   195,    -1,    -1,   195,   196,   197,    -1,   199,    -1,
      -1,   202,   203,    -1,    -1,    -1,   210,     4,     5,   210,
     211,   212,   216,   214,   215,    12,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    48,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    -1,    70,    71,    72,    -1,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    92,    93,    94,    -1,    -1,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,   735,
     736,   737,   738,   739,   740,   741,    -1,    -1,    -1,   116,
     117,   747,   748,    -1,    -1,    -1,    -1,   753,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   762,   763,    -1,    -1,
      -1,   767,   768,   769,   141,   771,    -1,    -1,    -1,    -1,
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
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
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
      -1,    64,    -1,    -1,    67,    68,    69,    70,    71,    72,
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
     211,   212,    -1,   214,   215,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
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
      57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
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
      -1,     4,     5,   210,    -1,   212,    -1,   214,   215,    12,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
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
      -1,    -1,   193,    -1,   195,   196,   197,    -1,   199,    -1,
      -1,   202,   203,    -1,    -1,    -1,    -1,     4,     5,   210,
      -1,   212,    -1,   214,   215,    12,    -1,    14,    15,    16,
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
      15,    16,    17,    18,    -1,    -1,    21,    -1,    -1,    24,
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
      -1,    -1,   193,    -1,   195,   196,   197,    -1,   199,    -1,
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
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
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
     169,   170,   171,    -1,    12,    -1,    -1,    -1,    -1,   178,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,   195,   196,   197,    -1,
     199,    -1,    40,   202,   203,    -1,    -1,    -1,    -1,    -1,
      48,   210,    -1,   212,    -1,   214,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    40,    -1,   141,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
     168,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      40,    -1,   141,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,   168,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,    -1,   216,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,   168,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
     180,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,   214,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    40,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,   154,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,   168,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   214,    20,
      21,   134,   135,   136,   137,   138,   139,    -1,    -1,   142,
     143,   144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
      -1,   214,   215,    -1,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,    20,
      21,    -1,    -1,   134,   135,   136,   137,   138,   139,    -1,
      -1,   142,   143,   144,   145,   146,   147,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,    -1,    -1,    -1,    -1,    -1,   187,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,    -1,   214,   215,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,   135,   136,   137,   138,   139,    -1,
      -1,   142,   143,   144,   145,   146,   147,   148,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    20,    21,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,   180,
      -1,    -1,   148,    -1,    -1,    -1,   187,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,    -1,   214,   215,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    20,
      21,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    20,    21,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    20,
      21,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    20,    21,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    20,
      21,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    20,    21,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    20,
      21,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    20,    21,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    20,
      21,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    20,    21,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    20,
      21,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    20,    21,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    20,
      21,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    20,    21,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    20,
      21,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    20,    21,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,   114,   115,   116,   117,   118,    20,    21,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,   211,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
     211,    -1,   114,   115,   116,   117,   118,    20,    21,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,   211,
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
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    20,    21,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    37,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,   211,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    -1,    -1,   126,   127,   128,
      -1,    -1,    -1,    -1,    -1,    -1,   135,   136,   137,   138,
     139,    -1,    -1,   142,   143,   144,   145,   146,   147,   148,
     114,   115,   116,   117,   118,    20,    21,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,   202,   203,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   114,
     115,   116,   117,   118,    20,    21,   121,   122,   123,   124,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,   136,   137,    -1,    -1,   140,    -1,   142,   143,   144,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,   140,    -1,   142,   143,   144,    20,
      21,    -1,   148,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    20,    21,    -1,   148,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    -1,
      -1,    -1,    -1,   114,   115,   116,   117,   118,   209,   210,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    20,    21,    -1,   148,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,   180,
      -1,    -1,   148,    20,    21,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
     176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,   209,   210,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,    20,    21,    -1,    -1,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,   180,   142,   143,   144,    20,    21,
      -1,   148,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,   209,   210,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    20,    21,
      -1,   148,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,   180,    -1,    -1,   148,    20,    21,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,    -1,    -1,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,   209,   210,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,    20,    21,    -1,
      -1,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,   180,   142,
     143,   144,    20,    21,    -1,   148,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
      -1,   114,   115,   116,   117,   118,   209,   210,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    20,    21,    -1,   148,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,    -1,   114,   115,   116,   117,
     118,   209,   210,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,    -1,
      -1,   121,   122,   123,   124,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    -1,   192,   193,   194,   195,   196,   197,   198,   199,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,
     210,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    -1,    97,
      98,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    70,    71,    72,
     168,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    97,    98,    99,   100,    70,    -1,
      72,    -1,    74,    75,    76,    77,    78,    -1,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    99,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   168
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
     289,   359,   376,   211,    64,   103,   169,   283,   345,   168,
     168,   418,   125,   135,   187,   373,   419,   424,   426,   345,
     428,   422,   168,   414,   430,   432,   434,   436,   438,   440,
     442,   444,   345,   193,   212,    32,   192,    32,   192,   210,
     216,   211,   345,   210,   216,   418,   168,   180,   461,   168,
     180,   365,   446,   451,   168,   180,   368,   456,   345,   150,
     168,   372,   406,   363,   193,   193,   345,   250,   193,   291,
     409,   462,   212,   289,   193,     4,   101,   102,   193,   212,
     125,   288,   319,   330,   345,   275,   193,   212,    60,   345,
     212,   345,   168,   193,   193,   212,   180,   193,   161,    57,
     345,   212,   275,   193,   212,   193,   193,   212,   193,   193,
     125,   288,   345,   345,   345,   215,   275,   321,   325,   325,
     325,   212,   212,   212,   212,   212,   212,    12,   418,    12,
     418,    12,   345,   486,   502,   193,   345,   193,   225,    12,
     486,   503,    36,   345,   345,   345,   345,   345,    12,    48,
     319,   345,   319,   215,   180,   180,   345,     9,   321,   327,
     168,   212,   180,   180,   180,   180,   180,    65,   304,   266,
     130,   180,    20,    21,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   134,   135,
     136,   137,   138,   139,   142,   143,   144,   145,   146,   147,
     148,   187,   188,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   209,   210,   327,   212,   180,   187,   179,
     383,   179,   210,   266,   360,   193,   213,    42,   180,   373,
     288,   345,   446,   446,   416,   446,   213,   446,   446,   213,
     168,   411,   446,   267,   446,   267,   446,   267,   365,   366,
     368,   369,   213,   421,   280,   319,   211,   211,   345,   180,
     179,   189,   180,   179,   189,   180,   179,   213,   212,    42,
     125,   186,   187,   189,   192,   371,   477,   479,   275,   406,
     292,   212,   289,   193,   212,   316,   193,   193,   193,   498,
     319,   288,   319,   186,   106,   107,   108,   109,   110,   111,
     112,   113,   119,   120,   125,   138,   139,   145,   146,   147,
     187,    13,   418,   503,   345,   345,   275,   187,   309,   311,
     345,   313,   189,   161,   345,   500,   319,   483,   488,   319,
     481,   418,   288,   345,   215,   266,   345,   345,   345,   345,
     345,   345,   406,    52,   156,   168,   195,   210,   212,   345,
     463,   466,   469,   485,   490,   406,   212,   466,   490,   406,
     140,   179,   180,   181,   470,   284,   275,   286,   174,   175,
     223,   406,   186,   506,   178,   406,    12,   186,   506,   506,
     150,   155,   193,   289,   335,   275,   248,   328,   210,   213,
     319,   463,   465,   158,   212,   306,   376,   158,   324,   325,
      18,   156,   168,   407,    18,   156,   168,   407,   131,   132,
     133,   276,   331,   345,   331,   345,   331,   345,   331,   345,
     331,   345,   331,   345,   331,   345,   331,   345,   345,   345,
     345,   331,   345,   331,   345,   345,   345,   345,   168,   331,
     345,   345,   156,   168,   345,   345,   345,   407,   345,   345,
     345,   331,   345,   331,   345,   345,   345,   345,   331,   345,
     331,   345,   331,   345,   345,   331,   345,    21,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   127,
     128,   156,   168,   209,   210,   342,   345,   213,   319,   345,
     396,   265,     7,   353,   358,   418,   168,   288,   345,   180,
     194,   194,   194,   180,   194,   194,   180,   194,   194,   268,
     194,   268,   194,   268,   180,   194,   180,   194,   282,   446,
     213,   506,   211,   446,   446,   345,   168,   168,   446,   288,
     345,   418,   418,    19,   446,    69,   319,   465,   475,   193,
     345,   168,   345,   446,   492,   494,   496,   418,   506,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   275,   180,   194,
     506,   213,   255,   418,   418,   213,   418,   213,   418,   506,
     418,   418,   506,   418,   194,   324,   213,   213,   213,   213,
     213,   213,    19,   325,   212,   135,   371,   210,   345,   213,
     140,   186,   180,   469,   183,   184,   211,   473,   186,   180,
     183,   211,   472,    19,   213,   469,   179,   182,   471,    19,
     345,   179,   486,   282,   282,   345,    19,   486,   179,   278,
      19,   406,   211,   213,   212,   212,   337,   339,    11,    22,
      23,   241,   242,   345,   266,   463,   186,   213,   213,   168,
     305,   305,   125,   135,   187,   192,   322,   323,   267,   193,
     212,   193,   212,   325,   325,   325,   212,   212,   211,    18,
     156,   168,   407,   189,   156,   168,   345,   212,   212,   156,
     168,   345,     1,   211,   213,   180,   179,   211,    56,    62,
     356,    66,   357,   180,   194,   180,   420,   425,   427,   446,
     429,   423,   168,   415,   431,   194,   435,   194,   439,   194,
     365,   443,   368,   445,   180,   194,   213,    42,   371,   194,
     194,   319,   194,   168,   465,   213,   213,   213,   168,   213,
     180,   194,   213,   194,   418,   418,   418,   194,   213,   418,
     212,   213,   345,   194,   194,   194,   194,   213,   194,   194,
     213,   194,   324,   267,   212,   319,   345,   345,   345,   466,
     469,   345,   156,   168,   463,   473,   211,   345,   485,   211,
     319,   466,   179,   182,   185,   474,   211,   319,   194,   194,
     176,   319,   179,   319,    19,   345,   345,   418,   267,   275,
     345,    11,   244,   324,   211,   210,   186,   211,   213,   168,
     168,   168,   168,   186,   211,   268,   346,   345,   348,   345,
     213,   319,   345,   193,   212,   345,   212,   211,   345,   213,
     319,   212,   211,   343,   180,    46,   357,    45,   105,   354,
     324,   433,   437,   441,   446,   212,   168,   345,   478,   480,
     275,   293,   213,   180,   194,   168,   212,   317,   194,   194,
     194,   499,   194,   503,   310,   312,   314,   501,   484,   489,
     482,   212,   327,   268,   213,   319,   180,   213,   469,   473,
     212,   135,   371,   180,   469,   211,   180,   285,   287,   180,
     180,   319,   213,   213,   194,   268,   275,   245,   180,   267,
     213,   463,   168,   211,   189,   374,   322,   211,   140,   275,
     320,   418,   213,   446,   213,   213,   213,   350,   345,   345,
     213,   213,   345,    32,   355,   354,   356,   280,   212,   212,
     168,   345,   194,   345,   493,   495,   497,   212,   212,   506,
     345,   345,   345,   212,    69,   475,   212,   212,   213,   345,
     320,   213,   345,   135,   371,   473,   345,   345,   345,   345,
     474,   486,   345,   212,   281,   486,   345,   180,   338,   194,
     242,    25,   104,   246,   295,   296,   297,   299,   345,   268,
     211,   189,   374,   418,   373,   125,   345,   194,   194,   446,
     213,   213,   213,   361,   355,   372,   213,   475,   475,   194,
     213,   212,   213,   212,   212,   212,   280,   503,   213,   319,
     475,   463,   464,   213,   180,   505,   345,   345,   213,   505,
     505,   280,   505,   505,   345,   335,   340,   125,   125,   345,
     275,   213,   418,   373,   373,   345,   345,   347,   349,   194,
     272,   362,   212,   463,   467,   468,   468,   345,   345,   475,
     475,   464,   213,   506,   506,   468,   213,    52,   211,   135,
     371,   179,   179,   186,   506,   179,   211,   505,   335,   345,
     373,   345,   345,   180,   351,   180,   272,   463,   186,   506,
     213,   213,   213,   213,   468,   468,   213,   213,   213,   213,
     345,   211,   345,   345,   211,   179,   213,   211,   345,   180,
     180,   275,   213,   212,   213,   213,   180,   463,   213
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
     275,   276,   276,   276,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   278,   278,   278,   279,   280,
     280,   281,   281,   282,   282,   284,   285,   283,   286,   287,
     283,   288,   288,   288,   288,   288,   289,   289,   289,   290,
     290,   292,   293,   291,   291,   294,   294,   294,   294,   294,
     294,   295,   296,   297,   297,   297,   298,   298,   298,   299,
     299,   300,   300,   300,   301,   302,   302,   302,   303,   303,
     304,   304,   305,   305,   306,   306,   306,   306,   307,   307,
     309,   310,   308,   311,   312,   308,   313,   314,   308,   316,
     317,   315,   318,   318,   318,   318,   318,   318,   319,   319,
     320,   320,   320,   321,   321,   321,   322,   322,   322,   322,
     323,   323,   324,   324,   325,   325,   326,   328,   327,   329,
     329,   329,   329,   329,   329,   329,   330,   330,   330,   330,
     330,   330,   330,   330,   330,   330,   330,   330,   330,   330,
     330,   330,   330,   330,   330,   331,   331,   331,   331,   332,
     332,   332,   332,   332,   332,   332,   332,   332,   332,   332,
     332,   332,   332,   332,   332,   332,   333,   333,   334,   334,
     335,   335,   336,   337,   338,   336,   339,   340,   336,   341,
     341,   341,   341,   342,   343,   341,   344,   344,   344,   344,
     344,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   346,   347,   345,
     345,   345,   345,   348,   349,   345,   345,   345,   350,   351,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     352,   353,   353,   353,   354,   354,   354,   355,   355,   356,
     356,   356,   357,   357,   358,   359,   360,   359,   361,   359,
     362,   359,   359,   363,   363,   364,   364,   365,   365,   366,
     366,   367,   367,   367,   368,   369,   369,   370,   370,   370,
     371,   371,   372,   372,   372,   372,   372,   372,   373,   373,
     373,   374,   374,   375,   375,   375,   375,   375,   376,   376,
     376,   376,   376,   377,   378,   377,   379,   379,   380,   380,
     380,   381,   382,   381,   383,   383,   383,   383,   384,   384,
     384,   386,   385,   387,   387,   388,   389,   388,   390,   390,
     390,   391,   393,   394,   392,   395,   396,   392,   397,   397,
     398,   398,   399,   400,   400,   401,   401,   401,   402,   402,
     404,   405,   403,   406,   406,   406,   406,   406,   407,   407,
     407,   407,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   407,   407,   408,   408,   408,   408,   408,
     408,   408,   408,   409,   410,   410,   410,   411,   411,   412,
     412,   412,   414,   415,   413,   416,   416,   417,   417,   418,
     418,   418,   418,   418,   418,   419,   420,   418,   418,   418,
     421,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   422,   423,   418,   418,   424,   425,
     418,   426,   427,   418,   428,   429,   418,   418,   430,   431,
     418,   432,   433,   418,   418,   434,   435,   418,   436,   437,
     418,   418,   438,   439,   418,   440,   441,   418,   442,   443,
     418,   444,   445,   418,   446,   446,   446,   448,   449,   450,
     451,   447,   453,   454,   455,   456,   452,   458,   459,   460,
     461,   457,   462,   462,   462,   462,   462,   463,   463,   463,
     463,   463,   463,   463,   463,   464,   465,   466,   466,   467,
     467,   468,   468,   469,   469,   470,   470,   471,   471,   472,
     472,   473,   473,   473,   474,   474,   474,   475,   475,   476,
     476,   476,   476,   476,   476,   477,   478,   476,   479,   480,
     476,   481,   482,   476,   483,   484,   476,   485,   485,   485,
     486,   486,   487,   488,   489,   487,   490,   490,   491,   491,
     491,   492,   493,   491,   494,   495,   491,   496,   497,   491,
     491,   498,   499,   491,   491,   500,   501,   491,   502,   502,
     503,   503,   504,   504,   504,   504,   504,   505,   505,   506,
     506,   507,   507,   507,   507,   507,   507
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
       7,     2,     2,     6,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     2,     2,     2,     0,     2,     2,     3,     0,
       2,     0,     4,     1,     3,     0,     0,     7,     0,     0,
       7,     3,     2,     2,     2,     1,     1,     3,     2,     2,
       3,     0,     0,     5,     1,     2,     5,     5,     5,     6,
       2,     1,     1,     1,     2,     3,     2,     2,     3,     2,
       3,     2,     2,     3,     4,     1,     1,     0,     1,     1,
       1,     0,     1,     3,     9,     8,     8,     7,     3,     3,
       0,     0,     7,     0,     0,     7,     0,     0,     7,     0,
       0,     6,     5,     8,    10,     5,     8,    10,     1,     3,
       1,     2,     3,     1,     1,     2,     2,     2,     2,     2,
       1,     3,     0,     4,     1,     6,     6,     0,     7,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     6,     8,     5,     6,
       1,     4,     3,     0,     0,     8,     0,     0,     9,     3,
       4,     5,     6,     0,     0,     5,     3,     4,     4,     3,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     4,     4,     5,     4,     5,     3,     4,     1,     1,
       2,     4,     4,     7,     8,     3,     5,     0,     0,     8,
       3,     3,     3,     0,     0,     8,     3,     4,     0,     0,
       9,     4,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     2,     4,     1,     4,     4,     4,     4,     4,
       1,     6,     7,     6,     6,     7,     7,     6,     7,     6,
       6,     0,     4,     1,     0,     1,     1,     0,     1,     0,
       1,     1,     0,     1,     5,     0,     0,     4,     0,     9,
       0,    10,     5,     3,     4,     1,     3,     1,     3,     1,
       3,     0,     2,     3,     3,     1,     3,     0,     2,     3,
       1,     1,     1,     2,     3,     5,     3,     3,     1,     1,
       1,     0,     1,     1,     4,     3,     3,     5,     4,     6,
       5,     5,     4,     0,     0,     4,     0,     1,     0,     1,
       1,     6,     0,     6,     0,     2,     3,     5,     0,     1,
       1,     0,     5,     2,     3,     4,     0,     4,     0,     1,
       1,     1,     0,     0,     9,     0,     0,    11,     0,     2,
       0,     1,     3,     1,     1,     0,     1,     1,     0,     3,
       0,     0,     7,     1,     4,     3,     3,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     1,     3,     0,
       2,     3,     0,     0,     6,     1,     3,     3,     4,     1,
       1,     1,     1,     2,     3,     0,     0,     6,     4,     5,
       0,     9,     4,     2,     2,     3,     2,     3,     2,     2,
       3,     3,     3,     2,     0,     0,     6,     2,     0,     0,
       6,     0,     0,     6,     0,     0,     6,     1,     0,     0,
       6,     0,     0,     7,     1,     0,     0,     6,     0,     0,
       7,     1,     0,     0,     6,     0,     0,     7,     0,     0,
       6,     0,     0,     6,     1,     3,     3,     0,     0,     0,
       0,    10,     0,     0,     0,     0,    10,     0,     0,     0,
       0,    10,     1,     1,     1,     1,     1,     3,     3,     5,
       5,     6,     6,     8,     8,     1,     1,     1,     3,     3,
       5,     1,     2,     0,     2,     2,     1,     2,     1,     2,
       1,     2,     1,     1,     2,     1,     1,     0,     1,     5,
       4,     6,     7,     5,     7,     0,     0,    10,     0,     0,
      10,     0,     0,     9,     0,     0,     7,     1,     3,     3,
       3,     1,     5,     0,     0,    10,     1,     3,     4,     4,
       4,     0,     0,    11,     0,     0,    11,     0,     0,    10,
       5,     0,     0,     9,     5,     0,     0,    10,     1,     3,
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

  case 223: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 224: /* expression_any: "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 225: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 226: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 227: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 228: /* expression_any: expr_assign "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 229: /* expression_any: expression_delete "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 230: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 231: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 232: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 233: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 234: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 235: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 236: /* expression_any: expression_break "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 237: /* expression_any: expression_continue "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 238: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 239: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 240: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 241: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 242: /* expression_any: expression_label "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 243: /* expression_any: expression_goto "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 244: /* expression_any: "pass" "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 245: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 246: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 247: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 248: /* expr_keyword: "keyword" expr expression_block  */
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

  case 249: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 250: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 251: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 252: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 253: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 254: /* type_declaration_no_options_list: type_declaration_no_options_list "end of expression" type_declaration  */
                                                                           {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 255: /* $@7: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 256: /* $@8: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 257: /* expression_keyword: "keyword" '<' $@7 type_declaration_no_options_list '>' $@8 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 258: /* $@9: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 259: /* $@10: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 260: /* expression_keyword: "type function" '<' $@9 type_declaration_no_options_list '>' $@10 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 261: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 262: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 263: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 264: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 265: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 266: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 267: /* name_in_namespace: "name" "::" "name"  */
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

  case 268: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 269: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 270: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 271: /* $@11: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 272: /* $@12: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 273: /* new_type_declaration: '<' $@11 type_declaration '>' $@12  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 274: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 275: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 276: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 277: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 278: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 279: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 280: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 281: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 282: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 283: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 284: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 285: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 286: /* expression_return: expression_return_no_pipe "end of expression"  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 287: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 288: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 289: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 290: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 291: /* expression_yield: expression_yield_no_pipe "end of expression"  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 292: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 293: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 294: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 295: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 296: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 297: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 298: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 299: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 300: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 301: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 302: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 303: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 304: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 305: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 306: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 307: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 308: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 309: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 310: /* $@13: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 311: /* $@14: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 312: /* expr_cast: "cast" '<' $@13 type_declaration_no_options '>' $@14 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 313: /* $@15: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 314: /* $@16: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 315: /* expr_cast: "upcast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 316: /* $@17: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 317: /* $@18: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 318: /* expr_cast: "reinterpret" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 319: /* $@19: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 320: /* $@20: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 321: /* expr_type_decl: "type" '<' $@19 type_declaration '>' $@20  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 322: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 323: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 324: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" "end of expression" "name" '>' expr ')'  */
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

  case 325: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 326: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 327: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" "end of expression" "name" '>' '(' expr ')'  */
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

  case 328: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 329: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 330: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 331: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 332: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 333: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 334: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 335: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 336: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 337: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 338: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 339: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 340: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 341: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 342: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 343: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 344: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 345: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 346: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 347: /* $@21: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 348: /* expr_full_block_assumed_piped: block_or_lambda $@21 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 349: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 350: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 351: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 352: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 353: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 354: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 355: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 356: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 357: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 358: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 359: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 360: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 361: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 362: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 363: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 364: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 365: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 366: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 367: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 368: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 369: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 370: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 371: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 372: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 376: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 377: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 378: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 379: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 392: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 393: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 394: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 395: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 396: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 397: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 398: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 399: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 400: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 401: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 402: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 403: /* $@22: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 404: /* $@23: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 405: /* func_addr_expr: '@' '@' '<' $@22 type_declaration_no_options '>' $@23 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 406: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 407: /* $@25: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 408: /* func_addr_expr: '@' '@' '<' $@24 optional_function_argument_list optional_function_type '>' $@25 func_addr_name  */
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

  case 409: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 410: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 411: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 412: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 413: /* $@26: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 414: /* $@27: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 415: /* expr_field: expr '.' $@26 error $@27  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 416: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 417: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 418: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 419: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 420: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 421: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 422: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 423: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 424: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 425: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 426: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 427: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 428: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 429: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 430: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 431: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 432: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 433: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 434: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 435: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 436: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 437: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 438: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 439: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 440: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 441: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 442: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 443: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 444: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 445: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 446: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 447: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 448: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 449: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 450: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 451: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 452: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 453: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 454: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 455: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 456: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 457: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 458: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 459: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 460: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 461: /* expr: '(' expr_list optional_comma ')'  */
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

  case 462: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 463: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 464: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 465: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 466: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 467: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 468: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 469: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 470: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 471: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 472: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 473: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 474: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 475: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 476: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 477: /* $@28: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 478: /* $@29: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 479: /* expr: expr "is" "type" '<' $@28 type_declaration_no_options '>' $@29  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 480: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 481: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 482: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 483: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 484: /* $@31: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 485: /* expr: expr "as" "type" '<' $@30 type_declaration '>' $@31  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 486: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 487: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 488: /* $@32: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 489: /* $@33: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 490: /* expr: expr '?' "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 491: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 492: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 493: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 494: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 495: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 496: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 497: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 498: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 499: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 500: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 501: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 502: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 503: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 504: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 505: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 506: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 507: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 508: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 509: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 510: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 511: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 512: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 513: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 514: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 515: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 516: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 517: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 518: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 519: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 520: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 521: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 522: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); /*this one is gone when BRABRA is disabled*/ }
    break;

  case 523: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 524: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 525: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 526: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 527: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 528: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 529: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 530: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 531: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 532: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 533: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 534: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 535: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 536: /* $@34: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 537: /* struct_variable_declaration_list: struct_variable_declaration_list $@34 structure_variable_declaration "end of expression"  */
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

  case 538: /* $@35: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 539: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@35 function_declaration_header "end of expression"  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 540: /* $@36: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 541: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@36 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 542: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' "end of expression"  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 543: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 544: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 545: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 546: /* function_argument_list: function_argument_list "end of expression" function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 547: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 548: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 549: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 550: /* tuple_type_list: tuple_type_list "end of expression" tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 551: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 552: /* tuple_alias_type_list: tuple_alias_type_list "end of expression"  */
                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 553: /* tuple_alias_type_list: tuple_alias_type_list tuple_type "end of expression"  */
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

  case 554: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 555: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 556: /* variant_type_list: variant_type_list "end of expression" variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 557: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 558: /* variant_alias_type_list: variant_alias_type_list "end of expression"  */
                                        {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 559: /* variant_alias_type_list: variant_alias_type_list variant_type "end of expression"  */
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

  case 560: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 561: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 562: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 563: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 564: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 565: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 566: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 567: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 568: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 569: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 570: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 571: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 572: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 573: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 574: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 575: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 576: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 577: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 578: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options "end of expression"  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 579: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 580: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 581: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 582: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 583: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 584: /* $@37: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 585: /* global_variable_declaration_list: global_variable_declaration_list $@37 optional_field_annotation let_variable_declaration  */
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

  case 586: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 587: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 588: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 589: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 590: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 591: /* global_let: kwd_let optional_shared optional_public_or_private_variable "begin of code block" global_variable_declaration_list "end of code block"  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 592: /* $@38: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 593: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@38 optional_field_annotation let_variable_declaration  */
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

  case 594: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 595: /* enum_list: enum_list "end of expression"  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 596: /* enum_list: enum_list "name" "end of expression"  */
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

  case 597: /* enum_list: enum_list "name" '=' expr "end of expression"  */
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

  case 598: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 599: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 600: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 601: /* $@39: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 602: /* single_alias: optional_public_or_private_alias "name" $@39 '=' type_declaration  */
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

  case 606: /* $@40: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 608: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 609: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 610: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 611: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 612: /* $@41: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 613: /* $@42: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 614: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name "begin of code block" $@41 enum_list $@42 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 615: /* $@43: %empty  */
                                                                                                                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 616: /* $@44: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 617: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration "begin of code block" $@43 enum_list $@44 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 618: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 619: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 620: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 621: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 622: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 623: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 624: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 625: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 626: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 627: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 628: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 629: /* optional_struct_variable_declaration_list: "begin of code block" struct_variable_declaration_list "end of code block"  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 630: /* $@45: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 631: /* $@46: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 632: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@45 structure_name $@46 optional_struct_variable_declaration_list  */
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

  case 633: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 634: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 635: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 636: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 637: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 638: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 639: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 640: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 641: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 642: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 643: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 644: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 645: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 646: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 647: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 648: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 649: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 650: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 651: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 652: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 653: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 654: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 655: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 656: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 657: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 658: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 659: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 660: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 661: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 662: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 663: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 664: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 665: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 666: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 667: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 668: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 669: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 670: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 671: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 672: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 673: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 674: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 675: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 676: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 677: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 678: /* bitfield_bits: bitfield_bits "end of expression" "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 679: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 680: /* bitfield_alias_bits: bitfield_alias_bits "end of expression"  */
                                      {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 681: /* bitfield_alias_bits: bitfield_alias_bits "name" "end of expression"  */
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

  case 682: /* $@47: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 683: /* $@48: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 684: /* bitfield_type_declaration: "bitfield" '<' $@47 bitfield_bits '>' $@48  */
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

  case 685: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 686: /* table_type_pair: type_declaration "end of expression" type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 687: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 688: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 689: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 690: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 691: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 692: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 693: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 694: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 695: /* $@49: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 696: /* $@50: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 697: /* type_declaration_no_options: "type" '<' $@49 type_declaration '>' $@50  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 698: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 699: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 700: /* $@51: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 701: /* type_declaration_no_options: '$' name_in_namespace '<' $@51 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 702: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 703: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 704: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 705: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 706: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 707: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 708: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 709: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 710: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 711: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 712: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 713: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 714: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 715: /* $@53: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 716: /* type_declaration_no_options: "smart_ptr" '<' $@52 type_declaration '>' $@53  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 717: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 718: /* $@54: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 719: /* $@55: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 720: /* type_declaration_no_options: "array" '<' $@54 type_declaration '>' $@55  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 721: /* $@56: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 722: /* $@57: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 723: /* type_declaration_no_options: "table" '<' $@56 table_type_pair '>' $@57  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 724: /* $@58: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 725: /* $@59: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 726: /* type_declaration_no_options: "iterator" '<' $@58 type_declaration '>' $@59  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 727: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 728: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 729: /* $@61: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 730: /* type_declaration_no_options: "block" '<' $@60 type_declaration '>' $@61  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 731: /* $@62: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 732: /* $@63: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 733: /* type_declaration_no_options: "block" '<' $@62 optional_function_argument_list optional_function_type '>' $@63  */
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

  case 734: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 735: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 736: /* $@65: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 737: /* type_declaration_no_options: "function" '<' $@64 type_declaration '>' $@65  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 738: /* $@66: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 739: /* $@67: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 740: /* type_declaration_no_options: "function" '<' $@66 optional_function_argument_list optional_function_type '>' $@67  */
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

  case 741: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 742: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 743: /* $@69: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 744: /* type_declaration_no_options: "lambda" '<' $@68 type_declaration '>' $@69  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 745: /* $@70: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 746: /* $@71: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 747: /* type_declaration_no_options: "lambda" '<' $@70 optional_function_argument_list optional_function_type '>' $@71  */
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

  case 748: /* $@72: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 749: /* $@73: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 750: /* type_declaration_no_options: "tuple" '<' $@72 tuple_type_list '>' $@73  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 751: /* $@74: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 752: /* $@75: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 753: /* type_declaration_no_options: "variant" '<' $@74 variant_type_list '>' $@75  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 754: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 755: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 756: /* type_declaration: type_declaration '|' '#'  */
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

  case 757: /* $@76: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 758: /* $@77: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 759: /* $@78: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 760: /* $@79: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 761: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@76 "name" $@77 "begin of code block" $@78 tuple_alias_type_list $@79 "end of code block"  */
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

  case 762: /* $@80: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 763: /* $@81: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 764: /* $@82: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 765: /* $@83: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 766: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@80 "name" $@81 "begin of code block" $@82 variant_alias_type_list $@83 "end of code block"  */
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

  case 767: /* $@84: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 768: /* $@85: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 769: /* $@86: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 770: /* $@87: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 771: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@84 "name" $@85 "begin of code block" $@86 bitfield_alias_bits $@87 "end of code block"  */
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

  case 772: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 773: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 774: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 775: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 776: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 777: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 778: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 779: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 780: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 781: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 782: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 783: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 784: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 785: /* make_variant_dim: make_struct_fields  */
                                {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 786: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 787: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 788: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 789: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 790: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 791: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 792: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 793: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 794: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 807: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 808: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 809: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 810: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 811: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 812: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 813: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 814: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 815: /* $@88: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 816: /* $@89: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 817: /* make_struct_decl: "struct" '<' $@88 type_declaration_no_options '>' $@89 '(' use_initializer make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                   {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 818: /* $@90: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 819: /* $@91: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 820: /* make_struct_decl: "class" '<' $@90 type_declaration_no_options '>' $@91 '(' use_initializer make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                 {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 821: /* $@92: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 822: /* $@93: %empty  */
                                                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 823: /* make_struct_decl: "variant" '<' $@92 type_declaration_no_options '>' $@93 '(' make_variant_dim ')'  */
                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 824: /* $@94: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 825: /* $@95: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 826: /* make_struct_decl: "default" '<' $@94 type_declaration_no_options '>' $@95 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 827: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 828: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 829: /* make_tuple: make_tuple ',' expr  */
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

  case 830: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 831: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 832: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 833: /* $@96: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 834: /* $@97: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 835: /* make_tuple_call: "tuple" '<' $@96 type_declaration_no_options '>' $@97 '(' use_initializer make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                   {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 836: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 837: /* make_dim: make_dim "end of expression" make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 838: /* make_dim_decl: '[' expr_list optional_comma ']'  */
                                                          {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 839: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 840: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 841: /* $@98: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 842: /* $@99: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 843: /* make_dim_decl: "array" "struct" '<' $@98 type_declaration_no_options '>' $@99 '(' use_initializer make_struct_dim_decl ')'  */
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

  case 844: /* $@100: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 845: /* $@101: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 846: /* make_dim_decl: "array" "tuple" '<' $@100 type_declaration_no_options '>' $@101 '(' use_initializer make_struct_dim_decl ')'  */
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

  case 847: /* $@102: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 848: /* $@103: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 849: /* make_dim_decl: "array" "variant" '<' $@102 type_declaration_no_options '>' $@103 '(' make_variant_dim ')'  */
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

  case 850: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 851: /* $@104: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 852: /* $@105: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 853: /* make_dim_decl: "array" '<' $@104 type_declaration_no_options '>' $@105 '(' optional_expr_list ')'  */
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

  case 854: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 855: /* $@106: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 856: /* $@107: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 857: /* make_dim_decl: "fixed_array" '<' $@106 type_declaration_no_options '>' $@107 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        if ( !mka->makeType->dim.size() ) mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 858: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 859: /* make_table: make_table "end of expression" make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 860: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 861: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 862: /* make_table_decl: "begin of code block" expr_map_tuple_list optional_comma "end of code block"  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 863: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 864: /* make_table_decl: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 865: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
                                                                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-7])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-7])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 866: /* make_table_decl: "table" '<' type_declaration_no_options "end of expression" type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
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

  case 867: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 868: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 869: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 870: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 871: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 872: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 873: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 874: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 875: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 876: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block" "end of code block"  */
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


