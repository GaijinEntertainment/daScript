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
  YYSYMBOL_DAS_WITH = 40,                  /* "with"  */
  YYSYMBOL_DAS_AKA = 41,                   /* "aka"  */
  YYSYMBOL_DAS_ASSUME = 42,                /* "assume"  */
  YYSYMBOL_DAS_CAST = 43,                  /* "cast"  */
  YYSYMBOL_DAS_OVERRIDE = 44,              /* "override"  */
  YYSYMBOL_DAS_ABSTRACT = 45,              /* "abstract"  */
  YYSYMBOL_DAS_UPCAST = 46,                /* "upcast"  */
  YYSYMBOL_DAS_ITERATOR = 47,              /* "iterator"  */
  YYSYMBOL_DAS_VAR = 48,                   /* "var"  */
  YYSYMBOL_DAS_ADDR = 49,                  /* "addr"  */
  YYSYMBOL_DAS_CONTINUE = 50,              /* "continue"  */
  YYSYMBOL_DAS_WHERE = 51,                 /* "where"  */
  YYSYMBOL_DAS_PASS = 52,                  /* "pass"  */
  YYSYMBOL_DAS_REINTERPRET = 53,           /* "reinterpret"  */
  YYSYMBOL_DAS_MODULE = 54,                /* "module"  */
  YYSYMBOL_DAS_PUBLIC = 55,                /* "public"  */
  YYSYMBOL_DAS_LABEL = 56,                 /* "label"  */
  YYSYMBOL_DAS_GOTO = 57,                  /* "goto"  */
  YYSYMBOL_DAS_IMPLICIT = 58,              /* "implicit"  */
  YYSYMBOL_DAS_EXPLICIT = 59,              /* "explicit"  */
  YYSYMBOL_DAS_SHARED = 60,                /* "shared"  */
  YYSYMBOL_DAS_PRIVATE = 61,               /* "private"  */
  YYSYMBOL_DAS_SMART_PTR = 62,             /* "smart_ptr"  */
  YYSYMBOL_DAS_UNSAFE = 63,                /* "unsafe"  */
  YYSYMBOL_DAS_INSCOPE = 64,               /* "inscope"  */
  YYSYMBOL_DAS_STATIC = 65,                /* "static"  */
  YYSYMBOL_DAS_TBOOL = 66,                 /* "bool"  */
  YYSYMBOL_DAS_TVOID = 67,                 /* "void"  */
  YYSYMBOL_DAS_TSTRING = 68,               /* "string"  */
  YYSYMBOL_DAS_TAUTO = 69,                 /* "auto"  */
  YYSYMBOL_DAS_TINT = 70,                  /* "int"  */
  YYSYMBOL_DAS_TINT2 = 71,                 /* "int2"  */
  YYSYMBOL_DAS_TINT3 = 72,                 /* "int3"  */
  YYSYMBOL_DAS_TINT4 = 73,                 /* "int4"  */
  YYSYMBOL_DAS_TUINT = 74,                 /* "uint"  */
  YYSYMBOL_DAS_TBITFIELD = 75,             /* "bitfield"  */
  YYSYMBOL_DAS_TUINT2 = 76,                /* "uint2"  */
  YYSYMBOL_DAS_TUINT3 = 77,                /* "uint3"  */
  YYSYMBOL_DAS_TUINT4 = 78,                /* "uint4"  */
  YYSYMBOL_DAS_TFLOAT = 79,                /* "float"  */
  YYSYMBOL_DAS_TFLOAT2 = 80,               /* "float2"  */
  YYSYMBOL_DAS_TFLOAT3 = 81,               /* "float3"  */
  YYSYMBOL_DAS_TFLOAT4 = 82,               /* "float4"  */
  YYSYMBOL_DAS_TRANGE = 83,                /* "range"  */
  YYSYMBOL_DAS_TURANGE = 84,               /* "urange"  */
  YYSYMBOL_DAS_TRANGE64 = 85,              /* "range64"  */
  YYSYMBOL_DAS_TURANGE64 = 86,             /* "urange64"  */
  YYSYMBOL_DAS_TBLOCK = 87,                /* "block"  */
  YYSYMBOL_DAS_TINT64 = 88,                /* "int64"  */
  YYSYMBOL_DAS_TUINT64 = 89,               /* "uint64"  */
  YYSYMBOL_DAS_TDOUBLE = 90,               /* "double"  */
  YYSYMBOL_DAS_TFUNCTION = 91,             /* "function"  */
  YYSYMBOL_DAS_TLAMBDA = 92,               /* "lambda"  */
  YYSYMBOL_DAS_TINT8 = 93,                 /* "int8"  */
  YYSYMBOL_DAS_TUINT8 = 94,                /* "uint8"  */
  YYSYMBOL_DAS_TINT16 = 95,                /* "int16"  */
  YYSYMBOL_DAS_TUINT16 = 96,               /* "uint16"  */
  YYSYMBOL_DAS_TTUPLE = 97,                /* "tuple"  */
  YYSYMBOL_DAS_TVARIANT = 98,              /* "variant"  */
  YYSYMBOL_DAS_GENERATOR = 99,             /* "generator"  */
  YYSYMBOL_DAS_YIELD = 100,                /* "yield"  */
  YYSYMBOL_DAS_SEALED = 101,               /* "sealed"  */
  YYSYMBOL_ADDEQU = 102,                   /* "+="  */
  YYSYMBOL_SUBEQU = 103,                   /* "-="  */
  YYSYMBOL_DIVEQU = 104,                   /* "/="  */
  YYSYMBOL_MULEQU = 105,                   /* "*="  */
  YYSYMBOL_MODEQU = 106,                   /* "%="  */
  YYSYMBOL_ANDEQU = 107,                   /* "&="  */
  YYSYMBOL_OREQU = 108,                    /* "|="  */
  YYSYMBOL_XOREQU = 109,                   /* "^="  */
  YYSYMBOL_SHL = 110,                      /* "<<"  */
  YYSYMBOL_SHR = 111,                      /* ">>"  */
  YYSYMBOL_ADDADD = 112,                   /* "++"  */
  YYSYMBOL_SUBSUB = 113,                   /* "--"  */
  YYSYMBOL_LEEQU = 114,                    /* "<="  */
  YYSYMBOL_SHLEQU = 115,                   /* "<<="  */
  YYSYMBOL_SHREQU = 116,                   /* ">>="  */
  YYSYMBOL_GREQU = 117,                    /* ">="  */
  YYSYMBOL_EQUEQU = 118,                   /* "=="  */
  YYSYMBOL_NOTEQU = 119,                   /* "!="  */
  YYSYMBOL_RARROW = 120,                   /* "->"  */
  YYSYMBOL_LARROW = 121,                   /* "<-"  */
  YYSYMBOL_QQ = 122,                       /* "??"  */
  YYSYMBOL_QDOT = 123,                     /* "?."  */
  YYSYMBOL_QBRA = 124,                     /* "?["  */
  YYSYMBOL_LPIPE = 125,                    /* "<|"  */
  YYSYMBOL_LBPIPE = 126,                   /* " <|"  */
  YYSYMBOL_LLPIPE = 127,                   /* "$ <|"  */
  YYSYMBOL_LAPIPE = 128,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 129,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 130,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 131,                 /* ":="  */
  YYSYMBOL_ROTL = 132,                     /* "<<<"  */
  YYSYMBOL_ROTR = 133,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 134,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 135,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 136,                    /* "=>"  */
  YYSYMBOL_COLCOL = 137,                   /* "::"  */
  YYSYMBOL_ANDAND = 138,                   /* "&&"  */
  YYSYMBOL_OROR = 139,                     /* "||"  */
  YYSYMBOL_XORXOR = 140,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 141,                /* "&&="  */
  YYSYMBOL_OROREQU = 142,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 143,                /* "^^="  */
  YYSYMBOL_DOTDOT = 144,                   /* ".."  */
  YYSYMBOL_MTAG_E = 145,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 146,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 147,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 148,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 149,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 150,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 151,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 152,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 153,           /* "..."  */
  YYSYMBOL_BRABRAB = 154,                  /* "[["  */
  YYSYMBOL_BRACBRB = 155,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 156,                  /* "{{"  */
  YYSYMBOL_INTEGER = 157,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 158,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 159,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 160,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 161,            /* "unsigned int8 constant"  */
  YYSYMBOL_FLOAT = 162,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 163,                   /* "double constant"  */
  YYSYMBOL_NAME = 164,                     /* "name"  */
  YYSYMBOL_KEYWORD = 165,                  /* "keyword"  */
  YYSYMBOL_BEGIN_STRING = 166,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 167,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 168,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 169,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 170,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 171,          /* "}"  */
  YYSYMBOL_END_OF_READ = 172,              /* "end of failed eader macro"  */
  YYSYMBOL_SEMICOLON_CUR_CUR = 173,        /* ";}}"  */
  YYSYMBOL_SEMICOLON_CUR_SQR = 174,        /* ";}]"  */
  YYSYMBOL_SEMICOLON_SQR_SQR = 175,        /* ";]]"  */
  YYSYMBOL_COMMA_SQR_SQR = 176,            /* ",]]"  */
  YYSYMBOL_COMMA_CUR_SQR = 177,            /* ",}]"  */
  YYSYMBOL_178_ = 178,                     /* ','  */
  YYSYMBOL_179_ = 179,                     /* '='  */
  YYSYMBOL_180_ = 180,                     /* '?'  */
  YYSYMBOL_181_ = 181,                     /* ':'  */
  YYSYMBOL_182_ = 182,                     /* '|'  */
  YYSYMBOL_183_ = 183,                     /* '^'  */
  YYSYMBOL_184_ = 184,                     /* '&'  */
  YYSYMBOL_185_ = 185,                     /* '<'  */
  YYSYMBOL_186_ = 186,                     /* '>'  */
  YYSYMBOL_187_ = 187,                     /* '-'  */
  YYSYMBOL_188_ = 188,                     /* '+'  */
  YYSYMBOL_189_ = 189,                     /* '*'  */
  YYSYMBOL_190_ = 190,                     /* '/'  */
  YYSYMBOL_191_ = 191,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 192,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 193,               /* UNARY_PLUS  */
  YYSYMBOL_194_ = 194,                     /* '~'  */
  YYSYMBOL_195_ = 195,                     /* '!'  */
  YYSYMBOL_PRE_INC = 196,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 197,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 198,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 199,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 200,                    /* DEREF  */
  YYSYMBOL_201_ = 201,                     /* '.'  */
  YYSYMBOL_202_ = 202,                     /* '['  */
  YYSYMBOL_203_ = 203,                     /* ']'  */
  YYSYMBOL_204_ = 204,                     /* '('  */
  YYSYMBOL_205_ = 205,                     /* ')'  */
  YYSYMBOL_206_ = 206,                     /* '$'  */
  YYSYMBOL_207_ = 207,                     /* '@'  */
  YYSYMBOL_208_ = 208,                     /* ';'  */
  YYSYMBOL_209_ = 209,                     /* '{'  */
  YYSYMBOL_210_ = 210,                     /* '}'  */
  YYSYMBOL_211_ = 211,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 212,                 /* $accept  */
  YYSYMBOL_program = 213,                  /* program  */
  YYSYMBOL_top_level_reader_macro = 214,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 215, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 216,              /* module_name  */
  YYSYMBOL_module_declaration = 217,       /* module_declaration  */
  YYSYMBOL_character_sequence = 218,       /* character_sequence  */
  YYSYMBOL_string_constant = 219,          /* string_constant  */
  YYSYMBOL_string_builder_body = 220,      /* string_builder_body  */
  YYSYMBOL_string_builder = 221,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 222, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 223,              /* expr_reader  */
  YYSYMBOL_224_1 = 224,                    /* $@1  */
  YYSYMBOL_options_declaration = 225,      /* options_declaration  */
  YYSYMBOL_require_declaration = 226,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 227,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 228,      /* require_module_name  */
  YYSYMBOL_require_module = 229,           /* require_module  */
  YYSYMBOL_is_public_module = 230,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 231,       /* expect_declaration  */
  YYSYMBOL_expect_list = 232,              /* expect_list  */
  YYSYMBOL_expect_error = 233,             /* expect_error  */
  YYSYMBOL_expression_label = 234,         /* expression_label  */
  YYSYMBOL_expression_goto = 235,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 236,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 237,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 238,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 239, /* expression_else_one_liner  */
  YYSYMBOL_240_2 = 240,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 241,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 242,  /* expression_if_then_else  */
  YYSYMBOL_243_3 = 243,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 244,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 245,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 246,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 247,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 248,    /* expression_with_alias  */
  YYSYMBOL_249_4 = 249,                    /* $@4  */
  YYSYMBOL_annotation_argument_value = 250, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 251, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 252, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 253,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 254, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 255, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 256, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 257,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 258,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 259, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 260, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 261,   /* optional_function_type  */
  YYSYMBOL_function_name = 262,            /* function_name  */
  YYSYMBOL_global_function_declaration = 263, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 264, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 265, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 266,     /* function_declaration  */
  YYSYMBOL_267_5 = 267,                    /* $@5  */
  YYSYMBOL_expression_block = 268,         /* expression_block  */
  YYSYMBOL_expression_any = 269,           /* expression_any  */
  YYSYMBOL_expressions = 270,              /* expressions  */
  YYSYMBOL_expr_keyword = 271,             /* expr_keyword  */
  YYSYMBOL_expression_keyword = 272,       /* expression_keyword  */
  YYSYMBOL_273_6 = 273,                    /* $@6  */
  YYSYMBOL_274_7 = 274,                    /* $@7  */
  YYSYMBOL_expr_pipe = 275,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 276,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 277,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 278,     /* new_type_declaration  */
  YYSYMBOL_279_8 = 279,                    /* $@8  */
  YYSYMBOL_280_9 = 280,                    /* $@9  */
  YYSYMBOL_expr_new = 281,                 /* expr_new  */
  YYSYMBOL_expression_break = 282,         /* expression_break  */
  YYSYMBOL_expression_continue = 283,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 284, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 285,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 286, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 287,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 288,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 289,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 290,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 291,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 292, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 293,           /* expression_let  */
  YYSYMBOL_expr_cast = 294,                /* expr_cast  */
  YYSYMBOL_295_10 = 295,                   /* $@10  */
  YYSYMBOL_296_11 = 296,                   /* $@11  */
  YYSYMBOL_297_12 = 297,                   /* $@12  */
  YYSYMBOL_298_13 = 298,                   /* $@13  */
  YYSYMBOL_299_14 = 299,                   /* $@14  */
  YYSYMBOL_300_15 = 300,                   /* $@15  */
  YYSYMBOL_expr_type_decl = 301,           /* expr_type_decl  */
  YYSYMBOL_302_16 = 302,                   /* $@16  */
  YYSYMBOL_303_17 = 303,                   /* $@17  */
  YYSYMBOL_expr_type_info = 304,           /* expr_type_info  */
  YYSYMBOL_expr_list = 305,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 306,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 307,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 308,            /* capture_entry  */
  YYSYMBOL_capture_list = 309,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 310,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 311,               /* expr_block  */
  YYSYMBOL_expr_numeric_const = 312,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 313,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 314,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 315,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 316,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 317,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 318,           /* func_addr_expr  */
  YYSYMBOL_319_18 = 319,                   /* $@18  */
  YYSYMBOL_320_19 = 320,                   /* $@19  */
  YYSYMBOL_321_20 = 321,                   /* $@20  */
  YYSYMBOL_322_21 = 322,                   /* $@21  */
  YYSYMBOL_expr_field = 323,               /* expr_field  */
  YYSYMBOL_324_22 = 324,                   /* $@22  */
  YYSYMBOL_325_23 = 325,                   /* $@23  */
  YYSYMBOL_expr = 326,                     /* expr  */
  YYSYMBOL_327_24 = 327,                   /* $@24  */
  YYSYMBOL_328_25 = 328,                   /* $@25  */
  YYSYMBOL_329_26 = 329,                   /* $@26  */
  YYSYMBOL_330_27 = 330,                   /* $@27  */
  YYSYMBOL_331_28 = 331,                   /* $@28  */
  YYSYMBOL_332_29 = 332,                   /* $@29  */
  YYSYMBOL_expr_mtag = 333,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 334, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 335,        /* optional_override  */
  YYSYMBOL_optional_constant = 336,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 337, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 338, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 339, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 340, /* struct_variable_declaration_list  */
  YYSYMBOL_341_30 = 341,                   /* $@30  */
  YYSYMBOL_342_31 = 342,                   /* $@31  */
  YYSYMBOL_343_32 = 343,                   /* $@32  */
  YYSYMBOL_function_argument_declaration = 344, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 345,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 346,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 347,          /* tuple_type_list  */
  YYSYMBOL_variant_type = 348,             /* variant_type  */
  YYSYMBOL_variant_type_list = 349,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 350,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 351,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 352,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 353,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 354,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 355, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 356, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 357, /* global_variable_declaration_list  */
  YYSYMBOL_358_33 = 358,                   /* $@33  */
  YYSYMBOL_optional_shared = 359,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 360, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 361,               /* global_let  */
  YYSYMBOL_362_34 = 362,                   /* $@34  */
  YYSYMBOL_enum_list = 363,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 364, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 365,             /* single_alias  */
  YYSYMBOL_366_35 = 366,                   /* $@35  */
  YYSYMBOL_alias_list = 367,               /* alias_list  */
  YYSYMBOL_alias_declaration = 368,        /* alias_declaration  */
  YYSYMBOL_369_36 = 369,                   /* $@36  */
  YYSYMBOL_optional_public_or_private_enum = 370, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 371,                /* enum_name  */
  YYSYMBOL_enum_declaration = 372,         /* enum_declaration  */
  YYSYMBOL_373_37 = 373,                   /* $@37  */
  YYSYMBOL_374_38 = 374,                   /* $@38  */
  YYSYMBOL_375_39 = 375,                   /* $@39  */
  YYSYMBOL_376_40 = 376,                   /* $@40  */
  YYSYMBOL_optional_structure_parent = 377, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 378,          /* optional_sealed  */
  YYSYMBOL_structure_name = 379,           /* structure_name  */
  YYSYMBOL_class_or_struct = 380,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 381, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 382,    /* structure_declaration  */
  YYSYMBOL_383_41 = 383,                   /* $@41  */
  YYSYMBOL_384_42 = 384,                   /* $@42  */
  YYSYMBOL_variable_name_with_pos_list = 385, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 386,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 387, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 388, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 389,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 390,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 391,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 392, /* bitfield_type_declaration  */
  YYSYMBOL_393_43 = 393,                   /* $@43  */
  YYSYMBOL_394_44 = 394,                   /* $@44  */
  YYSYMBOL_table_type_pair = 395,          /* table_type_pair  */
  YYSYMBOL_type_declaration_no_options = 396, /* type_declaration_no_options  */
  YYSYMBOL_397_45 = 397,                   /* $@45  */
  YYSYMBOL_398_46 = 398,                   /* $@46  */
  YYSYMBOL_399_47 = 399,                   /* $@47  */
  YYSYMBOL_400_48 = 400,                   /* $@48  */
  YYSYMBOL_401_49 = 401,                   /* $@49  */
  YYSYMBOL_402_50 = 402,                   /* $@50  */
  YYSYMBOL_403_51 = 403,                   /* $@51  */
  YYSYMBOL_404_52 = 404,                   /* $@52  */
  YYSYMBOL_405_53 = 405,                   /* $@53  */
  YYSYMBOL_406_54 = 406,                   /* $@54  */
  YYSYMBOL_407_55 = 407,                   /* $@55  */
  YYSYMBOL_408_56 = 408,                   /* $@56  */
  YYSYMBOL_409_57 = 409,                   /* $@57  */
  YYSYMBOL_410_58 = 410,                   /* $@58  */
  YYSYMBOL_411_59 = 411,                   /* $@59  */
  YYSYMBOL_412_60 = 412,                   /* $@60  */
  YYSYMBOL_413_61 = 413,                   /* $@61  */
  YYSYMBOL_414_62 = 414,                   /* $@62  */
  YYSYMBOL_415_63 = 415,                   /* $@63  */
  YYSYMBOL_416_64 = 416,                   /* $@64  */
  YYSYMBOL_417_65 = 417,                   /* $@65  */
  YYSYMBOL_418_66 = 418,                   /* $@66  */
  YYSYMBOL_419_67 = 419,                   /* $@67  */
  YYSYMBOL_420_68 = 420,                   /* $@68  */
  YYSYMBOL_type_declaration = 421,         /* type_declaration  */
  YYSYMBOL_variant_alias_declaration = 422, /* variant_alias_declaration  */
  YYSYMBOL_423_69 = 423,                   /* $@69  */
  YYSYMBOL_424_70 = 424,                   /* $@70  */
  YYSYMBOL_425_71 = 425,                   /* $@71  */
  YYSYMBOL_426_72 = 426,                   /* $@72  */
  YYSYMBOL_bitfield_alias_declaration = 427, /* bitfield_alias_declaration  */
  YYSYMBOL_428_73 = 428,                   /* $@73  */
  YYSYMBOL_429_74 = 429,                   /* $@74  */
  YYSYMBOL_430_75 = 430,                   /* $@75  */
  YYSYMBOL_431_76 = 431,                   /* $@76  */
  YYSYMBOL_make_decl = 432,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 433,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 434,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 435,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 436, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 437, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 438, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 439, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 440, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_make_struct_decl = 441,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 442,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 443,           /* make_map_tuple  */
  YYSYMBOL_make_dim = 444,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 445,            /* make_dim_decl  */
  YYSYMBOL_make_table = 446,               /* make_table  */
  YYSYMBOL_make_table_decl = 447,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 448, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 449       /* array_comprehension  */
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
#define YYLAST   11414

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  212
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  238
/* YYNRULES -- Number of rules.  */
#define YYNRULES  749
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1305

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   439


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
       2,     2,     2,   195,     2,   211,   206,   191,   184,     2,
     204,   205,   189,   188,   178,   187,   201,   190,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   181,   208,
     185,   179,   186,   180,   207,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   202,     2,   203,   183,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   209,   182,   210,   194,     2,     2,     2,
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
     175,   176,   177,   192,   193,   196,   197,   198,   199,   200
};

