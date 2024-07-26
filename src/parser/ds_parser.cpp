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
  YYSYMBOL_DAS_TBOOL = 68,                 /* "bool"  */
  YYSYMBOL_DAS_TVOID = 69,                 /* "void"  */
  YYSYMBOL_DAS_TSTRING = 70,               /* "string"  */
  YYSYMBOL_DAS_TAUTO = 71,                 /* "auto"  */
  YYSYMBOL_DAS_TINT = 72,                  /* "int"  */
  YYSYMBOL_DAS_TINT2 = 73,                 /* "int2"  */
  YYSYMBOL_DAS_TINT3 = 74,                 /* "int3"  */
  YYSYMBOL_DAS_TINT4 = 75,                 /* "int4"  */
  YYSYMBOL_DAS_TUINT = 76,                 /* "uint"  */
  YYSYMBOL_DAS_TBITFIELD = 77,             /* "bitfield"  */
  YYSYMBOL_DAS_TUINT2 = 78,                /* "uint2"  */
  YYSYMBOL_DAS_TUINT3 = 79,                /* "uint3"  */
  YYSYMBOL_DAS_TUINT4 = 80,                /* "uint4"  */
  YYSYMBOL_DAS_TFLOAT = 81,                /* "float"  */
  YYSYMBOL_DAS_TFLOAT2 = 82,               /* "float2"  */
  YYSYMBOL_DAS_TFLOAT3 = 83,               /* "float3"  */
  YYSYMBOL_DAS_TFLOAT4 = 84,               /* "float4"  */
  YYSYMBOL_DAS_TRANGE = 85,                /* "range"  */
  YYSYMBOL_DAS_TURANGE = 86,               /* "urange"  */
  YYSYMBOL_DAS_TRANGE64 = 87,              /* "range64"  */
  YYSYMBOL_DAS_TURANGE64 = 88,             /* "urange64"  */
  YYSYMBOL_DAS_TBLOCK = 89,                /* "block"  */
  YYSYMBOL_DAS_TINT64 = 90,                /* "int64"  */
  YYSYMBOL_DAS_TUINT64 = 91,               /* "uint64"  */
  YYSYMBOL_DAS_TDOUBLE = 92,               /* "double"  */
  YYSYMBOL_DAS_TFUNCTION = 93,             /* "function"  */
  YYSYMBOL_DAS_TLAMBDA = 94,               /* "lambda"  */
  YYSYMBOL_DAS_TINT8 = 95,                 /* "int8"  */
  YYSYMBOL_DAS_TUINT8 = 96,                /* "uint8"  */
  YYSYMBOL_DAS_TINT16 = 97,                /* "int16"  */
  YYSYMBOL_DAS_TUINT16 = 98,               /* "uint16"  */
  YYSYMBOL_DAS_TTUPLE = 99,                /* "tuple"  */
  YYSYMBOL_DAS_TVARIANT = 100,             /* "variant"  */
  YYSYMBOL_DAS_GENERATOR = 101,            /* "generator"  */
  YYSYMBOL_DAS_YIELD = 102,                /* "yield"  */
  YYSYMBOL_DAS_SEALED = 103,               /* "sealed"  */
  YYSYMBOL_ADDEQU = 104,                   /* "+="  */
  YYSYMBOL_SUBEQU = 105,                   /* "-="  */
  YYSYMBOL_DIVEQU = 106,                   /* "/="  */
  YYSYMBOL_MULEQU = 107,                   /* "*="  */
  YYSYMBOL_MODEQU = 108,                   /* "%="  */
  YYSYMBOL_ANDEQU = 109,                   /* "&="  */
  YYSYMBOL_OREQU = 110,                    /* "|="  */
  YYSYMBOL_XOREQU = 111,                   /* "^="  */
  YYSYMBOL_SHL = 112,                      /* "<<"  */
  YYSYMBOL_SHR = 113,                      /* ">>"  */
  YYSYMBOL_ADDADD = 114,                   /* "++"  */
  YYSYMBOL_SUBSUB = 115,                   /* "--"  */
  YYSYMBOL_LEEQU = 116,                    /* "<="  */
  YYSYMBOL_SHLEQU = 117,                   /* "<<="  */
  YYSYMBOL_SHREQU = 118,                   /* ">>="  */
  YYSYMBOL_GREQU = 119,                    /* ">="  */
  YYSYMBOL_EQUEQU = 120,                   /* "=="  */
  YYSYMBOL_NOTEQU = 121,                   /* "!="  */
  YYSYMBOL_RARROW = 122,                   /* "->"  */
  YYSYMBOL_LARROW = 123,                   /* "<-"  */
  YYSYMBOL_QQ = 124,                       /* "??"  */
  YYSYMBOL_QDOT = 125,                     /* "?."  */
  YYSYMBOL_QBRA = 126,                     /* "?["  */
  YYSYMBOL_LPIPE = 127,                    /* "<|"  */
  YYSYMBOL_LBPIPE = 128,                   /* " <|"  */
  YYSYMBOL_LLPIPE = 129,                   /* "$ <|"  */
  YYSYMBOL_LAPIPE = 130,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 131,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 132,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 133,                 /* ":="  */
  YYSYMBOL_ROTL = 134,                     /* "<<<"  */
  YYSYMBOL_ROTR = 135,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 136,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 137,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 138,                    /* "=>"  */
  YYSYMBOL_COLCOL = 139,                   /* "::"  */
  YYSYMBOL_ANDAND = 140,                   /* "&&"  */
  YYSYMBOL_OROR = 141,                     /* "||"  */
  YYSYMBOL_XORXOR = 142,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 143,                /* "&&="  */
  YYSYMBOL_OROREQU = 144,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 145,                /* "^^="  */
  YYSYMBOL_DOTDOT = 146,                   /* ".."  */
  YYSYMBOL_MTAG_E = 147,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 148,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 149,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 150,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 151,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 152,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 153,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 154,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 155,           /* "..."  */
  YYSYMBOL_BRABRAB = 156,                  /* "[["  */
  YYSYMBOL_BRACBRB = 157,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 158,                  /* "{{"  */
  YYSYMBOL_INTEGER = 159,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 160,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 161,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 162,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 163,            /* "unsigned int8 constant"  */
  YYSYMBOL_FLOAT = 164,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 165,                   /* "double constant"  */
  YYSYMBOL_NAME = 166,                     /* "name"  */
  YYSYMBOL_KEYWORD = 167,                  /* "keyword"  */
  YYSYMBOL_TYPE_FUNCTION = 168,            /* "type function"  */
  YYSYMBOL_BEGIN_STRING = 169,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 170,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 171,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 172,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 173,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 174,          /* "}"  */
  YYSYMBOL_END_OF_READ = 175,              /* "end of failed eader macro"  */
  YYSYMBOL_SEMICOLON_CUR_CUR = 176,        /* ";}}"  */
  YYSYMBOL_SEMICOLON_CUR_SQR = 177,        /* ";}]"  */
  YYSYMBOL_SEMICOLON_SQR_SQR = 178,        /* ";]]"  */
  YYSYMBOL_COMMA_SQR_SQR = 179,            /* ",]]"  */
  YYSYMBOL_COMMA_CUR_SQR = 180,            /* ",}]"  */
  YYSYMBOL_181_ = 181,                     /* ','  */
  YYSYMBOL_182_ = 182,                     /* '='  */
  YYSYMBOL_183_ = 183,                     /* '?'  */
  YYSYMBOL_184_ = 184,                     /* ':'  */
  YYSYMBOL_185_ = 185,                     /* '|'  */
  YYSYMBOL_186_ = 186,                     /* '^'  */
  YYSYMBOL_187_ = 187,                     /* '&'  */
  YYSYMBOL_188_ = 188,                     /* '<'  */
  YYSYMBOL_189_ = 189,                     /* '>'  */
  YYSYMBOL_190_ = 190,                     /* '-'  */
  YYSYMBOL_191_ = 191,                     /* '+'  */
  YYSYMBOL_192_ = 192,                     /* '*'  */
  YYSYMBOL_193_ = 193,                     /* '/'  */
  YYSYMBOL_194_ = 194,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 195,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 196,               /* UNARY_PLUS  */
  YYSYMBOL_197_ = 197,                     /* '~'  */
  YYSYMBOL_198_ = 198,                     /* '!'  */
  YYSYMBOL_PRE_INC = 199,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 200,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 201,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 202,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 203,                    /* DEREF  */
  YYSYMBOL_204_ = 204,                     /* '.'  */
  YYSYMBOL_205_ = 205,                     /* '['  */
  YYSYMBOL_206_ = 206,                     /* ']'  */
  YYSYMBOL_207_ = 207,                     /* '('  */
  YYSYMBOL_208_ = 208,                     /* ')'  */
  YYSYMBOL_209_ = 209,                     /* '$'  */
  YYSYMBOL_210_ = 210,                     /* '@'  */
  YYSYMBOL_211_ = 211,                     /* ';'  */
  YYSYMBOL_212_ = 212,                     /* '{'  */
  YYSYMBOL_213_ = 213,                     /* '}'  */
  YYSYMBOL_214_ = 214,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 215,                 /* $accept  */
  YYSYMBOL_program = 216,                  /* program  */
  YYSYMBOL_top_level_reader_macro = 217,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 218, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 219,              /* module_name  */
  YYSYMBOL_module_declaration = 220,       /* module_declaration  */
  YYSYMBOL_character_sequence = 221,       /* character_sequence  */
  YYSYMBOL_string_constant = 222,          /* string_constant  */
  YYSYMBOL_string_builder_body = 223,      /* string_builder_body  */
  YYSYMBOL_string_builder = 224,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 225, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 226,              /* expr_reader  */
  YYSYMBOL_227_1 = 227,                    /* $@1  */
  YYSYMBOL_options_declaration = 228,      /* options_declaration  */
  YYSYMBOL_require_declaration = 229,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 230,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 231,      /* require_module_name  */
  YYSYMBOL_require_module = 232,           /* require_module  */
  YYSYMBOL_is_public_module = 233,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 234,       /* expect_declaration  */
  YYSYMBOL_expect_list = 235,              /* expect_list  */
  YYSYMBOL_expect_error = 236,             /* expect_error  */
  YYSYMBOL_expression_label = 237,         /* expression_label  */
  YYSYMBOL_expression_goto = 238,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 239,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 240,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 241,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 242, /* expression_else_one_liner  */
  YYSYMBOL_243_2 = 243,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 244,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 245,  /* expression_if_then_else  */
  YYSYMBOL_246_3 = 246,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 247,      /* expression_for_loop  */
  YYSYMBOL_248_4 = 248,                    /* $@4  */
  YYSYMBOL_expression_unsafe = 249,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 250,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 251,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 252,    /* expression_with_alias  */
  YYSYMBOL_253_5 = 253,                    /* $@5  */
  YYSYMBOL_annotation_argument_value = 254, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 255, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 256, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 257,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 258, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 259, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 260, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 261,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 262,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 263, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 264, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 265,   /* optional_function_type  */
  YYSYMBOL_function_name = 266,            /* function_name  */
  YYSYMBOL_global_function_declaration = 267, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 268, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 269, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 270,     /* function_declaration  */
  YYSYMBOL_271_6 = 271,                    /* $@6  */
  YYSYMBOL_expression_block = 272,         /* expression_block  */
  YYSYMBOL_expression_any = 273,           /* expression_any  */
  YYSYMBOL_expressions = 274,              /* expressions  */
  YYSYMBOL_expr_keyword = 275,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 276,       /* optional_expr_list  */
  YYSYMBOL_expression_keyword = 277,       /* expression_keyword  */
  YYSYMBOL_278_7 = 278,                    /* $@7  */
  YYSYMBOL_279_8 = 279,                    /* $@8  */
  YYSYMBOL_280_9 = 280,                    /* $@9  */
  YYSYMBOL_281_10 = 281,                   /* $@10  */
  YYSYMBOL_expr_pipe = 282,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 283,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 284,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 285,     /* new_type_declaration  */
  YYSYMBOL_286_11 = 286,                   /* $@11  */
  YYSYMBOL_287_12 = 287,                   /* $@12  */
  YYSYMBOL_expr_new = 288,                 /* expr_new  */
  YYSYMBOL_expression_break = 289,         /* expression_break  */
  YYSYMBOL_expression_continue = 290,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 291, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 292,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 293, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 294,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 295,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 296,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 297,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 298,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 299, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 300,           /* expression_let  */
  YYSYMBOL_expr_cast = 301,                /* expr_cast  */
  YYSYMBOL_302_13 = 302,                   /* $@13  */
  YYSYMBOL_303_14 = 303,                   /* $@14  */
  YYSYMBOL_304_15 = 304,                   /* $@15  */
  YYSYMBOL_305_16 = 305,                   /* $@16  */
  YYSYMBOL_306_17 = 306,                   /* $@17  */
  YYSYMBOL_307_18 = 307,                   /* $@18  */
  YYSYMBOL_expr_type_decl = 308,           /* expr_type_decl  */
  YYSYMBOL_309_19 = 309,                   /* $@19  */
  YYSYMBOL_310_20 = 310,                   /* $@20  */
  YYSYMBOL_expr_type_info = 311,           /* expr_type_info  */
  YYSYMBOL_expr_list = 312,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 313,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 314,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 315,            /* capture_entry  */
  YYSYMBOL_capture_list = 316,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 317,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 318,               /* expr_block  */
  YYSYMBOL_expr_full_block = 319,          /* expr_full_block  */
  YYSYMBOL_expr_numeric_const = 320,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 321,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 322,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 323,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 324,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 325,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 326,           /* func_addr_expr  */
  YYSYMBOL_327_21 = 327,                   /* $@21  */
  YYSYMBOL_328_22 = 328,                   /* $@22  */
  YYSYMBOL_329_23 = 329,                   /* $@23  */
  YYSYMBOL_330_24 = 330,                   /* $@24  */
  YYSYMBOL_expr_field = 331,               /* expr_field  */
  YYSYMBOL_332_25 = 332,                   /* $@25  */
  YYSYMBOL_333_26 = 333,                   /* $@26  */
  YYSYMBOL_expr = 334,                     /* expr  */
  YYSYMBOL_335_27 = 335,                   /* $@27  */
  YYSYMBOL_336_28 = 336,                   /* $@28  */
  YYSYMBOL_337_29 = 337,                   /* $@29  */
  YYSYMBOL_338_30 = 338,                   /* $@30  */
  YYSYMBOL_339_31 = 339,                   /* $@31  */
  YYSYMBOL_340_32 = 340,                   /* $@32  */
  YYSYMBOL_expr_mtag = 341,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 342, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 343,        /* optional_override  */
  YYSYMBOL_optional_constant = 344,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 345, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 346, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 347, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 348, /* struct_variable_declaration_list  */
  YYSYMBOL_349_33 = 349,                   /* $@33  */
  YYSYMBOL_350_34 = 350,                   /* $@34  */
  YYSYMBOL_351_35 = 351,                   /* $@35  */
  YYSYMBOL_function_argument_declaration = 352, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 353,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 354,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 355,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 356,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 357,             /* variant_type  */
  YYSYMBOL_variant_type_list = 358,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 359,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 360,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 361,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 362,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 363,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 364, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 365, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 366, /* global_variable_declaration_list  */
  YYSYMBOL_367_36 = 367,                   /* $@36  */
  YYSYMBOL_optional_shared = 368,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 369, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 370,               /* global_let  */
  YYSYMBOL_371_37 = 371,                   /* $@37  */
  YYSYMBOL_enum_list = 372,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 373, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 374,             /* single_alias  */
  YYSYMBOL_375_38 = 375,                   /* $@38  */
  YYSYMBOL_alias_list = 376,               /* alias_list  */
  YYSYMBOL_alias_declaration = 377,        /* alias_declaration  */
  YYSYMBOL_378_39 = 378,                   /* $@39  */
  YYSYMBOL_optional_public_or_private_enum = 379, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 380,                /* enum_name  */
  YYSYMBOL_enum_declaration = 381,         /* enum_declaration  */
  YYSYMBOL_382_40 = 382,                   /* $@40  */
  YYSYMBOL_383_41 = 383,                   /* $@41  */
  YYSYMBOL_384_42 = 384,                   /* $@42  */
  YYSYMBOL_385_43 = 385,                   /* $@43  */
  YYSYMBOL_optional_structure_parent = 386, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 387,          /* optional_sealed  */
  YYSYMBOL_structure_name = 388,           /* structure_name  */
  YYSYMBOL_class_or_struct = 389,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 390, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 391, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 392,    /* structure_declaration  */
  YYSYMBOL_393_44 = 393,                   /* $@44  */
  YYSYMBOL_394_45 = 394,                   /* $@45  */
  YYSYMBOL_variable_name_with_pos_list = 395, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 396,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 397, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 398, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 399,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 400,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 401,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 402, /* bitfield_type_declaration  */
  YYSYMBOL_403_46 = 403,                   /* $@46  */
  YYSYMBOL_404_47 = 404,                   /* $@47  */
  YYSYMBOL_table_type_pair = 405,          /* table_type_pair  */
  YYSYMBOL_type_declaration_no_options = 406, /* type_declaration_no_options  */
  YYSYMBOL_407_48 = 407,                   /* $@48  */
  YYSYMBOL_408_49 = 408,                   /* $@49  */
  YYSYMBOL_409_50 = 409,                   /* $@50  */
  YYSYMBOL_410_51 = 410,                   /* $@51  */
  YYSYMBOL_411_52 = 411,                   /* $@52  */
  YYSYMBOL_412_53 = 412,                   /* $@53  */
  YYSYMBOL_413_54 = 413,                   /* $@54  */
  YYSYMBOL_414_55 = 414,                   /* $@55  */
  YYSYMBOL_415_56 = 415,                   /* $@56  */
  YYSYMBOL_416_57 = 416,                   /* $@57  */
  YYSYMBOL_417_58 = 417,                   /* $@58  */
  YYSYMBOL_418_59 = 418,                   /* $@59  */
  YYSYMBOL_419_60 = 419,                   /* $@60  */
  YYSYMBOL_420_61 = 420,                   /* $@61  */
  YYSYMBOL_421_62 = 421,                   /* $@62  */
  YYSYMBOL_422_63 = 422,                   /* $@63  */
  YYSYMBOL_423_64 = 423,                   /* $@64  */
  YYSYMBOL_424_65 = 424,                   /* $@65  */
  YYSYMBOL_425_66 = 425,                   /* $@66  */
  YYSYMBOL_426_67 = 426,                   /* $@67  */
  YYSYMBOL_427_68 = 427,                   /* $@68  */
  YYSYMBOL_428_69 = 428,                   /* $@69  */
  YYSYMBOL_429_70 = 429,                   /* $@70  */
  YYSYMBOL_430_71 = 430,                   /* $@71  */
  YYSYMBOL_431_72 = 431,                   /* $@72  */
  YYSYMBOL_432_73 = 432,                   /* $@73  */
  YYSYMBOL_type_declaration = 433,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 434,  /* tuple_alias_declaration  */
  YYSYMBOL_435_74 = 435,                   /* $@74  */
  YYSYMBOL_436_75 = 436,                   /* $@75  */
  YYSYMBOL_437_76 = 437,                   /* $@76  */
  YYSYMBOL_438_77 = 438,                   /* $@77  */
  YYSYMBOL_variant_alias_declaration = 439, /* variant_alias_declaration  */
  YYSYMBOL_440_78 = 440,                   /* $@78  */
  YYSYMBOL_441_79 = 441,                   /* $@79  */
  YYSYMBOL_442_80 = 442,                   /* $@80  */
  YYSYMBOL_443_81 = 443,                   /* $@81  */
  YYSYMBOL_bitfield_alias_declaration = 444, /* bitfield_alias_declaration  */
  YYSYMBOL_445_82 = 445,                   /* $@82  */
  YYSYMBOL_446_83 = 446,                   /* $@83  */
  YYSYMBOL_447_84 = 447,                   /* $@84  */
  YYSYMBOL_448_85 = 448,                   /* $@85  */
  YYSYMBOL_make_decl = 449,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 450,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 451,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 452,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 453, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 454, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 455, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 456, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 457, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_make_struct_decl = 458,         /* make_struct_decl  */
  YYSYMBOL_459_86 = 459,                   /* $@86  */
  YYSYMBOL_460_87 = 460,                   /* $@87  */
  YYSYMBOL_461_88 = 461,                   /* $@88  */
  YYSYMBOL_462_89 = 462,                   /* $@89  */
  YYSYMBOL_463_90 = 463,                   /* $@90  */
  YYSYMBOL_464_91 = 464,                   /* $@91  */
  YYSYMBOL_make_tuple = 465,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 466,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 467,          /* make_tuple_call  */
  YYSYMBOL_make_dim = 468,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 469,            /* make_dim_decl  */
  YYSYMBOL_470_92 = 470,                   /* $@92  */
  YYSYMBOL_471_93 = 471,                   /* $@93  */
  YYSYMBOL_472_94 = 472,                   /* $@94  */
  YYSYMBOL_473_95 = 473,                   /* $@95  */
  YYSYMBOL_make_table = 474,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 475,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 476,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 477, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 478,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 479       /* array_comprehension  */
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
#define YYLAST   13780

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  215
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  265
/* YYNRULES -- Number of rules.  */
#define YYNRULES  805
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1452

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   442


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
       2,     2,     2,   198,     2,   214,   209,   194,   187,     2,
     207,   208,   192,   191,   181,   190,   204,   193,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   184,   211,
     188,   182,   189,   183,   210,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   205,     2,   206,   186,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   212,   185,   213,   197,     2,     2,     2,
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
     175,   176,   177,   178,   179,   180,   195,   196,   199,   200,
     201,   202,   203
};

