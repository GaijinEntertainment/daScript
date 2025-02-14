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
  YYSYMBOL_INTEGER = 163,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 164,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 165,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 166,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 167,            /* "unsigned int8 constant"  */
  YYSYMBOL_FLOAT = 168,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 169,                   /* "double constant"  */
  YYSYMBOL_NAME = 170,                     /* "name"  */
  YYSYMBOL_KEYWORD = 171,                  /* "keyword"  */
  YYSYMBOL_TYPE_FUNCTION = 172,            /* "type function"  */
  YYSYMBOL_BEGIN_STRING = 173,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 174,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 175,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 176,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 177,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 178,          /* "}"  */
  YYSYMBOL_END_OF_READ = 179,              /* "end of failed eader macro"  */
  YYSYMBOL_180_begin_of_code_block_ = 180, /* "begin of code block"  */
  YYSYMBOL_181_end_of_code_block_ = 181,   /* "end of code block"  */
  YYSYMBOL_182_end_of_expression_ = 182,   /* "end of expression"  */
  YYSYMBOL_SEMICOLON_CUR_CUR = 183,        /* ";}}"  */
  YYSYMBOL_SEMICOLON_CUR_SQR = 184,        /* ";}]"  */
  YYSYMBOL_SEMICOLON_SQR_SQR = 185,        /* ";]]"  */
  YYSYMBOL_COMMA_SQR_SQR = 186,            /* ",]]"  */
  YYSYMBOL_COMMA_CUR_SQR = 187,            /* ",}]"  */
  YYSYMBOL_188_ = 188,                     /* ','  */
  YYSYMBOL_189_ = 189,                     /* '='  */
  YYSYMBOL_190_ = 190,                     /* '?'  */
  YYSYMBOL_191_ = 191,                     /* ':'  */
  YYSYMBOL_192_ = 192,                     /* '|'  */
  YYSYMBOL_193_ = 193,                     /* '^'  */
  YYSYMBOL_194_ = 194,                     /* '&'  */
  YYSYMBOL_195_ = 195,                     /* '<'  */
  YYSYMBOL_196_ = 196,                     /* '>'  */
  YYSYMBOL_197_ = 197,                     /* '-'  */
  YYSYMBOL_198_ = 198,                     /* '+'  */
  YYSYMBOL_199_ = 199,                     /* '*'  */
  YYSYMBOL_200_ = 200,                     /* '/'  */
  YYSYMBOL_201_ = 201,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 202,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 203,               /* UNARY_PLUS  */
  YYSYMBOL_204_ = 204,                     /* '~'  */
  YYSYMBOL_205_ = 205,                     /* '!'  */
  YYSYMBOL_PRE_INC = 206,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 207,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 208,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 209,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 210,                    /* DEREF  */
  YYSYMBOL_211_ = 211,                     /* '.'  */
  YYSYMBOL_212_ = 212,                     /* '['  */
  YYSYMBOL_213_ = 213,                     /* ']'  */
  YYSYMBOL_214_ = 214,                     /* '('  */
  YYSYMBOL_215_ = 215,                     /* ')'  */
  YYSYMBOL_216_ = 216,                     /* '$'  */
  YYSYMBOL_217_ = 217,                     /* '@'  */
  YYSYMBOL_218_ = 218,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 219,                 /* $accept  */
  YYSYMBOL_program = 220,                  /* program  */
  YYSYMBOL_top_level_reader_macro = 221,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 222, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 223,              /* module_name  */
  YYSYMBOL_module_declaration = 224,       /* module_declaration  */
  YYSYMBOL_character_sequence = 225,       /* character_sequence  */
  YYSYMBOL_string_constant = 226,          /* string_constant  */
  YYSYMBOL_format_string = 227,            /* format_string  */
  YYSYMBOL_optional_format_string = 228,   /* optional_format_string  */
  YYSYMBOL_229_1 = 229,                    /* $@1  */
  YYSYMBOL_string_builder_body = 230,      /* string_builder_body  */
  YYSYMBOL_string_builder = 231,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 232, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 233,              /* expr_reader  */
  YYSYMBOL_234_2 = 234,                    /* $@2  */
  YYSYMBOL_options_declaration = 235,      /* options_declaration  */
  YYSYMBOL_require_declaration = 236,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 237,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 238,      /* require_module_name  */
  YYSYMBOL_require_module = 239,           /* require_module  */
  YYSYMBOL_is_public_module = 240,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 241,       /* expect_declaration  */
  YYSYMBOL_expect_list = 242,              /* expect_list  */
  YYSYMBOL_expect_error = 243,             /* expect_error  */
  YYSYMBOL_expression_label = 244,         /* expression_label  */
  YYSYMBOL_expression_goto = 245,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 246,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 247,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 248,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 249, /* expression_else_one_liner  */
  YYSYMBOL_250_3 = 250,                    /* $@3  */
  YYSYMBOL_expression_if_one_liner = 251,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 252,  /* expression_if_then_else  */
  YYSYMBOL_253_4 = 253,                    /* $@4  */
  YYSYMBOL_expression_for_loop = 254,      /* expression_for_loop  */
  YYSYMBOL_255_5 = 255,                    /* $@5  */
  YYSYMBOL_expression_unsafe = 256,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 257,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 258,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 259,    /* expression_with_alias  */
  YYSYMBOL_260_6 = 260,                    /* $@6  */
  YYSYMBOL_annotation_argument_value = 261, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 262, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 263, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 264,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 265, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 266,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 267, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 268, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 269,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 270,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 271, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 272, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 273,   /* optional_function_type  */
  YYSYMBOL_function_name = 274,            /* function_name  */
  YYSYMBOL_global_function_declaration = 275, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 276, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 277, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 278,     /* function_declaration  */
  YYSYMBOL_279_7 = 279,                    /* $@7  */
  YYSYMBOL_expression_block = 280,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 281,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 282,           /* expression_any  */
  YYSYMBOL_expressions = 283,              /* expressions  */
  YYSYMBOL_expr_keyword = 284,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 285,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 286, /* optional_expr_list_in_braces  */
  YYSYMBOL_optional_expr_map_tuple_list = 287, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 288, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 289,       /* expression_keyword  */
  YYSYMBOL_290_8 = 290,                    /* $@8  */
  YYSYMBOL_291_9 = 291,                    /* $@9  */
  YYSYMBOL_292_10 = 292,                   /* $@10  */
  YYSYMBOL_293_11 = 293,                   /* $@11  */
  YYSYMBOL_expr_pipe = 294,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 295,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 296,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 297,     /* new_type_declaration  */
  YYSYMBOL_298_12 = 298,                   /* $@12  */
  YYSYMBOL_299_13 = 299,                   /* $@13  */
  YYSYMBOL_expr_new = 300,                 /* expr_new  */
  YYSYMBOL_expression_break = 301,         /* expression_break  */
  YYSYMBOL_expression_continue = 302,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 303, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 304,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 305, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 306,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 307,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 308,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 309,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 310,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 311,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 312, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 313,           /* expression_let  */
  YYSYMBOL_expr_cast = 314,                /* expr_cast  */
  YYSYMBOL_315_14 = 315,                   /* $@14  */
  YYSYMBOL_316_15 = 316,                   /* $@15  */
  YYSYMBOL_317_16 = 317,                   /* $@16  */
  YYSYMBOL_318_17 = 318,                   /* $@17  */
  YYSYMBOL_319_18 = 319,                   /* $@18  */
  YYSYMBOL_320_19 = 320,                   /* $@19  */
  YYSYMBOL_expr_type_decl = 321,           /* expr_type_decl  */
  YYSYMBOL_322_20 = 322,                   /* $@20  */
  YYSYMBOL_323_21 = 323,                   /* $@21  */
  YYSYMBOL_expr_type_info = 324,           /* expr_type_info  */
  YYSYMBOL_expr_list = 325,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 326,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 327,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 328,            /* capture_entry  */
  YYSYMBOL_capture_list = 329,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 330,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 331,               /* expr_block  */
  YYSYMBOL_expr_full_block = 332,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 333, /* expr_full_block_assumed_piped  */
  YYSYMBOL_334_22 = 334,                   /* $@22  */
  YYSYMBOL_expr_numeric_const = 335,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 336,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 337,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 338,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 339,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 340,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 341,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 342,           /* func_addr_expr  */
  YYSYMBOL_343_23 = 343,                   /* $@23  */
  YYSYMBOL_344_24 = 344,                   /* $@24  */
  YYSYMBOL_345_25 = 345,                   /* $@25  */
  YYSYMBOL_346_26 = 346,                   /* $@26  */
  YYSYMBOL_expr_field = 347,               /* expr_field  */
  YYSYMBOL_348_27 = 348,                   /* $@27  */
  YYSYMBOL_349_28 = 349,                   /* $@28  */
  YYSYMBOL_expr_call = 350,                /* expr_call  */
  YYSYMBOL_expr = 351,                     /* expr  */
  YYSYMBOL_352_29 = 352,                   /* $@29  */
  YYSYMBOL_353_30 = 353,                   /* $@30  */
  YYSYMBOL_354_31 = 354,                   /* $@31  */
  YYSYMBOL_355_32 = 355,                   /* $@32  */
  YYSYMBOL_356_33 = 356,                   /* $@33  */
  YYSYMBOL_357_34 = 357,                   /* $@34  */
  YYSYMBOL_expr_mtag = 358,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 359, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 360,        /* optional_override  */
  YYSYMBOL_optional_constant = 361,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 362, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 363, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 364, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 365, /* struct_variable_declaration_list  */
  YYSYMBOL_366_35 = 366,                   /* $@35  */
  YYSYMBOL_367_36 = 367,                   /* $@36  */
  YYSYMBOL_368_37 = 368,                   /* $@37  */
  YYSYMBOL_function_argument_declaration = 369, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 370,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 371,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 372,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 373,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 374,             /* variant_type  */
  YYSYMBOL_variant_type_list = 375,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 376,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 377,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 378,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 379,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 380,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 381, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 382, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 383, /* global_variable_declaration_list  */
  YYSYMBOL_384_38 = 384,                   /* $@38  */
  YYSYMBOL_optional_shared = 385,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 386, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 387,               /* global_let  */
  YYSYMBOL_388_39 = 388,                   /* $@39  */
  YYSYMBOL_enum_list = 389,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 390, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 391,             /* single_alias  */
  YYSYMBOL_392_40 = 392,                   /* $@40  */
  YYSYMBOL_alias_list = 393,               /* alias_list  */
  YYSYMBOL_alias_declaration = 394,        /* alias_declaration  */
  YYSYMBOL_395_41 = 395,                   /* $@41  */
  YYSYMBOL_optional_public_or_private_enum = 396, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 397,                /* enum_name  */
  YYSYMBOL_enum_declaration = 398,         /* enum_declaration  */
  YYSYMBOL_399_42 = 399,                   /* $@42  */
  YYSYMBOL_400_43 = 400,                   /* $@43  */
  YYSYMBOL_401_44 = 401,                   /* $@44  */
  YYSYMBOL_402_45 = 402,                   /* $@45  */
  YYSYMBOL_optional_structure_parent = 403, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 404,          /* optional_sealed  */
  YYSYMBOL_structure_name = 405,           /* structure_name  */
  YYSYMBOL_class_or_struct = 406,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 407, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 408, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 409,    /* structure_declaration  */
  YYSYMBOL_410_46 = 410,                   /* $@46  */
  YYSYMBOL_411_47 = 411,                   /* $@47  */
  YYSYMBOL_variable_name_with_pos_list = 412, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 413,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 414, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 415, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 416,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 417,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 418,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 419, /* bitfield_type_declaration  */
  YYSYMBOL_420_48 = 420,                   /* $@48  */
  YYSYMBOL_421_49 = 421,                   /* $@49  */
  YYSYMBOL_c_or_s = 422,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 423,          /* table_type_pair  */
  YYSYMBOL_dim_list = 424,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 425, /* type_declaration_no_options  */
  YYSYMBOL_426_50 = 426,                   /* $@50  */
  YYSYMBOL_427_51 = 427,                   /* $@51  */
  YYSYMBOL_428_52 = 428,                   /* $@52  */
  YYSYMBOL_429_53 = 429,                   /* $@53  */
  YYSYMBOL_430_54 = 430,                   /* $@54  */
  YYSYMBOL_431_55 = 431,                   /* $@55  */
  YYSYMBOL_432_56 = 432,                   /* $@56  */
  YYSYMBOL_433_57 = 433,                   /* $@57  */
  YYSYMBOL_434_58 = 434,                   /* $@58  */
  YYSYMBOL_435_59 = 435,                   /* $@59  */
  YYSYMBOL_436_60 = 436,                   /* $@60  */
  YYSYMBOL_437_61 = 437,                   /* $@61  */
  YYSYMBOL_438_62 = 438,                   /* $@62  */
  YYSYMBOL_439_63 = 439,                   /* $@63  */
  YYSYMBOL_440_64 = 440,                   /* $@64  */
  YYSYMBOL_441_65 = 441,                   /* $@65  */
  YYSYMBOL_442_66 = 442,                   /* $@66  */
  YYSYMBOL_443_67 = 443,                   /* $@67  */
  YYSYMBOL_444_68 = 444,                   /* $@68  */
  YYSYMBOL_445_69 = 445,                   /* $@69  */
  YYSYMBOL_446_70 = 446,                   /* $@70  */
  YYSYMBOL_447_71 = 447,                   /* $@71  */
  YYSYMBOL_448_72 = 448,                   /* $@72  */
  YYSYMBOL_449_73 = 449,                   /* $@73  */
  YYSYMBOL_450_74 = 450,                   /* $@74  */
  YYSYMBOL_451_75 = 451,                   /* $@75  */
  YYSYMBOL_452_76 = 452,                   /* $@76  */
  YYSYMBOL_type_declaration = 453,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 454,  /* tuple_alias_declaration  */
  YYSYMBOL_455_77 = 455,                   /* $@77  */
  YYSYMBOL_456_78 = 456,                   /* $@78  */
  YYSYMBOL_457_79 = 457,                   /* $@79  */
  YYSYMBOL_458_80 = 458,                   /* $@80  */
  YYSYMBOL_variant_alias_declaration = 459, /* variant_alias_declaration  */
  YYSYMBOL_460_81 = 460,                   /* $@81  */
  YYSYMBOL_461_82 = 461,                   /* $@82  */
  YYSYMBOL_462_83 = 462,                   /* $@83  */
  YYSYMBOL_463_84 = 463,                   /* $@84  */
  YYSYMBOL_bitfield_alias_declaration = 464, /* bitfield_alias_declaration  */
  YYSYMBOL_465_85 = 465,                   /* $@85  */
  YYSYMBOL_466_86 = 466,                   /* $@86  */
  YYSYMBOL_467_87 = 467,                   /* $@87  */
  YYSYMBOL_468_88 = 468,                   /* $@88  */
  YYSYMBOL_make_decl = 469,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 470,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 471,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 472,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 473,          /* make_struct_dim  */
  YYSYMBOL_make_struct_dim_list = 474,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 475,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 476, /* optional_make_struct_dim_decl  */
  YYSYMBOL_optional_block = 477,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 478, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 479, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 480, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 481, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 482, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_use_initializer = 483,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 484,         /* make_struct_decl  */
  YYSYMBOL_485_89 = 485,                   /* $@89  */
  YYSYMBOL_486_90 = 486,                   /* $@90  */
  YYSYMBOL_487_91 = 487,                   /* $@91  */
  YYSYMBOL_488_92 = 488,                   /* $@92  */
  YYSYMBOL_489_93 = 489,                   /* $@93  */
  YYSYMBOL_490_94 = 490,                   /* $@94  */
  YYSYMBOL_491_95 = 491,                   /* $@95  */
  YYSYMBOL_492_96 = 492,                   /* $@96  */
  YYSYMBOL_make_tuple = 493,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 494,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 495,          /* make_tuple_call  */
  YYSYMBOL_496_97 = 496,                   /* $@97  */
  YYSYMBOL_497_98 = 497,                   /* $@98  */
  YYSYMBOL_make_dim = 498,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 499,            /* make_dim_decl  */
  YYSYMBOL_500_99 = 500,                   /* $@99  */
  YYSYMBOL_501_100 = 501,                  /* $@100  */
  YYSYMBOL_502_101 = 502,                  /* $@101  */
  YYSYMBOL_503_102 = 503,                  /* $@102  */
  YYSYMBOL_504_103 = 504,                  /* $@103  */
  YYSYMBOL_505_104 = 505,                  /* $@104  */
  YYSYMBOL_506_105 = 506,                  /* $@105  */
  YYSYMBOL_507_106 = 507,                  /* $@106  */
  YYSYMBOL_508_107 = 508,                  /* $@107  */
  YYSYMBOL_509_108 = 509,                  /* $@108  */
  YYSYMBOL_make_table = 510,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 511,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 512,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 513, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 514,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 515       /* array_comprehension  */
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
#define YYLAST   15897

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  219
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  297
/* YYNRULES -- Number of rules.  */
#define YYNRULES  905
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1708

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   446


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
       2,     2,     2,   205,     2,   218,   216,   201,   194,     2,
     214,   215,   199,   198,   188,   197,   211,   200,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   191,   182,
     195,   189,   196,   190,   217,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   212,     2,   213,   193,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   180,   192,   181,   204,     2,     2,     2,
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
     175,   176,   177,   178,   179,   183,   184,   185,   186,   187,
     202,   203,   206,   207,   208,   209,   210
};

#if DAS_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   555,   555,   556,   561,   562,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,   573,   577,   583,   584,
     585,   589,   590,   594,   612,   613,   614,   615,   619,   620,
     624,   625,   629,   630,   630,   634,   639,   648,   663,   679,
     684,   692,   692,   731,   761,   765,   766,   767,   771,   774,
     778,   784,   793,   796,   802,   803,   807,   811,   812,   816,
     819,   825,   831,   834,   840,   841,   845,   846,   847,   856,
     857,   861,   862,   862,   868,   869,   870,   871,   872,   876,
     882,   882,   888,   888,   894,   902,   912,   921,   921,   928,
     929,   930,   931,   932,   933,   937,   942,   950,   951,   952,
     956,   957,   958,   959,   960,   961,   962,   963,   969,   972,
     978,   981,   984,   990,   991,   992,   993,   997,  1010,  1028,
    1031,  1039,  1050,  1061,  1072,  1075,  1082,  1086,  1093,  1094,
    1098,  1099,  1100,  1104,  1107,  1114,  1118,  1119,  1120,  1121,
    1122,  1123,  1124,  1125,  1126,  1127,  1128,  1129,  1130,  1131,
    1132,  1133,  1134,  1135,  1136,  1137,  1138,  1139,  1140,  1141,
    1142,  1143,  1144,  1145,  1146,  1147,  1148,  1149,  1150,  1151,
    1152,  1153,  1154,  1155,  1156,  1157,  1158,  1159,  1160,  1161,
    1162,  1163,  1164,  1165,  1166,  1167,  1168,  1169,  1170,  1171,
    1172,  1173,  1174,  1175,  1176,  1177,  1178,  1179,  1180,  1181,
    1182,  1183,  1184,  1185,  1186,  1187,  1188,  1189,  1190,  1191,
    1192,  1193,  1194,  1195,  1196,  1197,  1198,  1199,  1200,  1201,
    1206,  1224,  1225,  1226,  1230,  1236,  1236,  1253,  1257,  1268,
    1277,  1286,  1292,  1293,  1294,  1295,  1296,  1297,  1298,  1299,
    1300,  1301,  1302,  1303,  1304,  1305,  1306,  1307,  1308,  1309,
    1310,  1311,  1312,  1316,  1321,  1327,  1333,  1344,  1345,  1349,
    1350,  1354,  1355,  1359,  1363,  1370,  1370,  1370,  1376,  1376,
    1376,  1385,  1419,  1422,  1425,  1428,  1434,  1435,  1446,  1450,
    1453,  1461,  1461,  1461,  1464,  1470,  1473,  1477,  1481,  1488,
    1495,  1501,  1505,  1509,  1512,  1515,  1523,  1526,  1529,  1537,
    1540,  1548,  1551,  1554,  1562,  1568,  1569,  1570,  1574,  1575,
    1579,  1580,  1584,  1589,  1597,  1603,  1609,  1615,  1621,  1630,
    1639,  1648,  1660,  1663,  1669,  1669,  1669,  1672,  1672,  1672,
    1677,  1677,  1677,  1685,  1685,  1685,  1691,  1701,  1712,  1725,
    1735,  1746,  1761,  1764,  1770,  1771,  1778,  1789,  1790,  1791,
    1795,  1796,  1797,  1798,  1799,  1803,  1808,  1816,  1817,  1818,
    1822,  1827,  1834,  1841,  1841,  1850,  1851,  1852,  1853,  1854,
    1855,  1856,  1860,  1861,  1862,  1863,  1864,  1865,  1866,  1867,
    1868,  1869,  1870,  1871,  1872,  1873,  1874,  1875,  1876,  1877,
    1878,  1882,  1883,  1884,  1885,  1890,  1891,  1892,  1893,  1894,
    1895,  1896,  1897,  1898,  1899,  1900,  1901,  1902,  1903,  1904,
    1905,  1906,  1911,  1918,  1930,  1935,  1945,  1949,  1956,  1959,
    1959,  1959,  1964,  1964,  1964,  1977,  1981,  1985,  1990,  1997,
    2006,  2011,  2018,  2018,  2018,  2025,  2029,  2038,  2046,  2054,
    2058,  2061,  2067,  2068,  2069,  2070,  2071,  2072,  2073,  2074,
    2075,  2076,  2077,  2078,  2079,  2080,  2081,  2082,  2083,  2084,
    2085,  2086,  2087,  2088,  2089,  2090,  2091,  2092,  2093,  2094,
    2095,  2096,  2097,  2098,  2099,  2100,  2101,  2102,  2108,  2109,
    2110,  2111,  2112,  2125,  2134,  2135,  2136,  2137,  2138,  2139,
    2140,  2141,  2142,  2143,  2144,  2145,  2148,  2151,  2152,  2155,
    2155,  2155,  2158,  2163,  2167,  2171,  2171,  2171,  2176,  2179,
    2183,  2183,  2183,  2188,  2191,  2192,  2193,  2194,  2195,  2196,
    2197,  2198,  2199,  2201,  2205,  2206,  2211,  2215,  2216,  2217,
    2218,  2219,  2220,  2221,  2225,  2229,  2233,  2237,  2241,  2245,
    2249,  2253,  2257,  2264,  2265,  2266,  2270,  2271,  2272,  2276,
    2277,  2281,  2282,  2283,  2287,  2288,  2292,  2303,  2306,  2309,
    2309,  2328,  2327,  2343,  2342,  2356,  2365,  2374,  2384,  2385,
    2389,  2392,  2401,  2402,  2406,  2409,  2412,  2428,  2437,  2438,
    2442,  2445,  2448,  2462,  2463,  2467,  2473,  2479,  2482,  2486,
    2492,  2501,  2502,  2503,  2507,  2508,  2512,  2519,  2524,  2533,
    2539,  2550,  2553,  2558,  2563,  2571,  2582,  2585,  2585,  2605,
    2606,  2610,  2611,  2612,  2616,  2619,  2619,  2638,  2641,  2644,
    2659,  2678,  2679,  2680,  2685,  2685,  2711,  2712,  2716,  2717,
    2717,  2721,  2722,  2723,  2727,  2737,  2742,  2737,  2754,  2759,
    2754,  2774,  2775,  2779,  2780,  2784,  2790,  2791,  2792,  2793,
    2797,  2798,  2799,  2803,  2806,  2812,  2817,  2812,  2837,  2844,
    2849,  2858,  2864,  2875,  2876,  2877,  2878,  2879,  2880,  2881,
    2882,  2883,  2884,  2885,  2886,  2887,  2888,  2889,  2890,  2891,
    2892,  2893,  2894,  2895,  2896,  2897,  2898,  2899,  2900,  2901,
    2905,  2906,  2907,  2908,  2909,  2910,  2911,  2912,  2916,  2927,
    2931,  2938,  2950,  2957,  2966,  2971,  2974,  2987,  2987,  2987,
    3000,  3001,  3005,  3009,  3016,  3020,  3027,  3028,  3029,  3030,
    3031,  3046,  3052,  3052,  3052,  3056,  3061,  3068,  3068,  3075,
    3079,  3083,  3088,  3093,  3098,  3103,  3107,  3111,  3116,  3120,
    3124,  3129,  3129,  3129,  3135,  3142,  3142,  3142,  3147,  3147,
    3147,  3153,  3153,  3153,  3158,  3162,  3162,  3162,  3167,  3167,
    3167,  3176,  3180,  3180,  3180,  3185,  3185,  3185,  3194,  3198,
    3198,  3198,  3203,  3203,  3203,  3212,  3212,  3212,  3218,  3218,
    3218,  3227,  3230,  3241,  3257,  3257,  3262,  3267,  3257,  3292,
    3292,  3297,  3303,  3292,  3328,  3328,  3333,  3338,  3328,  3368,
    3369,  3370,  3371,  3372,  3376,  3383,  3390,  3396,  3402,  3409,
    3416,  3422,  3431,  3437,  3445,  3450,  3457,  3462,  3469,  3474,
    3480,  3481,  3485,  3486,  3491,  3492,  3496,  3497,  3501,  3502,
    3506,  3507,  3508,  3512,  3513,  3514,  3518,  3519,  3523,  3529,
    3536,  3544,  3551,  3559,  3568,  3568,  3568,  3576,  3576,  3576,
    3583,  3583,  3583,  3590,  3590,  3590,  3601,  3604,  3610,  3624,
    3630,  3636,  3642,  3642,  3642,  3652,  3657,  3664,  3683,  3688,
    3695,  3695,  3695,  3705,  3705,  3705,  3715,  3715,  3715,  3725,
    3734,  3734,  3734,  3754,  3761,  3761,  3761,  3771,  3776,  3783,
    3786,  3792,  3811,  3820,  3828,  3848,  3873,  3874,  3878,  3879,
    3884,  3887,  3890,  3893,  3896,  3899
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
  "\"[{\"", "\"{{\"", "\"integer constant\"", "\"long integer constant\"",
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
  "elif_or_static_elif", "expression_else", "if_or_static_if",
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
  "function_declaration", "$@7", "expression_block", "expr_call_pipe",
  "expression_any", "expressions", "expr_keyword", "optional_expr_list",
  "optional_expr_list_in_braces", "optional_expr_map_tuple_list",
  "type_declaration_no_options_list", "expression_keyword", "$@8", "$@9",
  "$@10", "$@11", "expr_pipe", "name_in_namespace", "expression_delete",
  "new_type_declaration", "$@12", "$@13", "expr_new", "expression_break",
  "expression_continue", "expression_return_no_pipe", "expression_return",
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