#if DAS_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   507,   507,   508,   513,   514,   515,   516,   517,   518,
     519,   520,   521,   522,   523,   524,   528,   534,   535,   536,
     540,   541,   545,   563,   564,   565,   566,   570,   574,   579,
     588,   596,   612,   617,   625,   625,   664,   685,   689,   690,
     694,   697,   701,   707,   716,   719,   725,   726,   730,   734,
     735,   739,   742,   748,   754,   757,   763,   764,   768,   769,
     770,   779,   780,   784,   785,   785,   791,   792,   793,   794,
     795,   799,   805,   805,   811,   817,   825,   835,   844,   844,
     851,   852,   853,   854,   855,   856,   860,   865,   873,   874,
     875,   879,   880,   881,   882,   883,   884,   885,   886,   892,
     895,   901,   902,   903,   907,   915,   928,   931,   939,   950,
     961,   972,   975,   982,   986,   993,   994,   998,   999,  1000,
    1004,  1007,  1014,  1018,  1019,  1020,  1021,  1022,  1023,  1024,
    1025,  1026,  1027,  1028,  1029,  1030,  1031,  1032,  1033,  1034,
    1035,  1036,  1037,  1038,  1039,  1040,  1041,  1042,  1043,  1044,
    1045,  1046,  1047,  1048,  1049,  1050,  1051,  1052,  1053,  1054,
    1055,  1056,  1057,  1058,  1059,  1060,  1061,  1062,  1063,  1064,
    1065,  1066,  1067,  1068,  1069,  1070,  1071,  1072,  1073,  1074,
    1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,  1084,
    1085,  1086,  1087,  1088,  1089,  1090,  1091,  1092,  1093,  1094,
    1095,  1096,  1097,  1098,  1099,  1100,  1101,  1106,  1128,  1129,
    1130,  1134,  1140,  1140,  1157,  1161,  1172,  1173,  1174,  1175,
    1176,  1177,  1178,  1179,  1180,  1181,  1182,  1183,  1184,  1185,
    1186,  1187,  1188,  1189,  1190,  1191,  1192,  1196,  1201,  1207,
    1213,  1224,  1224,  1224,  1235,  1269,  1272,  1275,  1281,  1282,
    1293,  1297,  1300,  1308,  1308,  1308,  1311,  1317,  1320,  1323,
    1327,  1333,  1337,  1341,  1344,  1347,  1355,  1358,  1361,  1369,
    1372,  1380,  1383,  1386,  1394,  1400,  1401,  1405,  1406,  1410,
    1414,  1421,  1427,  1439,  1442,  1448,  1448,  1448,  1451,  1451,
    1451,  1456,  1456,  1456,  1464,  1464,  1464,  1470,  1480,  1491,
    1506,  1509,  1515,  1516,  1523,  1534,  1535,  1536,  1540,  1541,
    1542,  1543,  1547,  1552,  1560,  1561,  1565,  1570,  1577,  1578,
    1579,  1580,  1581,  1582,  1583,  1587,  1588,  1589,  1590,  1591,
    1592,  1593,  1594,  1595,  1596,  1597,  1598,  1599,  1600,  1601,
    1602,  1603,  1604,  1605,  1609,  1610,  1611,  1612,  1613,  1614,
    1618,  1625,  1637,  1642,  1652,  1656,  1663,  1666,  1666,  1666,
    1671,  1671,  1671,  1684,  1688,  1692,  1692,  1692,  1699,  1700,
    1701,  1702,  1703,  1704,  1705,  1706,  1707,  1708,  1709,  1710,
    1711,  1712,  1713,  1714,  1715,  1716,  1717,  1718,  1719,  1720,
    1721,  1722,  1723,  1724,  1725,  1726,  1727,  1728,  1729,  1730,
    1731,  1732,  1733,  1734,  1740,  1741,  1742,  1743,  1744,  1745,
    1746,  1747,  1748,  1749,  1750,  1751,  1752,  1756,  1760,  1763,
    1766,  1767,  1768,  1769,  1772,  1775,  1776,  1779,  1779,  1779,
    1782,  1787,  1791,  1795,  1795,  1795,  1800,  1803,  1807,  1807,
    1807,  1812,  1815,  1816,  1817,  1818,  1819,  1820,  1821,  1822,
    1823,  1824,  1825,  1830,  1834,  1835,  1836,  1837,  1838,  1839,
    1840,  1844,  1848,  1852,  1856,  1860,  1864,  1868,  1872,  1876,
    1883,  1884,  1888,  1889,  1890,  1894,  1895,  1899,  1900,  1901,
    1905,  1906,  1910,  1921,  1924,  1924,  1943,  1942,  1957,  1956,
    1969,  1978,  1984,  1989,  1999,  2000,  2004,  2007,  2016,  2017,
    2021,  2030,  2031,  2035,  2038,  2041,  2055,  2056,  2060,  2066,
    2072,  2075,  2079,  2085,  2094,  2095,  2096,  2100,  2101,  2105,
    2112,  2117,  2126,  2132,  2143,  2146,  2151,  2156,  2164,  2175,
    2178,  2178,  2198,  2199,  2203,  2204,  2205,  2209,  2212,  2212,
    2231,  2234,  2237,  2252,  2271,  2272,  2273,  2278,  2278,  2300,
    2301,  2305,  2306,  2306,  2310,  2311,  2312,  2316,  2326,  2331,
    2326,  2343,  2348,  2343,  2363,  2364,  2368,  2369,  2373,  2379,
    2380,  2384,  2385,  2386,  2390,  2395,  2390,  2409,  2416,  2421,
    2430,  2436,  2447,  2448,  2449,  2450,  2451,  2452,  2453,  2454,
    2455,  2456,  2457,  2458,  2459,  2460,  2461,  2462,  2463,  2464,
    2465,  2466,  2467,  2468,  2469,  2470,  2471,  2472,  2473,  2477,
    2478,  2479,  2480,  2481,  2482,  2486,  2497,  2501,  2508,  2520,
    2527,  2536,  2541,  2544,  2557,  2557,  2557,  2570,  2574,  2581,
    2582,  2583,  2584,  2585,  2599,  2605,  2609,  2613,  2618,  2623,
    2628,  2633,  2637,  2641,  2646,  2650,  2654,  2659,  2659,  2659,
    2665,  2672,  2672,  2672,  2677,  2677,  2677,  2683,  2683,  2683,
    2688,  2692,  2692,  2692,  2697,  2697,  2697,  2706,  2710,  2710,
    2710,  2715,  2715,  2715,  2724,  2728,  2728,  2728,  2733,  2733,
    2733,  2742,  2742,  2742,  2748,  2748,  2748,  2757,  2760,  2771,
    2787,  2787,  2792,  2798,  2787,  2823,  2823,  2828,  2833,  2823,
    2863,  2864,  2865,  2866,  2870,  2877,  2884,  2890,  2896,  2903,
    2910,  2916,  2926,  2931,  2938,  2939,  2944,  2945,  2949,  2950,
    2954,  2955,  2959,  2960,  2961,  2965,  2966,  2967,  2972,  2978,
    2985,  2993,  3000,  3008,  3020,  3023,  3029,  3043,  3049,  3055,
    3060,  3067,  3072,  3082,  3087,  3094,  3106,  3107,  3111,  3114
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
  "\"finally\"", "\"delete\"", "\"deref\"", "\"typedef\"", "\"with\"",
  "\"aka\"", "\"assume\"", "\"cast\"", "\"override\"", "\"abstract\"",
  "\"upcast\"", "\"iterator\"", "\"var\"", "\"addr\"", "\"continue\"",
  "\"where\"", "\"pass\"", "\"reinterpret\"", "\"module\"", "\"public\"",
  "\"label\"", "\"goto\"", "\"implicit\"", "\"explicit\"", "\"shared\"",
  "\"private\"", "\"smart_ptr\"", "\"unsafe\"", "\"inscope\"",
  "\"static\"", "\"bool\"", "\"void\"", "\"string\"", "\"auto\"",
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
  "\"keyword\"", "\"start of the string\"", "STRING_CHARACTER",
  "STRING_CHARACTER_ESC", "\"end of the string\"", "\"{\"", "\"}\"",
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
  "expression_for_loop", "expression_unsafe", "expression_while_loop",
  "expression_with", "expression_with_alias", "$@4",
  "annotation_argument_value", "annotation_argument_value_list",
  "annotation_argument_name", "annotation_argument",
  "annotation_argument_list", "annotation_declaration_name",
  "annotation_declaration_basic", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@5", "expression_block", "expression_any",
  "expressions", "expr_keyword", "expression_keyword", "$@6", "$@7",
  "expr_pipe", "name_in_namespace", "expression_delete",
  "new_type_declaration", "$@8", "$@9", "expr_new", "expression_break",
  "expression_continue", "expression_return_no_pipe", "expression_return",
  "expression_yield_no_pipe", "expression_yield", "expression_try_catch",
  "kwd_let", "optional_in_scope", "tuple_expansion",
  "tuple_expansion_variable_declaration", "expression_let", "expr_cast",
  "$@10", "$@11", "$@12", "$@13", "$@14", "$@15", "expr_type_decl", "$@16",
  "$@17", "expr_type_info", "expr_list", "block_or_simple_block",
  "block_or_lambda", "capture_entry", "capture_list",
  "optional_capture_list", "expr_block", "expr_numeric_const",
  "expr_assign", "expr_assign_pipe", "expr_named_call", "expr_method_call",
  "func_addr_name", "func_addr_expr", "$@18", "$@19", "$@20", "$@21",
  "expr_field", "$@22", "$@23", "expr", "$@24", "$@25", "$@26", "$@27",
  "$@28", "$@29", "expr_mtag", "optional_field_annotation",
  "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@30", "$@31", "$@32",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "variant_type", "variant_type_list",
  "variant_alias_type_list", "copy_or_move", "variable_declaration",
  "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@33", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@34", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@35", "alias_list",
  "alias_declaration", "$@36", "optional_public_or_private_enum",
  "enum_name", "enum_declaration", "$@37", "$@38", "$@39", "$@40",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "structure_declaration", "$@41", "$@42", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_alias_bits", "bitfield_type_declaration", "$@43", "$@44",
  "table_type_pair", "type_declaration_no_options", "$@45", "$@46", "$@47",
  "$@48", "$@49", "$@50", "$@51", "$@52", "$@53", "$@54", "$@55", "$@56",
  "$@57", "$@58", "$@59", "$@60", "$@61", "$@62", "$@63", "$@64", "$@65",
  "$@66", "$@67", "$@68", "type_declaration", "variant_alias_declaration",
  "$@69", "$@70", "$@71", "$@72", "bitfield_alias_declaration", "$@73",
  "$@74", "$@75", "$@76", "make_decl", "make_struct_fields",
  "make_struct_dim", "optional_block",
  "optional_trailing_semicolon_cur_cur",
  "optional_trailing_semicolon_cur_sqr",
  "optional_trailing_semicolon_sqr_sqr", "optional_trailing_delim_sqr_sqr",
  "optional_trailing_delim_cur_sqr", "make_struct_decl", "make_tuple",
  "make_map_tuple", "make_dim", "make_dim_decl", "make_table",
  "make_table_decl", "array_comprehension_where", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1088)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-679)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1088,    30, -1088, -1088,    53,   -77,   132,   -97, -1088,  -108,
     401,   401,    85,    -8, -1088, -1088, -1088,   -82, -1088, -1088,
   -1088,   272, -1088,    98, -1088, -1088, -1088, -1088, -1088, -1088,
   -1088, -1088, -1088,   -14, -1088,    36,   113,   131, -1088, -1088,
   -1088,   132, -1088,    18, -1088,   401,   401, -1088, -1088,    98,
   -1088, -1088, -1088, -1088,    97,   161, -1088, -1088, -1088,    -8,
      -8,    -8,   162, -1088,   513,    86, -1088, -1088, -1088, -1088,
     478,   519,   524, -1088,   541,    19,    53,   223,   -77,   145,
     244, -1088,   199,   199, -1088,   252,   256,     5,   269,   543,
     392,   395, -1088,   398,   322, -1088, -1088,   -43,    53,    -8,
      -8,    -8,    -8, -1088, -1088, -1088, -1088, -1088, -1088, -1088,
     403, -1088, -1088, -1088, -1088, -1088,   380, -1088, -1088, -1088,
   -1088, -1088,   449,    93, -1088, -1088, -1088, -1088,   485, -1088,
   -1088, -1088, -1088, -1088,   383, -1088, -1088, -1088, -1088, -1088,
   -1088, -1088, -1088,   434, -1088,    55, -1088,   323,   484,   513,
   11250, -1088,    43,   526, -1088,   476, -1088, -1088,   441, -1088,
   -1088, -1088, -1088, -1088, -1088, -1088,   112, -1088,   458, -1088,
     426,   436, -1088, -1088,  9936, -1088, -1088, -1088, -1088, -1088,
   -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088,
   -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088,
     622,   631, -1088,   469,   465,   339, -1088, -1088,   511, -1088,
     466,    53,   264, -1088, -1088, -1088,    93, -1088, 11062, -1088,
   -1088,   514,   515, -1088, -1088, -1088, -1088, -1088, -1088, -1088,
   -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088,
   -1088, -1088, -1088,   516,   474, -1088, -1088, -1088, -1088, -1088,
   -1088, -1088, -1088, -1088, -1088, -1088, -1088,   661, -1088, -1088,
   -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088,
     529,   491, -1088, -1088,   -87,   517, -1088, -1088, -1088, -1088,
   -1088, -1088, -1088, -1088,   486, -1088,   520,   487, -1088,   476,
     187,   493,   659,   376, -1088, -1088,   521,   522,   523,   525,
   -1088, -1088, -1088,   505, -1088, -1088, -1088, -1088, -1088,   527,
   -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088,
   -1088,   528, -1088, -1088, -1088,   531,   534, -1088, -1088, -1088,
   -1088,   535,   536,   507, -1088, -1088, -1088, -1088, -1088,  1672,
     540, -1088, -1088, -1088, -1088, -1088, -1088,   559,   573, -1088,
     530, -1088,   -11, -1088,    22, 11062, -1088,  1892, -1088,   -99,
      85, -1088, -1088,   264,   532,  6913,   560,   561, 11062, -1088,
     108, -1088, -1088, -1088, -1088,   563, -1088,   226,   274,   307,
   -1088, -1088,  6913, -1088, -1088, -1088,    32, -1088, -1088, -1088,
      72,  3995, -1088,  7347,   -89,   -71, -1088, -1088,  6913,   350,
     533,   690, -1088,   -64, -1088,   -73,   540, -1088,  6913, -1088,
   -1088,   350, -1088, -1088,   233,   537,   547,  2833, -1088, -1088,
     465,  4154,   538,  6913,   569,   551,   554,   539, -1088,   542,
     562,   587,  4313,   314, -1088,   568,  2992,  6913,  6913,   -34,
     -34,   -34,   550,   552,   553,   555,   556,   557, -1088, 10704,
   10803,  6913, -1088, -1088, -1088, -1088, -1088, -1088, -1088,  4472,
   -1088,  6913,  6913,  6913,  6913,  6913,  6913, -1088,   564, -1088,
     710, -1088, -1088,   567,   571,  6913,   739, -1088, -1088, -1088,
   -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088,   -65,   576,
   -1088,   577,   578,   579, -1088,   580, -1088, -1088,   687, -1088,
   -1088, -1088, -1088,   565, -1088, -1088,   -83, -1088, -1088, -1088,
   -1088, -1088,  1336, -1088,   558, -1088, -1088, -1088, -1088, -1088,
   -1088,    46, -1088,   548, -1088,    24, -1088, -1088,   570,   592,
    7382, -1088,   722,  1202, -1088, -1088, -1088,  3336, 11062, 11062,
   11062, 11062,   585,   600, 11062,   469, 11062,   469, 11062,   469,
   11161,   608,  7488, -1088, -1088, -1088, -1088,   588, -1088, -1088,
    9462, -1088,  1672,   584, -1088,   583,   613, -1088,   589,   586,
    7523, -1088,  6913,   617, -1088,   636, -1088, 11062, -1088,  3336,
   -1088,   709,    23, -1088,   591, -1088, -1088,    85, -1088,  3336,
   -1088,   675,  9828,   789,  6913, 10282,  6913,   709,   625, -1088,
   -1088,  6913, -1088, -1088,   624,   649, 10282,  6913, -1088, 11062,
    3336, -1088,  9828,   329,   329,   601, -1088, -1088, -1088,  6913,
    6913,  6913,  6913,  6913,  6913,   350,  2287,   350,  2469,  9971,
   -1088,   -42, -1088,   709,   500,   329,   329,   -38,   329,   329,
    7629,   155,   602, -1088, -1088,   709, -1088, -1088,  3813, -1088,
   -1088, -1088, -1088, -1088, -1088,   247,   655,   -34, -1088, 10837,
   10932,  6913,  6913,  6913,  6913,  6913,  6913,  6913,  6913,  6913,
    6913, -1088, -1088,  6913,  6913,  6913,  6913,  6913,  6913,   646,
    3495,  6913,   205,  6913,  6913,  6913,  6913,  6913,  6913,  6913,
    6913,  6913,  6913,  6913,  6913,  6913,  6913,  6913,  3654,  4654,
    6913,  6913,  6913,  6913,  6913,  6913,  6913,  6913,  6913,  6913,
     -41,  6913,  4813,   -99,  6913, -1088, -1088,    -8, -1088,   805,
     476, -1088,   650, -1088,  3336, -1088,  1553,   317,   627,   120,
     355,   402, -1088, -1088,   118,   421,   517,   450,   517,   456,
     517,    -7, -1088,   213,   540, -1088,   250, -1088, -1088, -1088,
   -1088, -1088, 11062, -1088, -1088, -1088,  7664, -1088,   774,   -76,
   -1088,  9828, -1088,  6913, 11062,  4995,  5177, 11062, -1088,  9828,
    6913,  6913,   465, 10282,  7770, -1088, -1088, 11062, 11062,  7805,
   11062, -1088, -1088,  7911,   596, -1088,  9828, -1088,  7946,  8052,
    8087,  8193,  8228,  8334,    31,   -34,   612,     3,  2651,  5359,
   10066,   639,   -13,    -5,   647,    83,    33,  5541,   -13,   -72,
    6913, -1088,  6913,   614, -1088, 11062, -1088, -1088,  6913,   496,
   -1088,   632,   633,   331, -1088, -1088, -1088,   502,  6913,   265,
   -1088,   122, 10282,   671, -1088, -1088,   -63,   469, -1088,   653,
     640, -1088, -1088,   658,   641, -1088, -1088, 10282, 10282, 10282,
   10282, 10282, 10282, 10282, 10282,   470,   470,  1725, 10282, 10282,
    1725,    80,    80,   642,   -34,   -34,   -34, 10282,   385,   648,
   -1088,  9497,   -69,   -69, 10282,   470,   470, 10282, 10282, 10438,
     501,  1379, 10282, 10282, 10282, 10317,   -34,   -34,   -34, 10282,
   10963, 10161, 10524, 10552,   749,  1725,  1725,   123,   123,   385,
     385,   385,   319,  6913,   651, -1088,   352,  6913,   839,  9592,
   -1088,   137,   644,   983,   189,   544,   785,   643, -1088, -1088,
    7021, -1088, -1088, -1088, 11062, -1088, -1088, -1088,   694, -1088,
     679, -1088,   684, -1088,   689, 11062, -1088, 11161, -1088,   608,
     540, -1088,   712,  6913,   -30,   461, -1088,   166,   714,  8369,
     462, -1088, -1088,  6913,   798,   873, -1088,  1207, -1088,   655,
   -1088, -1088, -1088, -1088, -1088,   676,  6913, -1088,  6913,  6913,
    6913,   -10,  6913,   409,   265,    -5, -1088, -1088,   680, -1088,
    6913, -1088,   681,  6913, -1088,  6913,   265,   -22, -1088,   682,
   -1088, 10282, -1088, -1088,  1951, 10196,  6913,  6913, 11062,   469,
    2102,   465, -1088, -1088,  6913, -1088,   362,   141,   200,  5723,
   -1088, -1088,   220,   723,   724,   726,   737, -1088,   236,   517,
   -1088,  6913, -1088,  6913,  5890, -1088, -1088, -1088,  6913, -1088,
   -1088, -1088, -1088,   717,   699, -1088, -1088,  6913,   700, -1088,
    9687,  6913,   702, -1088,  9722, -1088, -1088,  6913, -1088, -1088,
   -1088,   701, -1088, -1088,    29, -1088,    26, -1088, -1088, -1088,
   -1088,   540, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088,
   -1088, -1088,   540, -1088, -1088, -1088, -1088, -1088, 10282,   465,
   -1088, -1088,   261, -1088, -1088, 10282, -1088, -1088, -1088,   703,
    6072,   -29,  8475, 10282, 10282,   -13,    -5, 10282,   704,   142,
     639, -1088, -1088, 10282, -1088,   647,    -1,   -13, -1088, -1088,
     711, -1088, -1088, -1088, -1088,  8510,  8616,  2399,   517, -1088,
   -1088,   709, -1088,   705,   716,   265, 10282,   751,   719, -1088,
   -1088, -1088, -1088,   -63,   721,   -96, 11062,  8651, 11062,  8757,
   -1088,   167,  8792, -1088,  6913, 10403,  6913, -1088,  8898,  6913,
   -1088, -1088, -1088,   885,    26, -1088, -1088,   544, -1088, -1088,
   -1088, -1088, -1088,  6913,   762, -1088,  6913,  6913,  6913,  6254,
   -1088,   169,  6913,   154,    -5, -1088,  6913,  6913,  6913,  6913,
     -22, -1088,  6913, -1088, -1088, -1088,   742,   502,  3177, -1088,
   -1088,   237, -1088,   124, -1088, -1088,  6436, -1088, -1088,  2581,
   -1088,   464, -1088, -1088, -1088, 11062,  8933,  9039, -1088,  9074,
   -1088, -1088,   885,   350,  9180,   744,  1725,  1725,  1725, -1088,
    9215, -1088,  7127,  6913,  6913, -1088,  9321, 10282, 10282,  7127,
   -1088,  1725,   147, -1088, -1088,  6595,  6754, -1088, -1088, -1088,
   -1088, -1088, 10282,   736, 11062,   108,  6913, 10317, -1088, -1088,
     480, -1088, -1088, -1088, 11250, -1088, -1088, -1088,  6913, -1088,
     891,   740, 10282, 10282,   157,   734, -1088,   147,  6913, 10282,
    6913, 10282, -1088,  1580,  6913, 10403, -1088, -1088, -1088,   738,
   11250,  9356,  6913,   745,  6913,  6913,   746, -1088, 10282, 10282,
    6913,  7162, -1088, -1088,   465, -1088, 10282, -1088, 10282, 10282,
   -1088,  7268, -1088, -1088, -1088
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   115,     1,   275,     0,     0,     0,   552,   276,     0,
     544,   544,     0,     0,    15,    14,     3,     0,    10,     9,
       8,     0,     7,   532,     6,    11,     5,     4,    12,    13,
      89,    90,    88,    97,    99,    36,    51,    48,    49,    38,
      39,     0,    40,    46,    37,   544,   544,    21,    20,   532,
     546,   545,   695,   690,     0,   248,    34,   102,   103,     0,
       0,     0,   104,   106,   113,     0,   101,    16,   570,   569,
     208,   554,   571,   533,   534,     0,     0,     0,     0,    41,
       0,    47,     0,     0,    44,     0,     0,   544,     0,    17,
       0,     0,   250,     0,     0,   112,   107,     0,     0,     0,
       0,     0,     0,   116,   210,   209,   212,   207,   556,   555,
       0,   573,   572,   574,   536,   535,   538,    95,    96,    93,
      94,    92,     0,     0,    91,   100,    52,    50,    46,    43,
      42,   547,   549,   551,     0,   553,    18,    19,    22,   696,
     691,   249,    32,    35,   111,     0,   108,   109,   110,   114,
       0,   557,     0,   566,   529,   470,    23,    24,     0,    84,
      85,    82,    83,    81,    80,    86,     0,    45,     0,   550,
       0,     0,    33,   105,     0,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
       0,     0,   122,   117,     0,     0,   558,   567,     0,   575,
     530,     0,     0,    25,    26,    27,     0,    98,     0,   697,
     692,   176,   177,   174,   125,   126,   128,   127,   129,   130,
     131,   132,   158,   159,   156,   157,   149,   160,   161,   150,
     147,   148,   175,   169,     0,   173,   162,   163,   164,   165,
     136,   137,   138,   133,   134,   135,   146,     0,   152,   153,
     151,   144,   145,   140,   139,   141,   142,   143,   124,   123,
     168,     0,   154,   155,   470,   120,   237,   213,   609,   612,
     610,   613,   611,   614,     0,   540,   564,     0,   537,   470,
       0,     0,   519,   517,   539,    87,     0,     0,     0,     0,
     582,   602,   583,   616,   584,   588,   589,   590,   591,   608,
     595,   596,   597,   598,   599,   600,   601,   603,   604,   605,
     606,   660,   587,   594,   607,   667,   674,   585,   592,   586,
     593,     0,     0,     0,   615,   629,   632,   630,   631,   687,
     548,   621,   503,   178,   179,   172,   167,   180,   170,   166,
       0,   118,     0,   494,     0,     0,   211,     0,   561,   559,
       0,   568,   483,     0,     0,     0,     0,     0,     0,   518,
       0,   651,   654,   657,   647,     0,   624,   661,   668,   675,
     681,   684,     0,   637,   642,   636,     0,   650,   646,   639,
       0,     0,   641,     0,   698,   693,   181,   171,     0,     0,
       0,   577,   491,   508,   119,   470,   121,   239,     0,    61,
      62,     0,   374,   375,     0,     0,     0,   263,   368,   261,
       0,     0,     0,     0,     0,     0,     0,     0,   262,     0,
       0,     0,     0,     0,   608,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   459,     0,
       0,     0,   318,   320,   319,   321,   322,   323,   324,     0,
      28,     0,     0,     0,     0,     0,     0,   305,   306,   216,
     214,   372,   371,     0,     0,     0,     0,   232,   227,   224,
     223,   225,   226,   316,   238,   218,   453,   217,   369,     0,
     445,    69,    70,    67,   230,    68,   231,   233,   278,   222,
     444,   443,   442,   115,   448,   370,     0,   219,   447,   446,
     415,   376,   325,   377,     0,   373,   700,   701,   702,   703,
     540,     0,   541,     0,   565,   484,   531,   471,     0,     0,
       0,   521,   522,     0,   515,   516,   514,     0,     0,     0,
       0,     0,     0,     0,     0,   117,     0,   117,     0,   117,
       0,     0,     0,   644,   645,   638,   640,     0,   643,   634,
       0,   689,   688,     0,   622,     0,     0,   504,     0,     0,
       0,   492,     0,     0,   507,     0,   506,     0,   509,     0,
     495,     0,     0,   253,   257,   256,   260,     0,   294,     0,
     267,     0,   264,     0,     0,   251,     0,     0,     0,   285,
     288,     0,   236,   291,     0,     0,    55,     0,    75,     0,
       0,   272,   269,   404,   405,   306,   247,   245,   246,     0,
       0,     0,     0,     0,     0,     0,   714,     0,     0,   738,
     743,     0,   241,     0,     0,   381,   380,   420,   379,   378,
       0,   307,     0,   234,   235,     0,    72,   451,     0,   221,
     228,   229,   266,   271,   277,     0,   314,     0,   220,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   406,   407,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     365,     0,     0,   562,     0,   542,   560,     0,   576,     0,
     470,   520,     0,   524,     0,   528,   325,     0,     0,   627,
       0,     0,   617,   619,     0,     0,   120,     0,   120,     0,
     120,   248,   498,     0,   496,   501,     0,   618,   635,   633,
     623,   699,     0,   505,   694,   493,     0,   579,   580,   510,
     513,   512,    76,     0,     0,     0,     0,     0,   268,   265,
       0,     0,     0,   252,     0,    77,    78,     0,     0,     0,
       0,    53,    54,     0,     0,   273,   270,   307,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   248,     0,     0,
     734,   712,   714,     0,   739,     0,     0,     0,   714,     0,
       0,   717,     0,     0,   745,     0,   240,    31,     0,    29,
     408,     0,     0,   357,   354,   356,   237,    58,     0,     0,
     416,     0,   300,     0,   284,   283,     0,   117,   244,     0,
       0,   431,   430,     0,     0,   432,   436,   335,   336,   338,
     337,   339,   329,   330,   331,   382,   383,   395,   340,   341,
     396,   393,   394,     0,     0,     0,     0,   327,   425,     0,
     413,     0,   449,   450,   328,   384,   385,   342,   343,   400,
     401,   402,   332,   333,   334,   403,     0,     0,     0,   326,
       0,     0,   398,   399,   397,   391,   392,   387,   386,   388,
     389,   390,     0,     0,     0,   363,     0,     0,     0,     0,
     418,     0,     0,     0,     0,   477,   480,     0,   523,   526,
     325,   527,   652,   655,     0,   658,   648,   625,     0,   662,
       0,   669,     0,   676,     0,     0,   682,     0,   685,     0,
     500,   578,     0,     0,     0,     0,   258,     0,     0,     0,
       0,   274,   421,     0,     0,     0,   422,     0,   452,   314,
     454,   455,   456,   457,   458,     0,     0,   715,     0,     0,
       0,   714,     0,     0,     0,     0,   723,   724,     0,   729,
       0,   721,     0,     0,   741,     0,     0,     0,   719,     0,
     742,   737,   744,   716,     0,     0,     0,     0,     0,   117,
       0,     0,    56,    57,     0,    71,    63,     0,     0,     0,
     417,   279,     0,     0,     0,     0,     0,   312,     0,   120,
     427,     0,   433,     0,     0,   349,   347,   348,     0,   411,
     346,   344,   345,     0,     0,   437,   441,     0,     0,   414,
       0,     0,     0,   364,     0,   366,   409,     0,   419,   563,
     543,   116,   478,   479,   480,   481,   472,   485,   525,   653,
     656,   628,   659,   649,   626,   620,   663,   665,   670,   672,
     677,   679,   497,   683,   499,   686,   502,   581,   511,     0,
     254,   259,     0,   297,   295,    79,   286,   289,   292,     0,
       0,     0,     0,   705,   704,   714,     0,   735,     0,     0,
     713,   728,   722,   736,   720,   740,     0,   714,   726,   727,
       0,   732,   718,   242,    30,     0,     0,     0,   120,   215,
      59,     0,    64,     0,     0,     0,   301,     0,     0,   310,
     311,   309,   308,     0,     0,     0,     0,     0,     0,     0,
     352,     0,     0,   438,     0,   426,     0,   412,     0,     0,
     410,   367,   490,   475,   472,   473,   474,   477,   666,   673,
     680,    74,   255,     0,     0,   296,     0,     0,     0,     0,
     460,     0,     0,     0,     0,   730,     0,     0,     0,     0,
       0,   725,     0,   355,   469,   358,     0,    58,     0,    73,
     350,     0,   280,   517,   313,   315,     0,   302,   317,     0,
     468,     0,   466,   353,   463,     0,     0,     0,   462,     0,
     476,   486,   475,     0,     0,     0,   287,   290,   293,   423,
       0,   461,   746,     0,     0,   731,     0,   707,   706,   746,
     733,   243,     0,   361,    60,   263,     0,    65,    69,    70,
      67,    68,    66,     0,     0,     0,     0,   303,   428,   434,
       0,   467,   465,   464,     0,   488,   482,   298,     0,   424,
       0,     0,   709,   708,     0,     0,   359,     0,     0,   264,
       0,   269,   351,     0,     0,   304,   429,   435,   439,     0,
       0,     0,     0,     0,     0,     0,     0,   362,   265,   270,
       0,     0,   440,   487,     0,   299,   747,   748,   711,   710,
     749,     0,   282,   489,   281
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1088, -1088, -1088, -1088, -1088, -1088,   311,   872, -1088, -1088,
   -1088,   952, -1088, -1088, -1088,   437,   913, -1088,   827, -1088,
   -1088,   878, -1088, -1088, -1088,  -230, -1088, -1088, -1088,  -229,
   -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088,   747, -1088,
   -1088,   882,   -54, -1088, -1088,   325,   243,   -55,  -534,  -724,
   -1088, -1088, -1088,  -903, -1088, -1088,  -202, -1088,   135, -1088,
   -1088, -1088, -1088,  -390,   -12, -1088, -1088, -1088, -1088, -1088,
    -226,  -223,  -222, -1088,  -221, -1088, -1088,   967, -1088, -1088,
   -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088,
   -1088, -1088, -1088,  -686, -1088, -1088,  -164, -1088,    11,  -420,
   -1088,  -351, -1088, -1088, -1088, -1087, -1088, -1088, -1088, -1088,
   -1088, -1088, -1088, -1088,   607, -1088, -1088, -1088, -1088, -1088,
   -1088, -1088,  -148,  -183,  -239,  -182,   -80, -1088, -1088, -1088,
   -1088, -1088,   572, -1088,    39, -1088,  -543, -1088, -1088,  -400,
    -395,  -528,  -214, -1088,  -339, -1088, -1088,   931, -1088, -1088,
   -1088,   463,   566,    64, -1088, -1088, -1088, -1088, -1088, -1088,
   -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088,
   -1088, -1088, -1088,  -383,  -209, -1088,   574, -1088, -1088, -1088,
   -1088, -1088, -1088, -1088,  -358, -1088, -1088, -1088, -1088, -1088,
   -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088,
   -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088,  -186,
   -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088, -1088,
     593,  -807,  -610,  -785, -1088, -1088, -1088,  -946,  -199, -1088,
       4,   178,   364, -1088, -1088, -1088,  -236, -1088
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    15,   138,    49,    16,   158,   164,   634,   471,
     143,   472,    94,    18,    19,    42,    43,    44,    84,    20,
      37,    38,   473,   474,  1004,  1005,   475,  1123,  1188,   476,
     477,   828,   478,   479,   480,   481,   482,   953,   165,   166,
      33,    34,    35,    62,    63,    64,    65,    21,   275,   356,
     203,    22,   106,   204,   107,   150,   483,   484,   357,   485,
     486,   815,  1182,   487,   488,   489,   584,   764,  1162,   490,
     491,   492,   493,   494,   495,   496,   497,   498,   655,  1012,
     834,   499,   500,   777,  1166,   778,  1167,   780,  1168,   501,
     767,  1165,   502,   831,  1198,   503,  1017,  1018,   837,   504,
     505,   591,   507,   508,   509,   825,   510,   998,  1232,   999,
    1267,   511,   908,  1151,   832,  1136,  1276,  1138,  1277,  1205,
    1292,   513,   352,  1157,  1211,  1054,  1056,   917,   525,   720,
    1254,  1280,   353,   354,   742,   743,   568,   746,   395,   970,
     402,   537,   370,   293,   294,   210,   289,    74,   116,    24,
     155,   359,    85,    86,   168,    87,    25,    46,   110,   152,
      26,   285,   523,   520,   912,   361,   208,   209,    72,   113,
      27,   153,   287,   403,   514,   284,   336,   337,   734,   394,
     338,   543,  1064,   728,   339,   541,  1063,   538,  1059,   539,
    1060,   540,  1062,   544,  1066,   545,  1158,   546,  1068,   547,
    1159,   548,  1070,   549,  1160,   550,  1073,   551,  1075,   744,
      28,    91,   171,   342,   569,    29,    90,   170,   341,   565,
     515,   801,   802,   803,   814,   990,   984,   979,  1111,   516,
     804,   630,   805,   517,   631,   518,  1261,   519
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      56,    66,   277,   579,   571,   724,   506,   212,   745,   335,
     533,   736,   930,   738,   932,   740,   934,   975,   808,   616,
     617,   618,  1008,   987,   526,    57,   911,   590,   582,  1101,
       2,  -115,   340,   117,   118,   562,     3,   399,   795,    80,
    1196,   795,   763,   657,   145,   574,   611,    66,    66,    66,
     966,   679,   985,    58,   682,   683,    47,   574,  1013,     4,
      50,     5,   350,     6,   553,   521,    51,   211,  1014,     7,
    1155,    30,    31,    81,  1153,   563,   350,   944,     8,   947,
      36,   211,   902,   903,     9,   682,   683,    66,    66,    66,
      66,   626,   628,   566,  1055,    99,   100,   101,    48,   647,
     659,   660,   988,   576,   555,    10,   393,   159,   160,   522,
      88,   904,    45,   276,   575,   576,  1015,   577,   351,   564,
     578,  1016,    59,   905,   574,   658,    67,  1156,    11,    54,
      93,   811,   710,   711,   969,   400,   983,   567,   989,   648,
      93,   363,   796,   659,   660,  1266,   335,   725,  1047,  1047,
    1175,   134,  1108,   401,  1007,  1109,    55,   290,    73,   335,
     906,   907,   144,   710,   711,    75,   812,  1100,   813,   406,
     976,   977,   467,   615,   935,   276,   119,  1047,  1079,  1172,
    1287,   120,   576,   121,   335,   122,  1096,    60,  1110,   760,
     669,   670,   671,   672,   673,   974,    61,   676,   978,   768,
     679,   575,   681,   682,   683,   684,   334,  1179,    82,   575,
     685,   575,   687,   688,    76,   133,   554,    32,   593,    83,
     785,    12,    54,   123,   205,   714,   717,   404,  1225,   534,
     405,   608,    13,    76,   718,   671,   672,   838,    14,   535,
     335,   335,   794,   679,   806,   681,   682,   683,   684,    55,
     161,   784,   206,   685,   715,   162,   556,   163,   981,   122,
     173,    92,   574,   574,   102,   703,   704,   705,   706,   707,
     708,   709,   969,  1177,   557,   574,    68,    69,   574,    70,
    1091,   710,   711,   558,    54,  1223,   982,   536,  1284,   103,
     216,   983,    54,   821,    77,  1135,    39,    40,    93,  1106,
    1009,   821,   393,  1019,   927,  1244,   822,    71,   369,    78,
    1174,    55,   707,   708,   709,  1047,   835,   217,  1191,    55,
     576,   576,  1180,    41,   710,   711,   928,  1010,   924,   335,
     335,   335,   335,   576,   919,   335,   576,   335,  1141,   335,
     823,   335,  1048,   334,  1047,  1047,    83,  1047,   524,   659,
     660,  1279,   727,   729,   730,   731,   334,   869,   735,   943,
     737,  1095,   739,    39,    40,    76,    98,   102,   335,   870,
      54,  1081,  1203,  1122,  1221,   967,  1107,  1294,   973,   762,
     126,   334,   659,   660,    95,    96,    97,   449,   450,   451,
     364,   759,  1051,   291,  1186,   775,  1076,    55,  1127,   936,
     335,   833,   334,  1124,  1171,   659,   660,  -664,   128,   278,
     291,   292,  -664,   279,  1133,   973,   131,   796,   583,   954,
     955,   937,   957,  1128,   146,   147,   148,   149,   292,  1007,
    -664,   816,   280,   281,   282,   283,   938,   334,   334,  1134,
    1243,   671,   672,   827,  1025,  1026,  1027,  1163,   656,   679,
     842,   846,   682,   683,   684,  -671,    50,   994,   939,   685,
    -671,    99,    51,   101,   132,  1118,  1030,  1031,  1032,  1164,
     719,  1038,   669,   670,   671,   672,   673,   135,  -671,   676,
     677,   678,   679,  1039,   681,   682,   683,   684,  -678,   142,
     659,   660,   685,  -678,   687,   688,   400,   671,   672,   393,
     691,   692,   693,   922,  1042,   679,   697,   681,   682,   683,
     684,  -678,  -360,  1001,   401,   685,  1043,  -360,   607,   129,
     130,   659,   660,   276,  1002,  1003,   334,   334,   334,   334,
     710,   711,   334,   104,   334,  -360,   334,   393,   334,   105,
      81,   925,   699,   335,   700,   701,   702,   703,   704,   705,
     706,   707,   708,   709,   367,   335,   139,   368,   335,   140,
     369,  1098,   141,   710,   711,   334,   940,   151,   335,   335,
     951,   335,   916,  1099,   108,   766,    52,    53,   945,   111,
     109,   950,   671,   672,   393,   112,   710,   711,   926,   154,
     679,   169,   681,   682,   683,   684,   114,   334,   136,  1052,
     685,   172,   115,   393,   137,  1053,   335,   929,   213,   214,
     215,   669,   670,   671,   672,   673,   156,   157,   676,   677,
     678,   679,    99,   681,   682,   683,   684,   207,   383,   824,
     211,   685,   393,   687,   688,   219,   931,   218,   393,   691,
    1117,   693,   933,   393,   393,   220,   393,  1080,  1084,   506,
    1249,    99,   100,   101,   384,   385,   272,   705,   706,   707,
     708,   709,   393,   213,   214,   273,  1278,   156,   157,   817,
     818,   710,   711,   274,   276,   286,   288,   346,   343,   344,
     345,  1036,   347,   700,   701,   702,   703,   704,   705,   706,
     707,   708,   709,   348,   349,   358,   362,   365,   355,  1178,
     366,   360,   710,   711,   397,    66,   371,   372,   373,   375,
     374,   382,   376,   377,   386,   335,   378,  1274,   387,   379,
     380,   381,   393,   396,   531,   532,   335,   542,   335,   659,
     660,   573,   588,   598,   398,   527,   599,   572,  1061,   600,
     334,   587,   596,   601,   604,  1290,   642,   603,   646,  1072,
     602,   654,   334,   609,   619,   334,   620,   621,   716,   622,
     623,   624,   712,   722,   733,   334,   334,    13,   334,   659,
     660,   641,   566,  1224,   607,   643,   388,   632,  1199,   644,
     389,   757,   959,   390,   649,   650,   651,   652,   653,   335,
     732,   748,   750,   751,   752,   765,   754,   753,   391,  1120,
     758,   657,   772,   334,   776,   781,   782,   392,   787,   836,
     863,   826,   915,   923,   918,   942,   968,   973,  1256,   669,
     670,   671,   672,   673,   993,   980,   676,   677,   678,   679,
     383,   681,   682,   683,   684,  1011,   996,   997,  1020,   685,
    1045,   687,   688,  1022,  1021,  1023,  1024,   691,   692,   693,
    1055,  1057,  1028,   697,  1049,  1041,   384,   385,  1065,   669,
     670,   671,   672,   673,  1285,  1067,   676,   677,   678,   679,
    1069,   681,   682,   683,   684,  1071,  1077,  1161,  1082,   685,
    1090,   687,   688,  1102,  1104,  1112,  1273,  1129,  1130,   699,
    1131,   700,   701,   702,   703,   704,   705,   706,   707,   708,
     709,  1132,  1143,  1144,  1146,   383,  1149,  1169,  1176,  1152,
     710,   711,   334,  1189,  1181,  1192,   386,  1210,   276,  1187,
     387,  1190,  1193,   334,  1195,   334,  1215,   335,  1233,   335,
    1258,   384,   385,  1197,   703,   704,   705,   706,   707,   708,
     709,  1272,  1282,  1283,  1286,   819,  1293,   124,  1297,  1300,
     710,   711,  1201,    17,    79,   167,   127,  1234,   125,  1237,
     914,  1000,  1238,   295,   512,  1239,  1240,  1241,    23,  1194,
    1089,  1212,   530,  1255,  1154,  1213,  1074,   580,   388,  1245,
      89,  1230,   389,   713,  1086,   390,   334,  1105,   585,   552,
     992,   386,   809,  1265,     0,   387,   335,     0,   560,     0,
     391,     0,     0,   659,   660,   570,     0,   586,     0,   392,
       0,     0,     0,     0,     0,   581,     0,     0,     0,  1250,
       0,     0,     0,     0,   592,     0,     0,     0,   595,     0,
     597,     0,     0,     0,     0,   335,     0,     0,     0,   606,
       0,     0,     0,   612,   613,   614,     0,     0,     0,     0,
       0,     0,     0,   388,     0,     0,     0,   389,   629,  1087,
     390,     0,     0,     0,     0,     0,   633,     0,   635,   636,
     637,   638,   639,   640,     0,   391,     0,     0,     0,     0,
       0,     0,   645,     0,   392,     0,     0,     0,     0,     0,
       0,     0,  1303,   669,   670,   671,   672,   673,     0,     0,
     676,   677,   678,   679,     0,   681,   682,   683,   684,     0,
       0,     0,     0,   685,     0,   687,   688,     0,     0,     0,
       0,   691,   692,   693,   334,     0,   334,   697,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   726,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   699,     0,   700,   701,   702,   703,   704,
     705,   706,   707,   708,   709,     0,     0,     0,     0,   756,
       0,     0,     0,     0,   710,   711,   761,     0,     0,     0,
       0,  1050,     0,   334,     0,     0,   769,     0,     0,     0,
       0,   773,     0,   774,     0,     0,     0,     0,   779,     0,
       0,     0,     0,     0,   783,     0,     0,   786,     0,     0,
     824,     0,     0,     0,     0,     0,   788,   789,   790,   791,
     792,   793,   334,   800,   383,   800,     0,     0,     0,   383,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   824,     0,     0,     0,     0,
     384,   385,     0,     0,     0,   384,   385,     0,   847,   848,
     849,   850,   851,   852,   853,   854,   855,   856,     0,     0,
     857,   858,   859,   860,   861,   862,     0,   867,   868,     0,
     871,   872,   873,   874,   875,   876,   877,   878,   879,   880,
     881,   882,   883,   884,   885,   889,   891,   892,   893,   894,
     895,   896,   897,   898,   899,   900,   901,     0,   909,     0,
     386,   913,     0,   534,   387,   386,     0,     0,     0,   387,
       0,   920,     0,   535,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   -66,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   659,   660,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   949,     0,     0,     0,   867,   889,     0,
       0,   536,   388,     0,     0,     0,   389,   388,     0,   390,
       0,   389,     0,  1088,   390,     0,     0,     0,     0,   659,
     660,     0,     0,     0,   391,   635,   640,     0,     0,   391,
     723,     0,     0,   392,   640,     0,     0,   991,   392,   629,
       0,     0,     0,     0,     0,   995,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1006,     0,     0,   661,   662,
     663,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,   677,   678,   679,   680,   681,   682,
     683,   684,     0,     0,     0,     0,   685,   686,   687,   688,
     689,   690,     0,     0,   691,   692,   693,   694,   695,   696,
     697,     0,     0,     0,     0,     0,     0,     0,     0,   669,
     670,   671,   672,   673,     0,     0,   676,   677,   678,   679,
       0,   681,   682,   683,   684,     0,     0,     0,     0,   685,
    1040,   687,   688,     0,  1044,   698,   699,   691,   700,   701,
     702,   703,   704,   705,   706,   707,   708,   709,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   710,   711,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1078,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1085,   700,   701,   702,   703,   704,   705,   706,   707,   708,
     709,     0,     0,   659,   660,  1092,  1093,  1094,     0,  1097,
     710,   711,     0,     0,     0,     0,     0,  1103,     0,     0,
     800,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1115,  1116,     0,     0,   512,     0,     0,
       0,  1121,   383,     0,     0,     0,  1126,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1137,     0,
    1139,     0,     0,     0,     0,  1142,     0,     0,   384,   385,
       0,     0,     0,     0,  1145,     0,     0,     0,  1148,     0,
       0,     0,     0,     0,  1126,   661,   662,   663,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,   677,   678,   679,   770,   681,   682,   683,   684,     0,
       0,     0,     0,   685,   686,   687,   688,   689,   690,     0,
       0,   691,   692,   693,   694,   695,   696,   697,   386,     0,
       0,   534,   387,     0,   383,     0,     0,     0,     0,     0,
       0,   535,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     384,   385,   771,   699,     0,   700,   701,   702,   703,   704,
     705,   706,   707,   708,   709,   659,   660,     0,     0,     0,
       0,  1206,     0,  1207,   710,   711,  1209,     0,     0,   536,
     388,   921,     0,     0,   389,     0,     0,   390,     0,     0,
    1214,     0,     0,  1216,  1217,  1218,  1220,     0,     0,  1222,
       0,     0,   391,  1226,  1227,  1228,  1229,     0,     0,  1231,
     386,   392,     0,     0,   387,  1242,     0,     0,     0,     0,
       0,     0,     0,  1247,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1262,  1263,     0,     0,     0,   669,   670,   671,   672,     0,
       0,     0,  1269,  1271,     0,   679,     0,   681,   682,   683,
     684,     0,   388,  1275,     0,   685,   389,   687,   688,   390,
       0,     0,     0,     0,     0,  1281,     0,     0,     0,     0,
       0,     0,     0,     0,   391,  1288,     0,  1289,     0,     0,
       0,  1291,     0,   392,     0,     0,     0,     0,     0,  1296,
       0,  1298,  1299,   407,     0,     0,     0,  1301,     3,     0,
     408,   409,   410,     0,   411,     0,   412,   413,   414,   415,
     416,     0,   705,   706,   707,   708,   709,   417,   418,   419,
     420,     0,     0,     0,     0,     0,   710,   711,     0,   421,
     422,     0,   423,     0,   424,   425,     0,     0,   426,     0,
       8,   427,   428,     0,   429,   430,     0,     0,   431,   432,
       0,     0,     0,     0,     0,   433,     0,     0,   300,   301,
     302,     0,   304,   305,   306,   307,   308,   434,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,     0,
     322,   323,   324,   383,     0,   327,   328,   329,   330,     0,
       0,   435,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,   438,     0,     0,     0,   384,
     385,     0,     0,     0,     0,     0,     0,     0,     0,   439,
     440,   441,     0,     0,     0,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,   442,   443,   444,
     445,   446,     0,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,    55,   459,   460,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   386,
       0,     0,     0,   387,     0,     0,     0,     0,     0,   461,
     462,   463,     0,    12,     0,     0,   464,   465,     0,     0,
       0,     0,     0,     0,     0,     0,   466,     0,   467,   468,
     469,   276,   470,   407,     0,     0,     0,     0,     3,     0,
     408,   409,   410,     0,   411,     0,   412,   413,   414,   415,
     416,     0,     0,     0,     0,     0,     0,   417,   418,   419,
     420,   388,     0,     0,     0,   389,     0,  1113,   390,   421,
     422,     0,   423,     0,   424,   425,     0,     0,   426,     0,
       8,   427,   428,   391,   429,   430,     0,     0,   431,   432,
       0,     0,   392,     0,     0,   433,     0,     0,   300,   301,
     302,     0,   304,   305,   306,   307,   308,   434,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,     0,
     322,   323,   324,     0,     0,   327,   328,   329,   330,     0,
       0,   435,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,   438,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   439,
     440,   441,     0,     0,     0,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,   442,   443,   444,
     445,   446,     0,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,    55,   459,   460,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   461,
     462,   463,     0,    12,     0,     0,   464,   465,     0,     0,
       0,   412,   413,   414,   415,   416,   466,     0,   467,   468,
     469,   276,  1119,   418,     0,     0,     0,     0,     0,   383,
       0,     0,     0,     0,     0,   422,     0,     0,     0,     0,
     425,     0,     0,   426,     0,     0,   427,     0,   795,     0,
     430,     0,     0,     0,     0,   384,   385,     0,     0,     0,
     528,     0,     0,   300,   301,   302,     0,   304,   305,   306,
     307,   308,   434,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,     0,   322,   323,   324,     0,     0,
     327,   328,   329,   330,     0,     0,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   437,
     438,     0,     0,     0,     0,   386,     0,     0,     0,   387,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,   383,   442,   443,   444,   445,   446,     0,   447,   796,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   797,   529,   460,     0,     0,     0,   384,   385,     0,
       0,     0,     0,     0,     0,     0,     0,   388,     0,     0,
       0,   389,     0,     0,   798,   462,   463,     0,    12,     0,
       0,   464,   465,   412,   413,   414,   415,   416,     0,   391,
       0,   799,     0,   467,   468,   418,   276,     0,   392,     0,
       0,   383,     0,     0,     0,     0,     0,   422,     0,     0,
       0,     0,   425,     0,     0,   426,     0,   386,   427,     0,
       0,   387,   430,     0,     0,     0,     0,   384,   385,     0,
       0,     0,   528,     0,     0,   300,   301,   302,     0,   304,
     305,   306,   307,   308,   434,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,     0,   322,   323,   324,
       0,     0,   327,   328,   329,   330,     0,     0,   435,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   388,
       0,   437,   438,   389,     0,  1185,   390,   386,     0,     0,
       0,   387,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   391,     0,     0,     0,     0,    54,     0,     0,     0,
     392,     0,     0,   383,   442,   443,   444,   445,   446,     0,
     447,   796,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   797,   529,   460,     0,     0,     0,   384,
     385,     0,     0,     0,     0,     0,     0,     0,     0,   388,
       0,     0,     0,   389,     0,     0,   798,   462,   463,     0,
      12,     0,     0,   464,   465,   412,   413,   414,   415,   416,
       0,   391,     0,   807,     0,   467,   468,   418,   276,     0,
     392,     0,     0,   555,     0,     0,     0,     0,     0,   422,
       0,     0,     0,     0,   425,     0,     0,   426,     0,   386,
     427,     0,     0,   387,   430,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   528,     0,     0,   300,   301,   302,
       0,   304,   305,   306,   307,   308,   434,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,     0,   322,
     323,   324,     0,     0,   327,   328,   329,   330,     0,     0,
     435,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   388,     0,   437,   438,   389,     0,  1248,   390,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   391,     0,     0,     0,     0,    54,     0,
       0,     0,   392,     0,     0,     0,   442,   443,   444,   445,
     446,     0,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,    55,   529,   460,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   556,     0,     0,   461,   462,
     463,     0,    12,     0,     0,   464,   465,   412,   413,   414,
     415,   416,     0,   557,     0,   466,     0,   467,   468,   418,
     276,     0,   558,     0,     0,     0,     0,     0,     0,     0,
       0,   422,     0,     0,     0,     0,   425,     0,     0,   426,
       0,     0,   427,     0,     0,     0,   430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   528,     0,     0,   300,
     301,   302,     0,   304,   305,   306,   307,   308,   434,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
       0,   322,   323,   324,     0,     0,   327,   328,   329,   330,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,   438,     0,     0,     0,
       0,     0,     0,     0,   589,     0,     0,     0,     0,     0,
     439,   440,   441,     0,     0,     0,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,   442,   443,
     444,   445,   446,     0,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,    55,   529,   460,
       0,     0,     0,     0,     0,     0,   412,   413,   414,   415,
     416,     0,     0,     0,     0,     0,     0,     0,   418,     0,
     461,   462,   463,     0,    12,     0,     0,   464,   465,     0,
     422,     0,     0,     0,     0,   425,     0,   466,   426,   467,
     468,   427,   276,     0,     0,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   528,     0,     0,   300,   301,
     302,     0,   304,   305,   306,   307,   308,   434,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,     0,
     322,   323,   324,     0,     0,   327,   328,   329,   330,     0,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,   438,     0,     0,     0,     0,
       0,     0,     0,   610,     0,     0,     0,     0,     0,   439,
     440,   441,     0,     0,     0,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,   442,   443,   444,
     445,   446,     0,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,    55,   529,   460,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   461,
     462,   463,     0,    12,     0,     0,   464,   465,     0,     0,
       0,   412,   413,   414,   415,   416,   466,     0,   467,   468,
       0,   276,  1235,   418,   419,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   422,     0,     0,     0,     0,
     425,     0,     0,   426,     0,     0,   427,   428,     0,     0,
     430,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     528,     0,     0,   300,   301,   302,     0,   304,   305,   306,
     307,   308,   434,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,     0,   322,   323,   324,     0,     0,
     327,   328,   329,   330,     0,     0,   435,  1236,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   437,
     438,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,   442,   443,   444,   445,   446,     0,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,    55,   529,   460,     0,     0,     0,     0,     0,     0,
     412,   413,   414,   415,   416,     0,     0,     0,     0,     0,
       0,     0,   418,     0,   461,   462,   463,     0,    12,     0,
       0,   464,   465,     0,   422,     0,     0,     0,     0,   425,
       0,   466,   426,   467,   468,   427,   276,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   528,
       0,     0,   300,   301,   302,     0,   304,   305,   306,   307,
     308,   434,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,     0,   322,   323,   324,     0,     0,   327,
     328,   329,   330,     0,     0,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437,   438,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   439,   440,   441,     0,     0,     0,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       0,   442,   443,   444,   445,   446,     0,   447,     0,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
      55,   529,   460,     0,     0,     0,     0,     0,     0,   412,
     413,   414,   415,   416,     0,     0,     0,     0,     0,     0,
       0,   418,     0,   461,   462,   463,     0,    12,     0,     0,
     464,   465,     0,   422,     0,     0,     0,     0,   425,     0,
     466,   426,   467,   468,   427,   276,     0,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   528,     0,
       0,   300,   301,   302,     0,   304,   305,   306,   307,   308,
     434,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,     0,   322,   323,   324,     0,     0,   327,   328,
     329,   330,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   437,   438,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   864,   865,   866,     0,     0,     0,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
     442,   443,   444,   445,   446,     0,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,    55,
     529,   460,     0,     0,     0,     0,     0,     0,   412,   413,
     414,   415,   416,     0,     0,     0,     0,     0,     0,     0,
     418,     0,   461,   462,   463,     0,    12,     0,     0,   464,
     465,     0,   422,     0,     0,     0,     0,   425,     0,   466,
     426,   467,   468,   427,   276,     0,     0,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   528,     0,     0,
     300,   301,   302,     0,   304,   305,   306,   307,   308,   434,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,     0,   322,   323,   324,     0,     0,   327,   328,   329,
     330,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   437,   438,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   886,   887,   888,     0,     0,     0,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,   442,
     443,   444,   445,   446,     0,   447,     0,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,    55,   529,
     460,     0,     0,     0,     0,     0,     0,   412,   413,   414,
     415,   416,     0,     0,     0,     0,     0,     0,     0,   418,
       0,   461,   462,   463,     0,    12,     0,     0,   464,   465,
       0,   422,     0,     0,     0,     0,   425,     0,   466,   426,
     467,   468,   427,   276,     0,     0,   430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   528,     0,     0,   300,
     301,   302,     0,   304,   305,   306,   307,   308,   434,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
       0,   322,   323,   324,     0,     0,   327,   328,   329,   330,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,   438,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,   442,   443,
     444,   445,   446,     0,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,    55,   529,   460,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     461,   462,   463,     0,    12,     0,     0,   464,   465,   412,
     413,   414,   415,   416,     0,   829,     0,   466,   830,   467,
     468,   418,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   422,     0,     0,     0,     0,   425,     0,
       0,   426,     0,     0,   427,     0,     0,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   528,     0,
       0,   300,   301,   302,     0,   304,   305,   306,   307,   308,
     434,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,     0,   322,   323,   324,     0,     0,   327,   328,
     329,   330,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   437,   438,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
     442,   443,   444,   445,   446,     0,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,    55,
     529,   460,     0,     0,     0,     0,     0,     0,   412,   413,
     414,   415,   416,     0,     0,     0,     0,     0,     0,     0,
     418,     0,   461,   462,   463,     0,    12,     0,     0,   464,
     465,     0,   422,     0,     0,     0,     0,   425,   559,   466,
     426,   467,   468,   427,   276,     0,     0,   430,     0,     0,
       0,     0,     0,   594,     0,     0,     0,   528,     0,     0,
     300,   301,   302,     0,   304,   305,   306,   307,   308,   434,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,     0,   322,   323,   324,     0,     0,   327,   328,   329,
     330,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   437,   438,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,   442,
     443,   444,   445,   446,     0,   447,     0,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,    55,   529,
     460,     0,     0,     0,     0,     0,     0,   412,   413,   414,
     415,   416,     0,     0,     0,     0,     0,     0,     0,   418,
       0,   461,   462,   463,     0,    12,     0,     0,   464,   465,
       0,   422,     0,     0,     0,     0,   425,     0,   466,   426,
     467,   468,   427,   276,     0,     0,   430,     0,     0,   605,
       0,     0,     0,     0,     0,     0,   528,     0,     0,   300,
     301,   302,     0,   304,   305,   306,   307,   308,   434,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
       0,   322,   323,   324,     0,     0,   327,   328,   329,   330,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,   438,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,   442,   443,
     444,   445,   446,     0,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,    55,   529,   460,
       0,     0,     0,     0,     0,     0,   412,   413,   414,   415,
     416,     0,     0,     0,     0,     0,     0,     0,   418,     0,
     461,   462,   463,     0,    12,     0,     0,   464,   465,     0,
     422,     0,     0,     0,     0,   425,     0,   466,   426,   467,
     468,   427,   276,     0,     0,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   528,     0,     0,   300,   301,
     302,     0,   304,   305,   306,   307,   308,   434,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,     0,
     322,   323,   324,     0,     0,   327,   328,   329,   330,     0,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,   438,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,   442,   443,   444,
     445,   446,     0,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,    55,   529,   460,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   632,     0,   461,
     462,   463,     0,    12,     0,     0,   464,   465,   412,   413,
     414,   415,   416,     0,     0,   890,   466,     0,   467,   468,
     418,   276,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   422,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   528,     0,     0,
     300,   301,   302,     0,   304,   305,   306,   307,   308,   434,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,     0,   322,   323,   324,     0,     0,   327,   328,   329,
     330,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   437,   438,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,   442,
     443,   444,   445,   446,     0,   447,     0,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,    55,   529,
     460,     0,     0,     0,     0,     0,     0,   412,   413,   414,
     415,   416,     0,     0,     0,     0,     0,     0,     0,   418,
       0,   461,   462,   463,     0,    12,     0,     0,   464,   465,
       0,   422,     0,     0,     0,     0,   425,     0,   466,   426,
     467,   468,   427,   276,     0,     0,   430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   528,     0,     0,   300,
     301,   302,     0,   304,   305,   306,   307,   308,   434,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
       0,   322,   323,   324,     0,     0,   327,   328,   329,   330,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,   438,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,   442,   443,
     444,   445,   446,     0,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,    55,   529,   460,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     461,   462,   463,     0,    12,     0,     0,   464,   465,   412,
     413,   414,   415,   416,     0,     0,     0,   466,   910,   467,
     468,   418,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   422,     0,     0,     0,     0,   425,     0,
       0,   426,     0,     0,   427,     0,     0,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   528,     0,
       0,   300,   301,   302,     0,   304,   305,   306,   307,   308,
     434,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,     0,   322,   323,   324,     0,     0,   327,   328,
     329,   330,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   437,   438,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
     442,   443,   444,   445,   446,     0,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,    55,
     529,   460,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   461,   462,   463,     0,    12,     0,     0,   464,
     465,   412,   413,   414,   415,   416,     0,     0,     0,   466,
     946,   467,   468,   418,   276,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   422,     0,     0,     0,     0,
     425,     0,     0,   426,     0,     0,   427,     0,     0,     0,
     430,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     528,     0,     0,   300,   301,   302,     0,   304,   305,   306,
     307,   308,   434,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,     0,   322,   323,   324,     0,     0,
     327,   328,   329,   330,     0,     0,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   437,
     438,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,   442,   443,   444,   445,   446,     0,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,    55,   529,   460,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   948,     0,   461,   462,   463,     0,    12,     0,
       0,   464,   465,   412,   413,   414,   415,   416,     0,     0,
       0,   466,     0,   467,   468,   418,   276,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   422,     0,     0,
       0,     0,   425,     0,     0,   426,     0,     0,   427,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   528,     0,     0,   300,   301,   302,     0,   304,
     305,   306,   307,   308,   434,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,     0,   322,   323,   324,
       0,     0,   327,   328,   329,   330,     0,     0,   435,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,   438,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,     0,   442,   443,   444,   445,   446,     0,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,    55,   529,   460,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   461,   462,   463,     0,
      12,     0,     0,   464,   465,   412,   413,   414,   415,   416,
       0,     0,     0,   466,   971,   467,   468,   418,   276,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   422,
       0,     0,     0,     0,   425,     0,     0,   426,     0,     0,
     427,     0,     0,     0,   430,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   528,     0,     0,   300,   301,   302,
       0,   304,   305,   306,   307,   308,   434,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,     0,   322,
     323,   324,     0,     0,   327,   328,   329,   330,     0,     0,
     435,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   437,   438,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     0,   442,   443,   444,   445,
     446,     0,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,    55,   529,   460,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   461,   462,
     463,     0,    12,     0,     0,   464,   465,   412,   413,   414,
     415,   416,     0,     0,     0,   466,   986,   467,   468,   418,
     276,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   422,     0,     0,     0,     0,   425,     0,     0,   426,
       0,     0,   427,     0,     0,     0,   430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   528,     0,     0,   300,
     301,   302,     0,   304,   305,   306,   307,   308,   434,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
       0,   322,   323,   324,     0,     0,   327,   328,   329,   330,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,   438,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,   442,   443,
     444,   445,   446,     0,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,    55,   529,   460,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   412,   413,   414,   415,   416,     0,
     461,   462,   463,     0,    12,     0,   418,   464,   465,     0,
       0,     0,     0,     0,     0,  1125,     0,   466,   422,   467,
     468,     0,   276,   425,     0,     0,   426,     0,     0,   427,
       0,     0,     0,   430,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   528,     0,     0,   300,   301,   302,     0,
     304,   305,   306,   307,   308,   434,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,     0,   322,   323,
     324,     0,     0,   327,   328,   329,   330,     0,     0,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,   438,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,     0,   442,   443,   444,   445,   446,
       0,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,    55,   529,   460,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   461,   462,   463,
       0,    12,     0,     0,   464,   465,   412,   413,   414,   415,
     416,     0,     0,     0,   466,  1140,   467,   468,   418,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     422,     0,     0,     0,     0,   425,     0,     0,   426,     0,
       0,   427,     0,     0,     0,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   528,     0,     0,   300,   301,
     302,     0,   304,   305,   306,   307,   308,   434,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,     0,
     322,   323,   324,     0,     0,   327,   328,   329,   330,     0,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,   438,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,   442,   443,   444,
     445,   446,     0,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,    55,   529,   460,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   461,
     462,   463,     0,    12,     0,     0,   464,   465,   412,   413,
     414,   415,   416,     0,     0,     0,   466,  1170,   467,   468,
     418,   276,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   422,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   528,     0,     0,
     300,   301,   302,     0,   304,   305,   306,   307,   308,   434,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,     0,   322,   323,   324,     0,     0,   327,   328,   329,
     330,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   437,   438,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,   442,
     443,   444,   445,   446,     0,   447,     0,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,    55,   529,
     460,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   461,   462,   463,     0,    12,     0,     0,   464,   465,
     412,   413,   414,   415,   416,     0,     0,     0,   466,  1219,
     467,   468,   418,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   422,     0,     0,     0,     0,   425,
       0,     0,   426,     0,     0,   427,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   528,
       0,     0,   300,   301,   302,     0,   304,   305,   306,   307,
     308,   434,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,     0,   322,   323,   324,     0,     0,   327,
     328,   329,   330,     0,     0,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437,   438,
       0,     0,     0,     0,     0,     0,     0,  1246,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       0,   442,   443,   444,   445,   446,     0,   447,     0,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
      55,   529,   460,     0,     0,     0,     0,     0,     0,   412,
     413,   414,   415,   416,     0,     0,     0,     0,     0,     0,
       0,   418,     0,   461,   462,   463,     0,    12,     0,     0,
     464,   465,     0,   422,     0,     0,     0,     0,   425,     0,
     466,   426,   467,   468,   427,   276,     0,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   528,     0,
       0,   300,   301,   302,     0,   304,   305,   306,   307,   308,
     434,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,     0,   322,   323,   324,     0,     0,   327,   328,
     329,   330,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   437,   438,     0,
       0,     0,     0,     0,     0,     0,  1268,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
     442,   443,   444,   445,   446,     0,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,    55,
     529,   460,     0,     0,     0,     0,     0,     0,   412,   413,
     414,   415,   416,     0,     0,     0,     0,     0,     0,     0,
     418,     0,   461,   462,   463,     0,    12,     0,     0,   464,
     465,     0,   422,     0,     0,     0,     0,   425,     0,   466,
     426,   467,   468,   427,   276,     0,     0,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   528,     0,     0,
     300,   301,   302,     0,   304,   305,   306,   307,   308,   434,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,     0,   322,   323,   324,     0,     0,   327,   328,   329,
     330,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   437,   438,     0,     0,
       0,     0,     0,     0,     0,  1270,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,   442,
     443,   444,   445,   446,     0,   447,     0,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,    55,   529,
     460,     0,     0,     0,     0,     0,     0,   412,   413,   414,
     415,   416,     0,     0,     0,     0,     0,     0,     0,   418,
       0,   461,   462,   463,     0,    12,     0,     0,   464,   465,
       0,   422,     0,     0,     0,     0,   425,     0,   466,   426,
     467,   468,   427,   276,     0,     0,   430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   528,     0,     0,   300,
     301,   302,     0,   304,   305,   306,   307,   308,   434,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
       0,   322,   323,   324,     0,     0,   327,   328,   329,   330,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,   438,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   659,   660,     0,     0,     0,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,   442,   443,
     444,   445,   446,     0,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,    55,   529,   460,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     461,   462,   463,     0,    12,     0,     0,   464,   465,     0,
       0,     0,     0,     0,     0,     0,     0,   466,     0,   467,
     468,     0,   276,   661,   662,   663,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,   677,
     678,   679,   770,   681,   682,   683,   684,   659,   660,     0,
       0,   685,   686,   687,   688,   689,   690,     0,     0,   691,
     692,   693,   694,   695,   696,   697,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   659,   660,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     771,   699,     0,   700,   701,   702,   703,   704,   705,   706,
     707,   708,   709,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   710,   711,     0,     0,     0,     0,     0,  1058,
       0,     0,     0,     0,     0,     0,     0,   669,   670,   671,
     672,   673,     0,     0,   676,   677,   678,   679,     0,   681,
     682,   683,   684,     0,     0,     0,     0,   685,     0,   687,
     688,     0,     0,     0,     0,   691,   692,   693,     0,     0,
       0,   697,   669,   670,   671,   672,   673,     0,     0,   676,
     677,   678,   679,     0,   681,   682,   683,   684,   659,   660,
       0,     0,   685,     0,   687,   688,     0,     0,     0,     0,
     691,   692,   693,     0,     0,     0,   697,   699,     0,   700,
     701,   702,   703,   704,   705,   706,   707,   708,   709,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   710,   711,
       0,     0,     0,     0,     0,  1260,     0,     0,     0,     0,
       0,     0,   699,     0,   700,   701,   702,   703,   704,   705,
     706,   707,   708,   709,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   710,   711,     0,     0,     0,     0,     0,
    1302,   296,     0,     0,     0,     0,     0,   297,   669,   670,
     671,   672,   673,     0,     0,   676,   677,   678,   679,     0,
     681,   682,   683,   684,   298,     0,     0,     0,   685,     0,
     687,   688,   659,   660,     0,     0,   691,   692,   693,   299,
       0,     0,   697,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,     0,     0,   699,     0,
     700,   701,   702,   703,   704,   705,   706,   707,   708,   709,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   710,
     711,     0,     0,     0,     0,     0,  1304,     0,     0,     0,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,   669,   670,   671,   672,   673,   333,     0,   676,
     677,   678,   679,     0,   681,   682,   683,   684,   659,   660,
       0,    55,   685,     0,   687,   688,     0,     0,     0,     0,
     691,   692,   693,     0,     0,     0,   697,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   659,   660,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   561,     0,
       0,     0,   699,     0,   700,   701,   702,   703,   704,   705,
     706,   707,   708,   709,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   710,   711,     0,     0,   721,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   669,   670,
     671,   672,   673,     0,     0,   676,   677,   678,   679,     0,
     681,   682,   683,   684,     0,     0,     0,     0,   685,     0,
     687,   688,     0,     0,     0,     0,   691,   692,   693,     0,
       0,     0,   697,   669,   670,   671,   672,   673,     0,     0,
     676,   677,   678,   679,     0,   681,   682,   683,   684,   659,
     660,     0,     0,   685,     0,   687,   688,     0,     0,     0,
       0,   691,   692,   693,     0,     0,     0,   697,   699,     0,
     700,   701,   702,   703,   704,   705,   706,   707,   708,   709,
       0,     0,     0,     0,   659,   660,     0,     0,     0,   710,
     711,     0,     0,   747,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   699,     0,   700,   701,   702,   703,   704,
     705,   706,   707,   708,   709,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   710,   711,     0,     0,   755,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   669,
     670,   671,   672,   673,     0,     0,   676,   677,   678,   679,
       0,   681,   682,   683,   684,     0,     0,     0,     0,   685,
       0,   687,   688,     0,     0,     0,     0,   691,   692,   693,
       0,     0,     0,   697,   669,   670,   671,   672,   673,     0,
       0,   676,   677,   678,   679,     0,   681,   682,   683,   684,
     659,   660,     0,     0,   685,     0,   687,   688,     0,     0,
       0,     0,   691,   692,   693,     0,     0,     0,   697,   699,
       0,   700,   701,   702,   703,   704,   705,   706,   707,   708,
     709,     0,     0,     0,     0,   659,   660,     0,     0,     0,
     710,   711,     0,     0,   820,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   699,     0,   700,   701,   702,   703,
     704,   705,   706,   707,   708,   709,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   710,   711,     0,     0,   941,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     669,   670,   671,   672,   673,     0,     0,   676,   677,   678,
     679,     0,   681,   682,   683,   684,     0,     0,     0,     0,
     685,     0,   687,   688,     0,     0,     0,     0,   691,   692,
     693,     0,     0,     0,   697,   669,   670,   671,   672,   673,
       0,     0,   676,   677,   678,   679,     0,   681,   682,   683,
     684,   659,   660,     0,     0,   685,     0,   687,   688,     0,
       0,     0,     0,   691,   692,   693,     0,     0,     0,   697,
     699,     0,   700,   701,   702,   703,   704,   705,   706,   707,
     708,   709,     0,     0,     0,     0,   659,   660,     0,     0,
       0,   710,   711,     0,     0,   952,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   699,     0,   700,   701,   702,
     703,   704,   705,   706,   707,   708,   709,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   710,   711,     0,     0,
     956,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   669,   670,   671,   672,   673,     0,     0,   676,   677,
     678,   679,     0,   681,   682,   683,   684,     0,     0,     0,
       0,   685,     0,   687,   688,     0,     0,     0,     0,   691,
     692,   693,     0,     0,     0,   697,   669,   670,   671,   672,
     673,     0,     0,   676,   677,   678,   679,     0,   681,   682,
     683,   684,   659,   660,     0,     0,   685,     0,   687,   688,
       0,     0,     0,     0,   691,   692,   693,     0,     0,     0,
     697,   699,     0,   700,   701,   702,   703,   704,   705,   706,
     707,   708,   709,     0,     0,     0,     0,   659,   660,     0,
       0,     0,   710,   711,     0,     0,   958,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   699,     0,   700,   701,
     702,   703,   704,   705,   706,   707,   708,   709,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   710,   711,     0,
       0,   960,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   669,   670,   671,   672,   673,     0,     0,   676,
     677,   678,   679,     0,   681,   682,   683,   684,     0,     0,
       0,     0,   685,     0,   687,   688,     0,     0,     0,     0,
     691,   692,   693,     0,     0,     0,   697,   669,   670,   671,
     672,   673,     0,     0,   676,   677,   678,   679,     0,   681,
     682,   683,   684,   659,   660,     0,     0,   685,     0,   687,
     688,     0,     0,     0,     0,   691,   692,   693,     0,     0,
       0,   697,   699,     0,   700,   701,   702,   703,   704,   705,
     706,   707,   708,   709,     0,     0,     0,     0,   659,   660,
       0,     0,     0,   710,   711,     0,     0,   961,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   699,     0,   700,
     701,   702,   703,   704,   705,   706,   707,   708,   709,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   710,   711,
       0,     0,   962,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   669,   670,   671,   672,   673,     0,     0,
     676,   677,   678,   679,     0,   681,   682,   683,   684,     0,
       0,     0,     0,   685,     0,   687,   688,     0,     0,     0,
       0,   691,   692,   693,     0,     0,     0,   697,   669,   670,
     671,   672,   673,     0,     0,   676,   677,   678,   679,     0,
     681,   682,   683,   684,   659,   660,     0,     0,   685,     0,
     687,   688,     0,     0,     0,     0,   691,   692,   693,     0,
       0,     0,   697,   699,     0,   700,   701,   702,   703,   704,
     705,   706,   707,   708,   709,     0,     0,     0,     0,   659,
     660,     0,     0,     0,   710,   711,     0,     0,   963,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   699,     0,
     700,   701,   702,   703,   704,   705,   706,   707,   708,   709,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   710,
     711,     0,     0,   964,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   669,   670,   671,   672,   673,     0,
       0,   676,   677,   678,   679,     0,   681,   682,   683,   684,
       0,     0,     0,     0,   685,     0,   687,   688,     0,     0,
       0,     0,   691,   692,   693,     0,     0,     0,   697,   669,
     670,   671,   672,   673,     0,     0,   676,   677,   678,   679,
       0,   681,   682,   683,   684,   659,   660,     0,     0,   685,
       0,   687,   688,     0,     0,     0,     0,   691,   692,   693,
       0,     0,     0,   697,   699,     0,   700,   701,   702,   703,
     704,   705,   706,   707,   708,   709,     0,     0,     0,     0,
     659,   660,     0,     0,     0,   710,   711,     0,     0,   965,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   699,
       0,   700,   701,   702,   703,   704,   705,   706,   707,   708,
     709,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     710,   711,     0,     0,  1083,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   669,   670,   671,   672,   673,
       0,     0,   676,   677,   678,   679,     0,   681,   682,   683,
     684,     0,     0,     0,     0,   685,     0,   687,   688,     0,
       0,     0,     0,   691,   692,   693,     0,     0,     0,   697,
     669,   670,   671,   672,   673,     0,     0,   676,   677,   678,
     679,     0,   681,   682,   683,   684,   659,   660,     0,     0,
     685,     0,   687,   688,     0,     0,     0,     0,   691,   692,
     693,     0,     0,     0,   697,   699,     0,   700,   701,   702,
     703,   704,   705,   706,   707,   708,   709,     0,     0,     0,
       0,   659,   660,     0,     0,     0,   710,   711,     0,     0,
    1173,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     699,     0,   700,   701,   702,   703,   704,   705,   706,   707,
     708,   709,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   710,   711,     0,     0,  1183,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   669,   670,   671,   672,
     673,     0,     0,   676,   677,   678,   679,     0,   681,   682,
     683,   684,     0,     0,     0,     0,   685,     0,   687,   688,
       0,     0,     0,     0,   691,   692,   693,     0,     0,     0,
     697,   669,   670,   671,   672,   673,     0,     0,   676,   677,
     678,   679,     0,   681,   682,   683,   684,   659,   660,     0,
       0,   685,     0,   687,   688,     0,     0,     0,     0,   691,
     692,   693,     0,     0,     0,   697,   699,     0,   700,   701,
     702,   703,   704,   705,   706,   707,   708,   709,     0,     0,
       0,     0,   659,   660,     0,     0,     0,   710,   711,     0,
       0,  1184,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   699,     0,   700,   701,   702,   703,   704,   705,   706,
     707,   708,   709,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   710,   711,     0,     0,  1200,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   669,   670,   671,
     672,   673,     0,     0,   676,   677,   678,   679,     0,   681,
     682,   683,   684,     0,     0,     0,     0,   685,     0,   687,
     688,     0,     0,     0,     0,   691,   692,   693,     0,     0,
       0,   697,   669,   670,   671,   672,   673,     0,     0,   676,
     677,   678,   679,     0,   681,   682,   683,   684,   659,   660,
       0,     0,   685,     0,   687,   688,     0,     0,     0,     0,
     691,   692,   693,     0,     0,     0,   697,   699,     0,   700,
     701,   702,   703,   704,   705,   706,   707,   708,   709,     0,
       0,     0,     0,   659,   660,     0,     0,     0,   710,   711,
       0,     0,  1202,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   699,     0,   700,   701,   702,   703,   704,   705,
     706,   707,   708,   709,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   710,   711,     0,     0,  1204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   669,   670,
     671,   672,   673,     0,     0,   676,   677,   678,   679,     0,
     681,   682,   683,   684,     0,     0,     0,     0,   685,     0,
     687,   688,     0,     0,     0,     0,   691,   692,   693,     0,
       0,     0,   697,   669,   670,   671,   672,   673,     0,     0,
     676,   677,   678,   679,     0,   681,   682,   683,   684,   659,
     660,     0,     0,   685,     0,   687,   688,     0,     0,     0,
       0,   691,   692,   693,     0,     0,     0,   697,   699,     0,
     700,   701,   702,   703,   704,   705,   706,   707,   708,   709,
       0,     0,     0,     0,   659,   660,     0,     0,     0,   710,
     711,     0,     0,  1208,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   699,     0,   700,   701,   702,   703,   704,
     705,   706,   707,   708,   709,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   710,   711,     0,     0,  1251,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   669,
     670,   671,   672,   673,     0,     0,   676,   677,   678,   679,
       0,   681,   682,   683,   684,     0,     0,     0,     0,   685,
       0,   687,   688,     0,     0,     0,     0,   691,   692,   693,
       0,     0,     0,   697,   669,   670,   671,   672,   673,     0,
       0,   676,   677,   678,   679,     0,   681,   682,   683,   684,
     659,   660,     0,     0,   685,     0,   687,   688,     0,     0,
       0,     0,   691,   692,   693,     0,     0,     0,   697,   699,
       0,   700,   701,   702,   703,   704,   705,   706,   707,   708,
     709,     0,     0,     0,     0,   659,   660,     0,     0,     0,
     710,   711,     0,     0,  1252,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   699,     0,   700,   701,   702,   703,
     704,   705,   706,   707,   708,   709,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   710,   711,     0,     0,  1253,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     669,   670,   671,   672,   673,     0,     0,   676,   677,   678,
     679,     0,   681,   682,   683,   684,     0,     0,     0,     0,
     685,     0,   687,   688,     0,     0,     0,     0,   691,   692,
     693,     0,     0,     0,   697,   669,   670,   671,   672,   673,
       0,     0,   676,   677,   678,   679,     0,   681,   682,   683,
     684,   659,   660,     0,     0,   685,     0,   687,   688,     0,
       0,     0,     0,   691,   692,   693,     0,     0,     0,   697,
     699,     0,   700,   701,   702,   703,   704,   705,   706,   707,
     708,   709,     0,     0,     0,     0,   659,   660,     0,     0,
       0,   710,   711,     0,     0,  1257,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   699,     0,   700,   701,   702,
     703,   704,   705,   706,   707,   708,   709,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   710,   711,     0,     0,
    1259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   669,   670,   671,   672,   673,     0,     0,   676,   677,
     678,   679,     0,   681,   682,   683,   684,     0,     0,     0,
       0,   685,     0,   687,   688,     0,     0,     0,     0,   691,
     692,   693,     0,     0,     0,   697,   669,   670,   671,   672,
     673,     0,     0,   676,   677,   678,   679,     0,   681,   682,
     683,   684,   659,   660,     0,     0,   685,     0,   687,   688,
       0,     0,     0,     0,   691,   692,   693,     0,     0,     0,
     697,   699,     0,   700,   701,   702,   703,   704,   705,   706,
     707,   708,   709,     0,     0,     0,     0,   659,   660,     0,
       0,     0,   710,   711,     0,     0,  1264,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   699,     0,   700,   701,
     702,   703,   704,   705,   706,   707,   708,   709,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   710,   711,     0,
       0,  1295,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   669,   670,   671,   672,   673,     0,     0,   676,
     677,   678,   679,     0,   681,   682,   683,   684,     0,     0,
       0,     0,   685,     0,   687,   688,     0,     0,     0,     0,
     691,   692,   693,     0,     0,     0,   697,   669,   670,   671,
     672,   673,   659,   660,   676,   677,   678,   679,     0,   681,
     682,   683,   684,     0,     0,     0,     0,   685,     0,   687,
     688,     0,     0,     0,     0,   691,   692,   693,     0,     0,
       0,   697,   699,     0,   700,   701,   702,   703,   704,   705,
     706,   707,   708,   709,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   710,   711,   749,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   699,     0,   700,
     701,   702,   703,   704,   705,   706,   707,   708,   709,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   710,   711,
    1029,     0,   669,   670,   671,   672,   673,   659,   660,   676,
     677,   678,   679,     0,   681,   682,   683,   684,     0,     0,
       0,     0,   685,     0,   687,   688,     0,     0,     0,     0,
     691,   692,   693,     0,     0,     0,   697,     0,     0,     0,
       0,     0,   659,   660,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   699,     0,   700,   701,   702,   703,   704,   705,
     706,   707,   708,   709,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   710,   711,  1046,     0,   669,   670,   671,
     672,   673,     0,     0,   676,   677,   678,   679,     0,   681,
     682,   683,   684,     0,     0,     0,     0,   685,     0,   687,
     688,     0,     0,     0,     0,   691,   692,   693,     0,     0,
       0,   697,   669,   670,   671,   672,   673,     0,     0,   676,
     677,   678,   679,     0,   681,   682,   683,   684,   659,   660,
       0,     0,   685,     0,   687,   688,     0,     0,     0,     0,
     691,   692,   693,     0,     0,     0,   697,   699,     0,   700,
     701,   702,   703,   704,   705,   706,   707,   708,   709,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   710,   711,
    1147,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   699,     0,   700,   701,   702,   703,   704,   705,
     706,   707,   708,   709,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   710,   711,  1150,     0,     0,     0,     0,
     661,   662,   663,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,   677,   678,   679,   770,
     681,   682,   683,   684,  -325,     0,   221,   222,   685,   686,
     687,   688,   689,   690,     0,     0,   691,   692,   693,   694,
     695,   696,   697,   223,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   659,   660,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   771,   699,     0,
     700,   701,   702,   703,   704,   705,   706,   707,   708,   709,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   710,
     711,     0,     0,     0,     0,     0,     0,     0,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,     0,     0,   242,   243,
     244,     0,     0,     0,     0,     0,     0,   245,   246,   247,
     248,   249,     0,     0,   250,   251,   252,   253,   254,   255,
     256,   669,   670,   671,   672,   673,   659,   660,   676,   677,
     678,   679,     0,   681,   682,   683,   684,     0,     0,     0,
       0,   685,     0,   687,   688,     0,     0,   810,     0,   691,
     692,   693,     0,     0,     0,   697,   257,     0,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,     0,     0,
     268,   269,     0,     0,     0,     0,     0,   270,   271,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   699,     0,   700,   701,   702,   703,   704,   705,   706,
     707,   708,   709,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   710,   711,     0,     0,   669,   670,   671,   672,
     673,   659,   660,   676,   677,   678,   679,     0,   681,   682,
     683,   684,     0,     0,     0,     0,   685,     0,   687,   688,
       0,     0,   972,     0,   691,   692,   693,     0,     0,     0,
     697,     0,     0,     0,     0,     0,   659,   660,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   699,     0,   700,   701,
     702,   703,   704,   705,   706,   707,   708,   709,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   710,   711,     0,
       0,   669,   670,   671,   672,   673,     0,     0,   676,   677,
     678,   679,     0,   681,   682,   683,   684,     0,     0,     0,
       0,   685,     0,   687,   688,     0,     0,     0,     0,   691,
     692,   693,   659,   660,     0,   697,   669,   670,   671,   672,
     673,     0,     0,   676,   677,   678,   679,     0,   681,   682,
     683,   684,     0,     0,     0,     0,   685,     0,   687,   688,
       0,     0,     0,     0,   691,   692,   693,   659,   660,     0,
     697,   699,  1037,   700,   701,   702,   703,   704,   705,   706,
     707,   708,   709,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   710,   711,     0,     0,     0,  1114,     0,     0,
       0,     0,     0,     0,     0,     0,   699,     0,   700,   701,
     702,   703,   704,   705,   706,   707,   708,   709,     0,     0,
       0,     0,   669,   670,   671,   672,   673,   710,   711,   676,
     677,   678,   679,     0,   681,   682,   683,   684,     0,     0,
       0,     0,   685,     0,   687,   688,     0,     0,     0,     0,
     691,   692,   693,   659,   660,     0,   697,   669,   670,   671,
     672,   673,     0,     0,   676,   677,   678,   679,     0,   681,
     682,   683,   684,     0,     0,     0,     0,   685,     0,   687,
     688,     0,     0,     0,     0,   691,   692,   693,   659,   660,
       0,  -679,   699,     0,   700,   701,   702,   703,   704,   705,
     706,   707,   708,   709,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   710,   711,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   699,     0,   700,
     701,   702,   703,   704,   705,   706,   707,   708,   709,     0,
       0,     0,     0,   669,   670,   671,   672,   673,   710,   711,
     676,   677,   678,   679,     0,   681,   682,   683,   684,     0,
       0,     0,     0,   685,     0,   687,   688,     0,     0,     0,
       0,   691,   692,   693,   659,   660,     0,     0,   669,   670,
     671,   672,   673,     0,     0,   676,   677,   678,   679,     0,
     681,   682,   683,   684,     0,     0,     0,     0,   685,     0,
     687,   688,   659,   660,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   699,     0,   700,   701,   702,   703,   704,
     705,   706,   707,   708,   709,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   710,   711,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     700,   701,   702,   703,   704,   705,   706,   707,   708,   709,
       0,     0,     0,     0,   669,   670,   671,   672,   673,   710,
     711,   676,   677,   678,   679,     0,   681,   682,   683,   684,
       0,     0,     0,     0,   685,     0,   687,   688,     0,     0,
       0,     0,   669,   670,   671,   672,   673,     0,     0,   676,
     677,   678,   679,     0,   681,   682,   683,   684,     0,     0,
       0,     0,   685,     0,   687,   688,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   701,   702,   703,
     704,   705,   706,   707,   708,   709,   625,     0,     0,     0,
       0,     0,     0,     0,     0,   710,   711,     0,   296,     0,
       0,     0,     0,     0,   297,     0,   702,   703,   704,   705,
     706,   707,   708,   709,     0,     0,     0,     0,     0,     0,
       0,   298,     0,   710,   711,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   299,     0,     0,     0,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   627,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   296,     0,     0,
       0,     0,     0,   297,     0,     0,     0,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,     0,
     298,     0,     0,     0,   333,   839,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   299,     0,     0,    55,   300,
     301,   302,   303,   304,   305,   306,   307,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,     0,   300,   301,   302,     0,   304,   305,   306,
     307,   308,   434,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,     0,   322,   323,   324,     0,     0,
     327,   328,   329,   330,     0,     0,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     843,     0,     0,   333,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1033,     0,     0,     0,     0,     0,     0,     0,   840,
       0,     0,     0,     0,     0,     0,     0,     0,   300,   301,
     302,   841,   304,   305,   306,   307,   308,   434,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,     0,
     322,   323,   324,     0,     0,   327,   328,   329,   330,   300,
     301,   302,     0,   304,   305,   306,   307,   308,   434,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
       0,   322,   323,   324,     0,     0,   327,   328,   329,   330,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   844,     0,   296,     0,     0,     0,
       0,     0,   297,     0,     0,     0,   845,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   298,
       0,     0,     0,     0,     0,  1034,     0,     0,     0,     0,
       0,     0,     0,     0,   299,     0,     0,  1035,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   296,     0,     0,     0,     0,
       0,   297,     0,     0,     0,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,     0,   298,     0,
       0,     0,   333,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   299,     0,     0,    55,   300,   301,   302,
     303,   304,   305,   306,   307,   308,   309,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   174,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   333,     0,     0,     0,     0,   175,     0,   176,     0,
     177,   178,   179,   180,   181,   741,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,     0,   193,   194,
     195,     0,     0,   196,   197,   198,   199,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   200,   201,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   202
};

