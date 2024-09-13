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
  YYSYMBOL_DAS_TBOOL = 69,                 /* "bool"  */
  YYSYMBOL_DAS_TVOID = 70,                 /* "void"  */
  YYSYMBOL_DAS_TSTRING = 71,               /* "string"  */
  YYSYMBOL_DAS_TAUTO = 72,                 /* "auto"  */
  YYSYMBOL_DAS_TINT = 73,                  /* "int"  */
  YYSYMBOL_DAS_TINT2 = 74,                 /* "int2"  */
  YYSYMBOL_DAS_TINT3 = 75,                 /* "int3"  */
  YYSYMBOL_DAS_TINT4 = 76,                 /* "int4"  */
  YYSYMBOL_DAS_TUINT = 77,                 /* "uint"  */
  YYSYMBOL_DAS_TBITFIELD = 78,             /* "bitfield"  */
  YYSYMBOL_DAS_TUINT2 = 79,                /* "uint2"  */
  YYSYMBOL_DAS_TUINT3 = 80,                /* "uint3"  */
  YYSYMBOL_DAS_TUINT4 = 81,                /* "uint4"  */
  YYSYMBOL_DAS_TFLOAT = 82,                /* "float"  */
  YYSYMBOL_DAS_TFLOAT2 = 83,               /* "float2"  */
  YYSYMBOL_DAS_TFLOAT3 = 84,               /* "float3"  */
  YYSYMBOL_DAS_TFLOAT4 = 85,               /* "float4"  */
  YYSYMBOL_DAS_TRANGE = 86,                /* "range"  */
  YYSYMBOL_DAS_TURANGE = 87,               /* "urange"  */
  YYSYMBOL_DAS_TRANGE64 = 88,              /* "range64"  */
  YYSYMBOL_DAS_TURANGE64 = 89,             /* "urange64"  */
  YYSYMBOL_DAS_TBLOCK = 90,                /* "block"  */
  YYSYMBOL_DAS_TINT64 = 91,                /* "int64"  */
  YYSYMBOL_DAS_TUINT64 = 92,               /* "uint64"  */
  YYSYMBOL_DAS_TDOUBLE = 93,               /* "double"  */
  YYSYMBOL_DAS_TFUNCTION = 94,             /* "function"  */
  YYSYMBOL_DAS_TLAMBDA = 95,               /* "lambda"  */
  YYSYMBOL_DAS_TINT8 = 96,                 /* "int8"  */
  YYSYMBOL_DAS_TUINT8 = 97,                /* "uint8"  */
  YYSYMBOL_DAS_TINT16 = 98,                /* "int16"  */
  YYSYMBOL_DAS_TUINT16 = 99,               /* "uint16"  */
  YYSYMBOL_DAS_TTUPLE = 100,               /* "tuple"  */
  YYSYMBOL_DAS_TVARIANT = 101,             /* "variant"  */
  YYSYMBOL_DAS_GENERATOR = 102,            /* "generator"  */
  YYSYMBOL_DAS_YIELD = 103,                /* "yield"  */
  YYSYMBOL_DAS_SEALED = 104,               /* "sealed"  */
  YYSYMBOL_ADDEQU = 105,                   /* "+="  */
  YYSYMBOL_SUBEQU = 106,                   /* "-="  */
  YYSYMBOL_DIVEQU = 107,                   /* "/="  */
  YYSYMBOL_MULEQU = 108,                   /* "*="  */
  YYSYMBOL_MODEQU = 109,                   /* "%="  */
  YYSYMBOL_ANDEQU = 110,                   /* "&="  */
  YYSYMBOL_OREQU = 111,                    /* "|="  */
  YYSYMBOL_XOREQU = 112,                   /* "^="  */
  YYSYMBOL_SHL = 113,                      /* "<<"  */
  YYSYMBOL_SHR = 114,                      /* ">>"  */
  YYSYMBOL_ADDADD = 115,                   /* "++"  */
  YYSYMBOL_SUBSUB = 116,                   /* "--"  */
  YYSYMBOL_LEEQU = 117,                    /* "<="  */
  YYSYMBOL_SHLEQU = 118,                   /* "<<="  */
  YYSYMBOL_SHREQU = 119,                   /* ">>="  */
  YYSYMBOL_GREQU = 120,                    /* ">="  */
  YYSYMBOL_EQUEQU = 121,                   /* "=="  */
  YYSYMBOL_NOTEQU = 122,                   /* "!="  */
  YYSYMBOL_RARROW = 123,                   /* "->"  */
  YYSYMBOL_LARROW = 124,                   /* "<-"  */
  YYSYMBOL_QQ = 125,                       /* "??"  */
  YYSYMBOL_QDOT = 126,                     /* "?."  */
  YYSYMBOL_QBRA = 127,                     /* "?["  */
  YYSYMBOL_LPIPE = 128,                    /* "<|"  */
  YYSYMBOL_LBPIPE = 129,                   /* " <|"  */
  YYSYMBOL_LLPIPE = 130,                   /* "$ <|"  */
  YYSYMBOL_LAPIPE = 131,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 132,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 133,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 134,                 /* ":="  */
  YYSYMBOL_ROTL = 135,                     /* "<<<"  */
  YYSYMBOL_ROTR = 136,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 137,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 138,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 139,                    /* "=>"  */
  YYSYMBOL_COLCOL = 140,                   /* "::"  */
  YYSYMBOL_ANDAND = 141,                   /* "&&"  */
  YYSYMBOL_OROR = 142,                     /* "||"  */
  YYSYMBOL_XORXOR = 143,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 144,                /* "&&="  */
  YYSYMBOL_OROREQU = 145,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 146,                /* "^^="  */
  YYSYMBOL_DOTDOT = 147,                   /* ".."  */
  YYSYMBOL_MTAG_E = 148,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 149,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 150,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 151,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 152,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 153,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 154,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 155,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 156,           /* "..."  */
  YYSYMBOL_BRABRAB = 157,                  /* "[["  */
  YYSYMBOL_BRACBRB = 158,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 159,                  /* "{{"  */
  YYSYMBOL_INTEGER = 160,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 161,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 162,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 163,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 164,            /* "unsigned int8 constant"  */
  YYSYMBOL_FLOAT = 165,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 166,                   /* "double constant"  */
  YYSYMBOL_NAME = 167,                     /* "name"  */
  YYSYMBOL_KEYWORD = 168,                  /* "keyword"  */
  YYSYMBOL_TYPE_FUNCTION = 169,            /* "type function"  */
  YYSYMBOL_BEGIN_STRING = 170,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 171,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 172,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 173,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 174,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 175,          /* "}"  */
  YYSYMBOL_END_OF_READ = 176,              /* "end of failed eader macro"  */
  YYSYMBOL_SEMICOLON_CUR_CUR = 177,        /* ";}}"  */
  YYSYMBOL_SEMICOLON_CUR_SQR = 178,        /* ";}]"  */
  YYSYMBOL_SEMICOLON_SQR_SQR = 179,        /* ";]]"  */
  YYSYMBOL_COMMA_SQR_SQR = 180,            /* ",]]"  */
  YYSYMBOL_COMMA_CUR_SQR = 181,            /* ",}]"  */
  YYSYMBOL_182_ = 182,                     /* ','  */
  YYSYMBOL_183_ = 183,                     /* '='  */
  YYSYMBOL_184_ = 184,                     /* '?'  */
  YYSYMBOL_185_ = 185,                     /* ':'  */
  YYSYMBOL_186_ = 186,                     /* '|'  */
  YYSYMBOL_187_ = 187,                     /* '^'  */
  YYSYMBOL_188_ = 188,                     /* '&'  */
  YYSYMBOL_189_ = 189,                     /* '<'  */
  YYSYMBOL_190_ = 190,                     /* '>'  */
  YYSYMBOL_191_ = 191,                     /* '-'  */
  YYSYMBOL_192_ = 192,                     /* '+'  */
  YYSYMBOL_193_ = 193,                     /* '*'  */
  YYSYMBOL_194_ = 194,                     /* '/'  */
  YYSYMBOL_195_ = 195,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 196,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 197,               /* UNARY_PLUS  */
  YYSYMBOL_198_ = 198,                     /* '~'  */
  YYSYMBOL_199_ = 199,                     /* '!'  */
  YYSYMBOL_PRE_INC = 200,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 201,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 202,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 203,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 204,                    /* DEREF  */
  YYSYMBOL_205_ = 205,                     /* '.'  */
  YYSYMBOL_206_ = 206,                     /* '['  */
  YYSYMBOL_207_ = 207,                     /* ']'  */
  YYSYMBOL_208_ = 208,                     /* '('  */
  YYSYMBOL_209_ = 209,                     /* ')'  */
  YYSYMBOL_210_ = 210,                     /* '$'  */
  YYSYMBOL_211_ = 211,                     /* '@'  */
  YYSYMBOL_212_ = 212,                     /* ';'  */
  YYSYMBOL_213_ = 213,                     /* '{'  */
  YYSYMBOL_214_ = 214,                     /* '}'  */
  YYSYMBOL_215_ = 215,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 216,                 /* $accept  */
  YYSYMBOL_program = 217,                  /* program  */
  YYSYMBOL_top_level_reader_macro = 218,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 219, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 220,              /* module_name  */
  YYSYMBOL_module_declaration = 221,       /* module_declaration  */
  YYSYMBOL_character_sequence = 222,       /* character_sequence  */
  YYSYMBOL_string_constant = 223,          /* string_constant  */
  YYSYMBOL_string_builder_body = 224,      /* string_builder_body  */
  YYSYMBOL_string_builder = 225,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 226, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 227,              /* expr_reader  */
  YYSYMBOL_228_1 = 228,                    /* $@1  */
  YYSYMBOL_options_declaration = 229,      /* options_declaration  */
  YYSYMBOL_require_declaration = 230,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 231,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 232,      /* require_module_name  */
  YYSYMBOL_require_module = 233,           /* require_module  */
  YYSYMBOL_is_public_module = 234,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 235,       /* expect_declaration  */
  YYSYMBOL_expect_list = 236,              /* expect_list  */
  YYSYMBOL_expect_error = 237,             /* expect_error  */
  YYSYMBOL_expression_label = 238,         /* expression_label  */
  YYSYMBOL_expression_goto = 239,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 240,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 241,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 242,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 243, /* expression_else_one_liner  */
  YYSYMBOL_244_2 = 244,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 245,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 246,  /* expression_if_then_else  */
  YYSYMBOL_247_3 = 247,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 248,      /* expression_for_loop  */
  YYSYMBOL_249_4 = 249,                    /* $@4  */
  YYSYMBOL_expression_unsafe = 250,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 251,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 252,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 253,    /* expression_with_alias  */
  YYSYMBOL_254_5 = 254,                    /* $@5  */
  YYSYMBOL_annotation_argument_value = 255, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 256, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 257, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 258,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 259, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 260, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 261, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 262,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 263,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 264, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 265, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 266,   /* optional_function_type  */
  YYSYMBOL_function_name = 267,            /* function_name  */
  YYSYMBOL_global_function_declaration = 268, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 269, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 270, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 271,     /* function_declaration  */
  YYSYMBOL_272_6 = 272,                    /* $@6  */
  YYSYMBOL_expression_block = 273,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 274,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 275,           /* expression_any  */
  YYSYMBOL_expressions = 276,              /* expressions  */
  YYSYMBOL_expr_keyword = 277,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 278,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 279, /* optional_expr_list_in_braces  */
  YYSYMBOL_type_declaration_no_options_list = 280, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 281,       /* expression_keyword  */
  YYSYMBOL_282_7 = 282,                    /* $@7  */
  YYSYMBOL_283_8 = 283,                    /* $@8  */
  YYSYMBOL_284_9 = 284,                    /* $@9  */
  YYSYMBOL_285_10 = 285,                   /* $@10  */
  YYSYMBOL_expr_pipe = 286,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 287,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 288,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 289,     /* new_type_declaration  */
  YYSYMBOL_290_11 = 290,                   /* $@11  */
  YYSYMBOL_291_12 = 291,                   /* $@12  */
  YYSYMBOL_expr_new = 292,                 /* expr_new  */
  YYSYMBOL_expression_break = 293,         /* expression_break  */
  YYSYMBOL_expression_continue = 294,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 295, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 296,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 297, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 298,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 299,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 300,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 301,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 302,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 303, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 304,           /* expression_let  */
  YYSYMBOL_expr_cast = 305,                /* expr_cast  */
  YYSYMBOL_306_13 = 306,                   /* $@13  */
  YYSYMBOL_307_14 = 307,                   /* $@14  */
  YYSYMBOL_308_15 = 308,                   /* $@15  */
  YYSYMBOL_309_16 = 309,                   /* $@16  */
  YYSYMBOL_310_17 = 310,                   /* $@17  */
  YYSYMBOL_311_18 = 311,                   /* $@18  */
  YYSYMBOL_expr_type_decl = 312,           /* expr_type_decl  */
  YYSYMBOL_313_19 = 313,                   /* $@19  */
  YYSYMBOL_314_20 = 314,                   /* $@20  */
  YYSYMBOL_expr_type_info = 315,           /* expr_type_info  */
  YYSYMBOL_expr_list = 316,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 317,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 318,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 319,            /* capture_entry  */
  YYSYMBOL_capture_list = 320,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 321,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 322,               /* expr_block  */
  YYSYMBOL_expr_full_block = 323,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 324, /* expr_full_block_assumed_piped  */
  YYSYMBOL_325_21 = 325,                   /* $@21  */
  YYSYMBOL_expr_numeric_const = 326,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 327,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 328,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 329,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 330,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 331,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 332,           /* func_addr_expr  */
  YYSYMBOL_333_22 = 333,                   /* $@22  */
  YYSYMBOL_334_23 = 334,                   /* $@23  */
  YYSYMBOL_335_24 = 335,                   /* $@24  */
  YYSYMBOL_336_25 = 336,                   /* $@25  */
  YYSYMBOL_expr_field = 337,               /* expr_field  */
  YYSYMBOL_338_26 = 338,                   /* $@26  */
  YYSYMBOL_339_27 = 339,                   /* $@27  */
  YYSYMBOL_expr_call = 340,                /* expr_call  */
  YYSYMBOL_expr = 341,                     /* expr  */
  YYSYMBOL_342_28 = 342,                   /* $@28  */
  YYSYMBOL_343_29 = 343,                   /* $@29  */
  YYSYMBOL_344_30 = 344,                   /* $@30  */
  YYSYMBOL_345_31 = 345,                   /* $@31  */
  YYSYMBOL_346_32 = 346,                   /* $@32  */
  YYSYMBOL_347_33 = 347,                   /* $@33  */
  YYSYMBOL_expr_mtag = 348,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 349, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 350,        /* optional_override  */
  YYSYMBOL_optional_constant = 351,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 352, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 353, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 354, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 355, /* struct_variable_declaration_list  */
  YYSYMBOL_356_34 = 356,                   /* $@34  */
  YYSYMBOL_357_35 = 357,                   /* $@35  */
  YYSYMBOL_358_36 = 358,                   /* $@36  */
  YYSYMBOL_function_argument_declaration = 359, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 360,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 361,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 362,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 363,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 364,             /* variant_type  */
  YYSYMBOL_variant_type_list = 365,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 366,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 367,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 368,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 369,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 370,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 371, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 372, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 373, /* global_variable_declaration_list  */
  YYSYMBOL_374_37 = 374,                   /* $@37  */
  YYSYMBOL_optional_shared = 375,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 376, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 377,               /* global_let  */
  YYSYMBOL_378_38 = 378,                   /* $@38  */
  YYSYMBOL_enum_list = 379,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 380, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 381,             /* single_alias  */
  YYSYMBOL_382_39 = 382,                   /* $@39  */
  YYSYMBOL_alias_list = 383,               /* alias_list  */
  YYSYMBOL_alias_declaration = 384,        /* alias_declaration  */
  YYSYMBOL_385_40 = 385,                   /* $@40  */
  YYSYMBOL_optional_public_or_private_enum = 386, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 387,                /* enum_name  */
  YYSYMBOL_enum_declaration = 388,         /* enum_declaration  */
  YYSYMBOL_389_41 = 389,                   /* $@41  */
  YYSYMBOL_390_42 = 390,                   /* $@42  */
  YYSYMBOL_391_43 = 391,                   /* $@43  */
  YYSYMBOL_392_44 = 392,                   /* $@44  */
  YYSYMBOL_optional_structure_parent = 393, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 394,          /* optional_sealed  */
  YYSYMBOL_structure_name = 395,           /* structure_name  */
  YYSYMBOL_class_or_struct = 396,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 397, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 398, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 399,    /* structure_declaration  */
  YYSYMBOL_400_45 = 400,                   /* $@45  */
  YYSYMBOL_401_46 = 401,                   /* $@46  */
  YYSYMBOL_variable_name_with_pos_list = 402, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 403,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 404, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 405, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 406,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 407,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 408,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 409, /* bitfield_type_declaration  */
  YYSYMBOL_410_47 = 410,                   /* $@47  */
  YYSYMBOL_411_48 = 411,                   /* $@48  */
  YYSYMBOL_table_type_pair = 412,          /* table_type_pair  */
  YYSYMBOL_dim_list = 413,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 414, /* type_declaration_no_options  */
  YYSYMBOL_415_49 = 415,                   /* $@49  */
  YYSYMBOL_416_50 = 416,                   /* $@50  */
  YYSYMBOL_417_51 = 417,                   /* $@51  */
  YYSYMBOL_418_52 = 418,                   /* $@52  */
  YYSYMBOL_419_53 = 419,                   /* $@53  */
  YYSYMBOL_420_54 = 420,                   /* $@54  */
  YYSYMBOL_421_55 = 421,                   /* $@55  */
  YYSYMBOL_422_56 = 422,                   /* $@56  */
  YYSYMBOL_423_57 = 423,                   /* $@57  */
  YYSYMBOL_424_58 = 424,                   /* $@58  */
  YYSYMBOL_425_59 = 425,                   /* $@59  */
  YYSYMBOL_426_60 = 426,                   /* $@60  */
  YYSYMBOL_427_61 = 427,                   /* $@61  */
  YYSYMBOL_428_62 = 428,                   /* $@62  */
  YYSYMBOL_429_63 = 429,                   /* $@63  */
  YYSYMBOL_430_64 = 430,                   /* $@64  */
  YYSYMBOL_431_65 = 431,                   /* $@65  */
  YYSYMBOL_432_66 = 432,                   /* $@66  */
  YYSYMBOL_433_67 = 433,                   /* $@67  */
  YYSYMBOL_434_68 = 434,                   /* $@68  */
  YYSYMBOL_435_69 = 435,                   /* $@69  */
  YYSYMBOL_436_70 = 436,                   /* $@70  */
  YYSYMBOL_437_71 = 437,                   /* $@71  */
  YYSYMBOL_438_72 = 438,                   /* $@72  */
  YYSYMBOL_439_73 = 439,                   /* $@73  */
  YYSYMBOL_440_74 = 440,                   /* $@74  */
  YYSYMBOL_441_75 = 441,                   /* $@75  */
  YYSYMBOL_type_declaration = 442,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 443,  /* tuple_alias_declaration  */
  YYSYMBOL_444_76 = 444,                   /* $@76  */
  YYSYMBOL_445_77 = 445,                   /* $@77  */
  YYSYMBOL_446_78 = 446,                   /* $@78  */
  YYSYMBOL_447_79 = 447,                   /* $@79  */
  YYSYMBOL_variant_alias_declaration = 448, /* variant_alias_declaration  */
  YYSYMBOL_449_80 = 449,                   /* $@80  */
  YYSYMBOL_450_81 = 450,                   /* $@81  */
  YYSYMBOL_451_82 = 451,                   /* $@82  */
  YYSYMBOL_452_83 = 452,                   /* $@83  */
  YYSYMBOL_bitfield_alias_declaration = 453, /* bitfield_alias_declaration  */
  YYSYMBOL_454_84 = 454,                   /* $@84  */
  YYSYMBOL_455_85 = 455,                   /* $@85  */
  YYSYMBOL_456_86 = 456,                   /* $@86  */
  YYSYMBOL_457_87 = 457,                   /* $@87  */
  YYSYMBOL_make_decl = 458,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 459,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 460,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 461,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 462, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 463, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 464, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 465, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 466, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_make_struct_decl = 467,         /* make_struct_decl  */
  YYSYMBOL_468_88 = 468,                   /* $@88  */
  YYSYMBOL_469_89 = 469,                   /* $@89  */
  YYSYMBOL_470_90 = 470,                   /* $@90  */
  YYSYMBOL_471_91 = 471,                   /* $@91  */
  YYSYMBOL_472_92 = 472,                   /* $@92  */
  YYSYMBOL_473_93 = 473,                   /* $@93  */
  YYSYMBOL_474_94 = 474,                   /* $@94  */
  YYSYMBOL_475_95 = 475,                   /* $@95  */
  YYSYMBOL_make_tuple = 476,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 477,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 478,          /* make_tuple_call  */
  YYSYMBOL_479_96 = 479,                   /* $@96  */
  YYSYMBOL_480_97 = 480,                   /* $@97  */
  YYSYMBOL_make_dim = 481,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 482,            /* make_dim_decl  */
  YYSYMBOL_483_98 = 483,                   /* $@98  */
  YYSYMBOL_484_99 = 484,                   /* $@99  */
  YYSYMBOL_485_100 = 485,                  /* $@100  */
  YYSYMBOL_486_101 = 486,                  /* $@101  */
  YYSYMBOL_make_table = 487,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 488,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 489,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 490, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 491,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 492       /* array_comprehension  */
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
#define YYLAST   14496

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  216
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  277
/* YYNRULES -- Number of rules.  */
#define YYNRULES  831
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1525

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   443


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
       2,     2,     2,   199,     2,   215,   210,   195,   188,     2,
     208,   209,   193,   192,   182,   191,   205,   194,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   185,   212,
     189,   183,   190,   184,   211,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   206,     2,   207,   187,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   213,   186,   214,   198,     2,     2,     2,
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
     175,   176,   177,   178,   179,   180,   181,   196,   197,   200,
     201,   202,   203,   204
};

