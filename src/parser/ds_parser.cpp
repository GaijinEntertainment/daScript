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
  YYSYMBOL_DAS_TBOOL = 65,                 /* "bool"  */
  YYSYMBOL_DAS_TVOID = 66,                 /* "void"  */
  YYSYMBOL_DAS_TSTRING = 67,               /* "string"  */
  YYSYMBOL_DAS_TAUTO = 68,                 /* "auto"  */
  YYSYMBOL_DAS_TINT = 69,                  /* "int"  */
  YYSYMBOL_DAS_TINT2 = 70,                 /* "int2"  */
  YYSYMBOL_DAS_TINT3 = 71,                 /* "int3"  */
  YYSYMBOL_DAS_TINT4 = 72,                 /* "int4"  */
  YYSYMBOL_DAS_TUINT = 73,                 /* "uint"  */
  YYSYMBOL_DAS_TBITFIELD = 74,             /* "bitfield"  */
  YYSYMBOL_DAS_TUINT2 = 75,                /* "uint2"  */
  YYSYMBOL_DAS_TUINT3 = 76,                /* "uint3"  */
  YYSYMBOL_DAS_TUINT4 = 77,                /* "uint4"  */
  YYSYMBOL_DAS_TFLOAT = 78,                /* "float"  */
  YYSYMBOL_DAS_TFLOAT2 = 79,               /* "float2"  */
  YYSYMBOL_DAS_TFLOAT3 = 80,               /* "float3"  */
  YYSYMBOL_DAS_TFLOAT4 = 81,               /* "float4"  */
  YYSYMBOL_DAS_TRANGE = 82,                /* "range"  */
  YYSYMBOL_DAS_TURANGE = 83,               /* "urange"  */
  YYSYMBOL_DAS_TRANGE64 = 84,              /* "range64"  */
  YYSYMBOL_DAS_TURANGE64 = 85,             /* "urange64"  */
  YYSYMBOL_DAS_TBLOCK = 86,                /* "block"  */
  YYSYMBOL_DAS_TINT64 = 87,                /* "int64"  */
  YYSYMBOL_DAS_TUINT64 = 88,               /* "uint64"  */
  YYSYMBOL_DAS_TDOUBLE = 89,               /* "double"  */
  YYSYMBOL_DAS_TFUNCTION = 90,             /* "function"  */
  YYSYMBOL_DAS_TLAMBDA = 91,               /* "lambda"  */
  YYSYMBOL_DAS_TINT8 = 92,                 /* "int8"  */
  YYSYMBOL_DAS_TUINT8 = 93,                /* "uint8"  */
  YYSYMBOL_DAS_TINT16 = 94,                /* "int16"  */
  YYSYMBOL_DAS_TUINT16 = 95,               /* "uint16"  */
  YYSYMBOL_DAS_TTUPLE = 96,                /* "tuple"  */
  YYSYMBOL_DAS_TVARIANT = 97,              /* "variant"  */
  YYSYMBOL_DAS_GENERATOR = 98,             /* "generator"  */
  YYSYMBOL_DAS_YIELD = 99,                 /* "yield"  */
  YYSYMBOL_DAS_SEALED = 100,               /* "sealed"  */
  YYSYMBOL_ADDEQU = 101,                   /* "+="  */
  YYSYMBOL_SUBEQU = 102,                   /* "-="  */
  YYSYMBOL_DIVEQU = 103,                   /* "/="  */
  YYSYMBOL_MULEQU = 104,                   /* "*="  */
  YYSYMBOL_MODEQU = 105,                   /* "%="  */
  YYSYMBOL_ANDEQU = 106,                   /* "&="  */
  YYSYMBOL_OREQU = 107,                    /* "|="  */
  YYSYMBOL_XOREQU = 108,                   /* "^="  */
  YYSYMBOL_SHL = 109,                      /* "<<"  */
  YYSYMBOL_SHR = 110,                      /* ">>"  */
  YYSYMBOL_ADDADD = 111,                   /* "++"  */
  YYSYMBOL_SUBSUB = 112,                   /* "--"  */
  YYSYMBOL_LEEQU = 113,                    /* "<="  */
  YYSYMBOL_SHLEQU = 114,                   /* "<<="  */
  YYSYMBOL_SHREQU = 115,                   /* ">>="  */
  YYSYMBOL_GREQU = 116,                    /* ">="  */
  YYSYMBOL_EQUEQU = 117,                   /* "=="  */
  YYSYMBOL_NOTEQU = 118,                   /* "!="  */
  YYSYMBOL_RARROW = 119,                   /* "->"  */
  YYSYMBOL_LARROW = 120,                   /* "<-"  */
  YYSYMBOL_QQ = 121,                       /* "??"  */
  YYSYMBOL_QDOT = 122,                     /* "?."  */
  YYSYMBOL_QBRA = 123,                     /* "?["  */
  YYSYMBOL_LPIPE = 124,                    /* "<|"  */
  YYSYMBOL_LBPIPE = 125,                   /* " <|"  */
  YYSYMBOL_LLPIPE = 126,                   /* "$ <|"  */
  YYSYMBOL_LAPIPE = 127,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 128,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 129,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 130,                 /* ":="  */
  YYSYMBOL_ROTL = 131,                     /* "<<<"  */
  YYSYMBOL_ROTR = 132,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 133,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 134,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 135,                    /* "=>"  */
  YYSYMBOL_COLCOL = 136,                   /* "::"  */
  YYSYMBOL_ANDAND = 137,                   /* "&&"  */
  YYSYMBOL_OROR = 138,                     /* "||"  */
  YYSYMBOL_XORXOR = 139,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 140,                /* "&&="  */
  YYSYMBOL_OROREQU = 141,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 142,                /* "^^="  */
  YYSYMBOL_DOTDOT = 143,                   /* ".."  */
  YYSYMBOL_MTAG_E = 144,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 145,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 146,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 147,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 148,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 149,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 150,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 151,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 152,           /* "..."  */
  YYSYMBOL_BRABRAB = 153,                  /* "[["  */
  YYSYMBOL_BRACBRB = 154,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 155,                  /* "{{"  */
  YYSYMBOL_INTEGER = 156,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 157,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 158,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 159,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 160,            /* "unsigned int8 constant"  */
  YYSYMBOL_FLOAT = 161,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 162,                   /* "double constant"  */
  YYSYMBOL_NAME = 163,                     /* "name"  */
  YYSYMBOL_KEYWORD = 164,                  /* "keyword"  */
  YYSYMBOL_BEGIN_STRING = 165,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 166,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 167,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 168,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 169,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 170,          /* "}"  */
  YYSYMBOL_END_OF_READ = 171,              /* "end of failed eader macro"  */
  YYSYMBOL_SEMICOLON_CUR_CUR = 172,        /* ";}}"  */
  YYSYMBOL_SEMICOLON_CUR_SQR = 173,        /* ";}]"  */
  YYSYMBOL_SEMICOLON_SQR_SQR = 174,        /* ";]]"  */
  YYSYMBOL_COMMA_SQR_SQR = 175,            /* ",]]"  */
  YYSYMBOL_COMMA_CUR_SQR = 176,            /* ",}]"  */
  YYSYMBOL_177_ = 177,                     /* ','  */
  YYSYMBOL_178_ = 178,                     /* '='  */
  YYSYMBOL_179_ = 179,                     /* '?'  */
  YYSYMBOL_180_ = 180,                     /* ':'  */
  YYSYMBOL_181_ = 181,                     /* '|'  */
  YYSYMBOL_182_ = 182,                     /* '^'  */
  YYSYMBOL_183_ = 183,                     /* '&'  */
  YYSYMBOL_184_ = 184,                     /* '<'  */
  YYSYMBOL_185_ = 185,                     /* '>'  */
  YYSYMBOL_186_ = 186,                     /* '-'  */
  YYSYMBOL_187_ = 187,                     /* '+'  */
  YYSYMBOL_188_ = 188,                     /* '*'  */
  YYSYMBOL_189_ = 189,                     /* '/'  */
  YYSYMBOL_190_ = 190,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 191,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 192,               /* UNARY_PLUS  */
  YYSYMBOL_193_ = 193,                     /* '~'  */
  YYSYMBOL_194_ = 194,                     /* '!'  */
  YYSYMBOL_PRE_INC = 195,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 196,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 197,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 198,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 199,                    /* DEREF  */
  YYSYMBOL_200_ = 200,                     /* '.'  */
  YYSYMBOL_201_ = 201,                     /* '['  */
  YYSYMBOL_202_ = 202,                     /* ']'  */
  YYSYMBOL_203_ = 203,                     /* '('  */
  YYSYMBOL_204_ = 204,                     /* ')'  */
  YYSYMBOL_205_ = 205,                     /* '$'  */
  YYSYMBOL_206_ = 206,                     /* '@'  */
  YYSYMBOL_207_ = 207,                     /* ';'  */
  YYSYMBOL_208_ = 208,                     /* '{'  */
  YYSYMBOL_209_ = 209,                     /* '}'  */
  YYSYMBOL_210_ = 210,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 211,                 /* $accept  */
  YYSYMBOL_program = 212,                  /* program  */
  YYSYMBOL_optional_public_or_private_module = 213, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 214,              /* module_name  */
  YYSYMBOL_module_declaration = 215,       /* module_declaration  */
  YYSYMBOL_character_sequence = 216,       /* character_sequence  */
  YYSYMBOL_string_constant = 217,          /* string_constant  */
  YYSYMBOL_string_builder_body = 218,      /* string_builder_body  */
  YYSYMBOL_string_builder = 219,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 220, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 221,              /* expr_reader  */
  YYSYMBOL_222_1 = 222,                    /* $@1  */
  YYSYMBOL_options_declaration = 223,      /* options_declaration  */
  YYSYMBOL_require_declaration = 224,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 225,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 226,      /* require_module_name  */
  YYSYMBOL_require_module = 227,           /* require_module  */
  YYSYMBOL_is_public_module = 228,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 229,       /* expect_declaration  */
  YYSYMBOL_expect_list = 230,              /* expect_list  */
  YYSYMBOL_expect_error = 231,             /* expect_error  */
  YYSYMBOL_expression_label = 232,         /* expression_label  */
  YYSYMBOL_expression_goto = 233,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 234,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 235,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 236,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 237, /* expression_else_one_liner  */
  YYSYMBOL_238_2 = 238,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 239,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 240,  /* expression_if_then_else  */
  YYSYMBOL_241_3 = 241,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 242,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 243,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 244,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 245,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 246,    /* expression_with_alias  */
  YYSYMBOL_247_4 = 247,                    /* $@4  */
  YYSYMBOL_annotation_argument_value = 248, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 249, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 250, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 251,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 252, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 253, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 254, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 255,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 256,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 257, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 258, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 259,   /* optional_function_type  */
  YYSYMBOL_function_name = 260,            /* function_name  */
  YYSYMBOL_global_function_declaration = 261, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 262, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 263, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 264,     /* function_declaration  */
  YYSYMBOL_265_5 = 265,                    /* $@5  */
  YYSYMBOL_expression_block = 266,         /* expression_block  */
  YYSYMBOL_expression_any = 267,           /* expression_any  */
  YYSYMBOL_expressions = 268,              /* expressions  */
  YYSYMBOL_expr_keyword = 269,             /* expr_keyword  */
  YYSYMBOL_expression_keyword = 270,       /* expression_keyword  */
  YYSYMBOL_271_6 = 271,                    /* $@6  */
  YYSYMBOL_272_7 = 272,                    /* $@7  */
  YYSYMBOL_expr_pipe = 273,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 274,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 275,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 276,     /* new_type_declaration  */
  YYSYMBOL_277_8 = 277,                    /* $@8  */
  YYSYMBOL_278_9 = 278,                    /* $@9  */
  YYSYMBOL_expr_new = 279,                 /* expr_new  */
  YYSYMBOL_expression_break = 280,         /* expression_break  */
  YYSYMBOL_expression_continue = 281,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 282, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 283,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 284, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 285,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 286,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 287,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 288,        /* optional_in_scope  */
  YYSYMBOL_expression_let = 289,           /* expression_let  */
  YYSYMBOL_expr_cast = 290,                /* expr_cast  */
  YYSYMBOL_291_10 = 291,                   /* $@10  */
  YYSYMBOL_292_11 = 292,                   /* $@11  */
  YYSYMBOL_293_12 = 293,                   /* $@12  */
  YYSYMBOL_294_13 = 294,                   /* $@13  */
  YYSYMBOL_295_14 = 295,                   /* $@14  */
  YYSYMBOL_296_15 = 296,                   /* $@15  */
  YYSYMBOL_expr_type_decl = 297,           /* expr_type_decl  */
  YYSYMBOL_298_16 = 298,                   /* $@16  */
  YYSYMBOL_299_17 = 299,                   /* $@17  */
  YYSYMBOL_expr_type_info = 300,           /* expr_type_info  */
  YYSYMBOL_expr_list = 301,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 302,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 303,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 304,            /* capture_entry  */
  YYSYMBOL_capture_list = 305,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 306,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 307,               /* expr_block  */
  YYSYMBOL_expr_numeric_const = 308,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 309,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 310,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 311,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 312,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 313,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 314,           /* func_addr_expr  */
  YYSYMBOL_315_18 = 315,                   /* $@18  */
  YYSYMBOL_316_19 = 316,                   /* $@19  */
  YYSYMBOL_317_20 = 317,                   /* $@20  */
  YYSYMBOL_318_21 = 318,                   /* $@21  */
  YYSYMBOL_expr_field = 319,               /* expr_field  */
  YYSYMBOL_320_22 = 320,                   /* $@22  */
  YYSYMBOL_321_23 = 321,                   /* $@23  */
  YYSYMBOL_expr = 322,                     /* expr  */
  YYSYMBOL_323_24 = 323,                   /* $@24  */
  YYSYMBOL_324_25 = 324,                   /* $@25  */
  YYSYMBOL_325_26 = 325,                   /* $@26  */
  YYSYMBOL_326_27 = 326,                   /* $@27  */
  YYSYMBOL_327_28 = 327,                   /* $@28  */
  YYSYMBOL_328_29 = 328,                   /* $@29  */
  YYSYMBOL_expr_mtag = 329,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 330, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 331,        /* optional_override  */
  YYSYMBOL_optional_constant = 332,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 333, /* optional_public_or_private_member_variable  */
  YYSYMBOL_structure_variable_declaration = 334, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 335, /* struct_variable_declaration_list  */
  YYSYMBOL_336_30 = 336,                   /* $@30  */
  YYSYMBOL_337_31 = 337,                   /* $@31  */
  YYSYMBOL_338_32 = 338,                   /* $@32  */
  YYSYMBOL_function_argument_declaration = 339, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 340,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 341,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 342,          /* tuple_type_list  */
  YYSYMBOL_variant_type = 343,             /* variant_type  */
  YYSYMBOL_variant_type_list = 344,        /* variant_type_list  */
  YYSYMBOL_copy_or_move = 345,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 346,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 347,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 348,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 349, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 350, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 351, /* global_variable_declaration_list  */
  YYSYMBOL_352_33 = 352,                   /* $@33  */
  YYSYMBOL_optional_shared = 353,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 354, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 355,               /* global_let  */
  YYSYMBOL_356_34 = 356,                   /* $@34  */
  YYSYMBOL_enum_list = 357,                /* enum_list  */
  YYSYMBOL_single_alias = 358,             /* single_alias  */
  YYSYMBOL_359_35 = 359,                   /* $@35  */
  YYSYMBOL_alias_list = 360,               /* alias_list  */
  YYSYMBOL_alias_declaration = 361,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 362, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 363,                /* enum_name  */
  YYSYMBOL_enum_declaration = 364,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 365, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 366,          /* optional_sealed  */
  YYSYMBOL_structure_name = 367,           /* structure_name  */
  YYSYMBOL_class_or_struct = 368,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 369, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 370,    /* structure_declaration  */
  YYSYMBOL_371_36 = 371,                   /* $@36  */
  YYSYMBOL_372_37 = 372,                   /* $@37  */
  YYSYMBOL_variable_name_with_pos_list = 373, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 374,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 375, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 376, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 377,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 378,            /* bitfield_bits  */
  YYSYMBOL_bitfield_type_declaration = 379, /* bitfield_type_declaration  */
  YYSYMBOL_380_38 = 380,                   /* $@38  */
  YYSYMBOL_381_39 = 381,                   /* $@39  */
  YYSYMBOL_table_type_pair = 382,          /* table_type_pair  */
  YYSYMBOL_type_declaration_no_options = 383, /* type_declaration_no_options  */
  YYSYMBOL_384_40 = 384,                   /* $@40  */
  YYSYMBOL_385_41 = 385,                   /* $@41  */
  YYSYMBOL_386_42 = 386,                   /* $@42  */
  YYSYMBOL_387_43 = 387,                   /* $@43  */
  YYSYMBOL_388_44 = 388,                   /* $@44  */
  YYSYMBOL_389_45 = 389,                   /* $@45  */
  YYSYMBOL_390_46 = 390,                   /* $@46  */
  YYSYMBOL_391_47 = 391,                   /* $@47  */
  YYSYMBOL_392_48 = 392,                   /* $@48  */
  YYSYMBOL_393_49 = 393,                   /* $@49  */
  YYSYMBOL_394_50 = 394,                   /* $@50  */
  YYSYMBOL_395_51 = 395,                   /* $@51  */
  YYSYMBOL_396_52 = 396,                   /* $@52  */
  YYSYMBOL_397_53 = 397,                   /* $@53  */
  YYSYMBOL_398_54 = 398,                   /* $@54  */
  YYSYMBOL_399_55 = 399,                   /* $@55  */
  YYSYMBOL_400_56 = 400,                   /* $@56  */
  YYSYMBOL_401_57 = 401,                   /* $@57  */
  YYSYMBOL_402_58 = 402,                   /* $@58  */
  YYSYMBOL_403_59 = 403,                   /* $@59  */
  YYSYMBOL_404_60 = 404,                   /* $@60  */
  YYSYMBOL_405_61 = 405,                   /* $@61  */
  YYSYMBOL_406_62 = 406,                   /* $@62  */
  YYSYMBOL_407_63 = 407,                   /* $@63  */
  YYSYMBOL_type_declaration = 408,         /* type_declaration  */
  YYSYMBOL_variant_alias_declaration = 409, /* variant_alias_declaration  */
  YYSYMBOL_410_64 = 410,                   /* $@64  */
  YYSYMBOL_411_65 = 411,                   /* $@65  */
  YYSYMBOL_bitfield_alias_declaration = 412, /* bitfield_alias_declaration  */
  YYSYMBOL_413_66 = 413,                   /* $@66  */
  YYSYMBOL_make_decl = 414,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 415,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 416,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 417,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 418, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 419, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 420, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 421, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 422, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_make_struct_decl = 423,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 424,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 425,           /* make_map_tuple  */
  YYSYMBOL_make_dim = 426,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 427,            /* make_dim_decl  */
  YYSYMBOL_make_table = 428,               /* make_table  */
  YYSYMBOL_make_table_decl = 429,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 430, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 431       /* array_comprehension  */
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
#define YYLAST   11088

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  211
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  221
/* YYNRULES -- Number of rules.  */
#define YYNRULES  717
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1246

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   438


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
       2,     2,     2,   194,     2,   210,   205,   190,   183,     2,
     203,   204,   188,   187,   177,   186,   200,   189,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   180,   207,
     184,   178,   185,   179,   206,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   201,     2,   202,   182,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   208,   181,   209,   193,     2,     2,     2,
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
     175,   176,   191,   192,   195,   196,   197,   198,   199
};