#if DAS_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   516,   516,   517,   522,   523,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,   534,   538,   544,   545,
     546,   550,   551,   555,   573,   574,   575,   576,   580,   584,
     589,   598,   606,   622,   627,   635,   635,   674,   701,   705,
     706,   707,   711,   714,   718,   724,   733,   736,   742,   743,
     747,   751,   752,   756,   759,   765,   771,   774,   780,   781,
     785,   786,   787,   796,   797,   801,   802,   802,   808,   809,
     810,   811,   812,   816,   822,   822,   828,   828,   834,   842,
     852,   861,   861,   868,   869,   870,   871,   872,   873,   877,
     882,   890,   891,   892,   896,   897,   898,   899,   900,   901,
     902,   903,   909,   912,   918,   919,   920,   924,   932,   945,
     948,   956,   967,   978,   989,   992,   999,  1003,  1010,  1011,
    1015,  1016,  1017,  1021,  1024,  1031,  1035,  1036,  1037,  1038,
    1039,  1040,  1041,  1042,  1043,  1044,  1045,  1046,  1047,  1048,
    1049,  1050,  1051,  1052,  1053,  1054,  1055,  1056,  1057,  1058,
    1059,  1060,  1061,  1062,  1063,  1064,  1065,  1066,  1067,  1068,
    1069,  1070,  1071,  1072,  1073,  1074,  1075,  1076,  1077,  1078,
    1079,  1080,  1081,  1082,  1083,  1084,  1085,  1086,  1087,  1088,
    1089,  1090,  1091,  1092,  1093,  1094,  1095,  1096,  1097,  1098,
    1099,  1100,  1101,  1102,  1103,  1104,  1105,  1106,  1107,  1108,
    1109,  1110,  1111,  1112,  1113,  1114,  1115,  1116,  1117,  1118,
    1123,  1145,  1146,  1147,  1151,  1157,  1157,  1174,  1178,  1189,
    1190,  1191,  1192,  1193,  1194,  1195,  1196,  1197,  1198,  1199,
    1200,  1201,  1202,  1203,  1204,  1205,  1206,  1207,  1208,  1209,
    1213,  1218,  1224,  1230,  1241,  1242,  1246,  1246,  1246,  1254,
    1254,  1254,  1267,  1301,  1304,  1307,  1313,  1314,  1325,  1329,
    1332,  1340,  1340,  1340,  1343,  1349,  1352,  1355,  1359,  1365,
    1369,  1373,  1376,  1379,  1387,  1390,  1393,  1401,  1404,  1412,
    1415,  1418,  1426,  1432,  1433,  1437,  1438,  1442,  1447,  1455,
    1461,  1473,  1476,  1482,  1482,  1482,  1485,  1485,  1485,  1490,
    1490,  1490,  1498,  1498,  1498,  1504,  1514,  1525,  1540,  1543,
    1549,  1550,  1557,  1568,  1569,  1570,  1574,  1575,  1576,  1577,
    1581,  1586,  1594,  1595,  1599,  1604,  1611,  1618,  1619,  1620,
    1621,  1622,  1623,  1624,  1628,  1629,  1630,  1631,  1632,  1633,
    1634,  1635,  1636,  1637,  1638,  1639,  1640,  1641,  1642,  1643,
    1644,  1645,  1646,  1650,  1651,  1652,  1653,  1654,  1655,  1659,
    1666,  1678,  1683,  1693,  1697,  1704,  1707,  1707,  1707,  1712,
    1712,  1712,  1725,  1729,  1733,  1739,  1746,  1746,  1746,  1753,
    1754,  1755,  1756,  1757,  1758,  1759,  1760,  1761,  1762,  1763,
    1764,  1765,  1766,  1767,  1768,  1769,  1770,  1771,  1772,  1773,
    1774,  1775,  1776,  1777,  1778,  1779,  1780,  1781,  1782,  1783,
    1784,  1785,  1786,  1787,  1788,  1794,  1795,  1796,  1797,  1798,
    1799,  1800,  1801,  1802,  1803,  1804,  1805,  1806,  1810,  1814,
    1817,  1820,  1821,  1822,  1823,  1826,  1829,  1830,  1833,  1833,
    1833,  1836,  1841,  1845,  1849,  1849,  1849,  1854,  1857,  1861,
    1861,  1861,  1866,  1869,  1870,  1871,  1872,  1873,  1874,  1875,
    1876,  1877,  1879,  1883,  1884,  1889,  1893,  1894,  1895,  1896,
    1897,  1898,  1899,  1903,  1907,  1911,  1915,  1919,  1923,  1927,
    1931,  1935,  1942,  1943,  1947,  1948,  1949,  1953,  1954,  1958,
    1959,  1960,  1964,  1965,  1969,  1980,  1983,  1983,  2002,  2001,
    2016,  2015,  2028,  2037,  2043,  2048,  2058,  2059,  2063,  2066,
    2075,  2076,  2080,  2083,  2086,  2102,  2111,  2112,  2116,  2119,
    2122,  2136,  2137,  2141,  2147,  2153,  2156,  2160,  2166,  2175,
    2176,  2177,  2181,  2182,  2186,  2193,  2198,  2207,  2213,  2224,
    2227,  2232,  2237,  2245,  2256,  2259,  2259,  2279,  2280,  2284,
    2285,  2286,  2290,  2293,  2293,  2312,  2315,  2318,  2333,  2352,
    2353,  2354,  2359,  2359,  2381,  2382,  2386,  2387,  2387,  2391,
    2392,  2393,  2397,  2407,  2412,  2407,  2424,  2429,  2424,  2444,
    2445,  2449,  2450,  2454,  2460,  2461,  2465,  2466,  2467,  2471,
    2474,  2480,  2485,  2480,  2499,  2506,  2511,  2520,  2526,  2537,
    2538,  2539,  2540,  2541,  2542,  2543,  2544,  2545,  2546,  2547,
    2548,  2549,  2550,  2551,  2552,  2553,  2554,  2555,  2556,  2557,
    2558,  2559,  2560,  2561,  2562,  2563,  2567,  2568,  2569,  2570,
    2571,  2572,  2576,  2587,  2591,  2598,  2610,  2617,  2626,  2631,
    2634,  2647,  2647,  2647,  2660,  2664,  2671,  2672,  2673,  2674,
    2675,  2696,  2702,  2702,  2702,  2706,  2711,  2718,  2722,  2726,
    2731,  2736,  2741,  2746,  2750,  2754,  2759,  2763,  2767,  2772,
    2772,  2772,  2778,  2785,  2785,  2785,  2790,  2790,  2790,  2796,
    2796,  2796,  2801,  2805,  2805,  2805,  2810,  2810,  2810,  2819,
    2823,  2823,  2823,  2828,  2828,  2828,  2837,  2841,  2841,  2841,
    2846,  2846,  2846,  2855,  2855,  2855,  2861,  2861,  2861,  2870,
    2873,  2884,  2900,  2900,  2905,  2910,  2900,  2935,  2935,  2940,
    2946,  2935,  2971,  2971,  2976,  2981,  2971,  3011,  3012,  3013,
    3014,  3015,  3019,  3026,  3033,  3039,  3045,  3052,  3059,  3065,
    3075,  3080,  3087,  3088,  3093,  3094,  3098,  3099,  3103,  3104,
    3108,  3109,  3110,  3114,  3115,  3116,  3121,  3127,  3134,  3142,
    3149,  3157,  3166,  3166,  3166,  3173,  3173,  3173,  3180,  3180,
    3180,  3190,  3193,  3199,  3213,  3219,  3225,  3234,  3239,  3246,
    3254,  3259,  3266,  3274,  3274,  3274,  3282,  3289,  3289,  3289,
    3299,  3304,  3311,  3314,  3320,  3328,  3337,  3345,  3353,  3366,
    3367,  3371,  3372,  3377,  3380,  3383
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
  "\"inscope\"", "\"static\"", "\"fixed_array\"", "\"bool\"", "\"void\"",
  "\"string\"", "\"auto\"", "\"int\"", "\"int2\"", "\"int3\"", "\"int4\"",
  "\"uint\"", "\"bitfield\"", "\"uint2\"", "\"uint3\"", "\"uint4\"",
  "\"float\"", "\"float2\"", "\"float3\"", "\"float4\"", "\"range\"",
  "\"urange\"", "\"range64\"", "\"urange64\"", "\"block\"", "\"int64\"",
  "\"uint64\"", "\"double\"", "\"function\"", "\"lambda\"", "\"int8\"",
  "\"uint8\"", "\"int16\"", "\"uint16\"", "\"tuple\"", "\"variant\"",
  "\"generator\"", "\"yield\"", "\"sealed\"", "\"+=\"", "\"-=\"", "\"/=\"",
  "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"",
  "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"",
  "\"!=\"", "\"->\"", "\"<-\"", "\"??\"", "\"?.\"", "\"?[\"", "\"<|\"",
  "\" <|\"", "\"$ <|\"", "\"@ <|\"", "\"@@ <|\"", "\"|>\"", "\":=\"",
  "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"::\"",
  "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"", "\"||=\"", "\"^^=\"", "\"..\"",
  "\"$$\"", "\"$i\"", "\"$v\"", "\"$b\"", "\"$a\"", "\"$t\"", "\"$c\"",
  "\"$f\"", "\"...\"", "\"[[\"", "\"[{\"", "\"{{\"",
  "\"integer constant\"", "\"long integer constant\"",
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
  "function_declaration", "$@6", "expression_block", "expression_any",
  "expressions", "expr_keyword", "optional_expr_list",
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
  "expr_numeric_const", "expr_assign", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_name",
  "func_addr_expr", "$@21", "$@22", "$@23", "$@24", "expr_field", "$@25",
  "$@26", "expr", "$@27", "$@28", "$@29", "$@30", "$@31", "$@32",
  "expr_mtag", "optional_field_annotation", "optional_override",
  "optional_constant", "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@33", "$@34", "$@35",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@36", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@37", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@38", "alias_list",
  "alias_declaration", "$@39", "optional_public_or_private_enum",
  "enum_name", "enum_declaration", "$@40", "$@41", "$@42", "$@43",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@44", "$@45", "variable_name_with_pos_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "bitfield_bits", "bitfield_alias_bits",
  "bitfield_type_declaration", "$@46", "$@47", "table_type_pair",
  "type_declaration_no_options", "$@48", "$@49", "$@50", "$@51", "$@52",
  "$@53", "$@54", "$@55", "$@56", "$@57", "$@58", "$@59", "$@60", "$@61",
  "$@62", "$@63", "$@64", "$@65", "$@66", "$@67", "$@68", "$@69", "$@70",
  "$@71", "$@72", "$@73", "type_declaration", "tuple_alias_declaration",
  "$@74", "$@75", "$@76", "$@77", "variant_alias_declaration", "$@78",
  "$@79", "$@80", "$@81", "bitfield_alias_declaration", "$@82", "$@83",
  "$@84", "$@85", "make_decl", "make_struct_fields", "make_struct_dim",
  "optional_block", "optional_trailing_semicolon_cur_cur",
  "optional_trailing_semicolon_cur_sqr",
  "optional_trailing_semicolon_sqr_sqr", "optional_trailing_delim_sqr_sqr",
  "optional_trailing_delim_cur_sqr", "make_struct_decl", "$@86", "$@87",
  "$@88", "$@89", "$@90", "$@91", "make_tuple", "make_map_tuple",
  "make_tuple_call", "make_dim", "make_dim_decl", "$@92", "$@93", "$@94",
  "$@95", "make_table", "expr_map_tuple_list", "make_table_decl",
  "array_comprehension_where", "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1281)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-701)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1281,    35, -1281, -1281,    54,    27,   247,    62, -1281,   -67,
     279,   279,   279,    82,   330, -1281, -1281, -1281,    71, -1281,
   -1281, -1281,   345, -1281,   228, -1281, -1281, -1281, -1281, -1281,
   -1281, -1281, -1281, -1281, -1281,   156, -1281,   238,   251,   261,
   -1281, -1281, -1281, -1281,   247, -1281,    24, -1281,   279,   279,
   -1281, -1281,   228, -1281, -1281, -1281, -1281, -1281,   365,   358,
   -1281, -1281, -1281,   330,   330,   330,   421, -1281,   615,   339,
   -1281, -1281, -1281, -1281,   476,   503,   512, -1281,   545,    45,
      54,   471,    27,   432,   485, -1281,   616,   616, -1281,   509,
     479,    -7,   481,   583,   531,   536,   556, -1281,   559,   491,
   -1281, -1281,   -25,    54,   330,   330,   330,   330, -1281, -1281,
   -1281, -1281, -1281, -1281, -1281,   565, -1281, -1281, -1281, -1281,
   -1281,   539, -1281, -1281, -1281, -1281, -1281,   295,    73, -1281,
   -1281, -1281, -1281,   697, -1281, -1281, -1281, -1281, -1281,   562,
   -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281,   600,
   -1281,    75, -1281,   443,   634,   615, 13614, -1281,   198,   673,
   -1281,   621, -1281, -1281,   629, -1281, -1281, -1281, -1281, -1281,
   -1281, -1281,   244, -1281,   598, -1281,   573,   578,   579, -1281,
   -1281, 12507, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281,
   -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281,
   -1281, -1281, -1281, -1281, -1281, -1281, -1281,   760,   763, -1281,
     599,   601,   640, -1281, -1281,   639, -1281,   597,    54,   122,
   -1281, -1281, -1281,    73, -1281,  9355, -1281, -1281, -1281,   648,
     652, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281,
   -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281,
   -1281,   655,   617, -1281, -1281, -1281, -1281, -1281, -1281, -1281,
   -1281, -1281, -1281, -1281, -1281,   801, -1281, -1281, -1281, -1281,
   -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281,   658,   620,
   -1281, -1281,   125,   643, -1281, -1281, -1281, -1281, -1281, -1281,
   -1281, -1281,   623, -1281,   644,   624, -1281,   621,   349,   622,
     790,   459, -1281, -1281,   656,   657,   660,   632,   661,   662,
   -1281, -1281, -1281,   636, -1281, -1281, -1281, -1281, -1281,   664,
   -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281,
   -1281,   665, -1281, -1281, -1281,   666,   667, -1281, -1281, -1281,
   -1281,   669,   670,   654,    82, -1281, -1281, -1281, -1281, -1281,
     651,   674, -1281, -1281, -1281, -1281, -1281, -1281, -1281,   680,
     730, -1281,   659, -1281,     4, -1281,   409,  9355, -1281,  1956,
   -1281,   -90,    82, -1281, -1281, -1281,   122,   663,  8584,   698,
     701,  9355, -1281,   -14, -1281, -1281, -1281,  8584, -1281, -1281,
     702, -1281,   389,   395,   398, -1281, -1281,  8584,   668, -1281,
   -1281, -1281,    14, -1281, -1281, -1281,    29,  4803, -1281,   223,
     -83,  9069,   -81, -1281, -1281,  8584,   381,   671,   829, -1281,
     103, -1281,    51,   674, -1281,   684,   685,  8584, -1281, -1281,
   -1281, -1281, -1281,    39,   672,   692,   216,  3012, -1281, -1281,
     601,   266,  5002,   675,  8584,   715,   695,   696,   678, -1281,
     676,   700,   733,  5201,   360,   287, -1281,   679,   706,   707,
    3211,  8584,  8584,   495,   495,   495,   689,   690,   691,   699,
     704,   708, -1281,  1370,  1604,  8584, -1281, -1281, -1281, -1281,
   -1281, -1281, -1281,  5400,   711, -1281,  8584,  8584,  8584,  8584,
    8584,  5599,  8584, -1281,   694, -1281,  8584,   866, -1281, -1281,
     709,   710,  8584,   896, -1281, -1281, -1281, -1281, -1281, -1281,
   -1281, -1281, -1281, -1281,   -89,   712, -1281,   713,   714,   716,
   -1281,   717, -1281, -1281,   842, -1281, -1281, -1281, -1281,   728,
   -1281, -1281,   -74, -1281, -1281, -1281, -1281, -1281,  8892, -1281,
     735, -1281, -1281, -1281, -1281, -1281, -1281, -1281,   194, -1281,
     731, -1281,    20, -1281, -1281,   736,   725,  9735, -1281,   880,
     606, -1281, -1281, -1281,  3609,  9355,  9355,  9355,  9770,  9355,
    9355,   737,   780,  9355,   599,  9355,   599,  9355,   599,  9455,
     781,  9867,  8584, -1281, -1281, -1281, -1281,   742, -1281, -1281,
   12025, -1281,   651,   741, -1281,   740,   -15, -1281,   744,   674,
     743,   773, -1281,   747,   748,  9964, -1281,  8584,   794, -1281,
     796, -1281,  9355, -1281,  3609, -1281, -1281, -1281,  1599,   381,
   -1281,   756, -1281, -1281,    82, -1281, -1281,  8584,  3609, -1281,
     784,   841, 12397,   957,  9355,  8584,  8584, 12868,  8584,  1599,
     789, -1281, -1281,  8584, -1281, -1281,   793,   814, 12868,  8584,
   -1281, -1281,  8584,  8584, -1281,  9355,  3609, -1281, 12397,   366,
     366,   764, -1281,   728, -1281, -1281, -1281,  8584,  8584,  8584,
    8584,  8584,  8584,   381,  2379,   381,  2590, 12542, -1281,   154,
   -1281,  1599, -1281,   590,   366,   366,   -24,   366,   366,   381,
     795, 12868,  9999,   246, -1281,   800,   774, -1281, -1281,  1599,
   -1281, -1281,  5798, -1281, -1281, -1281, -1281, -1281, -1281,   452,
     832,   495, -1281, 13411, 13506,  8584,  8584,  8584,  8584,  8584,
    8584,  8584,  8584,  8584,  8584, -1281, -1281,  8584,  8584,  8584,
    8584,  8584,  8584,   819,  3808,  8584,   -43,  8584,  8584,  8584,
    8584,  8584,  8584,  8584,  8584,  8584,  8584,  8584,  8584,  8584,
    8584,  8584,  4007,  5997,  8584,  8584,  8584,  8584,  8584,  8584,
    8584,  8584,  8584,  8584,   -44,  8584,  6196,   -90,  8584, -1281,
   -1281,   330, -1281,   982,   621, -1281,   826, -1281,  3609, -1281,
    9070,   478,   492,   804,    -1, -1281,   530,   535, -1281, -1281,
     -23,   538,   643,   543,   643,   555,   643, -1281,   189, -1281,
     222, -1281,   791,   784, -1281, -1281, -1281, -1281,  9355, -1281,
   -1281,  9355, -1281, -1281, -1281, 10096, -1281,   956,   -34, -1281,
   12397,  9355,  9355, -1281,    11,  9355,  6395,  6594,  9355,  9355,
     255, -1281,   784,  8584,  8584,  8584,   601,   727,   276, 12868,
   10193, -1281, -1281,  9355,  9355, 10228,  9355, -1281, -1281, 10325,
    9355,   277,   297,  9355,   489, -1281, 12397, -1281,   832, 10422,
   10457, 10554, 10651, 10686, 10783,    12,   495,   799,   -19,  2801,
    4206,  6793, 12639,   822,   -20,   250,   834,   182,    37,  6992,
     -20,    53,  8584, -1281,  8584,   803, -1281,  9355, -1281,  9355,
   -1281,  8584,   498,    38,  8584,   815, -1281,   820,   823,   430,
   -1281, -1281,  8584,   809, -1281,   612,  8584,  4405, -1281,   298,
     865, -1281, -1281,   -28,   599, -1281,   845,   827, -1281, -1281,
     849,   831, -1281, -1281, 12868, 12868, 12868, 12868, 12868, 12868,
   12868, 12868,   135,   135,   574, 12868, 12868,   574, 13343, 13343,
     833,   495,   495,   495, 12868,    50,   835, -1281, 12060,   -47,
     -47,   735, 12868,   135,   135, 12868, 12868, 13140, 13097,  1734,
   12868, 12868, 12868, 12965,   495,   495,   495, 12868, 13537, 12736,
   13227,  9217, 13256,   574,   574,   466,   466,    50,    50,    50,
     114,  8584,   838,   840,   310,  8584,  1006, 12157, -1281,   301,
     828,  9446,   352,   593,   983,   837, -1281, -1281,  9178, -1281,
   -1281, -1281, -1281,  9355, -1281, -1281, -1281,   885, -1281,   873,
   -1281,   875, -1281,   878, -1281,  9455, -1281,   781, -1281,   674,
     674, -1281,   889,  8584,   830,   876,  8584,   557, -1281,   302,
     894, 10880,   558,   908, -1281, 12868, -1281,   861,  9355,  8584,
   -1281, -1281,  8584,   919,   997, -1281,  1014, -1281,  1703, -1281,
   -1281,  2231,   832,   599, -1281, -1281, -1281, -1281, -1281,   862,
    8584, -1281,  8584,  8584,  8584,  4604, 12025,    13,  8584,   450,
     456,   250, -1281, -1281,   864, -1281,  8584, -1281,   869,  8584,
   -1281,  8584,   456,   -77, -1281,   872, -1281, 12868, -1281, -1281,
    2519,  2730, 12771,  8584, -1281,  8584,  8584,  9355,   599, -1281,
   -1281,  2169,   601, -1281, -1281,  8584, -1281,  1364,   355,  8783,
   -1281, -1281,   363,   905,   906,   911,   913, -1281,   369,   643,
   -1281,  8584, -1281,  8584,  7191, -1281, -1281, -1281,  8584, -1281,
   -1281, -1281, -1281,   895,   881, -1281, -1281,  8584,   882, -1281,
   12254,  8584,  7390,   886, -1281, 12289, -1281, -1281, -1281, -1281,
   -1281,   883, -1281, -1281,    80, -1281,    22, -1281, -1281, -1281,
   -1281, -1281,   674, -1281, -1281, -1281, -1281, -1281, -1281, -1281,
   -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281, 12868, -1281,
   -1281,   -85, -1281, -1281,   232, -1281, -1281, -1281,  8584,  2941,
   12868, -1281, -1281, -1281, -1281, -1281,   892,   643,  7589,   -68,
   10915, 12868, 12868,     8,   -20,   250, 12868,   893,   142,   822,
   -1281, -1281, 12868, -1281,   834,   153,   -20, -1281, -1281,   890,
   -1281, -1281, -1281, -1281, -1281,   220, 11012, 11109,  3009,   643,
   -1281, -1281,  1599, -1281,   899,   874,  4405,   921,   897, -1281,
   -1281, -1281, -1281,   -28,   898,   -80,  9355, 11144,  9355, 11241,
   -1281,   308, 11338, -1281,  8584, 13000,  8584, -1281, 11373, -1281,
     326,  8584, -1281, -1281, -1281,  1060,    22, -1281, -1281,   593,
   -1281, -1281, -1281,   900,   904, -1281, -1281,  8584,   935, -1281,
     907,   327,   909,  8584,  8584,  8584,   912,   916,  7788,   -80,
   -1281,   331,  8584,   209,   250, -1281,  8584,  8584,  8584,  8584,
     -77, -1281,  8584,   918,  8584, -1281, -1281, -1281,   923,   612,
    3410, -1281, -1281,   370, -1281,   457, -1281, -1281,  7987, -1281,
   -1281,  3208, -1281,   560, -1281, -1281, -1281,  9355, 11470, 11567,
   -1281, -1281, 11602, -1281, -1281,  1060,   381,   456,   456, 11699,
     926,  8584, -1281,  8584,   574,   574,   574,  8584,   456, -1281,
   11796, -1281, -1281,  9484,  8584,  8584, -1281, 11831, 12868, 12868,
    9484, -1281,   574,  8584,  9484,   278, -1281, -1281,  8186,  8385,
   -1281, -1281, -1281, -1281, -1281, 12868,   910,  9355,   -14,  8584,
   12965, -1281, -1281,   563, -1281, -1281, -1281, 13614, -1281, -1281,
     445,   474, -1281,  8584,   333,   334,   338,   506, -1281,  1068,
     920, 12868, 12868,   213,   914,   922,   929, -1281,   278,  8584,
    8584, 12868, -1281,   871,  8584, 13000, -1281, -1281, -1281,   917,
   13614, -1281, -1281, 11928, -1281, -1281, -1281, -1281,  8584,   931,
    8584,  8584,   933, -1281, -1281, -1281, 12868,  8584,  9592, -1281,
   -1281,   601, -1281, 12868, -1281, 12868, 12868, -1281,  9627, -1281,
   -1281, -1281
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   118,     1,   283,     0,     0,     0,   567,   284,     0,
     559,   559,   559,     0,     0,    16,    15,     3,     0,    10,
       9,     8,     0,     7,   547,     6,    11,     5,     4,    13,
      12,    14,    92,    93,    91,   100,   102,    37,    53,    50,
      51,    39,    40,    41,     0,    42,    48,    38,   559,   559,
      22,    21,   547,   561,   560,   722,   712,   717,     0,   256,
      35,   105,   106,     0,     0,     0,   107,   109,   116,     0,
     104,    17,   585,   584,   211,   569,   586,   548,   549,     0,
       0,     0,     0,    43,     0,    49,     0,     0,    46,     0,
       0,   559,     0,    18,     0,     0,     0,   258,     0,     0,
     115,   110,     0,     0,     0,     0,     0,     0,   119,   213,
     212,   215,   210,   571,   570,     0,   588,   587,   591,   551,
     550,   553,    98,    99,    96,    97,    95,     0,     0,    94,
     103,    54,    52,    48,    45,    44,   562,   564,   566,     0,
     568,    19,    20,    23,   723,   713,   718,   257,    33,    36,
     114,     0,   111,   112,   113,   117,     0,   572,     0,   581,
     544,   482,    24,    25,     0,    87,    88,    85,    86,    84,
      83,    89,     0,    47,     0,   565,     0,     0,     0,    34,
     108,     0,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,     0,     0,   125,
     120,     0,     0,   573,   582,     0,   592,   545,     0,     0,
      26,    27,    28,     0,   101,     0,   724,   714,   719,   179,
     180,   177,   128,   129,   131,   130,   132,   133,   134,   135,
     161,   162,   159,   160,   152,   163,   164,   153,   150,   151,
     178,   172,     0,   176,   165,   166,   167,   168,   139,   140,
     141,   136,   137,   138,   149,     0,   155,   156,   154,   147,
     148,   143,   142,   144,   145,   146,   127,   126,   171,     0,
     157,   158,   482,   123,   240,   216,   626,   629,   627,   630,
     628,   631,     0,   555,   579,   589,   552,   482,     0,     0,
     534,   532,   554,    90,     0,     0,     0,     0,     0,     0,
     599,   619,   600,   633,   601,   605,   606,   607,   608,   625,
     612,   613,   614,   615,   616,   617,   618,   620,   621,   622,
     623,   682,   604,   611,   624,   689,   696,   602,   609,   603,
     610,     0,     0,     0,     0,   632,   646,   649,   647,   648,
     709,   563,   638,   512,   518,   181,   182,   175,   170,   183,
     173,   169,     0,   121,     0,   506,     0,     0,   214,     0,
     576,   574,     0,   583,   495,   593,     0,     0,     0,     0,
       0,     0,   533,     0,   652,   673,   676,     0,   679,   669,
       0,   641,   683,   690,   697,   703,   706,     0,     0,   659,
     664,   658,     0,   672,   668,   661,     0,     0,   663,     0,
     725,   715,   720,   184,   174,     0,     0,     0,   594,   503,
     523,   122,   482,   124,   242,     0,     0,     0,    63,    64,
      76,   385,   386,     0,     0,     0,     0,   271,   379,   269,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   270,
       0,     0,     0,     0,     0,     0,   625,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   471,     0,     0,     0,   327,   329,   328,   330,
     331,   332,   333,     0,     0,    29,     0,     0,     0,     0,
       0,     0,     0,   313,   314,   219,     0,   217,   383,   382,
       0,     0,     0,     0,   235,   230,   227,   226,   228,   229,
     241,   221,   465,   220,   380,     0,   456,    71,    72,    69,
     233,    70,   234,   236,   286,   225,   455,   454,   453,   118,
     459,   381,     0,   222,   458,   457,   426,   387,   334,   388,
       0,   384,   727,   731,   728,   729,   730,   555,     0,   556,
       0,   580,   496,   546,   483,     0,     0,     0,   536,   537,
       0,   530,   531,   529,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   120,     0,   120,     0,   120,     0,
       0,     0,   244,   666,   667,   660,   662,     0,   665,   651,
       0,   711,   710,     0,   639,     0,   256,   513,     0,   508,
       0,     0,   519,     0,     0,     0,   504,     0,     0,   522,
       0,   521,     0,   524,     0,   507,   762,   765,     0,     0,
     261,   265,   264,   268,     0,   302,   783,     0,     0,   275,
     272,     0,   308,     0,     0,     0,     0,   259,     0,     0,
       0,   293,   296,     0,   239,   299,     0,     0,    57,     0,
      78,   787,     0,     0,   768,     0,     0,   280,   277,   415,
     416,   314,   324,   118,   255,   253,   254,     0,     0,     0,
       0,     0,     0,     0,   742,     0,     0,   775,   790,     0,
     246,     0,   249,     0,   392,   391,   431,   390,   389,     0,
     801,   308,     0,   315,   792,   801,     0,   237,   238,     0,
      74,   463,     0,   224,   231,   232,   274,   279,   285,     0,
     322,     0,   223,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   417,   418,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   376,     0,     0,   577,     0,   557,
     575,     0,   590,     0,   482,   535,     0,   539,     0,   543,
     334,     0,     0,     0,   644,   655,     0,     0,   634,   636,
       0,     0,   123,     0,   123,     0,   123,   510,     0,   516,
       0,   635,     0,   245,   657,   650,   640,   726,     0,   514,
     716,     0,   520,   721,   505,     0,   596,   597,   525,   528,
     527,     0,     0,    79,     0,     0,     0,     0,     0,     0,
       0,   276,   273,     0,     0,     0,     0,     0,     0,   260,
       0,    80,    81,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,     0,     0,   281,   278,   315,   322,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   256,     0,
       0,     0,   771,   740,   742,     0,   777,     0,     0,     0,
     742,     0,     0,   745,     0,     0,   795,     0,   243,     0,
      32,     0,    30,     0,   802,     0,   419,     0,     0,   366,
     363,   365,   802,     0,   240,    60,     0,     0,   427,     0,
       0,   292,   291,     0,   120,   252,     0,     0,   442,   441,
       0,     0,   443,   447,   344,   345,   347,   346,   348,   338,
     339,   340,   393,   394,   406,   349,   350,   407,   404,   405,
       0,     0,     0,     0,   336,   436,     0,   424,     0,   460,
     461,   462,   337,   395,   396,   351,   352,   411,   412,   413,
     341,   342,   343,   414,     0,     0,     0,   335,     0,     0,
     409,   410,   408,   402,   403,   398,   397,   399,   400,   401,
       0,     0,     0,   372,     0,     0,     0,     0,   429,     0,
       0,     0,     0,   489,   492,     0,   538,   541,   334,   542,
     653,   674,   677,     0,   680,   670,   642,     0,   684,     0,
     691,     0,   698,     0,   704,     0,   707,     0,   656,   509,
     515,   595,     0,     0,     0,     0,     0,     0,   266,     0,
       0,     0,     0,     0,   782,   309,   282,     0,     0,     0,
     796,   432,     0,     0,     0,   433,     0,   464,     0,   786,
     776,     0,   322,   120,   466,   467,   468,   469,   470,     0,
       0,   743,     0,     0,     0,     0,   308,   742,     0,     0,
       0,     0,   751,   752,     0,   757,     0,   749,     0,     0,
     780,     0,     0,     0,   747,     0,   781,   774,   791,   744,
       0,     0,     0,     0,   779,     0,     0,     0,   120,   793,
     794,     0,     0,    58,    59,     0,    73,    65,     0,     0,
     428,   287,     0,     0,     0,     0,     0,   320,     0,   123,
     438,     0,   444,     0,     0,   358,   356,   357,     0,   422,
     355,   353,   354,     0,     0,   448,   452,     0,     0,   425,
       0,     0,     0,     0,   373,     0,   377,   420,   430,   578,
     558,   119,   490,   491,   492,   493,   484,   497,   540,   654,
     675,   678,   645,   681,   671,   643,   637,   685,   687,   692,
     694,   699,   701,   705,   511,   708,   517,   598,   526,   763,
     766,     0,   262,   267,     0,   305,   303,   784,     0,     0,
      82,   294,   297,   300,   788,   769,     0,   123,     0,     0,
       0,   733,   732,     0,   742,     0,   772,     0,     0,   741,
     756,   750,   773,   748,   778,     0,   742,   754,   755,     0,
     760,   746,   247,   250,    31,     0,     0,     0,     0,   123,
     218,    61,     0,    66,     0,     0,     0,     0,     0,   318,
     319,   317,   316,     0,     0,     0,     0,     0,     0,     0,
     361,     0,     0,   449,     0,   437,     0,   423,     0,   374,
       0,     0,   421,   378,   502,   487,   484,   485,   486,   489,
     688,   695,   702,     0,     0,    77,   263,     0,     0,   304,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     472,     0,     0,     0,     0,   758,     0,     0,     0,     0,
       0,   753,     0,     0,     0,   364,   481,   367,     0,    60,
       0,    75,   359,     0,   288,   532,   321,   323,     0,   310,
     326,     0,   480,     0,   478,   362,   475,     0,     0,     0,
     474,   375,     0,   488,   498,   487,     0,     0,     0,     0,
       0,     0,   797,     0,   295,   298,   301,     0,     0,   434,
       0,   325,   473,   799,     0,     0,   759,     0,   735,   734,
     799,   761,   248,   244,   799,     0,   370,    62,   271,     0,
      67,    71,    72,    69,    70,    68,     0,     0,     0,     0,
     311,   439,   445,     0,   479,   477,   476,     0,   500,   494,
       0,     0,   306,     0,     0,     0,     0,     0,   435,     0,
       0,   737,   736,     0,     0,     0,     0,   368,     0,     0,
       0,   277,   360,     0,     0,   312,   440,   446,   450,     0,
       0,   764,   767,     0,   785,   798,   789,   770,     0,     0,
       0,     0,     0,   251,   803,   371,   278,     0,     0,   451,
     499,     0,   307,   800,   804,   739,   738,   805,     0,   290,
     501,   289
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1281, -1281, -1281, -1281, -1281, -1281,   449,  1061, -1281, -1281,
   -1281,  1140, -1281, -1281, -1281,   647,  1098, -1281,  1010, -1281,
   -1281,  1062, -1281, -1281, -1281,  -164, -1281, -1281, -1281,  -163,
   -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281,   925,
   -1281, -1281,  1066,   -56, -1281, -1281,   343,   378,  -490,  -558,
    -758, -1281, -1281, -1281, -1280, -1281, -1281,  -209, -1281,   248,
   -1281,  -212, -1281, -1281, -1281, -1281, -1281,  -420,   -13, -1281,
   -1281, -1281, -1281, -1281,  -157,  -156,  -155, -1281,  -154, -1281,
   -1281,  1158, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281,
   -1281, -1281, -1281, -1281, -1281, -1281, -1281,  -429,  -128,  -442,
     -79, -1281,  -830,  -439, -1281, -1281,  -360, -1281, -1281, -1281,
   -1279, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281,   522,
   -1281, -1281, -1281, -1281, -1281, -1281, -1281,  -150,  -104,  -172,
    -103,    15, -1281, -1281, -1281, -1281, -1281,   745, -1281,  -560,
   -1281, -1281,  -566, -1281, -1281,  -416,  -410,  -553,  -147, -1281,
    -352, -1281, -1281,  1118, -1281, -1281, -1281,   625,   792,    59,
   -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281,
   -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281,
    -486,  -196, -1281,   746, -1281, -1281, -1281, -1281, -1281, -1281,
   -1281,  -376, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281,
   -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281,
   -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281,  -222, -1281,
   -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281, -1281,
   -1281, -1281, -1281, -1281,   749,  -870,  -664,  -832, -1281, -1281,
   -1281, -1056,  -127, -1281, -1281, -1281, -1281, -1281, -1281, -1281,
      95,  -462, -1281,   499, -1281, -1281, -1281, -1281, -1281, -1281,
    -625, -1281,  -610,   482, -1281
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    16,   143,    52,    17,   164,   170,   683,   498,
     149,   499,    99,    19,    20,    45,    46,    47,    88,    21,
      39,    40,   500,   501,  1105,  1106,   502,  1234,  1310,   503,
     504,   906,   505,   619,   506,   507,   508,   509,  1042,   171,
     172,    35,    36,    37,    66,    67,    68,    69,    22,   283,
     368,   210,    23,   111,   211,   112,   156,   662,   510,   369,
     511,   802,   512,   887,  1302,   889,  1303,   513,   514,   515,
     621,   825,  1276,   516,   517,   518,   519,   520,   521,   522,
     523,   524,   709,  1112,   911,   525,   526,   843,  1283,   844,
    1284,   846,  1285,   527,   828,  1279,   528,   690,  1320,   529,
    1117,  1118,   914,   664,   530,   531,   631,   533,   534,   535,
     901,   536,  1097,  1365,  1098,  1408,   537,   986,  1263,   691,
    1246,  1416,  1248,  1417,  1327,  1439,   539,   364,  1269,  1334,
    1154,  1156,   995,   552,   774,  1387,  1420,   365,   366,   598,
     798,   411,   603,   800,   412,  1064,   419,   564,   383,   301,
     302,   217,   297,    78,   121,    25,   161,   371,    89,    90,
     174,    91,    26,    49,   115,   158,    27,   293,   550,   547,
     990,   373,   215,   216,    76,   118,   375,    28,   159,   295,
     420,   540,   292,   347,   348,   790,   410,   349,   572,  1165,
     783,   350,   565,  1159,   570,  1164,   566,  1160,   567,  1161,
     569,  1163,   573,  1167,   574,  1270,   575,  1169,   576,  1271,
     577,  1171,   578,  1272,   579,  1173,   580,  1175,   599,    29,
      95,   177,   353,   600,    30,    96,   178,   354,   604,    31,
      94,   176,   352,   595,   541,   873,   874,   875,   886,  1086,
    1080,  1075,  1220,   542,   821,  1273,   822,  1274,   853,  1287,
     876,   694,   543,   877,   544,   829,  1280,   850,  1286,   679,
     695,   545,  1400,   895,   546
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,    70,   285,   351,   614,   560,   606,   778,   630,   532,
     838,   219,   880,   678,   799,  1210,   792,   629,   794,   797,
     796,   663,   663,   663,   553,   665,   666,  -118,  1053,   346,
    1026,  1060,   866,   592,  1009,     2,  1011,  1108,  1013,   710,
     657,     3,  1071,   425,   426,    84,   583,   151,  1083,    53,
      70,    70,    70,   416,   711,    54,  1081,  1093,  1318,   122,
     123,   585,   773,   436,     4,   866,     5,  1267,     6,   441,
     713,   714,    32,    33,     7,   733,   548,   701,   736,   737,
      85,   980,   981,   593,     8,   601,  1407,   165,   166,   609,
       9,    70,    70,    70,    70,  1113,   833,   674,   676,    50,
    1217,   736,   737,  1218,   609,  1114,   455,  1419,    92,   561,
     982,   946,    10,   833,  1063,   104,   105,   106,   702,   562,
      98,   549,   983,   947,    98,  1268,  1265,   284,   594,  1435,
     602,   609,   284,   824,    11,    12,  1219,   712,   457,   458,
    1441,  1063,    51,  1292,   779,   423,  1155,   376,   611,  1295,
     139,   409,   417,   803,  1115,   713,   714,   764,   765,  1116,
     984,   985,   298,   611,   725,   726,  1006,   867,   563,   808,
     418,   346,   733,   858,   735,   736,   737,   738,    58,  1203,
     764,   765,   739,   150,   409,   346,    38,   865,  1007,   878,
     611,  1070,   610,   610,   819,   473,   474,   475,   830,   832,
    1209,   584,   362,   893,   124,    59,   138,   218,   831,   125,
    1003,   126,   345,   346,   127,   346,   586,    86,   610,   610,
      34,    58,  1196,   851,   852,   771,   609,   620,    87,    13,
    1084,   633,   167,   772,   587,  1205,   855,   168,  1356,   169,
      14,   304,   127,   588,   491,   650,    15,   305,    59,   725,
     726,   496,   128,   306,   764,   765,    80,   733,   837,   735,
     736,   737,   738,   307,  1079,   609,  1085,   739,  1138,   663,
     299,   308,   915,   909,    48,  1297,   362,   346,   346,   854,
    1139,   218,    71,   180,   610,   611,   309,   612,   300,    77,
     613,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   611,   759,   760,   761,   762,   763,
     883,   398,   609,   363,   833,    53,   609,   989,    79,   764,
     765,    54,  1354,   781,   782,   784,  1430,   786,   787,    72,
      73,   791,    74,   793,   345,   795,  1119,   912,   997,   551,
    1077,  1245,    58,    61,  1299,   884,  1313,   885,   345,   346,
     346,   346,  1294,   346,   346,   343,   768,   346,  1014,   346,
      75,   346,   212,   346,  1300,    58,   713,   714,  1078,    59,
     818,   611,    62,  1079,   897,   611,   345,  1029,   345,   898,
    1015,   833,  1023,  1204,   626,   769,   100,   101,   102,   823,
     213,  1016,    59,    41,    42,    43,   346,    58,  1216,    80,
     345,  1277,  1088,   627,   663,   223,   897,  1061,  1072,  1073,
     841,  1304,   344,  1017,   899,    81,   833,   591,   346,  1289,
    1099,    44,    82,  1278,    59,  1024,  1025,   152,   153,   154,
     155,  1176,   224,  1033,   634,  1174,  1074,  1039,   833,   346,
     345,   345,    63,  1034,  1143,   162,   163,  1043,  1044,    58,
    1046,  1308,   888,   635,  1048,   651,  1144,  1051,   833,  1109,
     725,   726,   833,   833,  1040,  1049,   713,   714,   733,   833,
     905,   736,   737,   738,   652,  1197,    59,    98,   739,   663,
     663,   663,  1125,  1126,  1127,  1050,  1110,   833,  1039,  1148,
    1183,  1090,   833,  1091,   833,  1039,  1325,   919,   923,   833,
     107,   399,   663,   663,   663,  1130,  1131,  1132,    64,   417,
      80,    97,   109,   107,  1331,  1342,  1069,    65,   110,  1352,
    1229,  1424,  1425,   951,  1237,   108,  1426,   418,   400,   401,
    1243,  1069,   345,   345,   345,   377,   345,   345,  1151,   113,
     345,  1235,   345,  1281,   345,   114,   345,   649,   116,  1238,
     764,   765,   284,  -686,   117,  1244,  1376,  1099,  -686,  -693,
     725,   726,  -700,   104,  -693,   106,  1019,  -700,   733,  1020,
     735,   736,   737,   738,   713,   714,  -686,  1181,   739,   345,
     299,   119,  -693,  1027,  1207,  -700,  1032,   120,   910,   402,
     867,   827,   346,   403,  -369,   346,  1208,   421,   300,  -369,
     422,   345,  1203,  1102,   994,   346,   346,  1036,   103,   346,
     131,  1199,   346,   346,  1103,  1104,    87,  -369,   399,   141,
     380,  1377,   345,   381,   382,   142,   382,   346,   346,  1152,
     346,   133,  1215,  1421,   346,  1153,  1070,   346,   761,   762,
     763,   148,  1189,   409,  1225,   400,   401,  1000,   220,   221,
     764,   765,   404,  1390,  1391,   136,   405,   409,  1052,   406,
     900,  1001,  1422,   399,  1397,  1070,   723,   724,   725,   726,
     137,   346,   140,   346,   407,  1251,   733,   144,   735,   736,
     737,   738,   145,   408,   493,   661,   739,   284,   741,   742,
     400,   401,   286,  1260,  1427,   409,   287,  1070,  1395,  1004,
     409,  1228,   146,   409,  1005,   147,   402,  1008,   409,   561,
     403,   157,  1010,   134,   135,   288,   289,   290,   291,   562,
     409,   532,   409,   409,  1012,   409,  1182,  1186,   409,  1382,
    1404,   160,  1418,    85,  1406,   104,   105,   106,    70,   399,
     162,   163,   890,   891,   759,   760,   761,   762,   763,  1291,
     179,   402,  1136,   175,   104,   403,   214,   218,   764,   765,
     225,  1162,    41,    42,    43,   226,   400,   401,   563,   404,
     227,   228,  1298,   405,   280,   345,   406,   281,   345,   220,
     221,   222,    55,    56,    57,   294,   282,   346,   345,   345,
     296,   407,   345,   284,   355,   345,   345,   777,   356,   346,
     408,   357,   359,   358,   360,  1414,   361,   367,   372,   378,
     345,   345,   379,   345,   404,   370,   374,   345,   405,   387,
     345,   406,   346,   390,   384,   385,   413,   402,   386,   388,
     389,   403,   391,   392,   393,   394,   407,   395,   396,   409,
    1437,   397,   399,   414,   558,   408,   415,   559,   571,   554,
    1321,   608,   616,   617,   345,   582,   345,  1355,   607,   624,
     625,   640,   638,   641,   642,   643,   653,   644,   645,   400,
     401,   538,   646,  1231,   654,   655,   667,   668,   669,   682,
     557,   346,   696,   399,   693,   700,   670,   708,   399,   568,
     404,   671,  1394,   680,   405,   672,  1037,   406,  1396,   581,
     697,   698,   776,   703,   704,   705,  1389,   706,   707,   590,
     400,   401,   407,    14,   803,   400,   401,   605,  1038,   630,
     399,   408,   766,   649,   770,   788,   789,   601,   804,   618,
     402,   399,   806,   807,   403,   809,   810,   811,   812,   632,
     816,   813,   817,   826,   637,   833,   639,   400,   401,   711,
     836,   842,  1275,   848,   857,   648,   894,   847,   400,   401,
     832,   902,   658,   659,   660,   940,   904,  1431,   913,   993,
     345,   402,   996,  1002,   561,   403,   402,   677,  1022,  1018,
     403,  1413,   345,  1069,   562,   681,  1062,  1146,   684,   685,
     686,   687,   688,   404,   692,  1076,  1089,   405,   677,  1179,
     406,  1094,  1100,  1309,   699,   345,  1323,  1095,   402,   399,
    1096,  1111,   403,  1120,  1121,   407,  1319,  1122,  1123,   402,
    1124,  1149,  1128,   403,   408,  1141,   399,  1142,  1157,  1155,
     346,  1166,   346,   563,   404,  1177,   400,   401,   405,   404,
    1184,   406,  1168,   405,  1170,  1180,   406,  1172,  1188,  1198,
    1211,  1239,  1240,   400,   401,  1213,   407,  1241,  1221,  1242,
    1319,   407,  1312,  1253,   345,   408,   780,  1314,  1254,  1256,
     408,   404,  1333,  1261,  1264,   405,  1301,  1187,   406,  1288,
    1296,  1340,   404,  1315,  1317,  1383,   405,  1337,  1191,   406,
    1311,  1338,  1366,   407,  1341,  1393,  1343,   402,  1412,  1347,
    1428,   403,   408,  1348,   407,  1363,  1429,  1432,  1440,   815,
    1433,   346,   892,   408,   402,  1434,   820,  1444,   403,  1447,
     129,    18,    83,   173,   132,  1367,   130,  1370,   303,   992,
     632,  1405,  1101,  1371,  1372,  1373,  1374,   677,   839,    24,
     840,  1351,  1335,  1388,  1316,   845,  1336,   615,  1378,  1266,
      93,   849,   767,  1361,  1214,   881,     0,   903,   856,   622,
     404,   346,   623,     0,   405,     0,  1192,   406,     0,   859,
     860,   861,   862,   863,   864,     0,   872,   404,   872,     0,
       0,   405,   407,  1193,   406,     0,     0,     0,     0,     0,
       0,   408,     0,     0,     0,     0,     0,     0,     0,   407,
       0,     0,     0,     0,     0,     0,     0,     0,   408,     0,
       0,     0,  1450,   345,     0,   345,     0,   924,   925,   926,
     927,   928,   929,   930,   931,   932,   933,     0,     0,   934,
     935,   936,   937,   938,   939,     0,   944,   945,     0,   948,
     949,   950,   952,   953,   954,   955,   956,   957,   958,   959,
     960,   961,   962,   963,   967,   969,   970,   971,   972,   973,
     974,   975,   976,   977,   978,   979,     0,   987,     0,     0,
     991,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     998,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1031,
       0,     0,   900,     0,     0,  1035,   944,   967,     0,     0,
       0,     0,     0,     0,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1233,     0,     0,     0,     0,
       0,     0,   673,     0,   713,   714,     0,     0,   304,     0,
       0,   684,  1066,   692,   305,   900,     0,     0,     0,     0,
     306,   692,     0,     0,  1087,     0,   677,     0,     0,     0,
     307,     0,     0,  1092,     0,     0,  1035,     0,   308,     0,
       0,     0,     0,     0,   677,     0,     0,     0,  1107,     0,
       0,     0,     0,   309,     0,     0,     0,     0,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,     0,     0,     0,     0,     0,   723,   724,   725,   726,
     727,     0,     0,   730,   731,   732,   733,     0,   735,   736,
     737,   738,     0,     0,     0,     0,   739,     0,   741,   742,
       0,     0,     0,  1140,   745,   746,   747,  1145,     0,    58,
     751,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   343,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,  1178,     0,   753,     0,   754,
     755,   756,   757,   758,   759,   760,   761,   762,   763,     0,
       0,   677,     0,     0,  1190,     0,     0,     0,   764,   765,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   344,
       0,     0,     0,     0,  1200,  1201,  1202,     0,     0,     0,
    1206,     0,     0,     0,     0,     0,     0,     0,  1212,     0,
       0,   872,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   675,  1226,  1227,   713,
     714,     0,   304,   538,     0,     0,     0,  1232,   305,     0,
       0,  1035,     0,     0,   306,     0,     0,     0,     0,     0,
       0,     0,     0,  1247,   307,  1249,     0,     0,     0,     0,
    1252,     0,   308,     0,     0,     0,     0,     0,     0,  1255,
       0,     0,     0,  1258,     0,     0,     0,   309,     0,     0,
       0,     0,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,     0,     0,     0,     0,     0,
     677,   723,   724,   725,   726,   727,     0,     0,   730,   731,
     732,   733,     0,   735,   736,   737,   738,     0,     0,     0,
       0,   739,     0,   741,   742,   399,     0,     0,     0,   745,
     746,   747,     0,    58,     0,   751,     0,     0,     0,     0,
       0,     0,     0,     0,   713,   714,   343,     0,     0,     0,
       0,     0,   400,   401,     0,     0,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,  1328,     0,  1329,     0,
       0,     0,   753,  1332,   754,   755,   756,   757,   758,   759,
     760,   761,   762,   763,     0,     0,     0,     0,     0,  1339,
       0,     0,     0,   764,   765,  1344,  1345,  1346,     0,     0,
    1350,   284,     0,   344,  1353,     0,     0,     0,  1357,  1358,
    1359,  1360,     0,   402,  1362,     0,  1364,   403,     0,     0,
       0,     0,  1375,     0,     0,     0,     0,     0,     0,     0,
    1380,     0,     0,     0,     0,     0,   723,   724,   725,   726,
     727,     0,     0,   730,   731,   732,   733,     0,   735,   736,
     737,   738,     0,     0,     0,   677,   739,     0,   741,   742,
       0,     0,     0,     0,   745,     0,  1401,  1402,     0,     0,
       0,     0,     0,     0,     0,     0,   404,     0,     0,     0,
     405,  1411,  1194,   406,     0,     0,     0,     0,     0,     0,
       0,  1415,     0,     0,     0,     0,     0,     0,   407,     0,
       0,     0,     0,     0,     0,  1423,     0,   408,     0,   754,
     755,   756,   757,   758,   759,   760,   761,   762,   763,     0,
       0,     0,  1436,     0,     0,     0,  1438,     0,   764,   765,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1443,     0,  1445,  1446,     0,     0,     0,   424,     0,  1448,
     425,   426,     3,     0,   427,   428,   429,     0,   430,     0,
     431,   432,   433,   434,   435,     0,     0,     0,     0,     0,
     436,   437,   438,   439,   440,     0,   441,     0,     0,     0,
       0,     0,     0,   442,   443,     0,     0,   444,     0,   445,
     446,     0,     0,   447,     0,     8,   448,   449,     0,   450,
     451,     0,     0,   452,   453,     0,     0,     0,     0,     0,
     454,     0,     0,   455,   310,   311,   312,     0,   314,   315,
     316,   317,   318,   456,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,     0,   332,   333,   334,     0,
       0,   337,   338,   339,   340,   457,   458,   459,   460,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     461,   462,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   463,   464,   465,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   466,   467,   468,   469,   470,     0,   471,
       0,   472,   473,   474,   475,   476,   477,   478,   479,   480,
     481,   482,    59,   483,   484,   485,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,   488,     0,
      13,     0,     0,   489,   490,     0,     0,     0,     0,     0,
       0,   491,     0,   492,     0,   493,   494,   495,   496,   497,
     424,     0,     0,   425,   426,     3,     0,   427,   428,   429,
       0,   430,     0,   431,   432,   433,   434,   435,     0,     0,
       0,     0,     0,   436,   437,   438,   439,   440,     0,   441,
       0,     0,     0,     0,     0,     0,   442,   443,     0,     0,
     444,     0,   445,   446,     0,     0,   447,     0,     8,   448,
     449,     0,   450,   451,     0,     0,   452,   453,     0,     0,
       0,     0,     0,   454,     0,     0,   455,   310,   311,   312,
       0,   314,   315,   316,   317,   318,   456,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,     0,   332,
     333,   334,     0,   399,   337,   338,   339,   340,   457,   458,
     459,   460,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   461,   462,     0,     0,     0,     0,     0,
     400,   401,     0,     0,     0,     0,     0,     0,   463,   464,
     465,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   466,   467,   468,   469,
     470,     0,   471,     0,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    59,   483,   484,   485,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,     0,     0,     0,   403,     0,     0,     0,   486,
     487,   488,     0,    13,     0,     0,   489,   490,     0,     0,
       0,     0,     0,     0,   491,     0,   492,     0,   493,   494,
     495,   496,  1230,   425,   426,     0,     0,     0,     0,     0,
       0,     0,     0,   431,   432,   433,   434,   435,     0,     0,
       0,     0,     0,   436,     0,   438,     0,     0,     0,   441,
       0,   399,     0,     0,   404,     0,     0,   443,   405,     0,
    1195,   406,     0,   446,     0,     0,   447,     0,     0,   448,
       0,   866,     0,   451,     0,     0,   407,     0,   400,   401,
       0,     0,     0,   555,     0,   408,   455,   310,   311,   312,
       0,   314,   315,   316,   317,   318,   456,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,     0,   332,
     333,   334,     0,     0,   337,   338,   339,   340,   457,   458,
     459,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   461,   462,     0,     0,     0,     0,   402,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   466,   467,   468,   469,
     470,     0,   471,   867,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   868,   556,   484,   485,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   404,     0,     0,     0,   405,     0,     0,   869,
     487,   488,     0,    13,     0,     0,   489,   490,   400,   401,
       0,     0,     0,     0,   870,     0,   871,     0,   493,   494,
       0,   496,     0,   408,   425,   426,     0,     0,     0,     0,
       0,     0,     0,     0,   431,   432,   433,   434,   435,     0,
       0,     0,     0,     0,   436,     0,   438,     0,     0,     0,
     441,     0,   399,     0,     0,     0,     0,     0,   443,     0,
       0,     0,     0,     0,   446,     0,     0,   447,     0,   402,
     448,     0,     0,   403,   451,     0,     0,     0,     0,   400,
     401,     0,     0,     0,   555,     0,     0,   455,   310,   311,
     312,     0,   314,   315,   316,   317,   318,   456,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,     0,
     332,   333,   334,     0,     0,   337,   338,   339,   340,   457,
     458,   459,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   404,     0,   461,   462,   405,     0,  1222,   406,
     402,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   407,     0,     0,     0,     0,    58,
       0,     0,     0,   408,     0,     0,     0,   466,   467,   468,
     469,   470,     0,   471,   867,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   868,   556,   484,   485,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   404,     0,     0,     0,   405,     0,     0,
     869,   487,   488,     0,    13,     0,     0,   489,   490,   400,
     401,     0,     0,     0,     0,   870,     0,   879,     0,   493,
     494,     0,   496,     0,   408,   425,   426,     0,     0,     0,
       0,     0,     0,     0,     0,   431,   432,   433,   434,   435,
       0,     0,     0,     0,     0,   436,     0,   438,     0,     0,
       0,   441,     0,   585,     0,     0,     0,     0,     0,   443,
       0,     0,     0,     0,     0,   446,     0,     0,   447,     0,
     402,   448,     0,     0,   403,   451,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   555,     0,     0,   455,   310,
     311,   312,     0,   314,   315,   316,   317,   318,   456,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
       0,   332,   333,   334,     0,     0,   337,   338,   339,   340,
     457,   458,   459,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   404,     0,   461,   462,   405,     0,  1223,
     406,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   407,     0,     0,     0,     0,
      58,     0,     0,     0,   408,     0,     0,     0,   466,   467,
     468,   469,   470,     0,   471,     0,   472,   473,   474,   475,
     476,   477,   478,   479,   480,   481,   482,    59,   556,   484,
     485,     0,     0,   399,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   586,     0,
       0,   486,   487,   488,     0,    13,     0,     0,   489,   490,
     400,   401,     0,     0,     0,     0,  1065,     0,   492,     0,
     493,   494,     0,   496,     0,   588,   425,   426,     0,     0,
       0,     0,     0,     0,     0,     0,   431,   432,   433,   434,
     435,     0,     0,     0,     0,     0,   436,     0,   438,     0,
       0,   399,   441,     0,     0,     0,     0,     0,     0,     0,
     443,     0,     0,     0,     0,     0,   446,     0,     0,   447,
       0,   402,   448,     0,     0,   403,   451,     0,   400,   401,
       0,     0,     0,     0,     0,     0,   555,     0,     0,   455,
     310,   311,   312,     0,   314,   315,   316,   317,   318,   456,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,     0,   332,   333,   334,     0,     0,   337,   338,   339,
     340,   457,   458,   459,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   404,     0,   461,   462,   405,   402,
    1282,   406,     0,   403,     0,   628,     0,     0,     0,     0,
       0,   463,   464,   465,     0,     0,   407,     0,     0,     0,
       0,    58,     0,     0,     0,   408,     0,     0,     0,   466,
     467,   468,   469,   470,     0,   471,     0,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,    59,   556,
     484,   485,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   404,     0,     0,     0,   405,     0,  1307,   406,
       0,     0,   486,   487,   488,     0,    13,     0,     0,   489,
     490,     0,     0,     0,   407,   425,   426,   491,     0,   492,
       0,   493,   494,   408,   496,   431,   432,   433,   434,   435,
       0,     0,     0,     0,     0,   436,     0,   438,     0,     0,
     399,   441,     0,     0,     0,     0,     0,     0,     0,   443,
       0,     0,     0,     0,     0,   446,     0,     0,   447,     0,
       0,   448,     0,     0,     0,   451,     0,   400,   401,     0,
       0,     0,     0,     0,     0,   555,     0,     0,   455,   310,
     311,   312,     0,   314,   315,   316,   317,   318,   456,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
       0,   332,   333,   334,     0,     0,   337,   338,   339,   340,
     457,   458,   459,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   461,   462,     0,   402,     0,
       0,     0,   403,     0,   656,     0,     0,     0,     0,     0,
     463,   464,   465,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   466,   467,
     468,   469,   470,     0,   471,     0,   472,   473,   474,   475,
     476,   477,   478,   479,   480,   481,   482,    59,   556,   484,
     485,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,     0,     0,     0,   405,     0,  1381,   406,     0,
       0,   486,   487,   488,     0,    13,     0,     0,   489,   490,
       0,     0,     0,   407,   425,   426,   491,     0,   492,     0,
     493,   494,   408,   496,   431,   432,   433,   434,   435,     0,
       0,     0,     0,     0,   436,  1368,   438,   439,     0,     0,
     441,     0,     0,     0,     0,     0,     0,     0,   443,     0,
       0,     0,     0,     0,   446,     0,     0,   447,     0,     0,
     448,   449,     0,     0,   451,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   555,     0,     0,   455,   310,   311,
     312,     0,   314,   315,   316,   317,   318,   456,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,     0,
     332,   333,   334,     0,     0,   337,   338,   339,   340,   457,
     458,   459,  1369,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   461,   462,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   466,   467,   468,
     469,   470,     0,   471,     0,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,    59,   556,   484,   485,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,   488,     0,    13,     0,     0,   489,   490,     0,
       0,     0,     0,   425,   426,   491,     0,   492,     0,   493,
     494,     0,   496,   431,   432,   433,   434,   435,     0,     0,
       0,     0,     0,   436,     0,   438,     0,     0,     0,   441,
       0,     0,     0,     0,     0,     0,     0,   443,     0,     0,
       0,     0,     0,   446,     0,     0,   447,     0,     0,   448,
       0,     0,     0,   451,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   555,     0,     0,   455,   310,   311,   312,
       0,   314,   315,   316,   317,   318,   456,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,     0,   332,
     333,   334,     0,     0,   337,   338,   339,   340,   457,   458,
     459,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   461,   462,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   463,   464,
     465,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   466,   467,   468,   469,
     470,     0,   471,     0,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    59,   556,   484,   485,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   486,
     487,   488,     0,    13,     0,     0,   489,   490,     0,     0,
       0,     0,   425,   426,   491,     0,   492,     0,   493,   494,
       0,   496,   431,   432,   433,   434,   435,     0,     0,     0,
       0,     0,   436,     0,   438,     0,     0,     0,   441,     0,
       0,     0,     0,     0,     0,     0,   443,     0,     0,     0,
       0,     0,   446,     0,     0,   447,     0,     0,   448,     0,
       0,     0,   451,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   555,     0,     0,   455,   310,   311,   312,     0,
     314,   315,   316,   317,   318,   456,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,     0,   332,   333,
     334,     0,     0,   337,   338,   339,   340,   457,   458,   459,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   461,   462,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   941,   942,   943,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   466,   467,   468,   469,   470,
       0,   471,     0,   472,   473,   474,   475,   476,   477,   478,
     479,   480,   481,   482,    59,   556,   484,   485,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
     488,     0,    13,     0,     0,   489,   490,     0,     0,     0,
       0,   425,   426,   491,     0,   492,     0,   493,   494,     0,
     496,   431,   432,   433,   434,   435,     0,     0,     0,     0,
       0,   436,     0,   438,     0,     0,     0,   441,     0,     0,
       0,     0,     0,     0,     0,   443,     0,     0,     0,     0,
       0,   446,     0,     0,   447,     0,     0,   448,     0,     0,
       0,   451,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   555,     0,     0,   455,   310,   311,   312,     0,   314,
     315,   316,   317,   318,   456,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,     0,   332,   333,   334,
       0,     0,   337,   338,   339,   340,   457,   458,   459,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   461,   462,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   964,   965,   966,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   466,   467,   468,   469,   470,     0,
     471,     0,   472,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   482,    59,   556,   484,   485,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   486,   487,   488,
       0,    13,     0,     0,   489,   490,     0,     0,     0,     0,
     425,   426,   491,     0,   492,     0,   493,   494,   689,   496,
     431,   432,   433,   434,   435,     0,     0,     0,     0,     0,
     436,     0,   438,     0,     0,     0,   441,     0,     0,     0,
       0,     0,     0,     0,   443,     0,     0,     0,     0,     0,
     446,     0,     0,   447,     0,     0,   448,     0,     0,     0,
     451,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     555,     0,     0,   455,   310,   311,   312,     0,   314,   315,
     316,   317,   318,   456,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,     0,   332,   333,   334,     0,
       0,   337,   338,   339,   340,   457,   458,   459,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     461,   462,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   466,   467,   468,   469,   470,     0,   471,
       0,   472,   473,   474,   475,   476,   477,   478,   479,   480,
     481,   482,    59,   556,   484,   485,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,   488,     0,
      13,     0,     0,   489,   490,     0,     0,     0,     0,   425,
     426,   491,   589,   492,     0,   493,   494,   689,   496,   431,
     432,   433,   434,   435,     0,     0,     0,     0,     0,   436,
       0,   438,     0,     0,     0,   441,     0,     0,     0,     0,
       0,     0,     0,   443,     0,     0,     0,     0,     0,   446,
       0,     0,   447,     0,     0,   448,     0,     0,     0,   451,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   555,
       0,     0,   455,   310,   311,   312,     0,   314,   315,   316,
     317,   318,   456,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,     0,   332,   333,   334,     0,     0,
     337,   338,   339,   340,   457,   458,   459,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   461,
     462,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   466,   467,   468,   469,   470,     0,   471,   867,
     472,   473,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   868,   556,   484,   485,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   486,   487,   488,     0,    13,
       0,     0,   489,   490,     0,     0,     0,     0,   425,   426,
     491,     0,   492,     0,   493,   494,   689,   496,   431,   432,
     433,   434,   435,     0,     0,     0,     0,     0,   436,     0,
     438,     0,     0,     0,   441,     0,     0,     0,     0,     0,
       0,     0,   443,     0,     0,     0,     0,     0,   446,     0,
       0,   447,     0,     0,   448,     0,     0,     0,   451,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   555,     0,
       0,   455,   310,   311,   312,     0,   314,   315,   316,   317,
     318,   456,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,     0,   332,   333,   334,     0,     0,   337,
     338,   339,   340,   457,   458,   459,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   461,   462,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   466,   467,   468,   469,   470,     0,   471,     0,   472,
     473,   474,   475,   476,   477,   478,   479,   480,   481,   482,
      59,   556,   484,   485,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,   488,     0,    13,     0,
       0,   489,   490,     0,     0,     0,     0,   425,   426,   491,
     804,   492,     0,   493,   494,     0,   496,   431,   432,   433,
     434,   435,     0,     0,     0,     0,     0,   436,     0,   438,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,   443,     0,     0,     0,     0,     0,   446,     0,     0,
     447,     0,     0,   448,     0,     0,     0,   451,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   555,     0,     0,
     455,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     456,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   457,   458,   459,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   461,   462,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     466,   467,   468,   469,   470,     0,   471,     0,   472,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   482,    59,
     556,   484,   485,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   486,   487,   488,     0,    13,     0,     0,
     489,   490,     0,     0,     0,     0,   425,   426,   491,   589,
     492,     0,   493,   494,     0,   496,   431,   432,   433,   434,
     435,     0,     0,     0,     0,     0,   436,     0,   438,     0,
       0,     0,   441,     0,     0,     0,     0,     0,     0,     0,
     443,     0,     0,     0,     0,     0,   446,     0,     0,   447,
       0,     0,   448,     0,     0,     0,   451,     0,     0,     0,
       0,     0,   636,     0,     0,     0,   555,     0,     0,   455,
     310,   311,   312,     0,   314,   315,   316,   317,   318,   456,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,     0,   332,   333,   334,     0,     0,   337,   338,   339,
     340,   457,   458,   459,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   461,   462,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   466,
     467,   468,   469,   470,     0,   471,     0,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,    59,   556,
     484,   485,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,   487,   488,     0,    13,     0,     0,   489,
     490,     0,     0,     0,     0,   425,   426,   491,     0,   492,
       0,   493,   494,     0,   496,   431,   432,   433,   434,   435,
       0,     0,     0,     0,     0,   436,     0,   438,     0,     0,
       0,   441,     0,     0,     0,     0,     0,     0,     0,   443,
       0,     0,     0,     0,     0,   446,     0,     0,   447,     0,
       0,   448,     0,     0,     0,   451,     0,     0,   647,     0,
       0,     0,     0,     0,     0,   555,     0,     0,   455,   310,
     311,   312,     0,   314,   315,   316,   317,   318,   456,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
       0,   332,   333,   334,     0,     0,   337,   338,   339,   340,
     457,   458,   459,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   461,   462,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   466,   467,
     468,   469,   470,     0,   471,     0,   472,   473,   474,   475,
     476,   477,   478,   479,   480,   481,   482,    59,   556,   484,
     485,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   486,   487,   488,     0,    13,     0,     0,   489,   490,
       0,     0,     0,     0,   425,   426,   491,     0,   492,     0,
     493,   494,     0,   496,   431,   432,   433,   434,   435,     0,
       0,     0,     0,     0,   436,     0,   438,     0,     0,     0,
     441,     0,     0,     0,     0,     0,     0,     0,   443,     0,
       0,     0,     0,     0,   446,     0,     0,   447,     0,     0,
     448,     0,     0,     0,   451,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   555,     0,     0,   455,   310,   311,
     312,     0,   314,   315,   316,   317,   318,   456,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,     0,
     332,   333,   334,     0,     0,   337,   338,   339,   340,   457,
     458,   459,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   461,   462,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   466,   467,   468,
     469,   470,     0,   471,     0,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,    59,   556,   484,   485,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   680,     0,
     486,   487,   488,     0,    13,     0,     0,   489,   490,     0,
       0,     0,     0,   425,   426,   491,     0,   492,     0,   493,
     494,   689,   496,   431,   432,   433,   434,   435,     0,     0,
       0,     0,     0,   436,     0,   438,     0,     0,     0,   441,
       0,     0,     0,     0,     0,     0,     0,   443,     0,     0,
       0,     0,     0,   446,     0,     0,   447,     0,     0,   448,
       0,     0,     0,   451,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   555,     0,     0,   455,   310,   311,   312,
       0,   314,   315,   316,   317,   318,   456,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,     0,   332,
     333,   334,     0,     0,   337,   338,   339,   340,   457,   458,
     459,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   461,   462,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   466,   467,   468,   469,
     470,     0,   471,     0,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    59,   556,   484,   485,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   486,
     487,   488,     0,    13,     0,     0,   489,   490,     0,     0,
       0,     0,   425,   426,   491,     0,   492,     0,   493,   494,
       0,   496,   431,   432,   433,   434,   435,     0,     0,     0,
       0,     0,   436,     0,   438,     0,     0,     0,   441,     0,
       0,     0,     0,     0,     0,     0,   443,     0,     0,     0,
       0,     0,   446,     0,     0,   447,     0,     0,   448,     0,
       0,     0,   451,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   555,     0,     0,   455,   310,   311,   312,     0,
     314,   315,   316,   317,   318,   456,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,     0,   332,   333,
     334,     0,     0,   337,   338,   339,   340,   457,   458,   459,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   461,   462,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   466,   467,   468,   469,   470,
       0,   471,     0,   472,   473,   474,   475,   476,   477,   478,
     479,   480,   481,   482,    59,   556,   484,   485,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
     488,     0,    13,     0,     0,   489,   490,     0,     0,     0,
       0,   425,   426,   907,     0,   492,   908,   493,   494,     0,
     496,   431,   432,   433,   434,   435,     0,     0,   968,     0,
       0,   436,     0,   438,     0,     0,     0,   441,     0,     0,
       0,     0,     0,     0,     0,   443,     0,     0,     0,     0,
       0,   446,     0,     0,   447,     0,     0,   448,     0,     0,
       0,   451,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   555,     0,     0,   455,   310,   311,   312,     0,   314,
     315,   316,   317,   318,   456,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,     0,   332,   333,   334,
       0,     0,   337,   338,   339,   340,   457,   458,   459,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   461,   462,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   466,   467,   468,   469,   470,     0,
     471,     0,   472,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   482,    59,   556,   484,   485,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   486,   487,   488,
       0,    13,     0,     0,   489,   490,     0,     0,     0,     0,
     425,   426,   491,     0,   492,     0,   493,   494,     0,   496,
     431,   432,   433,   434,   435,     0,     0,     0,     0,     0,
     436,     0,   438,     0,     0,     0,   441,     0,     0,     0,
       0,     0,     0,     0,   443,     0,     0,     0,     0,     0,
     446,     0,     0,   447,     0,     0,   448,     0,     0,     0,
     451,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     555,     0,     0,   455,   310,   311,   312,     0,   314,   315,
     316,   317,   318,   456,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,     0,   332,   333,   334,     0,
       0,   337,   338,   339,   340,   457,   458,   459,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     461,   462,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   466,   467,   468,   469,   470,     0,   471,
       0,   472,   473,   474,   475,   476,   477,   478,   479,   480,
     481,   482,    59,   556,   484,   485,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,   488,     0,
      13,     0,     0,   489,   490,     0,     0,     0,     0,   425,
     426,   491,     0,   492,   988,   493,   494,     0,   496,   431,
     432,   433,   434,   435,     0,     0,     0,     0,     0,   436,
       0,   438,     0,     0,     0,   441,     0,     0,     0,     0,
       0,     0,     0,   443,     0,     0,     0,     0,     0,   446,
       0,     0,   447,     0,     0,   448,     0,     0,     0,   451,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   555,
       0,     0,   455,   310,   311,   312,     0,   314,   315,   316,
     317,   318,   456,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,     0,   332,   333,   334,     0,     0,
     337,   338,   339,   340,   457,   458,   459,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   461,
     462,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   466,   467,   468,   469,   470,     0,   471,     0,
     472,   473,   474,   475,   476,   477,   478,   479,   480,   481,
     482,    59,   556,   484,   485,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   486,   487,   488,     0,    13,
       0,     0,   489,   490,     0,     0,     0,     0,   425,   426,
     491,     0,   492,  1028,   493,   494,     0,   496,   431,   432,
     433,   434,   435,     0,     0,     0,     0,     0,   436,     0,
     438,     0,     0,     0,   441,     0,     0,     0,     0,     0,
       0,     0,   443,     0,     0,     0,     0,     0,   446,     0,
       0,   447,     0,     0,   448,     0,     0,     0,   451,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   555,     0,
       0,   455,   310,   311,   312,     0,   314,   315,   316,   317,
     318,   456,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,     0,   332,   333,   334,     0,     0,   337,
     338,   339,   340,   457,   458,   459,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   461,   462,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   466,   467,   468,   469,   470,     0,   471,     0,   472,
     473,   474,   475,   476,   477,   478,   479,   480,   481,   482,
      59,   556,   484,   485,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1030,     0,   486,   487,   488,     0,    13,     0,
       0,   489,   490,     0,     0,     0,     0,   425,   426,   491,
       0,   492,     0,   493,   494,     0,   496,   431,   432,   433,
     434,   435,     0,     0,     0,     0,     0,   436,     0,   438,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,   443,     0,     0,     0,     0,     0,   446,     0,     0,
     447,     0,     0,   448,     0,     0,     0,   451,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   555,     0,     0,
     455,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     456,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   457,   458,   459,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   461,   462,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     466,   467,   468,   469,   470,     0,   471,     0,   472,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   482,    59,
     556,   484,   485,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   486,   487,   488,     0,    13,     0,     0,
     489,   490,     0,     0,     0,     0,   425,   426,   491,     0,
     492,  1067,   493,   494,     0,   496,   431,   432,   433,   434,
     435,     0,     0,     0,     0,     0,   436,     0,   438,     0,
       0,     0,   441,     0,     0,     0,     0,     0,     0,     0,
     443,     0,     0,     0,     0,     0,   446,     0,     0,   447,
       0,     0,   448,     0,     0,     0,   451,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   555,     0,     0,   455,
     310,   311,   312,     0,   314,   315,   316,   317,   318,   456,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,     0,   332,   333,   334,     0,     0,   337,   338,   339,
     340,   457,   458,   459,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   461,   462,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   466,
     467,   468,   469,   470,     0,   471,     0,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,    59,   556,
     484,   485,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,   487,   488,     0,    13,     0,     0,   489,
     490,     0,     0,     0,     0,   425,   426,   491,     0,   492,
    1082,   493,   494,     0,   496,   431,   432,   433,   434,   435,
       0,     0,     0,     0,     0,   436,     0,   438,     0,     0,
       0,   441,     0,     0,     0,     0,     0,     0,     0,   443,
       0,     0,     0,     0,     0,   446,     0,     0,   447,     0,
       0,   448,     0,     0,     0,   451,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   555,     0,     0,   455,   310,
     311,   312,     0,   314,   315,   316,   317,   318,   456,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
       0,   332,   333,   334,     0,     0,   337,   338,   339,   340,
     457,   458,   459,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   461,   462,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   466,   467,
     468,   469,   470,     0,   471,     0,   472,   473,   474,   475,
     476,   477,   478,   479,   480,   481,   482,    59,   556,   484,
     485,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   486,   487,   488,     0,    13,     0,     0,   489,   490,
       0,     0,     0,     0,   425,   426,   491,     0,   492,  1250,
     493,   494,     0,   496,   431,   432,   433,   434,   435,     0,
       0,     0,     0,     0,   436,     0,   438,     0,     0,     0,
     441,     0,     0,     0,     0,     0,     0,     0,   443,     0,
       0,     0,     0,     0,   446,     0,     0,   447,     0,     0,
     448,     0,     0,     0,   451,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   555,     0,     0,   455,   310,   311,
     312,     0,   314,   315,   316,   317,   318,   456,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,     0,
     332,   333,   334,     0,     0,   337,   338,   339,   340,   457,
     458,   459,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   461,   462,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   466,   467,   468,
     469,   470,     0,   471,     0,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,    59,   556,   484,   485,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,   488,     0,    13,     0,     0,   489,   490,     0,
       0,     0,     0,   425,   426,   491,     0,   492,  1259,   493,
     494,     0,   496,   431,   432,   433,   434,   435,     0,     0,
       0,     0,     0,   436,     0,   438,     0,     0,     0,   441,
       0,     0,     0,     0,     0,     0,     0,   443,     0,     0,
       0,     0,     0,   446,     0,     0,   447,     0,     0,   448,
       0,     0,     0,   451,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   555,     0,     0,   455,   310,   311,   312,
       0,   314,   315,   316,   317,   318,   456,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,     0,   332,
     333,   334,     0,     0,   337,   338,   339,   340,   457,   458,
     459,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   461,   462,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   466,   467,   468,   469,
     470,     0,   471,     0,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    59,   556,   484,   485,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   486,
     487,   488,     0,    13,     0,     0,   489,   490,     0,     0,
       0,     0,   425,   426,   491,     0,   492,  1290,   493,   494,
       0,   496,   431,   432,   433,   434,   435,     0,     0,     0,
       0,     0,   436,     0,   438,     0,     0,     0,   441,     0,
       0,     0,     0,     0,     0,     0,   443,     0,     0,     0,
       0,     0,   446,     0,     0,   447,     0,     0,   448,     0,
       0,     0,   451,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   555,     0,     0,   455,   310,   311,   312,     0,
     314,   315,   316,   317,   318,   456,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,     0,   332,   333,
     334,     0,     0,   337,   338,   339,   340,   457,   458,   459,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   461,   462,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   466,   467,   468,   469,   470,
       0,   471,     0,   472,   473,   474,   475,   476,   477,   478,
     479,   480,   481,   482,    59,   556,   484,   485,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
     488,     0,    13,     0,     0,   489,   490,     0,     0,     0,
       0,   425,   426,   491,     0,   492,  1349,   493,   494,     0,
     496,   431,   432,   433,   434,   435,     0,     0,     0,     0,
       0,   436,     0,   438,     0,     0,     0,   441,     0,     0,
       0,     0,     0,     0,     0,   443,     0,     0,     0,     0,
       0,   446,     0,     0,   447,     0,     0,   448,     0,     0,
       0,   451,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   555,     0,     0,   455,   310,   311,   312,     0,   314,
     315,   316,   317,   318,   456,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,     0,   332,   333,   334,
       0,     0,   337,   338,   339,   340,   457,   458,   459,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   461,   462,     0,     0,     0,     0,     0,     0,     0,
    1379,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   466,   467,   468,   469,   470,     0,
     471,     0,   472,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   482,    59,   556,   484,   485,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   486,   487,   488,
       0,    13,     0,     0,   489,   490,     0,     0,     0,     0,
     425,   426,   491,     0,   492,     0,   493,   494,     0,   496,
     431,   432,   433,   434,   435,     0,     0,     0,     0,     0,
     436,     0,   438,     0,     0,     0,   441,     0,     0,     0,
       0,     0,     0,     0,   443,     0,     0,     0,     0,     0,
     446,     0,     0,   447,     0,     0,   448,     0,     0,     0,
     451,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     555,     0,     0,   455,   310,   311,   312,     0,   314,   315,
     316,   317,   318,   456,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,     0,   332,   333,   334,     0,
       0,   337,   338,   339,   340,   457,   458,   459,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     461,   462,     0,     0,     0,     0,     0,     0,     0,  1409,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   466,   467,   468,   469,   470,     0,   471,
       0,   472,   473,   474,   475,   476,   477,   478,   479,   480,
     481,   482,    59,   556,   484,   485,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,   488,     0,
      13,     0,     0,   489,   490,     0,     0,     0,     0,   425,
     426,   491,     0,   492,     0,   493,   494,     0,   496,   431,
     432,   433,   434,   435,     0,     0,     0,     0,     0,   436,
       0,   438,     0,     0,     0,   441,     0,     0,     0,     0,
       0,     0,     0,   443,     0,     0,     0,     0,     0,   446,
       0,     0,   447,     0,     0,   448,     0,     0,     0,   451,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   555,
       0,     0,   455,   310,   311,   312,     0,   314,   315,   316,
     317,   318,   456,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,     0,   332,   333,   334,     0,     0,
     337,   338,   339,   340,   457,   458,   459,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   461,
     462,     0,     0,     0,     0,     0,     0,     0,  1410,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   466,   467,   468,   469,   470,     0,   471,     0,
     472,   473,   474,   475,   476,   477,   478,   479,   480,   481,
     482,    59,   556,   484,   485,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   486,   487,   488,     0,    13,
       0,     0,   489,   490,     0,     0,     0,     0,   425,   426,
     491,     0,   492,     0,   493,   494,     0,   496,   431,   432,
     433,   434,   435,     0,     0,     0,     0,     0,   436,     0,
     438,     0,     0,     0,   441,     0,     0,     0,     0,     0,
       0,     0,   443,     0,     0,     0,     0,     0,   446,     0,
       0,   447,     0,     0,   448,     0,     0,     0,   451,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   555,     0,
       0,   455,   310,   311,   312,     0,   314,   315,   316,   317,
     318,   456,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,     0,   332,   333,   334,     0,     0,   337,
     338,   339,   340,   457,   458,   459,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   461,   462,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   466,   467,   468,   469,   470,     0,   471,     0,   472,
     473,   474,   475,   476,   477,   478,   479,   480,   481,   482,
      59,   556,   484,   485,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,   488,     0,    13,     0,
       0,   489,   490,     0,     0,     0,     0,   425,   426,   491,
       0,   492,     0,   493,   494,     0,   496,   431,   432,   433,
     434,   435,     0,     0,     0,     0,     0,   436,     0,   438,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,   443,     0,     0,     0,     0,     0,   446,     0,     0,
     447,     0,     0,   448,     0,     0,     0,   451,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   555,     0,     0,
     455,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     456,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   457,   458,   459,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   461,   462,     0,
       0,   -68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   713,   714,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     466,   467,   468,   469,   470,     0,   471,     0,   472,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   482,    59,
     556,   484,   485,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   486,   487,   488,     0,    13,     0,     0,
     489,   490,     0,     0,     0,     0,     0,     0,  1236,     0,
     492,     0,   493,   494,     0,   496,   715,   716,   717,   718,
     719,   720,   721,   722,   723,   724,   725,   726,   727,   728,
     729,   730,   731,   732,   733,   734,   735,   736,   737,   738,
       0,     0,     0,     0,   739,   740,   741,   742,   743,   744,
       0,     0,   745,   746,   747,   748,   749,   750,   751,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   752,   753,     0,   754,   755,   756,
     757,   758,   759,   760,   761,   762,   763,   304,     0,     0,
     713,   714,     0,   305,     0,     0,   764,   765,     0,   306,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   307,
       0,     0,     0,     0,     0,     0,     0,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   309,     0,     0,     0,     0,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
       0,     0,     0,     0,   715,   716,   717,   718,   719,   720,
     721,   722,   723,   724,   725,   726,   727,   728,   729,   730,
     731,   732,   733,   834,   735,   736,   737,   738,   713,   714,
       0,     0,   739,   740,   741,   742,   743,   744,    58,     0,
     745,   746,   747,   748,   749,   750,   751,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   596,     0,   713,   714,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   835,   753,     0,   754,   755,   756,   757,   758,
     759,   760,   761,   762,   763,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   764,   765,     0,     0,   344,     0,
     597,   999,   715,   716,   717,   718,   719,   720,   721,   722,
     723,   724,   725,   726,   727,   728,   729,   730,   731,   732,
     733,   834,   735,   736,   737,   738,     0,     0,     0,     0,
     739,   740,   741,   742,   743,   744,     0,     0,   745,   746,
     747,   748,   749,   750,   751,     0,     0,     0,     0,   723,
     724,   725,   726,   727,     0,     0,   730,   731,   732,   733,
       0,   735,   736,   737,   738,     0,     0,     0,     0,   739,
       0,   741,   742,     0,     0,     0,     0,     0,     0,     0,
     835,   753,     0,   754,   755,   756,   757,   758,   759,   760,
     761,   762,   763,   304,     0,     0,     0,     0,     0,   305,
       0,     0,   764,   765,     0,   306,     0,     0,     0,  1158,
       0,     0,     0,     0,     0,   307,     0,     0,     0,     0,
       0,     0,     0,   308,   756,   757,   758,   759,   760,   761,
     762,   763,     0,     0,     0,     0,     0,     0,   309,     0,
       0,   764,   765,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   713,   714,     0,     0,
       0,     0,     0,   304,     0,     0,     0,     0,     0,   305,
       0,     0,     0,     0,     0,   306,     0,     0,     0,     0,
       0,     0,     0,     0,    58,   307,     0,     0,     0,     0,
       0,     0,     0,   308,   713,   714,     0,   343,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   309,     0,
       0,    59,     0,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,     0,     0,   723,   724,
     725,   726,   727,     0,   344,   730,   731,   732,   733,     0,
     735,   736,   737,   738,     0,     0,     0,     0,   739,     0,
     741,   742,     0,     0,     0,     0,   745,   746,   747,     0,
       0,     0,   751,     0,    58,     0,   723,   724,   725,   726,
     727,     0,     0,   730,   731,   732,   733,   343,   735,   736,
     737,   738,   713,   714,     0,     0,   739,     0,   741,   742,
       0,   596,     0,     0,   745,   746,   747,     0,     0,   753,
     751,   754,   755,   756,   757,   758,   759,   760,   761,   762,
     763,     0,     0,     0,     0,     0,     0,   713,   714,     0,
     764,   765,     0,     0,     0,     0,     0,  1150,     0,     0,
       0,     0,     0,     0,   344,     0,     0,   753,     0,   754,
     755,   756,   757,   758,   759,   760,   761,   762,   763,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   764,   765,
       0,     0,     0,     0,     0,  1399,     0,     0,     0,     0,
       0,     0,     0,     0,   723,   724,   725,   726,   727,     0,
       0,   730,   731,   732,   733,     0,   735,   736,   737,   738,
       0,     0,     0,     0,   739,     0,   741,   742,     0,     0,
       0,     0,   745,   746,   747,     0,     0,     0,   751,   723,
     724,   725,   726,   727,     0,     0,   730,   731,   732,   733,
       0,   735,   736,   737,   738,   713,   714,     0,     0,   739,
       0,   741,   742,     0,     0,     0,     0,   745,   746,   747,
       0,     0,     0,   751,     0,   753,     0,   754,   755,   756,
     757,   758,   759,   760,   761,   762,   763,     0,     0,     0,
     713,   714,     0,     0,     0,     0,   764,   765,     0,     0,
       0,     0,     0,  1449,     0,     0,     0,     0,     0,     0,
     753,     0,   754,   755,   756,   757,   758,   759,   760,   761,
     762,   763,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   764,   765,     0,     0,     0,     0,     0,  1451,     0,
       0,     0,     0,     0,     0,     0,     0,   723,   724,   725,
     726,   727,     0,     0,   730,   731,   732,   733,     0,   735,
     736,   737,   738,     0,     0,     0,     0,   739,     0,   741,
     742,     0,     0,     0,     0,   745,   746,   747,     0,     0,
       0,   751,   723,   724,   725,   726,   727,   713,   714,   730,
     731,   732,   733,     0,   735,   736,   737,   738,     0,     0,
       0,     0,   739,     0,   741,   742,     0,     0,     0,     0,
     745,   746,   747,     0,     0,     0,   751,     0,   753,     0,
     754,   755,   756,   757,   758,   759,   760,   761,   762,   763,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   764,
     765,     0,     0,   775,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   753,     0,   754,   755,   756,   757,   758,
     759,   760,   761,   762,   763,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   764,   765,     0,     0,   785,   723,
     724,   725,   726,   727,   713,   714,   730,   731,   732,   733,
       0,   735,   736,   737,   738,     0,     0,     0,     0,   739,
       0,   741,   742,     0,     0,     0,     0,   745,   746,   747,
       0,     0,     0,   751,     0,     0,     0,     0,     0,   713,
     714,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     753,     0,   754,   755,   756,   757,   758,   759,   760,   761,
     762,   763,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   764,   765,     0,     0,   801,   723,   724,   725,   726,
     727,     0,     0,   730,   731,   732,   733,     0,   735,   736,
     737,   738,     0,     0,     0,     0,   739,     0,   741,   742,
       0,     0,     0,     0,   745,   746,   747,     0,     0,     0,
     751,   723,   724,   725,   726,   727,   713,   714,   730,   731,
     732,   733,     0,   735,   736,   737,   738,     0,     0,     0,
       0,   739,     0,   741,   742,     0,     0,     0,     0,   745,
     746,   747,     0,     0,     0,   751,     0,   753,     0,   754,
     755,   756,   757,   758,   759,   760,   761,   762,   763,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   764,   765,
       0,     0,   814,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   753,     0,   754,   755,   756,   757,   758,   759,
     760,   761,   762,   763,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   764,   765,     0,     0,   896,   723,   724,
     725,   726,   727,   713,   714,   730,   731,   732,   733,     0,
     735,   736,   737,   738,     0,     0,     0,     0,   739,     0,
     741,   742,     0,     0,     0,     0,   745,   746,   747,     0,
       0,     0,   751,     0,     0,     0,     0,     0,   713,   714,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   753,
       0,   754,   755,   756,   757,   758,   759,   760,   761,   762,
     763,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     764,   765,     0,     0,  1021,   723,   724,   725,   726,   727,
       0,     0,   730,   731,   732,   733,     0,   735,   736,   737,
     738,     0,     0,     0,     0,   739,     0,   741,   742,     0,
       0,     0,     0,   745,   746,   747,     0,     0,     0,   751,
     723,   724,   725,   726,   727,   713,   714,   730,   731,   732,
     733,     0,   735,   736,   737,   738,     0,     0,     0,     0,
     739,     0,   741,   742,     0,     0,     0,     0,   745,   746,
     747,     0,     0,     0,   751,     0,   753,     0,   754,   755,
     756,   757,   758,   759,   760,   761,   762,   763,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   764,   765,     0,
       0,  1041,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   753,     0,   754,   755,   756,   757,   758,   759,   760,
     761,   762,   763,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   764,   765,     0,     0,  1045,   723,   724,   725,
     726,   727,   713,   714,   730,   731,   732,   733,     0,   735,
     736,   737,   738,     0,     0,     0,     0,   739,     0,   741,
     742,     0,     0,     0,     0,   745,   746,   747,     0,     0,
       0,   751,     0,     0,     0,     0,     0,   713,   714,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   753,     0,
     754,   755,   756,   757,   758,   759,   760,   761,   762,   763,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   764,
     765,     0,     0,  1047,   723,   724,   725,   726,   727,     0,
       0,   730,   731,   732,   733,     0,   735,   736,   737,   738,
       0,     0,     0,     0,   739,     0,   741,   742,     0,     0,
       0,     0,   745,   746,   747,     0,     0,     0,   751,   723,
     724,   725,   726,   727,   713,   714,   730,   731,   732,   733,
       0,   735,   736,   737,   738,     0,     0,     0,     0,   739,
       0,   741,   742,     0,     0,     0,     0,   745,   746,   747,
       0,     0,     0,   751,     0,   753,     0,   754,   755,   756,
     757,   758,   759,   760,   761,   762,   763,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   764,   765,     0,     0,
    1054,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     753,     0,   754,   755,   756,   757,   758,   759,   760,   761,
     762,   763,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   764,   765,     0,     0,  1055,   723,   724,   725,   726,
     727,   713,   714,   730,   731,   732,   733,     0,   735,   736,
     737,   738,     0,     0,     0,     0,   739,     0,   741,   742,
       0,     0,     0,     0,   745,   746,   747,     0,     0,     0,
     751,     0,     0,     0,     0,     0,   713,   714,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   753,     0,   754,
     755,   756,   757,   758,   759,   760,   761,   762,   763,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   764,   765,
       0,     0,  1056,   723,   724,   725,   726,   727,     0,     0,
     730,   731,   732,   733,     0,   735,   736,   737,   738,     0,
       0,     0,     0,   739,     0,   741,   742,     0,     0,     0,
       0,   745,   746,   747,     0,     0,     0,   751,   723,   724,
     725,   726,   727,   713,   714,   730,   731,   732,   733,     0,
     735,   736,   737,   738,     0,     0,     0,     0,   739,     0,
     741,   742,     0,     0,     0,     0,   745,   746,   747,     0,
       0,     0,   751,     0,   753,     0,   754,   755,   756,   757,
     758,   759,   760,   761,   762,   763,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   764,   765,     0,     0,  1057,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   753,
       0,   754,   755,   756,   757,   758,   759,   760,   761,   762,
     763,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     764,   765,     0,     0,  1058,   723,   724,   725,   726,   727,
     713,   714,   730,   731,   732,   733,     0,   735,   736,   737,
     738,     0,     0,     0,     0,   739,     0,   741,   742,     0,
       0,     0,     0,   745,   746,   747,     0,     0,     0,   751,
       0,     0,     0,     0,     0,   713,   714,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   753,     0,   754,   755,
     756,   757,   758,   759,   760,   761,   762,   763,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   764,   765,     0,
       0,  1059,   723,   724,   725,   726,   727,     0,     0,   730,
     731,   732,   733,     0,   735,   736,   737,   738,     0,     0,
       0,     0,   739,     0,   741,   742,     0,     0,     0,     0,
     745,   746,   747,     0,     0,     0,   751,   723,   724,   725,
     726,   727,   713,   714,   730,   731,   732,   733,     0,   735,
     736,   737,   738,     0,     0,     0,     0,   739,     0,   741,
     742,     0,     0,     0,     0,   745,   746,   747,     0,     0,
       0,   751,     0,   753,     0,   754,   755,   756,   757,   758,
     759,   760,   761,   762,   763,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   764,   765,     0,     0,  1185,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   753,     0,
     754,   755,   756,   757,   758,   759,   760,   761,   762,   763,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   764,
     765,     0,     0,  1293,   723,   724,   725,   726,   727,   713,
     714,   730,   731,   732,   733,     0,   735,   736,   737,   738,
       0,     0,     0,     0,   739,     0,   741,   742,     0,     0,
       0,     0,   745,   746,   747,     0,     0,     0,   751,     0,
       0,     0,     0,     0,   713,   714,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   753,     0,   754,   755,   756,
     757,   758,   759,   760,   761,   762,   763,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   764,   765,     0,     0,
    1305,   723,   724,   725,   726,   727,     0,     0,   730,   731,
     732,   733,     0,   735,   736,   737,   738,     0,     0,     0,
       0,   739,     0,   741,   742,     0,     0,     0,     0,   745,
     746,   747,     0,     0,     0,   751,   723,   724,   725,   726,
     727,   713,   714,   730,   731,   732,   733,     0,   735,   736,
     737,   738,     0,     0,     0,     0,   739,     0,   741,   742,
       0,     0,     0,     0,   745,   746,   747,     0,     0,     0,
     751,     0,   753,     0,   754,   755,   756,   757,   758,   759,
     760,   761,   762,   763,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   764,   765,     0,     0,  1306,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   753,     0,   754,
     755,   756,   757,   758,   759,   760,   761,   762,   763,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   764,   765,
       0,     0,  1322,   723,   724,   725,   726,   727,   713,   714,
     730,   731,   732,   733,     0,   735,   736,   737,   738,     0,
       0,     0,     0,   739,     0,   741,   742,     0,     0,     0,
       0,   745,   746,   747,     0,     0,     0,   751,     0,     0,
       0,     0,     0,   713,   714,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   753,     0,   754,   755,   756,   757,
     758,   759,   760,   761,   762,   763,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   764,   765,     0,     0,  1324,
     723,   724,   725,   726,   727,     0,     0,   730,   731,   732,
     733,     0,   735,   736,   737,   738,     0,     0,     0,     0,
     739,     0,   741,   742,     0,     0,     0,     0,   745,   746,
     747,     0,     0,     0,   751,   723,   724,   725,   726,   727,
     713,   714,   730,   731,   732,   733,     0,   735,   736,   737,
     738,     0,     0,     0,     0,   739,     0,   741,   742,     0,
       0,     0,     0,   745,   746,   747,     0,     0,     0,   751,
       0,   753,     0,   754,   755,   756,   757,   758,   759,   760,
     761,   762,   763,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   764,   765,     0,     0,  1326,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   753,     0,   754,   755,
     756,   757,   758,   759,   760,   761,   762,   763,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   764,   765,     0,
       0,  1330,   723,   724,   725,   726,   727,   713,   714,   730,
     731,   732,   733,     0,   735,   736,   737,   738,     0,     0,
       0,     0,   739,     0,   741,   742,     0,     0,     0,     0,
     745,   746,   747,     0,     0,     0,   751,     0,     0,     0,
       0,     0,   713,   714,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   753,     0,   754,   755,   756,   757,   758,
     759,   760,   761,   762,   763,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   764,   765,     0,     0,  1384,   723,
     724,   725,   726,   727,     0,     0,   730,   731,   732,   733,
       0,   735,   736,   737,   738,     0,     0,     0,     0,   739,
       0,   741,   742,     0,     0,     0,     0,   745,   746,   747,
       0,     0,     0,   751,   723,   724,   725,   726,   727,   713,
     714,   730,   731,   732,   733,     0,   735,   736,   737,   738,
       0,     0,     0,     0,   739,     0,   741,   742,     0,     0,
       0,     0,   745,   746,   747,     0,     0,     0,   751,     0,
     753,     0,   754,   755,   756,   757,   758,   759,   760,   761,
     762,   763,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   764,   765,     0,     0,  1385,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   753,     0,   754,   755,   756,
     757,   758,   759,   760,   761,   762,   763,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   764,   765,     0,     0,
    1386,   723,   724,   725,   726,   727,   713,   714,   730,   731,
     732,   733,     0,   735,   736,   737,   738,     0,     0,     0,
       0,   739,     0,   741,   742,     0,     0,     0,     0,   745,
     746,   747,     0,     0,     0,   751,     0,     0,     0,     0,
       0,   713,   714,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   753,     0,   754,   755,   756,   757,   758,   759,
     760,   761,   762,   763,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   764,   765,     0,     0,  1392,   723,   724,
     725,   726,   727,     0,     0,   730,   731,   732,   733,     0,
     735,   736,   737,   738,     0,     0,     0,     0,   739,     0,
     741,   742,     0,     0,     0,     0,   745,   746,   747,     0,
       0,     0,   751,   723,   724,   725,   726,   727,   713,   714,
     730,   731,   732,   733,     0,   735,   736,   737,   738,     0,
       0,     0,     0,   739,     0,   741,   742,     0,     0,     0,
       0,   745,   746,   747,     0,     0,     0,   751,     0,   753,
       0,   754,   755,   756,   757,   758,   759,   760,   761,   762,
     763,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     764,   765,     0,     0,  1398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   753,     0,   754,   755,   756,   757,
     758,   759,   760,   761,   762,   763,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   764,   765,     0,     0,  1403,
     723,   724,   725,   726,   727,   713,   714,   730,   731,   732,
     733,     0,   735,   736,   737,   738,     0,     0,     0,     0,
     739,     0,   741,   742,     0,     0,     0,     0,   745,   746,
     747,     0,     0,     0,   751,     0,     0,     0,     0,     0,
     713,   714,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   753,     0,   754,   755,   756,   757,   758,   759,   760,
     761,   762,   763,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   764,   765,     0,     0,  1442,   723,   724,   725,
     726,   727,     0,     0,   730,   731,   732,   733,     0,   735,
     736,   737,   738,     0,     0,     0,     0,   739,     0,   741,
     742,     0,     0,     0,     0,   745,   746,   747,     0,     0,
       0,   751,   723,   724,   725,   726,   727,   713,   714,   730,
     731,   732,   733,     0,   735,   736,   737,   738,     0,     0,
       0,     0,   739,     0,   741,   742,     0,     0,     0,     0,
     745,   746,   747,     0,     0,     0,   751,     0,   753,     0,
     754,   755,   756,   757,   758,   759,   760,   761,   762,   763,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   764,
     765,   805,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   753,     0,   754,   755,   756,   757,   758,
     759,   760,   761,   762,   763,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   764,   765,  1129,     0,     0,   723,
     724,   725,   726,   727,   713,   714,   730,   731,   732,   733,
       0,   735,   736,   737,   738,     0,     0,     0,     0,   739,
       0,   741,   742,     0,     0,     0,     0,   745,   746,   747,
       0,     0,     0,   751,     0,     0,     0,     0,     0,   713,
     714,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     753,     0,   754,   755,   756,   757,   758,   759,   760,   761,
     762,   763,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   764,   765,  1147,     0,     0,   723,   724,   725,   726,
     727,     0,     0,   730,   731,   732,   733,     0,   735,   736,
     737,   738,     0,     0,     0,     0,   739,     0,   741,   742,
       0,     0,     0,     0,   745,   746,   747,     0,     0,     0,
     751,   723,   724,   725,   726,   727,     0,     0,   730,   731,
     732,   733,     0,   735,   736,   737,   738,   713,   714,     0,
       0,   739,     0,   741,   742,     0,     0,     0,     0,   745,
     746,   747,     0,     0,     0,   751,     0,   753,     0,   754,
     755,   756,   757,   758,   759,   760,   761,   762,   763,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   764,   765,
    1257,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   753,     0,   754,   755,   756,   757,   758,   759,
     760,   761,   762,   763,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   764,   765,  1262,     0,     0,     0,     0,
       0,   715,   716,   717,   718,   719,   720,   721,   722,   723,
     724,   725,   726,   727,   728,   729,   730,   731,   732,   733,
     834,   735,   736,   737,   738,  -334,     0,   229,   230,   739,
     740,   741,   742,   743,   744,     0,     0,   745,   746,   747,
     748,   749,   750,   751,   231,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   713,   714,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   835,
     753,     0,   754,   755,   756,   757,   758,   759,   760,   761,
     762,   763,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   764,   765,     0,     0,     0,     0,     0,     0,     0,
       0,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,     0,
       0,   250,   251,   252,     0,     0,     0,     0,     0,     0,
     253,   254,   255,   256,   257,     0,     0,   258,   259,   260,
     261,   262,   263,   264,   723,   724,   725,   726,   727,   713,
     714,   730,   731,   732,   733,     0,   735,   736,   737,   738,
       0,     0,     0,     0,   739,     0,   741,   742,     0,     0,
     882,     0,   745,   746,   747,     0,     0,     0,   751,     0,
     265,     0,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,     0,     0,   276,   277,     0,     0,     0,     0,
       0,   278,   279,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   753,     0,   754,   755,   756,
     757,   758,   759,   760,   761,   762,   763,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   764,   765,     0,     0,
       0,   723,   724,   725,   726,   727,   713,   714,   730,   731,
     732,   733,     0,   735,   736,   737,   738,     0,     0,     0,
       0,   739,     0,   741,   742,     0,     0,  1068,     0,   745,
     746,   747,     0,     0,     0,   751,     0,     0,     0,     0,
       0,   713,   714,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   753,     0,   754,   755,   756,   757,   758,   759,
     760,   761,   762,   763,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   764,   765,     0,     0,     0,   723,   724,
     725,   726,   727,     0,     0,   730,   731,   732,   733,     0,
     735,   736,   737,   738,     0,     0,     0,     0,   739,     0,
     741,   742,     0,     0,     0,     0,   745,   746,   747,     0,
       0,     0,   751,   723,   724,   725,   726,   727,   713,   714,
     730,   731,   732,   733,     0,   735,   736,   737,   738,     0,
       0,     0,     0,   739,     0,   741,   742,     0,     0,     0,
       0,   745,   746,   747,     0,     0,     0,   751,     0,   753,
    1137,   754,   755,   756,   757,   758,   759,   760,   761,   762,
     763,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     764,   765,     0,     0,     0,  1224,     0,     0,     0,     0,
       0,     0,     0,     0,   753,     0,   754,   755,   756,   757,
     758,   759,   760,   761,   762,   763,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   764,   765,     0,     0,     0,
     723,   724,   725,   726,   727,   713,   714,   730,   731,   732,
     733,     0,   735,   736,   737,   738,     0,     0,     0,     0,
     739,     0,   741,   742,     0,     0,     0,     0,   745,   746,
     747,     0,     0,     0,   751,     0,     0,     0,     0,     0,
     713,   714,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   753,     0,   754,   755,   756,   757,   758,   759,   760,
     761,   762,   763,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   764,   765,     0,     0,     0,   723,   724,   725,
     726,   727,     0,     0,   730,   731,   732,   733,     0,   735,
     736,   737,   738,     0,     0,     0,     0,   739,     0,   741,
     742,     0,     0,     0,     0,   745,   746,   747,     0,     0,
       0,  -701,   723,   724,   725,   726,   727,   713,   714,   730,
     731,   732,   733,     0,   735,   736,   737,   738,     0,     0,
       0,     0,   739,     0,   741,   742,     0,     0,     0,     0,
     745,   746,   747,     0,     0,     0,     0,     0,   753,     0,
     754,   755,   756,   757,   758,   759,   760,   761,   762,   763,
     713,   714,     0,     0,     0,     0,     0,     0,     0,   764,
     765,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   753,     0,   754,   755,   756,   757,   758,
     759,   760,   761,   762,   763,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   764,   765,     0,     0,     0,   723,
     724,   725,   726,   727,     0,     0,   730,   731,   732,   733,
       0,   735,   736,   737,   738,     0,     0,     0,     0,   739,
       0,   741,   742,     0,     0,     0,     0,   745,     0,   747,
       0,     0,     0,     0,     0,     0,     0,   713,   714,     0,
       0,     0,   723,   724,   725,   726,   727,     0,     0,   730,
     731,   732,   733,     0,   735,   736,   737,   738,     0,     0,
       0,     0,   739,     0,   741,   742,   713,   714,     0,     0,
       0,     0,   754,   755,   756,   757,   758,   759,   760,   761,
     762,   763,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   764,   765,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   754,   755,   756,   757,   758,
     759,   760,   761,   762,   763,     0,     0,     0,     0,   723,
     724,   725,   726,   727,   764,   765,   730,   731,   732,   733,
       0,   735,   736,   737,   738,     0,     0,     0,     0,   739,
       0,   741,   742,   713,   714,     0,     0,     0,   723,   724,
     725,   726,   727,     0,     0,   730,   731,   732,   733,     0,
     735,   736,   737,   738,     0,     0,     0,     0,   739,     0,
     741,   742,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   755,   756,   757,   758,   759,   760,   761,
     762,   763,     0,     0,     0,     0,     0,     0,     0,   916,
       0,   764,   765,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   757,   758,   759,   760,   761,   762,
     763,     0,     0,     0,     0,   723,   724,   725,   726,   727,
     764,   765,   730,     0,     0,   733,     0,   735,   736,   737,
     738,     0,     0,     0,     0,   739,     0,   741,   742,   310,
     311,   312,     0,   314,   315,   316,   317,   318,   456,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
       0,   332,   333,   334,     0,     0,   337,   338,   339,   340,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   920,     0,     0,     0,     0,     0,
       0,   757,   758,   759,   760,   761,   762,   763,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   764,   765,     0,
       0,     0,     0,     0,     0,  1133,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   917,     0,     0,     0,     0,
       0,     0,     0,     0,   310,   311,   312,   918,   314,   315,
     316,   317,   318,   456,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,     0,   332,   333,   334,     0,
       0,   337,   338,   339,   340,   310,   311,   312,     0,   314,
     315,   316,   317,   318,   456,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,     0,   332,   333,   334,
       0,     0,   337,   338,   339,   340,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   181,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     921,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   922,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   182,     0,   183,     0,   184,   185,   186,   187,
     188,  1134,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,  1135,   200,   201,   202,     0,     0,   203,
     204,   205,   206,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   207,   208,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     209
};