#if DAS_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   526,   526,   527,   532,   533,   534,   535,   536,   537,
     538,   539,   540,   541,   542,   543,   544,   548,   554,   555,
     556,   560,   561,   565,   583,   584,   585,   586,   590,   594,
     599,   608,   616,   632,   637,   645,   645,   684,   714,   718,
     719,   720,   724,   727,   731,   737,   746,   749,   755,   756,
     760,   764,   765,   769,   772,   778,   784,   787,   793,   794,
     798,   799,   800,   809,   810,   814,   815,   815,   821,   822,
     823,   824,   825,   829,   835,   835,   841,   841,   847,   855,
     865,   874,   874,   881,   882,   883,   884,   885,   886,   890,
     895,   903,   904,   905,   909,   910,   911,   912,   913,   914,
     915,   916,   922,   925,   931,   932,   933,   937,   950,   968,
     971,   979,   990,  1001,  1012,  1015,  1022,  1026,  1033,  1034,
    1038,  1039,  1040,  1044,  1047,  1054,  1058,  1059,  1060,  1061,
    1062,  1063,  1064,  1065,  1066,  1067,  1068,  1069,  1070,  1071,
    1072,  1073,  1074,  1075,  1076,  1077,  1078,  1079,  1080,  1081,
    1082,  1083,  1084,  1085,  1086,  1087,  1088,  1089,  1090,  1091,
    1092,  1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,  1101,
    1102,  1103,  1104,  1105,  1106,  1107,  1108,  1109,  1110,  1111,
    1112,  1113,  1114,  1115,  1116,  1117,  1118,  1119,  1120,  1121,
    1122,  1123,  1124,  1125,  1126,  1127,  1128,  1129,  1130,  1131,
    1132,  1133,  1134,  1135,  1136,  1137,  1138,  1139,  1140,  1141,
    1146,  1164,  1165,  1166,  1170,  1176,  1176,  1193,  1197,  1208,
    1217,  1229,  1230,  1231,  1232,  1233,  1234,  1235,  1236,  1237,
    1238,  1239,  1240,  1241,  1242,  1243,  1244,  1245,  1246,  1247,
    1248,  1249,  1253,  1258,  1264,  1270,  1281,  1282,  1286,  1287,
    1291,  1295,  1302,  1302,  1302,  1308,  1308,  1308,  1317,  1351,
    1354,  1357,  1360,  1366,  1367,  1378,  1382,  1385,  1393,  1393,
    1393,  1396,  1402,  1405,  1408,  1412,  1419,  1425,  1429,  1433,
    1436,  1439,  1447,  1450,  1453,  1461,  1464,  1472,  1475,  1478,
    1486,  1492,  1493,  1497,  1498,  1502,  1507,  1515,  1521,  1533,
    1536,  1542,  1542,  1542,  1545,  1545,  1545,  1550,  1550,  1550,
    1558,  1558,  1558,  1564,  1574,  1585,  1600,  1603,  1609,  1610,
    1617,  1628,  1629,  1630,  1634,  1635,  1636,  1637,  1641,  1646,
    1654,  1655,  1659,  1664,  1671,  1678,  1678,  1687,  1688,  1689,
    1690,  1691,  1692,  1693,  1697,  1698,  1699,  1700,  1701,  1702,
    1703,  1704,  1705,  1706,  1707,  1708,  1709,  1710,  1711,  1712,
    1713,  1714,  1715,  1719,  1720,  1721,  1722,  1723,  1724,  1725,
    1726,  1730,  1737,  1749,  1754,  1764,  1768,  1775,  1778,  1778,
    1778,  1783,  1783,  1783,  1796,  1800,  1804,  1810,  1817,  1817,
    1817,  1824,  1828,  1837,  1841,  1844,  1850,  1851,  1852,  1853,
    1854,  1855,  1856,  1857,  1858,  1859,  1860,  1861,  1862,  1863,
    1864,  1865,  1866,  1867,  1868,  1869,  1870,  1871,  1872,  1873,
    1874,  1875,  1876,  1877,  1878,  1879,  1880,  1881,  1882,  1883,
    1884,  1885,  1891,  1892,  1893,  1894,  1895,  1908,  1909,  1910,
    1911,  1912,  1913,  1914,  1915,  1916,  1917,  1918,  1919,  1922,
    1925,  1926,  1929,  1929,  1929,  1932,  1937,  1941,  1945,  1945,
    1945,  1950,  1953,  1957,  1957,  1957,  1962,  1965,  1966,  1967,
    1968,  1969,  1970,  1971,  1972,  1973,  1975,  1979,  1980,  1985,
    1989,  1990,  1991,  1992,  1993,  1994,  1995,  1999,  2003,  2007,
    2011,  2015,  2019,  2023,  2027,  2031,  2038,  2039,  2043,  2044,
    2045,  2049,  2050,  2054,  2055,  2056,  2060,  2061,  2065,  2076,
    2079,  2079,  2098,  2097,  2112,  2111,  2124,  2133,  2139,  2144,
    2154,  2155,  2159,  2162,  2171,  2172,  2176,  2179,  2182,  2198,
    2207,  2208,  2212,  2215,  2218,  2232,  2233,  2237,  2243,  2249,
    2252,  2256,  2262,  2271,  2272,  2273,  2277,  2278,  2282,  2289,
    2294,  2303,  2309,  2320,  2323,  2328,  2333,  2341,  2352,  2355,
    2355,  2375,  2376,  2380,  2381,  2382,  2386,  2389,  2389,  2408,
    2411,  2414,  2429,  2448,  2449,  2450,  2455,  2455,  2481,  2482,
    2486,  2487,  2487,  2491,  2492,  2493,  2497,  2507,  2512,  2507,
    2524,  2529,  2524,  2544,  2545,  2549,  2550,  2554,  2560,  2561,
    2565,  2566,  2567,  2571,  2574,  2580,  2585,  2580,  2599,  2606,
    2611,  2620,  2626,  2637,  2638,  2639,  2640,  2641,  2642,  2643,
    2644,  2645,  2646,  2647,  2648,  2649,  2650,  2651,  2652,  2653,
    2654,  2655,  2656,  2657,  2658,  2659,  2660,  2661,  2662,  2663,
    2667,  2668,  2669,  2670,  2671,  2672,  2676,  2687,  2691,  2698,
    2710,  2717,  2726,  2731,  2734,  2747,  2747,  2747,  2760,  2764,
    2771,  2775,  2782,  2783,  2784,  2785,  2786,  2801,  2807,  2807,
    2807,  2811,  2816,  2823,  2823,  2830,  2834,  2838,  2843,  2848,
    2853,  2858,  2862,  2866,  2871,  2875,  2879,  2884,  2884,  2884,
    2890,  2897,  2897,  2897,  2902,  2902,  2902,  2908,  2908,  2908,
    2913,  2917,  2917,  2917,  2922,  2922,  2922,  2931,  2935,  2935,
    2935,  2940,  2940,  2940,  2949,  2953,  2953,  2953,  2958,  2958,
    2958,  2967,  2967,  2967,  2973,  2973,  2973,  2982,  2985,  2996,
    3012,  3012,  3017,  3022,  3012,  3047,  3047,  3052,  3058,  3047,
    3083,  3083,  3088,  3093,  3083,  3123,  3124,  3125,  3126,  3127,
    3131,  3138,  3145,  3151,  3157,  3164,  3171,  3177,  3187,  3192,
    3199,  3200,  3205,  3206,  3210,  3211,  3215,  3216,  3220,  3221,
    3222,  3226,  3227,  3228,  3233,  3239,  3246,  3254,  3261,  3269,
    3278,  3278,  3278,  3286,  3286,  3286,  3293,  3293,  3293,  3300,
    3300,  3300,  3311,  3314,  3320,  3334,  3340,  3346,  3352,  3352,
    3352,  3362,  3367,  3374,  3382,  3387,  3394,  3402,  3402,  3402,
    3410,  3417,  3417,  3417,  3427,  3432,  3439,  3442,  3448,  3456,
    3465,  3473,  3481,  3494,  3495,  3499,  3500,  3505,  3508,  3511,
    3514,  3517
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
  "\"bool\"", "\"void\"", "\"string\"", "\"auto\"", "\"int\"", "\"int2\"",
  "\"int3\"", "\"int4\"", "\"uint\"", "\"bitfield\"", "\"uint2\"",
  "\"uint3\"", "\"uint4\"", "\"float\"", "\"float2\"", "\"float3\"",
  "\"float4\"", "\"range\"", "\"urange\"", "\"range64\"", "\"urange64\"",
  "\"block\"", "\"int64\"", "\"uint64\"", "\"double\"", "\"function\"",
  "\"lambda\"", "\"int8\"", "\"uint8\"", "\"int16\"", "\"uint16\"",
  "\"tuple\"", "\"variant\"", "\"generator\"", "\"yield\"", "\"sealed\"",
  "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"",
  "\"^=\"", "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"",
  "\">>=\"", "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"", "\"??\"",
  "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"", "\"$ <|\"", "\"@ <|\"",
  "\"@@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"",
  "\">>>=\"", "\"=>\"", "\"::\"", "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"",
  "\"||=\"", "\"^^=\"", "\"..\"", "\"$$\"", "\"$i\"", "\"$v\"", "\"$b\"",
  "\"$a\"", "\"$t\"", "\"$c\"", "\"$f\"", "\"...\"", "\"[[\"", "\"[{\"",
  "\"{{\"", "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"unsigned int8 constant\"", "\"floating point constant\"",
  "\"double constant\"", "\"name\"", "\"keyword\"", "\"type function\"",
  "\"start of the string\"", "STRING_CHARACTER", "STRING_CHARACTER_ESC",
  "\"end of the string\"", "\"{\"", "\"}\"",
  "\"end of failed eader macro\"", "\";}}\"", "\";}]\"", "\";]]\"",
  "\",]]\"", "\",}]\"", "','", "'='", "'?'", "':'", "'|'", "'^'", "'&'",
  "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'", "UNARY_MINUS",
  "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC",
  "DEREF", "'.'", "'['", "']'", "'('", "')'", "'$'", "'@'", "';'", "'{'",
  "'}'", "'#'", "$accept", "program", "top_level_reader_macro",
  "optional_public_or_private_module", "module_name", "module_declaration",
  "character_sequence", "string_constant", "string_builder_body",
  "string_builder", "reader_character_sequence", "expr_reader", "$@1",
  "options_declaration", "require_declaration", "keyword_or_name",
  "require_module_name", "require_module", "is_public_module",
  "expect_declaration", "expect_list", "expect_error", "expression_label",
  "expression_goto", "elif_or_static_elif", "expression_else",
  "if_or_static_if", "expression_else_one_liner", "$@2",
  "expression_if_one_liner", "expression_if_then_else", "$@3",
  "expression_for_loop", "$@4", "expression_unsafe",
  "expression_while_loop", "expression_with", "expression_with_alias",
  "$@5", "annotation_argument_value", "annotation_argument_value_list",
  "annotation_argument_name", "annotation_argument",
  "annotation_argument_list", "annotation_declaration_name",
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
  "kwd_let", "optional_in_scope", "tuple_expansion",
  "tuple_expansion_variable_declaration", "expression_let", "expr_cast",
  "$@13", "$@14", "$@15", "$@16", "$@17", "$@18", "expr_type_decl", "$@19",
  "$@20", "expr_type_info", "expr_list", "block_or_simple_block",
  "block_or_lambda", "capture_entry", "capture_list",
  "optional_capture_list", "expr_block", "expr_full_block",
  "expr_full_block_assumed_piped", "$@21", "expr_numeric_const",
  "expr_assign", "expr_assign_pipe", "expr_named_call", "expr_method_call",
  "func_addr_name", "func_addr_expr", "$@22", "$@23", "$@24", "$@25",
  "expr_field", "$@26", "$@27", "expr_call", "expr", "$@28", "$@29",
  "$@30", "$@31", "$@32", "$@33", "expr_mtag", "optional_field_annotation",
  "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
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
  "make_decl", "make_struct_fields", "make_struct_dim", "optional_block",
  "optional_trailing_semicolon_cur_cur",
  "optional_trailing_semicolon_cur_sqr",
  "optional_trailing_semicolon_sqr_sqr", "optional_trailing_delim_sqr_sqr",
  "optional_trailing_delim_cur_sqr", "make_struct_decl", "$@88", "$@89",
  "$@90", "$@91", "$@92", "$@93", "$@94", "$@95", "make_tuple",
  "make_map_tuple", "make_tuple_call", "$@96", "$@97", "make_dim",
  "make_dim_decl", "$@98", "$@99", "$@100", "$@101", "make_table",
  "expr_map_tuple_list", "make_table_decl", "array_comprehension_where",
  "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1353)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-719)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1353,    48, -1353, -1353,    89,   -74,   233,    72, -1353,   -57,
     528,   528,   528,   156,    11, -1353, -1353, -1353,    97, -1353,
   -1353, -1353,   358, -1353,   133, -1353, -1353, -1353, -1353, -1353,
   -1353, -1353, -1353, -1353, -1353,   163, -1353,   171,   205,   237,
   -1353, -1353, -1353, -1353,   233, -1353,    20, -1353,   528,   528,
   -1353, -1353,   133, -1353, -1353, -1353, -1353, -1353,   260,   318,
   -1353, -1353, -1353,    11,    11,    11,   259, -1353,   641,    -7,
   -1353, -1353, -1353, -1353,   554,   556,   568, -1353,   595,    51,
      89,   316,   -74,   296,   357, -1353,   622,   622, -1353,   384,
     362,    13,   377,   612,   442,   479,   491, -1353,   502,   467,
   -1353, -1353,    -1,    89,    11,    11,    11,    11, -1353, -1353,
   -1353, -1353, -1353, -1353, -1353,   525, -1353, -1353, -1353, -1353,
   -1353,   487, -1353, -1353, -1353, -1353, -1353,   548,    68, -1353,
   -1353, -1353, -1353,   627, -1353, -1353, -1353, -1353, -1353,   466,
   -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353,   537,
   -1353,   152, -1353,   132,   586,   641, 14329, -1353,    77,   637,
   -1353,   594, -1353, -1353,   630, -1353, -1353, -1353, -1353, -1353,
   -1353, -1353,   168, -1353,   570, -1353,   559,   561,   567, -1353,
   -1353, 13261, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353,
   -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353,
   -1353, -1353, -1353, -1353, -1353, -1353, -1353,   723,   736, -1353,
     577,   574,   408, -1353, -1353,   638, -1353,   593,    89,   170,
   -1353, -1353, -1353,    68, -1353,  9825, -1353, -1353, -1353,   642,
     643, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353,
   -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353,
   -1353,   644,   601, -1353, -1353, -1353, -1353, -1353, -1353, -1353,
   -1353, -1353, -1353, -1353, -1353,   793, -1353, -1353, -1353, -1353,
   -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353,   649,   611,
   -1353, -1353,   -52,   636, -1353, -1353, -1353, -1353, -1353, -1353,
   -1353, -1353,   609, -1353,   639,   610, -1353,   594,    92,   619,
     786,   465, -1353, -1353,   647,   650,   651,   645,   660,   662,
   -1353, -1353, -1353,   653, -1353, -1353, -1353, -1353, -1353,   666,
   -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353,
   -1353,   669, -1353, -1353, -1353,   675,   678, -1353, -1353, -1353,
   -1353,   679,   682,   664,   156, -1353, -1353, -1353, -1353, -1353,
     496,   689, -1353, -1353, -1353, -1353, -1353, -1353, -1353,   663,
     742, -1353,   670, -1353,    96, -1353,   497,  9825, -1353,  2071,
   -1353,  -100,   156, -1353, -1353, -1353,   170,   672,  9132,   714,
     715,  9825, -1353,   -46, -1353, -1353, -1353,  9132, -1353, -1353,
     716, -1353,   292,   340,   344, -1353, -1353,  9132,   282, -1353,
   -1353, -1353,    32, -1353, -1353, -1353,    38,  5332, -1353,   680,
    9619,   -87,  9718,   -78, -1353, -1353,  9132,   199,   676,   843,
   -1353,    -2, -1353,   254,   689, -1353,   698,   700,  9132, -1353,
   -1353, -1353, -1353, -1353,    91,   683,   701,   361,  3132, -1353,
   -1353,   574,   392,  5532,   684,  9132,   726,   706,   708,   690,
   -1353,   687,   712,   740,  5732,   400,   393,   713, -1353,   394,
     717,   718,  3332,  9132,  9132,   513,   513,   513,   695,   696,
     697,   702,   703,   704, -1353,  1437,  1687,  5932, -1353, -1353,
   -1353, -1353, -1353, -1353, -1353,  6132,   719, -1353,  9132,  9132,
    9132,  9132,  9132,  6332,  9132, -1353,   705, -1353,  6532,   873,
   -1353, -1353,   709,   710,  9132,   904, -1353, -1353, -1353, -1353,
   -1353, -1353, -1353, -1353, -1353,   566, -1353,   -77,   722, -1353,
     724,   725,   728, -1353,   729, -1353, -1353,   849, -1353, -1353,
   -1353, -1353,   721, -1353, -1353,   -73, -1353, -1353, -1353, -1353,
   -1353,   566,  9441, -1353,   707, -1353, -1353, -1353, -1353, -1353,
   -1353, -1353,    58, -1353,   730, -1353,    33, -1353, -1353,   720,
     731, -1353, -1353, 10402, -1353,   881,   654, -1353, -1353, -1353,
    3732,  9825,  9825,  9825, 10500,  9825,  9825,   733,   757,  9825,
     577,  9825,   577,  9825,   577, 10113,   758, 10598, -1353,  9132,
   -1353, -1353, -1353, -1353,   739, -1353, -1353, 12677,  9132, -1353,
     496,   735, -1353,   737,     3, -1353,   738,   689,   746,   744,
   -1353,   741,   747, 10633, -1353,  9132,   763, -1353,   764, -1353,
    9825, -1353,  3732, -1353, -1353, -1353,  1830,   199, -1353,   756,
   -1353, -1353,   156, -1353, -1353,  9132,  3732, -1353,   761,   820,
   13150,   953,  9825,  9132,  9132, 13625,  9132,  1830,   788, -1353,
   -1353,  9132, -1353, -1353,   787,   821, 13625,  9132, -1353, -1353,
    9132, -1353, -1353,  9132, -1353,  9825,  3732, -1353, 13150,   499,
     499,   771, -1353,   721, -1353, -1353, -1353,  9132,  9132,  9132,
    9132,  9132,  9132,   199,  2496,   199,  2708,   199, 13296, -1353,
      94, -1353,  1830, -1353,   544,   499,   499,    -3,   499,   499,
     199,   792, 13625,   792,   158,   199, -1353,   802,   772, -1353,
   -1353,  1830, -1353, -1353, -1353, -1353,  4332, -1353, -1353, -1353,
   -1353, -1353, -1353,   428,   831,   513, -1353, -1353, 14172, 14203,
    9132,  9132,  9132,  9132,  9132,  9132,  9132,  9132,  9132,  9132,
   -1353, -1353,  9132,  9132,  9132,  9132,  9132,  9132,   822,  3932,
    9132,   306,  9132,  9132,  9132,  9132,  9132,  9132,  9132,  9132,
    9132,  9132,  9132,  9132,  9132,  9132,  9132,  4132,  6732,  9132,
    9132,  9132,  9132,  9132,  9132,  9132,  9132,  9132,  9132,   150,
    9132,  6932,  -100,  9132, -1353, -1353,    11, -1353,   983,   594,
   -1353,   824, -1353,  3732, -1353,  9826,   313,   354,   803,   -19,
   -1353,   402,   447, -1353, -1353,   177,   469,   636,   470,   636,
     495,   636, -1353,   193, -1353,   298, -1353,  9825,   785,   761,
   -1353, -1353, 12712, -1353, -1353,  9825, -1353, -1353,  9825, -1353,
   -1353, -1353, 10731, -1353,   954,   -56, -1353, 13150,  9825,  9825,
   -1353,    19,  9825,  4532,  7132,  9825,  9825,   792, -1353,   761,
    9132,  9132,  9132,   574,   546,   802, 13625, 10829, -1353, -1353,
    9825,  9825, 10864,  9825, -1353, -1353, 10962,  9825,   792,  9825,
    9825,   792,  9825,   130, -1353, 13150, -1353,   831, 11060, 11095,
   11193, 11291, 11326, 11424,    23,   513,   791,   160,  2920,  4732,
    7332, 13394,   818,    -5,   178,   819,   -25,    24,  7532,    -5,
     137,    27,  9132, -1353,  9132,   789, -1353,  9825, -1353,  9825,
   -1353,  9132,   596,    40,  9132,   799,   798,   800,   804,   353,
   -1353, -1353,    42,  9132,   796, -1353,   486,  9132,   721,  4932,
   -1353,   216,   516,   844, -1353, -1353,   -50,   577, -1353,   828,
     805, -1353, -1353,   829,   815, -1353, -1353, 13625, 13625, 13625,
   13625, 13625, 13625, 13625, 13625,   966,   966,  1080, 13625, 13625,
    1080,   842,   842,   816,   513,   513,   513, -1353, 13625,   471,
     817, -1353, 12810,    -9,    -9,   707, 13625,   966,   966, 13625,
   13625, 13986, 13856, 13899, 13625, 13625, 13625, 13723,   513,   513,
     513, -1353, 13625, 14298, 13492,  9975, 14010, 14034,  1080,  1080,
     321,   321,   471,   471,   471,   386,  9132,   834,   835,   412,
    9132,  1021, 12908, -1353,   221,   812, 10114,   145,   634,   962,
     832, -1353, -1353,  9935, -1353, -1353, -1353, -1353,  9825, -1353,
   -1353, -1353,   879, -1353,   859, -1353,   861, -1353,   862, -1353,
   10113, -1353,   758,   327,   689, -1353, -1353,   689,   689, -1353,
     889,  9132,   629,   920,  9132,   508, -1353,   225,   526,   890,
   11522,   509,  1055,   850, 13625, -1353,   852,  9825,   853, -1353,
    9132,  1245,  1532, -1353,  1540, -1353,  1616,   863,  1624,  1854,
     864,  2347,   831,   577, -1353, -1353, -1353, -1353, -1353,   867,
    9132, -1353,  9132,  9132,  9132,  5132, 12677,     1,  9132,   436,
     456,   178, -1353, -1353,   851, -1353,  9132, -1353,   869,  9132,
   -1353,  9132,   456,   -15, -1353,   871, -1353,  9132, 13625, -1353,
   -1353,   352,   359, 13527,  9132, -1353, -1353,  9132,  9132,  9825,
     577,  9132, -1353, -1353,  2285,   574, -1353, -1353,  9132, -1353,
    1426,   831,   253,  9332, -1353, -1353, -1353,   271,   903,   912,
     917,   918, -1353,   273,   636, -1353,  9132, -1353,  9132,  7732,
   -1353, -1353, -1353,  9132, -1353, -1353, -1353, -1353,   897,   880,
   -1353, -1353,  9132,   882, -1353, 12943,  9132,  7932,   895, -1353,
   13041, -1353, -1353, -1353, -1353, -1353,   885, -1353, -1353,   294,
   -1353,    15, -1353, -1353, -1353, -1353, -1353,   689, -1353, -1353,
   -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353,
   -1353, -1353,   898,  9825, -1353, 13625, -1353, -1353,    79, -1353,
   -1353, -1353,   364, -1353, -1353, -1353, -1353,  9132,  2850, -1353,
   13625, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353,
     899,   636,  8132,   -83, 11557, 13625, 13625,   -18,    -5,   178,
   13625,   901,    12,   818, -1353, -1353, 13625, -1353,   819,    75,
      -5, -1353, -1353,   905, -1353, -1353,    83, -1353, -1353, -1353,
     104, 11655, 11753,  3062,   636,   121, -1353, -1353,  1830, -1353,
     893,   577,   908,  4932,   946,   911, -1353, -1353, -1353, -1353,
     -50,   913,   -81,  9825, 11788,  9825, 11886, -1353,   257, 11984,
   -1353,  9132, 13758,  9132, -1353, 12019, -1353,   261,  9132, -1353,
   -1353, -1353,  1087,    15, -1353, -1353,   634, -1353, -1353, -1353,
    9132,   689,   914,   915, -1353, -1353,  9132,   957, -1353,   919,
     802,   921,  9132,  9132,  9132,   922, -1353,   925,   926,  8332,
     -81, -1353,   270,  9132,   106,   178, -1353,  9132,  9132,  9132,
    9132,   -15, -1353,  9132,  9132,   928,  9132, -1353, -1353, -1353,
     931,  9132,   486,  3532, -1353,   636, -1353,   287, -1353,   555,
   -1353, -1353,  8532, -1353, -1353,  3253, -1353,   511, -1353, -1353,
   -1353,  9825, 12117, 12215, -1353, -1353, 12250, -1353, -1353,  1087,
     199,   916,   456,   456, 12348,   938,  9132,   929,  9132,  1080,
    1080,  1080,  9132,   456,   456, -1353, 12446, -1353, -1353, 10223,
    9132,  9132, -1353, 12481, 13625, 13625, 10223, -1353,   927,  1080,
    9132, -1353, 10223,   255, -1353,   927, -1353,  8732,  8932, -1353,
   -1353, -1353, -1353, -1353, 13625,   574,   932,  9825,   -46,  9132,
   13723, -1353, -1353,   517, -1353, -1353, -1353, 14329, -1353, -1353,
   -1353,   530,   536, -1353,  9132,   792, -1353,   802,   792,   547,
     557, -1353,  1085,   933, 13625, 13625,   118,   930,   934,   936,
     935, -1353,   255,   937,  9132,  9132, 13625, -1353, -1353,   814,
    9132, 13758, -1353, -1353, -1353,   940, 14329, -1353, -1353, 12579,
     941,   944,   945, -1353, -1353,  9132,   948,  9132,  9132,   949,
     950, -1353, -1353, -1353, -1353, 13625,  9132, 10258, -1353, -1353,
     574, -1353, -1353, -1353, -1353, 13625, -1353, 13625, 13625, -1353,
   -1353, 10367, -1353, -1353, -1353
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   118,     1,   291,     0,     0,     0,   581,   292,     0,
     573,   573,   573,     0,     0,    16,    15,     3,     0,    10,
       9,     8,     0,     7,   561,     6,    11,     5,     4,    13,
      12,    14,    92,    93,    91,   100,   102,    37,    53,    50,
      51,    39,    40,    41,     0,    42,    48,    38,   573,   573,
      22,    21,   561,   575,   574,   740,   730,   735,     0,   263,
      35,   105,   106,     0,     0,     0,   107,   109,   116,     0,
     104,    17,   599,   598,   211,   583,   600,   562,   563,     0,
       0,     0,     0,    43,     0,    49,     0,     0,    46,     0,
       0,   573,     0,    18,     0,     0,     0,   265,     0,     0,
     115,   110,     0,     0,     0,     0,     0,     0,   119,   213,
     212,   215,   210,   585,   584,     0,   602,   601,   605,   565,
     564,   567,    98,    99,    96,    97,    95,     0,     0,    94,
     103,    54,    52,    48,    45,    44,   576,   578,   580,     0,
     582,    19,    20,    23,   741,   731,   736,   264,    33,    36,
     114,     0,   111,   112,   113,   117,     0,   586,     0,   595,
     558,   496,    24,    25,     0,    87,    88,    85,    86,    84,
      83,    89,     0,    47,     0,   579,     0,     0,     0,    34,
     108,     0,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,     0,     0,   125,
     120,     0,     0,   587,   596,     0,   606,   559,     0,     0,
      26,    27,    28,     0,   101,     0,   742,   732,   737,   179,
     180,   177,   128,   129,   131,   130,   132,   133,   134,   135,
     161,   162,   159,   160,   152,   163,   164,   153,   150,   151,
     178,   172,     0,   176,   165,   166,   167,   168,   139,   140,
     141,   136,   137,   138,   149,     0,   155,   156,   154,   147,
     148,   143,   142,   144,   145,   146,   127,   126,   171,     0,
     157,   158,   496,   123,   242,   216,   640,   643,   641,   644,
     642,   645,     0,   569,   593,   603,   566,   496,     0,     0,
     548,   546,   568,    90,     0,     0,     0,     0,     0,     0,
     613,   633,   614,   647,   615,   619,   620,   621,   622,   639,
     626,   627,   628,   629,   630,   631,   632,   634,   635,   636,
     637,   700,   618,   625,   638,   707,   714,   616,   623,   617,
     624,     0,     0,     0,     0,   646,   662,   665,   663,   664,
     727,   577,   652,   526,   532,   181,   182,   175,   170,   183,
     173,   169,     0,   121,     0,   520,     0,     0,   214,     0,
     590,   588,     0,   597,   509,   607,     0,     0,     0,     0,
       0,     0,   547,     0,   668,   691,   694,     0,   697,   687,
       0,   655,   701,   708,   715,   721,   724,     0,     0,   677,
     682,   676,     0,   690,   686,   679,     0,     0,   681,   666,
       0,   743,   733,   738,   184,   174,     0,     0,     0,   608,
     517,   537,   122,   496,   124,   244,     0,     0,     0,    63,
      64,    76,   402,   403,     0,     0,     0,     0,   279,   396,
     277,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     278,     0,     0,     0,     0,     0,     0,     0,   639,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   485,     0,     0,     0,   337,   339,
     338,   340,   341,   342,   343,     0,     0,    29,     0,     0,
       0,     0,     0,     0,     0,   321,   322,   221,     0,   217,
     400,   399,     0,     0,     0,     0,   237,   232,   229,   228,
     230,   231,   262,   243,   223,   479,   222,   397,     0,   470,
      71,    72,    69,   235,    70,   236,   238,   294,   227,   469,
     468,   467,   118,   473,   398,     0,   224,   472,   471,   443,
     404,   444,   344,   405,     0,   401,   745,   749,   746,   747,
     748,   569,     0,   570,     0,   594,   510,   560,   497,     0,
       0,   479,   444,     0,   550,   551,     0,   544,   545,   543,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     120,     0,   120,     0,   120,     0,     0,     0,   673,   246,
     684,   685,   678,   680,     0,   683,   667,     0,     0,   729,
     728,     0,   653,     0,   263,   527,     0,   522,     0,     0,
     533,     0,     0,     0,   518,     0,     0,   536,     0,   535,
       0,   538,     0,   521,   780,   783,     0,     0,   268,   272,
     271,   276,     0,   310,   807,     0,     0,   283,   280,     0,
     316,     0,     0,     0,     0,   266,     0,     0,     0,   301,
     304,     0,   241,   307,     0,     0,    57,     0,    78,   811,
       0,   789,   798,     0,   786,     0,     0,   288,   285,   432,
     433,   322,   332,   118,   261,   259,   260,     0,     0,     0,
       0,     0,     0,     0,   760,     0,     0,     0,   796,   814,
       0,   252,     0,   255,     0,   409,   408,   445,   407,   406,
       0,   825,   316,   825,   323,     0,   816,   825,     0,   239,
     240,     0,    74,   335,   220,   477,     0,   226,   233,   234,
     282,   287,   293,     0,   330,     0,   225,   219,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   388,
       0,     0,   591,     0,   571,   589,     0,   604,     0,   496,
     549,     0,   553,     0,   557,   344,     0,     0,     0,   658,
     671,     0,     0,   648,   650,     0,     0,   123,     0,   123,
       0,   123,   524,     0,   530,     0,   649,     0,     0,   247,
     675,   660,     0,   654,   744,     0,   528,   734,     0,   534,
     739,   519,     0,   610,   611,   539,   542,   541,     0,     0,
      79,     0,     0,     0,     0,     0,     0,   825,   284,   281,
       0,     0,     0,     0,     0,   825,   267,     0,    80,    81,
       0,     0,     0,     0,    55,    56,     0,     0,   825,     0,
       0,   825,     0,     0,   289,   286,   323,   330,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   263,     0,     0,
       0,   792,   758,   760,     0,   801,     0,     0,     0,   760,
       0,     0,     0,   763,     0,     0,   819,     0,   245,     0,
      32,     0,    30,     0,   826,     0,     0,     0,     0,   378,
     375,   377,     0,   826,     0,   242,    60,     0,   118,     0,
     391,     0,     0,     0,   300,   299,     0,   120,   258,     0,
       0,   456,   455,     0,     0,   457,   461,   354,   355,   357,
     356,   358,   348,   349,   350,   410,   411,   423,   359,   360,
     424,   421,   422,     0,     0,     0,     0,   370,   346,   450,
       0,   441,     0,   474,   475,   476,   347,   412,   413,   361,
     362,   428,   429,   430,   351,   352,   353,   431,     0,     0,
       0,   369,   345,     0,     0,   426,   427,   425,   419,   420,
     415,   414,   416,   417,   418,     0,     0,     0,   384,     0,
       0,     0,     0,   394,     0,     0,     0,     0,   503,   506,
       0,   552,   555,   344,   556,   669,   692,   695,     0,   698,
     688,   656,     0,   702,     0,   709,     0,   716,     0,   722,
       0,   725,     0,     0,   250,   672,   661,   523,   529,   609,
       0,     0,     0,     0,     0,     0,   273,     0,     0,     0,
       0,     0,     0,     0,   317,   290,     0,     0,     0,   446,
       0,     0,     0,   447,     0,   478,     0,     0,     0,     0,
       0,     0,   330,   120,   480,   481,   482,   483,   484,     0,
       0,   761,     0,     0,     0,     0,   316,   760,     0,     0,
       0,     0,   769,   770,     0,   775,     0,   767,     0,     0,
     804,     0,     0,     0,   765,     0,   805,     0,   795,   815,
     762,     0,     0,     0,     0,   803,   436,     0,     0,     0,
     120,     0,   817,   818,     0,     0,    58,    59,     0,    73,
      65,   330,     0,     0,   393,   392,   295,     0,     0,     0,
       0,     0,   328,     0,   123,   452,     0,   458,     0,     0,
     368,   366,   367,     0,   439,   365,   363,   364,     0,     0,
     462,   466,     0,     0,   442,     0,     0,     0,     0,   385,
       0,   389,   437,   395,   592,   572,   119,   504,   505,   506,
     507,   498,   511,   554,   670,   693,   696,   659,   699,   689,
     657,   651,   703,   705,   710,   712,   717,   719,   723,   525,
     726,   531,     0,     0,   612,   540,   781,   784,     0,   269,
     274,   275,     0,   313,   311,   808,   806,     0,     0,   820,
      82,   302,   305,   308,   812,   810,   790,   799,   797,   787,
       0,   123,     0,     0,     0,   751,   750,     0,   760,     0,
     793,     0,     0,   759,   774,   768,   794,   766,   802,     0,
     760,   772,   773,     0,   778,   764,     0,   253,   256,    31,
       0,     0,     0,     0,   123,     0,   218,    61,     0,    66,
       0,   120,     0,     0,     0,     0,   326,   327,   325,   324,
       0,     0,     0,     0,     0,     0,     0,   373,     0,     0,
     463,     0,   451,     0,   440,     0,   386,     0,     0,   438,
     390,   516,   501,   498,   499,   500,   503,   706,   713,   720,
     246,   251,     0,     0,    77,   270,     0,     0,   312,     0,
     825,     0,     0,     0,     0,     0,   791,     0,     0,     0,
       0,   486,     0,     0,     0,     0,   776,     0,     0,     0,
       0,     0,   771,     0,     0,   248,     0,   376,   495,   379,
       0,     0,    60,     0,    75,   123,   371,     0,   296,   546,
     329,   331,     0,   318,   334,     0,   494,     0,   492,   374,
     489,     0,     0,     0,   488,   387,     0,   502,   512,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   303,
     306,   309,     0,     0,     0,   448,     0,   333,   487,   823,
       0,     0,   777,     0,   753,   752,   823,   779,   823,   254,
     246,   257,   823,     0,   382,   823,    62,   279,     0,    67,
      71,    72,    69,    70,    68,     0,     0,     0,     0,     0,
     319,   453,   459,     0,   493,   491,   490,     0,   514,   508,
     674,     0,     0,   314,     0,   825,   821,   825,   825,     0,
       0,   449,     0,     0,   755,   754,     0,     0,     0,     0,
       0,   380,     0,     0,     0,     0,   285,   336,   372,     0,
       0,   320,   454,   460,   464,     0,     0,   782,   785,     0,
       0,     0,     0,   800,   788,     0,     0,     0,     0,     0,
       0,   249,   827,   383,   830,   286,     0,     0,   465,   513,
       0,   315,   809,   822,   813,   824,   828,   757,   756,   829,
     831,     0,   298,   515,   297
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1353, -1353, -1353, -1353, -1353, -1353,   449,  1068, -1353, -1353,
   -1353,  1161, -1353, -1353, -1353,   711,  1119, -1353,  1032, -1353,
   -1353,  1084, -1353, -1353, -1353,  -195, -1353, -1353, -1353,  -194,
   -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353,   947,
   -1353, -1353,  1088,   -54, -1353, -1353,   265,   387,  -499,  -550,
    -790, -1353, -1353, -1353, -1336, -1353, -1353,  -209,  -588, -1353,
     250, -1353, -1265, -1353,  -160,  -353, -1353, -1353, -1353, -1353,
    -399,   -13, -1353, -1353, -1353, -1353, -1353,  -186,  -185,  -184,
   -1353,  -182, -1353, -1353,  1181, -1353, -1353, -1353, -1353, -1353,
   -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353,
    -325,  -157,  -443,  -105, -1353,  -848,  -453, -1353,   646, -1353,
   -1353,  -363, -1353, -1353, -1353, -1352, -1353, -1353, -1353, -1353,
   -1353, -1353, -1353, -1353,  -334,   576, -1353, -1353, -1353, -1353,
   -1353, -1353, -1353,  -150,  -127,  -201,  -126,     0, -1353, -1353,
   -1353, -1353, -1353,   769, -1353,  -565, -1353, -1353,  -568, -1353,
   -1353,  -411,  -414,  -561,  -172, -1353,  -350, -1353, -1353,  1147,
   -1353, -1353, -1353,   658,   783,    86, -1353, -1353, -1353, -1353,
   -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353,
   -1353, -1353, -1353, -1353, -1353, -1353,  -361,  -197, -1353,   767,
   -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353,  -374, -1353,
   -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353,
   -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353,
   -1353, -1353, -1353, -1353, -1353, -1353,  -163, -1353, -1353, -1353,
   -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353,
   -1353, -1353,   768,  -902,  -671,  -868, -1353, -1353, -1353, -1064,
    -147, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353, -1353,
     101,  -473, -1353, -1353, -1353,   531, -1353, -1353, -1353, -1353,
   -1353, -1353,  -634, -1353,  -782,  -695, -1353
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    16,   143,    52,    17,   164,   170,   694,   500,
     149,   501,    99,    19,    20,    45,    46,    47,    88,    21,
      39,    40,   502,   503,  1138,  1139,   504,  1280,  1363,   505,
     506,   927,   507,   627,   508,   509,   510,   511,  1070,   171,
     172,    35,    36,    37,    66,    67,    68,    69,    22,   283,
     368,   210,    23,   111,   211,   112,   156,   672,   512,   513,
     369,   514,   818,  1421,  1043,   561,   907,  1354,   909,  1355,
     516,   517,   518,   629,   842,  1325,   519,   520,   521,   522,
     523,   524,   525,   526,   527,   723,  1147,   934,   528,   529,
     860,  1332,   861,  1333,   863,  1334,   530,   845,  1328,   531,
     701,  1374,   532,  1152,  1153,   937,   674,   533,   714,   928,
     534,   639,   536,   537,   538,   921,   539,  1129,  1423,  1130,
    1472,   540,  1011,  1310,   562,   702,  1293,  1482,  1295,  1483,
    1381,  1508,   543,   364,  1316,  1388,  1189,  1191,  1020,   556,
     789,  1447,  1486,   365,   366,   606,   813,   412,   611,   815,
     413,  1094,   420,   570,   383,   301,   302,   217,   297,    78,
     121,    25,   161,   371,    89,    90,   174,    91,    26,    49,
     115,   158,    27,   293,   554,   551,  1015,   373,   215,   216,
      76,   118,   375,    28,   159,   295,   421,   544,   292,   347,
     348,   805,   411,   349,   578,  1200,   798,   409,   350,   571,
    1194,   817,   576,  1199,   572,  1195,   573,  1196,   575,  1198,
     579,  1202,   580,  1317,   581,  1204,   582,  1318,   583,  1206,
     584,  1319,   585,  1208,   586,  1210,   607,    29,    95,   177,
     353,   608,    30,    96,   178,   354,   612,    31,    94,   176,
     352,   603,   545,   892,   893,   894,   906,  1116,  1110,  1105,
    1264,   546,   838,  1322,   839,  1323,   872,  1338,   869,  1336,
     895,   706,   547,   870,  1337,   896,   548,   846,  1329,   867,
    1335,   690,   707,   549,  1463,   915,   550
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,    70,   285,   614,   689,   793,   535,   566,   916,   855,
     622,   219,   924,   675,   676,   899,   515,  1034,   814,  1036,
     812,  1038,   673,   673,   673,  1101,   557,  1142,   346,  1083,
     807,  1113,   809,   724,   811,   541,   600,  1254,  1054,   637,
    -118,    84,  1090,  1111,    61,   932,  1117,   885,     2,   151,
      70,    70,    70,   885,     3,  1391,   725,   788,  1372,  1124,
    1314,  1131,   351,   667,   590,   122,   123,   552,   617,    53,
     592,  1471,   713,    62,  1148,    54,    85,     4,   567,     5,
     601,     6,   165,   166,  1149,   515,    38,     7,   568,   609,
     715,    70,    70,    70,    70,   426,   427,     8,   713,   850,
     362,   684,   686,     9,   541,   218,   617,    32,    33,   515,
      50,  1485,   553,   638,   748,   437,  1093,   751,   752,  1315,
    1503,   442,   617,   751,   752,   602,    10,   619,   541,  1343,
     410,   716,   284,  1150,   610,    92,   617,   569,  1151,   726,
     104,   105,   106,    98,    63,   417,  1348,   376,    11,    12,
    1510,    58,  1063,    51,  1107,  1469,   886,   363,   456,   457,
    1068,   967,   399,  1261,   298,   619,  1262,   410,  1247,   703,
     346,   794,  1058,  1077,   877,   107,  1080,   139,    59,   991,
     618,   619,  1108,   620,   346,  1346,   621,  1109,   825,   400,
     401,   459,   460,  1028,    77,   619,   779,   780,  1253,  1263,
     108,   618,   779,   780,   424,   618,   618,  1100,   150,   618,
      64,   124,   345,   346,    86,   346,   125,   515,   126,    65,
     591,   127,   618,   836,   618,    87,   593,   138,   167,  1249,
     617,    58,   641,   168,  1240,   169,   541,   848,   127,   786,
    1410,   783,   617,    13,   594,   418,   658,   787,   475,   476,
     477,   402,  1497,   595,    14,   403,    34,   850,    59,   128,
      15,   850,   212,   419,   819,   850,   841,   874,   854,   515,
     784,   903,   938,   104,    80,   106,  1005,  1006,   346,   346,
     628,  1412,   673,   515,   617,    48,   850,  1350,   541,   619,
     213,   873,   284,  1281,  1093,  1353,    58,   493,    58,   377,
      98,   619,   541,   850,   498,  1007,   904,   917,   905,    71,
     847,   849,   918,   515,   404,  1114,  1356,  1008,   405,   299,
    1082,   406,   884,    59,   897,    59,   901,   107,   100,   101,
     102,   398,   541,  1361,    80,   868,   407,   300,   871,   913,
    1312,   728,   729,   619,   922,   408,    79,   919,   418,  1109,
     223,  1115,  1186,    80,   345,  1009,  1010,  1102,  1103,   555,
    1190,   180,    72,    73,  1292,    74,   419,  1031,   345,   152,
     153,   154,   155,   935,   346,   346,   346,   224,   346,   346,
    1345,  1367,   346,  1039,   346,  1104,   346,  1154,   346,  1032,
      81,   931,  1351,    75,  1022,    58,   515,   345,  1143,   345,
      41,    42,    43,   850,   917,  1040,   362,   850,   796,   797,
     799,   218,   801,   802,   515,   541,   806,   840,   808,    82,
     810,   345,    59,   346,  1051,  1144,  1248,    97,    44,  1141,
    1183,  1119,  1091,   541,  1220,  1099,   740,   741,   858,   850,
     515,  1260,   673,   850,   748,   346,   750,   751,   752,   753,
    1132,  1340,   850,  1284,   754,  1290,  1014,   835,    98,   541,
    1282,   970,   345,   345,  1052,  1053,  1379,   103,   346,  1099,
    1385,   588,  1062,   971,  1211,  1209,   131,  -704,  1285,  1408,
    1291,   286,  -704,   908,  1360,   287,  1071,  1072,  1041,  1074,
     589,   728,   729,  1076,  1436,  1078,  1079,  1135,  1081,   410,
    -704,    87,   926,  1025,   288,   289,   290,   291,  1136,  1137,
    1042,  1160,  1161,  1162,   776,   777,   778,  1212,  1057,   728,
     729,   673,   673,   673,   133,  -711,   779,   780,   399,  -718,
    -711,   942,   946,  1241,  -718,  1165,  1166,  1167,  -381,  1213,
     410,  1173,  1267,  -381,  1026,   673,   673,   673,  -711,  1268,
     634,   136,  -718,  1174,  1326,   400,   401,   975,   345,   345,
     345,  -381,   345,   345,  1213,   703,   345,  1178,   345,   635,
     345,  1213,   345,   703,   137,  1435,  1327,   299,   399,  1179,
    1274,   642,   659,   662,    53,   933,   740,   741,   410,   140,
      54,  1251,  1029,  1330,   748,   300,   750,   751,   752,   753,
     643,   660,   663,  1252,   754,   400,   401,   345,   657,   144,
     109,   886,   113,   284,   740,   741,   110,   402,   114,   844,
     346,   403,   748,  1247,   116,   751,   752,   753,   346,   345,
     117,   346,   754,   410,  1467,  1397,  1468,  1030,   148,  1019,
    1470,   346,   346,  1473,  1065,   346,   145,   380,   346,   346,
     381,   119,   345,   382,  1044,   410,   410,   120,   146,  1033,
    1035,   399,  1047,   346,   346,  1048,   346,   402,   141,   147,
     346,   403,   346,   346,   142,   346,   779,   780,   175,  1055,
     404,   410,  1061,    85,   405,  1037,   399,   406,   400,   401,
    1187,   920,   157,  1228,   410,   410,  1188,   410,  1219,  1224,
     160,  1442,   407,   410,   779,   780,   422,  1484,   179,   423,
     346,   408,   346,   400,   401,   162,   163,   910,   911,   162,
     163,  1451,  1452,   495,   671,  1145,   284,   104,  1100,  1218,
     404,  1365,  1459,  1460,   405,  1221,  1066,   406,  1100,  1487,
    1437,   214,  1100,   382,  1044,  1488,  1044,  1121,  1100,  1122,
     402,   218,   407,   225,   403,  1273,  1493,   280,  1067,  1100,
    1490,   408,  1491,  1492,  1457,  1243,  1494,   220,   221,  1100,
     281,   535,   226,    70,   227,   402,   495,   671,   567,   403,
     228,   515,   104,   105,   106,   282,  1259,   284,   568,    41,
      42,    43,  1266,    55,    56,    57,  1171,   134,   135,  1270,
     541,   220,   221,   222,   345,   294,  1275,   296,   358,   355,
     356,   357,   345,   404,   359,   345,   360,   405,   361,  1216,
     406,   367,   370,   374,   372,   345,   345,   378,   379,   345,
     414,   346,   345,   345,  1298,   407,   384,   569,   404,   385,
     386,  1349,   405,   346,   408,   406,   399,   345,   345,   388,
     345,   389,  1307,   387,   345,   391,   345,   345,   392,   345,
     407,   390,   728,   729,   393,  1197,   792,   394,   395,   408,
     346,   396,   397,   400,   401,   410,   415,  1480,   416,   558,
    1418,   564,   565,   577,   615,   616,   598,   624,  1425,   625,
     633,   632,   646,   648,   345,   649,   345,   650,   651,   652,
     654,   653,   661,   677,   678,   679,   664,   665,   693,   708,
     680,   681,   682,   712,   722,   781,   704,  1342,  1506,  1375,
     691,   709,   710,   791,   804,   609,  1277,    14,   657,   828,
     833,   834,   346,  1411,   717,   402,   718,   719,   567,   403,
     720,   721,   803,   850,   785,   542,   820,   823,   568,   725,
     826,   824,   399,   829,   563,   738,   739,   740,   741,   742,
     827,   830,   745,   574,   843,   748,   853,   750,   751,   752,
     753,   859,   864,   587,   914,   754,  1449,   756,   757,   400,
     401,   865,   876,   597,   923,   925,   728,   729,   936,   963,
    1018,  1021,   613,  1027,  1045,   819,  1050,   569,   404,  1092,
    1099,  1106,   405,  1120,   626,   406,  1125,  1126,  1127,  1324,
    1133,  1146,  1128,  1156,   640,   345,   346,  1155,  1157,   645,
     407,   647,  1181,  1158,  1159,  1163,  1184,   345,  1190,   408,
     656,   772,   773,   774,   775,   776,   777,   778,   668,   669,
     670,   402,  1176,  1177,  1192,   403,  1201,   779,   780,  1203,
    1321,  1205,  1207,   688,   345,  1498,  1214,  1222,  1255,  1226,
    1227,   692,  1229,  1479,   695,   696,   697,   698,   699,  1362,
    1286,  1455,  1235,  1238,   688,  1242,  1257,  1458,  1265,  1287,
     711,   740,   741,  1373,  1288,  1289,  1300,   399,  1301,   748,
    1303,   750,   751,   752,   753,   819,   346,  1311,   346,   754,
     728,   729,   638,  1308,   404,  1364,  1320,  1339,   405,  1347,
    1217,   406,  1352,  1368,   400,   401,   345,  1366,  1369,  1387,
    1371,  1424,  1392,  1393,  1395,  1450,   407,  1396,  1454,  1398,
    1402,  1373,  1377,  1403,  1404,   408,  1420,  1495,  1456,  1462,
    1496,  1478,  1502,   912,  1499,  1501,   795,   129,  1500,   849,
    1512,  1504,  1509,  1513,  1514,  1516,  1519,   774,   775,   776,
     777,   778,    18,    83,  1520,   173,   132,  1426,   130,  1429,
     303,   779,   780,  1017,   822,  1134,   402,  1430,  1431,  1432,
     403,  1433,    24,  1407,   346,  1370,  1389,   727,  1448,  1313,
    1390,   832,   623,   738,   739,   740,   741,  1438,   837,    93,
     345,   630,   631,   748,  1417,   750,   751,   752,   753,   782,
    1258,     0,   640,   754,     0,   756,   757,   900,  1443,   688,
     856,     0,   857,     0,     0,     0,  1477,   862,     0,     0,
       0,     0,     0,   866,     0,     0,     0,     0,     0,   404,
     346,     0,   875,   405,     0,  1225,   406,     0,     0,     0,
       0,     0,     0,   878,   879,   880,   881,   882,   883,     0,
     891,   407,   891,     0,     0,     0,     0,     0,     0,     0,
     408,   774,   775,   776,   777,   778,     0,   399,     0,     0,
     345,     0,   345,     0,     0,   779,   780,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1523,     0,     0,   400,   401,   947,   948,   949,   950,
     951,   952,   953,   954,   955,   956,     0,     0,   957,   958,
     959,   960,   961,   962,     0,   968,   969,     0,   972,   973,
     974,   976,   977,   978,   979,   980,   981,   982,   983,   984,
     985,   986,   987,   992,   994,   995,   996,   997,   998,   999,
    1000,  1001,  1002,  1003,  1004,     0,  1012,     0,     0,  1016,
       0,     0,     0,     0,     0,     0,   402,     0,   345,  1023,
     403,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     920,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1060,     0,     0,     0,   345,     0,  1064,   968,   992,   404,
       0,     0,     0,   405,     0,  1231,   406,  1279,     0,     0,
       0,     0,     0,     0,     0,     0,   728,   729,     0,   683,
       0,   407,     0,     0,     0,   304,     0,     0,     0,   920,
     408,   305,     0,     0,   695,  1096,     0,   306,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   307,  1118,     0,
     688,     0,     0,     0,     0,   308,     0,  1123,     0,     0,
    1064,     0,     0,     0,     0,     0,     0,     0,     0,   688,
     309,     0,     0,  1140,     0,     0,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   738,
     739,   740,   741,   742,     0,     0,   745,   746,   747,   748,
       0,   750,   751,   752,   753,     0,     0,     0,     0,   754,
       0,   756,   757,     0,   399,     0,     0,   760,   761,   762,
       0,     0,   399,   766,     0,     0,     0,    58,     0,     0,
       0,     0,  1175,     0,     0,     0,  1180,     0,     0,     0,
     343,   400,   401,     0,     0,     0,     0,     0,     0,   400,
     401,     0,     0,     0,    59,     0,     0,     0,     0,     0,
     768,     0,   769,   770,   771,   772,   773,   774,   775,   776,
     777,   778,     0,     0,     0,     0,     0,  1215,     0,     0,
       0,   779,   780,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1230,   344,   399,     0,
       0,     0,     0,   402,     0,     0,   399,   403,     0,     0,
       0,   402,     0,     0,     0,   403,     0,     0,  1244,  1245,
    1246,     0,     0,     0,  1250,   400,   401,     0,     0,     0,
       0,     0,  1256,   400,   401,   891,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   685,
       0,     0,     0,  1271,  1272,   304,     0,     0,     0,     0,
     542,   305,     0,     0,  1278,     0,   404,   306,     0,  1064,
     405,     0,  1232,   406,   404,     0,     0,   307,   405,     0,
    1233,   406,  1294,     0,  1296,   308,     0,   402,   407,  1299,
       0,   403,     0,     0,     0,   402,   407,   408,  1302,   403,
     309,     0,  1305,     0,     0,   408,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     404,     0,     0,   688,   405,     0,  1234,   406,   404,     0,
       0,     0,   405,     0,  1236,   406,     0,     0,     0,     0,
       0,     0,   407,     0,     0,     0,     0,    58,     0,     0,
     407,   408,     0,     0,     0,     0,     0,     0,     0,   408,
     343,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     728,   729,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1382,     0,  1383,
       0,     0,     0,     0,  1386,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,     0,     0,
       0,     0,  1394,     0,     0,     0,     0,     0,  1399,  1400,
    1401,     0,     0,   400,   401,  1406,     0,     0,     0,  1409,
       0,     0,     0,  1413,  1414,  1415,  1416,     0,     0,   688,
    1419,     0,  1422,     0,     0,     0,     0,   688,     0,  1434,
       0,     0,     0,   738,   739,   740,   741,   742,  1440,     0,
     745,   746,   747,   748,     0,   750,   751,   752,   753,     0,
       0,     0,     0,   754,     0,   756,   757,     0,     0,     0,
       0,   760,   761,   762,   688,   402,     0,   766,     0,   403,
       0,     0,     0,     0,     0,     0,  1464,  1465,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1476,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   768,  1481,   769,   770,   771,   772,
     773,   774,   775,   776,   777,   778,     0,     0,     0,     0,
    1489,     0,     0,     0,     0,   779,   780,     0,   404,     0,
       0,     0,   405,   284,  1237,   406,     0,     0,     0,     0,
       0,  1505,     0,     0,     0,     0,  1507,     0,     0,     0,
     407,     0,     0,     0,     0,     0,     0,     0,     0,   408,
       0,  1515,   425,  1517,  1518,   426,   427,     3,     0,   428,
     429,   430,  1521,   431,     0,   432,   433,   434,   435,   436,
       0,     0,     0,     0,     0,   437,   438,   439,   440,   441,
       0,   442,     0,     0,     0,     0,     0,     0,   443,   444,
       0,     0,   445,     0,   446,   447,     0,     0,   448,     0,
       8,   449,   450,     0,   451,   452,     0,     0,   453,   454,
       0,     0,     0,     0,     0,   455,     0,     0,   456,   457,
     310,   311,   312,     0,   314,   315,   316,   317,   318,   458,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,     0,   332,   333,   334,     0,     0,   337,   338,   339,
     340,   459,   460,   461,   462,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   463,   464,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   465,   466,   467,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   468,
     469,   470,   471,   472,     0,   473,     0,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,    59,   485,
     486,   487,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   488,   489,   490,     0,    13,     0,     0,   491,
     492,     0,     0,     0,     0,     0,     0,   493,     0,   494,
       0,   495,   496,   497,   498,   499,   425,     0,     0,   426,
     427,     3,     0,   428,   429,   430,     0,   431,     0,   432,
     433,   434,   435,   436,     0,     0,     0,     0,     0,   437,
     438,   439,   440,   441,     0,   442,     0,     0,     0,     0,
       0,     0,   443,   444,     0,     0,   445,     0,   446,   447,
       0,     0,   448,     0,     8,   449,   450,     0,   451,   452,
       0,     0,   453,   454,     0,     0,     0,     0,     0,   455,
       0,     0,   456,   457,   310,   311,   312,     0,   314,   315,
     316,   317,   318,   458,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,     0,   332,   333,   334,   399,
       0,   337,   338,   339,   340,   459,   460,   461,   462,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     463,   464,     0,     0,     0,     0,   400,   401,     0,     0,
       0,     0,     0,     0,     0,   465,   466,   467,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   468,   469,   470,   471,   472,     0,   473,
       0,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,    59,   485,   486,   487,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   402,     0,
       0,     0,   403,     0,     0,     0,   488,   489,   490,     0,
      13,     0,     0,   491,   492,     0,     0,     0,     0,     0,
       0,   493,     0,   494,     0,   495,   496,   497,   498,  1276,
     426,   427,     0,     0,     0,     0,     0,     0,     0,     0,
     432,   433,   434,   435,   436,     0,     0,     0,     0,     0,
     437,     0,   439,     0,     0,     0,   442,     0,   399,     0,
       0,   404,     0,     0,   444,   405,     0,  1239,   406,     0,
     447,     0,     0,   448,     0,     0,   449,     0,   885,     0,
     452,     0,     0,   407,     0,   400,   401,     0,     0,     0,
     559,     0,   408,   456,   457,   310,   311,   312,     0,   314,
     315,   316,   317,   318,   458,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,     0,   332,   333,   334,
       0,     0,   337,   338,   339,   340,   459,   460,   461,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   463,   464,     0,     0,     0,     0,   402,     0,     0,
       0,   403,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   468,   469,   470,   471,   472,     0,
     473,   886,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   887,   560,   486,   487,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     404,     0,     0,     0,   405,     0,     0,   888,   489,   490,
       0,    13,     0,     0,   491,   492,     0,     0,     0,     0,
       0,     0,   889,     0,   890,     0,   495,   496,     0,   498,
       0,   408,   426,   427,     0,     0,     0,     0,     0,     0,
       0,     0,   432,   433,   434,   435,   436,     0,     0,     0,
       0,     0,   437,     0,   439,     0,     0,     0,   442,     0,
     399,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,   447,     0,     0,   448,     0,     0,   449,     0,
       0,     0,   452,     0,     0,     0,     0,   400,   401,     0,
       0,     0,   559,     0,     0,   456,   457,   310,   311,   312,
       0,   314,   315,   316,   317,   318,   458,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,     0,   332,
     333,   334,     0,     0,   337,   338,   339,   340,   459,   460,
     461,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   463,   464,     0,     0,     0,     0,   402,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   468,   469,   470,   471,
     472,     0,   473,   886,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   484,   887,   560,   486,   487,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   404,     0,     0,     0,   405,     0,     0,   888,
     489,   490,     0,    13,     0,     0,   491,   492,     0,   400,
     401,     0,     0,     0,   889,     0,   898,     0,   495,   496,
       0,   498,     0,   408,   426,   427,     0,     0,     0,     0,
       0,     0,     0,     0,   432,   433,   434,   435,   436,     0,
       0,     0,     0,     0,   437,     0,   439,     0,     0,     0,
     442,     0,   592,     0,     0,     0,     0,     0,   444,     0,
       0,     0,     0,     0,   447,     0,     0,   448,     0,     0,
     449,   402,     0,     0,   452,   403,     0,     0,     0,     0,
       0,     0,     0,     0,   559,     0,     0,   456,   457,   310,
     311,   312,     0,   314,   315,   316,   317,   318,   458,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
       0,   332,   333,   334,     0,     0,   337,   338,   339,   340,
     459,   460,   461,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   404,   463,   464,     0,   405,     0,
    1331,   406,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   407,     0,     0,     0,
      58,     0,     0,     0,     0,   408,     0,     0,   468,   469,
     470,   471,   472,     0,   473,     0,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,    59,   560,   486,
     487,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   593,     0,
       0,   488,   489,   490,     0,    13,     0,     0,   491,   492,
       0,   400,   401,     0,     0,     0,  1095,     0,   494,     0,
     495,   496,     0,   498,     0,   595,   426,   427,     0,     0,
       0,     0,     0,     0,     0,     0,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,   402,     0,     0,   452,   403,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   404,   463,   464,     0,
     405,     0,  1359,   406,     0,     0,   636,     0,     0,     0,
       0,     0,   465,   466,   467,     0,     0,     0,   407,     0,
       0,     0,    58,     0,     0,     0,     0,   408,     0,     0,
     468,   469,   470,   471,   472,   399,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,   401,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,     0,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,   402,     0,   447,     0,   403,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,   404,     0,     0,
       0,   405,     0,  1441,   406,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,   666,     0,     0,   407,
       0,     0,   465,   466,   467,     0,     0,     0,   408,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,     0,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,  1427,   439,   440,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,   450,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,  1428,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,     0,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   465,   466,   467,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,     0,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   964,   965,   966,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,     0,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   988,   989,   990,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,     0,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,   886,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,   887,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   929,     0,
     494,   930,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,   886,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,   887,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,  1056,   495,   496,   700,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,   596,
     494,     0,   495,   496,   700,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,   886,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,   887,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,     0,   495,   496,   700,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,   820,
     494,     0,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,   596,
     494,     0,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,   644,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,     0,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,   655,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,     0,   495,   496,   687,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,     0,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   691,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,     0,   495,   496,   700,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,     0,   495,   496,   705,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,     0,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,   993,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,     0,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,  1013,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1059,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,     0,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,  1097,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,  1112,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,  1297,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,  1306,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,  1341,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,  1405,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,  1439,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,     0,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,  1474,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,     0,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,  1475,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,     0,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,   426,   427,   493,     0,
     494,     0,   495,   496,     0,   498,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
     -68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   728,   729,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,    13,     0,     0,
     491,   492,     0,     0,     0,     0,     0,     0,  1283,     0,
     494,     0,   495,   496,     0,   498,   730,   731,   732,   733,
     734,   735,   736,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,   750,   751,   752,   753,
       0,     0,     0,     0,   754,   755,   756,   757,   758,   759,
       0,     0,   760,   761,   762,   763,   764,   765,   766,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   767,   768,     0,   769,   770,   771,
     772,   773,   774,   775,   776,   777,   778,   304,     0,     0,
       0,     0,     0,   305,     0,     0,   779,   780,     0,   306,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   307,
       0,     0,     0,     0,     0,     0,     0,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   309,     0,     0,     0,     0,     0,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   304,     0,     0,     0,
       0,     0,   305,     0,     0,     0,     0,     0,   306,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   307,    58,
       0,     0,     0,     0,     0,     0,   308,     0,     0,     0,
       0,     0,   343,     0,     0,     0,     0,     0,     0,     0,
       0,   309,     0,     0,     0,     0,    59,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   344,
       0,     0,     0,     0,   599,     0,     0,     0,     0,     0,
       0,     0,     0,   304,     0,     0,   728,   729,     0,   305,
       0,     0,     0,     0,     0,   306,     0,     0,    58,     0,
       0,     0,     0,     0,     0,   307,     0,     0,     0,     0,
       0,   343,     0,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   604,     0,     0,   309,     0,
       0,     0,     0,     0,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,     0,   344,     0,
     605,   730,   731,   732,   733,   734,   735,   736,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,   748,
     851,   750,   751,   752,   753,   728,   729,     0,     0,   754,
     755,   756,   757,   758,   759,    58,     0,   760,   761,   762,
     763,   764,   765,   766,     0,     0,     0,     0,   343,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    59,     0,     0,   728,   729,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   852,
     768,     0,   769,   770,   771,   772,   773,   774,   775,   776,
     777,   778,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   779,   780,     0,     0,   344,     0,     0,  1024,     0,
     730,   731,   732,   733,   734,   735,   736,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   851,
     750,   751,   752,   753,     0,     0,     0,     0,   754,   755,
     756,   757,   758,   759,     0,     0,   760,   761,   762,   763,
     764,   765,   766,     0,     0,     0,     0,     0,   738,   739,
     740,   741,   742,     0,     0,   745,   746,   747,   748,     0,
     750,   751,   752,   753,     0,     0,     0,     0,   754,     0,
     756,   757,     0,     0,     0,     0,     0,     0,   852,   768,
       0,   769,   770,   771,   772,   773,   774,   775,   776,   777,
     778,   304,     0,     0,   728,   729,     0,   305,     0,     0,
     779,   780,     0,   306,     0,     0,     0,  1193,     0,     0,
       0,     0,     0,   307,     0,     0,     0,     0,     0,     0,
       0,   308,   770,   771,   772,   773,   774,   775,   776,   777,
     778,     0,     0,     0,     0,     0,   309,     0,     0,     0,
     779,   780,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   738,   739,   740,
     741,   742,     0,     0,   745,   746,   747,   748,     0,   750,
     751,   752,   753,   728,   729,     0,     0,   754,     0,   756,
     757,     0,     0,    58,     0,   760,   761,   762,     0,     0,
       0,   766,     0,     0,     0,     0,   343,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   728,   729,
     604,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   768,     0,
     769,   770,   771,   772,   773,   774,   775,   776,   777,   778,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   779,
     780,     0,     0,   344,     0,     0,  1185,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   738,   739,   740,   741,
     742,     0,     0,   745,   746,   747,   748,     0,   750,   751,
     752,   753,     0,     0,     0,     0,   754,     0,   756,   757,
       0,     0,     0,     0,   760,   761,   762,     0,     0,     0,
     766,   738,   739,   740,   741,   742,     0,     0,   745,   746,
     747,   748,     0,   750,   751,   752,   753,   728,   729,     0,
       0,   754,     0,   756,   757,     0,     0,     0,     0,   760,
     761,   762,     0,     0,     0,   766,     0,   768,     0,   769,
     770,   771,   772,   773,   774,   775,   776,   777,   778,     0,
       0,     0,   728,   729,     0,     0,     0,     0,   779,   780,
       0,     0,     0,     0,     0,  1462,     0,     0,     0,     0,
       0,     0,   768,     0,   769,   770,   771,   772,   773,   774,
     775,   776,   777,   778,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   779,   780,     0,     0,     0,     0,     0,
    1522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     738,   739,   740,   741,   742,     0,     0,   745,   746,   747,
     748,     0,   750,   751,   752,   753,     0,     0,     0,     0,
     754,     0,   756,   757,     0,     0,     0,     0,   760,   761,
     762,     0,     0,     0,   766,   738,   739,   740,   741,   742,
     728,   729,   745,   746,   747,   748,     0,   750,   751,   752,
     753,     0,     0,     0,     0,   754,     0,   756,   757,     0,
       0,     0,     0,   760,   761,   762,     0,     0,     0,   766,
       0,   768,     0,   769,   770,   771,   772,   773,   774,   775,
     776,   777,   778,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   779,   780,     0,     0,     0,     0,     0,  1524,
       0,     0,     0,     0,     0,     0,   768,     0,   769,   770,
     771,   772,   773,   774,   775,   776,   777,   778,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   779,   780,     0,
       0,   790,     0,   738,   739,   740,   741,   742,   728,   729,
     745,   746,   747,   748,     0,   750,   751,   752,   753,     0,
       0,     0,     0,   754,     0,   756,   757,     0,     0,     0,
       0,   760,   761,   762,     0,     0,     0,   766,     0,     0,
       0,     0,     0,   728,   729,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   768,     0,   769,   770,   771,   772,
     773,   774,   775,   776,   777,   778,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   779,   780,     0,     0,   800,
       0,   738,   739,   740,   741,   742,     0,     0,   745,   746,
     747,   748,     0,   750,   751,   752,   753,     0,     0,     0,
       0,   754,     0,   756,   757,     0,     0,     0,     0,   760,
     761,   762,     0,     0,     0,   766,   738,   739,   740,   741,
     742,   728,   729,   745,   746,   747,   748,     0,   750,   751,
     752,   753,     0,     0,     0,     0,   754,     0,   756,   757,
       0,     0,     0,     0,   760,   761,   762,     0,     0,     0,
     766,     0,   768,     0,   769,   770,   771,   772,   773,   774,
     775,   776,   777,   778,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   779,   780,     0,     0,   816,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   768,     0,   769,
     770,   771,   772,   773,   774,   775,   776,   777,   778,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   779,   780,
       0,     0,   831,     0,   738,   739,   740,   741,   742,   728,
     729,   745,   746,   747,   748,     0,   750,   751,   752,   753,
       0,     0,     0,     0,   754,     0,   756,   757,     0,     0,
       0,     0,   760,   761,   762,     0,     0,     0,   766,     0,
       0,     0,     0,     0,   728,   729,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   768,     0,   769,   770,   771,
     772,   773,   774,   775,   776,   777,   778,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   779,   780,     0,     0,
    1049,     0,   738,   739,   740,   741,   742,     0,     0,   745,
     746,   747,   748,     0,   750,   751,   752,   753,     0,     0,
       0,     0,   754,     0,   756,   757,     0,     0,     0,     0,
     760,   761,   762,     0,     0,     0,   766,   738,   739,   740,
     741,   742,   728,   729,   745,   746,   747,   748,     0,   750,
     751,   752,   753,     0,     0,     0,     0,   754,     0,   756,
     757,     0,     0,     0,     0,   760,   761,   762,     0,     0,
       0,   766,     0,   768,     0,   769,   770,   771,   772,   773,
     774,   775,   776,   777,   778,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   779,   780,     0,     0,  1069,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   768,     0,
     769,   770,   771,   772,   773,   774,   775,   776,   777,   778,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   779,
     780,     0,     0,  1073,     0,   738,   739,   740,   741,   742,
     728,   729,   745,   746,   747,   748,     0,   750,   751,   752,
     753,     0,     0,     0,     0,   754,     0,   756,   757,     0,
       0,     0,     0,   760,   761,   762,     0,     0,     0,   766,
       0,     0,     0,     0,     0,   728,   729,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   768,     0,   769,   770,
     771,   772,   773,   774,   775,   776,   777,   778,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   779,   780,     0,
       0,  1075,     0,   738,   739,   740,   741,   742,     0,     0,
     745,   746,   747,   748,     0,   750,   751,   752,   753,     0,
       0,     0,     0,   754,     0,   756,   757,     0,     0,     0,
       0,   760,   761,   762,     0,     0,     0,   766,   738,   739,
     740,   741,   742,   728,   729,   745,   746,   747,   748,     0,
     750,   751,   752,   753,     0,     0,     0,     0,   754,     0,
     756,   757,     0,     0,     0,     0,   760,   761,   762,     0,
       0,     0,   766,     0,   768,     0,   769,   770,   771,   772,
     773,   774,   775,   776,   777,   778,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   779,   780,     0,     0,  1084,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   768,
       0,   769,   770,   771,   772,   773,   774,   775,   776,   777,
     778,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     779,   780,     0,     0,  1085,     0,   738,   739,   740,   741,
     742,   728,   729,   745,   746,   747,   748,     0,   750,   751,
     752,   753,     0,     0,     0,     0,   754,     0,   756,   757,
       0,     0,     0,     0,   760,   761,   762,     0,     0,     0,
     766,     0,     0,     0,     0,     0,   728,   729,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   768,     0,   769,
     770,   771,   772,   773,   774,   775,   776,   777,   778,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   779,   780,
       0,     0,  1086,     0,   738,   739,   740,   741,   742,     0,
       0,   745,   746,   747,   748,     0,   750,   751,   752,   753,
       0,     0,     0,     0,   754,     0,   756,   757,     0,     0,
       0,     0,   760,   761,   762,     0,     0,     0,   766,   738,
     739,   740,   741,   742,   728,   729,   745,   746,   747,   748,
       0,   750,   751,   752,   753,     0,     0,     0,     0,   754,
       0,   756,   757,     0,     0,     0,     0,   760,   761,   762,
       0,     0,     0,   766,     0,   768,     0,   769,   770,   771,
     772,   773,   774,   775,   776,   777,   778,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   779,   780,     0,     0,
    1087,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     768,     0,   769,   770,   771,   772,   773,   774,   775,   776,
     777,   778,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   779,   780,     0,     0,  1088,     0,   738,   739,   740,
     741,   742,   728,   729,   745,   746,   747,   748,     0,   750,
     751,   752,   753,     0,     0,     0,     0,   754,     0,   756,
     757,     0,     0,     0,     0,   760,   761,   762,     0,     0,
       0,   766,     0,     0,     0,     0,     0,   728,   729,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   768,     0,
     769,   770,   771,   772,   773,   774,   775,   776,   777,   778,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   779,
     780,     0,     0,  1089,     0,   738,   739,   740,   741,   742,
       0,     0,   745,   746,   747,   748,     0,   750,   751,   752,
     753,     0,     0,     0,     0,   754,     0,   756,   757,     0,
       0,     0,     0,   760,   761,   762,     0,     0,     0,   766,
     738,   739,   740,   741,   742,   728,   729,   745,   746,   747,
     748,     0,   750,   751,   752,   753,     0,     0,     0,     0,
     754,     0,   756,   757,     0,     0,     0,     0,   760,   761,
     762,     0,     0,     0,   766,     0,   768,     0,   769,   770,
     771,   772,   773,   774,   775,   776,   777,   778,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   779,   780,     0,
       0,  1223,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   768,     0,   769,   770,   771,   772,   773,   774,   775,
     776,   777,   778,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   779,   780,     0,     0,  1344,     0,   738,   739,
     740,   741,   742,   728,   729,   745,   746,   747,   748,     0,
     750,   751,   752,   753,     0,     0,     0,     0,   754,     0,
     756,   757,     0,     0,     0,     0,   760,   761,   762,     0,
       0,     0,   766,     0,     0,     0,     0,     0,   728,   729,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   768,
       0,   769,   770,   771,   772,   773,   774,   775,   776,   777,
     778,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     779,   780,     0,     0,  1357,     0,   738,   739,   740,   741,
     742,     0,     0,   745,   746,   747,   748,     0,   750,   751,
     752,   753,     0,     0,     0,     0,   754,     0,   756,   757,
       0,     0,     0,     0,   760,   761,   762,     0,     0,     0,
     766,   738,   739,   740,   741,   742,   728,   729,   745,   746,
     747,   748,     0,   750,   751,   752,   753,     0,     0,     0,
       0,   754,     0,   756,   757,     0,     0,     0,     0,   760,
     761,   762,     0,     0,     0,   766,     0,   768,     0,   769,
     770,   771,   772,   773,   774,   775,   776,   777,   778,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   779,   780,
       0,     0,  1358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   768,     0,   769,   770,   771,   772,   773,   774,
     775,   776,   777,   778,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   779,   780,     0,     0,  1376,     0,   738,
     739,   740,   741,   742,   728,   729,   745,   746,   747,   748,
       0,   750,   751,   752,   753,     0,     0,     0,     0,   754,
       0,   756,   757,     0,     0,     0,     0,   760,   761,   762,
       0,     0,     0,   766,     0,     0,     0,     0,     0,   728,
     729,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     768,     0,   769,   770,   771,   772,   773,   774,   775,   776,
     777,   778,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   779,   780,     0,     0,  1378,     0,   738,   739,   740,
     741,   742,     0,     0,   745,   746,   747,   748,     0,   750,
     751,   752,   753,     0,     0,     0,     0,   754,     0,   756,
     757,     0,     0,     0,     0,   760,   761,   762,     0,     0,
       0,   766,   738,   739,   740,   741,   742,   728,   729,   745,
     746,   747,   748,     0,   750,   751,   752,   753,     0,     0,
       0,     0,   754,     0,   756,   757,     0,     0,     0,     0,
     760,   761,   762,     0,     0,     0,   766,     0,   768,     0,
     769,   770,   771,   772,   773,   774,   775,   776,   777,   778,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   779,
     780,     0,     0,  1380,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   768,     0,   769,   770,   771,   772,   773,
     774,   775,   776,   777,   778,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   779,   780,     0,     0,  1384,     0,
     738,   739,   740,   741,   742,   728,   729,   745,   746,   747,
     748,     0,   750,   751,   752,   753,     0,     0,     0,     0,
     754,     0,   756,   757,     0,     0,     0,     0,   760,   761,
     762,     0,     0,     0,   766,     0,     0,     0,     0,     0,
     728,   729,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   768,     0,   769,   770,   771,   772,   773,   774,   775,
     776,   777,   778,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   779,   780,     0,     0,  1444,     0,   738,   739,
     740,   741,   742,     0,     0,   745,   746,   747,   748,     0,
     750,   751,   752,   753,     0,     0,     0,     0,   754,     0,
     756,   757,     0,     0,     0,     0,   760,   761,   762,     0,
       0,     0,   766,   738,   739,   740,   741,   742,   728,   729,
     745,   746,   747,   748,     0,   750,   751,   752,   753,     0,
       0,     0,     0,   754,     0,   756,   757,     0,     0,     0,
       0,   760,   761,   762,     0,     0,     0,   766,     0,   768,
       0,   769,   770,   771,   772,   773,   774,   775,   776,   777,
     778,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     779,   780,     0,     0,  1445,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   768,     0,   769,   770,   771,   772,
     773,   774,   775,   776,   777,   778,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   779,   780,     0,     0,  1446,
       0,   738,   739,   740,   741,   742,   728,   729,   745,   746,
     747,   748,     0,   750,   751,   752,   753,     0,     0,     0,
       0,   754,     0,   756,   757,     0,     0,     0,     0,   760,
     761,   762,     0,     0,     0,   766,     0,     0,     0,     0,
       0,   728,   729,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   768,     0,   769,   770,   771,   772,   773,   774,
     775,   776,   777,   778,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   779,   780,     0,     0,  1453,     0,   738,
     739,   740,   741,   742,     0,     0,   745,   746,   747,   748,
       0,   750,   751,   752,   753,     0,     0,     0,     0,   754,
       0,   756,   757,     0,     0,     0,     0,   760,   761,   762,
       0,     0,     0,   766,   738,   739,   740,   741,   742,   728,
     729,   745,   746,   747,   748,     0,   750,   751,   752,   753,
       0,     0,     0,     0,   754,     0,   756,   757,     0,     0,
       0,     0,   760,   761,   762,     0,     0,     0,   766,     0,
     768,     0,   769,   770,   771,   772,   773,   774,   775,   776,
     777,   778,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   779,   780,     0,     0,  1461,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   768,     0,   769,   770,   771,
     772,   773,   774,   775,   776,   777,   778,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   779,   780,     0,     0,
    1466,     0,   738,   739,   740,   741,   742,   728,   729,   745,
     746,   747,   748,     0,   750,   751,   752,   753,     0,     0,
       0,     0,   754,     0,   756,   757,     0,     0,     0,     0,
     760,   761,   762,     0,     0,     0,   766,     0,     0,     0,
       0,     0,   728,   729,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   768,     0,   769,   770,   771,   772,   773,
     774,   775,   776,   777,   778,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   779,   780,     0,     0,  1511,     0,
     738,   739,   740,   741,   742,     0,     0,   745,   746,   747,
     748,     0,   750,   751,   752,   753,     0,     0,     0,     0,
     754,     0,   756,   757,     0,     0,     0,     0,   760,   761,
     762,     0,     0,     0,   766,   738,   739,   740,   741,   742,
     728,   729,   745,   746,   747,   748,     0,   750,   751,   752,
     753,     0,     0,     0,     0,   754,     0,   756,   757,     0,
       0,     0,     0,   760,   761,   762,     0,     0,     0,   766,
       0,   768,     0,   769,   770,   771,   772,   773,   774,   775,
     776,   777,   778,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   779,   780,   821,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   768,     0,   769,   770,
     771,   772,   773,   774,   775,   776,   777,   778,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   779,   780,  1046,
       0,     0,     0,   738,   739,   740,   741,   742,   728,   729,
     745,   746,   747,   748,     0,   750,   751,   752,   753,     0,
       0,     0,     0,   754,     0,   756,   757,     0,     0,     0,
       0,   760,   761,   762,     0,     0,     0,   766,     0,     0,
       0,     0,     0,   728,   729,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   768,     0,   769,   770,   771,   772,
     773,   774,   775,   776,   777,   778,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   779,   780,  1164,     0,     0,
       0,   738,   739,   740,   741,   742,     0,     0,   745,   746,
     747,   748,     0,   750,   751,   752,   753,     0,     0,     0,
       0,   754,     0,   756,   757,     0,     0,     0,     0,   760,
     761,   762,     0,     0,     0,   766,   738,   739,   740,   741,
     742,   728,   729,   745,   746,   747,   748,     0,   750,   751,
     752,   753,     0,     0,     0,     0,   754,     0,   756,   757,
       0,     0,     0,     0,   760,   761,   762,     0,     0,     0,
     766,     0,   768,     0,   769,   770,   771,   772,   773,   774,
     775,   776,   777,   778,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   779,   780,  1182,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   768,     0,   769,
     770,   771,   772,   773,   774,   775,   776,   777,   778,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   779,   780,
    1304,     0,     0,     0,   738,   739,   740,   741,   742,     0,
       0,   745,   746,   747,   748,     0,   750,   751,   752,   753,
     728,   729,     0,     0,   754,     0,   756,   757,     0,     0,
       0,     0,   760,   761,   762,     0,     0,     0,   766,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   768,     0,   769,   770,   771,
     772,   773,   774,   775,   776,   777,   778,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   779,   780,  1309,     0,
       0,     0,     0,     0,     0,   730,   731,   732,   733,   734,
     735,   736,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   851,   750,   751,   752,   753,  -344,
       0,   229,   230,   754,   755,   756,   757,   758,   759,     0,
       0,   760,   761,   762,   763,   764,   765,   766,   231,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   728,   729,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   852,   768,     0,   769,   770,   771,   772,
     773,   774,   775,   776,   777,   778,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   779,   780,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,     0,     0,   250,   251,   252,     0,
       0,     0,     0,     0,     0,   253,   254,   255,   256,   257,
       0,     0,   258,   259,   260,   261,   262,   263,   264,   738,
     739,   740,   741,   742,   728,   729,   745,   746,   747,   748,
       0,   750,   751,   752,   753,     0,     0,     0,     0,   754,
       0,   756,   757,     0,     0,   902,     0,   760,   761,   762,
       0,     0,     0,   766,     0,   265,     0,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,     0,     0,   276,
     277,     0,     0,     0,     0,     0,   278,   279,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     768,     0,   769,   770,   771,   772,   773,   774,   775,   776,
     777,   778,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   779,   780,     0,     0,     0,     0,   738,   739,   740,
     741,   742,   728,   729,   745,   746,   747,   748,     0,   750,
     751,   752,   753,     0,     0,     0,     0,   754,     0,   756,
     757,     0,     0,  1098,     0,   760,   761,   762,     0,     0,
       0,   766,     0,     0,     0,     0,     0,   728,   729,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   768,     0,
     769,   770,   771,   772,   773,   774,   775,   776,   777,   778,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   779,
     780,     0,     0,     0,     0,   738,   739,   740,   741,   742,
       0,     0,   745,   746,   747,   748,     0,   750,   751,   752,
     753,     0,     0,     0,     0,   754,     0,   756,   757,     0,
       0,     0,     0,   760,   761,   762,     0,     0,     0,   766,
     738,   739,   740,   741,   742,   728,   729,   745,   746,   747,
     748,     0,   750,   751,   752,   753,     0,     0,     0,     0,
     754,     0,   756,   757,     0,     0,     0,     0,   760,   761,
     762,     0,     0,     0,   766,     0,   768,  1172,   769,   770,
     771,   772,   773,   774,   775,   776,   777,   778,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   779,   780,     0,
       0,     0,  1269,     0,     0,     0,     0,     0,     0,     0,
       0,   768,     0,   769,   770,   771,   772,   773,   774,   775,
     776,   777,   778,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   779,   780,     0,     0,     0,     0,   738,   739,
     740,   741,   742,   728,   729,   745,   746,   747,   748,     0,
     750,   751,   752,   753,     0,     0,     0,     0,   754,     0,
     756,   757,     0,     0,     0,     0,   760,   761,   762,     0,
       0,     0,   766,     0,     0,     0,     0,     0,   728,   729,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   768,
       0,   769,   770,   771,   772,   773,   774,   775,   776,   777,
     778,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     779,   780,     0,     0,     0,     0,   738,   739,   740,   741,
     742,     0,     0,   745,   746,   747,   748,     0,   750,   751,
     752,   753,     0,     0,     0,     0,   754,     0,   756,   757,
       0,     0,     0,     0,   760,   761,   762,     0,     0,     0,
    -719,   738,   739,   740,   741,   742,   728,   729,   745,   746,
     747,   748,     0,   750,   751,   752,   753,     0,     0,     0,
       0,   754,     0,   756,   757,     0,     0,     0,     0,   760,
     761,   762,     0,     0,     0,     0,     0,   768,     0,   769,
     770,   771,   772,   773,   774,   775,   776,   777,   778,   728,
     729,     0,     0,     0,     0,     0,     0,     0,   779,   780,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   768,     0,   769,   770,   771,   772,   773,   774,
     775,   776,   777,   778,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   779,   780,     0,     0,     0,     0,   738,
     739,   740,   741,   742,     0,     0,   745,   746,   747,   748,
       0,   750,   751,   752,   753,     0,     0,     0,     0,   754,
       0,   756,   757,     0,     0,     0,     0,   760,     0,   762,
       0,     0,     0,     0,     0,     0,   728,   729,     0,     0,
       0,     0,   738,   739,   740,   741,   742,     0,     0,   745,
     746,   747,   748,     0,   750,   751,   752,   753,     0,     0,
     728,   729,   754,     0,   756,   757,     0,     0,     0,     0,
     760,     0,   769,   770,   771,   772,   773,   774,   775,   776,
     777,   778,     0,     0,   728,   729,     0,     0,     0,     0,
       0,   779,   780,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   769,   770,   771,   772,   773,
     774,   775,   776,   777,   778,     0,     0,     0,     0,   738,
     739,   740,   741,   742,   779,   780,   745,   746,   747,   748,
       0,   750,   751,   752,   753,     0,     0,     0,     0,   754,
       0,   756,   757,   738,   739,   740,   741,   742,     0,     0,
     745,   746,   747,   748,     0,   750,   751,   752,   753,     0,
       0,     0,     0,   754,     0,   756,   757,   738,   739,   740,
     741,   742,     0,     0,   745,   746,   747,   748,     0,   750,
     751,   752,   753,     0,     0,     0,     0,   754,     0,   756,
     757,     0,   769,   770,   771,   772,   773,   774,   775,   776,
     777,   778,     0,     0,     0,     0,     0,     0,     0,     0,
     939,   779,   780,     0,     0,     0,     0,     0,   771,   772,
     773,   774,   775,   776,   777,   778,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   779,   780,     0,     0,     0,
       0,   943,     0,   772,   773,   774,   775,   776,   777,   778,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   779,
     780,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   310,   311,   312,     0,   314,   315,   316,   317,
     318,   458,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,     0,   332,   333,   334,     0,     0,   337,
     338,   339,   340,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1168,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   940,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   941,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   944,     0,
       0,     0,     0,   181,     0,     0,     0,   310,   311,   312,
     945,   314,   315,   316,   317,   318,   458,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,     0,   332,
     333,   334,     0,     0,   337,   338,   339,   340,   182,     0,
     183,     0,   184,   185,   186,   187,   188,     0,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,     0,
     200,   201,   202,     0,     0,   203,   204,   205,   206,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   207,   208,     0,     0,     0,     0,
       0,     0,     0,  1169,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1170,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   209
};