#if DAS_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   499,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   515,   516,   517,   521,   522,
     526,   544,   545,   546,   547,   551,   555,   560,   569,   577,
     593,   598,   606,   606,   636,   657,   661,   662,   666,   669,
     673,   679,   688,   691,   697,   698,   702,   706,   707,   711,
     714,   720,   726,   729,   735,   736,   740,   741,   742,   751,
     752,   756,   757,   757,   763,   764,   765,   766,   767,   771,
     777,   777,   783,   789,   797,   807,   816,   816,   823,   824,
     825,   826,   827,   828,   832,   837,   845,   846,   847,   851,
     852,   853,   854,   855,   856,   857,   858,   864,   867,   873,
     874,   875,   879,   887,   900,   903,   911,   922,   933,   944,
     947,   954,   958,   965,   966,   970,   971,   972,   976,   979,
     986,   990,   991,   992,   993,   994,   995,   996,   997,   998,
     999,  1000,  1001,  1002,  1003,  1004,  1005,  1006,  1007,  1008,
    1009,  1010,  1011,  1012,  1013,  1014,  1015,  1016,  1017,  1018,
    1019,  1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,
    1029,  1030,  1031,  1032,  1033,  1034,  1035,  1036,  1037,  1038,
    1039,  1040,  1041,  1042,  1043,  1044,  1045,  1046,  1047,  1048,
    1049,  1050,  1051,  1052,  1053,  1054,  1055,  1056,  1057,  1058,
    1059,  1060,  1061,  1062,  1063,  1064,  1065,  1066,  1067,  1068,
    1069,  1070,  1071,  1072,  1073,  1078,  1100,  1101,  1102,  1106,
    1112,  1112,  1129,  1133,  1144,  1145,  1146,  1147,  1148,  1149,
    1150,  1151,  1152,  1153,  1154,  1155,  1156,  1157,  1158,  1159,
    1160,  1161,  1162,  1163,  1164,  1168,  1173,  1179,  1185,  1196,
    1196,  1196,  1207,  1241,  1244,  1247,  1253,  1254,  1265,  1269,
    1272,  1280,  1280,  1280,  1283,  1289,  1292,  1295,  1299,  1305,
    1309,  1313,  1316,  1319,  1327,  1330,  1333,  1341,  1344,  1352,
    1355,  1358,  1366,  1372,  1373,  1377,  1378,  1382,  1388,  1388,
    1388,  1391,  1391,  1391,  1396,  1396,  1396,  1404,  1404,  1404,
    1410,  1420,  1431,  1446,  1449,  1455,  1456,  1463,  1474,  1475,
    1476,  1480,  1481,  1482,  1483,  1487,  1492,  1500,  1501,  1505,
    1510,  1517,  1518,  1519,  1520,  1521,  1522,  1523,  1527,  1528,
    1529,  1530,  1531,  1532,  1533,  1534,  1535,  1536,  1537,  1538,
    1539,  1540,  1541,  1542,  1543,  1544,  1545,  1549,  1550,  1551,
    1552,  1553,  1554,  1558,  1565,  1577,  1582,  1592,  1596,  1603,
    1606,  1606,  1606,  1611,  1611,  1611,  1624,  1628,  1632,  1632,
    1632,  1639,  1640,  1641,  1642,  1643,  1644,  1645,  1646,  1647,
    1648,  1649,  1650,  1651,  1652,  1653,  1654,  1655,  1656,  1657,
    1658,  1659,  1660,  1661,  1662,  1663,  1664,  1665,  1666,  1667,
    1668,  1669,  1670,  1671,  1672,  1673,  1674,  1680,  1681,  1682,
    1683,  1684,  1685,  1686,  1687,  1688,  1689,  1690,  1691,  1692,
    1696,  1700,  1703,  1706,  1707,  1708,  1709,  1712,  1715,  1716,
    1719,  1719,  1719,  1722,  1727,  1731,  1735,  1735,  1735,  1740,
    1743,  1747,  1747,  1747,  1752,  1755,  1756,  1757,  1758,  1759,
    1760,  1761,  1762,  1763,  1764,  1765,  1770,  1774,  1775,  1776,
    1777,  1778,  1779,  1780,  1784,  1788,  1792,  1796,  1800,  1804,
    1808,  1812,  1816,  1823,  1824,  1828,  1829,  1830,  1834,  1835,
    1839,  1840,  1841,  1845,  1855,  1858,  1858,  1877,  1876,  1891,
    1890,  1903,  1912,  1918,  1923,  1933,  1934,  1938,  1941,  1950,
    1951,  1955,  1964,  1965,  1970,  1971,  1975,  1981,  1987,  1990,
    1994,  2000,  2009,  2010,  2011,  2015,  2016,  2020,  2027,  2032,
    2041,  2047,  2058,  2061,  2066,  2071,  2079,  2090,  2093,  2093,
    2113,  2114,  2118,  2119,  2120,  2124,  2127,  2127,  2146,  2149,
    2152,  2161,  2174,  2174,  2195,  2196,  2200,  2201,  2205,  2206,
    2207,  2211,  2221,  2228,  2238,  2239,  2243,  2244,  2248,  2254,
    2255,  2259,  2260,  2261,  2265,  2270,  2265,  2284,  2291,  2296,
    2305,  2311,  2322,  2323,  2324,  2325,  2326,  2327,  2328,  2329,
    2330,  2331,  2332,  2333,  2334,  2335,  2336,  2337,  2338,  2339,
    2340,  2341,  2342,  2343,  2344,  2345,  2346,  2347,  2348,  2352,
    2353,  2354,  2355,  2356,  2357,  2361,  2372,  2376,  2383,  2395,
    2402,  2411,  2411,  2411,  2424,  2428,  2435,  2436,  2437,  2438,
    2439,  2453,  2459,  2463,  2467,  2472,  2477,  2482,  2487,  2491,
    2495,  2500,  2504,  2508,  2513,  2513,  2513,  2519,  2526,  2526,
    2526,  2531,  2531,  2531,  2537,  2537,  2537,  2542,  2546,  2546,
    2546,  2551,  2551,  2551,  2560,  2564,  2564,  2564,  2569,  2569,
    2569,  2578,  2582,  2582,  2582,  2587,  2587,  2587,  2596,  2596,
    2596,  2602,  2602,  2602,  2611,  2614,  2625,  2641,  2641,  2641,
    2665,  2665,  2685,  2686,  2687,  2688,  2692,  2699,  2706,  2712,
    2721,  2726,  2733,  2734,  2739,  2740,  2744,  2745,  2749,  2750,
    2754,  2755,  2756,  2760,  2761,  2762,  2767,  2773,  2780,  2788,
    2795,  2803,  2815,  2818,  2824,  2838,  2844,  2850,  2855,  2862,
    2867,  2877,  2882,  2889,  2901,  2902,  2906,  2909
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
  "\"private\"", "\"smart_ptr\"", "\"unsafe\"", "\"inscope\"", "\"bool\"",
  "\"void\"", "\"string\"", "\"auto\"", "\"int\"", "\"int2\"", "\"int3\"",
  "\"int4\"", "\"uint\"", "\"bitfield\"", "\"uint2\"", "\"uint3\"",
  "\"uint4\"", "\"float\"", "\"float2\"", "\"float3\"", "\"float4\"",
  "\"range\"", "\"urange\"", "\"range64\"", "\"urange64\"", "\"block\"",
  "\"int64\"", "\"uint64\"", "\"double\"", "\"function\"", "\"lambda\"",
  "\"int8\"", "\"uint8\"", "\"int16\"", "\"uint16\"", "\"tuple\"",
  "\"variant\"", "\"generator\"", "\"yield\"", "\"sealed\"", "\"+=\"",
  "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"", "\"^=\"",
  "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"", "\">>=\"",
  "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"", "\"??\"", "\"?.\"",
  "\"?[\"", "\"<|\"", "\" <|\"", "\"$ <|\"", "\"@ <|\"", "\"@@ <|\"",
  "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"",
  "\"=>\"", "\"::\"", "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"", "\"||=\"",
  "\"^^=\"", "\"..\"", "\"$$\"", "\"$i\"", "\"$v\"", "\"$b\"", "\"$a\"",
  "\"$t\"", "\"$c\"", "\"$f\"", "\"...\"", "\"[[\"", "\"[{\"", "\"{{\"",
  "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"unsigned int8 constant\"", "\"floating point constant\"",
  "\"double constant\"", "\"name\"", "\"keyword\"",
  "\"start of the string\"", "STRING_CHARACTER", "STRING_CHARACTER_ESC",
  "\"end of the string\"", "\"{\"", "\"}\"",
  "\"end of failed eader macro\"", "\";}}\"", "\";}]\"", "\";]]\"",
  "\",]]\"", "\",}]\"", "','", "'='", "'?'", "':'", "'|'", "'^'", "'&'",
  "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'", "UNARY_MINUS",
  "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC",
  "DEREF", "'.'", "'['", "']'", "'('", "')'", "'$'", "'@'", "';'", "'{'",
  "'}'", "'#'", "$accept", "program", "optional_public_or_private_module",
  "module_name", "module_declaration", "character_sequence",
  "string_constant", "string_builder_body", "string_builder",
  "reader_character_sequence", "expr_reader", "$@1", "options_declaration",
  "require_declaration", "keyword_or_name", "require_module_name",
  "require_module", "is_public_module", "expect_declaration",
  "expect_list", "expect_error", "expression_label", "expression_goto",
  "elif_or_static_elif", "expression_else", "if_or_static_if",
  "expression_else_one_liner", "$@2", "expression_if_one_liner",
  "expression_if_then_else", "$@3", "expression_for_loop",
  "expression_unsafe", "expression_while_loop", "expression_with",
  "expression_with_alias", "$@4", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "annotation_declaration_name", "annotation_declaration_basic",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@5", "expression_block", "expression_any",
  "expressions", "expr_keyword", "expression_keyword", "$@6", "$@7",
  "expr_pipe", "name_in_namespace", "expression_delete",
  "new_type_declaration", "$@8", "$@9", "expr_new", "expression_break",
  "expression_continue", "expression_return_no_pipe", "expression_return",
  "expression_yield_no_pipe", "expression_yield", "expression_try_catch",
  "kwd_let", "optional_in_scope", "expression_let", "expr_cast", "$@10",
  "$@11", "$@12", "$@13", "$@14", "$@15", "expr_type_decl", "$@16", "$@17",
  "expr_type_info", "expr_list", "block_or_simple_block",
  "block_or_lambda", "capture_entry", "capture_list",
  "optional_capture_list", "expr_block", "expr_numeric_const",
  "expr_assign", "expr_assign_pipe", "expr_named_call", "expr_method_call",
  "func_addr_name", "func_addr_expr", "$@18", "$@19", "$@20", "$@21",
  "expr_field", "$@22", "$@23", "expr", "$@24", "$@25", "$@26", "$@27",
  "$@28", "$@29", "expr_mtag", "optional_field_annotation",
  "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "$@30", "$@31", "$@32", "function_argument_declaration",
  "function_argument_list", "tuple_type", "tuple_type_list",
  "variant_type", "variant_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@33", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@34", "enum_list",
  "single_alias", "$@35", "alias_list", "alias_declaration",
  "optional_public_or_private_enum", "enum_name", "enum_declaration",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "structure_declaration", "$@36", "$@37", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "$@38", "$@39", "table_type_pair",
  "type_declaration_no_options", "$@40", "$@41", "$@42", "$@43", "$@44",
  "$@45", "$@46", "$@47", "$@48", "$@49", "$@50", "$@51", "$@52", "$@53",
  "$@54", "$@55", "$@56", "$@57", "$@58", "$@59", "$@60", "$@61", "$@62",
  "$@63", "type_declaration", "variant_alias_declaration", "$@64", "$@65",
  "bitfield_alias_declaration", "$@66", "make_decl", "make_struct_fields",
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