#define YYPACT_NINF (-1500)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-773)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1500,   117, -1500, -1500,    59,   -96,   214,   -42, -1500,   -98,
     581,   581,   581, -1500,   -29,   291, -1500, -1500,    68, -1500,
   -1500, -1500,   160, -1500,   239, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500, -1500, -1500, -1500,    89, -1500,   122,   124,   148,
   -1500, -1500, -1500, -1500,   214, -1500,    43, -1500,   581,   581,
   -1500, -1500,   239, -1500, -1500, -1500, -1500, -1500,   259,   283,
   -1500, -1500, -1500, -1500,   291,   291,   291,   298, -1500,   811,
     144, -1500, -1500, -1500, -1500,   681,   684,   544,   686, -1500,
     698,    41,    59,   379,   -96,   346,   397, -1500,   791,   791,
   -1500,   436,   454,    -3,   495,   709,   515,   525,   558, -1500,
     570,   574, -1500, -1500,   -55,    59,   291,   291,   291,   291,
   -1500, -1500, -1500, -1500, -1500, -1500, -1500,   586, -1500, -1500,
   -1500, -1500, -1500, -1500, -1500,   578, -1500, -1500, -1500, -1500,
   -1500,   796,   165, -1500, -1500, -1500, -1500,   712, -1500, -1500,
   -1500, -1500, -1500,   600, -1500, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500, -1500,   624, -1500,   156, -1500,   437,   669,   811,
   15727, -1500,   420,   716, -1500,   -26, -1500, -1500, -1500,   799,
   -1500, -1500, -1500, -1500, -1500, -1500, -1500,   178, -1500,   650,
   -1500,   693,   695,   696, -1500, -1500, 14240, -1500, -1500, -1500,
   -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500,   805,   816, -1500,   643,   701, -1500,   786, -1500,
     694, -1500,   711,    59,    59,    62,   101, -1500, -1500, -1500,
     165, -1500, 10323, -1500, -1500, -1500,   735,   736, -1500, -1500,
   -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500,   743,   705,
   -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500,   906, -1500, -1500, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500, -1500, -1500, -1500,   759,   719, -1500, -1500,   179,
     745, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500,   754,   746,   762, -1500,   -26,   195, -1500, -1500,
      59,   729,   910,   492, -1500, -1500,   763,   764,   772,   752,
     784,   789, -1500, -1500, -1500,   773, -1500, -1500, -1500, -1500,
   -1500,   790, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500, -1500,   793, -1500, -1500, -1500,   794,   797, -1500,
   -1500, -1500, -1500,   798,   801,   777,   -29, -1500, -1500, -1500,
   -1500, -1500,   485,   757, -1500, -1500, -1500, -1500, -1500, -1500,
   -1500,   824,   849, -1500,   783, -1500,   206, -1500,    99, 10323,
   -1500,  2278,   192, -1500,   -29, -1500, -1500, -1500,   101,   785,
   -1500,  9612,   829,   830, 10323, -1500,   -15, -1500, -1500, -1500,
    9612, -1500, -1500,   831, -1500,   344,   357,   370, -1500, -1500,
    9612,   155, -1500, -1500, -1500,    16, -1500, -1500, -1500,    33,
    5954, -1500,   800, 10077,   377, 10177,   327, -1500, -1500,  9612,
   -1500, -1500,   337,   -40, -1500,   757, -1500,   808,   814,  9612,
   -1500, -1500, -1500, -1500, -1500,   665,   -68,   815,    26,  3307,
   -1500, -1500,   701,   233,  6157,   792,  9612,   841,   823,   825,
     807, -1500,   837,   828,   862,  6360,   -71,   307,   836, -1500,
     360,   840,   843,  3510,  9612,  9612,   -61,   -61,   -61,   818,
     826,   832,   833,   834,   835, -1500,  1860,  9977,  6565, -1500,
   -1500, -1500, -1500, -1500, -1500, -1500,  6768,   844, -1500,  6973,
    1005, -1500,  9612,  9612,  9612,  9612,  9612,  5142,  7176, -1500,
     827, -1500, -1500,   863,   868,  9612,  1043, -1500, -1500, -1500,
   -1500, -1500, -1500, -1500, -1500, -1500,   226, -1500,   -74,   872,
   -1500,   874,   875,   876, -1500,   877, -1500, -1500,   994, -1500,
   -1500, -1500, -1500,   851, -1500, -1500,   -45, -1500, -1500, -1500,
   -1500, -1500, -1500,  1544, -1500,   850, -1500, -1500, -1500, -1500,
   -1500, -1500,   -57, -1500,   886, -1500, -1500,    25, -1500, -1500,
     854,   878,   881, -1500, 11209, -1500,  1026,   947, -1500, -1500,
   -1500,  3916, 10323, 10323, 10323, 11309, 10323, 10323,   855,   907,
   10323,   643, 10323,   643, 10323,   643, 10423,   912, 11409, -1500,
    9612, -1500, -1500, -1500, -1500,   870, -1500, -1500, 13759,  9612,
   -1500,   485,   904, -1500,   908,   -38, -1500, -1500,   588, -1500,
     757,   909,   896,   588, -1500,   913, 11444,   879,  1045, -1500,
     -81, -1500, -1500, -1500, 14275,   337, -1500,   889, -1500, -1500,
     -29,   365, -1500,   905,   916,   917, -1500,  9612,  3916, -1500,
     925,   972, 10506,  1091, 10323,  9612,  9612, 14845,  9612, 14275,
     927, -1500, -1500,  9612, -1500, -1500,   923,   955, 14845,  9612,
   -1500, -1500,  9612, -1500, -1500,  9612, -1500, 10323,  3916, -1500,
   10506,   164,   164,   902, -1500,   851, -1500, -1500, -1500,  9612,
    9612,  9612,  9612,  9612,  9612,   337,  2695,   337,  2899,   337,
   14375, -1500,   795, -1500, 14275, -1500,   771,   337,   942, -1500,
     936,   948,   164,   164,   119,   164,   164,   337,  1117,   918,
     944, 14845,   924,   170,   944,   951,   928,   489, -1500, -1500,
   14275, -1500, -1500, -1500, -1500,  4119, -1500, -1500, -1500, -1500,
   -1500, -1500,   -72,    65,   -61, -1500, 15491, 15522,  4322,  4322,
    4322,  4322,  4322,  4322,  4322,  4322,  9612,  9612, -1500, -1500,
    9612,  4322,  4322,  9612,  9612,  9612,   977,  4322,  9612,   417,
    9612,  9612,  9612,  9612,  9612,  9612,  4322,  4322,  9612,  9612,
    9612,  4322,  4322,  4322,  9612,  4322,  7379,  9612,  9612,  9612,
    9612,  9612,  9612,  9612,  9612,  9612,  9612, 15649,  9612, -1500,
    7582, -1500,  9612, -1500,   192, -1500, -1500,   291,  1141,   -26,
   10323, -1500,   980, -1500,  3916, -1500, 10619,   472,   510,   956,
     523, -1500,   534,   535, -1500, -1500,   182,   587,   745,   657,
     745,   660,   745, -1500,   411, -1500,   446, -1500, 10323,   938,
   -1500, -1500, 13794, -1500, -1500, 10323, -1500, -1500, 10323, -1500,
   -1500, -1500,  9612,   990, -1500,   991, -1500, 10323, -1500,  3916,
   10323, 10323, -1500,    15, 10323,  5345,  7785,   996,  9612, 10323,
   -1500, -1500, -1500, 10323,   944, -1500,   925,  9612,  9612,  9612,
    9612,  9612,  9612,  9612,  9612,  9612,  9612,  9612,  9612,  9612,
    9612,  9612,  9612,  9612,  9612,   701,   563,   949, 14845, 11544,
   -1500, -1500, 10323, 10323, 11644, 10323, -1500, -1500, 11679, 10323,
     944, 10323, 10323,   944, 10323,   228, -1500, 10506, -1500,    65,
   11779, 11879, 11914, 12014, 12114, 12149,    24,   -61,  3103,  4527,
    5548, 14475,   951,   -19,    61,   983,   254,    27,  5751,   -19,
     770,    28,  9612,   987,  9612, -1500, -1500, 10323, -1500, 10323,
   -1500,  9612,   416,    38, -1500,  9612, -1500, -1500,    51,   337,
   -1500,  9612, -1500,  9612,  9612,  9612,   961,   463, -1500,   963,
     964,   434, -1500, -1500,   677,  9612,   851,   -54,  4732, -1500,
     191,   966,  1012,  1012, -1500, -1500,   974,   133,   643, -1500,
     995,   975, -1500, -1500,   997,   979, -1500, -1500,   -61,   -61,
     -61, -1500, -1500,  2044, -1500,  2044, -1500,  2044, -1500,  2044,
   -1500,  2044, -1500,  2044, -1500,  2044, -1500,  2044,  1044,  1044,
     898, -1500,  2044, -1500,  2044,   898,   671,   671,   981, -1500,
    2044,   105,   982, -1500, 13894,   -67,   -67,   850, 14845,  1044,
    1044, -1500,  2044, -1500,  2044, 15201, 15080, 15111, -1500,  2044,
   -1500,  2044, -1500,  2044, 14945, -1500,  2044, 15618, 14510, 15230,
   15320, 15349,   898,   898,    64,    64,   105,   105,   105,   481,
    9612,   985,   992,   497,  9612,  1184,   999, 13994, -1500,   215,
   14610,  1010,   224,   714,  1119,  1015,   713, -1500, -1500, 10730,
   -1500, -1500, -1500, -1500, 10323, -1500, -1500,  1024, -1500, -1500,
    1009, -1500,  1014, -1500,  1020, -1500, 10423, -1500,   912,   449,
     757, -1500, -1500,   757,   757, 12249, -1500,  1174,   -59, -1500,
   10506,  1018,  1074,  9612,   662,   502,   235,  1003,  1004,  1050,
   12349,   208, 12384,   682, 10323, 10323, 10323,  1171,  1006, 14845,
   14845, 14845, 14845, 14845, 14845, 14845, 14845, 14845, 14845, 14845,
   14845, 14845, 14845, 14845, 14845, 14845, 14845, -1500,  1008, 10323,
   -1500, -1500,  9612,  1687,  1774, -1500,  1783, -1500,  1793,  1011,
    2011,  2424,  1019,  2818,    65,   643, -1500, -1500, -1500, -1500,
   -1500,  1013,  9612, -1500,  4937, 13759,    17,  9612,   502,    61,
   -1500, -1500,  1016, -1500,  9612,  9612, -1500,  1022, -1500,  9612,
     502,   555,  1025, -1500, -1500,  9612, 14845, -1500, -1500,   516,
     536, 14710,  9612, -1500,  2492,  9612,    54, 12484, 14845, 14845,
   -1500,  1032,   132,  9612,  9612, 10323,   643,   701, -1500, -1500,
    9612, -1500,  1579,    65,   159, -1500,  1034,   256,  9815, -1500,
   -1500, -1500,   278,   253,   133,  1054,  1069,  1033,  1070,  1080,
   -1500,   290,   745, -1500,  9612, -1500,  9612, -1500, -1500, -1500,
    7988,  9612, -1500,  1056,  1038, -1500, -1500,  9612,  1046, -1500,
   14029,  9612,  8191,  1048, -1500, 14129, -1500,  8394, -1500, -1500,
   -1500, -1500,  1076, -1500, -1500,   300, -1500,    30, -1500,    65,
   -1500, -1500, -1500, -1500,   757, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500,  1049,
   10323, -1500,  1095,  9612, -1500, -1500,   331, -1500,  1051, -1500,
   -1500, -1500,   541, -1500,  1097,  1055, -1500, -1500,  3022,  3041,
    4319, -1500, -1500,  9612,  4445, 14845, -1500, -1500, -1500, -1500,
   -1500, -1500, -1500, -1500, -1500,   589,   745,  8597,   596,   -19,
      61, 14845,   951, -1500, -1500, 14845,   983, -1500,   598,   -19,
    1059, -1500, -1500, -1500, -1500,   632, -1500, -1500, -1500,  1096,
     633, -1500,   641,  9612,   216,  9612,  9612,  9612, 12584, 12619,
    4524,   745, -1500, 14275, -1500,  1093,   643, -1500,  1058,  4732,
    1107,  1066,   482,   260, -1500, -1500,  1110, -1500, -1500,   133,
    1068,   181, 10323, 12719, 10323, 12819, -1500,   261, 12854, -1500,
    9612, 14980,  9612, -1500, 12954,  4732, -1500,   289,  9612, -1500,
   -1500, -1500,   299, -1500,  1250,    30, -1500, -1500,   714,  1071,
   -1500, -1500, -1500,  9612,   757, -1500, 14845,  1073,  1075, -1500,
   -1500, -1500,  9612,  1114,  1094,  9612, -1500, -1500, -1500, -1500,
    1077,  1079,  1083,  9612,  9612,  9612,  1084,  1233,  1090,  1092,
    8800, -1500,   181, -1500,   311,  9612,    61, -1500,  9612,   555,
   -1500,  9612,  9612,  1098, -1500, -1500,  9612,  9612,   648,  9612,
    9612, 13054, 14845, 14845, -1500, -1500, -1500,  1109,   677,  3713,
   -1500,   745, -1500,   310, -1500,   710, 10323,   -15, -1500,  1099,
   -1500, -1500,  9003, -1500, -1500,  4650, -1500,   720, -1500, -1500,
   -1500, 10323, 13089, 13189, -1500,   325, -1500, 13289, -1500, -1500,
   -1500,  1250,   337,  1102,  1233,  1233, 13324,  1113,  1104, 13424,
    1105,  1106,  1108,  9612, -1500,  9612,   898,   898,   898,  9612,
   -1500, -1500,  1233,   502, -1500, 13524, -1500, -1500, 14745, -1500,
   14745, -1500,  1129,   898,  9612, -1500,  1139,  1129, 14745,  9612,
   14845, 14845,   218,   230, -1500, -1500,  9206,  9409, -1500, -1500,
   -1500, -1500, -1500, 14845,   701,  1111, 10323,   -15,  1432,  3916,
   -1500,  9612, 14945, -1500, -1500,   731, -1500, -1500,  1112, -1500,
   15727, -1500, -1500, -1500,   110,   110, -1500,  9612,  9612, -1500,
    1233,  1233,   502,  1116,  1120,   944,   110,   951,  1121, -1500,
    1270,  1115,  1143,  1144,  1146, -1500,  1148,  1124, 14745,  9612,
    9612, -1500,   230,  9612,  9612, 14845, -1500, -1500,  1432,  3916,
    3916, -1500, 10841, 14980, -1500, -1500, -1500, -1500,  1156, 15727,
     502,   951,  1151, -1500,  1125,  1132, 13559, 13659,   110,   110,
    1134, -1500, -1500,  1137,  1140, -1500,  9612,  1145,  1149,  1173,
   -1500,  1142, -1500, -1500,  1150, 14845, 14845, -1500, 14845,  3916,
   -1500, 10952, -1500, 11063, -1500, -1500, -1500,   701,   312,  1152,
   -1500, -1500, -1500, -1500, -1500,  1154,  1155, -1500, -1500, -1500,
   14845, -1500, -1500, -1500, -1500, -1500, -1500, 11174, -1500, -1500,
   -1500, -1500,   502, -1500, -1500, -1500,   313, -1500
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   128,     1,   308,     0,     0,     0,   629,   309,     0,
     621,   621,   621,    16,     0,     0,    15,     3,     0,    10,
       9,     8,     0,     7,   609,     6,    11,     5,     4,    13,
      12,    14,    98,    99,    97,   106,   108,    43,    59,    56,
      57,    45,    46,    47,     0,    48,    54,    44,   621,   621,
      22,    21,   609,   623,   622,   794,   784,   789,     0,   276,
      41,   114,   115,   116,     0,     0,     0,   117,   119,   126,
       0,   113,    17,   647,   646,   221,   631,     0,   650,   610,
     611,     0,     0,     0,     0,    49,     0,    55,     0,     0,
      52,     0,     0,   621,     0,    18,     0,     0,     0,   278,
       0,     0,   125,   120,     0,     0,     0,     0,     0,     0,
     129,   223,   222,   225,   220,   633,   632,     0,   649,   648,
     652,   651,   655,   613,   612,   615,   104,   105,   102,   103,
     101,     0,     0,   100,   109,    60,    58,    54,    51,    50,
     624,   626,   628,     0,   630,    19,    20,    23,   795,   785,
     790,   277,    39,    42,   124,     0,   121,   122,   123,   127,
       0,   634,     0,   643,   606,   543,    24,    25,    29,     0,
      93,    94,    91,    92,    90,    89,    95,     0,    53,     0,
     627,     0,     0,     0,    40,   118,     0,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,     0,     0,   135,   130,     0,   635,     0,   644,
       0,   656,   607,     0,     0,   545,     0,    26,    27,    28,
       0,   107,     0,   796,   786,   791,   189,   190,   187,   138,
     139,   141,   140,   142,   143,   144,   145,   171,   172,   169,
     170,   162,   173,   174,   163,   160,   161,   188,   182,     0,
     186,   175,   176,   177,   178,   149,   150,   151,   146,   147,
     148,   159,     0,   165,   166,   164,   157,   158,   153,   152,
     154,   155,   156,   137,   136,   181,     0,   167,   168,   543,
     133,   253,   226,   617,   690,   693,   696,   697,   691,   694,
     692,   695,     0,   641,   653,   614,   543,     0,   110,   112,
       0,     0,   596,   594,   616,    96,     0,     0,     0,     0,
       0,     0,   663,   683,   664,   699,   665,   669,   670,   671,
     672,   689,   676,   677,   678,   679,   680,   681,   682,   684,
     685,   686,   687,   754,   668,   675,   688,   761,   768,   666,
     673,   667,   674,     0,     0,     0,     0,   698,   716,   719,
     717,   718,   781,   625,   704,   574,   580,   191,   192,   185,
     180,   193,   183,   179,     0,   131,   307,   568,     0,     0,
     224,     0,   636,   638,     0,   645,   557,   657,     0,     0,
     111,     0,     0,     0,     0,   595,     0,   722,   745,   748,
       0,   751,   741,     0,   707,   755,   762,   769,   775,   778,
       0,     0,   731,   736,   730,     0,   744,   740,   733,     0,
       0,   735,   720,     0,   797,   787,   792,   194,   184,     0,
     305,   306,     0,   543,   132,   134,   255,     0,     0,     0,
      69,    70,    82,   448,   449,     0,     0,     0,     0,   293,
     442,   291,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   292,     0,     0,     0,     0,     0,     0,     0,   689,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   532,     0,     0,     0,   365,
     367,   366,   368,   369,   370,   371,     0,     0,    35,   261,
     227,   232,     0,     0,     0,     0,     0,   257,     0,   347,
     348,   446,   445,     0,     0,     0,     0,   248,   243,   240,
     239,   241,   242,   275,   254,   234,   526,   233,   443,     0,
     517,    77,    78,    75,   246,    76,   247,   249,   311,   238,
     516,   515,   514,   128,   520,   444,     0,   235,   519,   518,
     490,   450,   491,   372,   451,     0,   447,   799,   803,   800,
     801,   802,     0,   618,     0,   617,   642,   559,   608,   544,
       0,     0,     0,   526,     0,   598,   599,     0,   592,   593,
     591,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   130,     0,   130,     0,   130,     0,     0,     0,   727,
     257,   738,   739,   732,   734,     0,   737,   721,     0,     0,
     783,   782,     0,   705,     0,   276,   711,   710,     0,   575,
     570,     0,     0,     0,   581,     0,     0,     0,   658,   566,
     585,   569,   844,   847,     0,     0,   281,   285,   284,   290,
       0,     0,   333,     0,     0,     0,   880,     0,     0,   297,
     294,     0,   342,     0,     0,   261,     0,   279,     0,     0,
       0,   324,   327,     0,   252,   330,     0,     0,    63,     0,
      84,   884,     0,   853,   862,     0,   850,     0,     0,   302,
     299,   478,   479,   348,   360,   128,   274,   272,   273,     0,
       0,     0,     0,     0,     0,     0,   822,     0,     0,     0,
     860,   887,     0,   265,     0,   268,     0,     0,     0,   889,
     898,     0,   455,   454,   492,   453,   452,     0,     0,     0,
     898,   342,     0,   276,   898,   813,     0,   349,   250,   251,
       0,    80,   363,   230,   524,     0,   237,   244,   245,   296,
     301,   310,     0,   357,     0,   236,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   480,   481,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,     0,   229,
       0,   619,     0,   637,   639,   654,   558,     0,     0,   543,
       0,   597,     0,   601,     0,   605,   372,     0,     0,     0,
     712,   725,     0,     0,   700,   702,     0,     0,   133,     0,
     133,     0,   133,   572,     0,   578,     0,   701,     0,     0,
     729,   714,     0,   706,   798,     0,   576,   788,     0,   582,
     793,   567,     0,     0,   584,     0,   583,     0,   586,     0,
       0,     0,    85,     0,     0,   836,     0,     0,     0,     0,
     870,   873,   876,     0,   898,   298,   295,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   280,     0,
      86,    87,     0,     0,     0,     0,    61,    62,     0,     0,
     898,     0,     0,   898,     0,     0,   303,   300,   349,   357,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   856,   814,   822,     0,   865,     0,     0,     0,   822,
       0,     0,     0,     0,     0,   825,   892,     0,   256,     0,
      38,     0,    36,     0,   891,   899,   262,   253,     0,     0,
     867,   899,   258,     0,     0,     0,     0,     0,   483,     0,
       0,   419,   416,   418,    66,     0,   128,     0,   257,   435,
       0,     0,     0,     0,   323,   322,     0,     0,   130,   271,
       0,     0,   503,   502,     0,     0,   504,   508,     0,     0,
       0,   394,   403,   382,   404,   383,   406,   385,   405,   384,
     407,   386,   397,   376,   398,   377,   399,   378,   456,   457,
     469,   408,   387,   409,   388,   470,   467,   468,     0,   396,
     374,   497,     0,   488,     0,   521,   522,   523,   375,   458,
     459,   410,   389,   411,   390,   474,   475,   476,   400,   379,
     401,   380,   402,   381,   477,   395,   373,     0,     0,   472,
     473,   471,   465,   466,   461,   460,   462,   463,   464,     0,
       0,     0,   425,     0,     0,     0,     0,     0,   440,     0,
       0,     0,     0,   551,   554,     0,     0,   600,   603,   372,
     604,   723,   746,   749,     0,   752,   742,     0,   708,   756,
       0,   763,     0,   770,     0,   776,     0,   779,     0,     0,
     263,   726,   715,   571,   577,     0,   660,   661,   587,   590,
     589,     0,     0,     0,     0,   837,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
     382,   383,   385,   384,   386,   376,   377,   378,   387,   388,
     374,   389,   390,   379,   380,   381,   373,   304,     0,     0,
     893,   493,     0,     0,     0,   494,     0,   525,     0,     0,
       0,     0,     0,     0,   357,   130,   527,   528,   529,   530,
     531,     0,     0,   823,     0,   342,   822,     0,     0,     0,
     831,   832,     0,   839,     0,     0,   829,     0,   868,     0,
       0,     0,     0,   827,   869,     0,   859,   824,   888,     0,
       0,    32,     0,   890,     0,     0,     0,     0,   805,   804,
     482,     0,     0,     0,     0,     0,   130,     0,    64,    65,
       0,    79,    71,   357,     0,   436,     0,     0,     0,   439,
     437,   312,     0,     0,     0,     0,     0,     0,     0,     0,
     355,     0,   133,   499,     0,   505,     0,   393,   391,   392,
       0,     0,   486,     0,     0,   509,   513,     0,     0,   489,
       0,     0,     0,     0,   426,     0,   433,     0,   484,   441,
     620,   640,   129,   552,   553,   554,   555,   546,   560,   357,
     602,   724,   747,   750,   713,   753,   743,   703,   709,   757,
     759,   764,   766,   771,   773,   777,   573,   780,   579,     0,
       0,   659,     0,     0,   845,   848,     0,   282,     0,   287,
     288,   286,     0,   336,     0,     0,   339,   334,     0,     0,
       0,   881,   879,   261,     0,    88,   325,   328,   331,   885,
     883,   854,   863,   861,   851,     0,   133,     0,     0,   822,
       0,   857,   815,   838,   830,   858,   866,   828,     0,   822,
       0,   834,   835,   842,   826,     0,   266,   269,    33,     0,
       0,   228,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   133,    67,     0,    72,     0,   130,   438,     0,   257,
       0,     0,   594,     0,   352,   353,     0,   351,   350,     0,
       0,     0,     0,     0,     0,     0,   414,     0,     0,   510,
       0,   498,     0,   487,     0,   257,   427,     0,     0,   485,
     434,   430,     0,   565,   549,   546,   547,   548,   551,     0,
     760,   767,   774,   257,   264,   662,   588,     0,     0,    83,
     283,   289,     0,     0,     0,     0,   335,   871,   874,   877,
       0,     0,     0,     0,     0,     0,     0,   836,     0,     0,
       0,   231,     0,   533,     0,     0,     0,   840,     0,     0,
     833,     0,     0,   259,    30,    37,     0,     0,     0,     0,
       0,     0,   807,   806,   417,   542,   420,     0,    66,     0,
      81,   133,   412,     0,   313,   594,     0,     0,   359,     0,
     356,   358,     0,   344,   362,     0,   541,     0,   539,   415,
     536,     0,     0,     0,   535,     0,   428,     0,   431,   550,
     561,   549,     0,     0,   836,   836,     0,     0,     0,     0,
       0,     0,     0,   257,   894,   261,   326,   329,   332,     0,
     837,   855,   836,     0,   495,     0,   361,   534,   896,   841,
     896,   843,   896,   267,   257,   270,    34,   896,   896,     0,
     809,   808,     0,     0,   423,    68,   293,     0,    73,    77,
      78,    75,    76,    74,     0,     0,     0,     0,     0,     0,
     354,     0,   345,   500,   506,     0,   540,   538,     0,   537,
       0,   563,   556,   728,   821,   821,   337,     0,     0,   340,
     836,   836,     0,     0,     0,   898,   821,   812,     0,   496,
       0,     0,     0,     0,   898,    31,     0,     0,   896,     0,
       0,   421,     0,     0,     0,   299,   364,   413,     0,     0,
       0,   321,   372,   346,   501,   507,   511,   429,     0,     0,
       0,   818,   898,   820,     0,     0,     0,     0,   821,   821,
       0,   882,   895,     0,     0,   852,     0,     0,     0,     0,
     899,     0,   904,   900,     0,   811,   810,   424,   300,     0,
     319,   372,   317,   372,   320,   512,   562,     0,     0,   899,
     819,   846,   849,   338,   341,     0,     0,   878,   886,   864,
     897,   902,   903,   905,   260,   901,   315,   372,   318,   316,
     564,   816,     0,   872,   875,   314,     0,   817
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1500, -1500, -1500, -1500, -1500, -1500,   653,  1275, -1500, -1500,
   -1500, -1500, -1500, -1500,  1363, -1500, -1500, -1500,   744,  1327,
   -1500,  1236, -1500, -1500,  1290, -1500, -1500, -1500,  -113, -1500,
   -1500, -1500,  -112, -1500, -1500, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500,  1157, -1500, -1500,   -52,   -64, -1500, -1500, -1500,
     656,   569,  -514,  -574,  -792, -1500, -1500, -1500, -1451, -1500,
   -1500,  -214,  -376, -1500,   421, -1500,  -589, -1500,  -645,  -330,
    -297, -1500, -1500, -1500, -1500,  -446,   -14, -1500, -1500, -1500,
   -1500, -1500,  -110,  -109,  -107, -1500,  -105, -1500, -1500, -1500,
    1384, -1500,   403, -1500, -1500, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500, -1500, -1500, -1500, -1500,   553,   -70,  -395,    -8,
     149,  -887,  -435, -1500,  -517, -1500, -1500,  -366,   718, -1500,
   -1500, -1500, -1499, -1500, -1500, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500,   652, -1500, -1500, -1500, -1500, -1500, -1500, -1500,
    -152,   -31,  -126,   -32,   112, -1500, -1500, -1500, -1500, -1500,
     965, -1500,  -570, -1500, -1500,  -583, -1500, -1500,  -624,  -123,
    -573, -1347, -1500,  -365, -1500, -1500,  1358, -1500, -1500, -1500,
     846,   971,   115, -1500, -1500, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500, -1500,  -596,  -195, -1500,   976, -1500, -1500, -1500,
   -1500, -1500, -1500,  -401, -1500, -1500,  -317, -1500, -1500, -1500,
   -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500, -1500, -1500,   -53, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500,
    1007,  -680,  -153,  -715,  -676, -1500, -1500, -1329,  -878, -1500,
   -1500, -1500, -1161,   -18, -1184, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500, -1500, -1500,   258,  -481, -1500, -1500, -1500,   758,
   -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500, -1500,
   -1500, -1500, -1500, -1500, -1246,  -712, -1500
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    16,   147,    52,    17,   169,   175,  1556,  1369,
    1474,   706,   511,   153,   512,   101,    19,    20,    45,    46,
      47,    90,    21,    39,    40,   513,   514,  1230,  1231,   515,
    1385,  1489,   516,   517,   975,   518,   635,   519,   520,   521,
     522,  1162,   176,   177,    35,    36,    37,   225,    67,    68,
      69,    70,    22,   290,   380,   215,    23,   113,   216,   114,
     160,   684,  1001,   524,   381,   525,   719,  1555,   708,  1109,
     573,   947,  1472,   949,  1473,   527,   528,   529,   637,   864,
    1440,   530,   531,   532,   533,   534,   535,   536,   537,   432,
     538,   742,  1242,   984,   539,   540,   902,  1453,   903,  1454,
     905,  1455,   541,   869,  1446,   542,   720,  1504,   543,  1250,
    1251,   988,   686,   544,   799,   976,   545,   651,  1002,   547,
     548,   549,   973,   550,  1225,  1563,  1226,  1622,   551,  1075,
    1420,   552,   721,  1402,  1634,  1404,  1635,  1511,  1675,   554,
     376,  1428,  1520,  1285,  1287,  1085,   567,   809,  1590,  1639,
     377,   378,   618,   834,   425,   623,   836,   426,   965,   629,
     581,   396,   313,   314,   222,   306,    80,   125,    25,   165,
     382,    91,    92,   179,    93,    26,    49,   117,   162,    27,
     293,   564,   565,  1081,   385,   220,   221,    78,   122,   387,
      28,   163,   304,   630,   555,   302,   359,   360,   826,   424,
     361,   589,  1298,  1310,   819,   422,   362,   582,  1291,   838,
     587,  1296,   583,  1292,   584,  1293,   586,  1295,   590,  1299,
     591,  1430,   592,  1301,   593,  1431,   594,  1303,   595,  1432,
     596,  1305,   597,  1307,   620,    29,    97,   182,   365,   621,
      30,    98,   183,   366,   625,    31,    96,   181,   364,   614,
     556,   725,  1608,   726,   933,  1642,  1643,  1644,   934,   946,
    1204,  1198,  1193,  1363,  1128,   557,   860,  1437,   861,  1438,
     914,  1459,   911,  1457,   935,   709,   558,   912,  1458,   936,
     559,  1134,  1530,  1135,  1531,  1136,  1532,   873,  1450,   909,
    1456,   702,   710,   560,  1611,   956,   561
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,    71,   292,   649,   814,   523,   859,   701,   962,   733,
     897,   839,   966,   226,   835,   546,   932,   828,   932,   830,
     981,   832,   939,   568,   619,   624,   833,   679,  1353,   743,
     134,   643,  1175,  -128,   927,  1123,  1100,   358,  1102,   863,
    1104,   155,   687,   688,  1182,  1497,   854,  1199,  1205,   601,
      71,    71,    71,   808,    53,  1189,   126,   127,  1212,   766,
      54,  1201,   769,   770,  1621,    86,   603,    38,   854,   986,
     927,  1215,    50,   523,  1373,    58,  1426,   577,    32,    33,
     311,   685,   685,   685,   526,   746,   747,   744,   982,   106,
     107,   108,    71,    71,    71,    71,   734,   523,   312,   926,
      87,   937,    59,   941,   722,   100,   611,   855,   856,   291,
     857,   953,   578,   858,    58,   374,  1234,     2,    51,   291,
     223,   958,   579,  1667,     3,   801,   746,   747,   644,   645,
     856,   732,   802,   423,   223,   815,  1427,   745,    48,  1638,
     735,    59,   983,   669,   797,   798,   640,     4,  1577,     5,
    1127,     6,   526,   845,   388,   509,   683,     7,   732,   307,
     154,  1235,  1138,  1188,    94,    73,    74,     8,    75,   696,
     698,   919,   308,     9,   580,   722,   526,   224,   142,   363,
     170,   171,   758,   759,   358,   746,   747,  1234,  1677,  1467,
     766,   224,   768,   769,   770,   771,    76,    10,  1169,   358,
     772,  1172,   875,   855,   128,   523,   805,   806,   143,   129,
     602,   130,   855,   430,   131,   855,   855,   846,   357,    11,
      12,   646,   849,   758,   759,   987,   855,   604,   358,    34,
     358,   766,   916,   768,   769,   770,   771,   807,   653,   855,
     647,   772,   855,    88,   309,   605,  1190,  1191,   769,   770,
      72,   606,   670,   311,    89,   132,   431,   732,   390,   854,
    1245,   412,  1236,   794,   795,   796,  1645,    77,   722,  1376,
    1246,   312,   523,  1541,  1192,   797,   798,  1654,    81,   310,
    1234,   433,   758,   759,   526,   732,   854,  1345,   413,   414,
     766,   358,   358,   769,   770,   771,   964,   854,  1237,    13,
     772,    79,   523,  1247,  1612,  1549,  1613,   964,  1350,   989,
      82,  1616,  1617,   100,   434,    83,   797,   798,    14,  1685,
    1686,   856,  1248,  1502,  1640,    61,   435,  1249,   172,    15,
     797,   798,   109,   173,   374,   174,    84,   896,   131,   223,
    1594,  1595,   411,   854,    82,   854,  1386,  1424,   856,   685,
     599,   526,   415,  1479,    62,  1619,   416,   110,  1606,   856,
     915,   291,   562,  1216,  1097,   357,   230,  1286,  1088,   600,
     566,   185,  1664,    58,   563,   797,   798,   985,  1098,  1238,
     357,   526,   969,    82,    41,    42,    43,   358,   358,   358,
    1324,   358,   358,   231,   375,   358,   224,   358,    63,   358,
      59,   358,  1429,   877,  1325,   856,  1239,   856,   389,   357,
    1318,   357,   109,  1119,  1252,    44,  1648,  1649,   417,  1094,
     862,   732,   418,   877,  1174,   419,   100,    64,   654,    99,
    1279,   357,   641,  1106,    58,  1108,  1195,  1282,   523,  1196,
     420,  1390,   509,   683,   967,   900,   421,   655,  1399,   877,
    1319,   526,   526,   526,   526,   526,   526,   526,   526,   358,
    1401,    59,  1233,  1208,   526,   526,  1390,  1197,  1392,  1388,
     526,  1466,   357,   357,  1213,  1498,  1509,   877,  1399,   526,
     526,  1469,   358,   523,   526,   526,   526,   877,   526,   627,
     948,  1391,  1183,  1086,  1313,  1159,    65,   622,   967,   877,
     967,   967,   671,  1400,  1516,    66,   932,   628,  1352,   616,
    1349,   291,   105,   967,  1518,   617,   974,   526,   412,   877,
     932,   672,   732,  1575,  1359,  1308,  1547,  1701,  1707,   817,
     818,   820,   685,   822,   823,  -758,  1306,   827,  1588,   829,
    -758,   831,   135,  1121,  1122,   413,   414,   612,  -765,   118,
     119,   993,   997,  -765,  1462,   674,  1137,    89,  -758,   613,
     867,  -772,   526,  1257,  1258,  1259,  -772,   137,   357,   357,
     357,  -765,   357,   357,   675,  1032,   357,  1037,   357,   868,
     357,   106,   357,   108,  -772,  1163,  1164,  1033,  1166,  1487,
     227,   228,  1168,   616,  1170,  1171,   412,  1173,  1377,   617,
     217,  1346,  1076,   685,   685,   685,   140,  1105,   732,   415,
     732,   218,   732,   416,   732,   358,   732,  1209,   732,  1210,
     732,  1221,   732,   413,   414,  -422,   866,   732,   616,   732,
    -422,   616,    58,  1222,   617,   732,   141,   617,    53,  1268,
     357,   969,  1107,   358,    54,  1309,   970,   732,  -422,   732,
     358,  1269,  1381,   358,   732,  1273,   732,  1084,   732,    59,
     722,   732,   358,   357,   423,   358,   358,  1274,  1091,   358,
     437,   438,  1234,  1496,   358,   417,   395,   144,   358,   418,
     393,  1157,   419,   394,   971,   148,   395,   415,  1451,  1227,
     448,   416,   746,   747,   732,   149,   453,   420,   616,  1574,
    1228,  1229,   423,   421,   617,   616,  1092,   358,   358,  1493,
     358,   617,  1366,   972,   358,   423,   358,   358,   616,   358,
     102,   103,   104,   616,   617,   732,   423,   423,   150,   617,
    1095,  1096,  1367,   467,   468,  1515,  1360,  1442,   111,  1361,
     151,   115,  1362,   120,   112,   616,   412,   116,   152,   121,
    1480,   617,   358,   417,   358,   123,   161,   418,   164,  1158,
     419,   124,   156,   157,   158,   159,   145,   470,   471,    87,
     616,  1283,   146,   413,   414,   420,   617,  1284,  1465,   423,
    1468,   421,   180,  1099,   877,  1110,   877,   756,   757,   758,
     759,   760,  1113,    71,   763,  1114,   357,   766,   184,   768,
     769,   770,   771,  1460,  1118,   509,   683,   772,    58,   774,
     775,  1124,  1491,   106,  1471,  1476,  1133,  1328,  1329,  1330,
     877,   877,   219,  1477,   357,   486,   487,   488,  1461,   877,
    1559,   357,   138,   139,   357,    59,   877,   415,   523,   232,
     287,   416,  1334,   357,  1523,   499,   357,   357,   546,   423,
     357,   288,   423,  1101,   423,   357,  1103,   289,  1317,   357,
     636,   294,  1266,  1607,   303,   295,   790,   791,   792,   793,
     794,   795,   796,   233,   423,   234,   235,   507,  1327,   296,
     297,   291,   797,   798,   298,   299,   300,   301,   357,   357,
    1604,   357,   305,  1653,  1110,   357,  1110,   357,   357,   358,
     357,  1576,  1661,   417,   395,   367,   368,   418,  1380,  1289,
     419,   358,   423,   369,  1641,  1641,  1584,   526,   370,   746,
     747,  1443,  1607,   423,  1579,   420,  1641,  1636,   371,   372,
    1680,   421,   373,   357,   383,   357,   379,   384,  1620,   358,
     358,   358,   386,   391,  1603,   166,   167,   950,   951,   423,
     732,  1202,  1195,   392,  1203,   106,   107,   108,   397,   398,
    1678,    41,    42,    43,   358,  1614,   400,   399,  1641,  1641,
     166,   167,   168,   227,   228,   229,   943,   944,   945,   401,
     412,    55,    56,    57,   402,   404,   428,   403,   405,   406,
    1552,   410,   407,   408,   427,  1557,   409,   429,   569,   575,
     576,   588,   650,   632,  1629,  1630,   658,   413,   414,   633,
     642,   660,   609,  1382,   756,   757,   758,   759,   661,   664,
     662,   663,  1706,   665,   766,   666,   768,   769,   770,   771,
     358,   673,   689,   553,   772,   676,   774,   775,   677,   705,
     690,  1294,   711,   574,   727,   728,   691,   692,   693,   694,
     729,   412,   585,   731,   736,  1669,   737,   738,   739,   740,
     741,   724,   598,    15,   800,   746,   747,   803,   669,   812,
     824,   415,   608,   810,   578,   416,   703,   825,   413,   414,
     357,   626,   622,   840,   579,  1505,   843,   848,   853,   844,
     847,   634,   357,   852,   850,   792,   793,   794,   795,   796,
     870,   652,  1439,   865,   744,   895,   657,   412,   659,   797,
     798,   871,   872,   877,   906,   358,   901,   668,   907,   918,
     357,   357,   357,   954,   955,   680,   681,   682,   957,   813,
     959,   960,   961,  1631,   413,   414,   580,   417,   963,   967,
     700,   418,   415,   968,   419,   357,   416,  1028,   704,  1083,
    1087,   700,  1093,  1111,   712,   713,   714,   715,   716,   420,
    1116,  1117,   758,   759,  1160,   421,  1131,   730,  1207,  1488,
     766,  1194,   768,   769,   770,   771,  1220,  1223,  1224,  1578,
     772,  1240,  1241,  1670,  1672,  1276,  1286,  1503,  1244,  1254,
    1253,  1281,  1255,  1256,  1297,  1260,  1261,  1288,   415,  1271,
     874,   876,   416,   523,   412,  1300,  1272,   358,   417,   358,
    1302,   357,   418,  1277,  1314,   419,  1304,  1312,  1320,  1321,
    1322,  1332,  1333,  1696,  1394,   910,  1340,  1347,   913,  1354,
     420,   413,   414,   816,  1343,  1357,   421,   732,  1364,  1395,
    1397,   792,   793,   794,   795,   796,  1375,  1396,  1503,  1387,
    1398,  1409,  1410,   523,   523,   797,   798,  1434,  1423,  1628,
    1412,   842,  1418,  1433,   417,  1435,  1441,  1444,   418,  1445,
    1315,   419,  1470,  1492,  1475,  1490,   732,  1494,   732,  1495,
    1499,  1501,   526,  1519,  1527,  1460,   420,  1524,   980,  1525,
    1528,  1533,   421,   523,  1534,   415,   357,  1535,  1539,   416,
     652,   358,   732,  1540,  1542,  1564,  1543,   700,   898,  1597,
     899,  1610,  1554,  1615,  1580,   904,   358,  1593,  1598,  1600,
    1601,   908,  1602,  1656,  1658,  1659,  1627,  1637,  1657,  1662,
     917,  1651,   526,   526,  1660,  1652,  1655,  1663,  1676,  1679,
    1681,   920,   921,   922,   923,   924,   925,  1682,   931,  1687,
     931,  1507,  1688,  1079,  1693,  1689,   133,  1694,  1691,   952,
    1626,   417,  1692,  1695,    18,   418,  1702,  1331,   419,  1703,
    1704,    85,   526,   178,   136,  1565,  1082,  1568,  1214,  1569,
    1570,   358,  1571,   420,  1572,    24,  1243,   315,   357,   421,
     357,  1500,  1546,  1393,  1521,  1591,  1522,  1425,   631,  1592,
    1003,  1005,  1007,  1009,  1011,  1013,  1015,  1017,  1018,  1019,
      95,   804,  1020,  1022,  1024,  1025,  1026,  1027,  1126,  1030,
    1031,   638,  1034,  1035,  1036,  1038,  1039,  1040,  1042,  1044,
    1045,  1046,  1047,  1049,  1051,  1053,  1054,  1056,  1058,  1059,
    1060,  1061,  1062,  1063,  1064,  1065,  1066,  1067,  1068,  1650,
    1077,  1551,   639,  1356,  1080,     0,   940,     0,  1585,     0,
       0,     0,     0,  1700,     0,   412,  1089,  1004,  1006,  1008,
    1010,  1012,  1014,  1016,     0,     0,     0,     0,     0,  1021,
    1023,     0,   357,   724,     0,  1029,     0,     0,     0,     0,
       0,   724,   413,   414,  1041,  1043,     0,   357,     0,  1048,
    1050,  1052,     0,  1055,  1115,     0,     0,     0,     0,     0,
       0,  1120,     0,     0,     0,     0,     0,     0,  1130,     0,
    1132,     0,     0,     0,     0,     0,     0,     0,     0,  1139,
    1140,  1141,  1142,  1143,  1144,  1145,  1146,  1147,  1148,  1149,
    1150,  1151,  1152,  1153,  1154,  1155,  1156,     0,     0,   972,
       0,     0,     0,     0,   -74,     0,   415,     0,     0,   578,
     416,     0,   357,     0,     0,   746,   747,     0,     0,   579,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     712,  1185,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1384,     0,     0,  1206,     0,   700,     0,     0,     0,
     746,   747,     0,  1211,     0,     0,     0,   700,   972,     0,
       0,     0,     0,  1139,     0,  1217,  1218,  1219,     0,     0,
       0,   580,   417,     0,     0,     0,   418,  1232,     0,   419,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   420,     0,     0,     0,     0,     0,
     421,     0,   748,   749,   750,   751,   752,   753,   754,   755,
     756,   757,   758,   759,   760,   761,   762,   763,   764,   765,
     766,   767,   768,   769,   770,   771,  1316,     0,     0,     0,
     772,   773,   774,   775,   776,   777,     0,     0,   778,   779,
     780,   781,   782,   783,   784,   756,   757,   758,   759,   760,
       0,     0,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
     412,     0,  1270,   778,   779,   780,  1275,     0,     0,   784,
       0,     0,     0,   785,   786,  1348,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,   413,   414,     0,
       0,     0,  1358,     0,     0,   797,   798,     0,  1365,     0,
     509,   683,     0,     0,     0,  1370,     0,     0,  1372,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   412,     0,     0,
       0,   415,     0,  1407,  1335,   416,   412,     0,     0,     0,
       0,     0,     0,     0,     0,  1417,   412,     0,     0,     0,
    1422,     0,     0,     0,   413,   414,     0,     0,     0,  1351,
       0,     0,     0,   413,   414,     0,  1355,   931,     0,     0,
       0,     0,     0,   413,   414,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   553,     0,     0,     0,
       0,     0,     0,   695,     0,  1378,  1379,   417,     0,   316,
       0,   418,  1383,  1336,   419,   317,     0,     0,     0,     0,
    1139,   318,     0,     0,     0,     0,     0,     0,   415,   420,
    1464,   319,   416,     0,     0,   421,  1403,   415,  1405,   320,
       0,   416,     0,  1408,     0,     0,     0,   415,     0,  1411,
       0,   416,     0,  1414,   321,     0,  1478,     0,     0,     0,
       0,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   417,  1436,     0,     0,   418,     0,
    1337,   419,     0,   417,     0,     0,     0,   418,     0,  1338,
     419,     0,     0,   417,     0,   700,   420,   418,     0,  1339,
     419,     0,   421,     0,     0,   420,     0,     0,     0,     0,
       0,   421,     0,    58,     0,   420,     0,     0,     0,     0,
       0,   421,     0,     0,     0,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1481,  1482,  1483,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   412,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1512,     0,  1513,   746,   747,     0,     0,     0,
    1517,   413,   414,     0,     0,     0,   356,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1605,     0,  1526,     0,     0,  1529,     0,     0,
       0,     0,     0,     0,     0,  1536,  1537,  1538,     0,     0,
       0,     0,  1545,     0,     0,     0,     0,  1548,     0,   650,
    1550,     0,     0,   700,  1553,     0,     0,     0,   700,  1558,
       0,  1560,  1561,     0,     0,   415,     0,     0,     0,   416,
       0,  1573,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1582,     0,     0,     0,     0,     0,
     756,   757,   758,   759,   760,     0,     0,   763,   764,   765,
     766,     0,   768,   769,   770,   771,   876,     0,     0,     0,
     772,     0,   774,   775,     0,     0,     0,   700,   778,   779,
     780,     0,     0,     0,   784,     0,     0,     0,     0,     0,
       0,   417,     0,     0,     0,   418,     0,  1341,   419,     0,
       0,  1618,     0,     0,     0,     0,     0,     0,     0,  1625,
       0,     0,     0,   420,     0,     0,     0,     0,     0,   421,
       0,  1632,     0,  1633,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,  1646,
    1647,     0,     0,     0,     0,   797,   798,     0,     0,     0,
     509,   683,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1665,  1666,     0,     0,     0,  1668,     0,     0,   436,
       0,  1671,  1673,   437,   438,     3,     0,   439,   440,   441,
       0,   442,     0,   443,   444,   445,   446,   447,     0,     0,
       0,     0,     0,   448,   449,   450,   451,   452,  1690,   453,
       0,     0,     0,     0,     0,     0,   454,   455,     0,     0,
     456,  1697,   457,   458,     0,     0,   459,     0,     8,   460,
     461,     0,   462,   463,     0,     0,   464,   465,     0,     0,
       0,     0,     0,   466,     0,     0,   467,   468,     0,   322,
     323,   324,     0,   326,   327,   328,   329,   330,   469,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
       0,   344,   345,   346,     0,     0,   349,   350,   351,   352,
     470,   471,   472,   473,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   474,   475,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   476,   477,   478,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   479,
     480,   481,   482,   483,     0,   484,     0,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,    59,   496,
     497,   498,     0,     0,     0,     0,     0,   412,   499,   500,
     501,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   502,   503,   504,     0,    14,
       0,     0,   505,   506,   413,   414,     0,     0,     0,     0,
     507,     0,   508,   436,   509,   510,     0,   437,   438,     3,
       0,   439,   440,   441,     0,   442,     0,   443,   444,   445,
     446,   447,     0,     0,     0,     0,     0,   448,   449,   450,
     451,   452,     0,   453,     0,     0,     0,     0,     0,     0,
     454,   455,     0,     0,   456,     0,   457,   458,     0,     0,
     459,     0,     8,   460,   461,     0,   462,   463,   415,     0,
     464,   465,   416,     0,     0,     0,     0,   466,     0,     0,
     467,   468,     0,   322,   323,   324,     0,   326,   327,   328,
     329,   330,   469,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,     0,   344,   345,   346,     0,     0,
     349,   350,   351,   352,   470,   471,   472,   473,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     474,   475,     0,     0,   417,     0,     0,     0,   418,     0,
    1342,   419,     0,     0,     0,   476,   477,   478,     0,     0,
       0,     0,     0,     0,     0,    58,   420,     0,     0,     0,
       0,     0,   421,   479,   480,   481,   482,   483,     0,   484,
       0,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,    59,   496,   497,   498,     0,     0,     0,     0,
       0,     0,   499,  1371,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   502,
     503,   504,     0,    14,     0,     0,   505,   506,     0,     0,
     437,   438,     0,     0,   507,     0,   508,     0,   509,   510,
     443,   444,   445,   446,   447,     0,     0,     0,     0,     0,
     448,     0,   450,     0,     0,     0,   453,     0,   412,     0,
       0,     0,     0,     0,   455,     0,     0,     0,     0,     0,
     458,     0,     0,   459,     0,     0,   460,     0,   927,     0,
     463,     0,     0,     0,     0,   413,   414,     0,     0,     0,
     570,     0,     0,   467,   468,     0,   322,   323,   324,     0,
     326,   327,   328,   329,   330,   469,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,     0,   344,   345,
     346,     0,     0,   349,   350,   351,   352,   470,   471,   571,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   474,   475,     0,     0,     0,     0,   415,
       0,     0,     0,   416,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   479,   480,   481,   482,
     483,   412,   484,   722,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   723,   572,   497,   498,     0,
       0,     0,     0,     0,     0,   499,     0,     0,   413,   414,
       0,     0,     0,     0,     0,   417,     0,     0,     0,   418,
       0,     0,   928,   503,   504,     0,    14,     0,     0,   505,
     506,     0,     0,     0,   437,   438,     0,   929,     0,   930,
       0,   509,   510,   421,   443,   444,   445,   446,   447,     0,
       0,     0,     0,     0,   448,     0,   450,     0,     0,     0,
     453,     0,   412,     0,     0,     0,     0,     0,   455,     0,
       0,     0,   415,     0,   458,     0,   416,   459,     0,     0,
     460,     0,     0,     0,   463,     0,     0,     0,     0,   413,
     414,     0,     0,     0,   570,     0,     0,   467,   468,     0,
     322,   323,   324,     0,   326,   327,   328,   329,   330,   469,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,     0,   344,   345,   346,     0,     0,   349,   350,   351,
     352,   470,   471,   571,     0,     0,     0,     0,   417,     0,
       0,     0,   418,     0,  1344,   419,     0,   474,   475,     0,
       0,     0,     0,   415,     0,     0,     0,   416,     0,     0,
     420,     0,     0,     0,     0,     0,   421,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     479,   480,   481,   482,   483,   412,   484,   722,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   723,
     572,   497,   498,     0,   412,     0,     0,     0,     0,   499,
       0,     0,   413,   414,     0,     0,     0,     0,     0,   417,
       0,     0,     0,   418,     0,     0,   928,   503,   504,     0,
      14,   413,   414,   505,   506,     0,     0,     0,   437,   438,
       0,   929,     0,   938,     0,   509,   510,   421,   443,   444,
     445,   446,   447,     0,     0,     0,     0,     0,   448,     0,
     450,     0,     0,     0,   453,     0,   603,     0,     0,     0,
       0,     0,   455,     0,     0,     0,   415,     0,   458,     0,
     416,   459,     0,     0,   460,     0,     0,     0,   463,     0,
       0,     0,     0,     0,     0,   415,     0,     0,   570,   416,
       0,   467,   468,     0,   322,   323,   324,     0,   326,   327,
     328,   329,   330,   469,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,     0,   344,   345,   346,     0,
       0,   349,   350,   351,   352,   470,   471,   571,     0,     0,
       0,     0,   417,     0,     0,     0,   418,     0,  1447,   419,
       0,   474,   475,     0,     0,     0,     0,     0,     0,     0,
       0,   417,     0,     0,   420,   418,     0,  1448,   419,     0,
     421,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,   420,   479,   480,   481,   482,   483,   421,
     484,     0,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    59,   572,   497,   498,     0,     0,     0,
       0,     0,     0,   499,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   604,     0,     0,
     502,   503,   504,     0,    14,     0,     0,   505,   506,     0,
       0,     0,   437,   438,     0,  1184,     0,   508,     0,   509,
     510,   606,   443,   444,   445,   446,   447,     0,     0,     0,
       0,     0,   448,     0,   450,     0,     0,     0,   453,     0,
       0,     0,     0,     0,     0,     0,   455,     0,     0,     0,
       0,     0,   458,     0,     0,   459,     0,     0,   460,     0,
       0,     0,   463,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   570,     0,     0,   467,   468,     0,   322,   323,
     324,     0,   326,   327,   328,   329,   330,   469,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,     0,
     344,   345,   346,     0,     0,   349,   350,   351,   352,   470,
     471,   472,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   474,   475,     0,     0,     0,
       0,     0,     0,     0,   648,     0,     0,     0,     0,     0,
     476,   477,   478,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   479,   480,
     481,   482,   483,     0,   484,     0,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,    59,   572,   497,
     498,     0,     0,     0,     0,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   502,   503,   504,     0,    14,     0,
       0,   505,   506,     0,     0,   437,   438,     0,     0,   507,
       0,   508,     0,   509,   510,   443,   444,   445,   446,   447,
       0,     0,     0,     0,     0,   448,     0,   450,     0,     0,
       0,   453,     0,     0,     0,     0,     0,     0,     0,   455,
       0,     0,     0,     0,     0,   458,     0,     0,   459,     0,
       0,   460,     0,     0,     0,   463,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   570,     0,     0,   467,   468,
       0,   322,   323,   324,     0,   326,   327,   328,   329,   330,
     469,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,     0,   344,   345,   346,     0,     0,   349,   350,
     351,   352,   470,   471,   472,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   474,   475,
       0,     0,     0,     0,     0,     0,     0,   678,     0,     0,
       0,     0,     0,   476,   477,   478,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   479,   480,   481,   482,   483,     0,   484,     0,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
      59,   572,   497,   498,     0,     0,     0,     0,     0,     0,
     499,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   502,   503,   504,
       0,    14,     0,     0,   505,   506,     0,     0,   437,   438,
       0,     0,   507,     0,   508,     0,   509,   510,   443,   444,
     445,   446,   447,     0,     0,     0,     0,     0,   448,  1566,
     450,   451,     0,     0,   453,     0,     0,     0,     0,     0,
       0,     0,   455,     0,     0,     0,     0,     0,   458,     0,
       0,   459,     0,     0,   460,   461,     0,     0,   463,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   570,     0,
       0,   467,   468,     0,   322,   323,   324,     0,   326,   327,
     328,   329,   330,   469,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,     0,   344,   345,   346,     0,
       0,   349,   350,   351,   352,   470,   471,   571,  1567,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   474,   475,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   479,   480,   481,   482,   483,     0,
     484,     0,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    59,   572,   497,   498,     0,     0,     0,
       0,     0,     0,   499,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     502,   503,   504,     0,    14,     0,     0,   505,   506,     0,
       0,   437,   438,     0,     0,   507,     0,   508,     0,   509,
     510,   443,   444,   445,   446,   447,     0,     0,     0,     0,
       0,   448,     0,   450,     0,     0,     0,   453,     0,     0,
       0,     0,     0,     0,     0,   455,     0,     0,     0,     0,
       0,   458,     0,     0,   459,     0,     0,   460,     0,     0,
       0,   463,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   570,     0,     0,   467,   468,     0,   322,   323,   324,
       0,   326,   327,   328,   329,   330,   469,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,     0,   344,
     345,   346,     0,     0,   349,   350,   351,   352,   470,   471,
     472,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   474,   475,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   476,
     477,   478,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   479,   480,   481,
     482,   483,     0,   484,     0,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,    59,   572,   497,   498,
       0,     0,     0,     0,     0,     0,   499,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,   504,     0,    14,     0,     0,
     505,   506,     0,     0,   437,   438,     0,     0,   507,     0,
     508,     0,   509,   510,   443,   444,   445,   446,   447,     0,
       0,     0,     0,     0,   448,     0,   450,     0,     0,     0,
     453,     0,     0,     0,     0,     0,     0,     0,   455,     0,
       0,     0,     0,     0,   458,     0,     0,   459,     0,     0,
     460,     0,     0,     0,   463,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   570,     0,     0,   467,   468,   977,
     322,   323,   324,     0,   326,   327,   328,   329,   330,   469,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,     0,   344,   345,   346,     0,     0,   349,   350,   351,
     352,   470,   471,   571,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   474,   475,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     479,   480,   481,   482,   483,     0,   484,   722,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   723,
     572,   497,   498,     0,     0,     0,     0,     0,     0,   499,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   502,   503,   504,     0,
      14,     0,     0,   505,   506,     0,     0,   437,   438,     0,
       0,   978,     0,   508,   979,   509,   510,   443,   444,   445,
     446,   447,     0,     0,     0,     0,     0,   448,     0,   450,
       0,     0,   412,   453,     0,     0,     0,     0,     0,     0,
       0,   455,     0,     0,     0,     0,     0,   458,     0,     0,
     459,     0,     0,   460,     0,     0,     0,   463,     0,   413,
     414,     0,     0,     0,     0,     0,     0,   570,     0,     0,
     467,   468,     0,   322,   323,   324,     0,   326,   327,   328,
     329,   330,   469,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,     0,   344,   345,   346,     0,     0,
     349,   350,   351,   352,   470,   471,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     474,   475,     0,   415,     0,     0,     0,   416,     0,     0,
       0,     0,     0,     0,     0,   998,   999,  1000,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   479,   480,   481,   482,   483,   412,   484,
       0,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,    59,   572,   497,   498,     0,     0,     0,     0,
       0,     0,   499,     0,     0,   413,   414,     0,     0,   417,
       0,     0,     0,   418,     0,  1449,   419,     0,     0,   502,
     503,   504,     0,    14,     0,     0,   505,   506,     0,     0,
       0,   420,   437,   438,   507,     0,   508,   421,   509,   510,
     717,     0,   443,   444,   445,   446,   447,     0,     0,     0,
       0,     0,   448,     0,   450,     0,     0,   412,   453,     0,
       0,     0,     0,     0,     0,     0,   455,     0,     0,   415,
       0,     0,   458,   416,     0,   459,   718,     0,   460,     0,
       0,     0,   463,     0,   413,   414,     0,     0,     0,     0,
       0,     0,   570,     0,     0,   467,   468,     0,   322,   323,
     324,     0,   326,   327,   328,   329,   330,   469,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,     0,
     344,   345,   346,     0,     0,   349,   350,   351,   352,   470,
     471,   571,     0,     0,     0,   417,     0,     0,     0,   418,
       0,  1452,   419,     0,     0,   474,   475,     0,   415,     0,
       0,     0,   416,     0,     0,     0,     0,   420,     0,     0,
       0,     0,     0,   421,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   479,   480,
     481,   482,   483,   412,   484,     0,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,    59,   572,   497,
     498,     0,     0,     0,     0,     0,     0,   499,     0,     0,
     413,   414,     0,     0,   417,     0,     0,     0,   418,     0,
    1486,   419,     0,     0,   502,   503,   504,     0,    14,     0,
       0,   505,   506,     0,     0,     0,   420,   437,   438,   507,
     607,   508,   421,   509,   510,   717,     0,   443,   444,   445,
     446,   447,     0,     0,     0,     0,     0,   448,     0,   450,
       0,     0,     0,   453,     0,     0,     0,     0,     0,     0,
       0,   455,     0,     0,   415,     0,     0,   458,   416,     0,
     459,   718,     0,   460,     0,     0,     0,   463,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   570,     0,     0,
     467,   468,     0,   322,   323,   324,     0,   326,   327,   328,
     329,   330,   469,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,     0,   344,   345,   346,     0,     0,
     349,   350,   351,   352,   470,   471,   571,     0,     0,     0,
     417,     0,     0,     0,   418,     0,  1583,   419,     0,     0,
     474,   475,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   420,     0,     0,     0,     0,     0,   421,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   479,   480,   481,   482,   483,     0,   484,
     722,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,   723,   572,   497,   498,     0,     0,     0,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   502,
     503,   504,     0,    14,     0,     0,   505,   506,     0,     0,
       0,     0,   437,   438,   507,     0,   508,     0,   509,   510,
     717,     0,   443,   444,   445,   446,   447,     0,     0,     0,
       0,     0,   448,     0,   450,     0,     0,     0,   453,     0,
       0,     0,     0,     0,     0,     0,   455,     0,     0,     0,
       0,     0,   458,     0,     0,   459,   718,     0,   460,     0,
       0,     0,   463,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   570,     0,     0,   467,   468,     0,   322,   323,
     324,     0,   326,   327,   328,   329,   330,   469,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,     0,
     344,   345,   346,     0,     0,   349,   350,   351,   352,   470,
     471,   571,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   474,   475,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   479,   480,
     481,   482,   483,     0,   484,     0,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,    59,   572,   497,
     498,     0,     0,     0,     0,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   502,   503,   504,     0,    14,     0,
       0,   505,   506,     0,     0,     0,     0,   437,   438,   507,
     840,   508,     0,   509,   510,   717,     0,   443,   444,   445,
     446,   447,     0,     0,     0,     0,     0,   448,     0,   450,
       0,     0,     0,   453,     0,     0,     0,     0,     0,     0,
       0,   455,     0,     0,     0,     0,     0,   458,     0,     0,
     459,   718,     0,   460,     0,     0,     0,   463,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   570,     0,     0,
     467,   468,     0,   322,   323,   324,     0,   326,   327,   328,
     329,   330,   469,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,     0,   344,   345,   346,     0,     0,
     349,   350,   351,   352,   470,   471,   571,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     474,   475,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   479,   480,   481,   482,   483,     0,   484,
       0,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,    59,   572,   497,   498,     0,     0,     0,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   502,
     503,   504,     0,    14,     0,     0,   505,   506,     0,     0,
     437,   438,     0,     0,   507,     0,   508,     0,   509,   510,
     443,   444,   445,   446,   447,     0,     0,     0,     0,     0,
     448,     0,   450,     0,     0,     0,   453,     0,     0,     0,
       0,     0,     0,     0,   455,     0,     0,     0,     0,     0,
     458,     0,     0,   459,     0,     0,   460,     0,     0,     0,
     463,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     570,     0,     0,   467,   468,  1125,   322,   323,   324,     0,
     326,   327,   328,   329,   330,   469,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,     0,   344,   345,
     346,     0,     0,   349,   350,   351,   352,   470,   471,   571,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   474,   475,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   479,   480,   481,   482,
     483,     0,   484,   722,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   723,   572,   497,   498,     0,
       0,     0,     0,     0,     0,   499,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   502,   503,   504,     0,    14,     0,     0,   505,
     506,     0,     0,   437,   438,     0,     0,   507,     0,   508,
       0,   509,   510,   443,   444,   445,   446,   447,     0,     0,
       0,     0,     0,   448,     0,   450,     0,     0,     0,   453,
       0,     0,     0,     0,     0,     0,     0,   455,     0,     0,
       0,     0,     0,   458,     0,     0,   459,     0,     0,   460,
       0,     0,     0,   463,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   570,     0,     0,   467,   468,     0,   322,
     323,   324,     0,   326,   327,   328,   329,   330,   469,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
       0,   344,   345,   346,     0,     0,   349,   350,   351,   352,
     470,   471,   571,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   474,   475,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   479,
     480,   481,   482,   483,     0,   484,   722,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   723,   572,
     497,   498,     0,     0,     0,     0,     0,     0,   499,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   502,   503,   504,     0,    14,
       0,     0,   505,   506,     0,     0,   437,   438,     0,     0,
     507,     0,   508,  1186,   509,   510,   443,   444,   445,   446,
     447,     0,     0,     0,     0,     0,   448,     0,   450,     0,
       0,     0,   453,     0,     0,     0,     0,     0,     0,     0,
     455,     0,     0,     0,     0,     0,   458,     0,     0,   459,
       0,     0,   460,     0,     0,     0,   463,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   570,     0,     0,   467,
     468,     0,   322,   323,   324,     0,   326,   327,   328,   329,
     330,   469,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,     0,   344,   345,   346,     0,     0,   349,
     350,   351,   352,   470,   471,   571,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   474,
     475,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   479,   480,   481,   482,   483,     0,   484,   722,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   723,   572,   497,   498,     0,     0,     0,     0,     0,
       0,   499,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
     504,     0,    14,     0,     0,   505,   506,     0,     0,   437,
     438,     0,     0,   507,     0,   508,  1200,   509,   510,   443,
     444,   445,   446,   447,     0,     0,     0,     0,     0,   448,
       0,   450,     0,     0,     0,   453,     0,     0,     0,     0,
       0,     0,     0,   455,     0,     0,     0,     0,     0,   458,
       0,     0,   459,     0,     0,   460,     0,     0,     0,   463,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   570,
       0,     0,   467,   468,     0,   322,   323,   324,     0,   326,
     327,   328,   329,   330,   469,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,     0,   344,   345,   346,
       0,     0,   349,   350,   351,   352,   470,   471,   571,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   474,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   479,   480,   481,   482,   483,
       0,   484,     0,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,    59,   572,   497,   498,     0,     0,
       0,     0,     0,     0,   499,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   502,   503,   504,     0,    14,     0,     0,   505,   506,
       0,     0,   437,   438,     0,     0,   507,   607,   508,     0,
     509,   510,   443,   444,   445,   446,   447,     0,     0,     0,
       0,     0,   448,     0,   450,     0,     0,     0,   453,     0,
       0,     0,     0,     0,     0,     0,   455,     0,     0,     0,
       0,     0,   458,     0,     0,   459,     0,     0,   460,     0,
       0,     0,   463,     0,     0,     0,     0,     0,   656,     0,
       0,     0,   570,     0,     0,   467,   468,     0,   322,   323,
     324,     0,   326,   327,   328,   329,   330,   469,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,     0,
     344,   345,   346,     0,     0,   349,   350,   351,   352,   470,
     471,   571,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   474,   475,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   479,   480,
     481,   482,   483,     0,   484,     0,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,    59,   572,   497,
     498,     0,     0,     0,     0,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   502,   503,   504,     0,    14,     0,
       0,   505,   506,     0,     0,   437,   438,     0,     0,   507,
       0,   508,     0,   509,   510,   443,   444,   445,   446,   447,
       0,     0,     0,     0,     0,   448,     0,   450,     0,     0,
       0,   453,     0,     0,     0,     0,     0,     0,     0,   455,
       0,     0,     0,     0,     0,   458,     0,     0,   459,     0,
       0,   460,     0,     0,     0,   463,     0,     0,   667,     0,
       0,     0,     0,     0,     0,   570,     0,     0,   467,   468,
       0,   322,   323,   324,     0,   326,   327,   328,   329,   330,
     469,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,     0,   344,   345,   346,     0,     0,   349,   350,
     351,   352,   470,   471,   571,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   474,   475,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   479,   480,   481,   482,   483,     0,   484,     0,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
      59,   572,   497,   498,     0,     0,     0,     0,     0,     0,
     499,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   502,   503,   504,
       0,    14,     0,     0,   505,   506,     0,     0,     0,     0,
     437,   438,   507,     0,   508,     0,   509,   510,   699,     0,
     443,   444,   445,   446,   447,     0,     0,     0,     0,     0,
     448,     0,   450,     0,     0,     0,   453,     0,     0,     0,
       0,     0,     0,     0,   455,     0,     0,     0,     0,     0,
     458,     0,     0,   459,     0,     0,   460,     0,     0,     0,
     463,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     570,     0,     0,   467,   468,     0,   322,   323,   324,     0,
     326,   327,   328,   329,   330,   469,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,     0,   344,   345,
     346,     0,     0,   349,   350,   351,   352,   470,   471,   571,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   474,   475,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   479,   480,   481,   482,
     483,     0,   484,     0,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,    59,   572,   497,   498,     0,
       0,     0,     0,     0,     0,   499,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   502,   503,   504,     0,    14,     0,     0,   505,
     506,     0,     0,   437,   438,     0,     0,   507,     0,   508,
       0,   509,   510,   443,   444,   445,   446,   447,     0,     0,
       0,     0,     0,   448,     0,   450,     0,     0,     0,   453,
       0,     0,     0,     0,     0,     0,     0,   455,     0,     0,
       0,     0,     0,   458,     0,     0,   459,     0,     0,   460,
       0,     0,     0,   463,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   570,     0,     0,   467,   468,     0,   322,
     323,   324,     0,   326,   327,   328,   329,   330,   469,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
       0,   344,   345,   346,     0,     0,   349,   350,   351,   352,
     470,   471,   571,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   474,   475,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   479,
     480,   481,   482,   483,     0,   484,     0,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,    59,   572,
     497,   498,     0,     0,     0,     0,     0,     0,   499,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   703,     0,   502,   503,   504,     0,    14,
       0,     0,   505,   506,     0,     0,     0,     0,   437,   438,
     507,     0,   508,     0,   509,   510,   707,     0,   443,   444,
     445,   446,   447,     0,     0,     0,     0,     0,   448,     0,
     450,     0,     0,     0,   453,     0,     0,     0,     0,     0,
       0,     0,   455,     0,     0,     0,     0,     0,   458,     0,
       0,   459,     0,     0,   460,     0,     0,     0,   463,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   570,     0,
       0,   467,   468,     0,   322,   323,   324,     0,   326,   327,
     328,   329,   330,   469,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,     0,   344,   345,   346,     0,
       0,   349,   350,   351,   352,   470,   471,   571,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   474,   475,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   479,   480,   481,   482,   483,     0,
     484,     0,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    59,   572,   497,   498,     0,     0,     0,
       0,     0,     0,   499,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     502,   503,   504,     0,    14,     0,     0,   505,   506,     0,
       0,   437,   438,     0,     0,   507,     0,   508,     0,   509,
     510,   443,   444,   445,   446,   447,     0,     0,     0,     0,
       0,   448,     0,   450,     0,     0,     0,   453,     0,     0,
       0,     0,     0,     0,     0,   455,     0,     0,     0,     0,
       0,   458,     0,     0,   459,     0,     0,   460,     0,     0,
       0,   463,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   570,     0,     0,   467,   468,     0,   322,   323,   324,
       0,   326,   327,   328,   329,   330,   469,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,     0,   344,
     345,   346,     0,     0,   349,   350,   351,   352,   470,   471,
     571,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   474,   475,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   479,   480,   481,
     482,   483,     0,   484,   722,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,   723,   572,   497,   498,
       0,     0,     0,     0,     0,     0,   499,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,   504,     0,    14,     0,     0,
     505,   506,     0,     0,   437,   438,     0,     0,   507,     0,
     508,     0,   509,   510,   443,   444,   445,   446,   447,     0,
       0,  1057,     0,     0,   448,     0,   450,     0,     0,     0,
     453,     0,     0,     0,     0,     0,     0,     0,   455,     0,
       0,     0,     0,     0,   458,     0,     0,   459,     0,     0,
     460,     0,     0,     0,   463,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   570,     0,     0,   467,   468,     0,
     322,   323,   324,     0,   326,   327,   328,   329,   330,   469,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,     0,   344,   345,   346,     0,     0,   349,   350,   351,
     352,   470,   471,   571,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   474,   475,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     479,   480,   481,   482,   483,     0,   484,     0,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,    59,
     572,   497,   498,     0,     0,     0,     0,     0,     0,   499,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   502,   503,   504,     0,
      14,     0,     0,   505,   506,     0,     0,   437,   438,     0,
       0,   507,     0,   508,     0,   509,   510,   443,   444,   445,
     446,   447,     0,     0,     0,     0,     0,   448,     0,   450,
       0,     0,     0,   453,     0,     0,     0,     0,     0,     0,
       0,   455,     0,     0,     0,     0,     0,   458,     0,     0,
     459,     0,     0,   460,     0,     0,     0,   463,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   570,     0,     0,
     467,   468,     0,   322,   323,   324,     0,   326,   327,   328,
     329,   330,   469,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,     0,   344,   345,   346,     0,     0,
     349,   350,   351,   352,   470,   471,   571,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     474,   475,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   479,   480,   481,   482,   483,     0,   484,
       0,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,    59,   572,   497,   498,     0,     0,     0,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   502,
     503,   504,     0,    14,     0,     0,   505,   506,     0,     0,
     437,   438,     0,     0,   507,     0,   508,  1078,   509,   510,
     443,   444,   445,   446,   447,     0,     0,     0,     0,     0,
     448,     0,   450,     0,     0,     0,   453,     0,     0,     0,
       0,     0,     0,     0,   455,     0,     0,     0,     0,     0,
     458,     0,     0,   459,     0,     0,   460,     0,     0,     0,
     463,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     570,     0,     0,   467,   468,     0,   322,   323,   324,     0,
     326,   327,   328,   329,   330,   469,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,     0,   344,   345,
     346,     0,     0,   349,   350,   351,   352,   470,   471,   571,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   474,   475,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   479,   480,   481,   482,
     483,     0,   484,     0,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,    59,   572,   497,   498,     0,
       0,     0,     0,     0,     0,   499,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1129,     0,   502,   503,   504,     0,    14,     0,     0,   505,
     506,     0,     0,   437,   438,     0,     0,   507,     0,   508,
       0,   509,   510,   443,   444,   445,   446,   447,     0,     0,
       0,     0,     0,   448,     0,   450,     0,     0,     0,   453,
       0,     0,     0,     0,     0,     0,     0,   455,     0,     0,
       0,     0,     0,   458,     0,     0,   459,     0,     0,   460,
       0,     0,     0,   463,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   570,     0,     0,   467,   468,     0,   322,
     323,   324,     0,   326,   327,   328,   329,   330,   469,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
       0,   344,   345,   346,     0,     0,   349,   350,   351,   352,
     470,   471,   571,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   474,   475,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   479,
     480,   481,   482,   483,     0,   484,     0,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,    59,   572,
     497,   498,     0,     0,     0,     0,     0,     0,   499,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   502,   503,   504,     0,    14,
       0,     0,   505,   506,     0,     0,   437,   438,     0,     0,
     507,     0,   508,  1406,   509,   510,   443,   444,   445,   446,
     447,     0,     0,     0,     0,     0,   448,     0,   450,     0,
       0,     0,   453,     0,     0,     0,     0,     0,     0,     0,
     455,     0,     0,     0,     0,     0,   458,     0,     0,   459,
       0,     0,   460,     0,     0,     0,   463,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   570,     0,     0,   467,
     468,     0,   322,   323,   324,     0,   326,   327,   328,   329,
     330,   469,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,     0,   344,   345,   346,     0,     0,   349,
     350,   351,   352,   470,   471,   571,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   474,
     475,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   479,   480,   481,   482,   483,     0,   484,     0,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,    59,   572,   497,   498,     0,     0,     0,     0,     0,
       0,   499,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
     504,     0,    14,     0,     0,   505,   506,     0,     0,   437,
     438,     0,     0,  1415,     0,   508,  1416,   509,   510,   443,
     444,   445,   446,   447,     0,     0,     0,     0,     0,   448,
       0,   450,     0,     0,     0,   453,     0,     0,     0,     0,
       0,     0,     0,   455,     0,     0,     0,     0,     0,   458,
       0,     0,   459,     0,     0,   460,     0,     0,     0,   463,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   570,
       0,     0,   467,   468,     0,   322,   323,   324,     0,   326,
     327,   328,   329,   330,   469,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,     0,   344,   345,   346,
       0,     0,   349,   350,   351,   352,   470,   471,   571,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   474,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   479,   480,   481,   482,   483,
       0,   484,     0,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,    59,   572,   497,   498,     0,     0,
       0,     0,     0,     0,   499,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   502,   503,   504,     0,    14,     0,     0,   505,   506,
       0,     0,   437,   438,     0,     0,   507,     0,   508,  1421,
     509,   510,   443,   444,   445,   446,   447,     0,     0,     0,
       0,     0,   448,     0,   450,     0,     0,     0,   453,     0,
       0,     0,     0,     0,     0,     0,   455,     0,     0,     0,
       0,     0,   458,     0,     0,   459,     0,     0,   460,     0,
       0,     0,   463,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   570,     0,     0,   467,   468,     0,   322,   323,
     324,     0,   326,   327,   328,   329,   330,   469,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,     0,
     344,   345,   346,     0,     0,   349,   350,   351,   352,   470,
     471,   571,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   474,   475,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   479,   480,
     481,   482,   483,     0,   484,     0,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,    59,   572,   497,
     498,     0,     0,     0,     0,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   502,   503,   504,     0,    14,     0,
       0,   505,   506,     0,     0,   437,   438,     0,     0,   507,
       0,   508,  1463,   509,   510,   443,   444,   445,   446,   447,
       0,     0,     0,     0,     0,   448,     0,   450,     0,     0,
       0,   453,     0,     0,     0,     0,     0,     0,     0,   455,
       0,     0,     0,     0,     0,   458,     0,     0,   459,     0,
       0,   460,     0,     0,     0,   463,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   570,     0,     0,   467,   468,
       0,   322,   323,   324,     0,   326,   327,   328,   329,   330,
     469,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,     0,   344,   345,   346,     0,     0,   349,   350,
     351,   352,   470,   471,   571,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   474,   475,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   479,   480,   481,   482,   483,     0,   484,     0,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
      59,   572,   497,   498,     0,     0,     0,     0,     0,     0,
     499,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   502,   503,   504,
       0,    14,     0,     0,   505,   506,     0,     0,   437,   438,
       0,     0,   507,     0,   508,  1544,   509,   510,   443,   444,
     445,   446,   447,     0,     0,     0,     0,     0,   448,     0,
     450,     0,     0,     0,   453,     0,     0,     0,     0,     0,
       0,     0,   455,     0,     0,     0,     0,     0,   458,     0,
       0,   459,     0,     0,   460,     0,     0,     0,   463,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   570,     0,
       0,   467,   468,     0,   322,   323,   324,     0,   326,   327,
     328,   329,   330,   469,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,     0,   344,   345,   346,     0,
       0,   349,   350,   351,   352,   470,   471,   571,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   474,   475,     0,     0,     0,     0,     0,     0,     0,
    1581,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   479,   480,   481,   482,   483,     0,
     484,     0,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    59,   572,   497,   498,     0,     0,     0,
       0,     0,     0,   499,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     502,   503,   504,     0,    14,     0,     0,   505,   506,     0,
       0,   437,   438,     0,     0,   507,     0,   508,     0,   509,
     510,   443,   444,   445,   446,   447,     0,     0,     0,     0,
       0,   448,     0,   450,     0,     0,     0,   453,     0,     0,
       0,     0,     0,     0,     0,   455,     0,     0,     0,     0,
       0,   458,     0,     0,   459,     0,     0,   460,     0,     0,
       0,   463,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   570,     0,     0,   467,   468,     0,   322,   323,   324,
       0,   326,   327,   328,   329,   330,   469,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,     0,   344,
     345,   346,     0,     0,   349,   350,   351,   352,   470,   471,
     571,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   474,   475,     0,     0,     0,     0,
       0,     0,     0,  1623,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   479,   480,   481,
     482,   483,     0,   484,     0,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,    59,   572,   497,   498,
       0,     0,     0,     0,     0,     0,   499,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,   504,     0,    14,     0,     0,
     505,   506,     0,     0,   437,   438,     0,     0,   507,     0,
     508,     0,   509,   510,   443,   444,   445,   446,   447,     0,
       0,     0,     0,     0,   448,     0,   450,     0,     0,     0,
     453,     0,     0,     0,     0,     0,     0,     0,   455,     0,
       0,     0,     0,     0,   458,     0,     0,   459,     0,     0,
     460,     0,     0,     0,   463,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   570,     0,     0,   467,   468,     0,
     322,   323,   324,     0,   326,   327,   328,   329,   330,   469,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,     0,   344,   345,   346,     0,     0,   349,   350,   351,
     352,   470,   471,   571,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   474,   475,     0,
       0,     0,     0,     0,     0,     0,  1624,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     479,   480,   481,   482,   483,     0,   484,     0,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,    59,
     572,   497,   498,     0,     0,     0,     0,     0,     0,   499,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   502,   503,   504,     0,
      14,     0,     0,   505,   506,     0,     0,   437,   438,     0,
       0,   507,     0,   508,     0,   509,   510,   443,   444,   445,
     446,   447,     0,     0,     0,     0,     0,   448,     0,   450,
       0,     0,     0,   453,     0,     0,     0,     0,     0,     0,
       0,   455,     0,     0,     0,     0,     0,   458,     0,     0,
     459,     0,     0,   460,     0,     0,     0,   463,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   570,     0,     0,
     467,   468,     0,   322,   323,   324,     0,   326,   327,   328,
     329,   330,   469,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,     0,   344,   345,   346,     0,     0,
     349,   350,   351,   352,   470,   471,   571,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     474,   475,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   479,   480,   481,   482,   483,     0,   484,
       0,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,    59,   572,   497,   498,     0,     0,     0,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   502,
     503,   504,     0,    14,     0,     0,   505,   506,     0,     0,
     437,   438,     0,     0,   507,     0,   508,     0,   509,   510,
     443,   444,   445,   446,   447,     0,     0,     0,     0,     0,
     448,     0,   450,     0,     0,     0,   453,     0,     0,     0,
       0,     0,     0,     0,   455,     0,     0,     0,     0,     0,
     458,     0,     0,   459,     0,     0,   460,     0,     0,     0,
     463,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     570,     0,     0,   467,   468,     0,   322,   323,   324,     0,
     326,   327,   328,   329,   330,   469,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,     0,   344,   345,
     346,     0,     0,   349,   350,   351,   352,   470,   471,   571,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   474,   475,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   479,   480,   481,   482,
     483,     0,   484,     0,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,    59,   572,   497,   498,     0,
     697,     0,     0,     0,     0,   499,   316,     0,     0,     0,
       0,     0,   317,     0,     0,     0,     0,     0,   318,     0,
       0,     0,   502,   503,   504,     0,    14,     0,   319,   505,
     506,     0,     0,     0,     0,     0,   320,  1389,     0,   508,
       0,   509,   510,     0,     0,     0,     0,     0,     0,     0,
       0,   321,     0,     0,     0,     0,     0,     0,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   316,     0,     0,     0,
       0,     0,   317,     0,     0,     0,     0,     0,   318,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   319,     0,
      58,     0,     0,     0,     0,     0,   320,     0,     0,     0,
       0,     0,     0,   355,     0,     0,     0,     0,     0,     0,
       0,   321,     0,     0,     0,     0,     0,    59,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   356,     0,     0,   316,     0,     0,     0,
       0,     0,   317,     0,     0,     0,     0,     0,   318,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   319,     0,
      58,     0,     0,     0,     0,     0,   320,     0,     0,     0,
       0,     0,     0,   355,     0,     0,     0,     0,     0,     0,
       0,   321,     0,     0,     0,     0,     0,    59,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   356,     0,   610,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   355,     0,     0,     0,     0,     0,     0,
       0,     0,   316,     0,     0,     0,     0,   615,   317,     0,
       0,     0,     0,     0,   318,     0,     0,     0,     0,   616,
       0,     0,     0,     0,   319,   617,     0,     0,     0,     0,
       0,     0,   320,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   321,     0,     0,
       0,     0,     0,   356,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   316,     0,     0,     0,     0,     0,   317,     0,
       0,     0,     0,     0,   318,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   319,     0,    58,     0,     0,     0,
       0,     0,   320,     0,     0,     0,     0,     0,     0,   355,
       0,     0,     0,     0,     0,     0,     0,   321,     0,     0,
       0,     0,     0,    59,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   746,   747,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   355,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   615,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   878,   879,   880,   881,   882,   883,
     884,   885,   756,   757,   758,   759,   760,   886,   887,   763,
     764,   765,   766,   888,   768,   769,   770,   771,  -372,   356,
     746,   747,   772,   773,   774,   775,   889,   890,     0,     0,
     778,   779,   780,   891,   892,   893,   784,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   894,   786,     0,   787,   788,
     789,   790,   791,   792,   793,   794,   795,   796,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   797,   798,     0,
       0,     0,   509,   683,     0,     0,     0,   878,   879,   880,
     881,   882,   883,   884,   885,   756,   757,   758,   759,   760,
     886,   887,   763,   764,   765,   766,   888,   768,   769,   770,
     771,   746,   747,     0,     0,   772,   773,   774,   775,   889,
     890,     0,     0,   778,   779,   780,   891,   892,   893,   784,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1090,     0,     0,     0,     0,     0,     0,   894,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,     0,   509,   683,     0,   878,   879,
     880,   881,   882,   883,   884,   885,   756,   757,   758,   759,
     760,   886,   887,   763,   764,   765,   766,   888,   768,   769,
     770,   771,   746,   747,     0,     0,   772,   773,   774,   775,
     889,   890,     0,     0,   778,   779,   780,   891,   892,   893,
     784,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1290,     0,     0,     0,     0,     0,     0,   894,
     786,     0,   787,   788,   789,   790,   791,   792,   793,   794,
     795,   796,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   797,   798,     0,     0,     0,   509,   683,     0,   878,
     879,   880,   881,   882,   883,   884,   885,   756,   757,   758,
     759,   760,   886,   887,   763,   764,   765,   766,   888,   768,
     769,   770,   771,   746,   747,     0,     0,   772,   773,   774,
     775,   889,   890,     0,     0,   778,   779,   780,   891,   892,
     893,   784,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1674,     0,     0,     0,     0,     0,     0,
     894,   786,     0,   787,   788,   789,   790,   791,   792,   793,
     794,   795,   796,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   797,   798,     0,     0,     0,   509,   683,     0,
     878,   879,   880,   881,   882,   883,   884,   885,   756,   757,
     758,   759,   760,   886,   887,   763,   764,   765,   766,   888,
     768,   769,   770,   771,   746,   747,     0,     0,   772,   773,
     774,   775,   889,   890,     0,     0,   778,   779,   780,   891,
     892,   893,   784,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1698,     0,     0,     0,     0,     0,
       0,   894,   786,     0,   787,   788,   789,   790,   791,   792,
     793,   794,   795,   796,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   797,   798,     0,     0,     0,   509,   683,
       0,   878,   879,   880,   881,   882,   883,   884,   885,   756,
     757,   758,   759,   760,   886,   887,   763,   764,   765,   766,
     888,   768,   769,   770,   771,   746,   747,     0,     0,   772,
     773,   774,   775,   889,   890,     0,     0,   778,   779,   780,
     891,   892,   893,   784,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     746,   747,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1699,     0,     0,     0,     0,
       0,     0,   894,   786,     0,   787,   788,   789,   790,   791,
     792,   793,   794,   795,   796,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   797,   798,     0,     0,     0,   509,
     683,     0,   878,   879,   880,   881,   882,   883,   884,   885,
     756,   757,   758,   759,   760,   886,   887,   763,   764,   765,
     766,   888,   768,   769,   770,   771,     0,     0,     0,     0,
     772,   773,   774,   775,   889,   890,     0,     0,   778,   779,
     780,   891,   892,   893,   784,   756,   757,   758,   759,   760,
     746,   747,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,  1705,     0,     0,   784,
       0,     0,     0,   894,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,     0,
     509,   683,     0,     0,     0,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,   811,   756,   757,   758,   759,   760,
     746,   747,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,     0,     0,     0,   784,
       0,     0,     0,     0,     0,   746,   747,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,   821,   756,   757,   758,   759,   760,
       0,     0,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,     0,     0,     0,   784,
     756,   757,   758,   759,   760,   746,   747,   763,   764,   765,
     766,     0,   768,   769,   770,   771,     0,     0,     0,     0,
     772,     0,   774,   775,     0,     0,     0,     0,   778,   779,
     780,     0,     0,     0,   784,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,   837,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,   851,
     756,   757,   758,   759,   760,   746,   747,   763,   764,   765,
     766,     0,   768,   769,   770,   771,     0,     0,     0,     0,
     772,     0,   774,   775,     0,     0,     0,     0,   778,   779,
     780,     0,     0,     0,   784,     0,     0,     0,     0,     0,
     746,   747,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,  1161,
     756,   757,   758,   759,   760,     0,     0,   763,   764,   765,
     766,     0,   768,   769,   770,   771,     0,     0,     0,     0,
     772,     0,   774,   775,     0,     0,     0,     0,   778,   779,
     780,     0,     0,     0,   784,   756,   757,   758,   759,   760,
     746,   747,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,     0,     0,     0,   784,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,  1165,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,  1167,   756,   757,   758,   759,   760,
     746,   747,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,     0,     0,     0,   784,
       0,     0,     0,     0,     0,   746,   747,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,  1176,   756,   757,   758,   759,   760,
       0,     0,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,     0,     0,     0,   784,
     756,   757,   758,   759,   760,   746,   747,   763,   764,   765,
     766,     0,   768,   769,   770,   771,     0,     0,     0,     0,
     772,     0,   774,   775,     0,     0,     0,     0,   778,   779,
     780,     0,     0,     0,   784,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,  1177,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,  1178,
     756,   757,   758,   759,   760,   746,   747,   763,   764,   765,
     766,     0,   768,   769,   770,   771,     0,     0,     0,     0,
     772,     0,   774,   775,     0,     0,     0,     0,   778,   779,
     780,     0,     0,     0,   784,     0,     0,     0,     0,     0,
     746,   747,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,  1179,
     756,   757,   758,   759,   760,     0,     0,   763,   764,   765,
     766,     0,   768,   769,   770,   771,     0,     0,     0,     0,
     772,     0,   774,   775,     0,     0,     0,     0,   778,   779,
     780,     0,     0,     0,   784,   756,   757,   758,   759,   760,
     746,   747,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,     0,     0,     0,   784,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,  1180,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,  1181,   756,   757,   758,   759,   760,
     746,   747,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,     0,     0,     0,   784,
       0,     0,     0,     0,     0,   746,   747,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,  1311,   756,   757,   758,   759,   760,
       0,     0,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,     0,     0,     0,   784,
     756,   757,   758,   759,   760,   746,   747,   763,   764,   765,
     766,     0,   768,   769,   770,   771,     0,     0,     0,     0,
     772,     0,   774,   775,     0,     0,     0,     0,   778,   779,
     780,     0,     0,     0,   784,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,  1323,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,  1326,
     756,   757,   758,   759,   760,   746,   747,   763,   764,   765,
     766,     0,   768,   769,   770,   771,     0,     0,     0,     0,
     772,     0,   774,   775,     0,     0,     0,     0,   778,   779,
     780,     0,     0,     0,   784,     0,     0,     0,     0,     0,
     746,   747,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,  1374,
     756,   757,   758,   759,   760,     0,     0,   763,   764,   765,
     766,     0,   768,   769,   770,   771,     0,     0,     0,     0,
     772,     0,   774,   775,     0,     0,     0,     0,   778,   779,
     780,     0,     0,     0,   784,   756,   757,   758,   759,   760,
     746,   747,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,     0,     0,     0,   784,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,  1484,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,  1485,   756,   757,   758,   759,   760,
     746,   747,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,     0,     0,     0,   784,
       0,     0,     0,     0,     0,   746,   747,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,  1506,   756,   757,   758,   759,   760,
       0,     0,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,     0,     0,     0,   784,
     756,   757,   758,   759,   760,   746,   747,   763,   764,   765,
     766,     0,   768,   769,   770,   771,     0,     0,     0,     0,
     772,     0,   774,   775,     0,     0,     0,     0,   778,   779,
     780,     0,     0,     0,   784,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,  1508,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,  1510,
     756,   757,   758,   759,   760,   746,   747,   763,   764,   765,
     766,     0,   768,   769,   770,   771,     0,     0,     0,     0,
     772,     0,   774,   775,     0,     0,     0,     0,   778,   779,
     780,     0,     0,     0,   784,     0,     0,     0,     0,     0,
     746,   747,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,  1514,
     756,   757,   758,   759,   760,     0,     0,   763,   764,   765,
     766,     0,   768,   769,   770,   771,     0,     0,     0,     0,
     772,     0,   774,   775,     0,     0,     0,     0,   778,   779,
     780,     0,     0,     0,   784,   756,   757,   758,   759,   760,
     746,   747,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,     0,     0,     0,   784,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,  1562,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,  1586,   756,   757,   758,   759,   760,
     746,   747,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,     0,     0,     0,   784,
       0,     0,     0,     0,     0,   746,   747,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,  1587,   756,   757,   758,   759,   760,
       0,     0,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,     0,     0,     0,   784,
     756,   757,   758,   759,   760,   746,   747,   763,   764,   765,
     766,     0,   768,   769,   770,   771,     0,     0,     0,     0,
     772,     0,   774,   775,     0,     0,     0,     0,   778,   779,
     780,     0,     0,     0,   784,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,  1589,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,  1596,
     756,   757,   758,   759,   760,   746,   747,   763,   764,   765,
     766,     0,   768,   769,   770,   771,     0,     0,     0,     0,
     772,     0,   774,   775,     0,     0,     0,     0,   778,   779,
     780,     0,     0,     0,   784,     0,     0,     0,     0,     0,
     746,   747,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,  1599,
     756,   757,   758,   759,   760,     0,     0,   763,   764,   765,
     766,     0,   768,   769,   770,   771,     0,     0,     0,     0,
     772,     0,   774,   775,     0,     0,     0,     0,   778,   779,
     780,     0,     0,     0,   784,   756,   757,   758,   759,   760,
     746,   747,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,     0,     0,     0,   784,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,  1609,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,  1683,   756,   757,   758,   759,   760,
     746,   747,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,     0,     0,     0,   784,
       0,     0,     0,     0,     0,   746,   747,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,  1684,   756,   757,   758,   759,   760,
       0,     0,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,     0,     0,     0,   784,
     756,   757,   758,   759,   760,   746,   747,   763,   764,   765,
     766,     0,   768,   769,   770,   771,     0,     0,     0,     0,
     772,     0,   774,   775,     0,     0,     0,     0,   778,   779,
     780,     0,     0,     0,   784,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,   841,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,  1112,     0,     0,
     756,   757,   758,   759,   760,   746,   747,   763,   764,   765,
     766,     0,   768,   769,   770,   771,     0,     0,     0,     0,
     772,     0,   774,   775,     0,     0,     0,     0,   778,   779,
     780,     0,     0,     0,   784,     0,     0,     0,     0,     0,
     746,   747,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,  1262,     0,     0,
     756,   757,   758,   759,   760,     0,     0,   763,   764,   765,
     766,     0,   768,   769,   770,   771,     0,     0,     0,     0,
     772,     0,   774,   775,     0,     0,     0,     0,   778,   779,
     780,     0,     0,     0,   784,   756,   757,   758,   759,   760,
     746,   747,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,     0,     0,     0,   784,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,  1278,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,  1413,     0,     0,   756,   757,   758,   759,   760,
       0,     0,   763,   764,   765,   766,     0,   768,   769,   770,
     771,   236,   237,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,   778,   779,   780,     0,     0,   238,   784,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   746,   747,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,  1419,     0,     0,     0,     0,     0,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,     0,     0,   257,   258,
     259,     0,     0,     0,     0,     0,     0,   260,   261,   262,
     263,   264,     0,     0,   265,   266,   267,   268,   269,   270,
     271,   756,   757,   758,   759,   760,   746,   747,   763,   764,
     765,   766,     0,   768,   769,   770,   771,     0,     0,     0,
       0,   772,     0,   774,   775,     0,     0,     0,     0,   778,
     779,   780,     0,     0,     0,   784,     0,     0,     0,     0,
     272,     0,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,     0,     0,   283,   284,     0,     0,     0,     0,
       0,   285,   286,     0,     0,   291,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,     0,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   797,   798,     0,     0,
       0,   756,   757,   758,   759,   760,   746,   747,   763,   764,
     765,   766,     0,   768,   769,   770,   771,     0,     0,     0,
       0,   772,     0,   774,   775,     0,     0,   942,     0,   778,
     779,   780,     0,     0,     0,   784,     0,     0,     0,     0,
       0,   746,   747,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,     0,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   797,   798,     0,     0,
       0,   756,   757,   758,   759,   760,     0,     0,   763,   764,
     765,   766,     0,   768,   769,   770,   771,     0,     0,     0,
       0,   772,     0,   774,   775,     0,     0,  1187,     0,   778,
     779,   780,     0,     0,     0,   784,   756,   757,   758,   759,
     760,   746,   747,   763,   764,   765,   766,     0,   768,   769,
     770,   771,     0,     0,     0,     0,   772,     0,   774,   775,
       0,     0,     0,     0,   778,   779,   780,     0,     0,     0,
     784,     0,     0,     0,     0,   786,     0,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   797,   798,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     786,  1267,   787,   788,   789,   790,   791,   792,   793,   794,
     795,   796,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   797,   798,     0,     0,     0,   756,   757,   758,   759,
     760,   746,   747,   763,   764,   765,   766,     0,   768,   769,
     770,   771,     0,     0,     0,     0,   772,     0,   774,   775,
       0,     0,     0,     0,   778,   779,   780,     0,     0,     0,
     784,     0,     0,     0,     0,     0,   746,   747,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1280,     0,     0,     0,     0,     0,     0,     0,
     786,     0,   787,   788,   789,   790,   791,   792,   793,   794,
     795,   796,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   797,   798,     0,     0,     0,   756,   757,   758,   759,
     760,     0,     0,   763,   764,   765,   766,     0,   768,   769,
     770,   771,     0,     0,     0,     0,   772,     0,   774,   775,
       0,     0,     0,     0,   778,   779,   780,     0,     0,     0,
     784,   756,   757,   758,   759,   760,   746,   747,   763,   764,
     765,   766,     0,   768,   769,   770,   771,     0,     0,     0,
       0,   772,     0,   774,   775,     0,     0,     0,     0,   778,
     779,   780,     0,     0,     0,   784,     0,     0,     0,     0,
     786,  1368,   787,   788,   789,   790,   791,   792,   793,   794,
     795,   796,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   797,   798,     0,     0,     0,     0,  1610,     0,     0,
       0,     0,     0,     0,     0,   786,     0,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   797,   798,     0,     0,
       0,   756,   757,   758,   759,   760,   746,   747,   763,   764,
     765,   766,     0,   768,   769,   770,   771,     0,     0,     0,
       0,   772,     0,   774,   775,     0,     0,     0,     0,   778,
     779,   780,     0,     0,     0,   784,     0,     0,     0,     0,
       0,   746,   747,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,     0,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   797,   798,     0,     0,
       0,   756,   757,   758,   759,   760,     0,     0,   763,   764,
     765,   766,     0,   768,   769,   770,   771,     0,     0,     0,
       0,   772,     0,   774,   775,     0,     0,     0,     0,   778,
     779,   780,     0,     0,     0,  -773,   756,   757,   758,   759,
     760,   746,   747,   763,   764,   765,   766,     0,   768,   769,
     770,   771,     0,     0,     0,     0,   772,     0,   774,   775,
       0,     0,     0,     0,   778,   779,   780,     0,     0,     0,
       0,     0,   746,   747,     0,   786,     0,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   797,   798,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     786,     0,   787,   788,   789,   790,   791,   792,   793,   794,
     795,   796,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   797,   798,     0,     0,     0,   756,   757,   758,   759,
     760,     0,     0,   763,   764,   765,   766,     0,   768,   769,
     770,   771,     0,     0,     0,     0,   772,     0,   774,   775,
       0,     0,   746,   747,   778,     0,   780,   756,   757,   758,
     759,   760,     0,     0,   763,   764,   765,   766,     0,   768,
     769,   770,   771,     0,     0,     0,     0,   772,     0,   774,
     775,   746,   747,     0,     0,   778,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   787,   788,   789,   790,   791,   792,   793,   794,
     795,   796,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   797,   798,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   787,   788,   789,   790,   791,   792,   793,
     794,   795,   796,     0,     0,     0,     0,   756,   757,   758,
     759,   760,   797,   798,   763,   764,   765,   766,     0,   768,
     769,   770,   771,     0,     0,     0,     0,   772,     0,   774,
     775,   746,   747,     0,     0,     0,   756,   757,   758,   759,
     760,     0,     0,   763,   764,   765,   766,     0,   768,   769,
     770,   771,     0,     0,     0,     0,   772,     0,   774,   775,
     746,   747,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   787,   788,   789,   790,   791,   792,   793,
     794,   795,   796,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   797,   798,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   788,   789,   790,   791,   792,   793,   794,
     795,   796,     0,     0,     0,     0,   756,   757,   758,   759,
     760,   797,   798,   763,   764,   765,   766,     0,   768,   769,
     770,   771,     0,     0,     0,     0,   772,     0,   774,   775,
       0,     0,     0,     0,     0,   756,   757,   758,   759,   760,
       0,     0,   763,   764,   765,   766,     0,   768,   769,   770,
     771,     0,     0,     0,     0,   772,     0,   774,   775,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     990,     0,     0,     0,   789,   790,   791,   792,   793,   794,
     795,   796,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   797,   798,     0,     0,     0,     0,     0,     0,     0,
       0,   994,     0,     0,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,   322,   323,   324,     0,   326,   327,   328,   329,
     330,   469,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,     0,   344,   345,   346,     0,     0,   349,
     350,   351,   352,   322,   323,   324,     0,   326,   327,   328,
     329,   330,   469,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,     0,   344,   345,   346,     0,     0,
     349,   350,   351,   352,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   991,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   992,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     995,     0,     0,     0,     0,     0,     0,     0,     0,   322,
     323,   324,   996,   326,   327,   328,   329,   330,   469,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
       0,   344,   345,   346,     0,     0,   349,   350,   351,   352,
     322,   323,   324,     0,   326,   327,   328,   329,   330,   469,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,     0,   344,   345,   346,     0,     0,   349,   350,   351,
     352,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   186,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1264,     0,  1069,  1070,
       0,     0,     0,     0,     0,     0,     0,     0,  1265,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   187,     0,
     188,     0,   189,   190,   191,   192,   193,  1071,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,  1072,
     205,   206,   207,     0,     0,   208,   209,   210,   211,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   212,   213,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1073,  1074,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   214
};