static const yytype_int16 yycheck[] =
{
      12,    13,   204,   403,   399,   533,   357,   155,   551,   218,
     368,   545,   736,   547,   738,   549,   740,   802,   628,   439,
     440,   441,   829,   808,   363,    33,   712,   417,   411,   975,
       0,     7,   218,    14,    15,   393,     6,    48,    51,    21,
     136,    51,    19,   126,    98,   121,   436,    59,    60,    61,
      19,   120,    19,    61,   123,   124,   164,   121,   121,    29,
      55,    31,   149,    33,    32,   164,    61,   154,   131,    39,
      44,    18,    19,    55,    45,   164,   149,   763,    48,   765,
     157,   154,   123,   124,    54,   123,   124,    99,   100,   101,
     102,   449,   450,   164,    65,   138,   139,   140,   206,   164,
      20,    21,   174,   179,    32,    75,   182,    14,    15,   208,
      46,   152,   209,   209,   178,   179,   179,   181,   205,   208,
     184,   184,   130,   164,   121,   208,   208,   101,    98,   137,
     137,   173,   201,   202,   131,   146,   208,   208,   210,   204,
     137,   289,   152,    20,    21,  1232,   355,   537,   178,   178,
    1096,    87,   174,   164,   164,   177,   164,   211,    60,   368,
     201,   202,   205,   201,   202,   179,   208,   974,   210,   355,
     175,   176,   206,   207,   181,   209,   157,   178,   208,   208,
    1267,   162,   179,   164,   393,   166,   971,   195,   210,   579,
     110,   111,   112,   113,   114,   208,   204,   117,   203,   589,
     120,   178,   122,   123,   124,   125,   218,   208,   190,   178,
     130,   178,   132,   133,   178,   210,   184,   164,   420,   201,
     610,   191,   137,   204,   181,   179,   202,   205,  1174,   121,
     208,   433,   202,   178,   210,   112,   113,   657,   208,   131,
     449,   450,   625,   120,   627,   122,   123,   124,   125,   164,
     157,   609,   209,   130,   208,   162,   184,   164,   175,   166,
     205,   164,   121,   121,   178,   185,   186,   187,   188,   189,
     190,   191,   131,   131,   202,   121,     4,     5,   121,     7,
     966,   201,   202,   211,   137,   131,   203,   179,   131,   203,
     178,   208,   137,   146,   181,  1019,   164,   165,   137,   985,
     178,   146,   182,   837,   186,   181,   151,    35,   184,   178,
    1095,   164,   189,   190,   191,   178,   655,   205,  1125,   164,
     179,   179,  1107,   191,   201,   202,   208,   205,   208,   538,
     539,   540,   541,   179,   724,   544,   179,   546,  1024,   548,
     185,   550,   205,   355,   178,   178,   201,   178,   360,    20,
      21,  1254,   538,   539,   540,   541,   368,   152,   544,   759,
     546,   971,   548,   164,   165,   178,   204,   178,   577,   164,
     137,   205,   205,    11,   205,   795,   986,  1280,   178,   581,
     157,   393,    20,    21,    59,    60,    61,   154,   155,   156,
     203,   577,   203,   146,  1118,   597,   939,   164,   178,   186,
     609,   154,   414,   203,  1090,    20,    21,   181,   164,    70,
     146,   164,   186,    74,   178,   178,   164,   152,   185,   777,
     778,   208,   780,   203,    99,   100,   101,   102,   164,   164,
     204,   633,    93,    94,    95,    96,   186,   449,   450,   203,
     203,   112,   113,   645,   864,   865,   866,   186,   503,   120,
     659,   660,   123,   124,   125,   181,    55,   815,   208,   130,
     186,   138,    61,   140,   208,   999,   886,   887,   888,   208,
     525,   152,   110,   111,   112,   113,   114,   208,   204,   117,
     118,   119,   120,   164,   122,   123,   124,   125,   181,   167,
      20,    21,   130,   186,   132,   133,   146,   112,   113,   182,
     138,   139,   140,   186,   152,   120,   144,   122,   123,   124,
     125,   204,   181,    11,   164,   130,   164,   186,   204,    82,
      83,    20,    21,   209,    22,    23,   538,   539,   540,   541,
     201,   202,   544,    55,   546,   204,   548,   182,   550,    61,
      55,   186,   180,   752,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   178,   764,   164,   181,   767,   164,
     184,   152,   164,   201,   202,   577,   752,   164,   777,   778,
     772,   780,   720,   164,    55,   587,    10,    11,   764,    55,
      61,   767,   112,   113,   182,    61,   201,   202,   186,   209,
     120,   208,   122,   123,   124,   125,    55,   609,    55,    55,
     130,   167,    61,   182,    61,    61,   815,   186,   167,   168,
     169,   110,   111,   112,   113,   114,   167,   168,   117,   118,
     119,   120,   138,   122,   123,   124,   125,   101,    32,   641,
     154,   130,   182,   132,   133,   209,   186,   179,   182,   138,
     998,   140,   186,   182,   182,   209,   182,   186,   186,  1000,
     186,   138,   139,   140,    58,    59,    34,   187,   188,   189,
     190,   191,   182,   167,   168,    34,   186,   167,   168,   169,
     170,   201,   202,   204,   209,   164,   210,   203,   164,   164,
     164,   890,    21,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   164,   203,   209,   209,   204,   181,  1099,
      41,   181,   201,   202,   131,   717,   185,   185,   185,   204,
     185,   204,   185,   185,   118,   924,   185,  1245,   122,   185,
     185,   185,   182,   164,   164,   164,   935,   164,   937,    20,
      21,    41,   185,   164,   204,   203,   185,   204,   924,   185,
     752,   204,   204,   204,   157,  1273,    36,   185,     9,   935,
     208,    64,   764,   185,   204,   767,   204,   204,   210,   204,
     204,   204,   204,    41,   164,   777,   778,   202,   780,    20,
      21,   207,   164,  1173,   204,   208,   180,   185,  1136,   208,
     184,   164,   186,   187,   208,   208,   208,   208,   208,   998,
     205,   203,   208,   210,   181,   204,   210,   208,   202,  1001,
     164,   126,    13,   815,   179,   181,   157,   211,   207,   154,
     164,   209,     7,   186,   164,    41,   204,   178,  1213,   110,
     111,   112,   113,   114,   210,   178,   117,   118,   119,   120,
      32,   122,   123,   124,   125,   164,   204,   204,   185,   130,
       1,   132,   133,   185,   204,   204,   204,   138,   139,   140,
      65,   208,   204,   144,   210,   204,    58,    59,   164,   110,
     111,   112,   113,   114,  1264,   186,   117,   118,   119,   120,
     186,   122,   123,   124,   125,   186,   164,  1079,   164,   130,
     204,   132,   133,   203,   203,   203,  1244,   164,   164,   180,
     164,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   164,   185,   204,   204,    32,   204,   204,   204,   208,
     201,   202,   924,   208,   203,   164,   118,    32,   209,  1121,
     122,   205,   203,   935,   203,   937,   164,  1136,   186,  1138,
     186,    58,    59,  1135,   185,   186,   187,   188,   189,   190,
     191,   205,    51,   203,   210,   634,   208,    75,   203,   203,
     201,   202,  1138,     1,    41,   128,    78,  1187,    76,  1188,
     717,   826,  1188,   216,   357,  1188,  1188,  1188,     1,  1133,
     959,  1154,   365,  1212,  1054,  1157,   937,   405,   180,  1193,
      49,  1180,   184,   520,   186,   187,   998,   983,   414,   382,
     812,   118,   628,  1229,    -1,   122,  1205,    -1,   391,    -1,
     202,    -1,    -1,    20,    21,   398,    -1,   414,    -1,   211,
      -1,    -1,    -1,    -1,    -1,   408,    -1,    -1,    -1,  1205,
      -1,    -1,    -1,    -1,   417,    -1,    -1,    -1,   421,    -1,
     423,    -1,    -1,    -1,    -1,  1244,    -1,    -1,    -1,   432,
      -1,    -1,    -1,   436,   437,   438,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,    -1,    -1,   184,   451,   186,
     187,    -1,    -1,    -1,    -1,    -1,   459,    -1,   461,   462,
     463,   464,   465,   466,    -1,   202,    -1,    -1,    -1,    -1,
      -1,    -1,   475,    -1,   211,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1294,   110,   111,   112,   113,   114,    -1,    -1,
     117,   118,   119,   120,    -1,   122,   123,   124,   125,    -1,
      -1,    -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,   138,   139,   140,  1136,    -1,  1138,   144,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   537,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,   572,
      -1,    -1,    -1,    -1,   201,   202,   579,    -1,    -1,    -1,
      -1,   208,    -1,  1205,    -1,    -1,   589,    -1,    -1,    -1,
      -1,   594,    -1,   596,    -1,    -1,    -1,    -1,   601,    -1,
      -1,    -1,    -1,    -1,   607,    -1,    -1,   610,    -1,    -1,
    1232,    -1,    -1,    -1,    -1,    -1,   619,   620,   621,   622,
     623,   624,  1244,   626,    32,   628,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1267,    -1,    -1,    -1,    -1,
      58,    59,    -1,    -1,    -1,    58,    59,    -1,   661,   662,
     663,   664,   665,   666,   667,   668,   669,   670,    -1,    -1,
     673,   674,   675,   676,   677,   678,    -1,   680,   681,    -1,
     683,   684,   685,   686,   687,   688,   689,   690,   691,   692,
     693,   694,   695,   696,   697,   698,   699,   700,   701,   702,
     703,   704,   705,   706,   707,   708,   709,    -1,   711,    -1,
     118,   714,    -1,   121,   122,   118,    -1,    -1,    -1,   122,
      -1,   724,    -1,   131,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   766,    -1,    -1,    -1,   770,   771,    -1,
      -1,   179,   180,    -1,    -1,    -1,   184,   180,    -1,   187,
      -1,   184,    -1,   186,   187,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,   202,   798,   799,    -1,    -1,   202,
     208,    -1,    -1,   211,   807,    -1,    -1,   810,   211,   812,
      -1,    -1,    -1,    -1,    -1,   818,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   828,    -1,    -1,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,   135,    -1,    -1,   138,   139,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,   114,    -1,    -1,   117,   118,   119,   120,
      -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,
     903,   132,   133,    -1,   907,   179,   180,   138,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     943,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     953,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    20,    21,   968,   969,   970,    -1,   972,
     201,   202,    -1,    -1,    -1,    -1,    -1,   980,    -1,    -1,
     983,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   996,   997,    -1,    -1,  1000,    -1,    -1,
      -1,  1004,    32,    -1,    -1,    -1,  1009,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1021,    -1,
    1023,    -1,    -1,    -1,    -1,  1028,    -1,    -1,    58,    59,
      -1,    -1,    -1,    -1,  1037,    -1,    -1,    -1,  1041,    -1,
      -1,    -1,    -1,    -1,  1047,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    -1,
      -1,    -1,    -1,   130,   131,   132,   133,   134,   135,    -1,
      -1,   138,   139,   140,   141,   142,   143,   144,   118,    -1,
      -1,   121,   122,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    59,   179,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    20,    21,    -1,    -1,    -1,
      -1,  1144,    -1,  1146,   201,   202,  1149,    -1,    -1,   179,
     180,   208,    -1,    -1,   184,    -1,    -1,   187,    -1,    -1,
    1163,    -1,    -1,  1166,  1167,  1168,  1169,    -1,    -1,  1172,
      -1,    -1,   202,  1176,  1177,  1178,  1179,    -1,    -1,  1182,
     118,   211,    -1,    -1,   122,  1188,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1196,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1223,  1224,    -1,    -1,    -1,   110,   111,   112,   113,    -1,
      -1,    -1,  1235,  1236,    -1,   120,    -1,   122,   123,   124,
     125,    -1,   180,  1246,    -1,   130,   184,   132,   133,   187,
      -1,    -1,    -1,    -1,    -1,  1258,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,  1268,    -1,  1270,    -1,    -1,
      -1,  1274,    -1,   211,    -1,    -1,    -1,    -1,    -1,  1282,
      -1,  1284,  1285,     1,    -1,    -1,    -1,  1290,     6,    -1,
       8,     9,    10,    -1,    12,    -1,    14,    15,    16,    17,
      18,    -1,   187,   188,   189,   190,   191,    25,    26,    27,
      28,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,    37,
      38,    -1,    40,    -1,    42,    43,    -1,    -1,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,
      68,    -1,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    89,    90,    32,    -1,    93,    94,    95,    96,    -1,
      -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,    58,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
     128,   129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,
     148,   149,    -1,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,   187,
     188,   189,    -1,   191,    -1,    -1,   194,   195,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   204,    -1,   206,   207,
     208,   209,   210,     1,    -1,    -1,    -1,    -1,     6,    -1,
       8,     9,    10,    -1,    12,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,   180,    -1,    -1,    -1,   184,    -1,   186,   187,    37,
      38,    -1,    40,    -1,    42,    43,    -1,    -1,    46,    -1,
      48,    49,    50,   202,    52,    53,    -1,    -1,    56,    57,
      -1,    -1,   211,    -1,    -1,    63,    -1,    -1,    66,    67,
      68,    -1,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    89,    90,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
     128,   129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,
     148,   149,    -1,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
     188,   189,    -1,   191,    -1,    -1,   194,   195,    -1,    -1,
      -1,    14,    15,    16,    17,    18,   204,    -1,   206,   207,
     208,   209,   210,    26,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,    51,    -1,
      53,    -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,
      63,    -1,    -1,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    89,    90,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    32,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    58,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,
      -1,   184,    -1,    -1,   187,   188,   189,    -1,   191,    -1,
      -1,   194,   195,    14,    15,    16,    17,    18,    -1,   202,
      -1,   204,    -1,   206,   207,    26,   209,    -1,   211,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,   118,    49,    -1,
      -1,   122,    53,    -1,    -1,    -1,    -1,    58,    59,    -1,
      -1,    -1,    63,    -1,    -1,    66,    67,    68,    -1,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,    88,    89,    90,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,   112,   113,   184,    -1,   186,   187,   118,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
     211,    -1,    -1,    32,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,    -1,    -1,    58,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,    -1,    -1,   184,    -1,    -1,   187,   188,   189,    -1,
     191,    -1,    -1,   194,   195,    14,    15,    16,    17,    18,
      -1,   202,    -1,   204,    -1,   206,   207,    26,   209,    -1,
     211,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,   118,
      49,    -1,    -1,   122,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,
      -1,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    -1,    88,
      89,    90,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,   112,   113,   184,    -1,   186,   187,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,   211,    -1,    -1,    -1,   145,   146,   147,   148,
     149,    -1,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,   187,   188,
     189,    -1,   191,    -1,    -1,   194,   195,    14,    15,    16,
      17,    18,    -1,   202,    -1,   204,    -1,   206,   207,    26,
     209,    -1,   211,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    90,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
     127,   128,   129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
     147,   148,   149,    -1,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
     187,   188,   189,    -1,   191,    -1,    -1,   194,   195,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,   204,    46,   206,
     207,    49,   209,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,
      68,    -1,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    89,    90,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,   127,
     128,   129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,
     148,   149,    -1,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
     188,   189,    -1,   191,    -1,    -1,   194,   195,    -1,    -1,
      -1,    14,    15,    16,    17,    18,   204,    -1,   206,   207,
      -1,   209,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    89,    90,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    99,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,   146,   147,   148,   149,    -1,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   187,   188,   189,    -1,   191,    -1,
      -1,   194,   195,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,   204,    46,   206,   207,    49,   209,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    66,    67,    68,    -1,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    89,    90,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,   147,   148,   149,    -1,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   187,   188,   189,    -1,   191,    -1,    -1,
     194,   195,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     204,    46,   206,   207,    49,   209,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    66,    67,    68,    -1,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    90,    -1,    -1,    93,    94,
      95,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,   128,   129,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,   147,   148,   149,    -1,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,
     195,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   204,
      46,   206,   207,    49,   209,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    90,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,   128,   129,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,   147,   148,   149,    -1,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,   195,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   204,    46,
     206,   207,    49,   209,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    90,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
     147,   148,   149,    -1,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,   188,   189,    -1,   191,    -1,    -1,   194,   195,    14,
      15,    16,    17,    18,    -1,   202,    -1,   204,   205,   206,
     207,    26,   209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    66,    67,    68,    -1,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    90,    -1,    -1,    93,    94,
      95,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,   147,   148,   149,    -1,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,
     195,    -1,    38,    -1,    -1,    -1,    -1,    43,   203,   204,
      46,   206,   207,    49,   209,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    59,    -1,    -1,    -1,    63,    -1,    -1,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    90,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,   147,   148,   149,    -1,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,   195,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   204,    46,
     206,   207,    49,   209,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    90,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
     147,   148,   149,    -1,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
     187,   188,   189,    -1,   191,    -1,    -1,   194,   195,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,   204,    46,   206,
     207,    49,   209,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,
      68,    -1,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    89,    90,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,
     148,   149,    -1,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,   187,
     188,   189,    -1,   191,    -1,    -1,   194,   195,    14,    15,
      16,    17,    18,    -1,    -1,    21,   204,    -1,   206,   207,
      26,   209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    90,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,   147,   148,   149,    -1,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,   195,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   204,    46,
     206,   207,    49,   209,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    90,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
     147,   148,   149,    -1,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,   188,   189,    -1,   191,    -1,    -1,   194,   195,    14,
      15,    16,    17,    18,    -1,    -1,    -1,   204,   205,   206,
     207,    26,   209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    66,    67,    68,    -1,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    90,    -1,    -1,    93,    94,
      95,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,   147,   148,   149,    -1,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,
     195,    14,    15,    16,    17,    18,    -1,    -1,    -1,   204,
     205,   206,   207,    26,   209,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    89,    90,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,   146,   147,   148,   149,    -1,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,    -1,   187,   188,   189,    -1,   191,    -1,
      -1,   194,   195,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   204,    -1,   206,   207,    26,   209,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    66,    67,    68,    -1,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,    88,    89,    90,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   145,   146,   147,   148,   149,    -1,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   187,   188,   189,    -1,
     191,    -1,    -1,   194,   195,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   204,   205,   206,   207,    26,   209,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,
      -1,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    -1,    88,
      89,    90,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,   148,
     149,    -1,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,
     189,    -1,   191,    -1,    -1,   194,   195,    14,    15,    16,
      17,    18,    -1,    -1,    -1,   204,   205,   206,   207,    26,
     209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    90,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
     147,   148,   149,    -1,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
     187,   188,   189,    -1,   191,    -1,    26,   194,   195,    -1,
      -1,    -1,    -1,    -1,    -1,   202,    -1,   204,    38,   206,
     207,    -1,   209,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    -1,    88,    89,
      90,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,   146,   147,   148,   149,
      -1,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,   189,
      -1,   191,    -1,    -1,   194,   195,    14,    15,    16,    17,
      18,    -1,    -1,    -1,   204,   205,   206,   207,    26,   209,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,
      68,    -1,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    89,    90,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,
     148,   149,    -1,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
     188,   189,    -1,   191,    -1,    -1,   194,   195,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   204,   205,   206,   207,
      26,   209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    90,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,   147,   148,   149,    -1,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,   195,
      14,    15,    16,    17,    18,    -1,    -1,    -1,   204,   205,
     206,   207,    26,   209,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    66,    67,    68,    -1,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    89,    90,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,   147,   148,   149,    -1,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   187,   188,   189,    -1,   191,    -1,    -1,
     194,   195,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     204,    46,   206,   207,    49,   209,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    66,    67,    68,    -1,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    90,    -1,    -1,    93,    94,
      95,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,   147,   148,   149,    -1,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,
     195,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   204,
      46,   206,   207,    49,   209,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    90,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,   147,   148,   149,    -1,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,   195,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   204,    46,
     206,   207,    49,   209,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    90,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
     147,   148,   149,    -1,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,   188,   189,    -1,   191,    -1,    -1,   194,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   204,    -1,   206,
     207,    -1,   209,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    20,    21,    -1,
      -1,   130,   131,   132,   133,   134,   135,    -1,    -1,   138,
     139,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   201,   202,    -1,    -1,    -1,    -1,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,   114,    -1,    -1,   117,   118,   119,   120,    -1,   122,
     123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,
     133,    -1,    -1,    -1,    -1,   138,   139,   140,    -1,    -1,
      -1,   144,   110,   111,   112,   113,   114,    -1,    -1,   117,
     118,   119,   120,    -1,   122,   123,   124,   125,    20,    21,
      -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
     138,   139,   140,    -1,    -1,    -1,   144,   180,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,
      -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,   202,    -1,    -1,    -1,    -1,    -1,
     208,    24,    -1,    -1,    -1,    -1,    -1,    30,   110,   111,
     112,   113,   114,    -1,    -1,   117,   118,   119,   120,    -1,
     122,   123,   124,   125,    47,    -1,    -1,    -1,   130,    -1,
     132,   133,    20,    21,    -1,    -1,   138,   139,   140,    62,
      -1,    -1,   144,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    -1,    -1,   180,    -1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,
     202,    -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,   114,   150,    -1,   117,
     118,   119,   120,    -1,   122,   123,   124,   125,    20,    21,
      -1,   164,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
     138,   139,   140,    -1,    -1,    -1,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,    -1,
      -1,    -1,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,   202,    -1,    -1,   205,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
     112,   113,   114,    -1,    -1,   117,   118,   119,   120,    -1,
     122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,
     132,   133,    -1,    -1,    -1,    -1,   138,   139,   140,    -1,
      -1,    -1,   144,   110,   111,   112,   113,   114,    -1,    -1,
     117,   118,   119,   120,    -1,   122,   123,   124,   125,    20,
      21,    -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,   138,   139,   140,    -1,    -1,    -1,   144,   180,    -1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,   201,
     202,    -1,    -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   201,   202,    -1,    -1,   205,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,   114,    -1,    -1,   117,   118,   119,   120,
      -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,
      -1,   132,   133,    -1,    -1,    -1,    -1,   138,   139,   140,
      -1,    -1,    -1,   144,   110,   111,   112,   113,   114,    -1,
      -1,   117,   118,   119,   120,    -1,   122,   123,   124,   125,
      20,    21,    -1,    -1,   130,    -1,   132,   133,    -1,    -1,
      -1,    -1,   138,   139,   140,    -1,    -1,    -1,   144,   180,
      -1,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
     201,   202,    -1,    -1,   205,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   201,   202,    -1,    -1,   205,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,   114,    -1,    -1,   117,   118,   119,
     120,    -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,
     130,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,   139,
     140,    -1,    -1,    -1,   144,   110,   111,   112,   113,   114,
      -1,    -1,   117,   118,   119,   120,    -1,   122,   123,   124,
     125,    20,    21,    -1,    -1,   130,    -1,   132,   133,    -1,
      -1,    -1,    -1,   138,   139,   140,    -1,    -1,    -1,   144,
     180,    -1,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,   201,   202,    -1,    -1,   205,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,    -1,
     205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,   114,    -1,    -1,   117,   118,
     119,   120,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,
     139,   140,    -1,    -1,    -1,   144,   110,   111,   112,   113,
     114,    -1,    -1,   117,   118,   119,   120,    -1,   122,   123,
     124,   125,    20,    21,    -1,    -1,   130,    -1,   132,   133,
      -1,    -1,    -1,    -1,   138,   139,   140,    -1,    -1,    -1,
     144,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,   201,   202,    -1,    -1,   205,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,
      -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,   114,    -1,    -1,   117,
     118,   119,   120,    -1,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
     138,   139,   140,    -1,    -1,    -1,   144,   110,   111,   112,
     113,   114,    -1,    -1,   117,   118,   119,   120,    -1,   122,
     123,   124,   125,    20,    21,    -1,    -1,   130,    -1,   132,
     133,    -1,    -1,    -1,    -1,   138,   139,   140,    -1,    -1,
      -1,   144,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,   201,   202,    -1,    -1,   205,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,
      -1,    -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,   114,    -1,    -1,
     117,   118,   119,   120,    -1,   122,   123,   124,   125,    -1,
      -1,    -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,   138,   139,   140,    -1,    -1,    -1,   144,   110,   111,
     112,   113,   114,    -1,    -1,   117,   118,   119,   120,    -1,
     122,   123,   124,   125,    20,    21,    -1,    -1,   130,    -1,
     132,   133,    -1,    -1,    -1,    -1,   138,   139,   140,    -1,
      -1,    -1,   144,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,   201,   202,    -1,    -1,   205,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,
     202,    -1,    -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,    -1,
      -1,   117,   118,   119,   120,    -1,   122,   123,   124,   125,
      -1,    -1,    -1,    -1,   130,    -1,   132,   133,    -1,    -1,
      -1,    -1,   138,   139,   140,    -1,    -1,    -1,   144,   110,
     111,   112,   113,   114,    -1,    -1,   117,   118,   119,   120,
      -1,   122,   123,   124,   125,    20,    21,    -1,    -1,   130,
      -1,   132,   133,    -1,    -1,    -1,    -1,   138,   139,   140,
      -1,    -1,    -1,   144,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,   201,   202,    -1,    -1,   205,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     201,   202,    -1,    -1,   205,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,
      -1,    -1,   117,   118,   119,   120,    -1,   122,   123,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,    -1,
      -1,    -1,    -1,   138,   139,   140,    -1,    -1,    -1,   144,
     110,   111,   112,   113,   114,    -1,    -1,   117,   118,   119,
     120,    -1,   122,   123,   124,   125,    20,    21,    -1,    -1,
     130,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,   139,
     140,    -1,    -1,    -1,   144,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   201,   202,    -1,    -1,
     205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    -1,    -1,   205,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,
     114,    -1,    -1,   117,   118,   119,   120,    -1,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,
      -1,    -1,    -1,    -1,   138,   139,   140,    -1,    -1,    -1,
     144,   110,   111,   112,   113,   114,    -1,    -1,   117,   118,
     119,   120,    -1,   122,   123,   124,   125,    20,    21,    -1,
      -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,
     139,   140,    -1,    -1,    -1,   144,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,   201,   202,    -1,
      -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   201,   202,    -1,    -1,   205,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,   114,    -1,    -1,   117,   118,   119,   120,    -1,   122,
     123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,
     133,    -1,    -1,    -1,    -1,   138,   139,   140,    -1,    -1,
      -1,   144,   110,   111,   112,   113,   114,    -1,    -1,   117,
     118,   119,   120,    -1,   122,   123,   124,   125,    20,    21,
      -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
     138,   139,   140,    -1,    -1,    -1,   144,   180,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,   201,   202,
      -1,    -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,   202,    -1,    -1,   205,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
     112,   113,   114,    -1,    -1,   117,   118,   119,   120,    -1,
     122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,
     132,   133,    -1,    -1,    -1,    -1,   138,   139,   140,    -1,
      -1,    -1,   144,   110,   111,   112,   113,   114,    -1,    -1,
     117,   118,   119,   120,    -1,   122,   123,   124,   125,    20,
      21,    -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,   138,   139,   140,    -1,    -1,    -1,   144,   180,    -1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,   201,
     202,    -1,    -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   201,   202,    -1,    -1,   205,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,   114,    -1,    -1,   117,   118,   119,   120,
      -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,
      -1,   132,   133,    -1,    -1,    -1,    -1,   138,   139,   140,
      -1,    -1,    -1,   144,   110,   111,   112,   113,   114,    -1,
      -1,   117,   118,   119,   120,    -1,   122,   123,   124,   125,
      20,    21,    -1,    -1,   130,    -1,   132,   133,    -1,    -1,
      -1,    -1,   138,   139,   140,    -1,    -1,    -1,   144,   180,
      -1,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
     201,   202,    -1,    -1,   205,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   201,   202,    -1,    -1,   205,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,   114,    -1,    -1,   117,   118,   119,
     120,    -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,
     130,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,   139,
     140,    -1,    -1,    -1,   144,   110,   111,   112,   113,   114,
      -1,    -1,   117,   118,   119,   120,    -1,   122,   123,   124,
     125,    20,    21,    -1,    -1,   130,    -1,   132,   133,    -1,
      -1,    -1,    -1,   138,   139,   140,    -1,    -1,    -1,   144,
     180,    -1,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,   201,   202,    -1,    -1,   205,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,    -1,
     205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,   114,    -1,    -1,   117,   118,
     119,   120,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,
     139,   140,    -1,    -1,    -1,   144,   110,   111,   112,   113,
     114,    -1,    -1,   117,   118,   119,   120,    -1,   122,   123,
     124,   125,    20,    21,    -1,    -1,   130,    -1,   132,   133,
      -1,    -1,    -1,    -1,   138,   139,   140,    -1,    -1,    -1,
     144,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,   201,   202,    -1,    -1,   205,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,
      -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,   114,    -1,    -1,   117,
     118,   119,   120,    -1,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
     138,   139,   140,    -1,    -1,    -1,   144,   110,   111,   112,
     113,   114,    20,    21,   117,   118,   119,   120,    -1,   122,
     123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,
     133,    -1,    -1,    -1,    -1,   138,   139,   140,    -1,    -1,
      -1,   144,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,   202,   203,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,
     203,    -1,   110,   111,   112,   113,   114,    20,    21,   117,
     118,   119,   120,    -1,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
     138,   139,   140,    -1,    -1,    -1,   144,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,   202,   203,    -1,   110,   111,   112,
     113,   114,    -1,    -1,   117,   118,   119,   120,    -1,   122,
     123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,
     133,    -1,    -1,    -1,    -1,   138,   139,   140,    -1,    -1,
      -1,   144,   110,   111,   112,   113,   114,    -1,    -1,   117,
     118,   119,   120,    -1,   122,   123,   124,   125,    20,    21,
      -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
     138,   139,   140,    -1,    -1,    -1,   144,   180,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,
     203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,   202,   203,    -1,    -1,    -1,    -1,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,    -1,    20,    21,   130,   131,
     132,   133,   134,   135,    -1,    -1,   138,   139,   140,   141,
     142,   143,   144,    37,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   179,   180,    -1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,
     202,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,
     134,   135,    -1,    -1,   138,   139,   140,   141,   142,   143,
     144,   110,   111,   112,   113,   114,    20,    21,   117,   118,
     119,   120,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,   132,   133,    -1,    -1,   136,    -1,   138,
     139,   140,    -1,    -1,    -1,   144,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
     194,   195,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   201,   202,    -1,    -1,   110,   111,   112,   113,
     114,    20,    21,   117,   118,   119,   120,    -1,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,
      -1,    -1,   136,    -1,   138,   139,   140,    -1,    -1,    -1,
     144,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,
      -1,   110,   111,   112,   113,   114,    -1,    -1,   117,   118,
     119,   120,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,
     139,   140,    20,    21,    -1,   144,   110,   111,   112,   113,
     114,    -1,    -1,   117,   118,   119,   120,    -1,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,
      -1,    -1,    -1,    -1,   138,   139,   140,    20,    21,    -1,
     144,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   201,   202,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,   114,   201,   202,   117,
     118,   119,   120,    -1,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
     138,   139,   140,    20,    21,    -1,   144,   110,   111,   112,
     113,   114,    -1,    -1,   117,   118,   119,   120,    -1,   122,
     123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,
     133,    -1,    -1,    -1,    -1,   138,   139,   140,    20,    21,
      -1,   144,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,   202,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,   114,   201,   202,
     117,   118,   119,   120,    -1,   122,   123,   124,   125,    -1,
      -1,    -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,   138,   139,   140,    20,    21,    -1,    -1,   110,   111,
     112,   113,   114,    -1,    -1,   117,   118,   119,   120,    -1,
     122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,
     132,   133,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   201,   202,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,   201,
     202,   117,   118,   119,   120,    -1,   122,   123,   124,   125,
      -1,    -1,    -1,    -1,   130,    -1,   132,   133,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,   114,    -1,    -1,   117,
     118,   119,   120,    -1,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   201,   202,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    47,    -1,   201,   202,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    -1,    -1,    -1,   150,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,   164,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    -1,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    89,    90,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,
      68,   164,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    89,    90,    -1,    -1,    93,    94,    95,    96,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    90,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,   164,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    -1,   164,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    -1,    -1,   164,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    66,    -1,    68,    -1,
      70,    71,    72,    73,    74,   164,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    -1,    88,    89,
      90,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   164
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   213,     0,     6,    29,    31,    33,    39,    48,    54,
      75,    98,   191,   202,   208,   214,   217,   223,   225,   226,
     231,   259,   263,   289,   361,   368,   372,   382,   422,   427,
      18,    19,   164,   252,   253,   254,   157,   232,   233,   164,
     165,   191,   227,   228,   229,   209,   369,   164,   206,   216,
      55,    61,   364,   364,   137,   164,   276,    33,    61,   130,
     195,   204,   255,   256,   257,   258,   276,   208,     4,     5,
       7,    35,   380,    60,   359,   179,   178,   181,   178,   228,
      21,    55,   190,   201,   230,   364,   365,   367,   365,   359,
     428,   423,   164,   137,   224,   257,   257,   257,   204,   138,
     139,   140,   178,   203,    55,    61,   264,   266,    55,    61,
     370,    55,    61,   381,    55,    61,   360,    14,    15,   157,
     162,   164,   166,   204,   219,   253,   157,   233,   164,   227,
     227,   164,   208,   210,   365,   208,    55,    61,   215,   164,
     164,   164,   167,   222,   205,   254,   257,   257,   257,   257,
     267,   164,   371,   383,   209,   362,   167,   168,   218,    14,
      15,   157,   162,   164,   219,   250,   251,   230,   366,   208,
     429,   424,   167,   205,    34,    66,    68,    70,    71,    72,
      73,    74,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    88,    89,    90,    93,    94,    95,    96,
     112,   113,   164,   262,   265,   181,   209,   101,   378,   379,
     357,   154,   334,   167,   168,   169,   178,   205,   179,   209,
     209,    20,    21,    37,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   122,   123,   124,   131,   132,   133,   134,   135,
     138,   139,   140,   141,   142,   143,   144,   180,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   194,   195,
     201,   202,    34,    34,   204,   260,   209,   268,    70,    74,
      93,    94,    95,    96,   387,   373,   164,   384,   210,   358,
     254,   146,   164,   355,   356,   250,    24,    30,    47,    62,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,   150,   276,   386,   388,   389,   392,   396,
     421,   430,   425,   164,   164,   164,   203,    21,   164,   203,
     149,   205,   334,   344,   345,   181,   261,   270,   209,   363,
     181,   377,   209,   334,   203,   204,    41,   178,   181,   184,
     354,   185,   185,   185,   185,   204,   185,   185,   185,   185,
     185,   185,   204,    32,    58,    59,   118,   122,   180,   184,
     187,   202,   211,   182,   391,   350,   164,   131,   204,    48,
     146,   164,   352,   385,   205,   208,   421,     1,     8,     9,
      10,    12,    14,    15,    16,    17,    18,    25,    26,    27,
      28,    37,    38,    40,    42,    43,    46,    49,    50,    52,
      53,    56,    57,    63,    75,    99,   100,   112,   113,   127,
     128,   129,   145,   146,   147,   148,   149,   151,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   165,
     166,   187,   188,   189,   194,   195,   204,   206,   207,   208,
     210,   221,   223,   234,   235,   238,   241,   242,   244,   245,
     246,   247,   248,   268,   269,   271,   272,   275,   276,   277,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   293,
     294,   301,   304,   307,   311,   312,   313,   314,   315,   316,
     318,   323,   326,   333,   386,   432,   441,   445,   447,   449,
     375,   164,   208,   374,   276,   340,   356,   203,    63,   165,
     326,   164,   164,   396,   121,   131,   179,   353,   399,   401,
     403,   397,   164,   393,   405,   407,   409,   411,   413,   415,
     417,   419,   326,    32,   184,    32,   184,   202,   211,   203,
     326,   211,   396,   164,   208,   431,   164,   208,   348,   426,
     326,   352,   204,    41,   121,   178,   179,   181,   184,   351,
     344,   326,   385,   185,   278,   388,   432,   204,   185,   121,
     275,   313,   326,   268,    59,   326,   204,   326,   164,   185,
     185,   204,   208,   185,   157,    56,   326,   204,   268,   185,
     121,   275,   326,   326,   326,   207,   311,   311,   311,   204,
     204,   204,   204,   204,   204,    12,   396,    12,   396,   326,
     443,   446,   185,   326,   220,   326,   326,   326,   326,   326,
     326,   207,    36,   208,   208,   326,     9,   164,   204,   208,
     208,   208,   208,   208,    64,   290,   259,   126,   208,    20,
      21,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   130,   131,   132,   133,   134,
     135,   138,   139,   140,   141,   142,   143,   144,   179,   180,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     201,   202,   204,   363,   179,   208,   210,   202,   210,   259,
     341,   205,    41,   208,   353,   275,   326,   421,   395,   421,
     421,   421,   205,   164,   390,   421,   260,   421,   260,   421,
     260,   164,   346,   347,   421,   348,   349,   205,   203,   203,
     208,   210,   181,   208,   210,   205,   326,   164,   164,   421,
     275,   326,   268,    19,   279,   204,   276,   302,   275,   326,
     121,   179,    13,   326,   326,   268,   179,   295,   297,   326,
     299,   181,   157,   326,   396,   275,   326,   207,   326,   326,
     326,   326,   326,   326,   385,    51,   152,   164,   187,   204,
     326,   433,   434,   435,   442,   444,   385,   204,   434,   444,
     136,   173,   208,   210,   436,   273,   268,   169,   170,   218,
     205,   146,   151,   185,   276,   317,   209,   268,   243,   202,
     205,   305,   326,   154,   292,   356,   154,   310,   311,    18,
     152,   164,   386,    18,   152,   164,   386,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   164,   127,   128,   129,   326,   326,   152,
     164,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   127,   128,   129,   326,
      21,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   123,   124,   152,   164,   201,   202,   324,   326,
     205,   305,   376,   326,   258,     7,   334,   339,   164,   275,
     326,   208,   186,   186,   208,   186,   186,   186,   208,   186,
     261,   186,   261,   186,   261,   181,   186,   208,   186,   208,
     421,   205,    41,   351,   305,   421,   205,   305,   185,   326,
     421,   268,   205,   249,   396,   396,   205,   396,   205,   186,
     205,   205,   205,   205,   205,   205,    19,   311,   204,   131,
     351,   205,   136,   178,   208,   435,   175,   176,   203,   439,
     178,   175,   203,   208,   438,    19,   205,   435,   174,   210,
     437,   326,   443,   210,   396,   326,   204,   204,   319,   321,
     270,    11,    22,    23,   236,   237,   326,   164,   433,   178,
     205,   164,   291,   121,   131,   179,   184,   308,   309,   260,
     185,   204,   185,   204,   204,   311,   311,   311,   204,   203,
     311,   311,   311,    18,   152,   164,   386,   181,   152,   164,
     326,   204,   152,   164,   326,     1,   203,   178,   205,   210,
     208,   203,    55,    61,   337,    65,   338,   208,   208,   400,
     402,   421,   404,   398,   394,   164,   406,   186,   410,   186,
     414,   186,   421,   418,   346,   420,   348,   164,   326,   208,
     186,   205,   164,   205,   186,   326,   186,   186,   186,   310,
     204,   305,   326,   326,   326,   434,   435,   326,   152,   164,
     433,   439,   203,   326,   203,   442,   305,   434,   174,   177,
     210,   440,   203,   186,   171,   326,   326,   396,   260,   210,
     268,   326,    11,   239,   203,   202,   326,   178,   203,   164,
     164,   164,   164,   178,   203,   261,   327,   326,   329,   326,
     205,   305,   326,   185,   204,   326,   204,   203,   326,   204,
     203,   325,   208,    45,   338,    44,   101,   335,   408,   412,
     416,   268,   280,   186,   208,   303,   296,   298,   300,   204,
     205,   305,   208,   205,   435,   439,   204,   131,   351,   208,
     435,   203,   274,   205,   205,   186,   261,   268,   240,   208,
     205,   433,   164,   203,   308,   203,   136,   268,   306,   396,
     205,   421,   205,   205,   205,   331,   326,   326,   205,   326,
      32,   336,   335,   337,   326,   164,   326,   326,   326,   205,
     326,   205,   326,   131,   351,   439,   326,   326,   326,   326,
     440,   326,   320,   186,   237,    25,   100,   241,   282,   283,
     284,   286,   326,   203,   181,   354,   121,   326,   186,   186,
     421,   205,   205,   205,   342,   336,   352,   205,   186,   205,
     208,   448,   326,   326,   205,   448,   317,   322,   121,   326,
     121,   326,   205,   396,   353,   326,   328,   330,   186,   265,
     343,   326,    51,   203,   131,   351,   210,   317,   326,   326,
     353,   326,   332,   208,   265,   205,   326,   203,   326,   326,
     203,   326,   208,   268,   208
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   212,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   214,   215,   215,   215,
     216,   216,   217,   218,   218,   218,   218,   219,   220,   220,
     220,   221,   222,   222,   224,   223,   225,   226,   227,   227,
     228,   228,   228,   228,   229,   229,   230,   230,   231,   232,
     232,   233,   233,   234,   235,   235,   236,   236,   237,   237,
     237,   238,   238,   239,   240,   239,   241,   241,   241,   241,
     241,   242,   243,   242,   244,   245,   246,   247,   249,   248,
     250,   250,   250,   250,   250,   250,   251,   251,   252,   252,
     252,   253,   253,   253,   253,   253,   253,   253,   253,   254,
     254,   255,   255,   255,   256,   256,   257,   257,   257,   257,
     257,   257,   257,   258,   258,   259,   259,   260,   260,   260,
     261,   261,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   263,   264,   264,
     264,   265,   267,   266,   268,   268,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   270,   270,   270,
     271,   273,   274,   272,   275,   275,   275,   275,   276,   276,
     276,   277,   277,   279,   280,   278,   278,   281,   281,   281,
     281,   282,   283,   284,   284,   284,   285,   285,   285,   286,
     286,   287,   287,   287,   288,   289,   289,   290,   290,   291,
     291,   292,   292,   293,   293,   295,   296,   294,   297,   298,
     294,   299,   300,   294,   302,   303,   301,   304,   304,   304,
     305,   305,   306,   306,   306,   307,   307,   307,   308,   308,
     308,   308,   309,   309,   310,   310,   311,   311,   312,   312,
     312,   312,   312,   312,   312,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   314,   314,   314,   314,   314,   314,
     315,   315,   316,   316,   317,   317,   318,   319,   320,   318,
     321,   322,   318,   323,   323,   324,   325,   323,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   327,   328,   326,
     326,   326,   326,   329,   330,   326,   326,   326,   331,   332,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   333,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   333,   333,   333,
     334,   334,   335,   335,   335,   336,   336,   337,   337,   337,
     338,   338,   339,   340,   341,   340,   342,   340,   343,   340,
     340,   344,   344,   344,   345,   345,   346,   346,   347,   347,
     348,   349,   349,   350,   350,   350,   351,   351,   352,   352,
     352,   352,   352,   352,   353,   353,   353,   354,   354,   355,
     355,   355,   355,   355,   356,   356,   356,   356,   356,   357,
     358,   357,   359,   359,   360,   360,   360,   361,   362,   361,
     363,   363,   363,   363,   364,   364,   364,   366,   365,   367,
     367,   368,   369,   368,   370,   370,   370,   371,   373,   374,
     372,   375,   376,   372,   377,   377,   378,   378,   379,   380,
     380,   381,   381,   381,   383,   384,   382,   385,   385,   385,
     385,   385,   386,   386,   386,   386,   386,   386,   386,   386,
     386,   386,   386,   386,   386,   386,   386,   386,   386,   386,
     386,   386,   386,   386,   386,   386,   386,   386,   386,   387,
     387,   387,   387,   387,   387,   388,   389,   389,   389,   390,
     390,   391,   391,   391,   393,   394,   392,   395,   395,   396,
     396,   396,   396,   396,   396,   396,   396,   396,   396,   396,
     396,   396,   396,   396,   396,   396,   396,   397,   398,   396,
     396,   399,   400,   396,   401,   402,   396,   403,   404,   396,
     396,   405,   406,   396,   407,   408,   396,   396,   409,   410,
     396,   411,   412,   396,   396,   413,   414,   396,   415,   416,
     396,   417,   418,   396,   419,   420,   396,   421,   421,   421,
     423,   424,   425,   426,   422,   428,   429,   430,   431,   427,
     432,   432,   432,   432,   433,   433,   433,   433,   433,   433,
     433,   433,   434,   434,   435,   435,   436,   436,   437,   437,
     438,   438,   439,   439,   439,   440,   440,   440,   441,   441,
     441,   441,   441,   441,   442,   442,   442,   443,   443,   444,
     444,   445,   445,   446,   446,   447,   448,   448,   449,   449
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     0,     1,     1,
       1,     1,     4,     1,     1,     2,     2,     3,     0,     2,
       4,     3,     1,     2,     0,     4,     2,     2,     1,     1,
       1,     2,     3,     3,     2,     4,     0,     1,     2,     1,
       3,     1,     3,     3,     3,     2,     1,     1,     0,     2,
       4,     1,     1,     0,     0,     3,     1,     1,     1,     1,
       1,     4,     0,     6,     6,     2,     3,     3,     0,     5,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     1,     5,     1,
       3,     1,     1,     1,     1,     4,     1,     2,     3,     3,
       3,     3,     2,     1,     3,     0,     3,     0,     2,     3,
       0,     2,     1,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     3,     3,     2,     2,
       3,     4,     3,     2,     2,     2,     2,     2,     3,     3,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     1,
       1,     3,     0,     4,     3,     7,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     1,     2,     2,     2,     0,     2,     2,
       3,     0,     0,     7,     3,     2,     2,     2,     1,     3,
       2,     2,     3,     0,     0,     5,     1,     2,     4,     5,
       2,     1,     1,     1,     2,     3,     2,     2,     3,     2,
       3,     2,     2,     3,     4,     1,     1,     1,     0,     1,
       3,     9,     8,     3,     3,     0,     0,     7,     0,     0,
       7,     0,     0,     7,     0,     0,     6,     5,     8,    10,
       1,     3,     1,     2,     3,     1,     1,     2,     2,     2,
       2,     2,     1,     3,     0,     4,     1,     6,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     4,     4,     4,     4,     4,     4,
       6,     8,     5,     6,     1,     4,     3,     0,     0,     8,
       0,     0,     9,     3,     4,     0,     0,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     3,     4,
       5,     4,     5,     3,     4,     1,     3,     4,     3,     4,
       2,     4,     4,     7,     8,     3,     5,     0,     0,     8,
       3,     3,     3,     0,     0,     8,     3,     4,     0,     0,
       9,     4,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     2,     4,     1,     4,     4,     4,     4,     4,     1,
       6,     7,     6,     6,     7,     7,     6,     7,     6,     6,
       0,     4,     0,     1,     1,     0,     1,     0,     1,     1,
       0,     1,     5,     0,     0,     4,     0,     9,     0,    10,
       5,     2,     3,     4,     1,     3,     1,     3,     1,     3,
       3,     1,     3,     0,     2,     3,     1,     1,     1,     2,
       3,     5,     3,     3,     1,     1,     1,     0,     1,     1,
       4,     3,     3,     5,     4,     6,     5,     5,     4,     0,
       0,     4,     0,     1,     0,     1,     1,     6,     0,     6,
       0,     2,     3,     5,     0,     1,     1,     0,     5,     2,
       3,     4,     0,     4,     0,     1,     1,     1,     0,     0,
       9,     0,     0,    11,     0,     2,     0,     1,     3,     1,
       1,     0,     1,     1,     0,     0,     9,     1,     4,     3,
       3,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     1,
       3,     0,     2,     3,     0,     0,     6,     1,     3,     1,
       1,     1,     1,     4,     3,     4,     2,     2,     3,     2,
       3,     2,     2,     3,     3,     3,     2,     0,     0,     6,
       2,     0,     0,     6,     0,     0,     6,     0,     0,     6,
       1,     0,     0,     6,     0,     0,     7,     1,     0,     0,
       6,     0,     0,     7,     1,     0,     0,     6,     0,     0,
       7,     0,     0,     6,     0,     0,     6,     1,     3,     3,
       0,     0,     0,     0,    10,     0,     0,     0,     0,    10,
       1,     1,     1,     1,     3,     3,     5,     5,     6,     6,
       8,     8,     1,     3,     0,     2,     2,     1,     2,     1,
       2,     1,     2,     1,     1,     2,     1,     1,     5,     4,
       6,     7,     5,     7,     1,     3,     3,     3,     1,     1,
       3,     4,     4,     1,     3,     3,     0,     3,    10,    10
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

    case YYSYMBOL_expression_any: /* expression_any  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expressions: /* expressions  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_keyword: /* expr_keyword  */
            { delete ((*yyvaluep).pExpression); }
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

    case YYSYMBOL_make_dim: /* make_dim  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_dim_decl: /* make_dim_decl  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_table: /* make_table  */
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

  case 13: /* program: program bitfield_alias_declaration  */
                                                { yyextra->das_has_type_declarations = true; }
    break;

  case 16: /* top_level_reader_macro: expr_reader ';'  */
                             {
        delete (yyvsp[-1].pExpression);    // we do nothing, we don't even attemp to 'visit'
    }
    break;

  case 17: /* optional_public_or_private_module: %empty  */
                        { (yyval.b) = yyextra->g_Program->policies.default_module_public; }
    break;

  case 18: /* optional_public_or_private_module: "public"  */
                        { (yyval.b) = true; }
    break;

  case 19: /* optional_public_or_private_module: "private"  */
                        { (yyval.b) = false; }
    break;

  case 20: /* module_name: '$'  */
                    { (yyval.s) = new string("$"); }
    break;

  case 21: /* module_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 22: /* module_declaration: "module" module_name optional_shared optional_public_or_private_module  */
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

  case 23: /* character_sequence: STRING_CHARACTER  */
                                                            { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
    break;

  case 24: /* character_sequence: STRING_CHARACTER_ESC  */
                                                            { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
    break;

  case 25: /* character_sequence: character_sequence STRING_CHARACTER  */
                                                            { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
    break;

  case 26: /* character_sequence: character_sequence STRING_CHARACTER_ESC  */
                                                            { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
    break;

  case 27: /* string_constant: "start of the string" character_sequence "end of the string"  */
                                                           { (yyval.s) = (yyvsp[-1].s); }
    break;

  case 28: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 29: /* string_builder_body: string_builder_body character_sequence  */
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

  case 30: /* string_builder_body: string_builder_body "{" expr "}"  */
                                                                                {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 31: /* string_builder: "start of the string" string_builder_body "end of the string"  */
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

  case 32: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 33: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 34: /* $@1: %empty  */
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

  case 35: /* expr_reader: '%' name_in_namespace $@1 reader_character_sequence  */
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

  case 36: /* options_declaration: "options" annotation_argument_list  */
                                                   {
        if ( yyextra->g_Program->options.size() ) {
            yyextra->g_Program->options.insert ( yyextra->g_Program->options.begin(),
                (yyvsp[0].aaList)->begin(), (yyvsp[0].aaList)->end() );
        } else {
            swap ( yyextra->g_Program->options, *(yyvsp[0].aaList) );
        }
        auto opt = yyextra->g_Program->options.find("indenting", tInt);
        if (opt)
        {
            if (opt->iValue != 0 && opt->iValue != 2 && opt->iValue != 4 && opt->iValue != 8)//this is error
                yyextra->das_tab_size = yyextra->das_def_tab_size;
            else
                yyextra->das_tab_size = opt->iValue ? opt->iValue : yyextra->das_def_tab_size;//0 is default
            yyextra->g_FileAccessStack.back()->tabSize = yyextra->das_tab_size;
        }
        delete (yyvsp[0].aaList);
    }
    break;

  case 38: /* keyword_or_name: "name"  */
                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 39: /* keyword_or_name: "keyword"  */
                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 40: /* require_module_name: keyword_or_name  */
                              {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 41: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 42: /* require_module_name: require_module_name '.' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 43: /* require_module_name: require_module_name '/' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 44: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 45: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 46: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 47: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 51: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 52: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 53: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 54: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 55: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 56: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 57: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 58: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 59: /* expression_else: "else" expression_block  */
                                                           { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 60: /* expression_else: elif_or_static_elif expr expression_block expression_else  */
                                                                                          {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 61: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 62: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 63: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 64: /* $@2: %empty  */
                      { yyextra->das_need_oxford_comma = true; }
    break;

  case 65: /* expression_else_one_liner: "else" $@2 expression_if_one_liner  */
                                                                                                 {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 66: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 67: /* expression_if_one_liner: expression_return_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 68: /* expression_if_one_liner: expression_yield_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 69: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 70: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 71: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 72: /* $@3: %empty  */
                                                     { yyextra->das_need_oxford_comma = true; }
    break;

  case 73: /* expression_if_then_else: expression_if_one_liner "if" $@3 expr expression_else_one_liner ';'  */
                                                                                                                                                   {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr(ast_wrapInBlock((yyvsp[-5].pExpression))),(yyvsp[-1].pExpression) ? ExpressionPtr(ast_wrapInBlock((yyvsp[-1].pExpression))) : nullptr);
    }
    break;

  case 74: /* expression_for_loop: "for" variable_name_with_pos_list "in" expr_list ';' expression_block  */
                                                                                                           {
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-4].pNameWithPosList),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 75: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 76: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 77: /* expression_with: "with" expr expression_block  */
                                                         {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
    break;

  case 78: /* $@4: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 79: /* expression_with_alias: "assume" "name" '=' $@4 expr  */
                                                                                               {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-3].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-3].s);
    }
    break;

  case 80: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 81: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 82: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 83: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 84: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 85: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 86: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 87: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 88: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 89: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 90: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 91: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 92: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 93: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 94: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 95: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 96: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 97: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 98: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 99: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 100: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 101: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 102: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 103: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 104: /* annotation_declaration_basic: annotation_declaration_name  */
                                          {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[0]));
        if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
    break;

  case 105: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
                                                                                 {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[-3]));
        if ( auto ann = findAnnotation(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
    break;

  case 106: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 107: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 108: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 109: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 110: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 111: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 112: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 113: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 114: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 115: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 116: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 117: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 118: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 119: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 120: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 121: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 122: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 123: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 124: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 125: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 126: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 127: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 128: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 129: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 130: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 131: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 132: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 133: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 134: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 135: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 136: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 137: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 138: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 139: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 140: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 141: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 142: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 143: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 144: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 145: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 146: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 147: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 148: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 149: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 150: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 151: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 152: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 153: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 154: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 155: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 156: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 157: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 158: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 159: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 160: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 161: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 162: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 163: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 164: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 165: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 166: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 167: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 168: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 169: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 170: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 171: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 172: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 173: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 174: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 175: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 176: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 177: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 178: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 179: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 180: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 181: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 182: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 183: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 184: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 185: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 186: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 187: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 188: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 189: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 190: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 191: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 192: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 193: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 194: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 195: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 196: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 197: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 198: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 199: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 200: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 201: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 202: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 203: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 204: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 205: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 206: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 207: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
                                                                                {
        (yyvsp[0].pFuncDecl)->atDecl = tokRangeAt(scanner,(yylsp[-1]),(yylsp[0]));
        assignDefaultArguments((yyvsp[0].pFuncDecl));
        runFunctionAnnotations(scanner, (yyvsp[0].pFuncDecl), (yyvsp[-2].faList), tokAt(scanner,(yylsp[-2])));
        if ( (yyvsp[0].pFuncDecl)->isGeneric() ) {
            if ( !yyextra->g_Program->addGeneric((yyvsp[0].pFuncDecl)) ) {
                das_yyerror(scanner,"generic function is already defined " +
                    (yyvsp[0].pFuncDecl)->getMangledName(),(yyvsp[0].pFuncDecl)->at,
                        CompilationError::function_already_declared);
            }
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

  case 208: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 209: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 210: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 211: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 212: /* $@5: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 213: /* function_declaration: optional_public_or_private_function $@5 function_declaration_header expression_block  */
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

  case 214: /* expression_block: '{' expressions '}'  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 215: /* expression_block: '{' expressions '}' "finally" '{' expressions '}'  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 216: /* expression_any: ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 217: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 218: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 219: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 220: /* expression_any: expr_assign ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 221: /* expression_any: expression_delete ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 222: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 223: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 224: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 225: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 226: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 227: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 228: /* expression_any: expression_break ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 229: /* expression_any: expression_continue ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 230: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 231: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 232: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 233: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 234: /* expression_any: expression_label ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 235: /* expression_any: expression_goto ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 236: /* expression_any: "pass" ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 237: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 238: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 239: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 240: /* expr_keyword: "keyword" expr expression_block  */
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

  case 241: /* $@6: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 242: /* $@7: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 243: /* expression_keyword: "keyword" '<' $@6 type_declaration_no_options '>' $@7 expr  */
                                                                                                                                               {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),*(yyvsp[-6].s));
        auto td = new ExprTypeDecl(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCall->arguments.push_back(ExpressionPtr(td));
        pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 244: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 245: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 246: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 247: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 248: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 249: /* name_in_namespace: "name" "::" "name"  */
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

  case 250: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 251: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 252: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 253: /* $@8: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 254: /* $@9: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 255: /* new_type_declaration: '<' $@8 type_declaration '>' $@9  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 256: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 257: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 258: /* expr_new: "new" new_type_declaration '(' ')'  */
                                                               {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 259: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 260: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 261: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 262: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 263: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 264: /* expression_return_no_pipe: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 265: /* expression_return_no_pipe: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 266: /* expression_return: expression_return_no_pipe ';'  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 267: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 268: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 269: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 270: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 271: /* expression_yield: expression_yield_no_pipe ';'  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 272: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 273: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 274: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 275: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 276: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 277: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 278: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 279: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
    }
    break;

  case 280: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 281: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 282: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr ';'  */
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

  case 283: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 284: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 285: /* $@10: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 286: /* $@11: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 287: /* expr_cast: "cast" '<' $@10 type_declaration_no_options '>' $@11 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 288: /* $@12: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 289: /* $@13: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 290: /* expr_cast: "upcast" '<' $@12 type_declaration_no_options '>' $@13 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 291: /* $@14: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 292: /* $@15: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 293: /* expr_cast: "reinterpret" '<' $@14 type_declaration_no_options '>' $@15 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 294: /* $@16: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 295: /* $@17: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 296: /* expr_type_decl: "type" '<' $@16 type_declaration '>' $@17  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 297: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 298: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 299: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
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

  case 300: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 301: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 302: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 303: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 304: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 305: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 306: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 307: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 308: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 309: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 310: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 311: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 312: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 313: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 314: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 315: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 316: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 317: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 318: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 319: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 320: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 321: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 322: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 323: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 324: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 325: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 326: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 327: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 328: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 329: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 330: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 331: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 332: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 333: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 334: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 335: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 336: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 337: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 338: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 339: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 340: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 341: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 342: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 343: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 344: /* expr_assign_pipe: expr '=' "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 345: /* expr_assign_pipe: expr '=' "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 346: /* expr_assign_pipe: expr '=' "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 347: /* expr_assign_pipe: expr "<-" "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 348: /* expr_assign_pipe: expr "<-" "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 349: /* expr_assign_pipe: expr "<-" "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 350: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 351: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 352: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 353: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 354: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 355: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 356: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 357: /* $@18: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 358: /* $@19: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 359: /* func_addr_expr: '@' '@' '<' $@18 type_declaration_no_options '>' $@19 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 360: /* $@20: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 361: /* $@21: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 362: /* func_addr_expr: '@' '@' '<' $@20 optional_function_argument_list optional_function_type '>' $@21 func_addr_name  */
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

  case 363: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 364: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 365: /* $@22: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 366: /* $@23: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 367: /* expr_field: expr '.' $@22 error $@23  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 368: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 369: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 370: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 371: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 372: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 373: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 374: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 375: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 376: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 377: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 378: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 392: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 393: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 394: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 395: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 396: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 397: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 398: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 399: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 400: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 401: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 402: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 403: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 404: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 405: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 406: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 407: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 408: /* expr: '(' expr ')'  */
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 409: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 410: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 411: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 412: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 413: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 414: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 415: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 416: /* expr: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
    break;

  case 417: /* expr: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
    break;

  case 418: /* expr: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 419: /* expr: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 420: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 421: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 422: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 423: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 424: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 425: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 426: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 427: /* $@24: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 428: /* $@25: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 429: /* expr: expr "is" "type" '<' $@24 type_declaration_no_options '>' $@25  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 430: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 431: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 432: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 433: /* $@26: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 434: /* $@27: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 435: /* expr: expr "as" "type" '<' $@26 type_declaration '>' $@27  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 436: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 437: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 438: /* $@28: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 439: /* $@29: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 440: /* expr: expr '?' "as" "type" '<' $@28 type_declaration '>' $@29  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 441: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 442: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 443: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 444: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 445: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 446: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 447: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 448: /* expr: expr_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 449: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 450: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 451: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 452: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 453: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 454: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 455: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 456: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 457: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 458: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 459: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 460: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 461: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 462: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 463: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 464: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 465: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 466: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 467: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 468: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 469: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 470: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 471: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 472: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 473: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 474: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 475: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 476: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 477: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 478: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 479: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 480: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 481: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 482: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 483: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 484: /* $@30: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 485: /* struct_variable_declaration_list: struct_variable_declaration_list $@30 structure_variable_declaration ';'  */
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

  case 486: /* $@31: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 487: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@31 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 488: /* $@32: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 489: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@32 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 490: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 491: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 492: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 493: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 494: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 495: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 496: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 497: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 498: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 499: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 500: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 501: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 502: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 503: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 504: /* variant_alias_type_list: variant_alias_type_list ';'  */
                                        {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 505: /* variant_alias_type_list: variant_alias_type_list variant_type ';'  */
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

  case 506: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 507: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 508: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 509: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 510: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 511: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 512: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 513: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 514: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 515: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 516: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 517: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 518: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 519: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 520: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 521: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 522: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 523: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 524: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options ';'  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 525: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 526: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 527: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 528: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 529: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 530: /* $@33: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 531: /* global_variable_declaration_list: global_variable_declaration_list $@33 optional_field_annotation let_variable_declaration  */
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

  case 532: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 533: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 534: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 535: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 536: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 537: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 538: /* $@34: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 539: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@34 optional_field_annotation let_variable_declaration  */
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

  case 540: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 541: /* enum_list: enum_list ';'  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 542: /* enum_list: enum_list "name" ';'  */
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

  case 543: /* enum_list: enum_list "name" '=' expr ';'  */
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

  case 544: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 545: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 546: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 547: /* $@35: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 548: /* single_alias: optional_public_or_private_alias "name" $@35 '=' type_declaration  */
                                  {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        (yyvsp[0].pTypeDecl)->isPrivateAlias = !(yyvsp[-4].b);
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

  case 552: /* $@36: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 554: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 555: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 556: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 557: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 558: /* $@37: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 559: /* $@38: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 560: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name '{' $@37 enum_list $@38 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 561: /* $@39: %empty  */
                                                                                                                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 562: /* $@40: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 563: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration '{' $@39 enum_list $@40 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 564: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 565: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 566: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 567: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 568: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 569: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 570: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 571: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 572: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 573: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 574: /* $@41: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 575: /* $@42: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 576: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@41 structure_name $@42 '{' struct_variable_declaration_list '}'  */
                                                                                                                                                   {
        if ( (yyvsp[-4].pStructure) ) {
            ast_structureDeclaration ( scanner, (yyvsp[-8].faList), tokAt(scanner,(yylsp[-7])), (yyvsp[-4].pStructure), tokAt(scanner,(yylsp[-4])), (yyvsp[-1].pVarDeclList) );
            if ( !yyextra->g_CommentReaders.empty() ) {
                auto tak = tokAt(scanner,(yylsp[-7]));
                for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructure((yyvsp[-4].pStructure),tak);
            }
        } else {
            deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
        }
    }
    break;

  case 577: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 578: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 579: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 580: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 581: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 582: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 583: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 584: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 585: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 586: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 587: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 588: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 589: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 590: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 591: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 592: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 593: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 594: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 595: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 596: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 597: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 598: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 599: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 600: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 601: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 602: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 603: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 604: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 605: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 606: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 607: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 608: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 609: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 610: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 611: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 612: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 613: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 614: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 615: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 616: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 617: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 618: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 619: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 620: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 621: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 622: /* bitfield_alias_bits: bitfield_alias_bits ';'  */
                                      {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 623: /* bitfield_alias_bits: bitfield_alias_bits "name" ';'  */
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

  case 624: /* $@43: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 625: /* $@44: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 626: /* bitfield_type_declaration: "bitfield" '<' $@43 bitfield_bits '>' $@44  */
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

  case 627: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 628: /* table_type_pair: type_declaration ';' type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 629: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 630: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 631: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 632: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 633: /* type_declaration_no_options: type_declaration_no_options '[' expr ']'  */
                                                                    {
        int32_t dI = TypeDecl::dimConst;
        if ( (yyvsp[-1].pExpression)->rtti_isConstant() ) {                // note: this shortcut is here so we don`t get extra infer pass on every array
            auto cI = (ExprConst *) (yyvsp[-1].pExpression);
            auto bt = cI->baseType;
            if ( bt==Type::tInt || bt==Type::tUInt ) {
                dI = cast<int32_t>::to(cI->value);
            }
        }
        (yyvsp[-3].pTypeDecl)->dim.push_back(dI);
        (yyvsp[-3].pTypeDecl)->dimExpr.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyvsp[-3].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 634: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 635: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 636: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 637: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 638: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 639: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 640: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 641: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 642: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 643: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 644: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 645: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 646: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 647: /* $@45: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 648: /* $@46: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 649: /* type_declaration_no_options: "smart_ptr" '<' $@45 type_declaration '>' $@46  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 650: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 651: /* $@47: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 652: /* $@48: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 653: /* type_declaration_no_options: "array" '<' $@47 type_declaration '>' $@48  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 654: /* $@49: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 655: /* $@50: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 656: /* type_declaration_no_options: "table" '<' $@49 table_type_pair '>' $@50  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 657: /* $@51: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 658: /* $@52: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 659: /* type_declaration_no_options: "iterator" '<' $@51 type_declaration '>' $@52  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 660: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 661: /* $@53: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 662: /* $@54: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 663: /* type_declaration_no_options: "block" '<' $@53 type_declaration '>' $@54  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 664: /* $@55: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 665: /* $@56: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 666: /* type_declaration_no_options: "block" '<' $@55 optional_function_argument_list optional_function_type '>' $@56  */
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

  case 667: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 668: /* $@57: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 669: /* $@58: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 670: /* type_declaration_no_options: "function" '<' $@57 type_declaration '>' $@58  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 671: /* $@59: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 672: /* $@60: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 673: /* type_declaration_no_options: "function" '<' $@59 optional_function_argument_list optional_function_type '>' $@60  */
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

  case 674: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 675: /* $@61: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 676: /* $@62: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 677: /* type_declaration_no_options: "lambda" '<' $@61 type_declaration '>' $@62  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 678: /* $@63: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 679: /* $@64: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 680: /* type_declaration_no_options: "lambda" '<' $@63 optional_function_argument_list optional_function_type '>' $@64  */
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

  case 681: /* $@65: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 682: /* $@66: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 683: /* type_declaration_no_options: "tuple" '<' $@65 tuple_type_list '>' $@66  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 684: /* $@67: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 685: /* $@68: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 686: /* type_declaration_no_options: "variant" '<' $@67 variant_type_list '>' $@68  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 687: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 688: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 689: /* type_declaration: type_declaration '|' '#'  */
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

  case 690: /* $@69: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 691: /* $@70: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 692: /* $@71: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 693: /* $@72: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 694: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@69 "name" $@70 '{' $@71 variant_alias_type_list $@72 '}'  */
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

  case 695: /* $@73: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 696: /* $@74: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 697: /* $@75: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 698: /* $@76: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 699: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@73 "name" $@74 '{' $@75 bitfield_alias_bits $@76 '}'  */
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

  case 700: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 701: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 702: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 703: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 704: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 705: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 706: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 707: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 708: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 709: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 710: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 711: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 712: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 713: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 714: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 715: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 728: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 729: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 730: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 731: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 732: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 733: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 734: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 735: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 736: /* make_tuple: make_tuple ',' expr  */
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

  case 737: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 738: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 739: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 740: /* make_dim: make_dim ';' make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 741: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
       ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
       (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
       (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 742: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
       ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
       (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
       auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
       (yyval.pExpression) = tam;
    }
    break;

  case 743: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 744: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 745: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 746: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 747: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 748: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 749: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false);
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