static const yytype_int16 yycheck[] =
{
      13,    14,   211,   225,   420,   381,   416,   560,   437,   369,
     635,   161,   676,   475,   580,  1071,   574,   437,   576,   579,
     578,   463,   464,   465,   376,   464,   465,     7,   858,   225,
      19,    19,    52,   409,   792,     0,   794,   907,   796,   529,
     460,     6,   874,     4,     5,    21,    32,   103,   880,    56,
      63,    64,    65,    49,   128,    62,    19,    19,   138,    14,
      15,    32,   552,    24,    29,    52,    31,    45,    33,    30,
      20,    21,    18,    19,    39,   122,   166,   166,   125,   126,
      56,   125,   126,   166,    49,   166,  1365,    14,    15,   123,
      55,   104,   105,   106,   107,   123,   181,   473,   474,   166,
     177,   125,   126,   180,   123,   133,    67,  1387,    49,   123,
     154,   154,    77,   181,   133,   140,   141,   142,   207,   133,
     139,   211,   166,   166,   139,   103,    46,   212,   211,  1408,
     211,   123,   212,   619,    99,   100,   213,   211,    99,   100,
    1420,   133,   209,   211,   564,   367,    66,   297,   182,  1205,
      91,   185,   148,   582,   182,    20,    21,   204,   205,   187,
     204,   205,   218,   182,   114,   115,   189,   154,   182,   184,
     166,   367,   122,   663,   124,   125,   126,   127,   139,   166,
     204,   205,   132,   208,   185,   381,   159,   673,   211,   675,
     182,   211,   181,   181,   614,   156,   157,   158,   627,   628,
    1070,   187,   151,   689,   159,   166,   213,   156,   628,   164,
     211,   166,   225,   409,   169,   411,   187,   193,   181,   181,
     166,   139,  1052,   652,   653,   205,   123,   188,   204,   194,
     177,   440,   159,   213,   205,  1067,   656,   164,  1294,   166,
     205,    18,   169,   214,   205,   454,   211,    24,   166,   114,
     115,   212,   207,    30,   204,   205,   181,   122,   634,   124,
     125,   126,   127,    40,   211,   123,   213,   132,   154,   711,
     148,    48,   711,   702,   212,   133,   151,   473,   474,   655,
     166,   156,   211,   208,   181,   182,    63,   184,   166,    61,
     187,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   182,   190,   191,   192,   193,   194,
     176,   344,   123,   208,   181,    56,   123,   766,   182,   204,
     205,    62,   133,   565,   566,   567,   133,   569,   570,     4,
       5,   573,     7,   575,   367,   577,   914,   709,   778,   372,
     178,  1119,   139,    33,   211,   211,  1236,   213,   381,   565,
     566,   567,  1204,   569,   570,   152,   182,   573,   189,   575,
      35,   577,   184,   579,  1216,   139,    20,    21,   206,   166,
     612,   182,    62,   211,   148,   182,   409,   826,   411,   153,
     211,   181,   818,  1067,   188,   211,    63,    64,    65,   618,
     212,   189,   166,   166,   167,   168,   612,   139,  1082,   181,
     433,   189,   884,   207,   866,   181,   148,   866,   178,   179,
     639,   211,   209,   211,   188,   184,   181,   214,   634,  1197,
     902,   194,   181,   211,   166,   821,   822,   104,   105,   106,
     107,  1017,   208,   829,   188,  1015,   206,   181,   181,   655,
     473,   474,   132,   208,   154,   170,   171,   843,   844,   139,
     846,  1229,   681,   207,   850,   188,   166,   853,   181,   181,
     114,   115,   181,   181,   208,   208,    20,    21,   122,   181,
     699,   125,   126,   127,   207,  1053,   166,   139,   132,   941,
     942,   943,   941,   942,   943,   208,   208,   181,   181,   208,
     208,   887,   181,   889,   181,   181,   208,   713,   714,   181,
     181,    32,   964,   965,   966,   964,   965,   966,   198,   148,
     181,   166,    56,   181,   208,   208,   181,   207,    62,   208,
    1098,   208,   208,   739,   181,   206,   208,   166,    59,    60,
     181,   181,   565,   566,   567,   206,   569,   570,   206,    56,
     573,   206,   575,  1188,   577,    62,   579,   207,    56,   206,
     204,   205,   212,   184,    62,   206,   206,  1039,   189,   184,
     114,   115,   184,   140,   189,   142,   808,   189,   122,   811,
     124,   125,   126,   127,    20,    21,   207,  1026,   132,   612,
     148,    56,   207,   825,   154,   207,   828,    62,   156,   120,
     154,   624,   808,   124,   184,   811,   166,   208,   166,   189,
     211,   634,   166,    11,   774,   821,   822,   836,   207,   825,
     159,  1060,   828,   829,    22,    23,   204,   207,    32,    56,
     181,   184,   655,   184,   187,    62,   187,   843,   844,    56,
     846,   166,  1081,   208,   850,    62,   211,   853,   192,   193,
     194,   170,  1038,   185,  1093,    59,    60,   189,   170,   171,
     204,   205,   183,  1337,  1338,   166,   187,   185,   189,   190,
     693,   189,   208,    32,  1348,   211,   112,   113,   114,   115,
     211,   887,   211,   889,   205,  1124,   122,   166,   124,   125,
     126,   127,   166,   214,   209,   210,   132,   212,   134,   135,
      59,    60,    72,  1142,   208,   185,    76,   211,  1343,   189,
     185,  1097,   166,   185,   189,   166,   120,   189,   185,   123,
     124,   166,   189,    86,    87,    95,    96,    97,    98,   133,
     185,  1101,   185,   185,   189,   185,   189,   189,   185,   189,
    1360,   212,   189,    56,  1364,   140,   141,   142,   771,    32,
     170,   171,   172,   173,   190,   191,   192,   193,   194,  1198,
     170,   120,   968,   211,   140,   124,   103,   156,   204,   205,
     182,  1003,   166,   167,   168,   212,    59,    60,   182,   183,
     212,   212,  1208,   187,    34,   808,   190,    34,   811,   170,
     171,   172,    10,    11,    12,   166,   207,  1003,   821,   822,
     213,   205,   825,   212,   166,   828,   829,   211,   166,  1015,
     214,   166,    21,   206,   166,  1378,   206,   184,   184,   207,
     843,   844,    42,   846,   183,   212,   212,   850,   187,   207,
     853,   190,  1038,   207,   188,   188,   166,   120,   188,   188,
     188,   124,   188,   188,   188,   188,   205,   188,   188,   185,
    1413,   207,    32,   133,   166,   214,   207,   166,   166,   206,
    1246,    42,   188,   188,   887,   207,   889,  1293,   207,   207,
     188,   166,   207,   188,   188,   207,   207,   211,   188,    59,
      60,   369,   159,  1102,   188,   188,   207,   207,   207,   188,
     378,  1097,    36,    32,   210,     9,   207,    65,    32,   387,
     183,   207,  1341,   188,   187,   207,   189,   190,  1347,   397,
     211,   211,    42,   211,   211,   211,  1336,   211,   211,   407,
      59,    60,   205,   205,  1363,    59,    60,   415,   211,  1368,
      32,   214,   207,   207,   213,   208,   166,   166,   206,   427,
     120,    32,   211,   213,   124,   211,   213,   184,   211,   437,
     166,   213,   166,   207,   442,   181,   444,    59,    60,   128,
      13,   182,  1181,   159,   210,   453,   181,   184,    59,    60,
    1409,   181,   460,   461,   462,   166,   212,  1403,   156,     7,
    1003,   120,   166,   189,   123,   124,   120,   475,    42,   208,
     124,  1377,  1015,   181,   133,   483,   207,     1,   486,   487,
     488,   489,   490,   183,   492,   181,   213,   187,   496,   189,
     190,   206,   213,  1232,   502,  1038,  1248,   207,   120,    32,
     207,   166,   124,   188,   207,   205,  1245,   188,   207,   120,
     207,   213,   207,   124,   214,   207,    32,   207,   211,    66,
    1246,   166,  1248,   182,   183,   166,    59,    60,   187,   183,
     166,   190,   189,   187,   189,   189,   190,   189,   207,   207,
     206,   166,   166,    59,    60,   206,   205,   166,   206,   166,
    1289,   205,   208,   188,  1097,   214,   564,   166,   207,   207,
     214,   183,    32,   207,   211,   187,   206,   189,   190,   207,
     207,   166,   183,   206,   206,  1327,   187,   207,   189,   190,
     211,   207,   189,   205,   207,   189,   207,   120,   208,   207,
      52,   124,   214,   207,   205,   207,   206,   213,   211,   607,
     208,  1327,   683,   214,   120,   206,   614,   206,   124,   206,
      79,     1,    44,   133,    82,  1309,    80,  1310,   223,   771,
     628,  1363,   904,  1310,  1310,  1310,  1310,   635,   636,     1,
     638,  1289,  1266,  1335,  1243,   643,  1269,   422,  1315,  1154,
      52,   649,   547,  1300,  1079,   676,    -1,   695,   656,   433,
     183,  1377,   433,    -1,   187,    -1,   189,   190,    -1,   667,
     668,   669,   670,   671,   672,    -1,   674,   183,   676,    -1,
      -1,   187,   205,   189,   190,    -1,    -1,    -1,    -1,    -1,
      -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,
      -1,    -1,  1441,  1246,    -1,  1248,    -1,   715,   716,   717,
     718,   719,   720,   721,   722,   723,   724,    -1,    -1,   727,
     728,   729,   730,   731,   732,    -1,   734,   735,    -1,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,   750,   751,   752,   753,   754,   755,   756,   757,
     758,   759,   760,   761,   762,   763,    -1,   765,    -1,    -1,
     768,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     778,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1327,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   827,
      -1,    -1,  1365,    -1,    -1,   833,   834,   835,    -1,    -1,
      -1,    -1,    -1,    -1,  1377,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    20,    21,    -1,    -1,    18,    -1,
      -1,   869,   870,   871,    24,  1408,    -1,    -1,    -1,    -1,
      30,   879,    -1,    -1,   882,    -1,   884,    -1,    -1,    -1,
      40,    -1,    -1,   891,    -1,    -1,   894,    -1,    48,    -1,
      -1,    -1,    -1,    -1,   902,    -1,    -1,    -1,   906,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,   120,   121,   122,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,   134,   135,
      -1,    -1,    -1,   981,   140,   141,   142,   985,    -1,   139,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1023,    -1,   183,    -1,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,    -1,
      -1,  1039,    -1,    -1,  1042,    -1,    -1,    -1,   204,   205,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,
      -1,    -1,    -1,    -1,  1062,  1063,  1064,    -1,    -1,    -1,
    1068,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1076,    -1,
      -1,  1079,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,  1095,  1096,    20,
      21,    -1,    18,  1101,    -1,    -1,    -1,  1105,    24,    -1,
      -1,  1109,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1121,    40,  1123,    -1,    -1,    -1,    -1,
    1128,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,  1137,
      -1,    -1,    -1,  1141,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,    -1,    -1,    -1,    -1,    -1,
    1188,   112,   113,   114,   115,   116,    -1,    -1,   119,   120,
     121,   122,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,   134,   135,    32,    -1,    -1,    -1,   140,
     141,   142,    -1,   139,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,   152,    -1,    -1,    -1,
      -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,    -1,    -1,    -1,    -1,  1254,    -1,  1256,    -1,
      -1,    -1,   183,  1261,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,    -1,    -1,    -1,    -1,    -1,  1277,
      -1,    -1,    -1,   204,   205,  1283,  1284,  1285,    -1,    -1,
    1288,   212,    -1,   209,  1292,    -1,    -1,    -1,  1296,  1297,
    1298,  1299,    -1,   120,  1302,    -1,  1304,   124,    -1,    -1,
      -1,    -1,  1310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1318,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,   120,   121,   122,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,  1343,   132,    -1,   134,   135,
      -1,    -1,    -1,    -1,   140,    -1,  1354,  1355,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
     187,  1369,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1379,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      -1,    -1,    -1,    -1,    -1,  1393,    -1,   214,    -1,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,    -1,
      -1,    -1,  1410,    -1,    -1,    -1,  1414,    -1,   204,   205,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1428,    -1,  1430,  1431,    -1,    -1,    -1,     1,    -1,  1437,
       4,     5,     6,    -1,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    25,    26,    27,    28,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    -1,    41,    -1,    43,
      44,    -1,    -1,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    69,    70,    -1,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,   191,   192,    -1,
     194,    -1,    -1,   197,   198,    -1,    -1,    -1,    -1,    -1,
      -1,   205,    -1,   207,    -1,   209,   210,   211,   212,   213,
       1,    -1,    -1,     4,     5,     6,    -1,     8,     9,    10,
      -1,    12,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    25,    26,    27,    28,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    -1,
      41,    -1,    43,    44,    -1,    -1,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    32,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,
      59,    60,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
     131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,
     151,    -1,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,   124,    -1,    -1,    -1,   190,
     191,   192,    -1,   194,    -1,    -1,   197,   198,    -1,    -1,
      -1,    -1,    -1,    -1,   205,    -1,   207,    -1,   209,   210,
     211,   212,   213,     4,     5,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    32,    -1,    -1,   183,    -1,    -1,    38,   187,    -1,
     189,   190,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    52,    -1,    54,    -1,    -1,   205,    -1,    59,    60,
      -1,    -1,    -1,    64,    -1,   214,    67,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    -1,    95,    96,    97,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,   187,    -1,    -1,   190,
     191,   192,    -1,   194,    -1,    -1,   197,   198,    59,    60,
      -1,    -1,    -1,    -1,   205,    -1,   207,    -1,   209,   210,
      -1,   212,    -1,   214,     4,     5,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,   120,
      50,    -1,    -1,   124,    54,    -1,    -1,    -1,    -1,    59,
      60,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,
      70,    -1,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    -1,    95,    96,    97,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,   114,   115,   187,    -1,   189,   190,
     120,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   205,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,   214,    -1,    -1,    -1,   147,   148,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,   187,    -1,    -1,
     190,   191,   192,    -1,   194,    -1,    -1,   197,   198,    59,
      60,    -1,    -1,    -1,    -1,   205,    -1,   207,    -1,   209,
     210,    -1,   212,    -1,   214,     4,     5,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
     120,    50,    -1,    -1,   124,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      69,    70,    -1,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,   114,   115,   187,    -1,   189,
     190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   205,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   214,    -1,    -1,    -1,   147,   148,
     149,   150,   151,    -1,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,
      -1,   190,   191,   192,    -1,   194,    -1,    -1,   197,   198,
      59,    60,    -1,    -1,    -1,    -1,   205,    -1,   207,    -1,
     209,   210,    -1,   212,    -1,   214,     4,     5,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    32,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,   120,    50,    -1,    -1,   124,    54,    -1,    59,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    -1,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    -1,    95,    96,    97,
      98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,   114,   115,   187,   120,
     189,   190,    -1,   124,    -1,   123,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,    -1,    -1,   205,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   214,    -1,    -1,    -1,   147,
     148,   149,   150,   151,    -1,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,   187,    -1,   189,   190,
      -1,    -1,   190,   191,   192,    -1,   194,    -1,    -1,   197,
     198,    -1,    -1,    -1,   205,     4,     5,   205,    -1,   207,
      -1,   209,   210,   214,   212,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      32,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    59,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      69,    70,    -1,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,   115,    -1,   120,    -1,
      -1,    -1,   124,    -1,   123,    -1,    -1,    -1,    -1,    -1,
     129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,
     149,   150,   151,    -1,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,   187,    -1,   189,   190,    -1,
      -1,   190,   191,   192,    -1,   194,    -1,    -1,   197,   198,
      -1,    -1,    -1,   205,     4,     5,   205,    -1,   207,    -1,
     209,   210,   214,   212,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    25,    26,    27,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      50,    51,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,
      70,    -1,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,
     150,   151,    -1,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     190,   191,   192,    -1,   194,    -1,    -1,   197,   198,    -1,
      -1,    -1,    -1,     4,     5,   205,    -1,   207,    -1,   209,
     210,    -1,   212,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    -1,    95,    96,    97,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
     131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,
     151,    -1,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
     191,   192,    -1,   194,    -1,    -1,   197,   198,    -1,    -1,
      -1,    -1,     4,     5,   205,    -1,   207,    -1,   209,   210,
      -1,   212,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    99,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,
      -1,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,
     192,    -1,   194,    -1,    -1,   197,   198,    -1,    -1,    -1,
      -1,     4,     5,   205,    -1,   207,    -1,   209,   210,    -1,
     212,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    69,    70,    -1,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    -1,    95,    96,    97,    98,    99,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,   192,
      -1,   194,    -1,    -1,   197,   198,    -1,    -1,    -1,    -1,
       4,     5,   205,    -1,   207,    -1,   209,   210,    12,   212,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    69,    70,    -1,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,   191,   192,    -1,
     194,    -1,    -1,   197,   198,    -1,    -1,    -1,    -1,     4,
       5,   205,   206,   207,    -1,   209,   210,    12,   212,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    69,    70,    -1,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    90,    91,    92,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   190,   191,   192,    -1,   194,
      -1,    -1,   197,   198,    -1,    -1,    -1,    -1,     4,     5,
     205,    -1,   207,    -1,   209,   210,    12,   212,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    67,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,   148,   149,   150,   151,    -1,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,   191,   192,    -1,   194,    -1,
      -1,   197,   198,    -1,    -1,    -1,    -1,     4,     5,   205,
     206,   207,    -1,   209,   210,    -1,   212,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    69,    70,    -1,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,   150,   151,    -1,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   190,   191,   192,    -1,   194,    -1,    -1,
     197,   198,    -1,    -1,    -1,    -1,     4,     5,   205,   206,
     207,    -1,   209,   210,    -1,   212,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    -1,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    -1,    95,    96,    97,
      98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
     148,   149,   150,   151,    -1,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,   191,   192,    -1,   194,    -1,    -1,   197,
     198,    -1,    -1,    -1,    -1,     4,     5,   205,    -1,   207,
      -1,   209,   210,    -1,   212,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      69,    70,    -1,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,
     149,   150,   151,    -1,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,   191,   192,    -1,   194,    -1,    -1,   197,   198,
      -1,    -1,    -1,    -1,     4,     5,   205,    -1,   207,    -1,
     209,   210,    -1,   212,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,
      70,    -1,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    -1,    95,    96,    97,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,
     150,   151,    -1,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,
     190,   191,   192,    -1,   194,    -1,    -1,   197,   198,    -1,
      -1,    -1,    -1,     4,     5,   205,    -1,   207,    -1,   209,
     210,    12,   212,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    -1,    95,    96,    97,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,
     151,    -1,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
     191,   192,    -1,   194,    -1,    -1,   197,   198,    -1,    -1,
      -1,    -1,     4,     5,   205,    -1,   207,    -1,   209,   210,
      -1,   212,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    99,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,
      -1,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,
     192,    -1,   194,    -1,    -1,   197,   198,    -1,    -1,    -1,
      -1,     4,     5,   205,    -1,   207,   208,   209,   210,    -1,
     212,    14,    15,    16,    17,    18,    -1,    -1,    21,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    69,    70,    -1,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    -1,    95,    96,    97,    98,    99,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,   192,
      -1,   194,    -1,    -1,   197,   198,    -1,    -1,    -1,    -1,
       4,     5,   205,    -1,   207,    -1,   209,   210,    -1,   212,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    69,    70,    -1,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,   191,   192,    -1,
     194,    -1,    -1,   197,   198,    -1,    -1,    -1,    -1,     4,
       5,   205,    -1,   207,   208,   209,   210,    -1,   212,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    69,    70,    -1,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    90,    91,    92,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,   148,   149,   150,   151,    -1,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   190,   191,   192,    -1,   194,
      -1,    -1,   197,   198,    -1,    -1,    -1,    -1,     4,     5,
     205,    -1,   207,   208,   209,   210,    -1,   212,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    67,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,   148,   149,   150,   151,    -1,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,    -1,   194,    -1,
      -1,   197,   198,    -1,    -1,    -1,    -1,     4,     5,   205,
      -1,   207,    -1,   209,   210,    -1,   212,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    69,    70,    -1,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,   150,   151,    -1,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   190,   191,   192,    -1,   194,    -1,    -1,
     197,   198,    -1,    -1,    -1,    -1,     4,     5,   205,    -1,
     207,   208,   209,   210,    -1,   212,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    -1,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    -1,    95,    96,    97,
      98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
     148,   149,   150,   151,    -1,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,   191,   192,    -1,   194,    -1,    -1,   197,
     198,    -1,    -1,    -1,    -1,     4,     5,   205,    -1,   207,
     208,   209,   210,    -1,   212,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      69,    70,    -1,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,
     149,   150,   151,    -1,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,   191,   192,    -1,   194,    -1,    -1,   197,   198,
      -1,    -1,    -1,    -1,     4,     5,   205,    -1,   207,   208,
     209,   210,    -1,   212,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,
      70,    -1,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    -1,    95,    96,    97,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,
     150,   151,    -1,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     190,   191,   192,    -1,   194,    -1,    -1,   197,   198,    -1,
      -1,    -1,    -1,     4,     5,   205,    -1,   207,   208,   209,
     210,    -1,   212,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    -1,    95,    96,    97,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,
     151,    -1,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
     191,   192,    -1,   194,    -1,    -1,   197,   198,    -1,    -1,
      -1,    -1,     4,     5,   205,    -1,   207,   208,   209,   210,
      -1,   212,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    99,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,
      -1,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,
     192,    -1,   194,    -1,    -1,   197,   198,    -1,    -1,    -1,
      -1,     4,     5,   205,    -1,   207,   208,   209,   210,    -1,
     212,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    69,    70,    -1,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    -1,    95,    96,    97,    98,    99,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,   192,
      -1,   194,    -1,    -1,   197,   198,    -1,    -1,    -1,    -1,
       4,     5,   205,    -1,   207,    -1,   209,   210,    -1,   212,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    69,    70,    -1,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,   191,   192,    -1,
     194,    -1,    -1,   197,   198,    -1,    -1,    -1,    -1,     4,
       5,   205,    -1,   207,    -1,   209,   210,    -1,   212,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    69,    70,    -1,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    90,    91,    92,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,   148,   149,   150,   151,    -1,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   190,   191,   192,    -1,   194,
      -1,    -1,   197,   198,    -1,    -1,    -1,    -1,     4,     5,
     205,    -1,   207,    -1,   209,   210,    -1,   212,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    67,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,   148,   149,   150,   151,    -1,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,   191,   192,    -1,   194,    -1,
      -1,   197,   198,    -1,    -1,    -1,    -1,     4,     5,   205,
      -1,   207,    -1,   209,   210,    -1,   212,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    69,    70,    -1,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,    -1,
      -1,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,   150,   151,    -1,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   190,   191,   192,    -1,   194,    -1,    -1,
     197,   198,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
     207,    -1,   209,   210,    -1,   212,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,   137,
      -1,    -1,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   182,   183,    -1,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,    18,    -1,    -1,
      20,    21,    -1,    24,    -1,    -1,   204,   205,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
      -1,    -1,    -1,    -1,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,    20,    21,
      -1,    -1,   132,   133,   134,   135,   136,   137,   139,    -1,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   182,   183,    -1,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   204,   205,    -1,    -1,   209,    -1,
     211,   211,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
     132,   133,   134,   135,   136,   137,    -1,    -1,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,   120,   121,   122,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     182,   183,    -1,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,   204,   205,    -1,    30,    -1,    -1,    -1,   211,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,   187,   188,   189,   190,   191,   192,
     193,   194,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,   204,   205,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    40,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    20,    21,    -1,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,   166,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,    -1,    -1,   112,   113,
     114,   115,   116,    -1,   209,   119,   120,   121,   122,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
     134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,    -1,   139,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,   120,   121,   122,   152,   124,   125,
     126,   127,    20,    21,    -1,    -1,   132,    -1,   134,   135,
      -1,   166,    -1,    -1,   140,   141,   142,    -1,    -1,   183,
     146,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
     204,   205,    -1,    -1,    -1,    -1,    -1,   211,    -1,    -1,
      -1,    -1,    -1,    -1,   209,    -1,    -1,   183,    -1,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   204,   205,
      -1,    -1,    -1,    -1,    -1,   211,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,   116,    -1,
      -1,   119,   120,   121,   122,    -1,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,   132,    -1,   134,   135,    -1,    -1,
      -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,   112,
     113,   114,   115,   116,    -1,    -1,   119,   120,   121,   122,
      -1,   124,   125,   126,   127,    20,    21,    -1,    -1,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,   183,    -1,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,   204,   205,    -1,    -1,
      -1,    -1,    -1,   211,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   204,   205,    -1,    -1,    -1,    -1,    -1,   211,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,
     115,   116,    -1,    -1,   119,   120,   121,   122,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,   112,   113,   114,   115,   116,    20,    21,   119,
     120,   121,   122,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,    -1,   183,    -1,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   204,
     205,    -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   204,   205,    -1,    -1,   208,   112,
     113,   114,   115,   116,    20,    21,   119,   120,   121,   122,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   204,   205,    -1,    -1,   208,   112,   113,   114,   115,
     116,    -1,    -1,   119,   120,   121,   122,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,   134,   135,
      -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,
     146,   112,   113,   114,   115,   116,    20,    21,   119,   120,
     121,   122,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,   183,    -1,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   204,   205,
      -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   204,   205,    -1,    -1,   208,   112,   113,
     114,   115,   116,    20,    21,   119,   120,   121,   122,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
     134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     204,   205,    -1,    -1,   208,   112,   113,   114,   115,   116,
      -1,    -1,   119,   120,   121,   122,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
     112,   113,   114,   115,   116,    20,    21,   119,   120,   121,
     122,    -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
     132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,
     142,    -1,    -1,    -1,   146,    -1,   183,    -1,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   204,   205,    -1,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   204,   205,    -1,    -1,   208,   112,   113,   114,
     115,   116,    20,    21,   119,   120,   121,   122,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   204,
     205,    -1,    -1,   208,   112,   113,   114,   115,   116,    -1,
      -1,   119,   120,   121,   122,    -1,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,   132,    -1,   134,   135,    -1,    -1,
      -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,   112,
     113,   114,   115,   116,    20,    21,   119,   120,   121,   122,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,   183,    -1,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   204,   205,    -1,    -1,   208,   112,   113,   114,   115,
     116,    20,    21,   119,   120,   121,   122,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,   134,   135,
      -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   204,   205,
      -1,    -1,   208,   112,   113,   114,   115,   116,    -1,    -1,
     119,   120,   121,   122,    -1,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,   112,   113,
     114,   115,   116,    20,    21,   119,   120,   121,   122,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
     134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,    -1,   183,    -1,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   204,   205,    -1,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     204,   205,    -1,    -1,   208,   112,   113,   114,   115,   116,
      20,    21,   119,   120,   121,   122,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   204,   205,    -1,
      -1,   208,   112,   113,   114,   115,   116,    -1,    -1,   119,
     120,   121,   122,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,   112,   113,   114,
     115,   116,    20,    21,   119,   120,   121,   122,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,   183,    -1,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   204,   205,    -1,    -1,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   204,
     205,    -1,    -1,   208,   112,   113,   114,   115,   116,    20,
      21,   119,   120,   121,   122,    -1,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,   132,    -1,   134,   135,    -1,    -1,
      -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   204,   205,    -1,    -1,
     208,   112,   113,   114,   115,   116,    -1,    -1,   119,   120,
     121,   122,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,   112,   113,   114,   115,
     116,    20,    21,   119,   120,   121,   122,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,   134,   135,
      -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,
     146,    -1,   183,    -1,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   204,   205,    -1,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   204,   205,
      -1,    -1,   208,   112,   113,   114,   115,   116,    20,    21,
     119,   120,   121,   122,    -1,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   204,   205,    -1,    -1,   208,
     112,   113,   114,   115,   116,    -1,    -1,   119,   120,   121,
     122,    -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
     132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,
     142,    -1,    -1,    -1,   146,   112,   113,   114,   115,   116,
      20,    21,   119,   120,   121,   122,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,   183,    -1,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   204,   205,    -1,
      -1,   208,   112,   113,   114,   115,   116,    20,    21,   119,
     120,   121,   122,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   204,   205,    -1,    -1,   208,   112,
     113,   114,   115,   116,    -1,    -1,   119,   120,   121,   122,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,   112,   113,   114,   115,   116,    20,
      21,   119,   120,   121,   122,    -1,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,   132,    -1,   134,   135,    -1,    -1,
      -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,
     183,    -1,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   204,   205,    -1,    -1,
     208,   112,   113,   114,   115,   116,    20,    21,   119,   120,
     121,   122,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   204,   205,    -1,    -1,   208,   112,   113,
     114,   115,   116,    -1,    -1,   119,   120,   121,   122,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
     134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,   112,   113,   114,   115,   116,    20,    21,
     119,   120,   121,   122,    -1,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,   183,
      -1,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     204,   205,    -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   204,   205,    -1,    -1,   208,
     112,   113,   114,   115,   116,    20,    21,   119,   120,   121,
     122,    -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
     132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,
     142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   204,   205,    -1,    -1,   208,   112,   113,   114,
     115,   116,    -1,    -1,   119,   120,   121,   122,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,   112,   113,   114,   115,   116,    20,    21,   119,
     120,   121,   122,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,    -1,   183,    -1,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   204,
     205,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   204,   205,   206,    -1,    -1,   112,
     113,   114,   115,   116,    20,    21,   119,   120,   121,   122,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   204,   205,   206,    -1,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,   120,   121,   122,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,   134,   135,
      -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,
     146,   112,   113,   114,   115,   116,    -1,    -1,   119,   120,
     121,   122,    -1,   124,   125,   126,   127,    20,    21,    -1,
      -1,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,   183,    -1,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   204,   205,
     206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   204,   205,   206,    -1,    -1,    -1,    -1,
      -1,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,    20,    21,   132,
     133,   134,   135,   136,   137,    -1,    -1,   140,   141,   142,
     143,   144,   145,   146,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,
     183,    -1,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   204,   205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    -1,
      -1,   124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,   140,   141,   142,
     143,   144,   145,   146,   112,   113,   114,   115,   116,    20,
      21,   119,   120,   121,   122,    -1,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,   132,    -1,   134,   135,    -1,    -1,
     138,    -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,
     183,    -1,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,    -1,    -1,   197,   198,    -1,    -1,    -1,    -1,
      -1,   204,   205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   204,   205,    -1,    -1,
      -1,   112,   113,   114,   115,   116,    20,    21,   119,   120,
     121,   122,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,   134,   135,    -1,    -1,   138,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   204,   205,    -1,    -1,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,   120,   121,   122,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
     134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,   112,   113,   114,   115,   116,    20,    21,
     119,   120,   121,   122,    -1,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     204,   205,    -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   204,   205,    -1,    -1,    -1,
     112,   113,   114,   115,   116,    20,    21,   119,   120,   121,
     122,    -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
     132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,
     142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   204,   205,    -1,    -1,    -1,   112,   113,   114,
     115,   116,    -1,    -1,   119,   120,   121,   122,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,   112,   113,   114,   115,   116,    20,    21,   119,
     120,   121,   122,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,
     140,   141,   142,    -1,    -1,    -1,    -1,    -1,   183,    -1,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   204,
     205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   204,   205,    -1,    -1,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,   120,   121,   122,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,   112,   113,   114,   115,   116,    -1,    -1,   119,
     120,   121,   122,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,   134,   135,    20,    21,    -1,    -1,
      -1,    -1,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   204,   205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,   116,   204,   205,   119,   120,   121,   122,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,   134,   135,    20,    21,    -1,    -1,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,   120,   121,   122,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   186,   187,   188,   189,   190,   191,   192,
     193,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,   204,   205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,   189,   190,   191,   192,   193,
     194,    -1,    -1,    -1,    -1,   112,   113,   114,   115,   116,
     204,   205,   119,    -1,    -1,   122,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,   134,   135,    68,
      69,    70,    -1,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    -1,    95,    96,    97,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,
      -1,   188,   189,   190,   191,   192,   193,   194,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   204,   205,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    68,    69,    70,   166,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      -1,    95,    96,    97,    98,    68,    69,    70,    -1,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    -1,    95,    96,    97,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    70,    -1,    72,    73,    74,    75,
      76,   154,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,   166,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   216,     0,     6,    29,    31,    33,    39,    49,    55,
      77,    99,   100,   194,   205,   211,   217,   220,   226,   228,
     229,   234,   263,   267,   296,   370,   377,   381,   392,   434,
     439,   444,    18,    19,   166,   256,   257,   258,   159,   235,
     236,   166,   167,   168,   194,   230,   231,   232,   212,   378,
     166,   209,   219,    56,    62,   373,   373,   373,   139,   166,
     283,    33,    62,   132,   198,   207,   259,   260,   261,   262,
     283,   211,     4,     5,     7,    35,   389,    61,   368,   182,
     181,   184,   181,   231,    21,    56,   193,   204,   233,   373,
     374,   376,   374,   368,   445,   435,   440,   166,   139,   227,
     261,   261,   261,   207,   140,   141,   142,   181,   206,    56,
      62,   268,   270,    56,    62,   379,    56,    62,   390,    56,
      62,   369,    14,    15,   159,   164,   166,   169,   207,   222,
     257,   159,   236,   166,   230,   230,   166,   211,   213,   374,
     211,    56,    62,   218,   166,   166,   166,   166,   170,   225,
     208,   258,   261,   261,   261,   261,   271,   166,   380,   393,
     212,   371,   170,   171,   221,    14,    15,   159,   164,   166,
     222,   254,   255,   233,   375,   211,   446,   436,   441,   170,
     208,    34,    68,    70,    72,    73,    74,    75,    76,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      90,    91,    92,    95,    96,    97,    98,   114,   115,   166,
     266,   269,   184,   212,   103,   387,   388,   366,   156,   342,
     170,   171,   172,   181,   208,   182,   212,   212,   212,    20,
      21,    37,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     124,   125,   126,   133,   134,   135,   136,   137,   140,   141,
     142,   143,   144,   145,   146,   183,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   197,   198,   204,   205,
      34,    34,   207,   264,   212,   272,    72,    76,    95,    96,
      97,    98,   397,   382,   166,   394,   213,   367,   258,   148,
     166,   364,   365,   254,    18,    24,    30,    40,    48,    63,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   152,   209,   283,   396,   398,   399,   402,
     406,   433,   447,   437,   442,   166,   166,   166,   206,    21,
     166,   206,   151,   208,   342,   352,   353,   184,   265,   274,
     212,   372,   184,   386,   212,   391,   342,   206,   207,    42,
     181,   184,   187,   363,   188,   188,   188,   207,   188,   188,
     207,   188,   188,   188,   188,   188,   188,   207,   283,    32,
      59,    60,   120,   124,   183,   187,   190,   205,   214,   185,
     401,   356,   359,   166,   133,   207,    49,   148,   166,   361,
     395,   208,   211,   433,     1,     4,     5,     8,     9,    10,
      12,    14,    15,    16,    17,    18,    24,    25,    26,    27,
      28,    30,    37,    38,    41,    43,    44,    47,    50,    51,
      53,    54,    57,    58,    64,    67,    77,    99,   100,   101,
     102,   114,   115,   129,   130,   131,   147,   148,   149,   150,
     151,   153,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   167,   168,   169,   190,   191,   192,   197,
     198,   205,   207,   209,   210,   211,   212,   213,   224,   226,
     237,   238,   241,   244,   245,   247,   249,   250,   251,   252,
     273,   275,   277,   282,   283,   284,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   300,   301,   308,   311,   314,
     319,   320,   321,   322,   323,   324,   326,   331,   334,   341,
     396,   449,   458,   467,   469,   476,   479,   384,   166,   211,
     383,   283,   348,   365,   206,    64,   167,   334,   166,   166,
     406,   123,   133,   182,   362,   407,   411,   413,   334,   415,
     409,   166,   403,   417,   419,   421,   423,   425,   427,   429,
     431,   334,   207,    32,   187,    32,   187,   205,   214,   206,
     334,   214,   406,   166,   211,   448,   166,   211,   354,   433,
     438,   166,   211,   357,   443,   334,   361,   207,    42,   123,
     181,   182,   184,   187,   360,   352,   188,   188,   334,   248,
     188,   285,   398,   449,   207,   188,   188,   207,   123,   282,
     312,   321,   334,   272,   188,   207,    60,   334,   207,   334,
     166,   188,   188,   207,   211,   188,   159,    57,   334,   207,
     272,   188,   207,   207,   188,   188,   123,   282,   334,   334,
     334,   210,   272,   314,   318,   318,   318,   207,   207,   207,
     207,   207,   207,    12,   406,    12,   406,   334,   466,   474,
     188,   334,   188,   223,   334,   334,   334,   334,   334,    12,
     312,   334,   334,   210,   466,   475,    36,   211,   211,   334,
       9,   166,   207,   211,   211,   211,   211,   211,    65,   297,
     263,   128,   211,    20,    21,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   132,
     133,   134,   135,   136,   137,   140,   141,   142,   143,   144,
     145,   146,   182,   183,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   204,   205,   207,   372,   182,   211,
     213,   205,   213,   263,   349,   208,    42,   211,   362,   282,
     334,   433,   433,   405,   433,   208,   433,   433,   208,   166,
     400,   433,   264,   433,   264,   433,   264,   354,   355,   357,
     358,   208,   276,   312,   206,   206,   211,   213,   184,   211,
     213,   184,   211,   213,   208,   334,   166,   166,   433,   282,
     334,   459,   461,   272,   395,   286,   207,   283,   309,   470,
     312,   282,   312,   181,   123,   182,    13,   406,   475,   334,
     334,   272,   182,   302,   304,   334,   306,   184,   159,   334,
     472,   312,   312,   463,   406,   282,   334,   210,   263,   334,
     334,   334,   334,   334,   334,   395,    52,   154,   166,   190,
     205,   207,   334,   450,   451,   452,   465,   468,   395,   207,
     451,   468,   138,   176,   211,   213,   453,   278,   272,   280,
     172,   173,   221,   395,   181,   478,   208,   148,   153,   188,
     283,   325,   181,   478,   212,   272,   246,   205,   208,   312,
     156,   299,   365,   156,   317,   318,    18,   154,   166,   396,
      18,   154,   166,   396,   334,   334,   334,   334,   334,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     166,   129,   130,   131,   334,   334,   154,   166,   334,   334,
     334,   396,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   334,   334,   334,   129,   130,   131,   334,    21,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     125,   126,   154,   166,   204,   205,   332,   334,   208,   312,
     385,   334,   262,     7,   342,   347,   166,   282,   334,   211,
     189,   189,   189,   211,   189,   189,   189,   211,   189,   265,
     189,   265,   189,   265,   189,   211,   189,   211,   208,   433,
     433,   208,    42,   360,   406,   406,    19,   433,   208,   312,
     188,   334,   433,   406,   208,   334,   272,   189,   211,   181,
     208,   208,   253,   406,   406,   208,   406,   208,   406,   208,
     208,   406,   189,   317,   208,   208,   208,   208,   208,   208,
      19,   318,   207,   133,   360,   205,   334,   208,   138,   181,
     211,   452,   178,   179,   206,   456,   181,   178,   206,   211,
     455,    19,   208,   452,   177,   213,   454,   334,   466,   213,
     406,   406,   334,    19,   206,   207,   207,   327,   329,   466,
     213,   274,    11,    22,    23,   239,   240,   334,   450,   181,
     208,   166,   298,   123,   133,   182,   187,   315,   316,   264,
     188,   207,   188,   207,   207,   318,   318,   318,   207,   206,
     318,   318,   318,    18,   154,   166,   396,   184,   154,   166,
     334,   207,   207,   154,   166,   334,     1,   206,   208,   213,
     211,   206,    56,    62,   345,    66,   346,   211,   211,   408,
     412,   414,   433,   416,   410,   404,   166,   418,   189,   422,
     189,   426,   189,   430,   354,   432,   357,   166,   334,   189,
     189,   312,   189,   208,   166,   208,   189,   189,   207,   406,
     334,   189,   189,   189,   189,   189,   317,   264,   207,   312,
     334,   334,   334,   166,   451,   452,   334,   154,   166,   450,
     456,   206,   334,   206,   465,   312,   451,   177,   180,   213,
     457,   206,   189,   189,   174,   312,   334,   334,   406,   264,
     213,   272,   334,    11,   242,   206,   205,   181,   206,   166,
     166,   166,   166,   181,   206,   265,   335,   334,   337,   334,
     208,   312,   334,   188,   207,   334,   207,   206,   334,   208,
     312,   207,   206,   333,   211,    46,   346,    45,   103,   343,
     420,   424,   428,   460,   462,   272,   287,   189,   211,   310,
     471,   475,   189,   303,   305,   307,   473,   464,   207,   265,
     208,   312,   211,   208,   452,   456,   207,   133,   360,   211,
     452,   206,   279,   281,   211,   208,   208,   189,   265,   272,
     243,   211,   208,   450,   166,   206,   315,   206,   138,   272,
     313,   406,   208,   433,   208,   208,   208,   339,   334,   334,
     208,   208,   334,    32,   344,   343,   345,   207,   207,   334,
     166,   207,   208,   207,   334,   334,   334,   207,   207,   208,
     334,   313,   208,   334,   133,   360,   456,   334,   334,   334,
     334,   457,   334,   207,   334,   328,   189,   240,    25,   102,
     244,   289,   290,   291,   293,   334,   206,   184,   363,   123,
     334,   189,   189,   433,   208,   208,   208,   350,   344,   361,
     451,   451,   208,   189,   312,   475,   312,   451,   208,   211,
     477,   334,   334,   208,   477,   276,   477,   325,   330,   123,
     123,   334,   208,   406,   362,   334,   336,   338,   189,   269,
     351,   208,   208,   334,   208,   208,   208,   208,    52,   206,
     133,   360,   213,   208,   206,   325,   334,   362,   334,   340,
     211,   269,   208,   334,   206,   334,   334,   206,   334,   211,
     272,   211
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   215,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   217,   218,   218,
     218,   219,   219,   220,   221,   221,   221,   221,   222,   223,
     223,   223,   224,   225,   225,   227,   226,   228,   229,   230,
     230,   230,   231,   231,   231,   231,   232,   232,   233,   233,
     234,   235,   235,   236,   236,   237,   238,   238,   239,   239,
     240,   240,   240,   241,   241,   242,   243,   242,   244,   244,
     244,   244,   244,   245,   246,   245,   248,   247,   249,   250,
     251,   253,   252,   254,   254,   254,   254,   254,   254,   255,
     255,   256,   256,   256,   257,   257,   257,   257,   257,   257,
     257,   257,   258,   258,   259,   259,   259,   260,   260,   261,
     261,   261,   261,   261,   261,   261,   262,   262,   263,   263,
     264,   264,   264,   265,   265,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     267,   268,   268,   268,   269,   271,   270,   272,   272,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     274,   274,   274,   275,   276,   276,   278,   279,   277,   280,
     281,   277,   282,   282,   282,   282,   283,   283,   283,   284,
     284,   286,   287,   285,   285,   288,   288,   288,   288,   289,
     290,   291,   291,   291,   292,   292,   292,   293,   293,   294,
     294,   294,   295,   296,   296,   297,   297,   298,   298,   299,
     299,   300,   300,   302,   303,   301,   304,   305,   301,   306,
     307,   301,   309,   310,   308,   311,   311,   311,   312,   312,
     313,   313,   313,   314,   314,   314,   315,   315,   315,   315,
     316,   316,   317,   317,   318,   318,   319,   320,   320,   320,
     320,   320,   320,   320,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   322,   322,   322,   322,   322,   322,   323,
     323,   324,   324,   325,   325,   326,   327,   328,   326,   329,
     330,   326,   331,   331,   331,   331,   332,   333,   331,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   335,   336,
     334,   334,   334,   334,   337,   338,   334,   334,   334,   339,
     340,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   334,   334,   334,   334,   334,   341,   341,   341,   341,
     341,   341,   341,   341,   341,   341,   341,   341,   341,   341,
     341,   341,   342,   342,   343,   343,   343,   344,   344,   345,
     345,   345,   346,   346,   347,   348,   349,   348,   350,   348,
     351,   348,   348,   352,   352,   352,   353,   353,   354,   354,
     355,   355,   356,   356,   356,   357,   358,   358,   359,   359,
     359,   360,   360,   361,   361,   361,   361,   361,   361,   362,
     362,   362,   363,   363,   364,   364,   364,   364,   364,   365,
     365,   365,   365,   365,   366,   367,   366,   368,   368,   369,
     369,   369,   370,   371,   370,   372,   372,   372,   372,   373,
     373,   373,   375,   374,   376,   376,   377,   378,   377,   379,
     379,   379,   380,   382,   383,   381,   384,   385,   381,   386,
     386,   387,   387,   388,   389,   389,   390,   390,   390,   391,
     391,   393,   394,   392,   395,   395,   395,   395,   395,   396,
     396,   396,   396,   396,   396,   396,   396,   396,   396,   396,
     396,   396,   396,   396,   396,   396,   396,   396,   396,   396,
     396,   396,   396,   396,   396,   396,   397,   397,   397,   397,
     397,   397,   398,   399,   399,   399,   400,   400,   401,   401,
     401,   403,   404,   402,   405,   405,   406,   406,   406,   406,
     406,   406,   407,   408,   406,   406,   406,   406,   406,   406,
     406,   406,   406,   406,   406,   406,   406,   406,   406,   409,
     410,   406,   406,   411,   412,   406,   413,   414,   406,   415,
     416,   406,   406,   417,   418,   406,   419,   420,   406,   406,
     421,   422,   406,   423,   424,   406,   406,   425,   426,   406,
     427,   428,   406,   429,   430,   406,   431,   432,   406,   433,
     433,   433,   435,   436,   437,   438,   434,   440,   441,   442,
     443,   439,   445,   446,   447,   448,   444,   449,   449,   449,
     449,   449,   450,   450,   450,   450,   450,   450,   450,   450,
     451,   451,   452,   452,   453,   453,   454,   454,   455,   455,
     456,   456,   456,   457,   457,   457,   458,   458,   458,   458,
     458,   458,   459,   460,   458,   461,   462,   458,   463,   464,
     458,   465,   465,   465,   466,   466,   467,   468,   468,   469,
     469,   469,   469,   470,   471,   469,   469,   472,   473,   469,
     474,   474,   475,   475,   476,   476,   476,   476,   476,   477,
     477,   478,   478,   479,   479,   479
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
       3,     0,     1,     1,     3,     0,     4,     3,     7,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     2,     2,     2,
       0,     2,     2,     3,     0,     1,     0,     0,     7,     0,
       0,     9,     3,     2,     2,     2,     1,     3,     2,     2,
       3,     0,     0,     5,     1,     2,     4,     5,     2,     1,
       1,     1,     2,     3,     2,     2,     3,     2,     3,     2,
       2,     3,     4,     1,     1,     1,     0,     1,     3,     9,
       8,     3,     3,     0,     0,     7,     0,     0,     7,     0,
       0,     7,     0,     0,     6,     5,     8,    10,     1,     3,
       1,     2,     3,     1,     1,     2,     2,     2,     2,     2,
       1,     3,     0,     4,     1,     6,     6,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     4,     4,     4,     4,     4,     4,     6,
       8,     5,     6,     1,     4,     3,     0,     0,     8,     0,
       0,     9,     3,     4,     5,     6,     0,     0,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     3,
       4,     5,     4,     5,     3,     4,     1,     3,     4,     3,
       4,     2,     4,     4,     7,     8,     3,     5,     0,     0,
       8,     3,     3,     3,     0,     0,     8,     3,     4,     0,
       0,     9,     4,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     2,     4,     1,     4,     4,     4,     4,
       4,     1,     6,     7,     6,     6,     7,     7,     6,     7,
       6,     6,     0,     4,     0,     1,     1,     0,     1,     0,
       1,     1,     0,     1,     5,     0,     0,     4,     0,     9,
       0,    10,     5,     2,     3,     4,     1,     3,     1,     3,
       1,     3,     0,     2,     3,     3,     1,     3,     0,     2,
       3,     1,     1,     1,     2,     3,     5,     3,     3,     1,
       1,     1,     0,     1,     1,     4,     3,     3,     5,     4,
       6,     5,     5,     4,     0,     0,     4,     0,     1,     0,
       1,     1,     6,     0,     6,     0,     2,     3,     5,     0,
       1,     1,     0,     5,     2,     3,     4,     0,     4,     0,
       1,     1,     1,     0,     0,     9,     0,     0,    11,     0,
       2,     0,     1,     3,     1,     1,     0,     1,     1,     0,
       3,     0,     0,     7,     1,     4,     3,     3,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     1,     3,     0,     2,
       3,     0,     0,     6,     1,     3,     1,     1,     1,     1,
       4,     3,     0,     0,     6,     4,     5,     4,     2,     2,
       3,     2,     3,     2,     2,     3,     3,     3,     2,     0,
       0,     6,     2,     0,     0,     6,     0,     0,     6,     0,
       0,     6,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     0,     0,     6,     0,     0,     6,     1,
       3,     3,     0,     0,     0,     0,    10,     0,     0,     0,
       0,    10,     0,     0,     0,     0,    10,     1,     1,     1,
       1,     1,     3,     3,     5,     5,     6,     6,     8,     8,
       1,     3,     0,     2,     2,     1,     2,     1,     2,     1,
       2,     1,     1,     2,     1,     1,     5,     4,     6,     7,
       5,     7,     0,     0,     9,     0,     0,     9,     0,     0,
       9,     1,     3,     3,     3,     1,     4,     1,     3,     4,
       4,     4,     4,     0,     0,     9,     4,     0,     0,     9,
       1,     3,     1,     3,     4,     3,     4,     7,     9,     0,
       3,     0,     1,     9,    10,    10
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
        if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
    break;

  case 108: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 219: /* expression_any: ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 220: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 221: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 222: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 223: /* expression_any: expr_assign ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 224: /* expression_any: expression_delete ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 225: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 226: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 227: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 228: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 229: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 230: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 231: /* expression_any: expression_break ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 232: /* expression_any: expression_continue ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 233: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 234: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 235: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 236: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 237: /* expression_any: expression_label ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 238: /* expression_any: expression_goto ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 239: /* expression_any: "pass" ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 240: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 241: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 242: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 243: /* expr_keyword: "keyword" expr expression_block  */
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

  case 244: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 245: /* optional_expr_list: expr_list  */
                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 246: /* $@7: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 247: /* $@8: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 248: /* expression_keyword: "keyword" '<' $@7 type_declaration_no_options '>' $@8 expr  */
                                                                                                                                               {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        auto td = new ExprTypeDecl(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCall->arguments.insert(pCall->arguments.begin(), ExpressionPtr(td));
        pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 249: /* $@9: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 250: /* $@10: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 251: /* expression_keyword: "type function" '<' $@9 type_declaration_no_options '>' $@10 '(' optional_expr_list ')'  */
                                                                                                                                                                                    {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),tokAt(scanner,(yylsp[0])),*(yyvsp[-8].s));
        if ( (yyvsp[-1].pExpression) ) {
            pCall->arguments = sequenceToList((yyvsp[-1].pExpression));
        }
        auto td = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-5].pTypeDecl)));
        pCall->arguments.insert(pCall->arguments.begin(), ExpressionPtr(td));
        delete (yyvsp[-8].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 252: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 253: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 254: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 255: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 256: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 257: /* name_in_namespace: "name" "::" "name"  */
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

  case 258: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 259: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 260: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 261: /* $@11: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 262: /* $@12: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 263: /* new_type_declaration: '<' $@11 type_declaration '>' $@12  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 264: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 265: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 266: /* expr_new: "new" new_type_declaration '(' ')'  */
                                                               {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 267: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 268: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 269: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 270: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 271: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 272: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 273: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 274: /* expression_return: expression_return_no_pipe ';'  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 275: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 276: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 277: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 278: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 279: /* expression_yield: expression_yield_no_pipe ';'  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 280: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 281: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 282: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 283: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 284: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 285: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 286: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 287: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 288: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 289: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 290: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr ';'  */
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

  case 291: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 292: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 293: /* $@13: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 294: /* $@14: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 295: /* expr_cast: "cast" '<' $@13 type_declaration_no_options '>' $@14 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 296: /* $@15: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 297: /* $@16: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 298: /* expr_cast: "upcast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 299: /* $@17: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 300: /* $@18: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 301: /* expr_cast: "reinterpret" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 302: /* $@19: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 303: /* $@20: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 304: /* expr_type_decl: "type" '<' $@19 type_declaration '>' $@20  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 305: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 306: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 307: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
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

  case 308: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 309: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 310: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 311: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 312: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 313: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 314: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 315: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 316: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 317: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 318: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 319: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 320: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 321: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 322: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 323: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 324: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 325: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 326: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 327: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 328: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 329: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 330: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 331: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 332: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 333: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 334: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 335: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 336: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 337: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 338: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 339: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 340: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 341: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 342: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 343: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 344: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 345: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 346: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 347: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 348: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 349: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 350: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 351: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 352: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 353: /* expr_assign_pipe: expr '=' "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 354: /* expr_assign_pipe: expr '=' "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 355: /* expr_assign_pipe: expr '=' "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 356: /* expr_assign_pipe: expr "<-" "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 357: /* expr_assign_pipe: expr "<-" "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 358: /* expr_assign_pipe: expr "<-" "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 359: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 360: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 361: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 362: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 363: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 364: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 365: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 366: /* $@21: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 367: /* $@22: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 368: /* func_addr_expr: '@' '@' '<' $@21 type_declaration_no_options '>' $@22 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 369: /* $@23: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 370: /* $@24: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 371: /* func_addr_expr: '@' '@' '<' $@23 optional_function_argument_list optional_function_type '>' $@24 func_addr_name  */
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

  case 372: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 373: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 374: /* expr_field: expr '.' "name" '(' ')'  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        das_yyerror(scanner,"foo.bar() method call syntax is not supported, use foo->bar() instead",
            tokAt(scanner,(yylsp[-3])),CompilationError::syntax_error);
    }
    break;

  case 375: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                       {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-5].pExpression)), *(yyvsp[-3].s));
        delete (yyvsp[-3].s);
        das_yyerror(scanner,"foo.bar(...) method call syntax is not supported, use foo->bar(...) instead",
            tokAt(scanner,(yylsp[-4])),CompilationError::syntax_error);
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 376: /* $@25: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 377: /* $@26: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 378: /* expr_field: expr '.' $@25 error $@26  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 379: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 380: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 381: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 382: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 383: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 384: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 385: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 386: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 387: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 388: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 389: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 392: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 393: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 394: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 395: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 396: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 397: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 398: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 399: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 400: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 401: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 402: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 403: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 404: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 405: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 406: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 407: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 408: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 409: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 410: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 411: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 412: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 413: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 414: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 415: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 416: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 417: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 418: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 419: /* expr: '(' expr ')'  */
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 420: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 421: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 422: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 423: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 424: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 425: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 426: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 427: /* expr: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
    break;

  case 428: /* expr: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 429: /* expr: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 430: /* expr: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 431: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 432: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 433: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 434: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 435: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 436: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 437: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 438: /* $@27: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 439: /* $@28: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 440: /* expr: expr "is" "type" '<' $@27 type_declaration_no_options '>' $@28  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 441: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 442: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 443: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 444: /* $@29: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 445: /* $@30: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 446: /* expr: expr "as" "type" '<' $@29 type_declaration '>' $@30  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 447: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 448: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 449: /* $@31: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 450: /* $@32: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 451: /* expr: expr '?' "as" "type" '<' $@31 type_declaration '>' $@32  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 452: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 453: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 454: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 455: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 456: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 457: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 458: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 459: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 460: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 461: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 462: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 463: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 464: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 465: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 466: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 467: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 468: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 469: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 470: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 471: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 472: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 473: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 474: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 475: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 476: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 477: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 478: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 479: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 480: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 481: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 482: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 483: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 484: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 485: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 486: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 487: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 488: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 489: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 490: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 491: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 492: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 493: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 494: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 495: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 496: /* $@33: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 497: /* struct_variable_declaration_list: struct_variable_declaration_list $@33 structure_variable_declaration ';'  */
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

  case 498: /* $@34: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 499: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@34 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 500: /* $@35: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 501: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@35 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 502: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 503: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 504: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 505: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 506: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 507: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 508: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 509: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 510: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 511: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 512: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 513: /* tuple_alias_type_list: tuple_alias_type_list ';'  */
                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 514: /* tuple_alias_type_list: tuple_alias_type_list tuple_type ';'  */
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

  case 515: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 516: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 517: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 518: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 519: /* variant_alias_type_list: variant_alias_type_list ';'  */
                                        {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 520: /* variant_alias_type_list: variant_alias_type_list variant_type ';'  */
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

  case 521: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 522: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 523: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 524: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 525: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 526: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 527: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 528: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 529: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 530: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 531: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 532: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 533: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 534: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 535: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 536: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 537: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 538: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 539: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options ';'  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 540: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 541: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 542: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 543: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 544: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 545: /* $@36: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 546: /* global_variable_declaration_list: global_variable_declaration_list $@36 optional_field_annotation let_variable_declaration  */
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

  case 547: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 548: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 549: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 550: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 551: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 552: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 553: /* $@37: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 554: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@37 optional_field_annotation let_variable_declaration  */
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

  case 555: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 556: /* enum_list: enum_list ';'  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 557: /* enum_list: enum_list "name" ';'  */
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

  case 558: /* enum_list: enum_list "name" '=' expr ';'  */
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

  case 559: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 560: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 561: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 562: /* $@38: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 563: /* single_alias: optional_public_or_private_alias "name" $@38 '=' type_declaration  */
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

  case 567: /* $@39: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 569: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 570: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 571: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 572: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 573: /* $@40: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 574: /* $@41: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 575: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name '{' $@40 enum_list $@41 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 576: /* $@42: %empty  */
                                                                                                                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 577: /* $@43: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 578: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration '{' $@42 enum_list $@43 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 579: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 580: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 581: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 582: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 583: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 584: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 585: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 586: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 587: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 588: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 589: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 590: /* optional_struct_variable_declaration_list: '{' struct_variable_declaration_list '}'  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 591: /* $@44: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 592: /* $@45: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 593: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@44 structure_name $@45 optional_struct_variable_declaration_list  */
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

  case 594: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 595: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 596: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 597: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 598: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 599: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 600: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 601: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 602: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 603: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 604: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 605: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 606: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 607: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 608: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 609: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 610: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 611: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 612: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 613: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 614: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 615: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 616: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 617: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 618: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 619: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 620: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 621: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 622: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 623: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 624: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 625: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 626: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 627: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 628: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 629: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 630: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 631: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 632: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 633: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 634: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 635: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 636: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 637: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 638: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 639: /* bitfield_alias_bits: bitfield_alias_bits ';'  */
                                      {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 640: /* bitfield_alias_bits: bitfield_alias_bits "name" ';'  */
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

  case 641: /* $@46: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 642: /* $@47: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 643: /* bitfield_type_declaration: "bitfield" '<' $@46 bitfield_bits '>' $@47  */
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

  case 644: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 645: /* table_type_pair: type_declaration ';' type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 646: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 647: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 648: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 649: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 650: /* type_declaration_no_options: type_declaration_no_options '[' expr ']'  */
                                                                    {
        if ( (yyvsp[-3].pTypeDecl)->baseType==Type::typeDecl ) {
            das_yyerror(scanner,"type declaration can`t be used as array base type",tokAt(scanner,(yylsp[-3])),
                CompilationError::invalid_type);
        } else if ( (yyvsp[-3].pTypeDecl)->baseType==Type::typeMacro ) {
            das_yyerror(scanner,"macro can`t be used as array base type",tokAt(scanner,(yylsp[-3])),
                CompilationError::invalid_type);
        }
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

  case 651: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 652: /* $@48: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 653: /* $@49: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 654: /* type_declaration_no_options: "type" '<' $@48 type_declaration '>' $@49  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 655: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 656: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-3].s);
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        delete (yyvsp[-3].s);
    }
    break;

  case 657: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 658: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 659: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 660: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 661: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 662: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 663: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 664: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 665: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 666: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 667: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 668: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 669: /* $@50: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 670: /* $@51: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 671: /* type_declaration_no_options: "smart_ptr" '<' $@50 type_declaration '>' $@51  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 672: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 673: /* $@52: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 674: /* $@53: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 675: /* type_declaration_no_options: "array" '<' $@52 type_declaration '>' $@53  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 676: /* $@54: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 677: /* $@55: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 678: /* type_declaration_no_options: "table" '<' $@54 table_type_pair '>' $@55  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 679: /* $@56: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 680: /* $@57: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 681: /* type_declaration_no_options: "iterator" '<' $@56 type_declaration '>' $@57  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 682: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 683: /* $@58: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 684: /* $@59: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 685: /* type_declaration_no_options: "block" '<' $@58 type_declaration '>' $@59  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 686: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 687: /* $@61: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 688: /* type_declaration_no_options: "block" '<' $@60 optional_function_argument_list optional_function_type '>' $@61  */
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

  case 689: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 690: /* $@62: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 691: /* $@63: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 692: /* type_declaration_no_options: "function" '<' $@62 type_declaration '>' $@63  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 693: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 694: /* $@65: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 695: /* type_declaration_no_options: "function" '<' $@64 optional_function_argument_list optional_function_type '>' $@65  */
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

  case 696: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 697: /* $@66: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 698: /* $@67: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 699: /* type_declaration_no_options: "lambda" '<' $@66 type_declaration '>' $@67  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 700: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 701: /* $@69: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 702: /* type_declaration_no_options: "lambda" '<' $@68 optional_function_argument_list optional_function_type '>' $@69  */
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

  case 703: /* $@70: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 704: /* $@71: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 705: /* type_declaration_no_options: "tuple" '<' $@70 tuple_type_list '>' $@71  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 706: /* $@72: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 707: /* $@73: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 708: /* type_declaration_no_options: "variant" '<' $@72 variant_type_list '>' $@73  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 709: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 710: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 711: /* type_declaration: type_declaration '|' '#'  */
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

  case 712: /* $@74: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 713: /* $@75: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 714: /* $@76: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 715: /* $@77: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 716: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@74 "name" $@75 '{' $@76 tuple_alias_type_list $@77 '}'  */
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

  case 717: /* $@78: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 718: /* $@79: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 719: /* $@80: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 720: /* $@81: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 721: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@78 "name" $@79 '{' $@80 variant_alias_type_list $@81 '}'  */
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

  case 722: /* $@82: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 723: /* $@83: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 724: /* $@84: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 725: /* $@85: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 726: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@82 "name" $@83 '{' $@84 bitfield_alias_bits $@85 '}'  */
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

  case 727: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 728: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 729: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 730: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 731: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 732: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 733: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 734: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 735: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 736: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 737: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 738: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 739: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 740: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 741: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 742: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 743: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 756: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 757: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 758: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 759: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 760: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 761: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 762: /* $@86: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 763: /* $@87: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 764: /* make_struct_decl: "struct" '<' $@86 type_declaration_no_options '>' $@87 '(' make_struct_dim ')'  */
                                                                                                                                                                                                        {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 765: /* $@88: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 766: /* $@89: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 767: /* make_struct_decl: "class" '<' $@88 type_declaration_no_options '>' $@89 '(' make_struct_dim ')'  */
                                                                                                                                                                                                       {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 768: /* $@90: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 769: /* $@91: %empty  */
                                                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 770: /* make_struct_decl: "variant" '<' $@90 type_declaration_no_options '>' $@91 '(' make_struct_dim ')'  */
                                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 771: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 772: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 773: /* make_tuple: make_tuple ',' expr  */
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

  case 774: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 775: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 776: /* make_tuple_call: "tuple" '(' expr_list ')'  */
                                                     {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-3])));
        mkt->values = sequenceToList((yyvsp[-1].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 777: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 778: /* make_dim: make_dim ';' make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 779: /* make_dim_decl: '[' expr_list optional_comma ']'  */
                                                          {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 780: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 781: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 782: /* make_dim_decl: "array" '(' expr_list ')'  */
                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 783: /* $@92: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 784: /* $@93: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 785: /* make_dim_decl: "array" '<' $@92 type_declaration_no_options '>' $@93 '(' expr_list ')'  */
                                                                                                                                                               {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-8])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 786: /* make_dim_decl: "fixed_array" '(' expr_list ')'  */
                                                          {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 787: /* $@94: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 788: /* $@95: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 789: /* make_dim_decl: "fixed_array" '<' $@94 type_declaration_no_options '>' $@95 '(' expr_list ')'  */
                                                                                                                                                                     {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-8])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( !mka->makeType->dim.size() ) mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 790: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 791: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 792: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 793: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 794: /* make_table_decl: '{' expr_map_tuple_list optional_comma '}'  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 795: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 796: /* make_table_decl: "table" '(' expr_map_tuple_list ')'  */
                                                              {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 797: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' expr_map_tuple_list ')'  */
                                                                                                        {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-6])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 798: /* make_table_decl: "table" '<' type_declaration_no_options ';' type_declaration_no_options '>' '(' expr_map_tuple_list ')'  */
                                                                                                                                                 {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-8])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::tTuple);
        mka->makeType->argTypes.push_back((yyvsp[-6].pTypeDecl));
        mka->makeType->argTypes.push_back((yyvsp[-4].pTypeDecl));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 799: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 800: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 803: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false);
    }
    break;

  case 804: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 805: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
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