static const yytype_int16 yycheck[] =
{
      14,    15,   216,   449,   577,   381,   630,   488,   720,   526,
     655,   600,   724,   165,   597,   381,   696,   591,   698,   593,
     735,   595,   698,   388,   425,   426,   596,   473,  1189,   543,
      82,     5,   919,     8,    53,    20,   828,   232,   830,   635,
     832,   105,   477,   478,    20,  1392,   127,    20,    20,    33,
      64,    65,    66,   567,    57,   933,    15,    16,    20,   126,
      63,   939,   129,   130,  1563,    22,    33,   163,   127,     4,
      53,    20,   170,   449,    20,   143,    46,   394,    19,    20,
     152,   476,   477,   478,   381,    21,    22,   132,   160,   144,
     145,   146,   106,   107,   108,   109,   170,   473,   170,   695,
      57,   697,   170,   699,   158,   143,   423,   188,   189,   180,
     191,   707,   127,   194,   143,   155,   170,     0,   216,   180,
     160,   717,   137,  1622,     7,   182,    21,    22,   102,   103,
     189,   526,   189,   192,   160,   581,   106,   182,   180,  1590,
     214,   170,   214,   214,   211,   212,   214,    30,  1495,    32,
     865,    34,   449,   191,   306,   216,   217,    40,   553,   223,
     215,   215,   874,   182,    49,     5,     6,    50,     8,   486,
     487,   685,   224,    56,   189,   158,   473,   217,   181,   232,
      15,    16,   118,   119,   379,    21,    22,   170,  1639,  1350,
     126,   217,   128,   129,   130,   131,    36,    80,   910,   394,
     136,   913,   648,   188,   163,   581,   181,   182,    93,   168,
     194,   170,   188,     7,   173,   188,   188,   618,   232,   102,
     103,   195,   623,   118,   119,   160,   188,   194,   423,   170,
     425,   126,   678,   128,   129,   130,   131,   212,   452,   188,
     214,   136,   188,   200,   182,   212,   185,   186,   129,   130,
     182,   218,   466,   152,   211,   214,    50,   652,   310,   127,
     127,    33,   977,   199,   200,   201,  1595,   107,   158,   137,
     137,   170,   648,  1457,   213,   211,   212,  1606,   189,   217,
     170,   182,   118,   119,   581,   680,   127,  1174,    60,    61,
     126,   486,   487,   129,   130,   131,   137,   127,   978,   182,
     136,    62,   678,   170,  1550,  1466,  1552,   137,  1186,   744,
     188,  1557,  1558,   143,   215,   191,   211,   212,   201,  1648,
    1649,   189,   189,   142,   214,    34,   379,   194,   163,   212,
     211,   212,   188,   168,   155,   170,   188,   654,   173,   160,
    1524,  1525,   356,   127,   188,   127,  1233,    47,   189,   744,
     195,   648,   124,   137,    63,   137,   128,   213,  1542,   189,
     677,   180,   170,   959,   182,   379,   188,    67,   814,   214,
     384,   215,  1618,   143,   182,   211,   212,   742,   196,   188,
     394,   678,   152,   188,   170,   171,   172,   582,   583,   584,
     182,   586,   587,   215,   215,   590,   217,   592,   107,   594,
     170,   596,  1289,   188,   196,   189,   215,   189,   213,   423,
    1125,   425,   188,   859,   988,   201,  1600,  1601,   190,   820,
     634,   816,   194,   188,   196,   197,   143,   136,   195,   170,
     215,   445,   446,   834,   143,   836,   182,   213,   814,   185,
     212,   188,   216,   217,   188,   659,   218,   214,   188,   188,
     215,   748,   749,   750,   751,   752,   753,   754,   755,   654,
    1252,   170,   976,   944,   761,   762,   188,   213,   215,   213,
     767,  1349,   486,   487,   955,   215,   215,   188,   188,   776,
     777,  1359,   677,   859,   781,   782,   783,   188,   785,   152,
     704,   213,   927,   810,  1118,   896,   205,   170,   188,   188,
     188,   188,   195,   213,   215,   214,  1186,   170,  1188,   182,
    1186,   180,   214,   188,   215,   188,   730,   814,    33,   188,
    1200,   214,   917,   213,  1200,  1108,   215,   215,   215,   582,
     583,   584,   927,   586,   587,   191,  1106,   590,   213,   592,
     196,   594,   163,   860,   861,    60,    61,   170,   191,     5,
       6,   746,   747,   196,  1346,   195,   873,   211,   214,   182,
     195,   191,   859,   998,   999,  1000,   196,   170,   582,   583,
     584,   214,   586,   587,   214,   158,   590,   772,   592,   214,
     594,   144,   596,   146,   214,   902,   903,   170,   905,  1381,
     174,   175,   909,   182,   911,   912,    33,   914,  1222,   188,
     180,  1175,   797,   998,   999,  1000,   170,   196,  1003,   124,
    1005,   191,  1007,   128,  1009,   810,  1011,   947,  1013,   949,
    1015,   158,  1017,    60,    61,   191,   640,  1022,   182,  1024,
     196,   182,   143,   170,   188,  1030,   182,   188,    57,   158,
     654,   152,   196,   838,    63,   196,   157,  1042,   214,  1044,
     845,   170,  1226,   848,  1049,   158,  1051,   809,  1053,   170,
     158,  1056,   857,   677,   192,   860,   861,   170,   196,   864,
       5,     6,   170,   191,   869,   190,   194,   182,   873,   194,
     188,   895,   197,   191,   195,   170,   194,   124,  1333,    12,
      25,   128,    21,    22,  1089,   170,    31,   212,   182,  1491,
      23,    24,   192,   218,   188,   182,   196,   902,   903,  1389,
     905,   188,   196,   727,   909,   192,   911,   912,   182,   914,
      64,    65,    66,   182,   188,  1120,   192,   192,   170,   188,
     196,   196,   196,    68,    69,  1415,   181,   196,    57,   184,
     170,    57,   187,    57,    63,   182,    33,    63,   174,    63,
    1374,   188,   947,   190,   949,    57,   170,   194,   180,   196,
     197,    63,   106,   107,   108,   109,    57,   102,   103,    57,
     182,    57,    63,    60,    61,   212,   188,    63,   182,   192,
     182,   218,   182,   196,   188,   838,   188,   116,   117,   118,
     119,   120,   845,   807,   123,   848,   810,   126,   174,   128,
     129,   130,   131,   214,   857,   216,   217,   136,   143,   138,
     139,   864,  1386,   144,   182,   182,   869,  1134,  1135,  1136,
     188,   188,   106,   182,   838,   160,   161,   162,  1345,   188,
     182,   845,    88,    89,   848,   170,   188,   124,  1214,   189,
      35,   128,  1159,   857,  1433,   180,   860,   861,  1214,   192,
     864,    35,   192,   196,   192,   869,   196,   214,   196,   873,
     195,    75,  1057,  1543,   170,    79,   195,   196,   197,   198,
     199,   200,   201,   180,   192,   180,   180,   212,   196,    93,
      94,   180,   211,   212,    98,    99,   100,   101,   902,   903,
    1535,   905,   181,  1605,   947,   909,   949,   911,   912,  1094,
     914,   191,  1614,   190,   194,   170,   170,   194,  1225,   196,
     197,  1106,   192,   170,  1594,  1595,   196,  1214,   213,    21,
      22,  1322,  1602,   192,  1497,   212,  1606,   196,    22,   170,
    1642,   218,   213,   947,   180,   949,   191,   191,  1562,  1134,
    1135,  1136,   180,   214,  1533,   174,   175,   176,   177,   192,
    1345,   181,   182,    43,   184,   144,   145,   146,   195,   195,
    1640,   170,   171,   172,  1159,  1554,   214,   195,  1648,  1649,
     174,   175,   176,   174,   175,   176,   181,   182,   183,   195,
      33,    10,    11,    12,   195,   195,   137,   214,   195,   195,
    1471,   214,   195,   195,   170,  1476,   195,   214,   213,   170,
     170,   170,   449,   195,  1577,  1578,   214,    60,    61,   195,
     195,   170,   212,  1227,   116,   117,   118,   119,   195,   182,
     195,   214,  1702,   195,   126,   163,   128,   129,   130,   131,
    1225,   195,   214,   381,   136,   195,   138,   139,   195,   195,
     214,  1094,    37,   391,   217,   182,   214,   214,   214,   214,
     182,    33,   400,    10,   182,  1628,   182,   182,   182,   182,
      66,   508,   410,   212,   214,    21,    22,   181,   214,    43,
     215,   124,   420,   195,   127,   128,   195,   170,    60,    61,
    1094,   429,   170,   213,   137,  1402,   182,   191,    43,   181,
     181,   439,  1106,   214,   181,   197,   198,   199,   200,   201,
     195,   449,  1316,   214,   132,    14,   454,    33,   456,   211,
     212,   195,   195,   188,   191,  1310,   189,   465,   163,   217,
    1134,  1135,  1136,   181,   188,   473,   474,   475,   180,   182,
      13,   213,   188,  1579,    60,    61,   189,   190,   214,   188,
     488,   194,   124,   215,   197,  1159,   128,   170,   496,     8,
     170,   499,   196,   215,   502,   503,   504,   505,   506,   212,
     170,   170,   118,   119,   215,   218,   170,   515,   181,  1383,
     126,   188,   128,   129,   130,   131,   215,   214,   214,  1496,
     136,   215,   170,  1629,  1630,     1,    67,  1401,   214,   214,
     195,   181,   195,   214,   170,   214,   214,   182,   124,   214,
     647,   648,   128,  1579,    33,   196,   214,  1402,   190,  1404,
     196,  1225,   194,   214,   196,   197,   196,    43,   215,   215,
     170,   215,   214,  1669,   170,   672,   215,   214,   675,   213,
     212,    60,    61,   581,   215,   213,   218,  1632,   213,   170,
     170,   197,   198,   199,   200,   201,   214,   214,  1462,   215,
     170,   195,   214,  1629,  1630,   211,   212,  1310,   182,  1576,
     214,   609,   214,   214,   190,   170,   215,   170,   194,   214,
     196,   197,   213,   215,   178,   182,  1671,   170,  1673,   213,
     170,   213,  1579,    33,   170,   214,   212,   214,   735,   214,
     196,   214,   218,  1669,   215,   124,  1310,   214,   214,   128,
     648,  1496,  1697,    70,   214,   196,   214,   655,   656,   196,
     658,   182,   214,   174,   215,   663,  1511,   215,   214,   214,
     214,   669,   214,    53,   181,   181,   215,   215,   213,   181,
     678,   215,  1629,  1630,   188,   215,   215,   213,   182,   188,
     215,   689,   690,   691,   692,   693,   694,   215,   696,   215,
     698,  1404,   215,   800,   181,   215,    81,   215,   213,   706,
    1574,   190,   213,   213,     1,   194,   214,   196,   197,   215,
     215,    44,  1669,   137,    84,  1488,   807,  1489,   957,  1489,
    1489,  1576,  1489,   212,  1489,     1,   983,   230,  1402,   218,
    1404,  1399,  1462,  1244,  1425,  1521,  1428,  1285,   433,  1522,
     748,   749,   750,   751,   752,   753,   754,   755,   756,   757,
      52,   565,   760,   761,   762,   763,   764,   765,   865,   767,
     768,   445,   770,   771,   772,   773,   774,   775,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,   786,   787,
     788,   789,   790,   791,   792,   793,   794,   795,   796,  1602,
     798,  1469,   445,  1195,   802,    -1,   698,    -1,  1511,    -1,
      -1,    -1,    -1,  1677,    -1,    33,   814,   749,   750,   751,
     752,   753,   754,   755,    -1,    -1,    -1,    -1,    -1,   761,
     762,    -1,  1496,   930,    -1,   767,    -1,    -1,    -1,    -1,
      -1,   938,    60,    61,   776,   777,    -1,  1511,    -1,   781,
     782,   783,    -1,   785,   852,    -1,    -1,    -1,    -1,    -1,
      -1,   859,    -1,    -1,    -1,    -1,    -1,    -1,   866,    -1,
     868,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   877,
     878,   879,   880,   881,   882,   883,   884,   885,   886,   887,
     888,   889,   890,   891,   892,   893,   894,    -1,    -1,  1563,
      -1,    -1,    -1,    -1,    10,    -1,   124,    -1,    -1,   127,
     128,    -1,  1576,    -1,    -1,    21,    22,    -1,    -1,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     928,   929,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,   942,    -1,   944,    -1,    -1,    -1,
      21,    22,    -1,   951,    -1,    -1,    -1,   955,  1622,    -1,
      -1,    -1,    -1,   961,    -1,   963,   964,   965,    -1,    -1,
      -1,   189,   190,    -1,    -1,    -1,   194,   975,    -1,   197,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   212,    -1,    -1,    -1,    -1,    -1,
     218,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,  1123,    -1,    -1,    -1,
     136,   137,   138,   139,   140,   141,    -1,    -1,   144,   145,
     146,   147,   148,   149,   150,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      33,    -1,  1070,   144,   145,   146,  1074,    -1,    -1,   150,
      -1,    -1,    -1,   189,   190,  1182,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    60,    61,    -1,
      -1,    -1,  1199,    -1,    -1,   211,   212,    -1,  1205,    -1,
     216,   217,    -1,    -1,    -1,  1212,    -1,    -1,  1215,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,   124,    -1,  1260,  1162,   128,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1272,    33,    -1,    -1,    -1,
    1277,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,  1187,
      -1,    -1,    -1,    60,    61,    -1,  1194,  1195,    -1,    -1,
      -1,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1214,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    -1,  1223,  1224,   190,    -1,    19,
      -1,   194,  1230,   196,   197,    25,    -1,    -1,    -1,    -1,
    1238,    31,    -1,    -1,    -1,    -1,    -1,    -1,   124,   212,
    1347,    41,   128,    -1,    -1,   218,  1254,   124,  1256,    49,
      -1,   128,    -1,  1261,    -1,    -1,    -1,   124,    -1,  1267,
      -1,   128,    -1,  1271,    64,    -1,  1373,    -1,    -1,    -1,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   190,  1313,    -1,    -1,   194,    -1,
     196,   197,    -1,   190,    -1,    -1,    -1,   194,    -1,   196,
     197,    -1,    -1,   190,    -1,  1333,   212,   194,    -1,   196,
     197,    -1,   218,    -1,    -1,   212,    -1,    -1,    -1,    -1,
      -1,   218,    -1,   143,    -1,   212,    -1,    -1,    -1,    -1,
      -1,   218,    -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1375,  1376,  1377,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1410,    -1,  1412,    21,    22,    -1,    -1,    -1,
    1418,    60,    61,    -1,    -1,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1539,    -1,  1442,    -1,    -1,  1445,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1453,  1454,  1455,    -1,    -1,
      -1,    -1,  1460,    -1,    -1,    -1,    -1,  1465,    -1,  1566,
    1468,    -1,    -1,  1471,  1472,    -1,    -1,    -1,  1476,  1477,
      -1,  1479,  1480,    -1,    -1,   124,    -1,    -1,    -1,   128,
      -1,  1489,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1502,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,  1623,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,  1535,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,    -1,    -1,   194,    -1,   196,   197,    -1,
      -1,  1559,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1567,
      -1,    -1,    -1,   212,    -1,    -1,    -1,    -1,    -1,   218,
      -1,  1579,    -1,  1581,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,  1597,
    1598,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,    -1,
     216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1619,  1620,    -1,    -1,    -1,  1624,    -1,    -1,     1,
      -1,  1629,  1630,     5,     6,     7,    -1,     9,    10,    11,
      -1,    13,    -1,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,  1656,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,
      42,  1669,    44,    45,    -1,    -1,    48,    -1,    50,    51,
      52,    -1,    54,    55,    -1,    -1,    58,    59,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   154,   155,    -1,   157,    -1,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,    -1,    -1,    -1,    -1,    -1,    33,   180,   181,
     182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,   199,    -1,   201,
      -1,    -1,   204,   205,    60,    61,    -1,    -1,    -1,    -1,
     212,    -1,   214,     1,   216,   217,    -1,     5,     6,     7,
      -1,     9,    10,    11,    -1,    13,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    29,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    39,    -1,    -1,    42,    -1,    44,    45,    -1,    -1,
      48,    -1,    50,    51,    52,    -1,    54,    55,   124,    -1,
      58,    59,   128,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,    -1,    -1,   190,    -1,    -1,    -1,   194,    -1,
     196,   197,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,   212,    -1,    -1,    -1,
      -1,    -1,   218,   151,   152,   153,   154,   155,    -1,   157,
      -1,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   181,   182,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,   199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,
       5,     6,    -1,    -1,   212,    -1,   214,    -1,   216,   217,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,
     155,    33,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,   190,    -1,    -1,    -1,   194,
      -1,    -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,
     205,    -1,    -1,    -1,     5,     6,    -1,   212,    -1,   214,
      -1,   216,   217,   218,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,   124,    -1,    45,    -1,   128,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    60,
      61,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,   190,    -1,
      -1,    -1,   194,    -1,   196,   197,    -1,   118,   119,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,   128,    -1,    -1,
     212,    -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    33,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,    -1,    33,    -1,    -1,    -1,    -1,   180,
      -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,   190,
      -1,    -1,    -1,   194,    -1,    -1,   197,   198,   199,    -1,
     201,    60,    61,   204,   205,    -1,    -1,    -1,     5,     6,
      -1,   212,    -1,   214,    -1,   216,   217,   218,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,   124,    -1,    45,    -1,
     128,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    65,   128,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,   190,    -1,    -1,    -1,   194,    -1,   196,   197,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,    -1,   212,   194,    -1,   196,   197,    -1,
     218,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,   212,   151,   152,   153,   154,   155,   218,
     157,    -1,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,    -1,
     197,   198,   199,    -1,   201,    -1,    -1,   204,   205,    -1,
      -1,    -1,     5,     6,    -1,   212,    -1,   214,    -1,   216,
     217,   218,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,    -1,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,   198,   199,    -1,   201,    -1,
      -1,   204,   205,    -1,    -1,     5,     6,    -1,    -1,   212,
      -1,   214,    -1,   216,   217,    15,    16,    17,    18,    19,
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
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,
      -1,   201,    -1,    -1,   204,   205,    -1,    -1,     5,     6,
      -1,    -1,   212,    -1,   214,    -1,   216,   217,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    52,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,    -1,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,   199,    -1,   201,    -1,    -1,   204,   205,    -1,
      -1,     5,     6,    -1,    -1,   212,    -1,   214,    -1,   216,
     217,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,   199,    -1,   201,    -1,    -1,
     204,   205,    -1,    -1,     5,     6,    -1,    -1,   212,    -1,
     214,    -1,   216,   217,    15,    16,    17,    18,    19,    -1,
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
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,    -1,
     201,    -1,    -1,   204,   205,    -1,    -1,     5,     6,    -1,
      -1,   212,    -1,   214,   215,   216,   217,    15,    16,    17,
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
      -1,    -1,    -1,   151,   152,   153,   154,   155,    33,   157,
      -1,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,    -1,    60,    61,    -1,    -1,   190,
      -1,    -1,    -1,   194,    -1,   196,   197,    -1,    -1,   197,
     198,   199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,
      -1,   212,     5,     6,   212,    -1,   214,   218,   216,   217,
      13,    -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    33,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,   124,
      -1,    -1,    45,   128,    -1,    48,    49,    -1,    51,    -1,
      -1,    -1,    55,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,   190,    -1,    -1,    -1,   194,
      -1,   196,   197,    -1,    -1,   118,   119,    -1,   124,    -1,
      -1,    -1,   128,    -1,    -1,    -1,    -1,   212,    -1,    -1,
      -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    33,   157,    -1,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,
      60,    61,    -1,    -1,   190,    -1,    -1,    -1,   194,    -1,
     196,   197,    -1,    -1,   197,   198,   199,    -1,   201,    -1,
      -1,   204,   205,    -1,    -1,    -1,   212,     5,     6,   212,
     213,   214,   218,   216,   217,    13,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,   124,    -1,    -1,    45,   128,    -1,
      48,    49,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
     190,    -1,    -1,    -1,   194,    -1,   196,   197,    -1,    -1,
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   212,    -1,    -1,    -1,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,   199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,
      -1,    -1,     5,     6,   212,    -1,   214,    -1,   216,   217,
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
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,   198,   199,    -1,   201,    -1,
      -1,   204,   205,    -1,    -1,    -1,    -1,     5,     6,   212,
     213,   214,    -1,   216,   217,    13,    -1,    15,    16,    17,
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
      -1,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,   199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,
       5,     6,    -1,    -1,   212,    -1,   214,    -1,   216,   217,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    70,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,
     205,    -1,    -1,     5,     6,    -1,    -1,   212,    -1,   214,
      -1,   216,   217,    15,    16,    17,    18,    19,    -1,    -1,
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
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,   199,    -1,   201,
      -1,    -1,   204,   205,    -1,    -1,     5,     6,    -1,    -1,
     212,    -1,   214,   215,   216,   217,    15,    16,    17,    18,
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
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,     5,
       6,    -1,    -1,   212,    -1,   214,   215,   216,   217,    15,
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
      -1,   157,    -1,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,   205,
      -1,    -1,     5,     6,    -1,    -1,   212,   213,   214,    -1,
     216,   217,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,   198,   199,    -1,   201,    -1,
      -1,   204,   205,    -1,    -1,     5,     6,    -1,    -1,   212,
      -1,   214,    -1,   216,   217,    15,    16,    17,    18,    19,
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
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,
      -1,   201,    -1,    -1,   204,   205,    -1,    -1,    -1,    -1,
       5,     6,   212,    -1,   214,    -1,   216,   217,    13,    -1,
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
     155,    -1,   157,    -1,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,
     205,    -1,    -1,     5,     6,    -1,    -1,   212,    -1,   214,
      -1,   216,   217,    15,    16,    17,    18,    19,    -1,    -1,
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
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,    -1,   197,   198,   199,    -1,   201,
      -1,    -1,   204,   205,    -1,    -1,    -1,    -1,     5,     6,
     212,    -1,   214,    -1,   216,   217,    13,    -1,    15,    16,
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
     157,    -1,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,   199,    -1,   201,    -1,    -1,   204,   205,    -1,
      -1,     5,     6,    -1,    -1,   212,    -1,   214,    -1,   216,
     217,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,   199,    -1,   201,    -1,    -1,
     204,   205,    -1,    -1,     5,     6,    -1,    -1,   212,    -1,
     214,    -1,   216,   217,    15,    16,    17,    18,    19,    -1,
      -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
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
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,    -1,
     201,    -1,    -1,   204,   205,    -1,    -1,     5,     6,    -1,
      -1,   212,    -1,   214,    -1,   216,   217,    15,    16,    17,
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
      -1,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,   199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,
       5,     6,    -1,    -1,   212,    -1,   214,   215,   216,   217,
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
     155,    -1,   157,    -1,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,    -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,
     205,    -1,    -1,     5,     6,    -1,    -1,   212,    -1,   214,
      -1,   216,   217,    15,    16,    17,    18,    19,    -1,    -1,
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
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,   199,    -1,   201,
      -1,    -1,   204,   205,    -1,    -1,     5,     6,    -1,    -1,
     212,    -1,   214,   215,   216,   217,    15,    16,    17,    18,
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
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,     5,
       6,    -1,    -1,   212,    -1,   214,   215,   216,   217,    15,
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
      -1,   157,    -1,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,   205,
      -1,    -1,     5,     6,    -1,    -1,   212,    -1,   214,   215,
     216,   217,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,   198,   199,    -1,   201,    -1,
      -1,   204,   205,    -1,    -1,     5,     6,    -1,    -1,   212,
      -1,   214,   215,   216,   217,    15,    16,    17,    18,    19,
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
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,
      -1,   201,    -1,    -1,   204,   205,    -1,    -1,     5,     6,
      -1,    -1,   212,    -1,   214,   215,   216,   217,    15,    16,
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
     157,    -1,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,   199,    -1,   201,    -1,    -1,   204,   205,    -1,
      -1,     5,     6,    -1,    -1,   212,    -1,   214,    -1,   216,
     217,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,   199,    -1,   201,    -1,    -1,
     204,   205,    -1,    -1,     5,     6,    -1,    -1,   212,    -1,
     214,    -1,   216,   217,    15,    16,    17,    18,    19,    -1,
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
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,    -1,
     201,    -1,    -1,   204,   205,    -1,    -1,     5,     6,    -1,
      -1,   212,    -1,   214,    -1,   216,   217,    15,    16,    17,
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
      -1,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,   199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,
       5,     6,    -1,    -1,   212,    -1,   214,    -1,   216,   217,
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
     155,    -1,   157,    -1,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,    -1,
      13,    -1,    -1,    -1,    -1,   180,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,   197,   198,   199,    -1,   201,    -1,    41,   204,
     205,    -1,    -1,    -1,    -1,    -1,    49,   212,    -1,   214,
      -1,   216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,   170,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,   170,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,   170,    25,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,   182,
      -1,    -1,    -1,    -1,    41,   188,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,   216,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,   170,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   216,
      21,    22,   136,   137,   138,   139,   140,   141,    -1,    -1,
     144,   145,   146,   147,   148,   149,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,
      -1,    -1,   216,   217,    -1,    -1,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,   137,   138,   139,   140,
     141,    -1,    -1,   144,   145,   146,   147,   148,   149,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   182,    -1,    -1,    -1,    -1,    -1,    -1,   189,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,    -1,   216,   217,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,    21,    22,    -1,    -1,   136,   137,   138,   139,
     140,   141,    -1,    -1,   144,   145,   146,   147,   148,   149,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   182,    -1,    -1,    -1,    -1,    -1,    -1,   189,
     190,    -1,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   211,   212,    -1,    -1,    -1,   216,   217,    -1,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,    21,    22,    -1,    -1,   136,   137,   138,
     139,   140,   141,    -1,    -1,   144,   145,   146,   147,   148,
     149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   182,    -1,    -1,    -1,    -1,    -1,    -1,
     189,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,    -1,    -1,    -1,   216,   217,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,    21,    22,    -1,    -1,   136,   137,
     138,   139,   140,   141,    -1,    -1,   144,   145,   146,   147,
     148,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   182,    -1,    -1,    -1,    -1,    -1,
      -1,   189,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,    -1,   216,   217,
      -1,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,    21,    22,    -1,    -1,   136,
     137,   138,   139,   140,   141,    -1,    -1,   144,   145,   146,
     147,   148,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   182,    -1,    -1,    -1,    -1,
      -1,    -1,   189,   190,    -1,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   211,   212,    -1,    -1,    -1,   216,
     217,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,   141,    -1,    -1,   144,   145,
     146,   147,   148,   149,   150,   116,   117,   118,   119,   120,
      21,    22,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,   182,    -1,    -1,   150,
      -1,    -1,    -1,   189,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,    -1,
     216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,   215,   116,   117,   118,   119,   120,
      21,    22,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,   215,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
     116,   117,   118,   119,   120,    21,    22,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,   215,
     116,   117,   118,   119,   120,    21,    22,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,   215,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,   116,   117,   118,   119,   120,
      21,    22,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,   215,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,   215,   116,   117,   118,   119,   120,
      21,    22,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,   215,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
     116,   117,   118,   119,   120,    21,    22,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,   215,
     116,   117,   118,   119,   120,    21,    22,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,   215,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,   116,   117,   118,   119,   120,
      21,    22,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,   215,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,   215,   116,   117,   118,   119,   120,
      21,    22,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,   215,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
     116,   117,   118,   119,   120,    21,    22,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,   215,
     116,   117,   118,   119,   120,    21,    22,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,   215,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,   116,   117,   118,   119,   120,
      21,    22,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,   215,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,   215,   116,   117,   118,   119,   120,
      21,    22,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,   215,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
     116,   117,   118,   119,   120,    21,    22,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,   215,
     116,   117,   118,   119,   120,    21,    22,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,   215,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,   116,   117,   118,   119,   120,
      21,    22,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,   215,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,   215,   116,   117,   118,   119,   120,
      21,    22,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,   215,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
     116,   117,   118,   119,   120,    21,    22,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,   215,
     116,   117,   118,   119,   120,    21,    22,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,   215,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,   116,   117,   118,   119,   120,
      21,    22,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,   215,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,   215,   116,   117,   118,   119,   120,
      21,    22,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,   215,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
     116,   117,   118,   119,   120,    21,    22,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,   213,    -1,    -1,
     116,   117,   118,   119,   120,    21,    22,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,   213,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,   116,   117,   118,   119,   120,
      21,    22,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,   213,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    38,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,   213,    -1,    -1,    -1,    -1,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,    -1,   128,   129,
     130,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,   144,   145,   146,   147,   148,   149,
     150,   116,   117,   118,   119,   120,    21,    22,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
     190,    -1,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,    -1,    -1,   204,   205,    -1,    -1,    -1,    -1,
      -1,   211,   212,    -1,    -1,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    21,    22,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,   142,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,   142,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,   116,   117,   118,   119,
     120,    21,    22,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   211,   212,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    21,    22,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     190,    -1,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   211,   212,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,   116,   117,   118,   119,   120,    21,    22,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   211,   212,    -1,    -1,    -1,    -1,   182,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    21,    22,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,   116,   117,   118,   119,
     120,    21,    22,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,   190,    -1,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     190,    -1,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   211,   212,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    21,    22,   144,    -1,   146,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    21,    22,    -1,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   211,   212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,   211,   212,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    21,    22,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   194,   195,   196,   197,   198,   199,
     200,   201,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,   211,   212,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,   194,   195,   196,   197,   198,   199,
     200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   211,   212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,   170,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,   129,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    76,    77,    78,    79,   158,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,   170,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   220,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   182,   201,   212,   221,   224,   233,   235,
     236,   241,   271,   275,   309,   387,   394,   398,   409,   454,
     459,   464,    19,    20,   170,   263,   264,   265,   163,   242,
     243,   170,   171,   172,   201,   237,   238,   239,   180,   395,
     170,   216,   223,    57,    63,   390,   390,   390,   143,   170,
     295,    34,    63,   107,   136,   205,   214,   267,   268,   269,
     270,   295,   182,     5,     6,     8,    36,   107,   406,    62,
     385,   189,   188,   191,   188,   238,    22,    57,   200,   211,
     240,   390,   391,   393,   391,   385,   465,   455,   460,   170,
     143,   234,   269,   269,   269,   214,   144,   145,   146,   188,
     213,    57,    63,   276,   278,    57,    63,   396,     5,     6,
      57,    63,   407,    57,    63,   386,    15,    16,   163,   168,
     170,   173,   214,   226,   264,   163,   243,   170,   237,   237,
     170,   182,   181,   391,   182,    57,    63,   222,   170,   170,
     170,   170,   174,   232,   215,   265,   269,   269,   269,   269,
     279,   170,   397,   410,   180,   388,   174,   175,   176,   225,
      15,    16,   163,   168,   170,   226,   261,   262,   240,   392,
     182,   466,   456,   461,   174,   215,    35,    71,    73,    75,
      76,    77,    78,    79,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    93,    94,    95,    98,    99,
     100,   101,   118,   119,   170,   274,   277,   180,   191,   106,
     404,   405,   383,   160,   217,   266,   359,   174,   175,   176,
     188,   215,   189,   180,   180,   180,    21,    22,    38,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   128,   129,   130,
     137,   138,   139,   140,   141,   144,   145,   146,   147,   148,
     149,   150,   190,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   204,   205,   211,   212,    35,    35,   214,
     272,   180,   280,   399,    75,    79,    93,    94,    98,    99,
     100,   101,   414,   170,   411,   181,   384,   265,   264,   182,
     217,   152,   170,   381,   382,   261,    19,    25,    31,    41,
      49,    64,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   156,   216,   295,   413,   415,
     416,   419,   425,   453,   467,   457,   462,   170,   170,   170,
     213,    22,   170,   213,   155,   215,   359,   369,   370,   191,
     273,   283,   389,   180,   191,   403,   180,   408,   359,   213,
     264,   214,    43,   188,   191,   194,   380,   195,   195,   195,
     214,   195,   195,   214,   195,   195,   195,   195,   195,   195,
     214,   295,    33,    60,    61,   124,   128,   190,   194,   197,
     212,   218,   424,   192,   418,   373,   376,   170,   137,   214,
       7,    50,   308,   182,   215,   453,     1,     5,     6,     9,
      10,    11,    13,    15,    16,    17,    18,    19,    25,    26,
      27,    28,    29,    31,    38,    39,    42,    44,    45,    48,
      51,    52,    54,    55,    58,    59,    65,    68,    69,    80,
     102,   103,   104,   105,   118,   119,   133,   134,   135,   151,
     152,   153,   154,   155,   157,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   171,   172,   173,   180,
     181,   182,   197,   198,   199,   204,   205,   212,   214,   216,
     217,   231,   233,   244,   245,   248,   251,   252,   254,   256,
     257,   258,   259,   281,   282,   284,   289,   294,   295,   296,
     300,   301,   302,   303,   304,   305,   306,   307,   309,   313,
     314,   321,   324,   327,   332,   335,   336,   338,   339,   340,
     342,   347,   350,   351,   358,   413,   469,   484,   495,   499,
     512,   515,   170,   182,   400,   401,   295,   365,   382,   213,
      65,   104,   171,   289,   351,   170,   170,   425,   127,   137,
     189,   379,   426,   431,   433,   351,   435,   429,   170,   420,
     437,   439,   441,   443,   445,   447,   449,   451,   351,   195,
     214,    33,   194,    33,   194,   212,   218,   213,   351,   212,
     218,   425,   170,   182,   468,   170,   182,   188,   371,   422,
     453,   458,   170,   374,   422,   463,   351,   152,   170,   378,
     412,   369,   195,   195,   351,   255,   195,   297,   415,   469,
     214,   295,   195,     5,   102,   103,   195,   214,   127,   294,
     325,   336,   351,   280,   195,   214,    61,   351,   214,   351,
     170,   195,   195,   214,   182,   195,   163,    58,   351,   214,
     280,   195,   214,   195,   195,   214,   195,   195,   127,   294,
     351,   351,   351,   217,   280,   327,   331,   331,   331,   214,
     214,   214,   214,   214,   214,    13,   425,    13,   425,    13,
     351,   494,   510,   195,   351,   195,   230,    13,   287,   494,
     511,    37,   351,   351,   351,   351,   351,    13,    49,   285,
     325,   351,   158,   170,   325,   470,   472,   217,   182,   182,
     351,    10,   327,   333,   170,   214,   182,   182,   182,   182,
     182,    66,   310,   271,   132,   182,    21,    22,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   136,   137,   138,   139,   140,   141,   144,   145,
     146,   147,   148,   149,   150,   189,   190,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   211,   212,   333,
     214,   182,   189,   181,   389,   181,   182,   212,   271,   366,
     195,   215,    43,   182,   379,   294,   351,   453,   453,   423,
     453,   215,   453,   453,   215,   170,   417,   453,   272,   453,
     272,   453,   272,   371,   372,   374,   375,   215,   428,   285,
     213,   213,   351,   182,   181,   191,   422,   181,   191,   422,
     181,   215,   214,    43,   127,   188,   189,   191,   194,   377,
     485,   487,   280,   412,   298,   214,   295,   195,   214,   322,
     195,   195,   195,   506,   325,   294,   325,   188,   108,   109,
     110,   111,   112,   113,   114,   115,   121,   122,   127,   140,
     141,   147,   148,   149,   189,    14,   425,   287,   351,   351,
     280,   189,   315,   317,   351,   319,   191,   163,   351,   508,
     325,   491,   496,   325,   489,   425,   294,   351,   217,   271,
     351,   351,   351,   351,   351,   351,   412,    53,   197,   212,
     214,   351,   470,   473,   477,   493,   498,   412,   214,   473,
     498,   412,   142,   181,   182,   183,   478,   290,   280,   292,
     176,   177,   225,   412,   181,   188,   514,   180,   412,    13,
     213,   188,   514,   214,   137,   377,   514,   188,   215,   152,
     157,   195,   295,   341,   280,   253,   334,    70,   212,   215,
     325,   472,   160,   214,   312,   382,     4,   160,   330,   331,
      19,   158,   170,   413,    19,   158,   170,   413,   133,   134,
     135,   281,   337,   351,   337,   351,   337,   351,   337,   351,
     337,   351,   337,   351,   337,   351,   337,   351,   351,   351,
     351,   337,   351,   337,   351,   351,   351,   351,   170,   337,
     351,   351,   158,   170,   351,   351,   351,   413,   351,   351,
     351,   337,   351,   337,   351,   351,   351,   351,   337,   351,
     337,   351,   337,   351,   351,   337,   351,    22,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   129,
     130,   158,   170,   211,   212,   348,   413,   351,   215,   325,
     351,   402,   270,     8,   359,   364,   425,   170,   294,   351,
     182,   196,   196,   196,   422,   196,   196,   182,   196,   196,
     273,   196,   273,   196,   273,   196,   422,   196,   422,   288,
     453,   215,   213,   453,   453,   351,   170,   170,   453,   294,
     351,   425,   425,    20,   453,    70,   325,   472,   483,   195,
     351,   170,   351,   453,   500,   502,   504,   425,   514,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   280,   196,   422,
     215,   215,   260,   425,   425,   215,   425,   215,   425,   514,
     425,   425,   514,   425,   196,   330,   215,   215,   215,   215,
     215,   215,    20,   331,   212,   351,   215,   142,   182,   477,
     185,   186,   213,   481,   188,   182,   185,   213,   480,    20,
     215,   477,   181,   184,   479,    20,   351,   181,   494,   288,
     288,   351,    20,   494,   283,    20,   412,   351,   351,   351,
     215,   158,   170,   214,   214,   343,   345,    12,    23,    24,
     246,   247,   351,   271,   170,   215,   472,   470,   188,   215,
     215,   170,   311,   311,   214,   127,   137,   170,   189,   194,
     328,   329,   272,   195,   214,   195,   214,   331,   331,   331,
     214,   214,   213,    19,   158,   170,   413,   191,   158,   170,
     351,   214,   214,   158,   170,   351,     1,   214,   213,   215,
     182,   181,   213,    57,    63,   362,    67,   363,   182,   196,
     182,   427,   432,   434,   453,   436,   430,   170,   421,   438,
     196,   442,   196,   446,   196,   450,   371,   452,   374,   196,
     422,   215,    43,   377,   196,   196,   325,   196,   472,   215,
     215,   215,   170,   215,   182,   196,   215,   196,   425,   425,
     425,   196,   215,   214,   425,   351,   196,   196,   196,   196,
     215,   196,   196,   215,   196,   330,   272,   214,   325,   473,
     477,   351,   470,   481,   213,   351,   493,   213,   325,   473,
     181,   184,   187,   482,   213,   325,   196,   196,   191,   228,
     325,   181,   325,    20,   215,   214,   137,   377,   351,   351,
     425,   272,   280,   351,    12,   249,   330,   215,   213,   212,
     188,   213,   215,   329,   170,   170,   214,   170,   170,   188,
     213,   273,   352,   351,   354,   351,   215,   325,   351,   195,
     214,   351,   214,   213,   351,   212,   215,   325,   214,   213,
     349,   215,   325,   182,    47,   363,    46,   106,   360,   330,
     440,   444,   448,   214,   453,   170,   351,   486,   488,   280,
     299,   215,   196,   422,   170,   214,   323,   196,   196,   196,
     507,   287,   196,   316,   318,   320,   509,   492,   497,   490,
     214,   333,   273,   215,   325,   182,   477,   481,   182,   477,
     213,   182,   291,   293,   229,   178,   182,   182,   325,   137,
     377,   351,   351,   351,   215,   215,   196,   273,   280,   250,
     182,   272,   215,   470,   170,   213,   191,   380,   215,   170,
     328,   213,   142,   280,   326,   425,   215,   453,   215,   215,
     215,   356,   351,   351,   215,   470,   215,   351,   215,    33,
     361,   360,   362,   285,   214,   214,   351,   170,   196,   351,
     501,   503,   505,   214,   215,   214,   351,   351,   351,   214,
      70,   483,   214,   214,   215,   351,   326,   215,   351,   481,
     351,   482,   494,   351,   214,   286,   227,   494,   351,   182,
     351,   351,   215,   344,   196,   247,    26,   105,   251,   301,
     302,   303,   305,   351,   273,   213,   191,   380,   425,   379,
     215,   127,   351,   196,   196,   453,   215,   215,   213,   215,
     367,   361,   378,   215,   483,   483,   215,   196,   214,   215,
     214,   214,   214,   285,   287,   325,   483,   470,   471,   215,
     182,   513,   513,   513,   285,   174,   513,   513,   351,   137,
     377,   341,   346,   127,   127,   351,   280,   215,   425,   379,
     379,   294,   351,   351,   353,   355,   196,   215,   277,   368,
     214,   470,   474,   475,   476,   476,   351,   351,   483,   483,
     471,   215,   215,   514,   476,   215,    53,   213,   181,   181,
     188,   514,   181,   213,   513,   351,   351,   341,   351,   379,
     294,   351,   294,   351,   182,   357,   182,   277,   470,   188,
     514,   215,   215,   215,   215,   476,   476,   215,   215,   215,
     351,   213,   213,   181,   215,   213,   294,   351,   182,   182,
     280,   215,   214,   215,   215,   182,   470,   215
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   219,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   221,   222,   222,
     222,   223,   223,   224,   225,   225,   225,   225,   226,   226,
     227,   227,   228,   229,   228,   230,   230,   230,   231,   232,
     232,   234,   233,   235,   236,   237,   237,   237,   238,   238,
     238,   238,   239,   239,   240,   240,   241,   242,   242,   243,
     243,   244,   245,   245,   246,   246,   247,   247,   247,   248,
     248,   249,   250,   249,   251,   251,   251,   251,   251,   252,
     253,   252,   255,   254,   256,   257,   258,   260,   259,   261,
     261,   261,   261,   261,   261,   262,   262,   263,   263,   263,
     264,   264,   264,   264,   264,   264,   264,   264,   265,   265,
     266,   266,   266,   267,   267,   267,   267,   268,   268,   269,
     269,   269,   269,   269,   269,   269,   270,   270,   271,   271,
     272,   272,   272,   273,   273,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     275,   276,   276,   276,   277,   279,   278,   280,   280,   281,
     281,   281,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   283,   283,   283,   284,   285,   285,   286,
     286,   287,   287,   288,   288,   290,   291,   289,   292,   293,
     289,   294,   294,   294,   294,   294,   295,   295,   295,   296,
     296,   298,   299,   297,   297,   300,   300,   300,   300,   300,
     300,   301,   302,   303,   303,   303,   304,   304,   304,   305,
     305,   306,   306,   306,   307,   308,   308,   308,   309,   309,
     310,   310,   311,   311,   312,   312,   312,   312,   312,   312,
     312,   312,   313,   313,   315,   316,   314,   317,   318,   314,
     319,   320,   314,   322,   323,   321,   324,   324,   324,   324,
     324,   324,   325,   325,   326,   326,   326,   327,   327,   327,
     328,   328,   328,   328,   328,   329,   329,   330,   330,   330,
     331,   331,   332,   334,   333,   335,   335,   335,   335,   335,
     335,   335,   336,   336,   336,   336,   336,   336,   336,   336,
     336,   336,   336,   336,   336,   336,   336,   336,   336,   336,
     336,   337,   337,   337,   337,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   339,   339,   340,   340,   341,   341,   342,   343,
     344,   342,   345,   346,   342,   347,   347,   347,   347,   347,
     347,   347,   348,   349,   347,   350,   350,   350,   350,   350,
     350,   350,   351,   351,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   352,
     353,   351,   351,   351,   351,   354,   355,   351,   351,   351,
     356,   357,   351,   351,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   359,   359,   359,   360,   360,   360,   361,
     361,   362,   362,   362,   363,   363,   364,   365,   365,   366,
     365,   367,   365,   368,   365,   365,   369,   369,   370,   370,
     371,   371,   372,   372,   373,   373,   373,   374,   375,   375,
     376,   376,   376,   377,   377,   378,   378,   378,   378,   378,
     378,   379,   379,   379,   380,   380,   381,   381,   381,   381,
     381,   382,   382,   382,   382,   382,   383,   384,   383,   385,
     385,   386,   386,   386,   387,   388,   387,   389,   389,   389,
     389,   390,   390,   390,   392,   391,   393,   393,   394,   395,
     394,   396,   396,   396,   397,   399,   400,   398,   401,   402,
     398,   403,   403,   404,   404,   405,   406,   406,   406,   406,
     407,   407,   407,   408,   408,   410,   411,   409,   412,   412,
     412,   412,   412,   413,   413,   413,   413,   413,   413,   413,
     413,   413,   413,   413,   413,   413,   413,   413,   413,   413,
     413,   413,   413,   413,   413,   413,   413,   413,   413,   413,
     414,   414,   414,   414,   414,   414,   414,   414,   415,   416,
     416,   416,   417,   417,   418,   418,   418,   420,   421,   419,
     422,   422,   423,   423,   424,   424,   425,   425,   425,   425,
     425,   425,   426,   427,   425,   425,   425,   428,   425,   425,
     425,   425,   425,   425,   425,   425,   425,   425,   425,   425,
     425,   429,   430,   425,   425,   431,   432,   425,   433,   434,
     425,   435,   436,   425,   425,   437,   438,   425,   439,   440,
     425,   425,   441,   442,   425,   443,   444,   425,   425,   445,
     446,   425,   447,   448,   425,   449,   450,   425,   451,   452,
     425,   453,   453,   453,   455,   456,   457,   458,   454,   460,
     461,   462,   463,   459,   465,   466,   467,   468,   464,   469,
     469,   469,   469,   469,   470,   470,   470,   470,   470,   470,
     470,   470,   471,   472,   473,   473,   474,   474,   475,   475,
     476,   476,   477,   477,   478,   478,   479,   479,   480,   480,
     481,   481,   481,   482,   482,   482,   483,   483,   484,   484,
     484,   484,   484,   484,   485,   486,   484,   487,   488,   484,
     489,   490,   484,   491,   492,   484,   493,   493,   493,   494,
     494,   495,   496,   497,   495,   498,   498,   499,   499,   499,
     500,   501,   499,   502,   503,   499,   504,   505,   499,   499,
     506,   507,   499,   499,   508,   509,   499,   510,   510,   511,
     511,   512,   512,   512,   512,   512,   513,   513,   514,   514,
     515,   515,   515,   515,   515,   515
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
       1,     0,     0,     3,     1,     1,     1,     1,     1,     4,
       0,     6,     0,     6,     2,     3,     3,     0,     5,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     1,     5,     1,     3,
       2,     3,     2,     1,     1,     1,     1,     1,     4,     1,
       2,     3,     3,     3,     3,     2,     1,     3,     0,     3,
       0,     2,     3,     0,     2,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     3,
       3,     2,     2,     3,     4,     3,     2,     2,     2,     2,
       2,     3,     3,     3,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     1,     1,     3,     0,     4,     3,     7,     2,
       2,     6,     1,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     2,     2,     1,     1,     1,     1,
       2,     2,     2,     0,     2,     2,     3,     0,     2,     0,
       4,     0,     2,     1,     3,     0,     0,     7,     0,     0,
       7,     3,     2,     2,     2,     1,     1,     3,     2,     2,
       3,     0,     0,     5,     1,     2,     5,     5,     5,     6,
       2,     1,     1,     1,     2,     3,     2,     2,     3,     2,
       3,     2,     2,     3,     4,     1,     1,     0,     1,     1,
       1,     0,     1,     3,     9,     8,     8,     7,     8,     7,
       7,     6,     3,     3,     0,     0,     7,     0,     0,     7,
       0,     0,     7,     0,     0,     6,     5,     8,    10,     5,
       8,    10,     1,     3,     1,     2,     3,     1,     1,     2,
       2,     2,     2,     2,     4,     1,     3,     0,     4,     4,
       1,     6,     6,     0,     7,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     6,     8,     5,     6,     1,     4,     3,     0,
       0,     8,     0,     0,     9,     3,     4,     5,     6,     8,
       5,     6,     0,     0,     5,     3,     4,     4,     5,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     4,     3,     4,     5,     4,     5,     3,     4,
       1,     1,     2,     4,     4,     7,     8,     3,     5,     0,
       0,     8,     3,     3,     3,     0,     0,     8,     3,     4,
       0,     0,     9,     4,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     2,     4,     1,     4,     4,     4,
       4,     4,     1,     6,     7,     6,     6,     7,     7,     6,
       7,     6,     6,     0,     4,     1,     0,     1,     1,     0,
       1,     0,     1,     1,     0,     1,     5,     0,     2,     0,
       4,     0,     9,     0,    10,     5,     3,     4,     1,     3,
       1,     3,     1,     3,     0,     2,     3,     3,     1,     3,
       0,     2,     3,     1,     1,     1,     2,     3,     5,     3,
       3,     1,     1,     1,     0,     1,     1,     4,     3,     3,
       5,     4,     6,     5,     5,     4,     0,     0,     4,     0,
       1,     0,     1,     1,     6,     0,     6,     0,     2,     3,
       5,     0,     1,     1,     0,     5,     2,     3,     4,     0,
       4,     0,     1,     1,     1,     0,     0,     9,     0,     0,
      11,     0,     2,     0,     1,     3,     1,     1,     2,     2,
       0,     1,     1,     0,     3,     0,     0,     7,     1,     4,
       3,     3,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     1,     3,     0,     2,     3,     0,     0,     6,
       1,     1,     1,     3,     3,     4,     1,     1,     1,     1,
       2,     3,     0,     0,     6,     4,     5,     0,     9,     4,
       2,     2,     3,     2,     3,     2,     2,     3,     3,     3,
       2,     0,     0,     6,     2,     0,     0,     6,     0,     0,
       6,     0,     0,     6,     1,     0,     0,     6,     0,     0,
       7,     1,     0,     0,     6,     0,     0,     7,     1,     0,
       0,     6,     0,     0,     7,     0,     0,     6,     0,     0,
       6,     1,     3,     3,     0,     0,     0,     0,    10,     0,
       0,     0,     0,    10,     0,     0,     0,     0,    10,     1,
       1,     1,     1,     1,     3,     3,     5,     5,     6,     6,
       8,     8,     1,     1,     1,     3,     3,     5,     1,     2,
       1,     0,     0,     2,     2,     1,     2,     1,     2,     1,
       2,     1,     1,     2,     1,     1,     0,     1,     5,     4,
       6,     7,     5,     7,     0,     0,    10,     0,     0,    10,
       0,     0,     9,     0,     0,     7,     1,     3,     3,     3,
       1,     5,     0,     0,    10,     1,     3,     3,     4,     4,
       0,     0,    11,     0,     0,    11,     0,     0,    10,     5,
       0,     0,     9,     5,     0,     0,    10,     1,     3,     1,
       3,     3,     3,     4,     7,     9,     0,     3,     0,     1,
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
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
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
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 69: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 70: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 71: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 72: /* $@3: %empty  */
                      { yyextra->das_need_oxford_comma = true; }
    break;

  case 73: /* expression_else_one_liner: "else" $@3 expression_if_one_liner  */
                                                                                                 {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 74: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 75: /* expression_if_one_liner: expression_return_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 76: /* expression_if_one_liner: expression_yield_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 77: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 78: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 79: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 80: /* $@4: %empty  */
                                                     { yyextra->das_need_oxford_comma = true; }
    break;

  case 81: /* expression_if_then_else: expression_if_one_liner "if" $@4 expr expression_else_one_liner "end of expression"  */
                                                                                                                                                   {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr(ast_wrapInBlock((yyvsp[-5].pExpression))),(yyvsp[-1].pExpression) ? ExpressionPtr(ast_wrapInBlock((yyvsp[-1].pExpression))) : nullptr);
    }
    break;

  case 82: /* $@5: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 83: /* expression_for_loop: "for" $@5 variable_name_with_pos_list "in" expr_list expression_block  */
                                                                                                                                                 {
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-3].pNameWithPosList),(yyvsp[-1].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 84: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 85: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 86: /* expression_with: "with" expr expression_block  */
                                                         {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
    break;

  case 87: /* $@6: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 88: /* expression_with_alias: "assume" "name" '=' $@6 expr  */
                                                                                               {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-3].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-3].s);
    }
    break;

  case 89: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 90: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 91: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 92: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 93: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 94: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 95: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 96: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 97: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 98: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 99: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 100: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 101: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 102: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 103: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 104: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 105: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 106: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 107: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 108: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 109: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 110: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 111: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
                                                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 112: /* metadata_argument_list: metadata_argument_list "end of expression"  */
                                         {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 113: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 114: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 115: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 116: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 117: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 118: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 119: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 120: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 121: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 122: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 123: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 124: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 125: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 126: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 127: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 128: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 129: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 130: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 131: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 132: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 133: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 134: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 135: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 136: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 137: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 138: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 139: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 140: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 141: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 142: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 143: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 144: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 145: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 146: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 147: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 148: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 149: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 150: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 151: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 152: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 153: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 154: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 155: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 156: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 157: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 158: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 159: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 160: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 161: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 162: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 163: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 164: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 165: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 166: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 167: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 168: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 169: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 170: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 171: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 172: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 173: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 174: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 175: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 176: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 177: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 178: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 179: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 180: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 181: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 182: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 183: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 184: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 185: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 186: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 187: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 188: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 189: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 190: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 191: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 192: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 193: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 194: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 195: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 196: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 197: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 198: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 199: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 200: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 201: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 202: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 203: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 204: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 205: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 206: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 207: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 208: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 209: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 210: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 211: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 212: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 213: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 214: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 215: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 216: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 217: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 218: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 219: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 220: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 221: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 222: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 223: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 224: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 225: /* $@7: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 226: /* function_declaration: optional_public_or_private_function $@7 function_declaration_header expression_block  */
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

  case 227: /* expression_block: "begin of code block" expressions "end of code block"  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 228: /* expression_block: "begin of code block" expressions "end of code block" "finally" "begin of code block" expressions "end of code block"  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 229: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 230: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 231: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 232: /* expression_any: "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 233: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 234: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 235: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 236: /* expression_any: expr_assign "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 237: /* expression_any: expression_delete "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 238: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 239: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 240: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 241: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 242: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 243: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 244: /* expression_any: expression_break "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 245: /* expression_any: expression_continue "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 246: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 247: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 248: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 249: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 250: /* expression_any: expression_label "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 251: /* expression_any: expression_goto "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 252: /* expression_any: "pass" "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 253: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 254: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 255: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 256: /* expr_keyword: "keyword" expr expression_block  */
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

  case 257: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 258: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 259: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 260: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 261: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 262: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 263: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 264: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 265: /* $@8: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 266: /* $@9: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 267: /* expression_keyword: "keyword" '<' $@8 type_declaration_no_options_list '>' $@9 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 268: /* $@10: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 269: /* $@11: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 270: /* expression_keyword: "type function" '<' $@10 type_declaration_no_options_list '>' $@11 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 271: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 272: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 273: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 274: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 275: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 276: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 277: /* name_in_namespace: "name" "::" "name"  */
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

  case 278: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 279: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 280: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 281: /* $@12: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 282: /* $@13: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 283: /* new_type_declaration: '<' $@12 type_declaration '>' $@13  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 284: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 285: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 286: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 287: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 288: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 289: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 290: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 291: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 292: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 293: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 294: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 295: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 296: /* expression_return: expression_return_no_pipe "end of expression"  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 297: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 298: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 299: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 300: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 301: /* expression_yield: expression_yield_no_pipe "end of expression"  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 302: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 303: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 304: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 305: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 306: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 307: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 308: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 309: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 310: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 311: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 312: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 313: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 314: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 315: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 316: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 317: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                           {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 318: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 319: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr_pipe  */
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

  case 320: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 321: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr_pipe  */
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

  case 322: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 323: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 324: /* $@14: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 325: /* $@15: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 326: /* expr_cast: "cast" '<' $@14 type_declaration_no_options '>' $@15 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 327: /* $@16: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 328: /* $@17: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 329: /* expr_cast: "upcast" '<' $@16 type_declaration_no_options '>' $@17 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 330: /* $@18: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 331: /* $@19: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 332: /* expr_cast: "reinterpret" '<' $@18 type_declaration_no_options '>' $@19 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 333: /* $@20: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 334: /* $@21: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 335: /* expr_type_decl: "type" '<' $@20 type_declaration '>' $@21  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 336: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 337: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 338: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
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

  case 339: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 340: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 341: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" "end of expression" "name" '>' '(' expr ')'  */
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

  case 342: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 343: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 344: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 345: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 346: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 347: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 348: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 349: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 350: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 351: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 352: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 353: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 354: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 355: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 356: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 357: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 358: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 359: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 360: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 361: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 362: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 363: /* $@22: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 364: /* expr_full_block_assumed_piped: block_or_lambda $@22 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 365: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 366: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 367: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 368: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 369: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 370: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 371: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 372: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 373: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 392: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 393: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 394: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 395: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 396: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 397: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 398: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 399: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 400: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 401: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 402: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 403: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 404: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 405: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 406: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 407: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 408: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 409: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 410: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 411: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 412: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 413: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 414: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 415: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 416: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 417: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 418: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 419: /* $@23: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 420: /* $@24: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 421: /* func_addr_expr: '@' '@' '<' $@23 type_declaration_no_options '>' $@24 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 422: /* $@25: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 423: /* $@26: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 424: /* func_addr_expr: '@' '@' '<' $@25 optional_function_argument_list optional_function_type '>' $@26 func_addr_name  */
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

  case 425: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 426: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 427: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 428: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 429: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
                                                                                       {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->methodCall = true;
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        nc->nonNamedArguments.push_back(ExpressionPtr((yyvsp[-7].pExpression)));
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 430: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 431: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 432: /* $@27: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 433: /* $@28: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 434: /* expr_field: expr '.' $@27 error $@28  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 435: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 436: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 437: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 438: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-4])),*(yyvsp[-4].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 439: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 440: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 441: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 442: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 443: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 444: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 445: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 446: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 447: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 448: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 449: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 450: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 451: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 452: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 453: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 454: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 455: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 456: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 457: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 458: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 459: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 460: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 461: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 462: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 463: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 464: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 465: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 466: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 467: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 468: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 469: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 470: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 471: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 472: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 473: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 474: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 475: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 476: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 477: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 478: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 479: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 480: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 481: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 482: /* expr: '(' expr_list optional_comma ')'  */
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

  case 483: /* expr: '(' make_struct_single ')'  */
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

  case 484: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 485: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 486: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 487: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 488: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 489: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 490: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 491: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 492: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 493: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 494: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 495: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 496: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 497: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 498: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 499: /* $@29: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 500: /* $@30: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 501: /* expr: expr "is" "type" '<' $@29 type_declaration_no_options '>' $@30  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 502: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 503: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 504: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 505: /* $@31: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 506: /* $@32: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 507: /* expr: expr "as" "type" '<' $@31 type_declaration '>' $@32  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 508: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 509: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 510: /* $@33: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 511: /* $@34: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 512: /* expr: expr '?' "as" "type" '<' $@33 type_declaration '>' $@34  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 513: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 514: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 515: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 516: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 517: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 518: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 519: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 520: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 521: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 522: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 523: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 524: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 525: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 526: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 527: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 528: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 529: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 530: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 531: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 532: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 533: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 534: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 535: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 536: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 537: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 538: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 539: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 540: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 541: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 542: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 543: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 544: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); /*this one is gone when BRABRA is disabled*/ }
    break;

  case 545: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 546: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 547: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 548: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 549: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 550: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 551: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 552: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 553: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 554: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 555: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 556: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 557: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 558: /* struct_variable_declaration_list: struct_variable_declaration_list "end of expression"  */
                                                   {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 559: /* $@35: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 560: /* struct_variable_declaration_list: struct_variable_declaration_list $@35 structure_variable_declaration "end of expression"  */
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

  case 561: /* $@36: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 562: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@36 function_declaration_header "end of expression"  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 563: /* $@37: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 564: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@37 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 565: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' "end of expression"  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 566: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 567: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 568: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 569: /* function_argument_list: function_argument_list "end of expression" function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 570: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 571: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 572: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 573: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 574: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 575: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 576: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 577: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 578: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 579: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 580: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 581: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 582: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 583: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 584: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 585: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 586: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 587: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 588: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 589: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 590: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 591: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 592: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 593: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 594: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 595: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 596: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 597: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 598: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 599: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 600: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 601: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options "end of expression"  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 602: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 603: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 604: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 605: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 606: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 607: /* $@38: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 608: /* global_variable_declaration_list: global_variable_declaration_list $@38 optional_field_annotation let_variable_declaration  */
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

  case 609: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 610: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 611: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 612: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 613: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 614: /* global_let: kwd_let optional_shared optional_public_or_private_variable "begin of code block" global_variable_declaration_list "end of code block"  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 615: /* $@39: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 616: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@39 optional_field_annotation let_variable_declaration  */
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

  case 617: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 618: /* enum_list: enum_list "end of expression"  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 619: /* enum_list: enum_list "name" "end of expression"  */
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

  case 620: /* enum_list: enum_list "name" '=' expr "end of expression"  */
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

  case 621: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 622: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 623: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 624: /* $@40: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 625: /* single_alias: optional_public_or_private_alias "name" $@40 '=' type_declaration  */
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

  case 629: /* $@41: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 631: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 632: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 633: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 634: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 635: /* $@42: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 636: /* $@43: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 637: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name "begin of code block" $@42 enum_list $@43 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 638: /* $@44: %empty  */
                                                                                                                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 639: /* $@45: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 640: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration "begin of code block" $@44 enum_list $@45 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 641: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 642: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 643: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 644: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 645: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 646: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 647: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 648: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 649: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 650: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 651: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 652: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 653: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 654: /* optional_struct_variable_declaration_list: "begin of code block" struct_variable_declaration_list "end of code block"  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 655: /* $@46: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 656: /* $@47: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 657: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@46 structure_name $@47 optional_struct_variable_declaration_list  */
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

  case 658: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 659: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 660: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 661: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 662: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 663: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 664: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 665: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 666: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 667: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 668: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 669: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 670: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 671: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 672: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 673: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 674: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 675: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 676: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 677: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 678: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 679: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 680: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 681: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 682: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 683: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 684: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 685: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 686: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 687: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 688: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 689: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 690: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 691: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 692: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 693: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 694: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 695: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 696: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 697: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 698: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 699: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 700: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 701: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 702: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 703: /* bitfield_bits: bitfield_bits "end of expression" "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 704: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 705: /* bitfield_alias_bits: bitfield_alias_bits "end of expression"  */
                                      {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 706: /* bitfield_alias_bits: bitfield_alias_bits "name" "end of expression"  */
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

  case 707: /* $@48: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 708: /* $@49: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 709: /* bitfield_type_declaration: "bitfield" '<' $@48 bitfield_bits '>' $@49  */
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

  case 712: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 713: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 714: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 715: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 716: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 717: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 718: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 719: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 720: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 721: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 722: /* $@50: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 723: /* $@51: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 724: /* type_declaration_no_options: "type" '<' $@50 type_declaration '>' $@51  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 725: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 726: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 727: /* $@52: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 728: /* type_declaration_no_options: '$' name_in_namespace '<' $@52 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 729: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 730: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 731: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 732: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 733: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 734: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 735: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 736: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 737: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 738: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 739: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 740: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 741: /* $@53: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 742: /* $@54: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 743: /* type_declaration_no_options: "smart_ptr" '<' $@53 type_declaration '>' $@54  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 744: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 745: /* $@55: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 746: /* $@56: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 747: /* type_declaration_no_options: "array" '<' $@55 type_declaration '>' $@56  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 748: /* $@57: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 749: /* $@58: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 750: /* type_declaration_no_options: "table" '<' $@57 table_type_pair '>' $@58  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 751: /* $@59: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 752: /* $@60: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 753: /* type_declaration_no_options: "iterator" '<' $@59 type_declaration '>' $@60  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 754: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 755: /* $@61: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 756: /* $@62: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 757: /* type_declaration_no_options: "block" '<' $@61 type_declaration '>' $@62  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 758: /* $@63: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 759: /* $@64: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 760: /* type_declaration_no_options: "block" '<' $@63 optional_function_argument_list optional_function_type '>' $@64  */
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

  case 761: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 762: /* $@65: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 763: /* $@66: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 764: /* type_declaration_no_options: "function" '<' $@65 type_declaration '>' $@66  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 765: /* $@67: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 766: /* $@68: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 767: /* type_declaration_no_options: "function" '<' $@67 optional_function_argument_list optional_function_type '>' $@68  */
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

  case 768: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 769: /* $@69: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 770: /* $@70: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 771: /* type_declaration_no_options: "lambda" '<' $@69 type_declaration '>' $@70  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 772: /* $@71: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 773: /* $@72: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 774: /* type_declaration_no_options: "lambda" '<' $@71 optional_function_argument_list optional_function_type '>' $@72  */
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

  case 775: /* $@73: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 776: /* $@74: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 777: /* type_declaration_no_options: "tuple" '<' $@73 tuple_type_list '>' $@74  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 778: /* $@75: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 779: /* $@76: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 780: /* type_declaration_no_options: "variant" '<' $@75 variant_type_list '>' $@76  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 781: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 782: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 783: /* type_declaration: type_declaration '|' '#'  */
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

  case 784: /* $@77: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 785: /* $@78: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 786: /* $@79: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 787: /* $@80: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 788: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@77 "name" $@78 "begin of code block" $@79 tuple_alias_type_list $@80 "end of code block"  */
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

  case 789: /* $@81: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 790: /* $@82: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 791: /* $@83: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 792: /* $@84: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 793: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@81 "name" $@82 "begin of code block" $@83 variant_alias_type_list $@84 "end of code block"  */
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

  case 794: /* $@85: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 795: /* $@86: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 796: /* $@87: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 797: /* $@88: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 798: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@85 "name" $@86 "begin of code block" $@87 bitfield_alias_bits $@88 "end of code block"  */
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

  case 799: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 800: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 801: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 802: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 803: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 804: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 805: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 806: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 807: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 808: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 809: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 810: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 811: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 812: /* make_variant_dim: make_struct_fields  */
                                {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 813: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 814: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 815: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 816: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 817: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 818: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 819: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 820: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 821: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 822: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 823: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 836: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 837: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 838: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 839: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 840: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 841: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 842: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 843: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 844: /* $@89: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 845: /* $@90: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 846: /* make_struct_decl: "struct" '<' $@89 type_declaration_no_options '>' $@90 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 847: /* $@91: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 848: /* $@92: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 849: /* make_struct_decl: "class" '<' $@91 type_declaration_no_options '>' $@92 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 850: /* $@93: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 851: /* $@94: %empty  */
                                                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 852: /* make_struct_decl: "variant" '<' $@93 type_declaration_no_options '>' $@94 '(' make_variant_dim ')'  */
                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 853: /* $@95: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 854: /* $@96: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 855: /* make_struct_decl: "default" '<' $@95 type_declaration_no_options '>' $@96 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 856: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 857: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 858: /* make_tuple: make_tuple ',' expr  */
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

  case 859: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 860: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 861: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 862: /* $@97: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 863: /* $@98: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 864: /* make_tuple_call: "tuple" '<' $@97 type_declaration_no_options '>' $@98 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 865: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 866: /* make_dim: make_dim "end of expression" make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 867: /* make_dim_decl: '[' optional_expr_list ']'  */
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

  case 868: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 869: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 870: /* $@99: %empty  */
                                       { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 871: /* $@100: %empty  */
                                                                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 872: /* make_dim_decl: "array" "struct" '<' $@99 type_declaration_no_options '>' $@100 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 873: /* $@101: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 874: /* $@102: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 875: /* make_dim_decl: "array" "tuple" '<' $@101 type_declaration_no_options '>' $@102 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 876: /* $@103: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 877: /* $@104: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 878: /* make_dim_decl: "array" "variant" '<' $@103 type_declaration_no_options '>' $@104 '(' make_variant_dim ')'  */
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

  case 879: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 880: /* $@105: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 881: /* $@106: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 882: /* make_dim_decl: "array" '<' $@105 type_declaration_no_options '>' $@106 '(' optional_expr_list ')'  */
                                                                                                                                                                        {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-8])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
            mka->gen2 = true;
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

  case 883: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 884: /* $@107: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 885: /* $@108: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 886: /* make_dim_decl: "fixed_array" '<' $@107 type_declaration_no_options '>' $@108 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 887: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 888: /* make_table: make_table "end of expression" make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 889: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 890: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 891: /* make_table_decl: "begin of code block" optional_expr_map_tuple_list "end of code block"  */
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

  case 892: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 893: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 894: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 895: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 896: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 897: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 898: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 899: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 900: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 901: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 902: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 903: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 904: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 905: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block" "end of code block"  */
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