#define YYPACT_NINF (-888)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-656)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -888,    58,  -888,  -888,    35,   -73,   263,  -108,  -888,   -97,
    -888,  -888,     6,  -888,  -888,  -888,  -888,  -888,   376,  -888,
      32,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,
     -20,  -888,   -13,    68,    87,  -888,  -888,  -888,   263,  -888,
      67,  -888,  -888,   122,  -888,  -888,  -888,    32,   124,   236,
    -888,  -888,     6,   287,   193,     6,     6,   312,  -888,   -64,
      66,  -888,  -888,  -888,    29,   170,   310,  -888,   375,    17,
      35,   377,   -73,   351,   401,  -888,   360,   360,  -888,   393,
     370,  -106,   416,   379,  -888,  -888,  -888,   429,  -888,   -68,
      35,     6,     6,     6,     6,  -888,  -888,  -888,  -888,  -888,
    -888,  -888,   446,  -888,  -888,  -888,  -888,  -888,   411,  -888,
    -888,  -888,  -888,  -888,   418,   148,  -888,  -888,  -888,  -888,
     570,  -888,  -888, 10737,  -888,  -888,   424,  -888,  -888,  -888,
     471,   430,  -888,  -888,    59,  -888,   292,   502,   -64, 10925,
    -888,   -63,   540,  -888,   488,  -888,  -888,   456,  -888,  -888,
    -888,  -888,  -888,  -888,  -888,   155,  -888,   458,   459,   460,
     462,  -888,  -888,  -888,   444,  -888,  -888,  -888,  -888,  -888,
     464,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,
    -888,  -888,   465,  -888,  -888,  -888,   466,   468,  -888,  -888,
    -888,  -888,   470,   475,   452,  -888,  -888,  -888,  -888,  -888,
     275,   479,  -888,  -888,   473,   500,  -888,  9536,  -888,  -888,
    -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,
    -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,
    -888,  -888,  -888,   631,   647,  -888,   483,   484,   445,  -888,
    -888,   520,  -888,   478,    35,   325,  -888,  -888,  -888,   148,
    -888,  -888,  -888,  -888,  -888,   525,  -888,   260,   293,   301,
    -888,  -888,  6545,  -888,  -888,  -888,    18,  -888,  -888,  -888,
      88,  3644,  -888,  7151,    12,   511,  -888,   486,   531,   532,
    -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,
    -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,
     533,   495,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,
    -888,  -888,  -888,  -888,   678,  -888,  -888,  -888,  -888,  -888,
    -888,  -888,  -888,  -888,  -888,  -888,  -888,   537,   499,  -888,
    -888,   -88,   522,  -888,  -888,  -888,  -888,  -888,  -888,  -888,
    -888,   496,   -42,   523,   498,  -888,   488,   115,   507,   670,
     365,  -888,  -888, 10737, 10737, 10737, 10737,   501,   471, 10737,
     483, 10737,   483, 10737,   483, 10836,   500,  -888,  -888,   329,
     509,   529,  -888,   513,   530,   539,   514,   541,   517,  -888,
     542,  6545,  6545,   521,   524,   528,   538,   544,   545,  -888,
   10380, 10479,  6545,  -888,  -888,  -888,  -888,  -888,  -888,  -888,
     551,  -888,  6545,  6545,  6545,   189,  6545,  6545,  6545,  -888,
     534,  -888,  -888,  -888,  -888,    26,  -888,  -888,  -888,  -888,
     535,  -888,  -888,  -888,  -888,  -888,  -888,  7140,  -888,   546,
    -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,   543,
    -888,  -888,  9066,  -888,   275,  -888,  -888, 10737,    40,  -888,
    -888,  -888,  -888,   576,   612,  -888,   547,  -888,     8,  -888,
     389, 10737,  -888,  1553,  -888,  -129,  -888,  -888,   189,  -888,
    -888,   325,   549,  6545,   580,   581, 10737,  -888,   -34,   261,
     567,  -109,   306,   369,  -888,   267,   378,   522,   380,   522,
     385,   522,   -75,  -888,   320,   479,   321,  -888,   550,  -888,
    -888,   189,  -888,  6545,  -888,  -888,  6545,  -888,  6545, 10737,
      73,    73,  6545,  6545,  6545,  6545,  6545,  6545,   362,  1946,
     362,  2127,  9571,  -888,    69,  -888,   448,    73,    73,   -71,
    -888,    73,    73,  7246,   143,  -888,  3463,   601, 10512, 10607,
    6545,  6545,  -888,  -888,  6545,  6545,  6545,  6545,   592,  6545,
       9,  6545,  6545,  6545,  6545,  6545,  6545,  6545,  6545,  6545,
    3802,  6545,  6545,  6545,  6545,  6545,  6545,  6545,  6545,  6545,
    6545,   168,  6545,  -888,  3960,  -888,  -888,   479,  -888,  -888,
    -888,  -888,  6545,   362,   554,   715,  -888,   -52,  -888,   363,
     479,  -888,  6545,  -888,  -888,   362,  2489,  -888,   484,  4141,
    6545,   595,  -888,   552,   604,  4299,   -29,  2647,   338,   338,
     338,  4457,  -888,   725,   555,   556,  6545,   755,  -888,  -888,
    -888,  -888,  -888,  -888,  -888,  -888,  -888,   558,   559,   560,
     561,  -888,   562,  -888,  -888,   706,  -888,   -80,  -888,  1248,
     -39,  6545,  -888,  -888,    21,  -888,  -888,  7281,  -888,   731,
     265,  -888,  -888,  -888,  2989,  -888,  -888, 10737,  -888,  -888,
    -888,   615,  -888,   594,  -888,   599,  -888,   605, 10737,  -888,
   10836,  -888,   500, 10737,  4638,  4819, 10737,  7386, 10737, 10737,
    7421, 10737,  7526,   129,  7561,  7666,  7701,  7806,  7841,  7946,
      14,   338,   -35,  2308,  5000,  9665,   614,   -17,   245,   617,
     103,    16,  5181,   -17,   128,  6545,  -888,  6545,   583,  -888,
   10737,  -888,  6545,   437,   634,  -888,   598,   600,   318,  -888,
    -888,   641,  -888,   173,  9880,     3,   483,   613,   602,  -888,
    -888,   622,   606,  -888,  -888,    94,    94,  1104,  1104,   883,
     883,   607,    27,   608,  -888,  9101,   -60,   -60,    94,    94,
   10150, 10035, 10121,  9915, 10638,  9759,  1210, 10236,   664,  1104,
    1104,   279,   279,    27,    27,    27,   210,  6545,   609,  -888,
     259,  6545,   806,  9195,  -888,   217,  7981,  -888,  6545,   645,
    -888,   650,  -888, 10737,  -888,  2989,  -888,  6645,    22,  2989,
    -888,   689,  9429,   802,  6545,  9880,  6645,   638,  -888,   637,
     662,  9880,  -888,  2989,  -888,  9429,   616,  -888,  -888,  -888,
    6645,   611,  -888,  -888,  6645,  -888,  -888,  -888,  -888,  -888,
    -888,  -888,   325,   338,  -888,  6545,  6545,  6545,  6545,  6545,
    6545,  6545,  6545,  6545,  6545,  3147,  6545,  6545,  6545,  6545,
    6545,  6545,  3305,  -888,  6902,     6,  -888,   813,   488,  -888,
     658,  -888,  2989,  -888,  6758,  -888,  -888,   479,  -888,  -888,
    -888,  -888,  -888,  -888,  -888,  -888,  -888,   479,  -888,  -888,
    -888,   388,  -888,   228,   660,  8086,   397,  -888,   316,   536,
    -888,   799,  -888,   601,  -888,  -888,  -888,  -888,  -888,   621,
    6545,  -888,  6545,  6545,    -9,  6545,   663,   641,   245,  -888,
    -888,   623,  -888,  6545,  -888,   625,  6545,  -888,  6545,   641,
     145,  -888,   626,  -888,  9880,  -888,  -888,   906,  9794,  -888,
     671,  6545,  6545, 10737,   483,   -21,   126,  5362,  -888,   667,
     673,   681,   683,  -888,   185,   522,  -888,  6545,  -888,  6545,
    5528,  6545,  -888,   672,   652,  -888,  -888,  6545,   657,  -888,
    9289,  6545,   659,  -888,  9324,  -888,  -888,  6545,  -888,  -888,
    8121,  -888,   820,   -44,  -888,  9429,  -888,  6545,  -888,  9429,
    6545,  6545,   484,  9880,  -888,  -888,  -888,  -888,  -888,  9429,
    -888,  -888,  -888,   497,  6545,  -888,  -888,  9880,  9880,  9880,
    9880,  9880,  9880,  9880,  9880,  9880,  9880,   338,   338,   338,
    9880,  9880,  9880,  9880,  9880,  9880,  9880,   338,   338,   338,
    9880,  -888,   239,   480,     2,   656,  -888,  -888,  6865,  -888,
    -888,  -888,  -888,  -888,  -888,   327,  -888,  -888,  -888,  -888,
    -888,   665,  5709,   -30,  9880,  9880,   -17,   245,  9880,    46,
     614,  -888,  -888,  9880,  -888,   617,    95,   -17,  -888,  -888,
     674,  -888,  -888,  -888,  -888,  -888,  8226,  8261,  1286,   522,
     693,   641,  9880,  -888,  -888,  -888,  -888,     3,   697,   -98,
   10737,  8366, 10737,  8401,  -888,   235,  8506,  -888,  6545, 10001,
    6545,  -888,  8541,  6545,  -888,  -888,  -888,   711,  6545,   109,
    -888,  6545,  1762,   484,  -888,  -888,  6545,  -888,   489,  -888,
    -888,  -888,  -888,  -888,  -888,   694,  -888,  -888,   166,  -888,
    -888,   480,  -888,  -888,  -888,  6545,   737,  -888,  6545,  6545,
    6545,  5890,  -888,   251,  6545,   245,  -888,  6545,  6545,  6545,
     145,  -888,  6545,  -888,  -888,  -888,   684,  -888,   257,  -888,
    -888,  6071,  -888,  -888,  1477,  -888,   398,  -888,  -888,  -888,
   10737,  8646,  8681,  -888,  8786,  -888,  9880,   484,  9880,  -888,
    -888,  6645,  -888,   695,  -888,   873,   873,   362,  8821,   721,
    1104,  1104,  1104,  -888,  8926,  -888,  7007,  -888,  9880,  9880,
    7007,  -888,  1104,   200,  -888,   703,  6545,  9915,  -888,  -888,
     405,  -888,  -888,  -888,  -888,   497,  2831,  -888,  -888,  -888,
    -888,  -888,  -888,  6545,  -888,   858,   708,   704,  -888,   200,
    -888, 10001,  -888,  -888,  -888,  -888,  6229,  6387,  -888,  -888,
    -888,  -888,  -888,  9880, 10925, 10925,  8961,  6545,   710,   712,
    -888,  -888,  6545,  9880,  6545,  9880,   714,   484,  -888,  9880,
    -888,  -888,  9880,  9880,  -888,  -888
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   113,     1,   273,     0,     0,     0,     0,   274,     0,
     670,   667,     0,    14,     3,    10,     9,     8,     0,     7,
     520,     6,    11,     5,     4,    12,    13,    87,    88,    86,
      95,    97,    34,    49,    46,    47,    36,    37,     0,    38,
      44,    35,   532,     0,   537,    19,    18,   520,     0,     0,
     100,   101,     0,     0,   246,     0,     0,   102,   104,   111,
       0,    99,   550,   549,   206,   538,   551,   521,   522,     0,
       0,     0,     0,    39,     0,    45,     0,     0,    42,     0,
       0,     0,    15,     0,   668,   110,   248,     0,   105,     0,
       0,     0,     0,     0,     0,   114,   208,   207,   210,   205,
     540,   539,     0,   553,   552,   554,   524,   523,   526,    93,
      94,    91,    92,    90,     0,     0,    89,    98,    50,    48,
      44,    41,    40,     0,   534,   536,     0,    16,    17,    20,
       0,     0,   247,   109,     0,   106,   107,   108,   112,     0,
     541,     0,   546,   517,   463,    21,    22,     0,    82,    83,
      80,    81,    79,    78,    84,     0,    43,     0,     0,     0,
       0,   562,   582,   563,   596,   564,   568,   569,   570,   571,
     588,   575,   576,   577,   578,   579,   580,   581,   583,   584,
     585,   586,   637,   567,   574,   587,   644,   651,   565,   572,
     566,   573,     0,     0,     0,   595,   606,   609,   607,   608,
     664,   533,   535,   599,     0,     0,   103,     0,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,     0,     0,   120,   115,     0,     0,   528,
     547,     0,   555,   518,     0,     0,    23,    24,    25,     0,
      96,   628,   631,   634,   624,     0,   601,   638,   645,   652,
     658,   661,     0,   614,   619,   613,     0,   627,   623,   616,
       0,     0,   618,     0,     0,     0,   492,     0,   174,   175,
     172,   123,   124,   126,   125,   127,   128,   129,   130,   156,
     157,   154,   155,   147,   158,   159,   148,   145,   146,   173,
     167,     0,   171,   160,   161,   162,   163,   134,   135,   136,
     131,   132,   133,   144,     0,   150,   151,   149,   142,   143,
     138,   137,   139,   140,   141,   122,   121,   166,     0,   152,
     153,   463,   118,   235,   211,   589,   592,   590,   593,   591,
     594,     0,     0,   544,     0,   525,   463,     0,     0,   507,
     505,   527,    85,     0,     0,     0,     0,     0,     0,     0,
     115,     0,   115,     0,   115,     0,     0,   367,   368,     0,
       0,     0,   361,     0,     0,     0,     0,     0,     0,   588,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   452,
       0,     0,     0,   311,   313,   312,   314,   315,   316,   317,
       0,    26,     0,     0,     0,     0,     0,     0,     0,   298,
     299,   365,   364,   309,   446,   362,   438,   437,   436,   435,
     113,   441,   363,   440,   439,   408,   369,     0,   370,     0,
     366,   672,   673,   674,   675,   621,   622,   615,   617,     0,
     620,   611,     0,   666,   665,   600,   671,     0,     0,   176,
     177,   170,   165,   178,   168,   164,     0,   116,     0,   485,
       0,     0,   209,     0,   528,     0,   529,   542,     0,   548,
     474,     0,     0,     0,     0,     0,     0,   506,     0,     0,
       0,   604,     0,     0,   597,     0,     0,   118,     0,   118,
       0,   118,   246,   489,     0,   487,     0,   251,   255,   254,
     258,     0,   287,     0,   278,   281,     0,   284,     0,     0,
     397,   398,     0,     0,     0,     0,     0,     0,     0,   682,
       0,     0,   706,   711,     0,   239,     0,   374,   373,   413,
      32,   372,   371,     0,   300,   444,     0,   307,     0,     0,
       0,     0,   399,   400,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   358,     0,   598,     0,   612,   610,   491,   669,   493,
     179,   169,     0,     0,     0,   557,   482,   496,   117,   463,
     119,   237,     0,    59,    60,     0,   261,   259,     0,     0,
       0,     0,   260,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   214,   212,     0,     0,     0,     0,   230,   225,
     222,   221,   223,   224,   236,   216,   215,     0,    67,    68,
      65,   228,    66,   229,   231,   276,   220,     0,   217,   318,
       0,     0,   530,   545,   475,   519,   464,     0,   509,   510,
       0,   503,   504,   502,     0,   629,   632,     0,   635,   625,
     602,     0,   639,     0,   646,     0,   653,     0,     0,   659,
       0,   662,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   246,     0,     0,   702,   680,   682,     0,   707,
       0,     0,     0,   682,     0,     0,   685,     0,     0,   713,
       0,    29,     0,    27,     0,   401,     0,     0,   350,   347,
     349,     0,   409,     0,   293,     0,   115,     0,     0,   424,
     423,     0,     0,   425,   429,   375,   376,   388,   389,   386,
     387,     0,   418,     0,   406,     0,   442,   443,   377,   378,
     393,   394,   395,   396,     0,     0,   391,   392,   390,   384,
     385,   380,   379,   381,   382,   383,     0,     0,     0,   356,
       0,     0,     0,     0,   411,     0,     0,   483,     0,     0,
     495,     0,   494,     0,   497,     0,   486,     0,     0,     0,
     265,     0,   262,     0,     0,   249,     0,     0,   234,     0,
       0,    53,    73,     0,   270,   267,   299,   245,   243,   244,
       0,     0,   232,   233,     0,    70,   219,   226,   227,   264,
     269,   275,     0,     0,   218,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   543,     0,     0,   556,     0,   463,   508,
       0,   512,     0,   516,   318,   630,   633,   605,   636,   626,
     603,   640,   642,   647,   649,   654,   656,   488,   660,   490,
     663,     0,   256,     0,     0,     0,     0,   414,     0,     0,
     415,     0,   445,   307,   447,   448,   449,   450,   451,     0,
       0,   683,     0,     0,   682,     0,     0,     0,     0,   691,
     692,     0,   697,     0,   689,     0,     0,   709,     0,     0,
       0,   687,     0,   710,   705,   712,   684,     0,     0,    30,
      33,     0,     0,     0,   115,     0,     0,     0,   410,     0,
       0,     0,     0,   305,     0,   118,   420,     0,   426,     0,
       0,     0,   404,     0,     0,   430,   434,     0,     0,   407,
       0,     0,     0,   357,     0,   359,   402,     0,   412,   484,
       0,   559,   560,   498,   501,   500,    74,     0,   266,   263,
       0,     0,     0,   250,    75,    76,    51,    52,   271,   268,
     300,   238,   235,    56,     0,   277,   242,   328,   329,   331,
     330,   332,   322,   323,   324,   333,   334,     0,     0,     0,
     320,   321,   335,   336,   325,   326,   327,     0,     0,     0,
     319,   531,     0,   470,   465,     0,   511,   514,   318,   515,
     643,   650,   657,   252,   257,     0,   290,   288,   279,   282,
     285,     0,     0,     0,   677,   676,   682,     0,   703,     0,
     681,   696,   690,   704,   688,   708,     0,   682,   694,   695,
       0,   700,   686,   240,    28,    31,     0,     0,     0,   118,
       0,     0,   294,   303,   304,   302,   301,     0,     0,     0,
       0,     0,     0,     0,   345,     0,     0,   431,     0,   419,
       0,   405,     0,     0,   403,   360,   558,     0,     0,     0,
     272,     0,     0,     0,    54,    55,     0,    69,    61,   342,
     340,   341,   339,   337,   338,   114,   471,   472,   465,   466,
     467,   470,   476,   513,   253,     0,     0,   289,     0,     0,
       0,     0,   453,     0,     0,     0,   698,     0,     0,     0,
       0,   693,     0,   348,   462,   351,     0,   343,     0,   306,
     308,     0,   295,   310,     0,   461,     0,   459,   346,   456,
       0,     0,     0,   455,     0,   561,   499,     0,    77,   213,
      57,     0,    62,     0,   481,   468,   468,     0,     0,     0,
     280,   283,   286,   416,     0,   454,   714,   699,   679,   678,
     714,   701,   241,     0,   354,     0,     0,   296,   421,   427,
       0,   460,   458,   457,    72,    56,     0,    71,   469,   477,
     479,   473,   291,     0,   417,     0,     0,     0,   352,     0,
     344,   297,   422,   428,   432,    58,   261,     0,    63,    67,
      68,    65,    66,    64,     0,     0,     0,     0,     0,     0,
     355,   433,     0,   262,     0,   267,     0,     0,   292,   715,
     716,   717,   263,   268,   478,   480
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -888,  -888,  -888,  -888,  -888,   391,   846,  -888,  -888,  -888,
    -888,  -888,  -888,  -888,   553,   881,  -888,   804,  -888,  -888,
     856,  -888,  -888,  -888,  -266,  -888,  -888,  -888,  -264,  -888,
    -888,  -888,  -888,  -888,  -888,  -888,  -888,   685,  -888,  -888,
     860,   -61,  -888,  -888,   438,    90,  -399,  -350,  -478,  -888,
    -888,  -888,  -588,  -888,  -888,  -234,  -888,   -46,  -888,  -888,
    -888,  -888,  -577,   -12,  -888,  -888,  -888,  -888,  -888,  -257,
    -256,  -255,  -888,  -253,  -888,  -888,   943,  -888,  -888,  -888,
    -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,
    -570,  -888,  -888,  -122,  -888,    63,  -584,  -888,  -457,  -888,
    -888,  -888,  -887,  -888,  -888,  -888,  -888,  -888,  -888,  -888,
    -888,   326,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -139,
    -161,  -218,  -159,  -888,  -888,  -888,  -888,  -888,   361,  -888,
     283,  -888,  -421,   588,  -579,  -576,   305,  -888,  -888,  -456,
    -888,  -888,   909,  -888,  -888,  -888,   493,   176,  -888,  -888,
    -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,
    -888,  -888,  -482,  -121,  -888,   589,  -888,   603,  -888,  -888,
    -888,  -888,  -272,  -888,  -888,  -888,  -888,  -888,  -888,  -888,
    -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,
    -888,  -888,  -888,  -888,  -888,  -888,  -888,  -101,  -888,  -888,
    -888,  -888,  -888,   590,  -703,  -505,  -680,  -888,  -888,  -888,
    -878,  -170,  -888,    56,   256,   447,  -888,  -888,  -888,  -214,
    -888
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   129,    47,    14,   147,   153,   526,   411,   920,
     412,   714,    15,    16,    39,    40,    41,    78,    17,    34,
      35,   614,   615,  1096,  1097,   616,  1163,  1196,   617,   618,
     984,   619,   620,   621,   622,   623,  1091,   154,   155,    30,
      31,    32,    57,    58,    59,    60,    18,   332,   462,   236,
      19,    98,   237,    99,   139,   413,   624,   463,   625,   414,
     710,  1132,   626,   415,   627,   498,   673,  1114,   416,   628,
     629,   630,   631,   632,   633,   634,   635,   822,   636,   417,
     678,  1118,   679,  1119,   681,  1120,   418,   676,  1117,   419,
     723,  1143,   420,   933,   934,   726,   421,   422,   791,   638,
     423,   424,   720,   425,   923,  1183,   924,  1209,   426,   772,
    1085,   724,  1070,  1212,  1072,  1213,  1150,  1231,   428,   458,
    1111,  1199,  1108,  1015,   644,   848,  1224,  1225,   459,   460,
     493,   494,   276,   277,   893,   586,   654,   478,   350,   351,
     243,   346,    68,   108,    21,   144,   342,    44,    79,    81,
      22,   102,   141,    23,   469,   241,   242,    66,   105,    24,
     142,   344,   587,   429,   341,   197,   198,   204,   199,   358,
     860,   480,   200,   356,   859,   353,   855,   354,   856,   355,
     858,   359,   861,   360,  1020,   361,   863,   362,  1021,   363,
     865,   364,  1022,   365,   868,   366,   870,   495,    25,    49,
     131,    26,    48,   430,   696,   697,   698,   709,   913,   907,
     902,  1051,   431,   699,   523,   700,   432,   524,   433,  1206,
     434
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      61,   444,   196,   334,   775,   245,   637,   777,   785,   663,
     487,   665,   489,   667,   491,   645,   703,   898,   926,   790,
    1041,   537,   201,   910,   807,   808,   809,   579,  -113,   134,
     804,   109,   110,   890,   691,   908,   690,  1141,   701,    50,
      61,   967,   691,    61,    61,   823,  1109,   538,   539,   641,
     435,   550,   551,    27,    28,    42,   583,    42,     2,   548,
     456,    87,   550,   551,     3,   244,    45,    51,   780,    91,
      92,    93,   273,    91,    92,    93,   780,   853,   642,    61,
      61,    61,    61,    33,    96,   780,   651,     4,    74,     5,
      97,     6,    67,   538,   539,   892,   652,     7,   657,   780,
      43,    87,  1110,   125,   873,   668,     8,   891,    46,   892,
     333,   195,     9,   788,   538,   539,   457,   238,   519,   521,
     437,   465,    75,   929,   465,   781,   782,   824,   783,   571,
     572,   784,    10,   930,   782,    52,   133,   273,   542,   543,
     571,   572,    53,   782,   653,   239,   548,   957,   549,   550,
     551,   552,   196,   584,   925,    11,   553,   782,    69,  1126,
     743,   263,   148,   149,    70,   466,   780,   467,   466,    54,
     843,   585,   744,   111,   508,   445,  1127,  1124,   112,   333,
     113,   931,   114,   347,   542,   543,   932,   264,   265,   535,
     897,   781,   548,   781,  1040,   550,   551,   552,    29,   781,
      55,   436,   553,   275,   650,   542,   543,   471,   964,    56,
    1109,  1165,   968,   548,  1037,   549,   550,   551,   552,    80,
     115,   446,   845,   553,   782,   100,   978,   571,   572,   536,
     846,   101,   196,   196,   196,   196,    70,   683,   196,   986,
     196,   706,   196,    94,   196,   847,   266,  1177,    71,   578,
     267,   579,   479,   481,   482,   483,    76,   126,   486,    12,
     488,   195,   490,   206,    72,    13,  1110,    77,    95,   196,
     196,   438,   957,   571,   572,  1017,   707,   904,   708,    53,
     566,   567,   568,   569,   570,    42,   957,    83,   716,   439,
     766,   767,    70,   717,   571,   572,  1208,   263,   440,   538,
     539,   911,  1129,   896,   150,   905,    54,   263,   268,   151,
     906,   152,   269,   114,   883,   270,  1157,   472,  1048,   768,
    1033,  1049,  1230,   264,   265,    53,   196,   718,  1060,    87,
     271,   769,   249,   264,   265,   906,    53,   912,  1046,   272,
     196,   195,   195,   195,   195,   716,   577,   195,   263,   195,
     927,   195,    54,   195,  1050,   196,  1125,   195,  1138,   250,
     590,   948,  1067,    54,   793,   103,   985,  1130,   770,   771,
    1075,   104,   802,   949,   264,   265,   935,   928,   195,   195,
      62,    63,   266,    64,  1088,   651,   267,  1068,   196,  1036,
     542,   543,   266,   530,   957,   652,   267,  1089,   548,    84,
     549,   550,   551,   552,  1047,   957,   878,   879,   553,   881,
     952,    65,   957,  1099,  1100,  1101,    94,   730,   734,   899,
     900,   958,   953,  1102,  1103,  1104,    36,    37,   957,    91,
     106,    93,  1024,   266,   896,   195,   107,   267,   917,  1148,
    -641,  1105,   273,   653,   268,  -641,   655,   901,   269,   195,
      86,   270,   660,    38,   268,  1175,   643,  1069,   269,  1185,
    1128,   270,  1123,  -641,   195,    53,   271,   568,   569,   570,
     348,   127,   851,  -648,   661,   272,   271,   128,  -648,   571,
     572,  -655,   390,   391,   392,   272,  -655,   273,   349,   675,
      85,   658,    54,    88,    89,   268,  -648,   195,  -353,   269,
    1162,  1028,   270,  -353,  -655,   669,   671,   584,  1093,   538,
     539,   456,  1115,   497,   335,    90,   244,   271,   336,  1094,
    1095,  -353,   719,    36,    37,   585,   272,   670,   672,   135,
     136,   137,   138,   118,  1116,  1106,   196,   337,   338,   339,
     340,  1107,   475,   409,   806,   476,   333,   196,   477,   196,
     273,    77,   196,   966,   659,   196,   857,   196,   196,   273,
     196,   273,   974,   662,   120,   664,   273,   867,   263,   273,
     666,   123,   871,  1023,  1059,   876,   981,   124,   273,   273,
     983,  1136,  1027,  1189,   145,   146,   273,   130,   427,   196,
    1214,  1201,   132,   588,   264,   265,   589,   442,   540,   541,
     542,   543,   544,   246,   247,   545,   546,   547,   548,   140,
     549,   550,   551,   552,   145,   146,   711,   712,   553,   143,
     554,   555,   246,   247,   248,    75,   556,   557,   558,   121,
     122,   202,   559,   946,   203,   637,  1236,  1237,   205,    91,
     240,   244,   251,   252,   253,   195,   254,   255,   256,   257,
     258,  1058,   259,   266,   260,   262,   195,   267,   195,   261,
     273,   195,   196,   275,   195,   329,   195,   195,   560,   195,
     561,   562,   563,   564,   565,   566,   567,   568,   569,   570,
     274,   330,   963,   343,   538,   539,   331,   345,   357,   571,
     572,   447,   333,   448,   449,   450,   451,   452,   195,   453,
     454,   455,   461,   468,   464,   484,   470,   510,   511,  1014,
     473,   474,   501,   502,   504,   268,   503,   506,   522,   269,
     508,  1029,   270,   505,   512,   507,   509,   513,   527,   528,
     529,   514,   531,   532,   533,   525,    12,   271,  1090,   580,
     534,   515,   581,   648,   649,   575,   272,   516,   517,   574,
     582,   646,   656,   674,   725,   741,   779,   778,   797,   798,
     799,   811,   812,   813,   815,   816,   817,   818,   819,   820,
     821,   195,   850,   540,   541,   542,   543,   544,   445,   862,
     545,   546,   547,   548,   864,   549,   550,   551,   552,   639,
     866,   896,   916,   553,   903,   554,   555,   936,  1144,   647,
     919,   921,   196,   922,   925,   937,   938,   955,   961,   939,
     940,   941,   951,   962,   823,   972,   975,   976,   977,   982,
    1013,  1016,   980,  1025,  1032,  1042,  1039,  1044,  1052,   677,
    1063,   263,   680,    61,   682,  1142,  1064,  1055,   684,   685,
     686,   687,   688,   689,  1065,   695,  1066,   695,   564,   565,
     566,   567,   568,   569,   570,  1078,  1077,   264,   265,  1160,
    1080,  1087,  1083,  1112,   571,   572,   735,   736,  1121,  1184,
     737,   738,   739,   740,  1155,   742,  1131,   745,   746,   747,
     748,   749,   750,   751,   752,   753,   755,   756,   757,   758,
     759,   760,   761,   762,   763,   764,   765,  1137,   773,  1140,
    1169,  1164,  1197,   538,   539,  1198,  1203,  1210,   776,  1227,
    1228,   195,  1240,  1229,  1241,   116,   266,   713,   787,    73,
     267,  1244,   792,  1194,   156,   795,   796,  1195,   119,  1215,
     117,   801,  1218,   805,   352,  1012,  1092,   810,   263,  1219,
    1220,  1221,   814,  1222,    20,  1139,  1031,  1166,  1200,   196,
     786,   196,  1167,   869,   496,   852,    82,   640,   499,   500,
    1181,   485,  1045,   915,   264,   265,  1207,   844,   704,     0,
       0,  1146,     0,     0,     0,     0,     0,     0,   268,     0,
     854,     0,   269,     0,  1030,   270,     0,     0,     0,     0,
       0,     0,   540,   541,   542,   543,   544,     0,     0,   545,
     271,   875,   548,  1245,   549,   550,   551,   552,     0,   272,
       0,     0,   553,     0,   554,   555,     0,     0,     0,   527,
     533,     0,     0,   266,     0,     0,     0,   267,   533,   196,
       0,   914,     0,   522,     0,     0,     0,     0,   918,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1190,
       0,     0,     0,     0,     0,     0,     0,     0,   195,     0,
     195,     0,     0,     0,     0,     0,     0,   564,   565,   566,
     567,   568,   569,   570,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   571,   572,   268,     0,     0,     0,   269,
       0,  1053,   270,   950,     0,     0,     0,   954,     0,     0,
       0,     0,     0,     0,   960,     0,     0,   271,     0,     0,
       0,   965,     0,     0,     0,   969,   272,     0,     0,     0,
     973,     0,     0,     0,   538,   539,     0,     0,     0,   979,
       0,     0,     0,     0,     0,     0,     0,     0,   195,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   987,   988,   989,   990,   991,   992,   993,   994,   995,
     996,  1000,  1001,  1002,  1003,  1004,  1005,  1006,  1010,     0,
       0,   719,     0,     0,     0,     0,     0,     0,  1018,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   719,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   540,   541,   542,   543,     0,  1034,  1035,
       0,  1038,     0,   548,     0,   549,   550,   551,   552,  1043,
     538,   539,   695,   553,     0,   554,   555,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1056,  1057,     0,
       0,     0,     0,  1062,     0,     0,     0,   -64,     0,     0,
       0,     0,     0,  1071,     0,  1073,     0,  1076,   538,   539,
       0,     0,     0,  1079,     0,     0,     0,  1082,     0,     0,
       0,     0,     0,  1062,     0,     0,     0,     0,     0,     0,
     566,   567,   568,   569,   570,     0,  1000,  1010,     0,     0,
       0,     0,     0,     0,   571,   572,     0,     0,     0,     0,
    1098,     0,     0,     0,     0,     0,     0,     0,   263,   540,
     541,   542,   543,   544,     0,     0,   545,   546,   547,   548,
       0,   549,   550,   551,   552,     0,     0,     0,     0,   553,
       0,   554,   555,     0,   264,   265,     0,     0,     0,   825,
     826,   827,   828,   829,   830,   831,   832,   540,   541,   542,
     543,   544,   833,   834,   545,   546,   547,   548,   835,   549,
     550,   551,   552,     0,     0,     0,     0,   553,   836,   554,
     555,   837,   838,     0,     0,   556,   557,   558,   839,   840,
     841,   559,   562,   563,   564,   565,   566,   567,   568,   569,
     570,     0,     0,   266,  1151,     0,  1152,   267,     0,  1154,
     571,   572,     0,     0,  1156,     0,     0,  1158,   639,     0,
       0,     0,  1161,     0,     0,     0,   842,   560,     0,   561,
     562,   563,   564,   565,   566,   567,   568,   569,   570,     0,
       0,  1168,     0,     0,  1170,  1171,  1172,  1174,   571,   572,
    1176,     0,     0,  1178,  1179,  1180,     0,     0,  1182,     0,
       0,     0,     0,     0,     0,   268,     0,  1187,     0,   269,
       0,  1135,   270,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   271,     0,     0,
       0,     0,     0,     0,     0,     0,   272,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   263,
       0,     0,  1211,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1223,     0,     0,     0,     0,     0,     0,  1226,
       0,     0,     0,     0,     0,   264,   265,     0,     0,     0,
       0,     0,  1233,  1235,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1239,   591,     0,     0,     0,  1242,     3,
    1243,   592,   593,   594,     0,   595,     0,   367,   368,   369,
     370,   371,     0,     0,     0,     0,     0,     0,   596,   372,
     597,   598,     0,     0,     0,     0,     0,     0,     0,     0,
     599,   373,     0,   600,   266,   601,   374,     0,   267,   375,
       0,     8,   376,   602,     0,   603,   377,     0,     0,   604,
     605,     0,     0,     0,     0,     0,   606,     0,   161,   162,
     163,     0,   165,   166,   167,   168,   169,   379,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,     0,
     183,   184,   185,     0,     0,   188,   189,   190,   191,     0,
       0,   380,   607,     0,     0,     0,   268,     0,     0,     0,
     269,     0,  1188,   270,   381,   382,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   271,   608,
     609,   610,     0,     0,     0,     0,     0,   272,     0,    53,
       0,     0,     0,     0,     0,     0,     0,   383,   384,   385,
     386,   387,     0,   388,     0,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,    54,   611,   401,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   402,
     403,   404,     0,   405,     0,     0,   406,   407,     0,     0,
       0,     0,     0,     0,     0,     0,   408,     0,   409,   410,
     612,   333,   613,   591,     0,     0,     0,     0,     3,     0,
     592,   593,   594,     0,   595,     0,   367,   368,   369,   370,
     371,     0,     0,     0,     0,     0,     0,   596,   372,   597,
     598,     0,     0,     0,     0,     0,     0,     0,     0,   599,
     373,     0,   600,     0,   601,   374,     0,     0,   375,     0,
       8,   376,   602,     0,   603,   377,     0,     0,   604,   605,
       0,     0,     0,     0,     0,   606,     0,   161,   162,   163,
       0,   165,   166,   167,   168,   169,   379,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,   183,
     184,   185,     0,     0,   188,   189,   190,   191,     0,     0,
     380,   607,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   381,   382,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   608,   609,
     610,     0,     0,     0,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,   383,   384,   385,   386,
     387,     0,   388,     0,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,    54,   611,   401,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   402,   403,
     404,     0,   405,     0,     0,   406,   407,     0,     0,     0,
     367,   368,   369,   370,   371,   408,     0,   409,   410,   612,
     333,  1159,   372,     0,     0,     0,     0,     0,   263,     0,
       0,     0,     0,     0,   373,     0,     0,     0,     0,   374,
       0,     0,   375,     0,     0,   376,     0,   691,     0,   377,
       0,     0,     0,     0,   264,   265,     0,     0,     0,   378,
       0,   161,   162,   163,     0,   165,   166,   167,   168,   169,
     379,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,     0,   183,   184,   185,     0,     0,   188,   189,
     190,   191,     0,     0,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   381,   382,     0,
       0,     0,     0,   266,     0,     0,     0,   267,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
     383,   384,   385,   386,   387,     0,   388,     0,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,   692,
     400,   401,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   268,     0,     0,     0,   269,
       0,     0,   693,   403,   404,     0,   405,     0,     0,   406,
     407,   367,   368,   369,   370,   371,     0,   271,     0,   694,
       0,   409,   410,   372,   333,     0,   272,     0,     0,   263,
       0,     0,     0,     0,     0,   373,     0,     0,     0,     0,
     374,     0,     0,   375,     0,     0,   376,     0,     0,     0,
     377,     0,     0,     0,     0,   264,   265,     0,     0,     0,
     378,     0,   161,   162,   163,     0,   165,   166,   167,   168,
     169,   379,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,     0,   183,   184,   185,     0,     0,   188,
     189,   190,   191,     0,     0,   380,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   381,   382,
       0,     0,     0,     0,   266,     0,     0,     0,   267,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,   383,   384,   385,   386,   387,     0,   388,     0,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     692,   400,   401,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   268,     0,     0,     0,
     269,     0,     0,   693,   403,   404,     0,   405,     0,     0,
     406,   407,   367,   368,   369,   370,   371,     0,   271,     0,
     702,     0,   409,   410,   372,   333,     0,   272,     0,     0,
     437,     0,     0,     0,     0,     0,   373,     0,     0,     0,
       0,   374,     0,     0,   375,     0,     0,   376,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,   161,   162,   163,     0,   165,   166,   167,
     168,   169,   379,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,     0,   183,   184,   185,     0,     0,
     188,   189,   190,   191,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   381,
     382,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,   383,   384,   385,   386,   387,     0,   388,     0,
     389,   390,   391,   392,   393,   394,   395,   396,   397,   398,
     399,    54,   400,   401,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,     0,     0,   402,   403,   404,     0,   405,     0,
       0,   406,   407,   367,   368,   369,   370,   371,     0,   439,
       0,   408,     0,   409,   410,   372,   333,     0,   440,     0,
       0,     0,     0,     0,     0,     0,     0,   373,     0,     0,
       0,     0,   374,     0,     0,   375,     0,     0,   376,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,   161,   162,   163,     0,   165,   166,
     167,   168,   169,   379,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,   183,   184,   185,     0,
       0,   188,   189,   190,   191,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     381,   382,     0,     0,     0,     0,     0,     0,     0,   789,
       0,     0,     0,     0,     0,   608,   609,   610,     0,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,   383,   384,   385,   386,   387,     0,   388,
       0,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,    54,   400,   401,     0,     0,     0,     0,     0,
       0,   367,   368,   369,   370,   371,     0,     0,     0,     0,
       0,     0,     0,   372,     0,   402,   403,   404,     0,   405,
       0,     0,   406,   407,     0,   373,     0,     0,     0,     0,
     374,     0,   408,   375,   409,   410,   376,   333,     0,     0,
     377,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,     0,   161,   162,   163,     0,   165,   166,   167,   168,
     169,   379,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,     0,   183,   184,   185,     0,     0,   188,
     189,   190,   191,     0,     0,   380,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   381,   382,
       0,     0,     0,     0,     0,     0,     0,   803,     0,     0,
       0,     0,     0,   608,   609,   610,     0,     0,     0,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,   383,   384,   385,   386,   387,     0,   388,     0,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
      54,   400,   401,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   402,   403,   404,     0,   405,     0,     0,
     406,   407,     0,     0,     0,   367,   368,   369,   370,   371,
     408,     0,   409,   410,     0,   333,  1216,   372,   597,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   373,
       0,     0,     0,     0,   374,     0,     0,   375,     0,     0,
     376,   602,     0,     0,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,     0,   161,   162,   163,     0,
     165,   166,   167,   168,   169,   379,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,     0,   183,   184,
     185,     0,     0,   188,   189,   190,   191,     0,     0,   380,
    1217,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   381,   382,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    53,     0,     0,
       0,     0,     0,     0,     0,   383,   384,   385,   386,   387,
       0,   388,     0,   389,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   399,    54,   400,   401,     0,     0,     0,
       0,     0,     0,   367,   368,   369,   370,   371,     0,     0,
       0,     0,     0,     0,     0,   372,     0,   402,   403,   404,
       0,   405,     0,     0,   406,   407,     0,   373,     0,     0,
       0,     0,   374,     0,   408,   375,   409,   410,   376,   333,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,   161,   162,   163,     0,   165,   166,
     167,   168,   169,   379,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,   183,   184,   185,     0,
       0,   188,   189,   190,   191,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     381,   382,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   608,   609,   610,     0,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,   383,   384,   385,   386,   387,     0,   388,
       0,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,    54,   400,   401,     0,     0,     0,     0,     0,
       0,   367,   368,   369,   370,   371,     0,     0,     0,     0,
       0,     0,     0,   372,     0,   402,   403,   404,     0,   405,
       0,     0,   406,   407,     0,   373,     0,     0,     0,     0,
     374,     0,   408,   375,   409,   410,   376,   333,     0,     0,
     377,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,     0,   161,   162,   163,     0,   165,   166,   167,   168,
     169,   379,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,     0,   183,   184,   185,     0,     0,   188,
     189,   190,   191,     0,     0,   380,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   381,   382,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   997,   998,   999,     0,     0,     0,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,   383,   384,   385,   386,   387,     0,   388,     0,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
      54,   400,   401,     0,     0,     0,     0,     0,     0,   367,
     368,   369,   370,   371,     0,     0,     0,     0,     0,     0,
       0,   372,     0,   402,   403,   404,     0,   405,     0,     0,
     406,   407,     0,   373,     0,     0,     0,     0,   374,     0,
     408,   375,   409,   410,   376,   333,     0,     0,   377,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,     0,
     161,   162,   163,     0,   165,   166,   167,   168,   169,   379,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,     0,   183,   184,   185,     0,     0,   188,   189,   190,
     191,     0,     0,   380,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   381,   382,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1007,  1008,  1009,     0,     0,     0,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,   383,
     384,   385,   386,   387,     0,   388,     0,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,    54,   400,
     401,     0,     0,     0,     0,     0,     0,   367,   368,   369,
     370,   371,     0,     0,     0,     0,     0,     0,     0,   372,
       0,   402,   403,   404,     0,   405,     0,     0,   406,   407,
       0,   373,     0,     0,     0,     0,   374,     0,   408,   375,
     409,   410,   376,   333,     0,     0,   377,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   378,     0,   161,   162,
     163,     0,   165,   166,   167,   168,   169,   379,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,     0,
     183,   184,   185,     0,     0,   188,   189,   190,   191,     0,
       0,   380,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   381,   382,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,   383,   384,   385,
     386,   387,     0,   388,     0,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,    54,   400,   401,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   402,
     403,   404,     0,   405,     0,     0,   406,   407,   367,   368,
     369,   370,   371,     0,   721,     0,   408,   722,   409,   410,
     372,   333,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   373,     0,     0,     0,     0,   374,     0,     0,
     375,     0,     0,   376,     0,     0,     0,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,   161,
     162,   163,     0,   165,   166,   167,   168,   169,   379,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
       0,   183,   184,   185,     0,     0,   188,   189,   190,   191,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   381,   382,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,   383,   384,
     385,   386,   387,     0,   388,     0,   389,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,    54,   400,   401,
       0,     0,     0,     0,     0,     0,   367,   368,   369,   370,
     371,     0,     0,   754,     0,     0,     0,     0,   372,     0,
     402,   403,   404,     0,   405,     0,     0,   406,   407,     0,
     373,     0,     0,     0,     0,   374,   441,   408,   375,   409,
     410,   376,   333,     0,     0,   377,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,     0,   161,   162,   163,
       0,   165,   166,   167,   168,   169,   379,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,   183,
     184,   185,     0,     0,   188,   189,   190,   191,     0,     0,
     380,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   381,   382,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,   383,   384,   385,   386,
     387,     0,   388,     0,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,    54,   400,   401,     0,     0,
       0,     0,     0,     0,   367,   368,   369,   370,   371,     0,
       0,     0,     0,     0,     0,     0,   372,     0,   402,   403,
     404,     0,   405,     0,     0,   406,   407,     0,   373,     0,
       0,     0,     0,   374,     0,   408,   375,   409,   410,   376,
     333,     0,     0,   377,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,   161,   162,   163,     0,   165,
     166,   167,   168,   169,   379,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,     0,   183,   184,   185,
       0,     0,   188,   189,   190,   191,     0,     0,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   381,   382,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,   383,   384,   385,   386,   387,     0,
     388,     0,   389,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,    54,   400,   401,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   402,   403,   404,     0,
     405,     0,     0,   406,   407,   367,   368,   369,   370,   371,
       0,     0,     0,   408,   774,   409,   410,   372,   333,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   373,
       0,     0,     0,     0,   374,     0,     0,   375,     0,     0,
     376,     0,     0,     0,   377,     0,     0,     0,     0,     0,
     794,     0,     0,     0,   378,     0,   161,   162,   163,     0,
     165,   166,   167,   168,   169,   379,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,     0,   183,   184,
     185,     0,     0,   188,   189,   190,   191,     0,     0,   380,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   381,   382,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    53,     0,     0,
       0,     0,     0,     0,     0,   383,   384,   385,   386,   387,
       0,   388,     0,   389,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   399,    54,   400,   401,     0,     0,     0,
       0,     0,     0,   367,   368,   369,   370,   371,     0,     0,
       0,     0,     0,     0,     0,   372,     0,   402,   403,   404,
       0,   405,     0,     0,   406,   407,     0,   373,     0,     0,
       0,     0,   374,     0,   408,   375,   409,   410,   376,   333,
       0,     0,   377,     0,     0,   800,     0,     0,     0,     0,
       0,     0,   378,     0,   161,   162,   163,     0,   165,   166,
     167,   168,   169,   379,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,   183,   184,   185,     0,
       0,   188,   189,   190,   191,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     381,   382,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,   383,   384,   385,   386,   387,     0,   388,
       0,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,    54,   400,   401,     0,     0,     0,     0,     0,
       0,   367,   368,   369,   370,   371,     0,     0,     0,     0,
       0,     0,     0,   372,     0,   402,   403,   404,     0,   405,
       0,     0,   406,   407,     0,   373,     0,     0,     0,     0,
     374,     0,   408,   375,   409,   410,   376,   333,     0,     0,
     377,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,     0,   161,   162,   163,     0,   165,   166,   167,   168,
     169,   379,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,     0,   183,   184,   185,     0,     0,   188,
     189,   190,   191,     0,     0,   380,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   381,   382,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,   383,   384,   385,   386,   387,     0,   388,     0,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
      54,   400,   401,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,     0,   402,   403,   404,     0,   405,     0,     0,
     406,   407,   367,   368,   369,   370,   371,     0,     0,     0,
     408,     0,   409,   410,   372,   333,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   373,     0,     0,     0,
       0,   374,     0,     0,   375,     0,     0,   376,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,   161,   162,   163,     0,   165,   166,   167,
     168,   169,   379,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,     0,   183,   184,   185,     0,     0,
     188,   189,   190,   191,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   381,
     382,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,   383,   384,   385,   386,   387,     0,   388,     0,
     389,   390,   391,   392,   393,   394,   395,   396,   397,   398,
     399,    54,   400,   401,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   402,   403,   404,     0,   405,     0,
       0,   406,   407,   367,   368,   369,   370,   371,     0,     0,
       0,   408,   872,   409,   410,   372,   333,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   373,     0,     0,
       0,     0,   374,     0,     0,   375,     0,     0,   376,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,   161,   162,   163,     0,   165,   166,
     167,   168,   169,   379,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,   183,   184,   185,     0,
       0,   188,   189,   190,   191,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     381,   382,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,   383,   384,   385,   386,   387,     0,   388,
       0,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,    54,   400,   401,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   874,     0,   402,   403,   404,     0,   405,
       0,     0,   406,   407,   367,   368,   369,   370,   371,     0,
       0,     0,   408,     0,   409,   410,   372,   333,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   373,     0,
       0,     0,     0,   374,     0,     0,   375,     0,     0,   376,
       0,     0,     0,   377,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,   161,   162,   163,     0,   165,
     166,   167,   168,   169,   379,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,     0,   183,   184,   185,
       0,     0,   188,   189,   190,   191,     0,     0,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   381,   382,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,   383,   384,   385,   386,   387,     0,
     388,     0,   389,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,    54,   400,   401,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   402,   403,   404,     0,
     405,     0,     0,   406,   407,   367,   368,   369,   370,   371,
       0,     0,     0,   408,   894,   409,   410,   372,   333,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   373,
       0,     0,     0,     0,   374,     0,     0,   375,     0,     0,
     376,     0,     0,     0,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,     0,   161,   162,   163,     0,
     165,   166,   167,   168,   169,   379,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,     0,   183,   184,
     185,     0,     0,   188,   189,   190,   191,     0,     0,   380,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   381,   382,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    53,     0,     0,
       0,     0,     0,     0,     0,   383,   384,   385,   386,   387,
       0,   388,     0,   389,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   399,    54,   400,   401,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   402,   403,   404,
       0,   405,     0,     0,   406,   407,   367,   368,   369,   370,
     371,     0,     0,     0,   408,   909,   409,   410,   372,   333,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     373,     0,     0,     0,     0,   374,     0,     0,   375,     0,
       0,   376,     0,     0,     0,   377,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,     0,   161,   162,   163,
       0,   165,   166,   167,   168,   169,   379,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,   183,
     184,   185,     0,     0,   188,   189,   190,   191,     0,     0,
     380,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   381,   382,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,   383,   384,   385,   386,
     387,     0,   388,     0,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,    54,   400,   401,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,   368,   369,   370,   371,     0,   402,   403,
     404,     0,   405,     0,   372,   406,   407,     0,     0,     0,
       0,     0,     0,  1061,     0,   408,   373,   409,   410,     0,
     333,   374,     0,     0,   375,     0,     0,   376,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,   161,   162,   163,     0,   165,   166,   167,
     168,   169,   379,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,     0,   183,   184,   185,     0,     0,
     188,   189,   190,   191,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   381,
     382,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,   383,   384,   385,   386,   387,     0,   388,     0,
     389,   390,   391,   392,   393,   394,   395,   396,   397,   398,
     399,    54,   400,   401,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   402,   403,   404,     0,   405,     0,
       0,   406,   407,   367,   368,   369,   370,   371,     0,     0,
       0,   408,  1074,   409,   410,   372,   333,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   373,     0,     0,
       0,     0,   374,     0,     0,   375,     0,     0,   376,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,   161,   162,   163,     0,   165,   166,
     167,   168,   169,   379,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,   183,   184,   185,     0,
       0,   188,   189,   190,   191,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     381,   382,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,   383,   384,   385,   386,   387,     0,   388,
       0,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,    54,   400,   401,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   402,   403,   404,     0,   405,
       0,     0,   406,   407,   367,   368,   369,   370,   371,     0,
       0,     0,   408,  1122,   409,   410,   372,   333,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   373,     0,
       0,     0,     0,   374,     0,     0,   375,     0,     0,   376,
       0,     0,     0,   377,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,   161,   162,   163,     0,   165,
     166,   167,   168,   169,   379,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,     0,   183,   184,   185,
       0,     0,   188,   189,   190,   191,     0,     0,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   381,   382,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,   383,   384,   385,   386,   387,     0,
     388,     0,   389,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,    54,   400,   401,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   402,   403,   404,     0,
     405,     0,     0,   406,   407,   367,   368,   369,   370,   371,
       0,     0,     0,   408,  1173,   409,   410,   372,   333,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   373,
       0,     0,     0,     0,   374,     0,     0,   375,     0,     0,
     376,     0,     0,     0,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,     0,   161,   162,   163,     0,
     165,   166,   167,   168,   169,   379,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,     0,   183,   184,
     185,     0,     0,   188,   189,   190,   191,     0,     0,   380,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   381,   382,     0,     0,     0,     0,     0,     0,
       0,  1186,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    53,     0,     0,
       0,     0,     0,     0,     0,   383,   384,   385,   386,   387,
       0,   388,     0,   389,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   399,    54,   400,   401,     0,     0,     0,
       0,     0,     0,   367,   368,   369,   370,   371,     0,     0,
       0,     0,     0,     0,     0,   372,     0,   402,   403,   404,
       0,   405,     0,     0,   406,   407,     0,   373,     0,     0,
       0,     0,   374,     0,   408,   375,   409,   410,   376,   333,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,   161,   162,   163,     0,   165,   166,
     167,   168,   169,   379,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,   183,   184,   185,     0,
       0,   188,   189,   190,   191,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     381,   382,     0,     0,     0,     0,     0,     0,     0,  1232,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,   383,   384,   385,   386,   387,     0,   388,
       0,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,    54,   400,   401,     0,     0,     0,     0,     0,
       0,   367,   368,   369,   370,   371,     0,     0,     0,     0,
       0,     0,     0,   372,     0,   402,   403,   404,     0,   405,
       0,     0,   406,   407,     0,   373,     0,     0,     0,     0,
     374,     0,   408,   375,   409,   410,   376,   333,     0,     0,
     377,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,     0,   161,   162,   163,     0,   165,   166,   167,   168,
     169,   379,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,     0,   183,   184,   185,     0,     0,   188,
     189,   190,   191,     0,     0,   380,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   381,   382,
       0,     0,     0,     0,     0,     0,     0,  1234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,   383,   384,   385,   386,   387,     0,   388,     0,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
      54,   400,   401,     0,     0,     0,     0,     0,     0,   367,
     368,   369,   370,   371,     0,     0,     0,     0,     0,     0,
       0,   372,     0,   402,   403,   404,     0,   405,     0,     0,
     406,   407,     0,   373,     0,     0,     0,     0,   374,     0,
     408,   375,   409,   410,   376,   333,     0,     0,   377,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,     0,
     161,   162,   163,     0,   165,   166,   167,   168,   169,   379,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,     0,   183,   184,   185,     0,     0,   188,   189,   190,
     191,     0,     0,   380,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   381,   382,     0,     0,
       0,     0,     0,     0,     0,   538,   539,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,   383,
     384,   385,   386,   387,     0,   388,     0,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,    54,   400,
     401,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,   403,   404,     0,   405,     0,     0,   406,   407,
       0,     0,     0,     0,     0,     0,     0,     0,   408,     0,
     409,   410,     0,   333,   540,   541,   542,   543,   544,     0,
       0,   545,   546,   547,   548,     0,   549,   550,   551,   552,
       0,     0,     0,     0,   553,     0,   554,   555,   538,   539,
       0,     0,   556,   557,   558,     0,     0,     0,   559,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   560,     0,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   571,   572,     0,     0,     0,
       0,     0,     0,   333,     0,     0,     0,     0,     0,   825,
     826,   827,   828,   829,   830,   831,   832,   540,   541,   542,
     543,   544,   833,   834,   545,   546,   547,   548,   970,   549,
     550,   551,   552,     0,     0,   538,   539,   553,   836,   554,
     555,   837,   838,     0,     0,   556,   557,   558,   839,   840,
     841,   559,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   538,   539,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   971,   560,     0,   561,
     562,   563,   564,   565,   566,   567,   568,   569,   570,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   571,   572,
       0,     0,     0,     0,     0,  1019,   825,   826,   827,   828,
     829,   830,   831,   832,   540,   541,   542,   543,   544,   833,
     834,   545,   546,   547,   548,   970,   549,   550,   551,   552,
       0,     0,     0,     0,   553,   836,   554,   555,   837,   838,
       0,     0,   556,   557,   558,   839,   840,   841,   559,     0,
       0,   540,   541,   542,   543,   544,     0,     0,   545,   546,
     547,   548,     0,   549,   550,   551,   552,   538,   539,     0,
       0,   553,     0,   554,   555,     0,     0,     0,     0,   556,
     557,   558,     0,   971,   560,   559,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   571,   572,     0,     0,     0,
       0,     0,  1113,     0,     0,     0,     0,     0,     0,     0,
       0,   560,     0,   561,   562,   563,   564,   565,   566,   567,
     568,   569,   570,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   571,   572,     0,     0,     0,     0,     0,  1011,
       0,     0,     0,     0,     0,     0,   540,   541,   542,   543,
     544,     0,     0,   545,   546,   547,   548,     0,   549,   550,
     551,   552,     0,     0,     0,     0,   553,     0,   554,   555,
       0,     0,     0,     0,   556,   557,   558,     0,     0,     0,
     559,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     538,   539,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   157,     0,     0,     0,     0,
       0,   158,     0,     0,     0,     0,   560,     0,   561,   562,
     563,   564,   565,   566,   567,   568,   569,   570,   159,     0,
       0,     0,     0,     0,     0,     0,     0,   571,   572,     0,
       0,     0,     0,   160,  1205,     0,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   540,
     541,   542,   543,   544,     0,     0,   545,   546,   547,   548,
       0,   549,   550,   551,   552,     0,   538,   539,     0,   553,
       0,   554,   555,     0,     0,     0,     0,   556,   557,   558,
       0,     0,     0,   559,     0,     0,     0,    53,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     194,   538,   539,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,     0,     0,     0,     0,   560,
       0,   561,   562,   563,   564,   565,   566,   567,   568,   569,
     570,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     571,   572,     0,     0,   573,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   540,   541,   542,   543,   544,
       0,   443,   545,   546,   547,   548,     0,   549,   550,   551,
     552,     0,     0,     0,     0,   553,     0,   554,   555,     0,
       0,     0,     0,   556,   557,   558,     0,     0,     0,   559,
     540,   541,   542,   543,   544,     0,     0,   545,   546,   547,
     548,     0,   549,   550,   551,   552,   538,   539,     0,     0,
     553,     0,   554,   555,     0,     0,     0,     0,   556,   557,
     558,     0,     0,     0,   559,   560,     0,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,     0,     0,     0,
       0,   538,   539,     0,     0,     0,   571,   572,     0,     0,
     715,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     560,     0,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,   572,     0,     0,   849,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   540,   541,   542,   543,   544,
       0,     0,   545,   546,   547,   548,     0,   549,   550,   551,
     552,     0,     0,     0,     0,   553,     0,   554,   555,     0,
       0,     0,     0,   556,   557,   558,     0,     0,     0,   559,
     540,   541,   542,   543,   544,     0,     0,   545,   546,   547,
     548,     0,   549,   550,   551,   552,   538,   539,     0,     0,
     553,     0,   554,   555,     0,     0,     0,     0,   556,   557,
     558,     0,     0,     0,   559,   560,     0,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,     0,     0,     0,
       0,   538,   539,     0,     0,     0,   571,   572,     0,     0,
     877,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     560,     0,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,   572,     0,     0,   880,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   540,   541,   542,   543,   544,
       0,     0,   545,   546,   547,   548,     0,   549,   550,   551,
     552,     0,     0,     0,     0,   553,     0,   554,   555,     0,
       0,     0,     0,   556,   557,   558,     0,     0,     0,   559,
     540,   541,   542,   543,   544,     0,     0,   545,   546,   547,
     548,     0,   549,   550,   551,   552,   538,   539,     0,     0,
     553,     0,   554,   555,     0,     0,     0,     0,   556,   557,
     558,     0,     0,     0,   559,   560,     0,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,     0,     0,     0,
       0,   538,   539,     0,     0,     0,   571,   572,     0,     0,
     882,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     560,     0,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,   572,     0,     0,   884,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   540,   541,   542,   543,   544,
       0,     0,   545,   546,   547,   548,     0,   549,   550,   551,
     552,     0,     0,     0,     0,   553,     0,   554,   555,     0,
       0,     0,     0,   556,   557,   558,     0,     0,     0,   559,
     540,   541,   542,   543,   544,     0,     0,   545,   546,   547,
     548,     0,   549,   550,   551,   552,   538,   539,     0,     0,
     553,     0,   554,   555,     0,     0,     0,     0,   556,   557,
     558,     0,     0,     0,   559,   560,     0,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,     0,     0,     0,
       0,   538,   539,     0,     0,     0,   571,   572,     0,     0,
     885,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     560,     0,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,   572,     0,     0,   886,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   540,   541,   542,   543,   544,
       0,     0,   545,   546,   547,   548,     0,   549,   550,   551,
     552,     0,     0,     0,     0,   553,     0,   554,   555,     0,
       0,     0,     0,   556,   557,   558,     0,     0,     0,   559,
     540,   541,   542,   543,   544,     0,     0,   545,   546,   547,
     548,     0,   549,   550,   551,   552,   538,   539,     0,     0,
     553,     0,   554,   555,     0,     0,     0,     0,   556,   557,
     558,     0,     0,     0,   559,   560,     0,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,     0,     0,     0,
       0,   538,   539,     0,     0,     0,   571,   572,     0,     0,
     887,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     560,     0,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,   572,     0,     0,   888,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   540,   541,   542,   543,   544,
       0,     0,   545,   546,   547,   548,     0,   549,   550,   551,
     552,     0,     0,     0,     0,   553,     0,   554,   555,     0,
       0,     0,     0,   556,   557,   558,     0,     0,     0,   559,
     540,   541,   542,   543,   544,     0,     0,   545,   546,   547,
     548,     0,   549,   550,   551,   552,   538,   539,     0,     0,
     553,     0,   554,   555,     0,     0,     0,     0,   556,   557,
     558,     0,     0,     0,   559,   560,     0,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,     0,     0,     0,
       0,   538,   539,     0,     0,     0,   571,   572,     0,     0,
     889,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     560,     0,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,   572,     0,     0,   959,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   540,   541,   542,   543,   544,
       0,     0,   545,   546,   547,   548,     0,   549,   550,   551,
     552,     0,     0,     0,     0,   553,     0,   554,   555,     0,
       0,     0,     0,   556,   557,   558,     0,     0,     0,   559,
     540,   541,   542,   543,   544,     0,     0,   545,   546,   547,
     548,     0,   549,   550,   551,   552,   538,   539,     0,     0,
     553,     0,   554,   555,     0,     0,     0,     0,   556,   557,
     558,     0,     0,     0,   559,   560,     0,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,     0,     0,     0,
       0,   538,   539,     0,     0,     0,   571,   572,     0,     0,
    1026,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     560,     0,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,   572,     0,     0,  1086,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   540,   541,   542,   543,   544,
       0,     0,   545,   546,   547,   548,     0,   549,   550,   551,
     552,     0,     0,     0,     0,   553,     0,   554,   555,     0,
       0,     0,     0,   556,   557,   558,     0,     0,     0,   559,
     540,   541,   542,   543,   544,     0,     0,   545,   546,   547,
     548,     0,   549,   550,   551,   552,   538,   539,     0,     0,
     553,     0,   554,   555,     0,     0,     0,     0,   556,   557,
     558,     0,     0,     0,   559,   560,     0,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,     0,     0,     0,
       0,   538,   539,     0,     0,     0,   571,   572,     0,     0,
    1133,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     560,     0,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,   572,     0,     0,  1134,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   540,   541,   542,   543,   544,
       0,     0,   545,   546,   547,   548,     0,   549,   550,   551,
     552,     0,     0,     0,     0,   553,     0,   554,   555,     0,
       0,     0,     0,   556,   557,   558,     0,     0,     0,   559,
     540,   541,   542,   543,   544,     0,     0,   545,   546,   547,
     548,     0,   549,   550,   551,   552,   538,   539,     0,     0,
     553,     0,   554,   555,     0,     0,     0,     0,   556,   557,
     558,     0,     0,     0,   559,   560,     0,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,     0,     0,     0,
       0,   538,   539,     0,     0,     0,   571,   572,     0,     0,
    1145,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     560,     0,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,   572,     0,     0,  1147,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   540,   541,   542,   543,   544,
       0,     0,   545,   546,   547,   548,     0,   549,   550,   551,
     552,     0,     0,     0,     0,   553,     0,   554,   555,     0,
       0,     0,     0,   556,   557,   558,     0,     0,     0,   559,
     540,   541,   542,   543,   544,     0,     0,   545,   546,   547,
     548,     0,   549,   550,   551,   552,   538,   539,     0,     0,
     553,     0,   554,   555,     0,     0,     0,     0,   556,   557,
     558,     0,     0,     0,   559,   560,     0,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,     0,     0,     0,
       0,   538,   539,     0,     0,     0,   571,   572,     0,     0,
    1149,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     560,     0,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,   572,     0,     0,  1153,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   540,   541,   542,   543,   544,
       0,     0,   545,   546,   547,   548,     0,   549,   550,   551,
     552,     0,     0,     0,     0,   553,     0,   554,   555,     0,
       0,     0,     0,   556,   557,   558,     0,     0,     0,   559,
     540,   541,   542,   543,   544,     0,     0,   545,   546,   547,
     548,     0,   549,   550,   551,   552,   538,   539,     0,     0,
     553,     0,   554,   555,     0,     0,     0,     0,   556,   557,
     558,     0,     0,     0,   559,   560,     0,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,     0,     0,     0,
       0,   538,   539,     0,     0,     0,   571,   572,     0,     0,
    1191,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     560,     0,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,   572,     0,     0,  1192,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   540,   541,   542,   543,   544,
       0,     0,   545,   546,   547,   548,     0,   549,   550,   551,
     552,     0,     0,     0,     0,   553,     0,   554,   555,     0,
       0,     0,     0,   556,   557,   558,     0,     0,     0,   559,
     540,   541,   542,   543,   544,     0,     0,   545,   546,   547,
     548,     0,   549,   550,   551,   552,   538,   539,     0,     0,
     553,     0,   554,   555,     0,     0,     0,     0,   556,   557,
     558,     0,     0,     0,   559,   560,     0,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,     0,     0,     0,
       0,   538,   539,     0,     0,     0,   571,   572,     0,     0,
    1193,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     560,     0,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,   572,     0,     0,  1202,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   540,   541,   542,   543,   544,
       0,     0,   545,   546,   547,   548,     0,   549,   550,   551,
     552,     0,     0,     0,     0,   553,     0,   554,   555,     0,
       0,     0,     0,   556,   557,   558,     0,     0,     0,   559,
     540,   541,   542,   543,   544,     0,     0,   545,   546,   547,
     548,     0,   549,   550,   551,   552,   538,   539,     0,     0,
     553,     0,   554,   555,     0,     0,     0,     0,   556,   557,
     558,     0,     0,     0,   559,   560,     0,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,     0,     0,     0,
       0,   538,   539,     0,     0,     0,   571,   572,     0,     0,
    1204,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     560,     0,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,   572,     0,     0,  1238,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   540,   541,   542,   543,   544,
       0,     0,   545,   546,   547,   548,     0,   549,   550,   551,
     552,     0,     0,     0,     0,   553,     0,   554,   555,     0,
       0,     0,     0,   556,   557,   558,     0,     0,     0,   559,
     540,   541,   542,   543,   544,   538,   539,   545,   546,   547,
     548,     0,   549,   550,   551,   552,     0,     0,     0,     0,
     553,     0,   554,   555,     0,     0,     0,     0,   556,   557,
     558,     0,     0,     0,   559,   560,     0,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   571,   572,   576,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     560,     0,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,   572,   942,   540,   541,   542,   543,   544,   538,
     539,   545,   546,   547,   548,     0,   549,   550,   551,   552,
       0,     0,     0,     0,   553,     0,   554,   555,     0,     0,
       0,     0,   556,   557,   558,     0,     0,     0,   559,     0,
       0,     0,     0,     0,   538,   539,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   560,     0,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   571,   572,   956,   540,   541,
     542,   543,   544,     0,     0,   545,   546,   547,   548,     0,
     549,   550,   551,   552,     0,     0,     0,     0,   553,     0,
     554,   555,     0,     0,     0,     0,   556,   557,   558,     0,
       0,     0,   559,   540,   541,   542,   543,   544,     0,     0,
     545,   546,   547,   548,     0,   549,   550,   551,   552,   538,
     539,     0,     0,   553,     0,   554,   555,     0,     0,     0,
       0,   556,   557,   558,     0,     0,     0,   559,   560,     0,
     561,   562,   563,   564,   565,   566,   567,   568,   569,   570,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   571,
     572,  1081,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   560,     0,   561,   562,   563,   564,   565,
     566,   567,   568,   569,   570,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   571,   572,  1084,     0,     0,     0,
     825,   826,   827,   828,   829,   830,   831,   832,   540,   541,
     542,   543,   544,   833,   834,   545,   546,   547,   548,   970,
     549,   550,   551,   552,  -318,     0,   278,   279,   553,   836,
     554,   555,   837,   838,     0,     0,   556,   557,   558,   839,
     840,   841,   559,   280,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   538,   539,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   971,   560,     0,
     561,   562,   563,   564,   565,   566,   567,   568,   569,   570,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   571,
     572,     0,     0,     0,     0,     0,     0,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,     0,     0,   299,   300,   301,
       0,     0,     0,     0,     0,     0,   302,   303,   304,   305,
     306,     0,     0,   307,   308,   309,   310,   311,   312,   313,
     540,   541,   542,   543,   544,   538,   539,   545,   546,   547,
     548,     0,   549,   550,   551,   552,     0,     0,     0,     0,
     553,     0,   554,   555,     0,     0,   705,     0,   556,   557,
     558,     0,     0,     0,   559,   314,     0,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,     0,     0,   325,
     326,     0,     0,     0,     0,     0,   327,   328,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     560,     0,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,   572,     0,   540,   541,   542,   543,   544,   538,
     539,   545,   546,   547,   548,     0,   549,   550,   551,   552,
       0,     0,     0,     0,   553,     0,   554,   555,     0,     0,
     895,     0,   556,   557,   558,     0,     0,     0,   559,     0,
       0,     0,     0,     0,   538,   539,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   560,     0,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   571,   572,     0,   540,   541,
     542,   543,   544,     0,     0,   545,   546,   547,   548,     0,
     549,   550,   551,   552,     0,     0,     0,     0,   553,     0,
     554,   555,     0,     0,     0,     0,   556,   557,   558,     0,
     538,   539,   559,   540,   541,   542,   543,   544,     0,     0,
     545,   546,   547,   548,     0,   549,   550,   551,   552,     0,
       0,     0,     0,   553,     0,   554,   555,     0,     0,     0,
       0,   556,   557,   558,     0,   538,   539,   559,   560,   947,
     561,   562,   563,   564,   565,   566,   567,   568,   569,   570,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   571,
     572,     0,     0,     0,  1054,     0,     0,     0,     0,     0,
       0,     0,     0,   560,     0,   561,   562,   563,   564,   565,
     566,   567,   568,   569,   570,     0,     0,     0,     0,   540,
     541,   542,   543,   544,   571,   572,   545,   546,   547,   548,
       0,   549,   550,   551,   552,     0,     0,     0,     0,   553,
       0,   554,   555,     0,     0,     0,     0,   556,   557,   558,
       0,   538,   539,   559,   540,   541,   542,   543,   544,     0,
       0,   545,   546,   547,   548,     0,   549,   550,   551,   552,
       0,     0,     0,     0,   553,     0,   554,   555,     0,     0,
       0,     0,   556,   557,   558,   538,   539,     0,  -656,   560,
       0,   561,   562,   563,   564,   565,   566,   567,   568,   569,
     570,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     571,   572,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   560,     0,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,     0,     0,     0,     0,
     540,   541,   542,   543,   544,   571,   572,   545,   546,   547,
     548,     0,   549,   550,   551,   552,     0,     0,     0,     0,
     553,     0,   554,   555,     0,     0,     0,     0,   556,   557,
     558,   538,   539,     0,   540,   541,   542,   543,   544,     0,
       0,   545,   546,   547,   548,     0,   549,   550,   551,   552,
       0,     0,     0,     0,   553,     0,   554,   555,     0,     0,
     538,   539,   556,     0,   558,     0,     0,     0,     0,     0,
     560,     0,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,   572,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,     0,     0,     0,     0,
     540,   541,   542,   543,   544,   571,   572,   545,   546,   547,
     548,     0,   549,   550,   551,   552,     0,     0,     0,     0,
     553,     0,   554,   555,     0,     0,   538,   539,   556,   540,
     541,   542,   543,   544,     0,     0,   545,   546,   547,   548,
       0,   549,   550,   551,   552,     0,     0,     0,     0,   553,
       0,   554,   555,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,   572,     0,     0,     0,     0,     0,     0,     0,
       0,   561,   562,   563,   564,   565,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   540,   541,   542,   543,   544,
     571,   572,   545,   546,   547,   548,     0,   549,   550,   551,
     552,     0,     0,     0,     0,   553,     0,   554,   555,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   518,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   157,     0,     0,     0,     0,     0,
     158,     0,     0,     0,     0,     0,     0,     0,     0,   563,
     564,   565,   566,   567,   568,   569,   570,   159,     0,     0,
       0,     0,     0,     0,     0,     0,   571,   572,     0,     0,
       0,     0,   160,     0,     0,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   520,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   157,     0,     0,     0,     0,     0,   158,
       0,     0,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   159,     0,     0,   194,
     727,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   160,     0,    54,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   161,   162,   163,
       0,   165,   166,   167,   168,   169,   379,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,   183,
     184,   185,     0,     0,   188,   189,   190,   191,     0,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   731,     0,     0,   194,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   943,     0,     0,     0,
       0,     0,     0,   728,     0,     0,     0,     0,     0,     0,
       0,     0,   161,   162,   163,   729,   165,   166,   167,   168,
     169,   379,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,     0,   183,   184,   185,     0,     0,   188,
     189,   190,   191,   161,   162,   163,     0,   165,   166,   167,
     168,   169,   379,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,     0,   183,   184,   185,     0,     0,
     188,   189,   190,   191,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   732,     0,
       0,   157,     0,     0,     0,     0,     0,   158,     0,     0,
     733,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   159,     0,     0,     0,     0,   944,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   160,
       0,   945,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     157,     0,     0,     0,     0,     0,   158,     0,     0,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   159,     0,     0,   194,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   160,     0,
      54,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   207,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   194,     0,     0,     0,     0,
     208,     0,   209,     0,   210,   211,   212,   213,   214,   492,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,     0,   226,   227,   228,     0,     0,   229,   230,   231,
     232,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   233,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   235
};