static const yytype_int16 yycheck[] =
{
      13,    14,   211,   417,   477,   566,   369,   381,   703,   643,
     421,   161,   707,   466,   467,   686,   369,   807,   586,   809,
     585,   811,   465,   466,   467,   893,   376,   929,   225,   877,
     580,   899,   582,   532,   584,   369,   410,  1101,    19,   438,
       7,    21,    19,    19,    33,   716,    19,    52,     0,   103,
      63,    64,    65,    52,     6,  1320,   129,   556,   139,    19,
      45,    19,   225,   462,    32,    14,    15,   167,   124,    56,
      32,  1423,   515,    62,   124,    62,    56,    29,   124,    31,
     167,    33,    14,    15,   134,   438,   160,    39,   134,   167,
     167,   104,   105,   106,   107,     4,     5,    49,   541,   182,
     152,   475,   476,    55,   438,   157,   124,    18,    19,   462,
     167,  1447,   212,   438,   123,    24,   134,   126,   127,   104,
    1472,    30,   124,   126,   127,   212,    78,   183,   462,   212,
     186,   208,   213,   183,   212,    49,   124,   183,   188,   212,
     141,   142,   143,   140,   133,    49,   134,   297,   100,   101,
    1486,   140,   847,   210,   179,  1420,   155,   209,    67,    68,
     855,   749,    32,   178,   218,   183,   181,   186,   167,   494,
     367,   570,   843,   868,   673,   182,   871,    91,   167,   767,
     182,   183,   207,   185,   381,  1249,   188,   212,   185,    59,
      60,   100,   101,   212,    61,   183,   205,   206,  1100,   214,
     207,   182,   205,   206,   367,   182,   182,   212,   209,   182,
     199,   160,   225,   410,   194,   412,   165,   570,   167,   208,
     188,   170,   182,   622,   182,   205,   188,   214,   160,  1097,
     124,   140,   441,   165,  1082,   167,   570,   636,   170,   206,
     134,   183,   124,   195,   206,   149,   455,   214,   157,   158,
     159,   121,   134,   215,   206,   125,   167,   182,   167,   208,
     212,   182,   185,   167,   589,   182,   627,   666,   642,   622,
     212,   177,   725,   141,   182,   143,   126,   127,   475,   476,
     189,  1345,   725,   636,   124,   213,   182,   212,   622,   183,
     213,   665,   213,  1141,   134,   212,   140,   206,   140,   207,
     140,   183,   636,   182,   213,   155,   212,   149,   214,   212,
     635,   636,   154,   666,   184,   178,   212,   167,   188,   149,
     190,   191,   683,   167,   685,   167,   687,   182,    63,    64,
      65,   344,   666,   212,   182,   660,   206,   167,   663,   700,
      46,    20,    21,   183,   705,   215,   183,   189,   149,   212,
     182,   214,   207,   182,   367,   205,   206,   179,   180,   372,
      66,   209,     4,     5,  1154,     7,   167,   190,   381,   104,
     105,   106,   107,   723,   571,   572,   573,   209,   575,   576,
    1248,  1283,   579,   190,   581,   207,   583,   937,   585,   212,
     185,   716,  1260,    35,   793,   140,   749,   410,   182,   412,
     167,   168,   169,   182,   149,   212,   152,   182,   571,   572,
     573,   157,   575,   576,   767,   749,   579,   626,   581,   182,
     583,   434,   167,   620,   835,   209,  1097,   167,   195,   928,
     209,   904,   885,   767,   209,   182,   115,   116,   647,   182,
     793,  1112,   885,   182,   123,   642,   125,   126,   127,   128,
     923,  1241,   182,   182,   133,   182,   781,   620,   140,   793,
     207,   155,   475,   476,   838,   839,   209,   208,   665,   182,
     209,   189,   846,   167,  1042,  1040,   160,   185,   207,   209,
     207,    73,   190,   692,  1274,    77,   860,   861,   190,   863,
     208,    20,    21,   867,   207,   869,   870,    11,   872,   186,
     208,   205,   711,   190,    96,    97,    98,    99,    22,    23,
     212,   964,   965,   966,   193,   194,   195,   190,   843,    20,
      21,   964,   965,   966,   167,   185,   205,   206,    32,   185,
     190,   728,   729,  1083,   190,   988,   989,   990,   185,   212,
     186,   155,   190,   190,   190,   988,   989,   990,   208,   190,
     189,   167,   208,   167,   190,    59,    60,   754,   571,   572,
     573,   208,   575,   576,   212,   890,   579,   155,   581,   208,
     583,   212,   585,   898,   212,  1365,   212,   149,    32,   167,
    1130,   189,   189,   189,    56,   157,   115,   116,   186,   212,
      62,   155,   190,  1227,   123,   167,   125,   126,   127,   128,
     208,   208,   208,   167,   133,    59,    60,   620,   208,   167,
      56,   155,    56,   213,   115,   116,    62,   121,    62,   632,
     817,   125,   123,   167,    56,   126,   127,   128,   825,   642,
      62,   828,   133,   186,  1416,  1330,  1418,   190,   171,   789,
    1422,   838,   839,  1425,   853,   842,   167,   182,   845,   846,
     185,    56,   665,   188,   817,   186,   186,    62,   167,   190,
     190,    32,   825,   860,   861,   828,   863,   121,    56,   167,
     867,   125,   869,   870,    62,   872,   205,   206,   212,   842,
     184,   186,   845,    56,   188,   190,    32,   191,    59,    60,
      56,   704,   167,  1067,   186,   186,    62,   186,   190,   190,
     213,   190,   206,   186,   205,   206,   209,   190,   171,   212,
     907,   215,   909,    59,    60,   171,   172,   173,   174,   171,
     172,  1392,  1393,   210,   211,   209,   213,   141,   212,  1054,
     184,  1281,  1403,  1404,   188,   209,   190,   191,   212,   209,
     185,   104,   212,   188,   907,   209,   909,   907,   212,   909,
     121,   157,   206,   183,   125,  1129,   209,    34,   212,   212,
    1455,   215,  1457,  1458,  1398,  1090,   209,   171,   172,   212,
      34,  1134,   213,   786,   213,   121,   210,   211,   124,   125,
     213,  1134,   141,   142,   143,   208,  1111,   213,   134,   167,
     168,   169,  1117,    10,    11,    12,   993,    86,    87,  1124,
    1134,   171,   172,   173,   817,   167,  1131,   214,   207,   167,
     167,   167,   825,   184,    21,   828,   167,   188,   207,   190,
     191,   185,   213,   213,   185,   838,   839,   208,    42,   842,
     167,  1028,   845,   846,  1159,   206,   189,   183,   184,   189,
     189,  1252,   188,  1040,   215,   191,    32,   860,   861,   189,
     863,   189,  1177,   208,   867,   189,   869,   870,   189,   872,
     206,   208,    20,    21,   189,  1028,   212,   189,   189,   215,
    1067,   189,   208,    59,    60,   186,   134,  1438,   208,   207,
    1353,   167,   167,   167,   208,    42,   206,   189,  1361,   189,
     189,   208,   208,   167,   907,   189,   909,   189,   208,   212,
     160,   189,   189,   208,   208,   208,   189,   189,   189,    36,
     208,   208,   208,     9,    65,   208,   211,  1242,  1479,  1293,
     189,   212,   212,    42,   167,   167,  1135,   206,   208,   185,
     167,   167,  1129,  1344,   212,   121,   212,   212,   124,   125,
     212,   212,   209,   182,   214,   369,   207,   212,   134,   129,
     212,   214,    32,   212,   378,   113,   114,   115,   116,   117,
     214,   214,   120,   387,   208,   123,    13,   125,   126,   127,
     128,   183,   185,   397,   182,   133,  1390,   135,   136,    59,
      60,   160,   211,   407,   182,   213,    20,    21,   157,   167,
       7,   167,   416,   190,   209,  1320,    42,   183,   184,   208,
     182,   182,   188,   214,   428,   191,   207,   209,   208,  1218,
     214,   167,   208,   208,   438,  1028,  1213,   189,   189,   443,
     206,   445,     1,   208,   208,   208,   214,  1040,    66,   215,
     454,   189,   190,   191,   192,   193,   194,   195,   462,   463,
     464,   121,   208,   208,   212,   125,   167,   205,   206,   190,
    1213,   190,   190,   477,  1067,  1466,   167,   167,   207,   209,
     208,   485,   209,  1437,   488,   489,   490,   491,   492,  1278,
     167,  1396,   209,   209,   498,   208,   207,  1402,   207,   167,
     504,   115,   116,  1292,   167,   167,   189,    32,   208,   123,
     208,   125,   126,   127,   128,  1420,  1293,   212,  1295,   133,
      20,    21,  1427,   208,   184,   212,   208,   208,   188,   208,
     190,   191,   207,   167,    59,    60,  1129,   209,   207,    32,
     207,   190,   208,   208,   167,   209,   206,   208,   190,   208,
     208,  1340,  1295,   208,   208,   215,   208,    52,   209,   212,
     207,   209,   207,   694,   214,   209,   570,    79,   214,  1474,
     209,   214,   212,   209,   209,   207,   207,   191,   192,   193,
     194,   195,     1,    44,   214,   133,    82,  1362,    80,  1363,
     223,   205,   206,   786,   598,   925,   121,  1363,  1363,  1363,
     125,  1363,     1,  1340,  1381,  1290,  1313,   541,  1389,  1189,
    1316,   615,   423,   113,   114,   115,   116,  1369,   622,    52,
    1213,   434,   434,   123,  1351,   125,   126,   127,   128,   551,
    1109,    -1,   636,   133,    -1,   135,   136,   686,  1381,   643,
     644,    -1,   646,    -1,    -1,    -1,  1435,   651,    -1,    -1,
      -1,    -1,    -1,   657,    -1,    -1,    -1,    -1,    -1,   184,
    1437,    -1,   666,   188,    -1,   190,   191,    -1,    -1,    -1,
      -1,    -1,    -1,   677,   678,   679,   680,   681,   682,    -1,
     684,   206,   686,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     215,   191,   192,   193,   194,   195,    -1,    32,    -1,    -1,
    1293,    -1,  1295,    -1,    -1,   205,   206,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1510,    -1,    -1,    59,    60,   730,   731,   732,   733,
     734,   735,   736,   737,   738,   739,    -1,    -1,   742,   743,
     744,   745,   746,   747,    -1,   749,   750,    -1,   752,   753,
     754,   755,   756,   757,   758,   759,   760,   761,   762,   763,
     764,   765,   766,   767,   768,   769,   770,   771,   772,   773,
     774,   775,   776,   777,   778,    -1,   780,    -1,    -1,   783,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,  1381,   793,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1423,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     844,    -1,    -1,    -1,  1437,    -1,   850,   851,   852,   184,
      -1,    -1,    -1,   188,    -1,   190,   191,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    12,
      -1,   206,    -1,    -1,    -1,    18,    -1,    -1,    -1,  1472,
     215,    24,    -1,    -1,   888,   889,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,   902,    -1,
     904,    -1,    -1,    -1,    -1,    48,    -1,   911,    -1,    -1,
     914,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   923,
      63,    -1,    -1,   927,    -1,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   113,
     114,   115,   116,   117,    -1,    -1,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    32,    -1,    -1,   141,   142,   143,
      -1,    -1,    32,   147,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,  1006,    -1,    -1,    -1,  1010,    -1,    -1,    -1,
     153,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,    59,
      60,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,
     184,    -1,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,    -1,    -1,    -1,    -1,    -1,  1051,    -1,    -1,
      -1,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1070,   210,    32,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    32,   125,    -1,    -1,
      -1,   121,    -1,    -1,    -1,   125,    -1,    -1,  1092,  1093,
    1094,    -1,    -1,    -1,  1098,    59,    60,    -1,    -1,    -1,
      -1,    -1,  1106,    59,    60,  1109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,  1127,  1128,    18,    -1,    -1,    -1,    -1,
    1134,    24,    -1,    -1,  1138,    -1,   184,    30,    -1,  1143,
     188,    -1,   190,   191,   184,    -1,    -1,    40,   188,    -1,
     190,   191,  1156,    -1,  1158,    48,    -1,   121,   206,  1163,
      -1,   125,    -1,    -1,    -1,   121,   206,   215,  1172,   125,
      63,    -1,  1176,    -1,    -1,   215,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     184,    -1,    -1,  1227,   188,    -1,   190,   191,   184,    -1,
      -1,    -1,   188,    -1,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,   206,    -1,    -1,    -1,    -1,   140,    -1,    -1,
     206,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   215,
     153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1301,    -1,  1303,
      -1,    -1,    -1,    -1,  1308,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,    -1,    -1,
      -1,    -1,  1326,    -1,    -1,    -1,    -1,    -1,  1332,  1333,
    1334,    -1,    -1,    59,    60,  1339,    -1,    -1,    -1,  1343,
      -1,    -1,    -1,  1347,  1348,  1349,  1350,    -1,    -1,  1353,
    1354,    -1,  1356,    -1,    -1,    -1,    -1,  1361,    -1,  1363,
      -1,    -1,    -1,   113,   114,   115,   116,   117,  1372,    -1,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,
      -1,   141,   142,   143,  1398,   121,    -1,   147,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,  1410,  1411,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1428,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   184,  1439,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,
    1454,    -1,    -1,    -1,    -1,   205,   206,    -1,   184,    -1,
      -1,    -1,   188,   213,   190,   191,    -1,    -1,    -1,    -1,
      -1,  1475,    -1,    -1,    -1,    -1,  1480,    -1,    -1,    -1,
     206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   215,
      -1,  1495,     1,  1497,  1498,     4,     5,     6,    -1,     8,
       9,    10,  1506,    12,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    25,    26,    27,    28,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,
      -1,    -1,    41,    -1,    43,    44,    -1,    -1,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    -1,    57,    58,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    93,    -1,    -1,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
     149,   150,   151,   152,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,   198,
     199,    -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,   208,
      -1,   210,   211,   212,   213,   214,     1,    -1,    -1,     4,
       5,     6,    -1,     8,     9,    10,    -1,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      25,    26,    27,    28,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    38,    -1,    -1,    41,    -1,    43,    44,
      -1,    -1,    47,    -1,    49,    50,    51,    -1,    53,    54,
      -1,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    32,
      -1,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,    -1,    -1,    -1,    -1,    59,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,   149,   150,   151,   152,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,   125,    -1,    -1,    -1,   191,   192,   193,    -1,
     195,    -1,    -1,   198,   199,    -1,    -1,    -1,    -1,    -1,
      -1,   206,    -1,   208,    -1,   210,   211,   212,   213,   214,
       4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    26,    -1,    -1,    -1,    30,    -1,    32,    -1,
      -1,   184,    -1,    -1,    38,   188,    -1,   190,   191,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    52,    -1,
      54,    -1,    -1,   206,    -1,    59,    60,    -1,    -1,    -1,
      64,    -1,   215,    67,    68,    69,    70,    71,    -1,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    -1,    96,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,   116,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     184,    -1,    -1,    -1,   188,    -1,    -1,   191,   192,   193,
      -1,   195,    -1,    -1,   198,   199,    -1,    -1,    -1,    -1,
      -1,    -1,   206,    -1,   208,    -1,   210,   211,    -1,   213,
      -1,   215,     4,     5,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    59,    60,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,    71,
      -1,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    93,    -1,    -1,    96,    97,    98,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,   116,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   184,    -1,    -1,    -1,   188,    -1,    -1,   191,
     192,   193,    -1,   195,    -1,    -1,   198,   199,    -1,    59,
      60,    -1,    -1,    -1,   206,    -1,   208,    -1,   210,   211,
      -1,   213,    -1,   215,     4,     5,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      50,   121,    -1,    -1,    54,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,
      70,    71,    -1,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    91,    92,    93,    -1,    -1,    96,    97,    98,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   184,   115,   116,    -1,   188,    -1,
     190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,
     140,    -1,    -1,    -1,    -1,   215,    -1,    -1,   148,   149,
     150,   151,   152,    -1,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,
      -1,   191,   192,   193,    -1,   195,    -1,    -1,   198,   199,
      -1,    59,    60,    -1,    -1,    -1,   206,    -1,   208,    -1,
     210,   211,    -1,   213,    -1,   215,     4,     5,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,   121,    -1,    -1,    54,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   184,   115,   116,    -1,
     188,    -1,   190,   191,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,   206,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,   215,    -1,    -1,
     148,   149,   150,   151,   152,    32,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,    -1,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,   121,    -1,    44,    -1,   125,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,   184,    -1,    -1,
      -1,   188,    -1,   190,   191,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,   206,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,   215,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,    -1,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    25,    26,    27,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    51,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,    -1,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,    -1,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,    -1,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,    -1,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,   209,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,   209,   210,   211,    12,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,   207,
     208,    -1,   210,   211,    12,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,    -1,   210,   211,    12,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,   207,
     208,    -1,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,   207,
     208,    -1,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,    -1,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,    -1,   210,   211,    12,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,    -1,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,    -1,   210,   211,    12,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,    -1,   210,   211,    12,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,    -1,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    21,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,    -1,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,   209,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,    -1,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,   209,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,   209,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,   209,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,   209,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,   209,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,   209,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,    -1,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,    -1,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,    -1,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,     4,     5,   206,    -1,
     208,    -1,   210,   211,    -1,   213,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
       9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,
     208,    -1,   210,   211,    -1,   213,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,   138,
      -1,    -1,   141,   142,   143,   144,   145,   146,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,   184,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,   205,   206,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,
      -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,   167,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,
      -1,    -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    20,    21,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,   140,    -1,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,   210,    -1,
     212,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    20,    21,    -1,    -1,   133,
     134,   135,   136,   137,   138,   140,    -1,   141,   142,   143,
     144,   145,   146,   147,    -1,    -1,    -1,    -1,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,
     184,    -1,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   205,   206,    -1,    -1,   210,    -1,    -1,   212,    -1,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,   136,   137,   138,    -1,    -1,   141,   142,   143,   144,
     145,   146,   147,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,    -1,    -1,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
     135,   136,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,
      -1,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,    18,    -1,    -1,    20,    21,    -1,    24,    -1,    -1,
     205,   206,    -1,    30,    -1,    -1,    -1,   212,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,   187,   188,   189,   190,   191,   192,   193,   194,
     195,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
     205,   206,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,    -1,    -1,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,    20,    21,    -1,    -1,   133,    -1,   135,
     136,    -1,    -1,   140,    -1,   141,   142,   143,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,
     206,    -1,    -1,   210,    -1,    -1,   212,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,    -1,    -1,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,
     147,   113,   114,   115,   116,   117,    -1,    -1,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    20,    21,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,    -1,    -1,   147,    -1,   184,    -1,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,   205,   206,
      -1,    -1,    -1,    -1,    -1,   212,    -1,    -1,    -1,    -1,
      -1,    -1,   184,    -1,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,   206,    -1,    -1,    -1,    -1,    -1,
     212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,   114,   115,   116,   117,    -1,    -1,   120,   121,   122,
     123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,
     143,    -1,    -1,    -1,   147,   113,   114,   115,   116,   117,
      20,    21,   120,   121,   122,   123,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,
      -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,
      -1,   184,    -1,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   205,   206,    -1,    -1,    -1,    -1,    -1,   212,
      -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,    -1,
      -1,   209,    -1,   113,   114,   115,   116,   117,    20,    21,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,
      -1,   141,   142,   143,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   184,    -1,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   205,   206,    -1,    -1,   209,
      -1,   113,   114,   115,   116,   117,    -1,    -1,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,    -1,    -1,   147,   113,   114,   115,   116,
     117,    20,    21,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,
     147,    -1,   184,    -1,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,   206,    -1,    -1,   209,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,
      -1,    -1,   209,    -1,   113,   114,   115,   116,   117,    20,
      21,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   184,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   205,   206,    -1,    -1,
     209,    -1,   113,   114,   115,   116,   117,    -1,    -1,   120,
     121,   122,   123,    -1,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,
     141,   142,   143,    -1,    -1,    -1,   147,   113,   114,   115,
     116,   117,    20,    21,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,
     136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,
      -1,   147,    -1,   184,    -1,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   205,   206,    -1,    -1,   209,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,
     206,    -1,    -1,   209,    -1,   113,   114,   115,   116,   117,
      20,    21,   120,   121,   122,   123,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,
      -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,    -1,
      -1,   209,    -1,   113,   114,   115,   116,   117,    -1,    -1,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,
      -1,   141,   142,   143,    -1,    -1,    -1,   147,   113,   114,
     115,   116,   117,    20,    21,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
     135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,
      -1,    -1,   147,    -1,   184,    -1,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   205,   206,    -1,    -1,   209,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
      -1,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,    -1,    -1,   209,    -1,   113,   114,   115,   116,
     117,    20,    21,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,
     147,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,
      -1,    -1,   209,    -1,   113,   114,   115,   116,   117,    -1,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,   113,
     114,   115,   116,   117,    20,    21,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,
      -1,    -1,    -1,   147,    -1,   184,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   205,   206,    -1,    -1,
     209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     184,    -1,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   205,   206,    -1,    -1,   209,    -1,   113,   114,   115,
     116,   117,    20,    21,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,
     136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,
     206,    -1,    -1,   209,    -1,   113,   114,   115,   116,   117,
      -1,    -1,   120,   121,   122,   123,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,
      -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,
     113,   114,   115,   116,   117,    20,    21,   120,   121,   122,
     123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,
     143,    -1,    -1,    -1,   147,    -1,   184,    -1,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,    -1,
      -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   184,    -1,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   205,   206,    -1,    -1,   209,    -1,   113,   114,
     115,   116,   117,    20,    21,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
     135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
      -1,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,    -1,    -1,   209,    -1,   113,   114,   115,   116,
     117,    -1,    -1,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,
     147,   113,   114,   115,   116,   117,    20,    21,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,    -1,    -1,   147,    -1,   184,    -1,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,
      -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   184,    -1,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,   206,    -1,    -1,   209,    -1,   113,
     114,   115,   116,   117,    20,    21,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     184,    -1,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   205,   206,    -1,    -1,   209,    -1,   113,   114,   115,
     116,   117,    -1,    -1,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,
     136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,
      -1,   147,   113,   114,   115,   116,   117,    20,    21,   120,
     121,   122,   123,    -1,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,
     141,   142,   143,    -1,    -1,    -1,   147,    -1,   184,    -1,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,
     206,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   184,    -1,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   205,   206,    -1,    -1,   209,    -1,
     113,   114,   115,   116,   117,    20,    21,   120,   121,   122,
     123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,
     143,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   184,    -1,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   205,   206,    -1,    -1,   209,    -1,   113,   114,
     115,   116,   117,    -1,    -1,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
     135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,
      -1,    -1,   147,   113,   114,   115,   116,   117,    20,    21,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,
      -1,   141,   142,   143,    -1,    -1,    -1,   147,    -1,   184,
      -1,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   184,    -1,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   205,   206,    -1,    -1,   209,
      -1,   113,   114,   115,   116,   117,    20,    21,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   184,    -1,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,   206,    -1,    -1,   209,    -1,   113,
     114,   115,   116,   117,    -1,    -1,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,
      -1,    -1,    -1,   147,   113,   114,   115,   116,   117,    20,
      21,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,    -1,
     184,    -1,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   205,   206,    -1,    -1,   209,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   184,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   205,   206,    -1,    -1,
     209,    -1,   113,   114,   115,   116,   117,    20,    21,   120,
     121,   122,   123,    -1,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,
     141,   142,   143,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   184,    -1,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   205,   206,    -1,    -1,   209,    -1,
     113,   114,   115,   116,   117,    -1,    -1,   120,   121,   122,
     123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,
     143,    -1,    -1,    -1,   147,   113,   114,   115,   116,   117,
      20,    21,   120,   121,   122,   123,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,
      -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,
      -1,   184,    -1,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   205,   206,   207,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,   207,
      -1,    -1,    -1,   113,   114,   115,   116,   117,    20,    21,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,
      -1,   141,   142,   143,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   184,    -1,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   205,   206,   207,    -1,    -1,
      -1,   113,   114,   115,   116,   117,    -1,    -1,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,    -1,    -1,   147,   113,   114,   115,   116,
     117,    20,    21,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,
     147,    -1,   184,    -1,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,   206,   207,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,
     207,    -1,    -1,    -1,   113,   114,   115,   116,   117,    -1,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      20,    21,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   184,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   205,   206,   207,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
      -1,    20,    21,   133,   134,   135,   136,   137,   138,    -1,
      -1,   141,   142,   143,   144,   145,   146,   147,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,   184,    -1,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   205,   206,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,    -1,    -1,   125,   126,   127,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
      -1,    -1,   141,   142,   143,   144,   145,   146,   147,   113,
     114,   115,   116,   117,    20,    21,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,   139,    -1,   141,   142,   143,
      -1,    -1,    -1,   147,    -1,   184,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,    -1,    -1,   198,
     199,    -1,    -1,    -1,    -1,    -1,   205,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     184,    -1,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   205,   206,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,    20,    21,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,
     136,    -1,    -1,   139,    -1,   141,   142,   143,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,
     206,    -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,
      -1,    -1,   120,   121,   122,   123,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,
      -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,
     113,   114,   115,   116,   117,    20,    21,   120,   121,   122,
     123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,
     143,    -1,    -1,    -1,   147,    -1,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,    -1,
      -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   184,    -1,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   205,   206,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,    20,    21,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
     135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
      -1,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,    -1,    -1,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,
     147,   113,   114,   115,   116,   117,    20,    21,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,    -1,    -1,    -1,    -1,   184,    -1,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   184,    -1,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,   206,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,   117,    -1,    -1,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,   141,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,   113,   114,   115,   116,   117,    -1,    -1,   120,
     121,   122,   123,    -1,   125,   126,   127,   128,    -1,    -1,
      20,    21,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,
     141,    -1,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,   117,   205,   206,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,   113,   114,   115,   116,   117,    -1,    -1,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,   136,   113,   114,   115,
     116,   117,    -1,    -1,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,
     136,    -1,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,   205,   206,    -1,    -1,    -1,    -1,    -1,   188,   189,
     190,   191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   205,   206,    -1,    -1,    -1,
      -1,    18,    -1,   189,   190,   191,   192,   193,   194,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,
     206,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    -1,    96,
      97,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    69,    70,    71,
     167,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    93,    -1,    -1,    96,    97,    98,    99,    69,    -1,
      71,    -1,    73,    74,    75,    76,    77,    -1,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    -1,    96,    97,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   217,     0,     6,    29,    31,    33,    39,    49,    55,
      78,   100,   101,   195,   206,   212,   218,   221,   227,   229,
     230,   235,   264,   268,   300,   377,   384,   388,   399,   443,
     448,   453,    18,    19,   167,   257,   258,   259,   160,   236,
     237,   167,   168,   169,   195,   231,   232,   233,   213,   385,
     167,   210,   220,    56,    62,   380,   380,   380,   140,   167,
     287,    33,    62,   133,   199,   208,   260,   261,   262,   263,
     287,   212,     4,     5,     7,    35,   396,    61,   375,   183,
     182,   185,   182,   232,    21,    56,   194,   205,   234,   380,
     381,   383,   381,   375,   454,   444,   449,   167,   140,   228,
     262,   262,   262,   208,   141,   142,   143,   182,   207,    56,
      62,   269,   271,    56,    62,   386,    56,    62,   397,    56,
      62,   376,    14,    15,   160,   165,   167,   170,   208,   223,
     258,   160,   237,   167,   231,   231,   167,   212,   214,   381,
     212,    56,    62,   219,   167,   167,   167,   167,   171,   226,
     209,   259,   262,   262,   262,   262,   272,   167,   387,   400,
     213,   378,   171,   172,   222,    14,    15,   160,   165,   167,
     223,   255,   256,   234,   382,   212,   455,   445,   450,   171,
     209,    34,    69,    71,    73,    74,    75,    76,    77,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      91,    92,    93,    96,    97,    98,    99,   115,   116,   167,
     267,   270,   185,   213,   104,   394,   395,   373,   157,   349,
     171,   172,   173,   182,   209,   183,   213,   213,   213,    20,
      21,    37,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     125,   126,   127,   134,   135,   136,   137,   138,   141,   142,
     143,   144,   145,   146,   147,   184,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   198,   199,   205,   206,
      34,    34,   208,   265,   213,   273,    73,    77,    96,    97,
      98,    99,   404,   389,   167,   401,   214,   374,   259,   149,
     167,   371,   372,   255,    18,    24,    30,    40,    48,    63,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   153,   210,   287,   403,   405,   406,   409,
     414,   442,   456,   446,   451,   167,   167,   167,   207,    21,
     167,   207,   152,   209,   349,   359,   360,   185,   266,   276,
     213,   379,   185,   393,   213,   398,   349,   207,   208,    42,
     182,   185,   188,   370,   189,   189,   189,   208,   189,   189,
     208,   189,   189,   189,   189,   189,   189,   208,   287,    32,
      59,    60,   121,   125,   184,   188,   191,   206,   215,   413,
     186,   408,   363,   366,   167,   134,   208,    49,   149,   167,
     368,   402,   209,   212,   442,     1,     4,     5,     8,     9,
      10,    12,    14,    15,    16,    17,    18,    24,    25,    26,
      27,    28,    30,    37,    38,    41,    43,    44,    47,    50,
      51,    53,    54,    57,    58,    64,    67,    68,    78,   100,
     101,   102,   103,   115,   116,   130,   131,   132,   148,   149,
     150,   151,   152,   154,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   168,   169,   170,   191,   192,
     193,   198,   199,   206,   208,   210,   211,   212,   213,   214,
     225,   227,   238,   239,   242,   245,   246,   248,   250,   251,
     252,   253,   274,   275,   277,   281,   286,   287,   288,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   304,   305,
     312,   315,   318,   323,   326,   327,   328,   329,   330,   332,
     337,   340,   341,   348,   403,   458,   467,   478,   482,   489,
     492,   391,   167,   212,   390,   287,   355,   372,   207,    64,
     168,   281,   340,   341,   167,   167,   414,   124,   134,   183,
     369,   415,   420,   422,   341,   424,   418,   167,   410,   426,
     428,   430,   432,   434,   436,   438,   440,   341,   189,   208,
      32,   188,    32,   188,   206,   215,   207,   341,   206,   215,
     414,   167,   212,   457,   167,   212,   361,   442,   447,   167,
     212,   364,   452,   341,   368,   208,    42,   124,   182,   183,
     185,   188,   367,   359,   189,   189,   341,   249,   189,   289,
     405,   458,   208,   189,   189,   208,   124,   286,   316,   327,
     341,   273,   189,   208,    60,   341,   208,   341,   167,   189,
     189,   208,   212,   189,   160,    57,   341,   208,   273,   189,
     208,   189,   189,   208,   189,   189,   124,   286,   341,   341,
     341,   211,   273,   318,   322,   322,   322,   208,   208,   208,
     208,   208,   208,    12,   414,    12,   414,    12,   341,   477,
     487,   189,   341,   189,   224,   341,   341,   341,   341,   341,
      12,   316,   341,   316,   211,    12,   477,   488,    36,   212,
     212,   341,     9,   318,   324,   167,   208,   212,   212,   212,
     212,   212,    65,   301,   264,   129,   212,   324,    20,    21,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   133,   134,   135,   136,   137,   138,
     141,   142,   143,   144,   145,   146,   147,   183,   184,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   205,
     206,   208,   379,   183,   212,   214,   206,   214,   264,   356,
     209,    42,   212,   369,   286,   341,   442,   442,   412,   442,
     209,   442,   442,   209,   167,   407,   442,   265,   442,   265,
     442,   265,   361,   362,   364,   365,   209,   417,   278,   316,
     207,   207,   341,   212,   214,   185,   212,   214,   185,   212,
     214,   209,   341,   167,   167,   442,   286,   341,   468,   470,
     273,   402,   290,   208,   287,   313,   483,   316,   286,   316,
     182,   124,   183,    13,   414,   488,   341,   341,   273,   183,
     306,   308,   341,   310,   185,   160,   341,   485,   316,   474,
     479,   316,   472,   414,   286,   341,   211,   264,   341,   341,
     341,   341,   341,   341,   402,    52,   155,   167,   191,   206,
     208,   341,   459,   460,   461,   476,   481,   402,   208,   460,
     481,   402,   139,   177,   212,   214,   462,   282,   273,   284,
     173,   174,   222,   402,   182,   491,   491,   149,   154,   189,
     287,   331,   402,   182,   491,   213,   273,   247,   325,   206,
     209,   316,   460,   157,   303,   372,   157,   321,   322,    18,
     155,   167,   403,    18,   155,   167,   403,   341,   341,   341,
     341,   341,   341,   341,   341,   341,   341,   341,   341,   341,
     341,   341,   341,   167,   130,   131,   132,   274,   341,   341,
     155,   167,   341,   341,   341,   403,   341,   341,   341,   341,
     341,   341,   341,   341,   341,   341,   341,   341,   130,   131,
     132,   274,   341,    21,   341,   341,   341,   341,   341,   341,
     341,   341,   341,   341,   341,   126,   127,   155,   167,   205,
     206,   338,   341,   209,   316,   392,   341,   263,     7,   349,
     354,   167,   286,   341,   212,   190,   190,   190,   212,   190,
     190,   190,   212,   190,   266,   190,   266,   190,   266,   190,
     212,   190,   212,   280,   442,   209,   207,   442,   442,   209,
      42,   367,   414,   414,    19,   442,   209,   316,   460,   189,
     341,   442,   414,   491,   341,   273,   190,   212,   491,   209,
     254,   414,   414,   209,   414,   209,   414,   491,   414,   414,
     491,   414,   190,   321,   209,   209,   209,   209,   209,   209,
      19,   322,   208,   134,   367,   206,   341,   209,   139,   182,
     212,   461,   179,   180,   207,   465,   182,   179,   207,   212,
     464,    19,   209,   461,   178,   214,   463,    19,   341,   477,
     214,   280,   280,   341,    19,   207,   209,   208,   208,   333,
     335,    19,   477,   214,   276,    11,    22,    23,   240,   241,
     341,   264,   459,   182,   209,   209,   167,   302,   124,   134,
     183,   188,   319,   320,   265,   189,   208,   189,   208,   208,
     322,   322,   322,   208,   207,   322,   322,   322,    18,   155,
     167,   403,   185,   155,   167,   341,   208,   208,   155,   167,
     341,     1,   207,   209,   214,   212,   207,    56,    62,   352,
      66,   353,   212,   212,   416,   421,   423,   442,   425,   419,
     411,   167,   427,   190,   431,   190,   435,   190,   439,   361,
     441,   364,   190,   212,   167,   341,   190,   190,   316,   190,
     209,   209,   167,   209,   190,   190,   209,   208,   414,   209,
     341,   190,   190,   190,   190,   209,   190,   190,   209,   190,
     321,   265,   208,   316,   341,   341,   341,   167,   460,   461,
     341,   155,   167,   459,   465,   207,   341,   207,   476,   316,
     460,   178,   181,   214,   466,   207,   316,   190,   190,   175,
     316,   341,   341,   414,   265,   316,   214,   273,   341,    11,
     243,   321,   207,   206,   182,   207,   167,   167,   167,   167,
     182,   207,   266,   342,   341,   344,   341,   209,   316,   341,
     189,   208,   341,   208,   207,   341,   209,   316,   208,   207,
     339,   212,    46,   353,    45,   104,   350,   429,   433,   437,
     208,   442,   469,   471,   273,   291,   190,   212,   314,   484,
     488,   190,   307,   309,   311,   486,   475,   480,   473,   208,
     266,   209,   316,   212,   209,   461,   465,   208,   134,   367,
     212,   461,   207,   212,   283,   285,   212,   209,   209,   190,
     266,   212,   273,   244,   212,   265,   209,   459,   167,   207,
     319,   207,   139,   273,   317,   414,   209,   442,   209,   209,
     209,   346,   341,   341,   209,   209,   341,    32,   351,   350,
     352,   278,   208,   208,   341,   167,   208,   491,   208,   341,
     341,   341,   208,   208,   208,   209,   341,   317,   209,   341,
     134,   367,   465,   341,   341,   341,   341,   466,   477,   341,
     208,   279,   341,   334,   190,   477,   241,    25,   103,   245,
     293,   294,   295,   297,   341,   266,   207,   185,   370,   124,
     341,   190,   190,   442,   209,   209,   209,   357,   351,   368,
     209,   460,   460,   209,   190,   316,   209,   488,   316,   460,
     460,   209,   212,   490,   341,   341,   209,   490,   490,   278,
     490,   331,   336,   490,   124,   124,   341,   273,   209,   414,
     369,   341,   343,   345,   190,   270,   358,   209,   209,   341,
     491,   491,   491,   209,   209,    52,   207,   134,   367,   214,
     214,   209,   207,   331,   214,   341,   369,   341,   347,   212,
     270,   209,   209,   209,   209,   341,   207,   341,   341,   207,
     214,   341,   212,   273,   212
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   216,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   218,   219,   219,
     219,   220,   220,   221,   222,   222,   222,   222,   223,   224,
     224,   224,   225,   226,   226,   228,   227,   229,   230,   231,
     231,   231,   232,   232,   232,   232,   233,   233,   234,   234,
     235,   236,   236,   237,   237,   238,   239,   239,   240,   240,
     241,   241,   241,   242,   242,   243,   244,   243,   245,   245,
     245,   245,   245,   246,   247,   246,   249,   248,   250,   251,
     252,   254,   253,   255,   255,   255,   255,   255,   255,   256,
     256,   257,   257,   257,   258,   258,   258,   258,   258,   258,
     258,   258,   259,   259,   260,   260,   260,   261,   261,   262,
     262,   262,   262,   262,   262,   262,   263,   263,   264,   264,
     265,   265,   265,   266,   266,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     268,   269,   269,   269,   270,   272,   271,   273,   273,   274,
     274,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   276,   276,   276,   277,   278,   278,   279,   279,
     280,   280,   282,   283,   281,   284,   285,   281,   286,   286,
     286,   286,   286,   287,   287,   287,   288,   288,   290,   291,
     289,   289,   292,   292,   292,   292,   292,   293,   294,   295,
     295,   295,   296,   296,   296,   297,   297,   298,   298,   298,
     299,   300,   300,   301,   301,   302,   302,   303,   303,   304,
     304,   306,   307,   305,   308,   309,   305,   310,   311,   305,
     313,   314,   312,   315,   315,   315,   316,   316,   317,   317,
     317,   318,   318,   318,   319,   319,   319,   319,   320,   320,
     321,   321,   322,   322,   323,   325,   324,   326,   326,   326,
     326,   326,   326,   326,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   328,   328,   328,   328,   328,   328,   328,
     328,   329,   329,   330,   330,   331,   331,   332,   333,   334,
     332,   335,   336,   332,   337,   337,   337,   337,   338,   339,
     337,   340,   340,   340,   340,   340,   341,   341,   341,   341,
     341,   341,   341,   341,   341,   341,   341,   341,   341,   341,
     341,   341,   341,   341,   341,   341,   341,   341,   341,   341,
     341,   341,   341,   341,   341,   341,   341,   341,   341,   341,
     341,   341,   341,   341,   341,   341,   341,   341,   341,   341,
     341,   341,   341,   341,   341,   341,   341,   341,   341,   341,
     341,   341,   342,   343,   341,   341,   341,   341,   344,   345,
     341,   341,   341,   346,   347,   341,   341,   341,   341,   341,
     341,   341,   341,   341,   341,   341,   341,   341,   341,   341,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   349,   349,   350,   350,
     350,   351,   351,   352,   352,   352,   353,   353,   354,   355,
     356,   355,   357,   355,   358,   355,   355,   359,   359,   359,
     360,   360,   361,   361,   362,   362,   363,   363,   363,   364,
     365,   365,   366,   366,   366,   367,   367,   368,   368,   368,
     368,   368,   368,   369,   369,   369,   370,   370,   371,   371,
     371,   371,   371,   372,   372,   372,   372,   372,   373,   374,
     373,   375,   375,   376,   376,   376,   377,   378,   377,   379,
     379,   379,   379,   380,   380,   380,   382,   381,   383,   383,
     384,   385,   384,   386,   386,   386,   387,   389,   390,   388,
     391,   392,   388,   393,   393,   394,   394,   395,   396,   396,
     397,   397,   397,   398,   398,   400,   401,   399,   402,   402,
     402,   402,   402,   403,   403,   403,   403,   403,   403,   403,
     403,   403,   403,   403,   403,   403,   403,   403,   403,   403,
     403,   403,   403,   403,   403,   403,   403,   403,   403,   403,
     404,   404,   404,   404,   404,   404,   405,   406,   406,   406,
     407,   407,   408,   408,   408,   410,   411,   409,   412,   412,
     413,   413,   414,   414,   414,   414,   414,   414,   415,   416,
     414,   414,   414,   417,   414,   414,   414,   414,   414,   414,
     414,   414,   414,   414,   414,   414,   414,   418,   419,   414,
     414,   420,   421,   414,   422,   423,   414,   424,   425,   414,
     414,   426,   427,   414,   428,   429,   414,   414,   430,   431,
     414,   432,   433,   414,   414,   434,   435,   414,   436,   437,
     414,   438,   439,   414,   440,   441,   414,   442,   442,   442,
     444,   445,   446,   447,   443,   449,   450,   451,   452,   448,
     454,   455,   456,   457,   453,   458,   458,   458,   458,   458,
     459,   459,   459,   459,   459,   459,   459,   459,   460,   460,
     461,   461,   462,   462,   463,   463,   464,   464,   465,   465,
     465,   466,   466,   466,   467,   467,   467,   467,   467,   467,
     468,   469,   467,   470,   471,   467,   472,   473,   467,   474,
     475,   467,   476,   476,   476,   477,   477,   478,   479,   480,
     478,   481,   481,   482,   482,   482,   482,   483,   484,   482,
     482,   485,   486,   482,   487,   487,   488,   488,   489,   489,
     489,   489,   489,   490,   490,   491,   491,   492,   492,   492,
     492,   492
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
       1,     5,     1,     3,     1,     1,     1,     1,     4,     1,
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
       2,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     2,
       2,     2,     0,     2,     2,     3,     0,     1,     0,     3,
       1,     3,     0,     0,     7,     0,     0,     7,     3,     2,
       2,     2,     1,     1,     3,     2,     2,     3,     0,     0,
       5,     1,     2,     4,     5,     5,     2,     1,     1,     1,
       2,     3,     2,     2,     3,     2,     3,     2,     2,     3,
       4,     1,     1,     1,     0,     1,     3,     9,     8,     3,
       3,     0,     0,     7,     0,     0,     7,     0,     0,     7,
       0,     0,     6,     5,     8,    10,     1,     3,     1,     2,
       3,     1,     1,     2,     2,     2,     2,     2,     1,     3,
       0,     4,     1,     6,     6,     0,     7,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     4,     4,     4,     4,     4,     4,     3,
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
       7,     7,     6,     7,     6,     6,     0,     4,     0,     1,
       1,     0,     1,     0,     1,     1,     0,     1,     5,     0,
       0,     4,     0,     9,     0,    10,     5,     2,     3,     4,
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
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       1,     3,     0,     2,     3,     0,     0,     6,     1,     3,
       3,     4,     1,     1,     1,     1,     2,     3,     0,     0,
       6,     4,     5,     0,     9,     4,     2,     2,     3,     2,
       3,     2,     2,     3,     3,     3,     2,     0,     0,     6,
       2,     0,     0,     6,     0,     0,     6,     0,     0,     6,
       1,     0,     0,     6,     0,     0,     7,     1,     0,     0,
       6,     0,     0,     7,     1,     0,     0,     6,     0,     0,
       7,     0,     0,     6,     0,     0,     6,     1,     3,     3,
       0,     0,     0,     0,    10,     0,     0,     0,     0,    10,
       0,     0,     0,     0,    10,     1,     1,     1,     1,     1,
       3,     3,     5,     5,     6,     6,     8,     8,     1,     3,
       0,     2,     2,     1,     2,     1,     2,     1,     2,     1,
       1,     2,     1,     1,     5,     4,     6,     7,     5,     7,
       0,     0,     9,     0,     0,     9,     0,     0,     9,     0,
       0,     6,     1,     3,     3,     3,     1,     5,     0,     0,
       9,     1,     3,     4,     4,     4,     5,     0,     0,    10,
       5,     0,     0,    10,     1,     3,     1,     3,     4,     3,
       5,     8,    10,     0,     3,     0,     1,     9,    10,    10,
       9,    10
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

  case 17: /* top_level_reader_macro: expr_reader ';'  */
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

  case 75: /* expression_if_then_else: expression_if_one_liner "if" $@3 expr expression_else_one_liner ';'  */
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

  case 104: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 105: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 106: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 107: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 108: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 109: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 110: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 111: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 112: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 113: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 114: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 115: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 116: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 117: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 118: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 119: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 120: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 121: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 122: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 123: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 124: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 125: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 126: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 127: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 128: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 129: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 130: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 131: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 132: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 133: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 134: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 135: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 136: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 137: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 138: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 139: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 140: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 141: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 142: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 143: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 144: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 145: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 146: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 147: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 148: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 149: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 150: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 151: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 152: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 153: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 154: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 155: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 156: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 157: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 158: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 159: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 160: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 161: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 162: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 163: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 164: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 165: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 166: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 167: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 168: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 169: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 170: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 171: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 172: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 173: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 174: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 175: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 176: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 177: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 178: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 179: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 180: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 181: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 182: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 183: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 184: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 185: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 186: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 187: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 188: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 189: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 190: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 191: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 192: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 193: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 194: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 195: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 196: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 197: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 198: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 199: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 200: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 201: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 202: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 203: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 204: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 205: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 206: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 207: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 208: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 209: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 210: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 211: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 212: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 213: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 214: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 215: /* $@6: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 216: /* function_declaration: optional_public_or_private_function $@6 function_declaration_header expression_block  */
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

  case 217: /* expression_block: '{' expressions '}'  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 218: /* expression_block: '{' expressions '}' "finally" '{' expressions '}'  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 219: /* expr_call_pipe: expr_call expr_full_block_assumed_piped  */
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

  case 220: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 221: /* expression_any: ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 222: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 223: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 224: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 225: /* expression_any: expr_assign ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 226: /* expression_any: expression_delete ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 227: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 228: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 229: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 230: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 231: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 232: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 233: /* expression_any: expression_break ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 234: /* expression_any: expression_continue ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 235: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 236: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 237: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 238: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 239: /* expression_any: expression_label ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 240: /* expression_any: expression_goto ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 241: /* expression_any: "pass" ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 242: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 243: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 244: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 245: /* expr_keyword: "keyword" expr expression_block  */
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

  case 246: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 247: /* optional_expr_list: expr_list  */
                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 248: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 249: /* optional_expr_list_in_braces: '(' optional_expr_list ')'  */
                                              { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 250: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 251: /* type_declaration_no_options_list: type_declaration_no_options_list ';' type_declaration  */
                                                                           {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 252: /* $@7: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 253: /* $@8: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 254: /* expression_keyword: "keyword" '<' $@7 type_declaration_no_options_list '>' $@8 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 255: /* $@9: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 256: /* $@10: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 257: /* expression_keyword: "type function" '<' $@9 type_declaration_no_options_list '>' $@10 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 258: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 259: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 260: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 261: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 262: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 263: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 264: /* name_in_namespace: "name" "::" "name"  */
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

  case 265: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 266: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 267: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 268: /* $@11: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 269: /* $@12: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 270: /* new_type_declaration: '<' $@11 type_declaration '>' $@12  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 271: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 272: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 273: /* expr_new: "new" new_type_declaration '(' ')'  */
                                                               {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 274: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 275: /* expr_new: "new" new_type_declaration '(' make_struct_dim ')'  */
                                                                                   {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 276: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 277: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 278: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 279: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 280: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 281: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 282: /* expression_return: expression_return_no_pipe ';'  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 283: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 284: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 285: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 286: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 287: /* expression_yield: expression_yield_no_pipe ';'  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 288: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 289: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 290: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 291: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 292: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 293: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 294: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 295: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 296: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 297: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 298: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr ';'  */
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

  case 299: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 300: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 301: /* $@13: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 302: /* $@14: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 303: /* expr_cast: "cast" '<' $@13 type_declaration_no_options '>' $@14 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 304: /* $@15: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 305: /* $@16: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 306: /* expr_cast: "upcast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 307: /* $@17: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 308: /* $@18: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 309: /* expr_cast: "reinterpret" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 310: /* $@19: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 311: /* $@20: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 312: /* expr_type_decl: "type" '<' $@19 type_declaration '>' $@20  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 313: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 314: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 315: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
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

  case 316: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 317: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 318: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 319: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 320: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 321: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 322: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 323: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 324: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 325: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 326: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 327: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 328: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 329: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 330: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 331: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 332: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 333: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 334: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 335: /* $@21: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 336: /* expr_full_block_assumed_piped: block_or_lambda $@21 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 337: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 338: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 339: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 340: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 341: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 342: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 343: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 344: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 345: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 346: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 347: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 348: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 349: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 350: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 351: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 352: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 353: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 354: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 355: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 356: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 357: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 358: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 359: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 360: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 361: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 362: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 363: /* expr_assign_pipe: expr '=' "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 364: /* expr_assign_pipe: expr '=' "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 365: /* expr_assign_pipe: expr '=' "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 366: /* expr_assign_pipe: expr "<-" "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 367: /* expr_assign_pipe: expr "<-" "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 368: /* expr_assign_pipe: expr "<-" "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 369: /* expr_assign_pipe: expr '=' expr_call_pipe  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 370: /* expr_assign_pipe: expr "<-" expr_call_pipe  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 371: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 372: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 373: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 374: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 375: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 376: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 377: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 378: /* $@22: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 379: /* $@23: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 380: /* func_addr_expr: '@' '@' '<' $@22 type_declaration_no_options '>' $@23 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 381: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 382: /* $@25: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 383: /* func_addr_expr: '@' '@' '<' $@24 optional_function_argument_list optional_function_type '>' $@25 func_addr_name  */
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

  case 384: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 385: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 386: /* expr_field: expr '.' "name" '(' ')'  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        das_yyerror(scanner,"foo.bar() method call syntax is not supported, use foo->bar() instead",
            tokAt(scanner,(yylsp[-3])),CompilationError::syntax_error);
    }
    break;

  case 387: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                       {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-5].pExpression)), *(yyvsp[-3].s));
        delete (yyvsp[-3].s);
        das_yyerror(scanner,"foo.bar(...) method call syntax is not supported, use foo->bar(...) instead",
            tokAt(scanner,(yylsp[-4])),CompilationError::syntax_error);
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 388: /* $@26: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 389: /* $@27: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 390: /* expr_field: expr '.' $@26 error $@27  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 391: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 392: /* expr_call: name_in_namespace '(' make_struct_dim ')'  */
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

  case 393: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 394: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 395: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 396: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 397: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 398: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 399: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 400: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 401: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 402: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 403: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 404: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 405: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 406: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 407: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 408: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 409: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 410: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 411: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 412: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 413: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 414: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 415: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 416: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 417: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 418: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 419: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 420: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 421: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 422: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 423: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 424: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 425: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 426: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 427: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 428: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 429: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 430: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 431: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 432: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 433: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 434: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 435: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 436: /* expr: '(' expr_list optional_comma ')'  */
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

  case 437: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 438: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 439: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 440: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 441: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 442: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 443: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 444: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 445: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 446: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 447: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 448: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 449: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 450: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 451: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 452: /* $@28: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 453: /* $@29: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 454: /* expr: expr "is" "type" '<' $@28 type_declaration_no_options '>' $@29  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 455: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 456: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 457: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 458: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 459: /* $@31: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 460: /* expr: expr "as" "type" '<' $@30 type_declaration '>' $@31  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 461: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 462: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 463: /* $@32: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 464: /* $@33: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 465: /* expr: expr '?' "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 466: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 467: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 468: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 469: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 470: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 471: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 472: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 473: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 474: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 475: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 476: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 477: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 478: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 479: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 480: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 481: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 482: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 483: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 484: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 485: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 486: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 487: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 488: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 489: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 490: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 491: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 492: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 493: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 494: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 495: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 496: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 497: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 498: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 499: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 500: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 501: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 502: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 503: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 504: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 505: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 506: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 507: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 508: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 509: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 510: /* $@34: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 511: /* struct_variable_declaration_list: struct_variable_declaration_list $@34 structure_variable_declaration ';'  */
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

  case 512: /* $@35: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 513: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@35 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 514: /* $@36: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 515: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@36 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 516: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 517: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 518: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 519: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 520: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 521: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 522: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 523: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 524: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 525: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 526: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 527: /* tuple_alias_type_list: tuple_alias_type_list ';'  */
                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 528: /* tuple_alias_type_list: tuple_alias_type_list tuple_type ';'  */
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

  case 529: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 530: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 531: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 532: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 533: /* variant_alias_type_list: variant_alias_type_list ';'  */
                                        {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 534: /* variant_alias_type_list: variant_alias_type_list variant_type ';'  */
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

  case 535: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 536: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 537: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 538: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 539: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 540: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 541: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 542: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 543: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 544: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 545: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 546: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 547: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 548: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 549: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 550: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 551: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 552: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 553: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options ';'  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 554: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 555: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 556: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 557: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 558: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 559: /* $@37: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 560: /* global_variable_declaration_list: global_variable_declaration_list $@37 optional_field_annotation let_variable_declaration  */
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

  case 561: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 562: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 563: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 564: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 565: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 566: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 567: /* $@38: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 568: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@38 optional_field_annotation let_variable_declaration  */
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

  case 569: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 570: /* enum_list: enum_list ';'  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 571: /* enum_list: enum_list "name" ';'  */
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

  case 572: /* enum_list: enum_list "name" '=' expr ';'  */
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

  case 573: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 574: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 575: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 576: /* $@39: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 577: /* single_alias: optional_public_or_private_alias "name" $@39 '=' type_declaration  */
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

  case 581: /* $@40: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 583: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 584: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 585: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 586: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 587: /* $@41: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 588: /* $@42: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 589: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name '{' $@41 enum_list $@42 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 590: /* $@43: %empty  */
                                                                                                                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 591: /* $@44: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 592: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration '{' $@43 enum_list $@44 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 593: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 594: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 595: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 596: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 597: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 598: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 599: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 600: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 601: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 602: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 603: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 604: /* optional_struct_variable_declaration_list: '{' struct_variable_declaration_list '}'  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 605: /* $@45: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 606: /* $@46: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 607: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@45 structure_name $@46 optional_struct_variable_declaration_list  */
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

  case 608: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 609: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 610: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 611: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 612: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 613: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 614: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 615: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 616: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 617: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 618: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 619: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 620: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 621: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 622: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 623: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 624: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 625: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 626: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 627: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 628: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 629: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 630: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 631: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 632: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 633: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 634: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 635: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 636: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 637: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 638: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 639: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 640: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 641: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 642: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 643: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 644: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 645: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 646: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 647: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 648: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 649: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 650: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 651: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 652: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 653: /* bitfield_alias_bits: bitfield_alias_bits ';'  */
                                      {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 654: /* bitfield_alias_bits: bitfield_alias_bits "name" ';'  */
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

  case 655: /* $@47: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 656: /* $@48: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 657: /* bitfield_type_declaration: "bitfield" '<' $@47 bitfield_bits '>' $@48  */
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

  case 658: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 659: /* table_type_pair: type_declaration ';' type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 660: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 661: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 662: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 663: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 664: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 665: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 666: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 667: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 668: /* $@49: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 669: /* $@50: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 670: /* type_declaration_no_options: "type" '<' $@49 type_declaration '>' $@50  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 671: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 672: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 673: /* $@51: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 674: /* type_declaration_no_options: '$' name_in_namespace '<' $@51 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 675: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 676: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 677: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 678: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 679: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 680: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 681: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 682: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 683: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 684: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 685: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 686: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 687: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 688: /* $@53: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 689: /* type_declaration_no_options: "smart_ptr" '<' $@52 type_declaration '>' $@53  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 690: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 691: /* $@54: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 692: /* $@55: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 693: /* type_declaration_no_options: "array" '<' $@54 type_declaration '>' $@55  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 694: /* $@56: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 695: /* $@57: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 696: /* type_declaration_no_options: "table" '<' $@56 table_type_pair '>' $@57  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 697: /* $@58: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 698: /* $@59: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 699: /* type_declaration_no_options: "iterator" '<' $@58 type_declaration '>' $@59  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 700: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 701: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 702: /* $@61: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 703: /* type_declaration_no_options: "block" '<' $@60 type_declaration '>' $@61  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 704: /* $@62: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 705: /* $@63: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 706: /* type_declaration_no_options: "block" '<' $@62 optional_function_argument_list optional_function_type '>' $@63  */
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

  case 707: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 708: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 709: /* $@65: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 710: /* type_declaration_no_options: "function" '<' $@64 type_declaration '>' $@65  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 711: /* $@66: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 712: /* $@67: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 713: /* type_declaration_no_options: "function" '<' $@66 optional_function_argument_list optional_function_type '>' $@67  */
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

  case 714: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 715: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 716: /* $@69: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 717: /* type_declaration_no_options: "lambda" '<' $@68 type_declaration '>' $@69  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 718: /* $@70: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 719: /* $@71: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 720: /* type_declaration_no_options: "lambda" '<' $@70 optional_function_argument_list optional_function_type '>' $@71  */
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

  case 721: /* $@72: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 722: /* $@73: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 723: /* type_declaration_no_options: "tuple" '<' $@72 tuple_type_list '>' $@73  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 724: /* $@74: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 725: /* $@75: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 726: /* type_declaration_no_options: "variant" '<' $@74 variant_type_list '>' $@75  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 727: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 728: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 729: /* type_declaration: type_declaration '|' '#'  */
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

  case 730: /* $@76: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 731: /* $@77: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 732: /* $@78: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 733: /* $@79: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 734: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@76 "name" $@77 '{' $@78 tuple_alias_type_list $@79 '}'  */
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

  case 735: /* $@80: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 736: /* $@81: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 737: /* $@82: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 738: /* $@83: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 739: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@80 "name" $@81 '{' $@82 variant_alias_type_list $@83 '}'  */
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

  case 740: /* $@84: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 741: /* $@85: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 742: /* $@86: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 743: /* $@87: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 744: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@84 "name" $@85 '{' $@86 bitfield_alias_bits $@87 '}'  */
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

  case 745: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 746: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 747: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 748: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 749: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 750: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 751: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 752: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 753: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 754: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 755: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 756: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 757: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 758: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 759: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 760: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 761: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 774: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 775: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 776: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 777: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 778: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 779: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 780: /* $@88: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 781: /* $@89: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 782: /* make_struct_decl: "struct" '<' $@88 type_declaration_no_options '>' $@89 '(' make_struct_dim ')'  */
                                                                                                                                                                                                        {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 783: /* $@90: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 784: /* $@91: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 785: /* make_struct_decl: "class" '<' $@90 type_declaration_no_options '>' $@91 '(' make_struct_dim ')'  */
                                                                                                                                                                                                       {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 786: /* $@92: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 787: /* $@93: %empty  */
                                                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 788: /* make_struct_decl: "variant" '<' $@92 type_declaration_no_options '>' $@93 '(' make_struct_dim ')'  */
                                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 789: /* $@94: %empty  */
                              { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 790: /* $@95: %empty  */
                                                                                                                                          { yyextra->das_arrow_depth --; }
    break;

  case 791: /* make_struct_decl: "default" '<' $@94 type_declaration_no_options '>' $@95  */
                                                                                                                                                                            {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-5]));
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->useInitializer = true;
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 792: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 793: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 794: /* make_tuple: make_tuple ',' expr  */
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

  case 795: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 796: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 797: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 798: /* $@96: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 799: /* $@97: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 800: /* make_tuple_call: "tuple" '<' $@96 type_declaration_no_options '>' $@97 '(' make_struct_dim ')'  */
                                                                                                                                                                                                        {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 801: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 802: /* make_dim: make_dim ';' make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 803: /* make_dim_decl: '[' expr_list optional_comma ']'  */
                                                          {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 804: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 805: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 806: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 807: /* $@98: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 808: /* $@99: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 809: /* make_dim_decl: "array" '<' $@98 type_declaration_no_options '>' $@99 '(' expr_list optional_comma ')'  */
                                                                                                                                                                              {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-9])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 810: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 811: /* $@100: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 812: /* $@101: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 813: /* make_dim_decl: "fixed_array" '<' $@100 type_declaration_no_options '>' $@101 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        if ( !mka->makeType->dim.size() ) mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 814: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 815: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 816: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 817: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 818: /* make_table_decl: '{' expr_map_tuple_list optional_comma '}'  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 819: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 820: /* make_table_decl: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 821: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
                                                                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-7])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-7])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 822: /* make_table_decl: "table" '<' type_declaration_no_options ';' type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
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

  case 823: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 824: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 825: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 826: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 827: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 828: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 829: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 830: /* array_comprehension: '{' "for" variable_name_with_pos_list "in" expr_list ';' make_map_tuple array_comprehension_where '}'  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 831: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list ';' make_map_tuple array_comprehension_where '}' '}'  */
                                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
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