static const yytype_int16 yycheck[] =
{
      12,   273,   123,   237,   574,   144,   463,   583,   587,   487,
     360,   489,   362,   491,   364,   471,   521,   697,   721,   596,
     898,   420,   123,   703,   608,   609,   610,   448,     7,    90,
     607,    14,    15,    19,    51,    19,   518,   135,   520,    33,
      52,    19,    51,    55,    56,   125,    44,    20,    21,   178,
      32,   122,   123,    18,    19,   163,    48,   163,     0,   119,
     148,   136,   122,   123,     6,   153,   163,    61,   120,   137,
     138,   139,   181,   137,   138,   139,   120,   654,   207,    91,
      92,    93,    94,   156,    55,   120,   120,    29,    21,    31,
      61,    33,    60,    20,    21,   130,   130,    39,   207,   120,
     208,   136,   100,   209,   674,   180,    48,   691,   205,   130,
     208,   123,    54,   595,    20,    21,   204,   180,   390,   391,
      32,   163,    55,   120,   163,   177,   178,   207,   180,   200,
     201,   183,    74,   130,   178,   129,   204,   181,   111,   112,
     200,   201,   136,   178,   178,   208,   119,   177,   121,   122,
     123,   124,   273,   145,   163,    97,   129,   178,   178,  1037,
     151,    32,    14,    15,   177,   207,   120,   209,   207,   163,
     209,   163,   163,   156,   203,   163,   130,   207,   161,   208,
     163,   178,   165,   244,   111,   112,   183,    58,    59,   163,
     207,   177,   119,   177,   897,   122,   123,   124,   163,   177,
     194,   183,   129,   163,   476,   111,   112,   346,   785,   203,
      44,    45,   789,   119,   894,   121,   122,   123,   124,    43,
     203,   209,   201,   129,   178,    55,   803,   200,   201,   203,
     209,    61,   353,   354,   355,   356,   177,   509,   359,   823,
     361,   172,   363,   177,   365,   644,   117,  1125,   180,   209,
     121,   672,   353,   354,   355,   356,   189,    81,   359,   201,
     361,   273,   363,   204,   177,   207,   100,   200,   202,   390,
     391,   183,   177,   200,   201,   852,   207,   174,   209,   136,
     186,   187,   188,   189,   190,   163,   177,   163,   145,   201,
     122,   123,   177,   150,   200,   201,  1183,    32,   210,    20,
      21,   173,   207,   177,   156,   202,   163,    32,   179,   161,
     207,   163,   183,   165,   185,   186,   207,   202,   173,   151,
     890,   176,  1209,    58,    59,   136,   447,   184,   202,   136,
     201,   163,   177,    58,    59,   207,   136,   209,   908,   210,
     461,   353,   354,   355,   356,   145,   447,   359,    32,   361,
     177,   363,   163,   365,   209,   476,  1036,   369,  1061,   204,
     461,   151,   177,   163,   598,    55,   822,  1047,   200,   201,
     940,    61,   606,   163,    58,    59,   726,   204,   390,   391,
       4,     5,   117,     7,   963,   120,   121,   202,   509,   894,
     111,   112,   117,   405,   177,   130,   121,   967,   119,   163,
     121,   122,   123,   124,   909,   177,   678,   679,   129,   681,
     151,    35,   177,   997,   998,   999,   177,   538,   539,   174,
     175,   204,   163,  1007,  1008,  1009,   163,   164,   177,   137,
      55,   139,   204,   117,   177,   447,    61,   121,   710,   204,
     180,   202,   181,   178,   179,   185,   185,   202,   183,   461,
     163,   186,   185,   190,   179,   204,   468,   935,   183,   202,
    1039,   186,  1032,   203,   476,   136,   201,   188,   189,   190,
     145,    55,   207,   180,   207,   210,   201,    61,   185,   200,
     201,   180,   153,   154,   155,   210,   185,   181,   163,   501,
      52,   185,   163,    55,    56,   179,   203,   509,   180,   183,
      11,   185,   186,   185,   203,   185,   185,   145,    11,    20,
      21,   148,   185,   184,    69,   203,   153,   201,    73,    22,
      23,   203,   534,   163,   164,   163,   210,   207,   207,    91,
      92,    93,    94,   156,   207,    55,   657,    92,    93,    94,
      95,    61,   177,   205,   206,   180,   208,   668,   183,   670,
     181,   200,   673,   787,   185,   676,   657,   678,   679,   181,
     681,   181,   796,   185,   163,   185,   181,   668,    32,   181,
     185,   178,   673,   185,   924,   676,   810,   207,   181,   181,
     814,  1059,   185,   185,   166,   167,   181,   208,   262,   710,
     185,  1167,   163,   204,    58,    59,   207,   271,   109,   110,
     111,   112,   113,   166,   167,   116,   117,   118,   119,   163,
     121,   122,   123,   124,   166,   167,   168,   169,   129,   208,
     131,   132,   166,   167,   168,    55,   137,   138,   139,    76,
      77,   207,   143,   754,   163,  1092,  1224,  1225,   208,   137,
     100,   153,   184,   184,   184,   657,   184,   203,   184,   184,
     184,   923,   184,   117,   184,   203,   668,   121,   670,   184,
     181,   673,   783,   163,   676,    34,   678,   679,   179,   681,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     207,    34,   783,   163,    20,    21,   203,   209,   163,   200,
     201,   180,   208,   207,   163,   163,   163,   202,   710,    21,
     163,   202,   180,   180,   208,   204,   208,   381,   382,   848,
     203,    41,   203,   184,   184,   179,   203,   203,   392,   183,
     203,   185,   186,   184,   203,   184,   184,   203,   402,   403,
     404,   203,   406,   407,   408,   184,   201,   201,   972,   163,
     206,   203,   130,   163,   163,   202,   210,   203,   203,   203,
     203,   202,   185,   203,   153,   163,    41,   203,   163,   207,
     156,    36,   207,   207,     9,   207,   207,   207,   207,   207,
      64,   783,    41,   109,   110,   111,   112,   113,   163,   185,
     116,   117,   118,   119,   185,   121,   122,   123,   124,   463,
     185,   177,   209,   129,   177,   131,   132,   184,  1070,   473,
     166,   203,   923,   203,   163,   203,   184,     1,   163,   203,
     203,   203,   203,   163,   125,    13,   178,   180,   156,   208,
       7,   163,   206,   163,   203,   202,   163,   202,   202,   503,
     163,    32,   506,   845,   508,  1069,   163,   166,   512,   513,
     514,   515,   516,   517,   163,   519,   163,   521,   184,   185,
     186,   187,   188,   189,   190,   203,   184,    58,    59,  1093,
     203,    41,   203,   207,   200,   201,   540,   541,   203,   185,
     544,   545,   546,   547,   163,   549,   202,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,   204,   572,   202,
     163,   207,   207,    20,    21,    32,   185,   204,   582,    51,
     202,   923,   202,   209,   202,    69,   117,   526,   592,    38,
     121,   207,   596,  1157,   120,   599,   600,  1161,    72,  1195,
      70,   605,  1196,   607,   249,   845,   982,   611,    32,  1196,
    1196,  1196,   616,  1196,     1,  1067,   883,  1108,  1166,  1070,
     589,  1072,  1111,   670,   366,   650,    47,   464,   369,   369,
    1130,   358,   906,   707,    58,    59,  1180,   641,   521,    -1,
      -1,  1072,    -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,
     654,    -1,   183,    -1,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   110,   111,   112,   113,    -1,    -1,   116,
     201,   675,   119,  1237,   121,   122,   123,   124,    -1,   210,
      -1,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,   693,
     694,    -1,    -1,   117,    -1,    -1,    -1,   121,   702,  1150,
      -1,   705,    -1,   707,    -1,    -1,    -1,    -1,   712,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1070,    -1,
    1072,    -1,    -1,    -1,    -1,    -1,    -1,   184,   185,   186,
     187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   200,   201,   179,    -1,    -1,    -1,   183,
      -1,   185,   186,   767,    -1,    -1,    -1,   771,    -1,    -1,
      -1,    -1,    -1,    -1,   778,    -1,    -1,   201,    -1,    -1,
      -1,   785,    -1,    -1,    -1,   789,   210,    -1,    -1,    -1,
     794,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,   803,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   825,   826,   827,   828,   829,   830,   831,   832,   833,
     834,   835,   836,   837,   838,   839,   840,   841,   842,    -1,
      -1,  1183,    -1,    -1,    -1,    -1,    -1,    -1,   852,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1209,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,   111,   112,    -1,   892,   893,
      -1,   895,    -1,   119,    -1,   121,   122,   123,   124,   903,
      20,    21,   906,   129,    -1,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   921,   922,    -1,
      -1,    -1,    -1,   927,    -1,    -1,    -1,     9,    -1,    -1,
      -1,    -1,    -1,   937,    -1,   939,    -1,   941,    20,    21,
      -1,    -1,    -1,   947,    -1,    -1,    -1,   951,    -1,    -1,
      -1,    -1,    -1,   957,    -1,    -1,    -1,    -1,    -1,    -1,
     186,   187,   188,   189,   190,    -1,   970,   971,    -1,    -1,
      -1,    -1,    -1,    -1,   200,   201,    -1,    -1,    -1,    -1,
     984,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,   109,
     110,   111,   112,   113,    -1,    -1,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,   129,
      -1,   131,   132,    -1,    58,    59,    -1,    -1,    -1,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    -1,    -1,    -1,    -1,   129,   130,   131,
     132,   133,   134,    -1,    -1,   137,   138,   139,   140,   141,
     142,   143,   182,   183,   184,   185,   186,   187,   188,   189,
     190,    -1,    -1,   117,  1078,    -1,  1080,   121,    -1,  1083,
     200,   201,    -1,    -1,  1088,    -1,    -1,  1091,  1092,    -1,
      -1,    -1,  1096,    -1,    -1,    -1,   178,   179,    -1,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,    -1,
      -1,  1115,    -1,    -1,  1118,  1119,  1120,  1121,   200,   201,
    1124,    -1,    -1,  1127,  1128,  1129,    -1,    -1,  1132,    -1,
      -1,    -1,    -1,    -1,    -1,   179,    -1,  1141,    -1,   183,
      -1,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   210,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,  1186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1196,    -1,    -1,    -1,    -1,    -1,    -1,  1203,
      -1,    -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,
      -1,    -1,  1216,  1217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1227,     1,    -1,    -1,    -1,  1232,     6,
    1234,     8,     9,    10,    -1,    12,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,   117,    42,    43,    -1,   121,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      87,    88,    89,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    98,    99,    -1,    -1,    -1,   179,    -1,    -1,    -1,
     183,    -1,   185,   186,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   126,
     127,   128,    -1,    -1,    -1,    -1,    -1,   210,    -1,   136,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,
     147,   148,    -1,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,
     187,   188,    -1,   190,    -1,    -1,   193,   194,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   203,    -1,   205,   206,
     207,   208,   209,     1,    -1,    -1,    -1,    -1,     6,    -1,
       8,     9,    10,    -1,    12,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      38,    -1,    40,    -1,    42,    43,    -1,    -1,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,   147,
     148,    -1,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,   187,
     188,    -1,   190,    -1,    -1,   193,   194,    -1,    -1,    -1,
      14,    15,    16,    17,    18,   203,    -1,   205,   206,   207,
     208,   209,    26,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    51,    -1,    53,
      -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    89,    -1,    -1,    92,    93,
      94,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,   145,   146,   147,   148,    -1,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,   183,
      -1,    -1,   186,   187,   188,    -1,   190,    -1,    -1,   193,
     194,    14,    15,    16,    17,    18,    -1,   201,    -1,   203,
      -1,   205,   206,    26,   208,    -1,   210,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    -1,    87,    88,    89,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,   146,   147,   148,    -1,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,
     183,    -1,    -1,   186,   187,   188,    -1,   190,    -1,    -1,
     193,   194,    14,    15,    16,    17,    18,    -1,   201,    -1,
     203,    -1,   205,   206,    26,   208,    -1,   210,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    -1,
      92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,   147,   148,    -1,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,   186,   187,   188,    -1,   190,    -1,
      -1,   193,   194,    14,    15,    16,    17,    18,    -1,   201,
      -1,   203,    -1,   205,   206,    26,   208,    -1,   210,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   186,   187,   188,    -1,   190,
      -1,    -1,   193,   194,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   203,    46,   205,   206,    49,   208,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    -1,    87,    88,    89,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,   146,   147,   148,    -1,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   186,   187,   188,    -1,   190,    -1,    -1,
     193,   194,    -1,    -1,    -1,    14,    15,    16,    17,    18,
     203,    -1,   205,   206,    -1,   208,    25,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    87,    88,
      89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,   145,   146,   147,   148,
      -1,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,   186,   187,   188,
      -1,   190,    -1,    -1,   193,   194,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,   203,    46,   205,   206,    49,   208,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   186,   187,   188,    -1,   190,
      -1,    -1,   193,   194,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   203,    46,   205,   206,    49,   208,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    -1,    87,    88,    89,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,   146,   147,   148,    -1,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   186,   187,   188,    -1,   190,    -1,    -1,
     193,   194,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     203,    46,   205,   206,    49,   208,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    89,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
     145,   146,   147,   148,    -1,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   186,   187,   188,    -1,   190,    -1,    -1,   193,   194,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   203,    46,
     205,   206,    49,   208,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      87,    88,    89,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,
     147,   148,    -1,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,
     187,   188,    -1,   190,    -1,    -1,   193,   194,    14,    15,
      16,    17,    18,    -1,   201,    -1,   203,   204,   205,   206,
      26,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
     146,   147,   148,    -1,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    21,    -1,    -1,    -1,    -1,    26,    -1,
     186,   187,   188,    -1,   190,    -1,    -1,   193,   194,    -1,
      38,    -1,    -1,    -1,    -1,    43,   202,   203,    46,   205,
     206,    49,   208,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,   147,
     148,    -1,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   186,   187,
     188,    -1,   190,    -1,    -1,   193,   194,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,   203,    46,   205,   206,    49,
     208,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
      -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,   187,   188,    -1,
     190,    -1,    -1,   193,   194,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   203,   204,   205,   206,    26,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      59,    -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    87,    88,
      89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,   145,   146,   147,   148,
      -1,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,   186,   187,   188,
      -1,   190,    -1,    -1,   193,   194,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,   203,    46,   205,   206,    49,   208,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   186,   187,   188,    -1,   190,
      -1,    -1,   193,   194,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   203,    46,   205,   206,    49,   208,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    -1,    87,    88,    89,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,   146,   147,   148,    -1,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   184,    -1,   186,   187,   188,    -1,   190,    -1,    -1,
     193,   194,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     203,    -1,   205,   206,    26,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    -1,
      92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,   147,   148,    -1,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,   187,   188,    -1,   190,    -1,
      -1,   193,   194,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   203,   204,   205,   206,    26,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   184,    -1,   186,   187,   188,    -1,   190,
      -1,    -1,   193,   194,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   203,    -1,   205,   206,    26,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
      -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,   187,   188,    -1,
     190,    -1,    -1,   193,   194,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   203,   204,   205,   206,    26,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    87,    88,
      89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,   145,   146,   147,   148,
      -1,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,   187,   188,
      -1,   190,    -1,    -1,   193,   194,    14,    15,    16,    17,
      18,    -1,    -1,    -1,   203,   204,   205,   206,    26,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,   147,
     148,    -1,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,   186,   187,
     188,    -1,   190,    -1,    26,   193,   194,    -1,    -1,    -1,
      -1,    -1,    -1,   201,    -1,   203,    38,   205,   206,    -1,
     208,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    -1,
      92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,   147,   148,    -1,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,   187,   188,    -1,   190,    -1,
      -1,   193,   194,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   203,   204,   205,   206,    26,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   186,   187,   188,    -1,   190,
      -1,    -1,   193,   194,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   203,   204,   205,   206,    26,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
      -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,   187,   188,    -1,
     190,    -1,    -1,   193,   194,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   203,   204,   205,   206,    26,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    87,    88,
      89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,   145,   146,   147,   148,
      -1,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,   186,   187,   188,
      -1,   190,    -1,    -1,   193,   194,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,   203,    46,   205,   206,    49,   208,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   186,   187,   188,    -1,   190,
      -1,    -1,   193,   194,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   203,    46,   205,   206,    49,   208,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    -1,    87,    88,    89,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,   146,   147,   148,    -1,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   186,   187,   188,    -1,   190,    -1,    -1,
     193,   194,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     203,    46,   205,   206,    49,   208,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    89,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
     145,   146,   147,   148,    -1,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   186,   187,   188,    -1,   190,    -1,    -1,   193,   194,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,    -1,
     205,   206,    -1,   208,   109,   110,   111,   112,   113,    -1,
      -1,   116,   117,   118,   119,    -1,   121,   122,   123,   124,
      -1,    -1,    -1,    -1,   129,    -1,   131,   132,    20,    21,
      -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   179,    -1,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   200,   201,    -1,    -1,    -1,
      -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    -1,    -1,    20,    21,   129,   130,   131,
     132,   133,   134,    -1,    -1,   137,   138,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   178,   179,    -1,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,
      -1,    -1,    -1,    -1,    -1,   207,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,   134,
      -1,    -1,   137,   138,   139,   140,   141,   142,   143,    -1,
      -1,   109,   110,   111,   112,   113,    -1,    -1,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    20,    21,    -1,
      -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,
     138,   139,    -1,   178,   179,   143,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   200,   201,    -1,    -1,    -1,
      -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   179,    -1,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   200,   201,    -1,    -1,    -1,    -1,    -1,   207,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,    -1,    -1,   116,   117,   118,   119,    -1,   121,   122,
     123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,
      -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,   179,    -1,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,    -1,
      -1,    -1,    -1,    62,   207,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,   109,
     110,   111,   112,   113,    -1,    -1,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    20,    21,    -1,   129,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,   139,
      -1,    -1,    -1,   143,    -1,    -1,    -1,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,    -1,   179,
      -1,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      -1,   210,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    20,    21,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,   179,    -1,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   200,   201,    -1,    -1,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    20,    21,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,   179,    -1,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   200,   201,    -1,    -1,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    20,    21,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,   179,    -1,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   200,   201,    -1,    -1,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    20,    21,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,   179,    -1,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   200,   201,    -1,    -1,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    20,    21,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,   179,    -1,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   200,   201,    -1,    -1,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    20,    21,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,   179,    -1,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   200,   201,    -1,    -1,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    20,    21,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,   179,    -1,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   200,   201,    -1,    -1,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    20,    21,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,   179,    -1,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   200,   201,    -1,    -1,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    20,    21,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,   179,    -1,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   200,   201,    -1,    -1,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    20,    21,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,   179,    -1,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   200,   201,    -1,    -1,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    20,    21,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,   179,    -1,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   200,   201,    -1,    -1,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    20,    21,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,   179,    -1,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   200,   201,    -1,    -1,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    20,    21,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,   179,    -1,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   200,   201,    -1,    -1,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
     109,   110,   111,   112,   113,    20,    21,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,   179,    -1,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,   202,   109,   110,   111,   112,   113,    20,
      21,   116,   117,   118,   119,    -1,   121,   122,   123,   124,
      -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,
      -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   179,    -1,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   200,   201,   202,   109,   110,
     111,   112,   113,    -1,    -1,   116,   117,   118,   119,    -1,
     121,   122,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,
     131,   132,    -1,    -1,    -1,    -1,   137,   138,   139,    -1,
      -1,    -1,   143,   109,   110,   111,   112,   113,    -1,    -1,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    20,
      21,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,
      -1,   137,   138,   139,    -1,    -1,    -1,   143,   179,    -1,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,
     201,   202,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   179,    -1,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   200,   201,   202,    -1,    -1,    -1,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,    20,    21,   129,   130,
     131,   132,   133,   134,    -1,    -1,   137,   138,   139,   140,
     141,   142,   143,    37,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,   179,    -1,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,    -1,    -1,   137,   138,   139,   140,   141,   142,   143,
     109,   110,   111,   112,   113,    20,    21,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,   135,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,   179,    -1,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,    -1,    -1,   193,
     194,    -1,    -1,    -1,    -1,    -1,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,    -1,   109,   110,   111,   112,   113,    20,
      21,   116,   117,   118,   119,    -1,   121,   122,   123,   124,
      -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,
     135,    -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   179,    -1,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   200,   201,    -1,   109,   110,
     111,   112,   113,    -1,    -1,   116,   117,   118,   119,    -1,
     121,   122,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,
     131,   132,    -1,    -1,    -1,    -1,   137,   138,   139,    -1,
      20,    21,   143,   109,   110,   111,   112,   113,    -1,    -1,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    -1,
      -1,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,
      -1,   137,   138,   139,    -1,    20,    21,   143,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,
     201,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   179,    -1,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   200,   201,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,   129,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,   139,
      -1,    20,    21,   143,   109,   110,   111,   112,   113,    -1,
      -1,   116,   117,   118,   119,    -1,   121,   122,   123,   124,
      -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,
      -1,    -1,   137,   138,   139,    20,    21,    -1,   143,   179,
      -1,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   179,    -1,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,    -1,    -1,    -1,    -1,
     109,   110,   111,   112,   113,   200,   201,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    20,    21,    -1,   109,   110,   111,   112,   113,    -1,
      -1,   116,   117,   118,   119,    -1,   121,   122,   123,   124,
      -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,
      20,    21,   137,    -1,   139,    -1,    -1,    -1,    -1,    -1,
     179,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,    -1,    -1,    -1,    -1,
     109,   110,   111,   112,   113,   200,   201,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    20,    21,   137,   109,
     110,   111,   112,   113,    -1,    -1,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,   129,
      -1,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
     200,   201,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,
     184,   185,   186,   187,   188,   189,   190,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   200,   201,    -1,    -1,
      -1,    -1,    62,    -1,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,   149,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    -1,   163,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    66,    67,   163,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    -1,    87,    88,    89,    -1,    -1,    92,
      93,    94,    95,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    -1,
      92,    93,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,   163,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,
     163,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      65,    -1,    67,    -1,    69,    70,    71,    72,    73,   163,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    89,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   212,     0,     6,    29,    31,    33,    39,    48,    54,
      74,    97,   201,   207,   215,   223,   224,   229,   257,   261,
     287,   355,   361,   364,   370,   409,   412,    18,    19,   163,
     250,   251,   252,   156,   230,   231,   163,   164,   190,   225,
     226,   227,   163,   208,   358,   163,   205,   214,   413,   410,
      33,    61,   129,   136,   163,   194,   203,   253,   254,   255,
     256,   274,     4,     5,     7,    35,   368,    60,   353,   178,
     177,   180,   177,   226,    21,    55,   189,   200,   228,   359,
     358,   360,   353,   163,   163,   255,   163,   136,   255,   255,
     203,   137,   138,   139,   177,   202,    55,    61,   262,   264,
      55,    61,   362,    55,    61,   369,    55,    61,   354,    14,
      15,   156,   161,   163,   165,   203,   217,   251,   156,   231,
     163,   225,   225,   178,   207,   209,   358,    55,    61,   213,
     208,   411,   163,   204,   252,   255,   255,   255,   255,   265,
     163,   363,   371,   208,   356,   166,   167,   216,    14,    15,
     156,   161,   163,   217,   248,   249,   228,    24,    30,    47,
      62,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,   149,   274,   374,   376,   377,   379,
     383,   408,   207,   163,   378,   208,   204,    34,    65,    67,
      69,    70,    71,    72,    73,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    87,    88,    89,    92,
      93,    94,    95,   111,   112,   163,   260,   263,   180,   208,
     100,   366,   367,   351,   153,   330,   166,   167,   168,   177,
     204,   184,   184,   184,   184,   203,   184,   184,   184,   184,
     184,   184,   203,    32,    58,    59,   117,   121,   179,   183,
     186,   201,   210,   181,   207,   163,   343,   344,    20,    21,
      37,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   121,
     122,   123,   130,   131,   132,   133,   134,   137,   138,   139,
     140,   141,   142,   143,   179,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   193,   194,   200,   201,    34,
      34,   203,   258,   208,   266,    69,    73,    92,    93,    94,
      95,   375,   357,   163,   372,   209,   352,   252,   145,   163,
     349,   350,   248,   386,   388,   390,   384,   163,   380,   392,
     394,   396,   398,   400,   402,   404,   406,    14,    15,    16,
      17,    18,    26,    38,    43,    46,    49,    53,    63,    74,
      98,   111,   112,   144,   145,   146,   147,   148,   150,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     164,   165,   186,   187,   188,   190,   193,   194,   203,   205,
     206,   219,   221,   266,   270,   274,   279,   290,   297,   300,
     303,   307,   308,   311,   312,   314,   319,   322,   329,   374,
     414,   423,   427,   429,   431,    32,   183,    32,   183,   201,
     210,   202,   322,   210,   383,   163,   209,   180,   207,   163,
     163,   163,   202,    21,   163,   202,   148,   204,   330,   339,
     340,   180,   259,   268,   208,   163,   207,   209,   180,   365,
     208,   330,   202,   203,    41,   177,   180,   183,   348,   408,
     382,   408,   408,   408,   204,   378,   408,   258,   408,   258,
     408,   258,   163,   341,   342,   408,   344,   184,   276,   376,
     414,   203,   184,   203,   184,   184,   203,   184,   203,   184,
     322,   322,   203,   203,   203,   203,   203,   203,    12,   383,
      12,   383,   322,   425,   428,   184,   218,   322,   322,   322,
     274,   322,   322,   322,   206,   163,   203,   257,    20,    21,
     109,   110,   111,   112,   113,   116,   117,   118,   119,   121,
     122,   123,   124,   129,   131,   132,   137,   138,   139,   143,
     179,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   200,   201,   204,   203,   202,   202,   408,   209,   343,
     163,   130,   203,    48,   145,   163,   346,   373,   204,   207,
     408,     1,     8,     9,    10,    12,    25,    27,    28,    37,
      40,    42,    50,    52,    56,    57,    63,    99,   126,   127,
     128,   164,   207,   209,   232,   233,   236,   239,   240,   242,
     243,   244,   245,   246,   267,   269,   273,   275,   280,   281,
     282,   283,   284,   285,   286,   287,   289,   309,   310,   322,
     357,   178,   207,   274,   335,   350,   202,   322,   163,   163,
     383,   120,   130,   178,   347,   185,   185,   207,   185,   185,
     185,   207,   185,   259,   185,   259,   185,   259,   180,   185,
     207,   185,   207,   277,   203,   274,   298,   322,   291,   293,
     322,   295,   322,   383,   322,   322,   322,   322,   322,   322,
     373,    51,   163,   186,   203,   322,   415,   416,   417,   424,
     426,   373,   203,   416,   426,   135,   172,   207,   209,   418,
     271,   168,   169,   216,   222,   204,   145,   150,   184,   274,
     313,   201,   204,   301,   322,   153,   306,    18,   151,   163,
     374,    18,   151,   163,   374,   322,   322,   322,   322,   322,
     322,   163,   322,   151,   163,   322,   322,   322,   322,   322,
     322,   322,   322,   322,    21,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   122,   123,   151,   163,
     200,   201,   320,   322,   204,   301,   322,   346,   203,    41,
     120,   177,   178,   180,   183,   345,   339,   322,   373,   120,
     273,   309,   322,   266,    59,   322,   322,   163,   207,   156,
      56,   322,   266,   120,   273,   322,   206,   307,   307,   307,
     322,    36,   207,   207,   322,     9,   207,   207,   207,   207,
     207,    64,   288,   125,   207,   101,   102,   103,   104,   105,
     106,   107,   108,   114,   115,   120,   130,   133,   134,   140,
     141,   142,   178,   209,   322,   201,   209,   257,   336,   204,
      41,   207,   347,   273,   322,   387,   389,   408,   391,   385,
     381,   393,   185,   397,   185,   401,   185,   408,   405,   341,
     407,   408,   204,   301,   184,   322,   408,   204,   383,   383,
     204,   383,   204,   185,   204,   204,   204,   204,   204,   204,
      19,   307,   130,   345,   204,   135,   177,   207,   417,   174,
     175,   202,   421,   177,   174,   202,   207,   420,    19,   204,
     417,   173,   209,   419,   322,   425,   209,   383,   322,   166,
     220,   203,   203,   315,   317,   163,   415,   177,   204,   120,
     130,   178,   183,   304,   305,   258,   184,   203,   184,   203,
     203,   203,   202,    18,   151,   163,   374,   180,   151,   163,
     322,   203,   151,   163,   322,     1,   202,   177,   204,   204,
     322,   163,   163,   408,   273,   322,   266,    19,   273,   322,
     120,   178,    13,   322,   266,   178,   180,   156,   273,   322,
     206,   266,   208,   266,   241,   350,   307,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   126,   127,   128,
     322,   322,   322,   322,   322,   322,   322,   126,   127,   128,
     322,   207,   256,     7,   330,   334,   163,   273,   322,   207,
     395,   399,   403,   185,   204,   163,   204,   185,   185,   185,
     185,   306,   203,   301,   322,   322,   416,   417,   322,   163,
     415,   421,   202,   322,   202,   424,   301,   416,   173,   176,
     209,   422,   202,   185,   170,   166,   322,   322,   383,   258,
     202,   201,   322,   163,   163,   163,   163,   177,   202,   259,
     323,   322,   325,   322,   204,   301,   322,   184,   203,   322,
     203,   202,   322,   203,   202,   321,   204,    41,   345,   301,
     266,   247,   268,    11,    22,    23,   234,   235,   322,   307,
     307,   307,   307,   307,   307,   202,    55,    61,   333,    44,
     100,   331,   207,   207,   278,   185,   207,   299,   292,   294,
     296,   203,   204,   301,   207,   417,   421,   130,   345,   207,
     417,   202,   272,   204,   204,   185,   259,   204,   415,   304,
     202,   135,   266,   302,   383,   204,   408,   204,   204,   204,
     327,   322,   322,   204,   322,   163,   322,   207,   322,   209,
     266,   322,    11,   237,   207,    45,   331,   333,   322,   163,
     322,   322,   322,   204,   322,   204,   322,   421,   322,   322,
     322,   422,   322,   316,   185,   202,   120,   322,   185,   185,
     408,   204,   204,   204,   266,   266,   238,   207,    32,   332,
     332,   346,   204,   185,   204,   207,   430,   430,   313,   318,
     204,   322,   324,   326,   185,   235,    25,    99,   239,   280,
     281,   282,   284,   322,   337,   338,   322,    51,   202,   209,
     313,   328,   120,   322,   120,   322,   263,   263,   204,   322,
     202,   202,   322,   322,   207,   266
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   211,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   213,   213,   213,   214,   214,
     215,   216,   216,   216,   216,   217,   218,   218,   218,   219,
     220,   220,   222,   221,   223,   224,   225,   225,   226,   226,
     226,   226,   227,   227,   228,   228,   229,   230,   230,   231,
     231,   232,   233,   233,   234,   234,   235,   235,   235,   236,
     236,   237,   238,   237,   239,   239,   239,   239,   239,   240,
     241,   240,   242,   243,   244,   245,   247,   246,   248,   248,
     248,   248,   248,   248,   249,   249,   250,   250,   250,   251,
     251,   251,   251,   251,   251,   251,   251,   252,   252,   253,
     253,   253,   254,   254,   255,   255,   255,   255,   255,   255,
     255,   256,   256,   257,   257,   258,   258,   258,   259,   259,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   261,   262,   262,   262,   263,
     265,   264,   266,   266,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   268,   268,   268,   269,   271,
     272,   270,   273,   273,   273,   273,   274,   274,   274,   275,
     275,   277,   278,   276,   276,   279,   279,   279,   279,   280,
     281,   282,   282,   282,   283,   283,   283,   284,   284,   285,
     285,   285,   286,   287,   287,   288,   288,   289,   291,   292,
     290,   293,   294,   290,   295,   296,   290,   298,   299,   297,
     300,   300,   300,   301,   301,   302,   302,   302,   303,   303,
     303,   304,   304,   304,   304,   305,   305,   306,   306,   307,
     307,   308,   308,   308,   308,   308,   308,   308,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   310,   310,   310,
     310,   310,   310,   311,   311,   312,   312,   313,   313,   314,
     315,   316,   314,   317,   318,   314,   319,   319,   320,   321,
     319,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     323,   324,   322,   322,   322,   322,   325,   326,   322,   322,
     322,   327,   328,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   330,   330,   331,   331,   331,   332,   332,
     333,   333,   333,   334,   335,   336,   335,   337,   335,   338,
     335,   335,   339,   339,   339,   340,   340,   341,   341,   342,
     342,   343,   344,   344,   345,   345,   346,   346,   346,   346,
     346,   346,   347,   347,   347,   348,   348,   349,   349,   349,
     349,   349,   350,   350,   350,   350,   350,   351,   352,   351,
     353,   353,   354,   354,   354,   355,   356,   355,   357,   357,
     357,   357,   359,   358,   360,   360,   361,   361,   362,   362,
     362,   363,   364,   364,   365,   365,   366,   366,   367,   368,
     368,   369,   369,   369,   371,   372,   370,   373,   373,   373,
     373,   373,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   375,
     375,   375,   375,   375,   375,   376,   377,   377,   377,   378,
     378,   380,   381,   379,   382,   382,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   384,   385,   383,   383,   386,   387,
     383,   388,   389,   383,   390,   391,   383,   383,   392,   393,
     383,   394,   395,   383,   383,   396,   397,   383,   398,   399,
     383,   383,   400,   401,   383,   402,   403,   383,   404,   405,
     383,   406,   407,   383,   408,   408,   408,   410,   411,   409,
     413,   412,   414,   414,   414,   414,   415,   415,   415,   415,
     416,   416,   417,   417,   418,   418,   419,   419,   420,   420,
     421,   421,   421,   422,   422,   422,   423,   423,   423,   423,
     423,   423,   424,   424,   424,   425,   425,   426,   426,   427,
     427,   428,   428,   429,   430,   430,   431,   431
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     0,     1,     1,     1,     1,
       4,     1,     1,     2,     2,     3,     0,     2,     4,     3,
       1,     2,     0,     4,     2,     2,     1,     1,     1,     2,
       3,     3,     2,     4,     0,     1,     2,     1,     3,     1,
       3,     3,     3,     2,     1,     1,     0,     2,     4,     1,
       1,     0,     0,     3,     1,     1,     1,     1,     1,     4,
       0,     6,     6,     2,     3,     3,     0,     5,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     1,     5,     1,     3,     1,
       1,     1,     1,     4,     1,     2,     3,     3,     3,     3,
       2,     1,     3,     0,     3,     0,     2,     3,     0,     2,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     3,     2,     2,     3,     4,
       3,     2,     2,     2,     2,     2,     3,     3,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     0,     1,     1,     3,
       0,     4,     3,     7,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     2,     2,     2,     0,     2,     2,     3,     0,
       0,     7,     3,     2,     2,     2,     1,     3,     2,     2,
       3,     0,     0,     5,     1,     2,     4,     5,     2,     1,
       1,     1,     2,     3,     2,     2,     3,     2,     3,     2,
       2,     3,     4,     1,     1,     1,     0,     3,     0,     0,
       7,     0,     0,     7,     0,     0,     7,     0,     0,     6,
       5,     8,    10,     1,     3,     1,     2,     3,     1,     1,
       2,     2,     2,     2,     2,     1,     3,     0,     4,     1,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     4,     4,
       4,     4,     4,     6,     8,     5,     6,     1,     4,     3,
       0,     0,     8,     0,     0,     9,     3,     4,     0,     0,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     3,     4,     5,     4,     5,     3,     4,     1,     3,
       4,     3,     4,     2,     4,     4,     7,     8,     3,     5,
       0,     0,     8,     3,     3,     3,     0,     0,     8,     3,
       4,     0,     0,     9,     4,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     2,     4,     1,     4,     4,     4,
       4,     4,     1,     6,     7,     6,     6,     7,     7,     6,
       7,     6,     6,     0,     4,     0,     1,     1,     0,     1,
       0,     1,     1,     4,     0,     0,     4,     0,     9,     0,
       9,     5,     2,     3,     4,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     1,     1,     1,     2,     3,     5,
       3,     3,     1,     1,     1,     0,     1,     1,     4,     3,
       3,     5,     4,     6,     5,     5,     4,     0,     0,     4,
       0,     1,     0,     1,     1,     6,     0,     6,     0,     2,
       3,     5,     0,     4,     2,     3,     4,     2,     0,     1,
       1,     1,     7,     9,     0,     2,     0,     1,     3,     1,
       1,     0,     1,     1,     0,     0,     9,     1,     4,     3,
       3,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     1,
       3,     0,     0,     6,     1,     3,     1,     1,     1,     1,
       4,     3,     4,     2,     2,     3,     2,     3,     2,     2,
       3,     3,     3,     2,     0,     0,     6,     2,     0,     0,
       6,     0,     0,     6,     0,     0,     6,     1,     0,     0,
       6,     0,     0,     7,     1,     0,     0,     6,     0,     0,
       7,     1,     0,     0,     6,     0,     0,     7,     0,     0,
       6,     0,     0,     6,     1,     3,     3,     0,     0,     8,
       0,     7,     1,     1,     1,     1,     3,     3,     5,     5,
       1,     3,     0,     2,     2,     1,     2,     1,     2,     1,
       2,     1,     1,     2,     1,     1,     5,     4,     6,     7,
       5,     7,     1,     3,     3,     3,     1,     1,     3,     4,
       4,     1,     3,     3,     0,     3,    10,    10
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
  case 15: /* optional_public_or_private_module: %empty  */
                        { (yyval.b) = yyextra->g_Program->policies.default_module_public; }
    break;

  case 16: /* optional_public_or_private_module: "public"  */
                        { (yyval.b) = true; }
    break;

  case 17: /* optional_public_or_private_module: "private"  */
                        { (yyval.b) = false; }
    break;

  case 18: /* module_name: '$'  */
                    { (yyval.s) = new string("$"); }
    break;

  case 19: /* module_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 20: /* module_declaration: "module" module_name optional_shared optional_public_or_private_module  */
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

  case 21: /* character_sequence: STRING_CHARACTER  */
                                                            { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
    break;

  case 22: /* character_sequence: STRING_CHARACTER_ESC  */
                                                            { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
    break;

  case 23: /* character_sequence: character_sequence STRING_CHARACTER  */
                                                            { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
    break;

  case 24: /* character_sequence: character_sequence STRING_CHARACTER_ESC  */
                                                            { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
    break;

  case 25: /* string_constant: "start of the string" character_sequence "end of the string"  */
                                                           { (yyval.s) = (yyvsp[-1].s); }
    break;

  case 26: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 27: /* string_builder_body: string_builder_body character_sequence  */
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

  case 28: /* string_builder_body: string_builder_body "{" expr "}"  */
                                                                                {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 29: /* string_builder: "start of the string" string_builder_body "end of the string"  */
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

  case 30: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 31: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 32: /* $@1: %empty  */
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

  case 33: /* expr_reader: '%' name_in_namespace $@1 reader_character_sequence  */
                                     {
        yyextra->g_ReaderExpr->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = yyextra->g_ReaderExpr;
        delete (yyvsp[-2].s);
        yyextra->g_ReaderMacro = nullptr;
        yyextra->g_ReaderExpr = nullptr;
    }
    break;

  case 34: /* options_declaration: "options" annotation_argument_list  */
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

  case 36: /* keyword_or_name: "name"  */
                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 37: /* keyword_or_name: "keyword"  */
                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 38: /* require_module_name: keyword_or_name  */
                              {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 39: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 40: /* require_module_name: require_module_name '.' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 41: /* require_module_name: require_module_name '/' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 42: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 43: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 44: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 45: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 49: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 50: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 51: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 52: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 53: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 54: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 55: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 56: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 57: /* expression_else: "else" expression_block  */
                                                           { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 58: /* expression_else: elif_or_static_elif expr expression_block expression_else  */
                                                                                          {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 59: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 60: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 61: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 62: /* $@2: %empty  */
                      { yyextra->das_need_oxford_comma = true; }
    break;

  case 63: /* expression_else_one_liner: "else" $@2 expression_if_one_liner  */
                                                                                                 {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 64: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 65: /* expression_if_one_liner: expression_return_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 66: /* expression_if_one_liner: expression_yield_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 67: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 68: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 69: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 70: /* $@3: %empty  */
                                                     { yyextra->das_need_oxford_comma = true; }
    break;

  case 71: /* expression_if_then_else: expression_if_one_liner "if" $@3 expr expression_else_one_liner ';'  */
                                                                                                                                                   {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr(ast_wrapInBlock((yyvsp[-5].pExpression))),(yyvsp[-1].pExpression) ? ExpressionPtr(ast_wrapInBlock((yyvsp[-1].pExpression))) : nullptr);
    }
    break;

  case 72: /* expression_for_loop: "for" variable_name_with_pos_list "in" expr_list ';' expression_block  */
                                                                                                           {
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-4].pNameWithPosList),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 73: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 74: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 75: /* expression_with: "with" expr expression_block  */
                                                         {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
    break;

  case 76: /* $@4: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 77: /* expression_with_alias: "assume" "name" '=' $@4 expr  */
                                                                                               {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-3].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-3].s);
    }
    break;

  case 78: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 79: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 80: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 81: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 82: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 83: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 84: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 85: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 86: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 87: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 88: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 89: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 90: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 91: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 92: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 93: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 94: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 95: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 96: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 97: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 98: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 99: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 100: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 101: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 102: /* annotation_declaration_basic: annotation_declaration_name  */
                                          {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[0]));
        if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
    break;

  case 103: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 104: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 105: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 106: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 107: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
                                                                            {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation); }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Or,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 108: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 109: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 110: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 111: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 112: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 113: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 114: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 115: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 116: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 117: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 118: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 119: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 120: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 121: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 122: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 123: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 124: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 125: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 126: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 127: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 128: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 129: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 130: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 131: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 132: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 133: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 134: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 135: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 136: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 137: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 138: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 139: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 140: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 141: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 142: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 143: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 144: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 145: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 146: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 147: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 148: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 149: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 150: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 151: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 152: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 153: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 154: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 155: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 156: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 157: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 158: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 159: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 160: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 161: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 162: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 163: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 164: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 165: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 166: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 167: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 168: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 169: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 170: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 171: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 172: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 173: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 174: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 175: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 176: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 177: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 178: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 179: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 180: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 181: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 182: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 183: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 184: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 185: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 186: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 187: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 188: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 189: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 190: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 191: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 192: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 193: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 194: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 195: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 196: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 197: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 198: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 199: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 200: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 201: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 202: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 203: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 204: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 205: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 206: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 207: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 208: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 209: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 210: /* $@5: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 211: /* function_declaration: optional_public_or_private_function $@5 function_declaration_header expression_block  */
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

  case 212: /* expression_block: '{' expressions '}'  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 213: /* expression_block: '{' expressions '}' "finally" '{' expressions '}'  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 214: /* expression_any: ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 215: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 216: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 217: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 218: /* expression_any: expr_assign ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 219: /* expression_any: expression_delete ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 220: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 221: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 222: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 223: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 224: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 225: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 226: /* expression_any: expression_break ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 227: /* expression_any: expression_continue ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 228: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 229: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 230: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 231: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 232: /* expression_any: expression_label ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 233: /* expression_any: expression_goto ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 234: /* expression_any: "pass" ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 235: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 236: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 237: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 238: /* expr_keyword: "keyword" expr expression_block  */
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

  case 239: /* $@6: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 240: /* $@7: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 241: /* expression_keyword: "keyword" '<' $@6 type_declaration_no_options '>' $@7 expr  */
                                                                                                                                               {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),*(yyvsp[-6].s));
        auto td = new ExprTypeDecl(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCall->arguments.push_back(ExpressionPtr(td));
        pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 242: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 243: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 244: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 245: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 246: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 247: /* name_in_namespace: "name" "::" "name"  */
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

  case 248: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 249: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 250: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 251: /* $@8: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 252: /* $@9: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 253: /* new_type_declaration: '<' $@8 type_declaration '>' $@9  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 254: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 255: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 256: /* expr_new: "new" new_type_declaration '(' ')'  */
                                                               {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 257: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 258: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 259: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 260: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 261: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 262: /* expression_return_no_pipe: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 263: /* expression_return_no_pipe: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 264: /* expression_return: expression_return_no_pipe ';'  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 265: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 266: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 267: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 268: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 269: /* expression_yield: expression_yield_no_pipe ';'  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 270: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 271: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 272: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 273: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 274: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 275: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 276: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 277: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 278: /* $@10: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 279: /* $@11: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 280: /* expr_cast: "cast" '<' $@10 type_declaration_no_options '>' $@11 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 281: /* $@12: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 282: /* $@13: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 283: /* expr_cast: "upcast" '<' $@12 type_declaration_no_options '>' $@13 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 284: /* $@14: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 285: /* $@15: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 286: /* expr_cast: "reinterpret" '<' $@14 type_declaration_no_options '>' $@15 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 287: /* $@16: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 288: /* $@17: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 289: /* expr_type_decl: "type" '<' $@16 type_declaration '>' $@17  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 290: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 291: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 292: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
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

  case 293: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 294: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 295: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 296: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 297: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 298: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 299: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 300: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 301: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 302: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 303: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 304: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 305: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 306: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 307: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 308: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 309: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 310: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 311: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 312: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 313: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 314: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 315: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 316: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 317: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 318: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 319: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 320: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 321: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 322: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 323: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 324: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 325: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 326: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 327: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 328: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 329: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 330: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 331: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 332: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 333: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 334: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 335: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 336: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 337: /* expr_assign_pipe: expr '=' "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 338: /* expr_assign_pipe: expr '=' "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 339: /* expr_assign_pipe: expr '=' "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 340: /* expr_assign_pipe: expr "<-" "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 341: /* expr_assign_pipe: expr "<-" "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 342: /* expr_assign_pipe: expr "<-" "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 343: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 344: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 345: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 346: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 347: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 348: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 349: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 350: /* $@18: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 351: /* $@19: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 352: /* func_addr_expr: '@' '@' '<' $@18 type_declaration_no_options '>' $@19 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 353: /* $@20: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 354: /* $@21: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 355: /* func_addr_expr: '@' '@' '<' $@20 optional_function_argument_list optional_function_type '>' $@21 func_addr_name  */
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

  case 356: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 357: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 358: /* $@22: %empty  */
                               { yyextra->das_supress_errors=true; }
    break;

  case 359: /* $@23: %empty  */
                                                                           { yyextra->das_supress_errors=false; }
    break;

  case 360: /* expr_field: expr '.' $@22 error $@23  */
                                                                                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 361: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 362: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 363: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 364: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 365: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 366: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 367: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 368: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 369: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 370: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 371: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 372: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 392: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 393: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 394: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 395: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 396: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 397: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 398: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 399: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 400: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 401: /* expr: '(' expr ')'  */
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 402: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 403: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 404: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 405: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 406: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 407: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 408: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 409: /* expr: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
    break;

  case 410: /* expr: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
    break;

  case 411: /* expr: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 412: /* expr: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 413: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 414: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 415: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 416: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 417: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 418: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 419: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 420: /* $@24: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 421: /* $@25: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 422: /* expr: expr "is" "type" '<' $@24 type_declaration_no_options '>' $@25  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 423: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 424: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 425: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 426: /* $@26: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 427: /* $@27: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 428: /* expr: expr "as" "type" '<' $@26 type_declaration '>' $@27  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 429: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 430: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 431: /* $@28: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 432: /* $@29: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 433: /* expr: expr '?' "as" "type" '<' $@28 type_declaration '>' $@29  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 434: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 435: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 436: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 437: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 438: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 439: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 440: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 441: /* expr: expr_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 442: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 443: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 444: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 445: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 446: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 447: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 448: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 449: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 450: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 451: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 452: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 453: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 454: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 455: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 456: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 457: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 458: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 459: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 460: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 461: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 462: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 463: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 464: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 465: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 466: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 467: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 468: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 469: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 470: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 471: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 472: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 473: /* structure_variable_declaration: optional_field_annotation optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                 {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 474: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 475: /* $@30: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 476: /* struct_variable_declaration_list: struct_variable_declaration_list $@30 structure_variable_declaration ';'  */
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

  case 477: /* $@31: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 478: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@31 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 479: /* $@32: %empty  */
                                                                                                                               {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 480: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_override optional_constant $@32 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-6]),(yylsp[0])),tokAt(scanner,(yylsp[-7])));
            }
    break;

  case 481: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 482: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 483: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 484: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 485: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 486: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 487: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 488: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 489: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 490: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 491: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 492: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 493: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 494: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 495: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 496: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 497: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 498: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 499: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 500: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 501: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 502: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 503: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 504: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 505: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 506: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 507: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 508: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 509: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 510: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 511: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 512: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options ';'  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 513: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 514: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 515: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 516: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 517: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 518: /* $@33: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 519: /* global_variable_declaration_list: global_variable_declaration_list $@33 optional_field_annotation let_variable_declaration  */
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

  case 520: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 521: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 522: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 523: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 524: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 525: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 526: /* $@34: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 527: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@34 optional_field_annotation let_variable_declaration  */
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

  case 528: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 529: /* enum_list: enum_list ';'  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 530: /* enum_list: enum_list "name" ';'  */
                                     {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt(scanner,(yylsp[-1]))) ) {
            das_yyerror(scanner,"enumeration already declared " + *(yyvsp[-1].s), tokAt(scanner,(yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
    break;

  case 531: /* enum_list: enum_list "name" '=' expr ';'  */
                                                     {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt(scanner,(yylsp[-3]))) ) {
            das_yyerror(scanner,"enumeration value already declared " + *(yyvsp[-3].s), tokAt(scanner,(yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
    break;

  case 532: /* $@35: %empty  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 533: /* single_alias: "name" $@35 '=' type_declaration  */
                                  {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
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

  case 538: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 539: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 540: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 541: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 542: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name '{' enum_list '}'  */
                                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-3].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-6].faList),tokAt(scanner,(yylsp[-6])),(yyvsp[-4].b),(yyvsp[-3].pEnum),(yyvsp[-1].pEnum),Type::tInt);
    }
    break;

  case 543: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration '{' enum_list '}'  */
                                                                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-1].pEnum),(yyvsp[-3].type));
    }
    break;

  case 544: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 545: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 546: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 547: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 548: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 549: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 550: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 551: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 552: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 553: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 554: /* $@36: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 555: /* $@37: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 556: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@36 structure_name $@37 '{' struct_variable_declaration_list '}'  */
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

  case 557: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 558: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 559: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 560: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 561: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 562: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 563: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 564: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 565: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 566: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 567: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 568: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 569: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 570: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 571: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 572: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 573: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 574: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 575: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 576: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 577: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 578: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 579: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 580: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 581: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 582: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 583: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 584: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 585: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 586: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 587: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 588: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 589: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 590: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 591: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 592: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 593: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 594: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 595: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 596: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 597: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 598: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 599: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 600: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 601: /* $@38: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 602: /* $@39: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 603: /* bitfield_type_declaration: "bitfield" '<' $@38 bitfield_bits '>' $@39  */
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

  case 604: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 605: /* table_type_pair: type_declaration ';' type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 606: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 607: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 608: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 609: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 610: /* type_declaration_no_options: type_declaration_no_options '[' expr ']'  */
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

  case 611: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 612: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 613: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 614: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 615: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 616: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 617: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 618: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 619: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 620: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 621: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 622: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 623: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 624: /* $@40: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 625: /* $@41: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 626: /* type_declaration_no_options: "smart_ptr" '<' $@40 type_declaration '>' $@41  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 627: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 628: /* $@42: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 629: /* $@43: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 630: /* type_declaration_no_options: "array" '<' $@42 type_declaration '>' $@43  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 631: /* $@44: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 632: /* $@45: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 633: /* type_declaration_no_options: "table" '<' $@44 table_type_pair '>' $@45  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 634: /* $@46: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 635: /* $@47: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 636: /* type_declaration_no_options: "iterator" '<' $@46 type_declaration '>' $@47  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 637: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 638: /* $@48: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 639: /* $@49: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 640: /* type_declaration_no_options: "block" '<' $@48 type_declaration '>' $@49  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 641: /* $@50: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 642: /* $@51: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 643: /* type_declaration_no_options: "block" '<' $@50 optional_function_argument_list optional_function_type '>' $@51  */
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

  case 644: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 645: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 646: /* $@53: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 647: /* type_declaration_no_options: "function" '<' $@52 type_declaration '>' $@53  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 648: /* $@54: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 649: /* $@55: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 650: /* type_declaration_no_options: "function" '<' $@54 optional_function_argument_list optional_function_type '>' $@55  */
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

  case 651: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 652: /* $@56: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 653: /* $@57: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 654: /* type_declaration_no_options: "lambda" '<' $@56 type_declaration '>' $@57  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 655: /* $@58: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 656: /* $@59: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 657: /* type_declaration_no_options: "lambda" '<' $@58 optional_function_argument_list optional_function_type '>' $@59  */
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

  case 658: /* $@60: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 659: /* $@61: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 660: /* type_declaration_no_options: "tuple" '<' $@60 tuple_type_list '>' $@61  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 661: /* $@62: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 662: /* $@63: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 663: /* type_declaration_no_options: "variant" '<' $@62 variant_type_list '>' $@63  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 664: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 665: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 666: /* type_declaration: type_declaration '|' '#'  */
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

  case 667: /* $@64: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 668: /* $@65: %empty  */
                                                                           {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 669: /* variant_alias_declaration: "variant" $@64 "name" $@65 '{' variant_type_list ';' '}'  */
                                          {
        auto vtype = make_smart<TypeDecl>(Type::tVariant);
        vtype->alias = *(yyvsp[-5].s);
        vtype->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, vtype.get(), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-5].s),tokAt(scanner,(yylsp[-5])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariant((yyvsp[-5].s)->c_str(),atvname);
        }
        delete (yyvsp[-5].s);
    }
    break;

  case 670: /* $@66: %empty  */
                      { yyextra->das_need_oxford_comma=false; }
    break;

  case 671: /* bitfield_alias_declaration: "bitfield" $@66 "name" '{' bitfield_bits ';' '}'  */
                                                                                                            {
        auto btype = make_smart<TypeDecl>(Type::tBitfield);
        btype->alias = *(yyvsp[-4].s);
        btype->at = tokAt(scanner,(yylsp[-4]));
        btype->argNames = *(yyvsp[-2].pNameList);
        if ( btype->argNames.size()>32 ) {
            das_yyerror(scanner,"only 32 different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-4])),
                CompilationError::invalid_type);
        }
        if ( !yyextra->g_Program->addAlias(btype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-4].s),tokAt(scanner,(yylsp[-4])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-4].s);
        delete (yyvsp[-2].pNameList);
    }
    break;

  case 672: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 673: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 674: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 675: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 676: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 677: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 678: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 679: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 680: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 681: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 682: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 683: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 696: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 697: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 698: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 699: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 700: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 701: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 702: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 703: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 704: /* make_tuple: make_tuple ',' expr  */
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

  case 705: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 706: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 707: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 708: /* make_dim: make_dim ';' make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 709: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
       ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
       (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
       (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 710: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
       ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
       (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
       auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
       (yyval.pExpression) = tam;
    }
    break;

  case 711: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 712: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 713: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 714: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 715: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 716: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 717: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
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
    if ( !yyextra->das_supress_errors ) {
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


