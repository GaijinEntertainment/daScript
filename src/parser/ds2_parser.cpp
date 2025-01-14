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
#define YYSTYPE         DAS2_YYSTYPE
#define YYLTYPE         DAS2_YYLTYPE
/* Substitute the variable and function names.  */
#define yyparse         das2_yyparse
#define yylex           das2_yylex
#define yyerror         das2_yyerror
#define yydebug         das2_yydebug
#define yynerrs         das2_yynerrs

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

    union DAS2_YYSTYPE;
    struct DAS2_YYLTYPE;

    #define YY_NO_UNISTD_H
    #include "lex2.yy.h"

    void das2_yyerror ( DAS2_YYLTYPE * lloc, yyscan_t scanner, const string & error );
    void das2_yyfatalerror ( DAS2_YYLTYPE * lloc, yyscan_t scanner, const string & error, CompilationError cerr );
    int yylex ( DAS2_YYSTYPE *lvalp, DAS2_YYLTYPE *llocp, yyscan_t scanner );
    void yybegin ( const char * str );

    void das2_yybegin_reader ( yyscan_t yyscanner );
    void das2_yyend_reader ( yyscan_t yyscanner );
    void das2_accept_sequence ( yyscan_t yyscanner, const char * seq, size_t seqLen, int lineNo, FileInfo * info );

    namespace das { class Module; }
    void das2_collect_keywords ( das::Module * mod, yyscan_t yyscanner );

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

#include "ds2_parser.hpp"
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
  YYSYMBOL_ADDEQU = 107,                   /* "+="  */
  YYSYMBOL_SUBEQU = 108,                   /* "-="  */
  YYSYMBOL_DIVEQU = 109,                   /* "/="  */
  YYSYMBOL_MULEQU = 110,                   /* "*="  */
  YYSYMBOL_MODEQU = 111,                   /* "%="  */
  YYSYMBOL_ANDEQU = 112,                   /* "&="  */
  YYSYMBOL_OREQU = 113,                    /* "|="  */
  YYSYMBOL_XOREQU = 114,                   /* "^="  */
  YYSYMBOL_SHL = 115,                      /* "<<"  */
  YYSYMBOL_SHR = 116,                      /* ">>"  */
  YYSYMBOL_ADDADD = 117,                   /* "++"  */
  YYSYMBOL_SUBSUB = 118,                   /* "--"  */
  YYSYMBOL_LEEQU = 119,                    /* "<="  */
  YYSYMBOL_SHLEQU = 120,                   /* "<<="  */
  YYSYMBOL_SHREQU = 121,                   /* ">>="  */
  YYSYMBOL_GREQU = 122,                    /* ">="  */
  YYSYMBOL_EQUEQU = 123,                   /* "=="  */
  YYSYMBOL_NOTEQU = 124,                   /* "!="  */
  YYSYMBOL_RARROW = 125,                   /* "->"  */
  YYSYMBOL_LARROW = 126,                   /* "<-"  */
  YYSYMBOL_QQ = 127,                       /* "??"  */
  YYSYMBOL_QDOT = 128,                     /* "?."  */
  YYSYMBOL_QBRA = 129,                     /* "?["  */
  YYSYMBOL_LPIPE = 130,                    /* "<|"  */
  YYSYMBOL_RPIPE = 131,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 132,                 /* ":="  */
  YYSYMBOL_ROTL = 133,                     /* "<<<"  */
  YYSYMBOL_ROTR = 134,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 135,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 136,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 137,                    /* "=>"  */
  YYSYMBOL_COLCOL = 138,                   /* "::"  */
  YYSYMBOL_ANDAND = 139,                   /* "&&"  */
  YYSYMBOL_OROR = 140,                     /* "||"  */
  YYSYMBOL_XORXOR = 141,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 142,                /* "&&="  */
  YYSYMBOL_OROREQU = 143,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 144,                /* "^^="  */
  YYSYMBOL_DOTDOT = 145,                   /* ".."  */
  YYSYMBOL_MTAG_E = 146,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 147,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 148,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 149,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 150,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 151,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 152,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 153,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 154,           /* "..."  */
  YYSYMBOL_INTEGER = 155,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 156,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 157,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 158,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 159,            /* "unsigned int8 constant"  */
  YYSYMBOL_FLOAT = 160,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 161,                   /* "double constant"  */
  YYSYMBOL_NAME = 162,                     /* "name"  */
  YYSYMBOL_DAS_EMIT_COMMA = 163,           /* "new line, comma"  */
  YYSYMBOL_DAS_EMIT_SEMICOLON = 164,       /* "new line, semicolon"  */
  YYSYMBOL_BEGIN_STRING = 165,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 166,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 167,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 168,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 169,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 170,          /* "}"  */
  YYSYMBOL_END_OF_READ = 171,              /* "end of failed eader macro"  */
  YYSYMBOL_172_ = 172,                     /* ','  */
  YYSYMBOL_173_ = 173,                     /* '='  */
  YYSYMBOL_174_ = 174,                     /* '?'  */
  YYSYMBOL_175_ = 175,                     /* ':'  */
  YYSYMBOL_176_ = 176,                     /* '|'  */
  YYSYMBOL_177_ = 177,                     /* '^'  */
  YYSYMBOL_178_ = 178,                     /* '&'  */
  YYSYMBOL_179_ = 179,                     /* '<'  */
  YYSYMBOL_180_ = 180,                     /* '>'  */
  YYSYMBOL_181_ = 181,                     /* '-'  */
  YYSYMBOL_182_ = 182,                     /* '+'  */
  YYSYMBOL_183_ = 183,                     /* '*'  */
  YYSYMBOL_184_ = 184,                     /* '/'  */
  YYSYMBOL_185_ = 185,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 186,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 187,               /* UNARY_PLUS  */
  YYSYMBOL_188_ = 188,                     /* '~'  */
  YYSYMBOL_189_ = 189,                     /* '!'  */
  YYSYMBOL_PRE_INC = 190,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 191,                  /* PRE_DEC  */
  YYSYMBOL_LLPIPE = 192,                   /* LLPIPE  */
  YYSYMBOL_POST_INC = 193,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 194,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 195,                    /* DEREF  */
  YYSYMBOL_196_ = 196,                     /* '.'  */
  YYSYMBOL_197_ = 197,                     /* '['  */
  YYSYMBOL_198_ = 198,                     /* ']'  */
  YYSYMBOL_199_ = 199,                     /* '('  */
  YYSYMBOL_200_ = 200,                     /* ')'  */
  YYSYMBOL_201_ = 201,                     /* '$'  */
  YYSYMBOL_202_ = 202,                     /* '@'  */
  YYSYMBOL_203_ = 203,                     /* ';'  */
  YYSYMBOL_204_ = 204,                     /* '{'  */
  YYSYMBOL_205_ = 205,                     /* '}'  */
  YYSYMBOL_206_ = 206,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 207,                 /* $accept  */
  YYSYMBOL_program = 208,                  /* program  */
  YYSYMBOL_COMMA = 209,                    /* COMMA  */
  YYSYMBOL_SEMICOLON = 210,                /* SEMICOLON  */
  YYSYMBOL_top_level_reader_macro = 211,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 212, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 213,              /* module_name  */
  YYSYMBOL_module_declaration = 214,       /* module_declaration  */
  YYSYMBOL_character_sequence = 215,       /* character_sequence  */
  YYSYMBOL_string_constant = 216,          /* string_constant  */
  YYSYMBOL_string_builder_body = 217,      /* string_builder_body  */
  YYSYMBOL_string_builder = 218,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 219, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 220,              /* expr_reader  */
  YYSYMBOL_221_1 = 221,                    /* $@1  */
  YYSYMBOL_options_declaration = 222,      /* options_declaration  */
  YYSYMBOL_require_declaration = 223,      /* require_declaration  */
  YYSYMBOL_require_module_name = 224,      /* require_module_name  */
  YYSYMBOL_require_module = 225,           /* require_module  */
  YYSYMBOL_is_public_module = 226,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 227,       /* expect_declaration  */
  YYSYMBOL_expect_list = 228,              /* expect_list  */
  YYSYMBOL_expect_error = 229,             /* expect_error  */
  YYSYMBOL_expression_label = 230,         /* expression_label  */
  YYSYMBOL_expression_goto = 231,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 232,      /* elif_or_static_elif  */
  YYSYMBOL_emit_semis = 233,               /* emit_semis  */
  YYSYMBOL_optional_emit_semis = 234,      /* optional_emit_semis  */
  YYSYMBOL_expression_else = 235,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 236,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 237, /* expression_else_one_liner  */
  YYSYMBOL_expression_if_one_liner = 238,  /* expression_if_one_liner  */
  YYSYMBOL_semis = 239,                    /* semis  */
  YYSYMBOL_optional_semis = 240,           /* optional_semis  */
  YYSYMBOL_expression_if_block = 241,      /* expression_if_block  */
  YYSYMBOL_242_2 = 242,                    /* $@2  */
  YYSYMBOL_243_3 = 243,                    /* $@3  */
  YYSYMBOL_244_4 = 244,                    /* $@4  */
  YYSYMBOL_expression_if_then_else = 245,  /* expression_if_then_else  */
  YYSYMBOL_246_5 = 246,                    /* $@5  */
  YYSYMBOL_expression_if_then_else_oneliner = 247, /* expression_if_then_else_oneliner  */
  YYSYMBOL_expression_for_loop = 248,      /* expression_for_loop  */
  YYSYMBOL_249_6 = 249,                    /* $@6  */
  YYSYMBOL_expression_unsafe = 250,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 251,    /* expression_while_loop  */
  YYSYMBOL_252_7 = 252,                    /* $@7  */
  YYSYMBOL_expression_with = 253,          /* expression_with  */
  YYSYMBOL_254_8 = 254,                    /* $@8  */
  YYSYMBOL_expression_with_alias = 255,    /* expression_with_alias  */
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
  YYSYMBOL_optional_annotation_list_with_emit_semis = 267, /* optional_annotation_list_with_emit_semis  */
  YYSYMBOL_optional_function_argument_list = 268, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 269,   /* optional_function_type  */
  YYSYMBOL_function_name = 270,            /* function_name  */
  YYSYMBOL_global_function_declaration = 271, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 272, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 273, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 274,     /* function_declaration  */
  YYSYMBOL_275_9 = 275,                    /* $@9  */
  YYSYMBOL_expression_block_finally = 276, /* expression_block_finally  */
  YYSYMBOL_277_10 = 277,                   /* $@10  */
  YYSYMBOL_278_11 = 278,                   /* $@11  */
  YYSYMBOL_expression_block = 279,         /* expression_block  */
  YYSYMBOL_280_12 = 280,                   /* $@12  */
  YYSYMBOL_281_13 = 281,                   /* $@13  */
  YYSYMBOL_expr_call_pipe = 282,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 283,           /* expression_any  */
  YYSYMBOL_expressions = 284,              /* expressions  */
  YYSYMBOL_optional_expr_list = 285,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_map_tuple_list = 286, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 287, /* type_declaration_no_options_list  */
  YYSYMBOL_name_in_namespace = 288,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 289,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 290,     /* new_type_declaration  */
  YYSYMBOL_291_14 = 291,                   /* $@14  */
  YYSYMBOL_292_15 = 292,                   /* $@15  */
  YYSYMBOL_expr_new = 293,                 /* expr_new  */
  YYSYMBOL_expression_break = 294,         /* expression_break  */
  YYSYMBOL_expression_continue = 295,      /* expression_continue  */
  YYSYMBOL_expression_return = 296,        /* expression_return  */
  YYSYMBOL_expression_yield = 297,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 298,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 299,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 300,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 301,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 302,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 303, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 304,           /* expression_let  */
  YYSYMBOL_expr_cast = 305,                /* expr_cast  */
  YYSYMBOL_306_16 = 306,                   /* $@16  */
  YYSYMBOL_307_17 = 307,                   /* $@17  */
  YYSYMBOL_308_18 = 308,                   /* $@18  */
  YYSYMBOL_309_19 = 309,                   /* $@19  */
  YYSYMBOL_310_20 = 310,                   /* $@20  */
  YYSYMBOL_311_21 = 311,                   /* $@21  */
  YYSYMBOL_expr_type_decl = 312,           /* expr_type_decl  */
  YYSYMBOL_313_22 = 313,                   /* $@22  */
  YYSYMBOL_314_23 = 314,                   /* $@23  */
  YYSYMBOL_expr_type_info = 315,           /* expr_type_info  */
  YYSYMBOL_expr_list = 316,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 317,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 318,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 319,            /* capture_entry  */
  YYSYMBOL_capture_list = 320,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 321,    /* optional_capture_list  */
  YYSYMBOL_expr_full_block = 322,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 323, /* expr_full_block_assumed_piped  */
  YYSYMBOL_expr_numeric_const = 324,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 325,              /* expr_assign  */
  YYSYMBOL_expr_named_call = 326,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 327,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 328,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 329,           /* func_addr_expr  */
  YYSYMBOL_330_24 = 330,                   /* $@24  */
  YYSYMBOL_331_25 = 331,                   /* $@25  */
  YYSYMBOL_332_26 = 332,                   /* $@26  */
  YYSYMBOL_333_27 = 333,                   /* $@27  */
  YYSYMBOL_expr_field = 334,               /* expr_field  */
  YYSYMBOL_335_28 = 335,                   /* $@28  */
  YYSYMBOL_336_29 = 336,                   /* $@29  */
  YYSYMBOL_expr_call = 337,                /* expr_call  */
  YYSYMBOL_expr = 338,                     /* expr  */
  YYSYMBOL_339_30 = 339,                   /* $@30  */
  YYSYMBOL_340_31 = 340,                   /* $@31  */
  YYSYMBOL_341_32 = 341,                   /* $@32  */
  YYSYMBOL_342_33 = 342,                   /* $@33  */
  YYSYMBOL_343_34 = 343,                   /* $@34  */
  YYSYMBOL_344_35 = 344,                   /* $@35  */
  YYSYMBOL_expr_generator = 345,           /* expr_generator  */
  YYSYMBOL_expr_mtag = 346,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 347, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 348,        /* optional_override  */
  YYSYMBOL_optional_constant = 349,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 350, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 351, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 352, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 353, /* struct_variable_declaration_list  */
  YYSYMBOL_354_36 = 354,                   /* $@36  */
  YYSYMBOL_355_37 = 355,                   /* $@37  */
  YYSYMBOL_356_38 = 356,                   /* $@38  */
  YYSYMBOL_function_argument_declaration = 357, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 358,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 359,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 360,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 361,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 362,             /* variant_type  */
  YYSYMBOL_variant_type_list = 363,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 364,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 365,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 366,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 367,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 368,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 369, /* let_variable_name_with_pos_list  */
  YYSYMBOL_global_let_variable_name_with_pos_list = 370, /* global_let_variable_name_with_pos_list  */
  YYSYMBOL_variable_declaration_list = 371, /* variable_declaration_list  */
  YYSYMBOL_let_variable_declaration = 372, /* let_variable_declaration  */
  YYSYMBOL_global_let_variable_declaration = 373, /* global_let_variable_declaration  */
  YYSYMBOL_optional_shared = 374,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 375, /* optional_public_or_private_variable  */
  YYSYMBOL_global_variable_declaration_list = 376, /* global_variable_declaration_list  */
  YYSYMBOL_377_39 = 377,                   /* $@39  */
  YYSYMBOL_global_let = 378,               /* global_let  */
  YYSYMBOL_379_40 = 379,                   /* $@40  */
  YYSYMBOL_enum_expression = 380,          /* enum_expression  */
  YYSYMBOL_commas = 381,                   /* commas  */
  YYSYMBOL_enum_list = 382,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 383, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 384,             /* single_alias  */
  YYSYMBOL_385_41 = 385,                   /* $@41  */
  YYSYMBOL_alias_declaration = 386,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 387, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 388,                /* enum_name  */
  YYSYMBOL_optional_enum_basic_type_declaration = 389, /* optional_enum_basic_type_declaration  */
  YYSYMBOL_optional_commas = 390,          /* optional_commas  */
  YYSYMBOL_emit_commas = 391,              /* emit_commas  */
  YYSYMBOL_optional_emit_commas = 392,     /* optional_emit_commas  */
  YYSYMBOL_enum_declaration = 393,         /* enum_declaration  */
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
  YYSYMBOL_406_47 = 406,                   /* $@47  */
  YYSYMBOL_variable_name_with_pos_list = 407, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 408,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 409, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 410, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 411,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 412,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 413,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 414, /* bitfield_type_declaration  */
  YYSYMBOL_415_48 = 415,                   /* $@48  */
  YYSYMBOL_416_49 = 416,                   /* $@49  */
  YYSYMBOL_c_or_s = 417,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 418,          /* table_type_pair  */
  YYSYMBOL_dim_list = 419,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 420, /* type_declaration_no_options  */
  YYSYMBOL_type_declaration_no_options_no_dim = 421, /* type_declaration_no_options_no_dim  */
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
  YYSYMBOL_448_76 = 448,                   /* $@76  */
  YYSYMBOL_type_declaration = 449,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 450,  /* tuple_alias_declaration  */
  YYSYMBOL_451_77 = 451,                   /* $@77  */
  YYSYMBOL_452_78 = 452,                   /* $@78  */
  YYSYMBOL_453_79 = 453,                   /* $@79  */
  YYSYMBOL_454_80 = 454,                   /* $@80  */
  YYSYMBOL_variant_alias_declaration = 455, /* variant_alias_declaration  */
  YYSYMBOL_456_81 = 456,                   /* $@81  */
  YYSYMBOL_457_82 = 457,                   /* $@82  */
  YYSYMBOL_458_83 = 458,                   /* $@83  */
  YYSYMBOL_459_84 = 459,                   /* $@84  */
  YYSYMBOL_bitfield_alias_declaration = 460, /* bitfield_alias_declaration  */
  YYSYMBOL_461_85 = 461,                   /* $@85  */
  YYSYMBOL_462_86 = 462,                   /* $@86  */
  YYSYMBOL_463_87 = 463,                   /* $@87  */
  YYSYMBOL_464_88 = 464,                   /* $@88  */
  YYSYMBOL_make_decl = 465,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 466,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 467,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 468,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim_list = 469,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 470,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 471, /* optional_make_struct_dim_decl  */
  YYSYMBOL_use_initializer = 472,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 473,         /* make_struct_decl  */
  YYSYMBOL_474_89 = 474,                   /* $@89  */
  YYSYMBOL_475_90 = 475,                   /* $@90  */
  YYSYMBOL_476_91 = 476,                   /* $@91  */
  YYSYMBOL_477_92 = 477,                   /* $@92  */
  YYSYMBOL_478_93 = 478,                   /* $@93  */
  YYSYMBOL_479_94 = 479,                   /* $@94  */
  YYSYMBOL_480_95 = 480,                   /* $@95  */
  YYSYMBOL_481_96 = 481,                   /* $@96  */
  YYSYMBOL_make_map_tuple = 482,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 483,          /* make_tuple_call  */
  YYSYMBOL_484_97 = 484,                   /* $@97  */
  YYSYMBOL_485_98 = 485,                   /* $@98  */
  YYSYMBOL_make_dim_decl = 486,            /* make_dim_decl  */
  YYSYMBOL_487_99 = 487,                   /* $@99  */
  YYSYMBOL_488_100 = 488,                  /* $@100  */
  YYSYMBOL_489_101 = 489,                  /* $@101  */
  YYSYMBOL_490_102 = 490,                  /* $@102  */
  YYSYMBOL_491_103 = 491,                  /* $@103  */
  YYSYMBOL_492_104 = 492,                  /* $@104  */
  YYSYMBOL_493_105 = 493,                  /* $@105  */
  YYSYMBOL_494_106 = 494,                  /* $@106  */
  YYSYMBOL_495_107 = 495,                  /* $@107  */
  YYSYMBOL_496_108 = 496,                  /* $@108  */
  YYSYMBOL_expr_map_tuple_list = 497,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 498,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 499, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 500,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 501       /* array_comprehension  */
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
         || (defined DAS2_YYLTYPE_IS_TRIVIAL && DAS2_YYLTYPE_IS_TRIVIAL \
             && defined DAS2_YYSTYPE_IS_TRIVIAL && DAS2_YYSTYPE_IS_TRIVIAL)))

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
#define YYLAST   11785

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  207
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  295
/* YYNRULES -- Number of rules.  */
#define YYNRULES  838
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1531

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   434


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
       2,     2,     2,   189,     2,   206,   201,   185,   178,     2,
     199,   200,   183,   182,   172,   181,   196,   184,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   175,   203,
     179,   173,   180,   174,   202,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   197,     2,   198,   177,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   204,   176,   205,   188,     2,     2,     2,
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
     165,   166,   167,   168,   169,   170,   171,   186,   187,   190,
     191,   192,   193,   194,   195
};

#if DAS2_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   543,   543,   544,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   565,   566,   570,
     571,   575,   581,   582,   583,   587,   588,   592,   610,   611,
     612,   613,   617,   618,   622,   627,   636,   644,   660,   665,
     673,   673,   712,   730,   734,   737,   741,   747,   756,   759,
     765,   766,   770,   774,   775,   779,   782,   788,   794,   797,
     803,   804,   808,   809,   813,   814,   818,   819,   820,   829,
     830,   834,   835,   841,   842,   843,   844,   845,   849,   850,
     854,   855,   859,   861,   859,   873,   873,   881,   881,   897,
     904,   904,   913,   921,   921,   934,   934,   946,   953,   954,
     955,   956,   957,   958,   962,   967,   975,   976,   977,   981,
     982,   983,   984,   985,   986,   987,   988,   994,   997,  1003,
    1006,  1012,  1013,  1014,  1018,  1031,  1049,  1052,  1060,  1071,
    1082,  1093,  1096,  1103,  1107,  1114,  1115,  1119,  1120,  1124,
    1125,  1126,  1130,  1133,  1140,  1144,  1145,  1146,  1147,  1148,
    1149,  1150,  1151,  1152,  1153,  1154,  1155,  1156,  1157,  1158,
    1159,  1160,  1161,  1162,  1163,  1164,  1165,  1166,  1167,  1168,
    1169,  1170,  1171,  1172,  1173,  1174,  1175,  1176,  1177,  1178,
    1179,  1180,  1181,  1182,  1183,  1184,  1185,  1186,  1187,  1188,
    1189,  1190,  1191,  1192,  1193,  1194,  1195,  1196,  1197,  1198,
    1199,  1200,  1201,  1202,  1203,  1204,  1205,  1206,  1207,  1208,
    1209,  1210,  1211,  1212,  1213,  1214,  1215,  1216,  1217,  1218,
    1219,  1220,  1221,  1222,  1223,  1224,  1225,  1226,  1227,  1232,
    1250,  1251,  1252,  1256,  1262,  1262,  1279,  1282,  1284,  1282,
    1292,  1294,  1292,  1309,  1322,  1323,  1324,  1325,  1326,  1327,
    1328,  1329,  1330,  1331,  1332,  1333,  1334,  1335,  1336,  1337,
    1338,  1339,  1340,  1344,  1349,  1355,  1361,  1362,  1366,  1367,
    1371,  1375,  1382,  1383,  1394,  1398,  1401,  1409,  1409,  1409,
    1412,  1418,  1421,  1425,  1429,  1436,  1443,  1449,  1453,  1457,
    1460,  1463,  1471,  1474,  1482,  1488,  1489,  1490,  1494,  1495,
    1499,  1500,  1504,  1509,  1517,  1523,  1535,  1538,  1541,  1547,
    1547,  1547,  1550,  1550,  1550,  1555,  1555,  1555,  1563,  1563,
    1563,  1569,  1579,  1590,  1605,  1608,  1614,  1615,  1622,  1633,
    1634,  1635,  1639,  1640,  1641,  1642,  1643,  1647,  1652,  1660,
    1661,  1665,  1672,  1676,  1682,  1683,  1684,  1685,  1686,  1687,
    1688,  1692,  1693,  1694,  1695,  1696,  1697,  1698,  1699,  1700,
    1701,  1702,  1703,  1704,  1705,  1706,  1707,  1708,  1709,  1710,
    1714,  1721,  1733,  1738,  1748,  1752,  1759,  1762,  1762,  1762,
    1767,  1767,  1767,  1780,  1784,  1788,  1793,  1800,  1809,  1814,
    1821,  1821,  1821,  1828,  1832,  1842,  1851,  1860,  1864,  1867,
    1873,  1874,  1875,  1876,  1877,  1878,  1879,  1880,  1881,  1882,
    1883,  1884,  1885,  1886,  1887,  1888,  1889,  1890,  1891,  1892,
    1893,  1894,  1895,  1896,  1897,  1898,  1899,  1900,  1901,  1902,
    1903,  1904,  1905,  1906,  1907,  1908,  1914,  1915,  1916,  1917,
    1918,  1931,  1932,  1933,  1934,  1935,  1936,  1937,  1938,  1939,
    1940,  1941,  1942,  1943,  1944,  1947,  1947,  1947,  1950,  1955,
    1959,  1963,  1963,  1963,  1968,  1971,  1975,  1975,  1975,  1980,
    1983,  1984,  1985,  1986,  1987,  1988,  1989,  1990,  1991,  1993,
    1997,  1998,  2006,  2009,  2012,  2021,  2022,  2023,  2024,  2025,
    2026,  2027,  2031,  2035,  2039,  2043,  2047,  2051,  2055,  2059,
    2063,  2070,  2071,  2075,  2076,  2077,  2081,  2082,  2086,  2087,
    2088,  2092,  2093,  2097,  2108,  2111,  2112,  2112,  2131,  2130,
    2144,  2143,  2159,  2168,  2178,  2179,  2183,  2186,  2195,  2196,
    2200,  2203,  2207,  2221,  2230,  2231,  2235,  2238,  2242,  2256,
    2257,  2261,  2267,  2273,  2276,  2280,  2289,  2290,  2291,  2295,
    2296,  2300,  2307,  2312,  2321,  2327,  2338,  2345,  2354,  2357,
    2360,  2367,  2370,  2375,  2386,  2389,  2394,  2406,  2407,  2411,
    2412,  2413,  2417,  2420,  2423,  2423,  2443,  2446,  2446,  2464,
    2469,  2477,  2478,  2482,  2485,  2498,  2515,  2516,  2517,  2522,
    2522,  2548,  2552,  2553,  2554,  2558,  2568,  2571,  2577,  2578,
    2582,  2583,  2587,  2588,  2592,  2594,  2599,  2592,  2615,  2616,
    2620,  2621,  2625,  2631,  2632,  2636,  2637,  2638,  2642,  2645,
    2651,  2653,  2658,  2651,  2673,  2680,  2685,  2694,  2700,  2711,
    2712,  2713,  2714,  2715,  2716,  2717,  2718,  2719,  2720,  2721,
    2722,  2723,  2724,  2725,  2726,  2727,  2728,  2729,  2730,  2731,
    2732,  2733,  2734,  2735,  2736,  2737,  2741,  2742,  2743,  2744,
    2745,  2746,  2747,  2748,  2752,  2763,  2767,  2774,  2786,  2793,
    2802,  2807,  2817,  2830,  2830,  2830,  2843,  2844,  2848,  2852,
    2859,  2863,  2867,  2871,  2878,  2881,  2899,  2900,  2901,  2902,
    2903,  2903,  2903,  2907,  2912,  2919,  2919,  2926,  2930,  2934,
    2939,  2944,  2949,  2954,  2958,  2962,  2967,  2971,  2975,  2980,
    2980,  2980,  2986,  2993,  2993,  2993,  2998,  2998,  2998,  3004,
    3004,  3004,  3009,  3013,  3013,  3013,  3018,  3018,  3018,  3027,
    3031,  3031,  3031,  3036,  3036,  3036,  3045,  3049,  3049,  3049,
    3054,  3054,  3054,  3063,  3063,  3063,  3069,  3069,  3069,  3078,
    3081,  3092,  3108,  3110,  3115,  3120,  3108,  3146,  3148,  3153,
    3159,  3146,  3185,  3187,  3192,  3197,  3185,  3228,  3229,  3230,
    3231,  3232,  3236,  3243,  3250,  3256,  3262,  3269,  3276,  3282,
    3291,  3297,  3305,  3310,  3317,  3322,  3328,  3329,  3333,  3334,
    3338,  3338,  3338,  3346,  3346,  3346,  3353,  3353,  3353,  3360,
    3360,  3360,  3371,  3377,  3383,  3389,  3389,  3389,  3399,  3408,
    3408,  3408,  3418,  3418,  3418,  3428,  3428,  3428,  3438,  3447,
    3447,  3447,  3467,  3474,  3474,  3474,  3484,  3487,  3493,  3501,
    3509,  3529,  3554,  3555,  3559,  3560,  3564,  3567,  3570
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
  "\"variant\"", "\"generator\"", "\"yield\"", "\"sealed\"", "\"+=\"",
  "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"", "\"^=\"",
  "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"", "\">>=\"",
  "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"", "\"??\"", "\"?.\"",
  "\"?[\"", "\"<|\"", "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"",
  "\">>>=\"", "\"=>\"", "\"::\"", "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"",
  "\"||=\"", "\"^^=\"", "\"..\"", "\"$$\"", "\"$i\"", "\"$v\"", "\"$b\"",
  "\"$a\"", "\"$t\"", "\"$c\"", "\"$f\"", "\"...\"",
  "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"unsigned int8 constant\"", "\"floating point constant\"",
  "\"double constant\"", "\"name\"", "\"new line, comma\"",
  "\"new line, semicolon\"", "\"start of the string\"", "STRING_CHARACTER",
  "STRING_CHARACTER_ESC", "\"end of the string\"", "\"{\"", "\"}\"",
  "\"end of failed eader macro\"", "','", "'='", "'?'", "':'", "'|'",
  "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'",
  "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC",
  "LLPIPE", "POST_INC", "POST_DEC", "DEREF", "'.'", "'['", "']'", "'('",
  "')'", "'$'", "'@'", "';'", "'{'", "'}'", "'#'", "$accept", "program",
  "COMMA", "SEMICOLON", "top_level_reader_macro",
  "optional_public_or_private_module", "module_name", "module_declaration",
  "character_sequence", "string_constant", "string_builder_body",
  "string_builder", "reader_character_sequence", "expr_reader", "$@1",
  "options_declaration", "require_declaration", "require_module_name",
  "require_module", "is_public_module", "expect_declaration",
  "expect_list", "expect_error", "expression_label", "expression_goto",
  "elif_or_static_elif", "emit_semis", "optional_emit_semis",
  "expression_else", "if_or_static_if", "expression_else_one_liner",
  "expression_if_one_liner", "semis", "optional_semis",
  "expression_if_block", "$@2", "$@3", "$@4", "expression_if_then_else",
  "$@5", "expression_if_then_else_oneliner", "expression_for_loop", "$@6",
  "expression_unsafe", "expression_while_loop", "$@7", "expression_with",
  "$@8", "expression_with_alias", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "metadata_argument_list", "annotation_declaration_name",
  "annotation_declaration_basic", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_annotation_list_with_emit_semis",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@9", "expression_block_finally", "$@10",
  "$@11", "expression_block", "$@12", "$@13", "expr_call_pipe",
  "expression_any", "expressions", "optional_expr_list",
  "optional_expr_map_tuple_list", "type_declaration_no_options_list",
  "name_in_namespace", "expression_delete", "new_type_declaration", "$@14",
  "$@15", "expr_new", "expression_break", "expression_continue",
  "expression_return", "expression_yield", "expression_try_catch",
  "kwd_let_var_or_nothing", "kwd_let", "optional_in_scope",
  "tuple_expansion", "tuple_expansion_variable_declaration",
  "expression_let", "expr_cast", "$@16", "$@17", "$@18", "$@19", "$@20",
  "$@21", "expr_type_decl", "$@22", "$@23", "expr_type_info", "expr_list",
  "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_full_block",
  "expr_full_block_assumed_piped", "expr_numeric_const", "expr_assign",
  "expr_named_call", "expr_method_call", "func_addr_name",
  "func_addr_expr", "$@24", "$@25", "$@26", "$@27", "expr_field", "$@28",
  "$@29", "expr_call", "expr", "$@30", "$@31", "$@32", "$@33", "$@34",
  "$@35", "expr_generator", "expr_mtag", "optional_field_annotation",
  "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@36", "$@37", "$@38",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list",
  "global_let_variable_name_with_pos_list", "variable_declaration_list",
  "let_variable_declaration", "global_let_variable_declaration",
  "optional_shared", "optional_public_or_private_variable",
  "global_variable_declaration_list", "$@39", "global_let", "$@40",
  "enum_expression", "commas", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@41",
  "alias_declaration", "optional_public_or_private_enum", "enum_name",
  "optional_enum_basic_type_declaration", "optional_commas", "emit_commas",
  "optional_emit_commas", "enum_declaration", "$@42", "$@43", "$@44",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@45", "$@46", "$@47", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_alias_bits", "bitfield_type_declaration", "$@48", "$@49",
  "c_or_s", "table_type_pair", "dim_list", "type_declaration_no_options",
  "type_declaration_no_options_no_dim", "$@50", "$@51", "$@52", "$@53",
  "$@54", "$@55", "$@56", "$@57", "$@58", "$@59", "$@60", "$@61", "$@62",
  "$@63", "$@64", "$@65", "$@66", "$@67", "$@68", "$@69", "$@70", "$@71",
  "$@72", "$@73", "$@74", "$@75", "$@76", "type_declaration",
  "tuple_alias_declaration", "$@77", "$@78", "$@79", "$@80",
  "variant_alias_declaration", "$@81", "$@82", "$@83", "$@84",
  "bitfield_alias_declaration", "$@85", "$@86", "$@87", "$@88",
  "make_decl", "make_struct_fields", "make_variant_dim",
  "make_struct_single", "make_struct_dim_list", "make_struct_dim_decl",
  "optional_make_struct_dim_decl", "use_initializer", "make_struct_decl",
  "$@89", "$@90", "$@91", "$@92", "$@93", "$@94", "$@95", "$@96",
  "make_map_tuple", "make_tuple_call", "$@97", "$@98", "make_dim_decl",
  "$@99", "$@100", "$@101", "$@102", "$@103", "$@104", "$@105", "$@106",
  "$@107", "$@108", "expr_map_tuple_list", "make_table_decl",
  "array_comprehension_where", "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1395)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-741)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1395,   427, -1395, -1395,    34,   -74,   261,   282, -1395,   -36,
   -1395, -1395, -1395, -1395,   139,   122, -1395, -1395, -1395, -1395,
     123,   123,   123, -1395,    75,   236, -1395,   226, -1395, -1395,
   -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395,   137, -1395,
     140,   148,   175, -1395, -1395,   261,    15, -1395, -1395, -1395,
     195,   123, -1395, -1395,   226,   282,   282,   282,   263,   324,
   -1395, -1395, -1395,   122,   122,   122,   274, -1395,   606,   171,
   -1395, -1395, -1395, -1395, -1395,   451,   169, -1395,   478,    24,
      34,   326,   -74,   293,   329, -1395,   384,   464, -1395, -1395,
   -1395,   543,   484,   509,   517, -1395,   519,   498, -1395, -1395,
     -73,    34,   122,   122,   122,   122,    29,   592, -1395, -1395,
   -1395, -1395, -1395, -1395,   602, -1395, -1395,   465, -1395, -1395,
   -1395, -1395, -1395,   584,    94, -1395, -1395, -1395, -1395,   629,
   -1395, -1395,   528, -1395, -1395, -1395,   549,   553,   553, -1395,
   -1395,   559, -1395,     8, -1395,   270,   580,   606, -1395,   563,
   -1395, -1395, -1395,   596,  7437, -1395, -1395, -1395, -1395,   561,
   -1395, -1395, -1395,   609, -1395, -1395, -1395, -1395, -1395, -1395,
   -1395,    55, -1395,  7185, -1395,   615, -1395, -1395, -1395, -1395,
   -1395, -1395, -1395,   590, 10639, -1395, -1395, -1395, -1395, -1395,
   -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395,
   -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395,
     747,   748, -1395,   585,   553,   680,   117,    34,   586,   625,
   -1395, -1395, -1395,    94, -1395,   610,   611,   613,   599,   620,
     621, -1395, -1395, -1395,   603, -1395, -1395, -1395, -1395, -1395,
     624, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395,
   -1395, -1395,   626, -1395, -1395, -1395,   627,   628, -1395, -1395,
   -1395, -1395,   631,   632,   614,   139, -1395, -1395, -1395, -1395,
   -1395, -1395,   101,   640, -1395,   600,   616,   617,   764,   549,
     653,   655, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395,
   -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395,
   -1395, -1395,   656,   633, -1395, -1395, -1395, -1395, -1395, -1395,
   -1395, -1395, -1395, -1395, -1395, -1395,   769, -1395, -1395, -1395,
   -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395,   657,
     634, -1395, -1395,   -86,   647, -1395, -1395,   662,   553, -1395,
   -1395,   561,   553,    34, -1395,   401, -1395, -1395, -1395, -1395,
   -1395,  6664, -1395, -1395,   665, -1395,   211,   372,   408, -1395,
   -1395,  6664,    53, -1395, -1395, -1395,    14, -1395, -1395, -1395,
      16,  3814, -1395,   636,  6999, -1395, -1395, -1395, -1395, -1395,
   -1395, -1395, -1395, -1395, -1395, -1395, -1395,   638, -1395, -1395,
   -1395, -1395,   667,   698, -1395,   646, -1395,   132, -1395,  -114,
    7185, -1395, -1395,   643,   673, -1395,    26, -1395,   553,   675,
    7185, -1395,   -26,  7185,  7185,  7185,   670,   674, -1395, -1395,
      99,   139,   676,    41, -1395,   243,   660,   677,   681,   669,
     682,   671,   265,   687, -1395,   277,   690,   692,  6664,  6664,
     678,   683,   684,   685,   688,   689, -1395, -1395, -1395, -1395,
   -1395, -1395, -1395, -1395, -1395,  6664,  6664,  6664,  6664,  6664,
    3434,  6664, -1395,   650,  4004, -1395, -1395, -1395,   696, -1395,
   -1395, -1395, -1395,   679, -1395, -1395, -1395, -1395, -1395, -1395,
     531,  7279, -1395, -1395,   697, -1395, -1395, -1395, -1395, -1395,
   -1395,  7185,  7185,   672,   713,  7185,   585,  7185,   585,  7185,
     585,  7277,   717,  7366, -1395,  6664, -1395, -1395, -1395, -1395,
     700, -1395, -1395,   944,  4194, -1395, -1395,   718,  7277,   717,
   -1395, -1395, -1395,  6664, -1395, -1395,   398, -1395,   -49,   640,
   -1395,   139, -1395,   506,   701,   838,   475, -1395, -1395, -1395,
     -71, -1395, -1395, -1395,  6664,   410,   449,   721,   267, -1395,
   -1395, -1395,   703, -1395, -1395,   354, -1395,   725,   728,   729,
   -1395,  6664,  7185,  6664,  6664, -1395, -1395,  6664, -1395,  6664,
   -1395,  6664, -1395, -1395,  6664, -1395,  7185,   639,   639,  6664,
    6664,  6664,  6664,  6664,  6664,   593,   639,   639,   -38,   639,
     639,   710,   897,   739, 10974,   739,   152,   715, 10722, -1395,
     740,  3054,   122,   914,   720, -1395,   679, -1395,  1446, 10793,
    6664,  6664, -1395, -1395,  6664,  6664,  6664,  6664,   757,  6664,
     191,  6664,  6664,  6664,  6664,  6664,  6664,  6664,  6664,  6664,
    4384,  6664,  6664,  6664,  6664,  6664,  6664,  6664,  6664,  6664,
    6664, 11588,  6664, -1395,  4574,   494,   500, -1395, -1395,   272,
     502,   647,   508,   647,   516,   647,    25, -1395,   218,   640,
     745, -1395,   315, -1395,  7185,   723,   739, -1395, -1395, -1395,
   10204, -1395,   385, -1395,   123, -1395,   123,   762,  7452,   726,
     883, -1395,    91, -1395,  2048, -1395, -1395, -1395, -1395,  6664,
     766,   767,  7185,   -26, -1395,  6664,  1242, -1395, -1395, -1395,
   -1395, -1395, -1395, -1395,  7185,  7185,  7185,  7185,  3624,   768,
    6664,  7185, -1395, -1395, -1395,  7185,   739,   356,   740,  7538,
    7185,  7185,  7624,  7185,  7710,  7185,   739,  7185,  7185,   739,
    7185,   751,  7796,  7882,  7968,  8054,  8140,  8226, -1395,  6664,
     527,   398,   733,  6664,   735,   736,   738,   741,   419, -1395,
   -1395,   398,  6664,  6664,   730,   -45,   742,   103,  3244, -1395,
     163,   771,   746,   189,   750,   585, -1395,  2253,   914,   760,
     752, -1395, -1395,   765,   753, -1395, -1395,   612,   612,   320,
     320, 11430, 11430,   754,   510,   755, -1395, 10288,   -55,   -55,
     697,   612,   612,  1059, 11171, 11202, 11057, 11557, 10808, 11285,
   11316, 11399,   320,   320,   188,   188,   510,   510,   510,   455,
    6664,   756,   758,   458,  6664,   946,   761, 10372, -1395,   165,
   -1395, -1395, -1395,   783, -1395,   776, -1395,   779, -1395,   781,
    7185, -1395,  7277,  7185, -1395,   717,   362,   640, -1395, -1395,
   -1395, -1395,   447, -1395, -1395,  7091, -1395,   133, -1395,   789,
   -1395,   385, -1395,  6664,   801, -1395,   802, -1395,  7185, -1395,
    6664, -1395,  4764, -1395, -1395,  4954,   805, -1395,   123,   813,
    5144,   -12,  5334, -1395,   123,   123,   959, -1395,   744, -1395,
   -1395,   957, -1395, -1395,   962, -1395,   930,   123,   770, -1395,
     123,   123,   123,   123,   123, -1395,   907, -1395,   123,  1686,
      33,  8312, -1395,   931,   -71,  6664,  1242, -1395, -1395, -1395,
   -1395,   640,   796,   797,   546,   483,   183,   778,   780,   441,
    8398,   548,  7185,  7185,  7185,   799,   782,   784,  7185,   785,
   -1395,   804,   806, -1395,   807, -1395,   808,   790,   811,   814,
     793,   815,   914, -1395, -1395, -1395, -1395, -1395,   798, 10891,
      23,   398, 10974, -1395, -1395,  6664,  6664,  7185,   585,    32,
   10974, -1395, -1395,   -19, -1395,   800,  6664,  6664,  6664,   190,
    6854, -1395,   490, -1395, -1395,   -50,   647, -1395,   585, -1395,
    6664, -1395,  6664,  5524,  6664, -1395,   817,   803, -1395, -1395,
    6664,   809, -1395, 10456,  6664,  5714,   810, -1395, 10540, -1395,
    5904, -1395,  6664, -1395, -1395, -1395, -1395, -1395, -1395, -1395,
   -1395, -1395, -1395, -1395,   640, -1395, -1395,   640, -1395, -1395,
     816,  7185, -1395, -1395,   794, -1395, -1395,   818, -1395,   819,
    6664,   460, -1395,  8484, -1395,   938,    45, 10974,  6664, 10974,
     984,  6664, 10974,   828, -1395,   829,   848, 10974, -1395,  6664,
   10974, -1395, -1395,   820, -1395, -1395,   821,   822,   823,   830,
   -1395,   968, -1395, -1395, -1395, -1395, -1395, -1395,   -84, -1395,
    6664,  6664,  6664,  6664,  6664,  6664,  6664,  6664,  6664,  6664,
    6664,  6664,  6664,  6664,  6664,  6664,  6664,  6664, -1395,   122,
   -1395,   998,   561, -1395,   845, -1395,  6664,  1242, -1395, -1395,
   -1395, -1395,   812, -1395, -1395, -1395,   831,   854, -1395, -1395,
     847,   851,   852, -1395, -1395,  6664,   853, -1395, -1395, -1395,
   -1395, -1395, -1395, -1395, -1395, -1395, -1395,     6,  6094, -1395,
    6664,    40,  8570,  8656,   855,   647,  6664, -1395,  8742, 10974,
   10974,   834,  3244,   837,     5,   856,   866,   839,   875,   877,
   -1395,   220,   553,   647,  7185,  8828,  7185,  8914, -1395,   224,
    9000, -1395,  6664, 11088,  6664, -1395,  9086,  3244, -1395,   240,
    6664, -1395, -1395, -1395,   260, -1395, -1395, -1395,  6664,   640,
   -1395, -1395, -1395, 10974, -1395,   835, -1395,   879,  6664, 10974,
   -1395, 10974,  6664, -1395, -1395, -1395, 10974,  6664,  6664,   398,
    6664,  6664, -1395, -1395,   885, -1395, -1395, -1395, 10974, 10974,
   10974, 10974, 10974, 10974, 10974, 10974, 10974, 10974, 10974, 10974,
   10974, 10974, 10974, 10974, 10974, 10974,   228,   605,   943,   123,
   -1395,  1242, -1395,   849,   850, -1395, -1395,  6664,   870, -1395,
   -1395, -1395, -1395,   857,   858,   740,   865,  6664,  6664,  6664,
     871,   982,   880,   887,  6284, -1395, -1395,   269,   286,  6664,
   -1395, -1395, -1395,   873,   296,   136, -1395,   257,  6664,  6664,
    6664, -1395, -1395,   892, -1395, -1395,   -50, -1395,   918,   553,
     896, -1395,   558, -1395, -1395, -1395,  7185,  9172,  9258, -1395,
     340, -1395,  9344, -1395,   888, -1395, -1395, 10974, -1395, 10974,
    9430,  9516,    42,  9602,  9688,   861, -1395,   299,   -70,   553,
   -1395, -1395,   507, -1395,    13, -1395, -1395,   982,   982,  9774,
     891,   893,   894,   898,  6664, -1395, -1395,  6664,   320,   320,
     320,  6664, -1395, -1395,   982,   483, -1395,  9860, -1395, -1395,
     884,   310,   454, -1395,   895,  6664,  6664,   899,  9946, 10974,
   10974,   900, -1395,  6474, -1395, -1395, -1395, -1395, -1395,   568,
   -1395, -1395,   902, -1395, -1395,  1045,   553,  6664,   553,   553,
   -1395,   929,   473, -1395, -1395, -1395,  1062,    13, -1395, -1395,
     605,    81,    81, -1395,  6664,   982,   982,   483,   903,   904,
     739,    81,   771,   905, -1395,  6664,   906, -1395,   454,  6664,
   10974, 10974, -1395,   201, -1395,  6664, 11057, -1395, -1395, -1395,
   -1395, -1395,  2864,   123,   908,   343, -1395, -1395,  2458, -1395,
    7185,   -26, -1395, -1395,  1062,   398,   483,   771,   934, -1395,
     911,   913, 10032,    81,    81,   915, -1395, -1395,   916,   917,
   -1395,  1770,  6664, -1395,   927,  6664,  6664, 11088, -1395, -1395,
   -1395, -1395, -1395, -1395, 10974, -1395,   591,   910,  2864,   553,
   -1395, -1395,   928,   -26,  6664,  7437, -1395, -1395,   350,   909,
   -1395, -1395, -1395, -1395,   932,   935, -1395, -1395, -1395,  1054,
     936,  1770,   933, 10974, 10974,   553, -1395, -1395,   937, -1395,
   -1395,   123, -1395, -1395,  6664,  1242,   123,  7437, -1395,   483,
   -1395, -1395,  6664, -1395,   939, -1395, -1395,  6664,  2663, -1395,
   -1395,  1242, -1395, -1395,   553,   351, 10974, -1395, -1395, 10118,
     940, -1395, -1395, -1395,   553,   968, -1395, -1395, -1395,   591,
   -1395
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   137,     1,   298,     0,     0,     0,   586,   299,     0,
     762,   752,   757,    20,     0,     0,    19,    16,    15,     3,
       0,     0,     0,     8,     0,   620,     7,   567,     6,    11,
       5,     4,    13,    12,    14,   107,   108,   106,   115,   117,
      42,    55,    52,    53,    44,     0,    50,    43,   588,   587,
       0,     0,    26,    25,   567,   586,   586,   586,     0,   272,
      40,   122,   123,     0,     0,     0,   124,   126,   133,     0,
     121,    21,    10,     9,   604,   230,     0,   568,   569,     0,
       0,     0,     0,    45,     0,    51,     0,     0,    48,   589,
     591,    22,     0,     0,     0,   274,     0,     0,   132,   127,
       0,     0,     0,     0,     0,     0,    64,   592,   232,   231,
     234,   229,   614,   613,   615,   571,   570,   577,   113,   114,
     111,   112,   110,     0,     0,   109,   118,    56,    54,    50,
      47,    46,     0,    23,    24,    27,   602,    64,    64,   273,
      38,    41,   131,     0,   128,   129,   130,   134,    62,    65,
     138,   594,   593,     0,     0,   617,   616,   621,   572,   501,
      28,    29,    33,     0,   102,   103,   100,   101,    99,    98,
     104,     0,    49,     0,   600,   603,   763,   753,   758,    39,
     125,    63,   595,   596,     0,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
       0,     0,   144,   139,    64,   610,   574,     0,   502,     0,
      30,    31,    32,     0,   116,     0,     0,     0,     0,     0,
       0,   629,   649,   630,   665,   631,   635,   636,   637,   638,
     655,   642,   643,   644,   645,   646,   647,   648,   650,   651,
     652,   653,   722,   634,   641,   654,   729,   736,   632,   639,
     633,   640,     0,     0,     0,     0,   664,   686,   689,   687,
     688,   749,   684,   590,   601,     0,     0,     0,     0,   602,
     198,   199,   196,   147,   148,   150,   149,   151,   152,   153,
     154,   180,   181,   178,   179,   171,   182,   183,   172,   169,
     170,   197,   191,     0,   195,   184,   185,   186,   187,   158,
     159,   160,   155,   156,   157,   168,     0,   174,   175,   173,
     166,   167,   162,   161,   163,   164,   165,   146,   145,   190,
       0,   176,   177,   501,   142,   240,   611,     0,    64,   576,
     573,   501,    64,     0,   556,   549,   578,   105,   690,   713,
     716,     0,   719,   709,     0,   673,   723,   730,   737,   743,
     746,     0,     0,   699,   704,   698,     0,   712,   708,   701,
       0,     0,   703,   685,     0,   764,   754,   759,   656,   659,
     662,   663,   657,   660,   658,   661,   597,     0,   200,   201,
     194,   189,   202,   192,   188,     0,   140,   297,   524,     0,
       0,   233,   235,     0,   608,   622,     0,   119,    64,     0,
       0,   550,     0,     0,     0,     0,     0,     0,   406,   407,
       0,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   655,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   490,   344,   346,   345,
     347,   348,   349,   350,    34,     0,     0,     0,     0,     0,
       0,     0,   329,   330,     0,   404,   403,   480,   401,   473,
     472,   471,   470,   135,   476,   402,   475,   474,   447,   408,
     448,     0,   452,   409,     0,   405,   767,   771,   768,   769,
     770,     0,     0,     0,     0,     0,   139,     0,   139,     0,
     139,     0,     0,     0,   695,   266,   706,   707,   700,   702,
       0,   705,   681,     0,     0,   751,   750,   670,   530,   536,
     605,   203,   193,     0,   295,   296,     0,   141,   501,   143,
     263,     0,   612,     0,     0,   551,   549,   575,   120,   557,
       0,   547,   548,   546,     0,     0,     0,     0,   678,   790,
     793,   277,   281,   280,   286,     0,   318,     0,     0,     0,
     819,     0,     0,     0,     0,   309,   312,     0,   315,     0,
     823,     0,   799,   805,     0,   796,     0,   436,   437,     0,
       0,     0,     0,     0,     0,     0,   413,   412,   449,   411,
     410,     0,     0,   834,   324,   834,   331,     0,   803,   826,
     834,     0,     0,   339,   330,   263,   135,   243,     0,     0,
       0,     0,   438,   439,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   390,     0,   693,     0,     0,     0,   666,   668,     0,
       0,   142,     0,   142,     0,   142,   272,   528,     0,   526,
       0,   534,     0,   667,     0,     0,   834,   697,   680,   683,
       0,   671,   598,   531,    80,   537,    80,   583,     0,     0,
     624,   522,   541,   525,     0,   609,   618,   514,   623,     0,
       0,     0,     0,     0,   564,     0,     0,   691,   714,   717,
      18,    17,   676,   677,     0,     0,     0,     0,   788,     0,
       0,     0,   809,   812,   815,     0,   834,     0,   834,     0,
       0,     0,     0,     0,     0,     0,   834,     0,     0,   834,
       0,     0,     0,     0,     0,     0,     0,     0,    37,     0,
      35,     0,     0,   835,     0,     0,     0,     0,   377,   374,
     376,     0,     0,   835,     0,     0,     0,   272,     0,   393,
       0,   781,     0,     0,     0,   139,   331,     0,   339,     0,
       0,   459,   458,     0,     0,   460,   464,   414,   415,   427,
     428,   425,   426,     0,   453,     0,   445,     0,   477,   478,
     479,   416,   417,   432,   433,   434,   435,     0,     0,   430,
     431,   429,   423,   424,   419,   418,   420,   421,   422,     0,
       0,     0,   383,     0,     0,     0,     0,     0,   398,     0,
     720,   710,   674,     0,   724,     0,   731,     0,   738,     0,
       0,   744,     0,     0,   747,     0,     0,   270,   694,   267,
     682,   581,   599,   765,    78,    81,   755,    81,   760,   579,
     584,   598,   523,     0,     0,   540,     0,   539,     0,   542,
       0,   265,   289,   287,   240,     0,     0,   288,     0,     0,
       0,    64,     0,   244,     0,     0,     0,   257,     0,   258,
     252,     0,   249,   248,     0,   250,     0,     0,     0,   264,
       0,    76,    77,    74,    75,   259,   301,   247,     0,   351,
     516,     0,   553,   554,     0,     0,     0,   566,   692,   715,
     718,   679,     0,     0,     0,   789,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     450,     0,     0,   451,     0,   481,     0,     0,     0,     0,
       0,     0,   339,   485,   486,   487,   488,   489,     0,     0,
       0,     0,   325,   808,   440,     0,     0,     0,   139,     0,
     802,   827,   828,     0,   394,     0,     0,     0,     0,     0,
       0,   397,     0,   395,   136,     0,   142,   343,   139,   455,
       0,   461,     0,     0,     0,   443,     0,     0,   465,   469,
       0,     0,   446,     0,     0,     0,     0,   384,     0,   391,
       0,   441,     0,   399,   721,   711,   675,   669,   725,   727,
     732,   734,   739,   741,   527,   745,   529,   533,   748,   535,
       0,     0,   672,   582,     0,    79,   532,     0,   538,     0,
       0,   599,   606,     0,   626,   627,   543,   545,     0,   290,
       0,     0,   275,     0,   262,     0,     0,    59,   240,     0,
     292,   260,   261,     0,    69,    70,     0,     0,     0,     0,
     251,   236,   246,   253,   254,   255,   256,   300,     0,   245,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   515,     0,
     619,     0,   501,   552,     0,   561,     0,     0,   565,   791,
     794,   278,     0,   283,   284,   282,     0,     0,   321,   319,
       0,     0,     0,   820,   818,   268,     0,   829,   310,   313,
     316,   824,   822,   800,   806,   804,   797,    64,     0,    36,
       0,     0,     0,     0,     0,   142,     0,   396,     0,   773,
     772,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     337,     0,    64,   142,     0,     0,     0,     0,   372,     0,
       0,   466,     0,   454,     0,   444,     0,     0,   385,     0,
       0,   442,   392,   388,     0,   728,   735,   742,   266,   271,
     766,   756,   761,   580,   585,     0,   625,     0,     0,   291,
     240,   276,     0,    57,    58,    92,   293,     0,     0,     0,
       0,     0,   237,   242,     0,   558,   307,   306,   361,   362,
     364,   363,   365,   355,   356,   357,   366,   367,   353,   354,
     368,   369,   358,   359,   360,   352,     0,   508,   511,     0,
     555,     0,   563,     0,     0,   279,   285,     0,     0,   320,
     810,   813,   816,     0,     0,   834,     0,     0,     0,     0,
       0,   788,     0,     0,     0,   240,   491,     0,     0,     0,
     375,   500,   378,     0,     0,     0,   370,     0,     0,     0,
       0,   334,   335,     0,   333,   332,     0,   340,   240,    64,
       0,   499,     0,   497,   373,   494,     0,     0,     0,   493,
       0,   386,     0,   389,     0,   607,   628,   544,   294,    97,
       0,     0,     0,     0,     0,     0,   302,     0,     0,    64,
     509,   510,   511,   512,   503,   517,   562,   788,   788,     0,
       0,     0,     0,     0,   266,   830,   269,   268,   311,   314,
     317,     0,   789,   801,   788,     0,   482,     0,   484,   492,
       0,     0,     0,   381,     0,     0,     0,     0,     0,   775,
     774,     0,   338,     0,   326,   341,   240,   456,   462,     0,
     498,   496,     0,   495,   696,    71,    64,     0,    64,    64,
     263,     0,   549,   308,   559,   560,   506,   503,   504,   505,
     508,   787,   787,   322,     0,   788,   788,     0,     0,     0,
     834,   787,   780,     0,   483,     0,     0,   379,     0,     0,
     777,   776,   371,     0,   336,     0,   327,   342,   457,   463,
     467,   387,     0,     0,    85,     0,   240,   240,     0,   303,
       0,     0,   507,   518,   506,     0,     0,   784,   834,   786,
       0,     0,     0,   787,   787,     0,   821,   831,     0,     0,
     798,   832,     0,   382,   832,     0,     0,   328,   468,    72,
      76,    77,    74,    75,    73,    89,    66,     0,     0,    64,
      94,    96,     0,     0,     0,     0,   520,   513,     0,   835,
     785,   792,   795,   323,     0,     0,   817,   825,   807,     0,
       0,   832,     0,   779,   778,    64,    60,    61,     0,    88,
     263,     0,   240,   239,     0,     0,     0,     0,   782,     0,
     811,   814,     0,   836,     0,   838,   240,     0,     0,    86,
      91,     0,   305,   519,    64,     0,   833,   837,    67,     0,
       0,   304,   240,   783,    64,   236,   521,   240,    84,    66,
      68
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1395, -1395,  -661,    -1, -1395, -1395, -1395, -1395,   554,  1067,
   -1395, -1395, -1395,  1147, -1395, -1395, -1395,  1107, -1395,  1024,
   -1395, -1395,  1073, -1395, -1395, -1395, -1395,  -135,  -372, -1395,
   -1395, -1337,   482,   485, -1395, -1395, -1395, -1395, -1395, -1395,
   -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395,   941,
   -1395, -1395,   -57,  1058, -1395, -1395, -1395,   313,  -589,  -429,
     262,  -465,  -626, -1395, -1395, -1395, -1394, -1395, -1395,  -365,
   -1395, -1395,  -850, -1395, -1395, -1395, -1395,  -596, -1133,  -164,
   -1395,    -7, -1395, -1395, -1395, -1395, -1395, -1329, -1313, -1294,
   -1173, -1395, -1395,  1164, -1395, -1395, -1395, -1395, -1395, -1395,
   -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395,  -425,
   -1395,   693,  -104, -1395,  -726, -1395, -1395, -1395, -1395, -1395,
   -1395, -1273, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395,
   -1395,   587, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395,
    -158,  -198,  -244,  -195,  -121, -1395, -1395, -1395, -1395, -1395,
     666, -1395,  -494, -1395, -1395,  -501, -1395, -1395,  -672,  -230,
    -536,  -531, -1395, -1395, -1395, -1034, -1395,  1142, -1395, -1395,
   -1395, -1395, -1395,   168,   349, -1395,   716, -1395, -1395, -1395,
   -1395, -1395, -1395,   381, -1395,   926, -1395, -1395, -1395, -1395,
   -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395,
    -725,  -143, -1395,   787, -1395, -1395, -1395, -1395, -1395, -1395,
    -630, -1395, -1395,  -326, -1395, -1395, -1395, -1395, -1395, -1395,
   -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395,
   -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395,
   -1395, -1395,  -167, -1395, -1395, -1395, -1395, -1395, -1395, -1395,
   -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395,   825,  -586,
    -141,  -670, -1395, -1395, -1140, -1026, -1395, -1395, -1395, -1395,
   -1395, -1395, -1395, -1395, -1395,  -741, -1395, -1395, -1395, -1395,
   -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395, -1395,
    -413, -1395, -1179,  -578, -1395
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   702,   703,    18,   135,    54,    19,   163,   169,
     585,   465,   141,   466,    97,    21,    22,    46,    47,    88,
      23,    42,    43,   874,   875,  1488,   149,   150,  1489,  1056,
    1413,   876,   845,   846,  1456,  1457,  1520,  1458,   877,   878,
     879,   880,   881,   882,   883,   884,   885,   886,   887,   170,
     171,    38,    39,    40,   218,    66,    67,    68,    69,    24,
      25,   334,   401,   213,    26,   110,   214,   111,   154,  1203,
    1305,  1462,   402,   403,   888,   467,   889,   684,   665,  1244,
     836,   468,   890,   552,   707,  1235,   469,   891,   892,   893,
     894,   895,   526,   896,  1068,  1307,  1206,   897,   470,   720,
    1247,   721,  1248,   723,  1249,   471,   711,  1239,   472,   593,
    1355,   473,  1150,  1151,   765,   474,   607,   475,   898,   476,
     477,   750,   478,   957,  1342,   958,  1398,   479,   815,  1172,
     480,   594,  1154,  1408,  1156,  1409,  1286,  1448,   482,   483,
     397,  1380,  1423,  1312,  1314,  1229,   900,  1092,  1465,  1497,
     398,   399,   657,   658,   674,   661,   662,   676,   968,   681,
     544,   412,   536,   345,  1308,   537,   346,    78,   117,   216,
     341,    28,   159,   850,   842,   851,    50,    51,   132,    29,
     153,   183,   279,   843,   175,   176,    30,   107,   677,  1185,
     532,   337,   338,   114,   157,   688,    31,    76,   215,   533,
     682,   484,   386,   268,   269,   649,   672,   270,   494,  1006,
     704,   547,   373,   271,   272,   413,   908,   664,   492,  1005,
     414,   909,   415,   910,   491,  1004,   495,  1008,   496,  1175,
     497,  1010,   498,  1176,   499,  1012,   500,  1177,   501,  1015,
     502,  1018,   659,    32,    56,   276,   518,  1027,    33,    57,
     277,   519,  1029,    34,    55,   275,   517,  1024,   485,  1427,
    1393,   762,  1428,  1429,  1430,   918,   486,   705,  1233,   706,
    1234,   730,  1253,   727,  1251,   599,   487,   728,  1252,   488,
     922,  1321,   923,  1322,   924,  1323,   715,  1243,   725,  1250,
    1245,   489,  1480,   744,   490
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,   219,   177,   178,   695,   693,   273,    60,    70,   767,
     860,   841,   961,   763,  1040,   761,   950,   745,   675,    71,
      72,    73,   754,   126,   673,   825,   959,   827,   832,   829,
     267,   651,   835,   653,  1207,   655,   595,    84,   917,   118,
     119,  -137,   978,  1130,   603,  1294,   557,   506,   516,   508,
      90,   600,  1136,    35,    36,   541,    70,    70,    70,  1378,
    1259,   542,  1367,   534,   395,  -136,   102,   103,   104,  1397,
     618,  1496,    85,   620,   621,  1449,  1145,   534,   535,   335,
     666,    41,  1146,  1450,   540,   965,   527,   928,   839,   528,
     620,   621,   535,    13,    13,    70,    70,    70,    70,  1451,
     541,   395,   543,  1514,   416,   417,   542,   855,   756,   164,
     165,    74,  1147,   967,   396,  1204,   217,   963,  1452,  1379,
    1205,  1491,   761,  1148,   423,  1443,    52,   142,  1149,  1450,
     425,   855,    16,    16,   363,  1373,   716,  1269,   926,   524,
     929,   641,   642,   558,   559,  1451,   726,   543,   937,   729,
     718,   940,   148,   217,   857,   964,    61,   905,   641,   642,
     342,   364,   365,    96,  1452,    53,   266,   432,   433,   761,
     148,   855,   969,   534,   112,   113,   760,   768,   857,   120,
      80,  1023,   525,   406,   121,    62,   122,   569,   535,   123,
     841,  1388,   507,   148,   509,   856,    37,  1088,  1195,    86,
     830,   435,   436,   405,   856,  1254,  1021,   407,   180,   608,
     609,    87,   856,   510,   856,   340,  1127,   855,   857,   819,
     560,   374,   511,   124,   366,  1333,  1131,   223,   367,   855,
    1089,   267,   504,   529,   756,   967,   717,    58,  1090,  1453,
     561,    96,  1431,   963,    75,  1102,   545,   546,   548,   166,
     731,  1439,   505,    63,   167,   224,   168,   267,   362,   123,
      58,    59,   855,   856,   857,  1482,   858,   267,  1345,   859,
     267,   267,   267,   538,  1375,   368,   857,    58,   551,   369,
    1426,    13,   370,   916,    59,  1453,   408,    13,    77,  1107,
      58,  1381,  1382,  1474,  1475,   660,   460,    13,   371,   746,
     976,    59,  1504,   464,   747,   612,   613,   372,  1391,   857,
      79,    64,    80,   618,    59,   619,   620,   621,   622,   623,
      16,    65,   339,    81,   645,   646,    16,   855,   650,   761,
     652,   748,   654,  1445,  1019,   970,    16,  1002,  1016,    48,
    1298,   608,   609,   105,   785,    49,  1028,    82,   267,   267,
    1152,  1026,   267,   786,   267,  1002,   267,    89,   267,  1433,
    1434,   105,   972,   971,  1188,  1003,   904,   266,  1096,   106,
    1023,   638,   639,   640,   857,   267,    98,    99,   100,   912,
     913,   700,    13,  1103,   641,   642,  -726,   974,  1141,   925,
     701,  -726,  1276,   266,   931,   932,  1002,   934,   831,   936,
     105,   938,   939,   266,   941,  1338,   266,   266,   266,   102,
    -726,   104,  1002,   266,   555,   144,   145,   146,   147,   267,
    1277,    16,   562,    44,  1284,    95,  1309,     2,  1354,   972,
     700,    13,  1002,   267,     3,   610,   611,   612,   613,   701,
    1291,  1002,   563,   374,   570,   618,    45,   619,   620,   621,
     622,   623,   822,   624,   625,  1347,   573,     4,  1002,     5,
    1293,     6,    96,  -135,   571,   772,   776,     7,  1002,  1339,
      16,  1371,  1270,   101,  1302,   823,   574,     8,   700,    13,
     790,   127,  1002,     9,   266,   266,  1340,   701,   266,    87,
     266,   129,   266,  1135,   266,   834,  1344,   837,   816,  1372,
    1226,   636,   637,   638,   639,   640,  1407,    10,   108,  1263,
    1396,   266,   972,  1153,   109,  1002,   641,   642,    16,   700,
      13,   267,   972,   972,   685,   700,    13,  1279,   701,    11,
      12,   608,   609,   709,   701,   115,   927,   911,  1362,   694,
     914,   116,  1020,  1459,   921,   679,   130,  -733,   700,   267,
    1498,  1523,  -733,   710,  1376,   266,  1267,   701,  1159,    16,
     680,   267,   267,   267,   267,    16,  1460,  1461,   267,   266,
    1169,  -733,   267,   409,  1313,  1174,   410,   267,   267,   411,
     267,  1290,   267,  -740,   267,   267,   374,   267,  -740,   749,
     697,    13,    58,  1346,  -380,    70,  1110,  1111,  1112,  -380,
     133,   746,  1116,  1485,   700,    13,   134,  -740,   991,  1022,
     700,   996,    14,   701,  1486,  1487,    59,   992,  -380,   701,
     997,  1106,   849,   700,    15,   374,   131,   612,   613,   698,
      16,  1134,   701,   608,   609,   618,   756,   619,   620,   621,
     622,   623,  1510,  1143,    16,   963,   136,   691,  1420,   151,
     692,   411,  1144,   411,   989,   152,  1518,   266,  1444,   155,
     608,   609,  1310,  1014,   140,   156,  1017,  1326,  1311,   158,
     374,   137,  1526,   844,   820,   844,   374,  1529,   374,   138,
     821,   139,   824,   873,   374,   266,    85,   267,   826,   267,
     267,  1036,   374,   220,   221,   907,   828,   266,   266,   266,
     266,   173,   267,  1257,   266,  1258,   641,   642,   266,   686,
     687,  1264,   174,   266,   266,   267,   266,   148,   266,   102,
     266,   266,   374,   266,   374,   179,  1101,   181,  1109,   612,
     613,  1446,   462,   604,   374,   605,  1048,   618,  1358,   619,
     620,   621,   622,   623,   374,   102,   103,   104,  1410,  1392,
     160,   161,   162,   666,  1054,  1055,   612,   613,   182,   160,
     161,   738,   739,   217,   618,   278,   873,   620,   621,   622,
     623,    92,    93,    94,  1418,   220,   221,   222,   274,   267,
     267,   267,   331,   332,   333,   267,   336,   344,   343,   348,
     349,   392,   350,   636,   637,   638,   639,   640,   351,   352,
     353,  1392,   354,   355,   375,   356,   357,   358,   641,   642,
     359,   360,  1438,   361,   267,   388,   374,   389,   390,   393,
     376,   377,   400,   266,   404,   266,   266,   493,  1280,   521,
     522,   391,   394,   514,  1341,   641,   642,   539,   266,   378,
    1468,  1421,   520,   379,  1025,   523,  1025,   530,   531,   549,
    1470,   266,   596,   550,  1179,   556,   565,   380,   381,   564,
     566,   568,   382,   383,   384,   385,   572,  1044,   567,   575,
     569,   576,   647,  1051,  1052,   648,   602,   579,   267,   660,
     671,   690,   580,   581,   582,  1464,  1060,   583,   584,  1062,
    1063,  1064,  1065,  1066,  1508,   601,   644,  1069,   667,   666,
     689,   699,   708,  1095,   712,  1098,  1390,   713,   714,   741,
     742,   743,   753,  1515,   751,   266,   266,   266,   764,   783,
     833,   266,   766,   838,   849,   853,   854,  1494,   902,   903,
     919,   942,   951,   953,  1228,   962,   954,   955,   481,   979,
     956,   966,  1415,   972,   981,  1007,   973,   999,   503,   975,
     266,   980,   982,   983,   984,   994,  1009,   995,   513,  1011,
    1000,  1013,  1030,  1034,  1035,   608,   609,  1043,  1045,  1053,
    1057,  1058,  1059,  1067,  1094,  1061,  1099,  1100,  1104,  1113,
    1105,  1187,  1114,  1115,  1118,  1117,  1119,  1120,  1121,  1282,
    1122,  1123,  1255,  1125,  1124,  1126,  1161,  1128,  1190,  1180,
    1137,  1192,  1162,  1194,  1193,  1202,  1227,  1230,  1164,  1170,
    1313,   267,  1236,   267,   266,  1178,  1238,  1278,  1271,  1197,
    1198,  1199,  1200,  1181,  1182,   577,   578,  1240,  1272,  1201,
    1237,  1241,  1242,  1246,  1266,  1262,  1268,  1274,  1273,  1275,
    1295,  1296,   586,   587,   588,   589,   590,  1306,  1317,  1318,
    1320,   598,  1332,  1343,  1351,  1353,  1324,  1412,  1325,   610,
     611,   612,   613,   614,  1327,  1370,   615,   616,   617,   618,
    1331,   619,   620,   621,   622,   623,  1357,   624,   625,  1334,
     608,   609,    70,   626,   627,   628,  1335,  1395,  1364,   629,
    1384,  1419,  1385,  1386,  1463,  1422,  1232,  1387,  1399,  1402,
    1404,   670,  1411,  1436,  1437,  1440,  1469,  1502,  1499,  1442,
     678,  1471,   -82,  1472,  1490,  1476,  1477,  1478,   630,  1359,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
    1479,   696,  1500,  1493,  1503,  1501,  1507,  1517,  1505,   740,
     641,   642,   668,   267,  1356,  1525,   125,   266,    20,   266,
     598,   719,    83,   172,   722,   128,   724,  1530,   847,   143,
    1528,   848,  1091,  1389,   347,    27,   732,   733,   734,   735,
     736,   737,  1352,   606,   610,   611,   612,   613,   614,  1424,
    1466,   615,   616,   617,   618,  1425,   619,   620,   621,   622,
     623,  1377,   624,   625,   683,  1467,    91,   777,   778,  1184,
    1031,   779,   780,   781,   782,   387,   784,   553,   787,   788,
     789,   791,   792,   793,   794,   795,   796,   798,   799,   800,
     801,   802,   803,   804,   805,   806,   807,   808,  1315,   817,
    1316,  1414,  1032,  1416,  1417,   631,   632,   633,   634,   635,
     636,   637,   638,   639,   640,   554,  1435,     0,     0,     0,
       0,     0,     0,     0,     0,   641,   642,     0,     0,     0,
       0,     0,     0,   608,   609,     0,     0,     0,     0,     0,
       0,   899,     0,     0,     0,     0,   901,   267,     0,   266,
       0,     0,   906,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   920,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1374,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1492,     0,   949,     0,     0,     0,
     952,     0,     0,     0,     0,   749,     0,     0,     0,   960,
     598,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1506,     0,     0,     0,   899,     0,     0,   610,   611,   612,
     613,   614,     0,     0,   615,   616,   617,   618,     0,   619,
     620,   621,   622,   623,     0,   624,   625,     0,     0,  1522,
       0,   626,   627,   628,     0,     0,     0,   629,     0,  1527,
       0,   749,     0,     0,     0,     0,     0,   993,     0,     0,
       0,   998,     0,     0,     0,     0,    13,     0,     0,     0,
       0,     0,  1455,   266,     0,     0,   630,   873,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   641,   642,
    1033,     0,     0,     0,     0,    16,     0,  1037,     0,  1039,
       0,     0,  1042,     0,     0,     0,     0,  1047,     0,  1050,
       0,     0,     0,     0,     0,   769,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1509,     0,  1097,     0,  1512,  1513,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   873,     0,     0,
    1521,     0,     0,     0,     0,     0,     0,   231,   232,   233,
       0,   235,   236,   237,   238,   239,   434,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,     0,   253,
     254,   255,  1132,  1133,   258,   259,   260,   261,     0,     0,
       0,     0,     0,  1138,  1139,  1140,     0,   952,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1155,     0,  1157,
       0,  1160,     0,     0,     0,     0,     0,  1163,     0,     0,
       0,  1166,     0,     0,     0,     0,     0,     0,     0,   952,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   770,
       0,     0,     0,     0,     0,     0,     0,     0,   771,     0,
       0,     0,     0,     0,     0,     0,     0,  1183,     0,     0,
       0,     0,     0,     0,     0,  1189,     0,     0,  1191,     0,
       0,     0,     0,     0,     0,     0,  1196,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1208,  1209,  1210,
    1211,  1212,  1213,  1214,  1215,  1216,  1217,  1218,  1219,  1220,
    1221,  1222,  1223,  1224,  1225,     0,     0,     0,     0,     0,
       0,     0,     0,  1231,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   -73,     0,     0,     0,
       0,     0,   598,     0,     0,     0,     0,   608,   609,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1287,
       0,  1288,     0,     0,     0,     0,     0,  1292,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1297,     0,     0,     0,  1299,
       0,     0,     0,     0,  1300,  1301,     0,  1303,  1304,     0,
       0,   608,   609,  1070,  1071,  1072,  1073,  1074,  1075,  1076,
    1077,   610,   611,   612,   613,   614,  1078,  1079,   615,   616,
     617,   618,  1080,   619,   620,   621,   622,   623,  1081,   624,
     625,  1082,  1083,     0,  1319,   626,   627,   628,  1084,  1085,
    1086,   629,     0,     0,  1328,  1329,  1330,     0,     0,     0,
       0,  1337,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1348,  1349,  1350,     0,  1087,
     630,     0,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   641,   642,     0,   610,   611,   612,   613,   614,
       0,     0,   615,   616,   617,   618,     0,   619,   620,   621,
     622,   623,     0,   624,   625,     0,     0,     0,     0,   626,
     627,   628,     0,     0,   598,   629,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1400,  1401,     0,     0,     0,     0,     0,     0,
    1406,     0,     0,     0,   630,     0,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   641,   642,     0,     0,
       0,  1432,     0,  1479,     0,     0,     0,     0,     0,     0,
       0,     0,  1441,     0,     0,     0,   598,     0,     0,     0,
       0,     0,  1447,     0,     0,     0,     0,     0,     0,  1454,
       0,     0,     0,     0,     0,   899,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1481,
       0,     0,  1483,  1484,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1454,     0,     0,     0,   861,
       0,  1495,     0,   416,   417,     3,     0,   -93,   -87,   -87,
       0,   -90,     0,   418,   419,   420,   421,   422,     0,     0,
       0,     0,     0,   423,   862,   424,   863,   864,     0,   425,
       0,  1511,     0,     0,     0,     0,   865,   426,     0,  1516,
     -95,     0,   866,   427,  1519,   899,   428,     0,     8,   429,
     867,     0,   868,   430,     0,     0,   869,   870,     0,     0,
       0,     0,     0,   871,     0,     0,   432,   433,     0,   231,
     232,   233,     0,   235,   236,   237,   238,   239,   434,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
       0,   253,   254,   255,     0,     0,   258,   259,   260,   261,
     435,   436,   437,   872,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   438,   439,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   440,   441,   442,   443,   444,     0,
     445,     0,   446,   447,   448,   449,   450,   451,   452,   453,
      59,     0,    13,   454,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   455,
     456,   457,     0,    14,     0,     0,   458,   459,     0,     0,
       0,     0,     0,     0,     0,   460,     0,   461,     0,   462,
     463,    16,   464,  -241,   861,     0,     0,     0,   416,   417,
       3,     0,   -93,   -87,   -87,     0,   -90,     0,   418,   419,
     420,   421,   422,     0,     0,     0,     0,     0,   423,   862,
     424,   863,   864,     0,   425,     0,     0,     0,     0,     0,
       0,   865,   426,     0,     0,   -95,     0,   866,   427,     0,
       0,   428,     0,     8,   429,   867,     0,   868,   430,     0,
       0,   869,   870,     0,     0,     0,     0,     0,   871,     0,
       0,   432,   433,     0,   231,   232,   233,     0,   235,   236,
     237,   238,   239,   434,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,     0,   253,   254,   255,     0,
       0,   258,   259,   260,   261,   435,   436,   437,   872,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     438,   439,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   440,
     441,   442,   443,   444,     0,   445,     0,   446,   447,   448,
     449,   450,   451,   452,   453,    59,     0,    13,   454,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   455,   456,   457,     0,    14,     0,
       0,   458,   459,     0,     0,     0,     0,     0,     0,     0,
     460,     0,   461,     0,   462,   463,    16,   464,   977,   861,
       0,     0,     0,   416,   417,     3,     0,   -93,   -87,   -87,
       0,   -90,     0,   418,   419,   420,   421,   422,     0,     0,
       0,     0,     0,   423,   862,   424,   863,   864,     0,   425,
       0,     0,     0,     0,     0,     0,   865,   426,     0,     0,
     -95,     0,   866,   427,     0,     0,   428,     0,     8,   429,
     867,     0,   868,   430,     0,     0,   869,   870,     0,     0,
       0,     0,     0,   871,     0,     0,   432,   433,     0,   231,
     232,   233,     0,   235,   236,   237,   238,   239,   434,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
       0,   253,   254,   255,     0,     0,   258,   259,   260,   261,
     435,   436,   437,   872,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   438,   439,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   440,   441,   442,   443,   444,     0,
     445,     0,   446,   447,   448,   449,   450,   451,   452,   453,
      59,     0,    13,   454,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   455,
     456,   457,     0,    14,     0,     0,   458,   459,     0,     0,
       0,     0,     0,     0,     0,   460,     0,   461,     0,   462,
     463,    16,   464,  -238,   861,     0,     0,     0,   416,   417,
       3,     0,   -93,   -87,   -87,     0,   -90,     0,   418,   419,
     420,   421,   422,     0,     0,     0,     0,     0,   423,   862,
     424,   863,   864,     0,   425,     0,     0,     0,     0,     0,
       0,   865,   426,     0,     0,   -95,     0,   866,   427,     0,
       0,   428,     0,     8,   429,   867,     0,   868,   430,     0,
       0,   869,   870,     0,     0,     0,     0,     0,   871,     0,
       0,   432,   433,     0,   231,   232,   233,     0,   235,   236,
     237,   238,   239,   434,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,     0,   253,   254,   255,     0,
       0,   258,   259,   260,   261,   435,   436,   437,   872,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     438,   439,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   440,
     441,   442,   443,   444,     0,   445,     0,   446,   447,   448,
     449,   450,   451,   452,   453,    59,     0,    13,   454,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   455,   456,   457,     0,    14,     0,
       0,   458,   459,     0,     0,     0,     0,     0,     0,     0,
     460,     0,   461,     0,   462,   463,    16,   464,   -83,   416,
     417,     0,     0,     0,     0,     0,     0,     0,     0,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
     862,   424,   863,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,   867,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,   872,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    59,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   460,     0,   461,     0,   462,   463,     0,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,   755,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,   756,   446,   447,
     448,   449,   450,   451,   452,   453,   757,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   758,     0,   461,   759,   462,   463,   591,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,   592,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,   756,   446,   447,
     448,   449,   450,   451,   452,   453,   757,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   460,     0,   461,     0,   462,   463,   591,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,   592,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    59,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   460,     0,   461,     0,   462,   463,     0,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,   915,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,   756,   446,   447,
     448,   449,   450,   451,   452,   453,   757,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   460,     0,   461,     0,   462,   463,     0,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    59,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   460,   512,   461,     0,   462,   463,   597,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    59,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   460,     0,   461,     0,   462,   463,     0,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    59,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   460,   669,   461,     0,   462,   463,     0,   464,   418,
     419,   420,   421,   422,     0,     0,   797,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    59,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   460,     0,   461,     0,   462,   463,     0,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    59,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   460,     0,   461,   818,   462,   463,     0,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
    1038,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    59,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   460,     0,   461,     0,   462,   463,     0,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,  1041,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    59,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   460,     0,   461,     0,   462,   463,     0,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   430,
       0,     0,  1046,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    59,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   460,     0,   461,     0,   462,   463,     0,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
    1049,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    59,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   460,     0,   461,     0,   462,   463,     0,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    59,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   460,     0,   461,  1158,   462,   463,     0,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    59,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,  1167,     0,   461,  1168,   462,   463,     0,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    59,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   460,     0,   461,  1173,   462,   463,     0,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    59,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   460,     0,   461,  1256,   462,   463,     0,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    59,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   460,     0,   461,  1336,   462,   463,     0,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
    1405,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    59,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   460,     0,   461,     0,   462,   463,     0,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    59,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,     0,    14,
       0,     0,   458,   459,     0,     0,     0,     0,     0,   416,
     417,   460,     0,   461,     0,   462,   463,     0,   464,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,   423,
       0,   424,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,   432,   433,     0,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   434,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    59,     0,   225,   454,
       0,     0,     0,     0,   226,     0,     0,     0,     0,     0,
     227,     0,     0,     0,     0,   455,   456,   457,     0,    14,
     228,     0,   458,   459,     0,     0,     0,     0,   229,     0,
       0,  1142,     0,   461,     0,   462,   463,     0,   464,     0,
       0,     0,     0,   230,     0,     0,     0,     0,     0,     0,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,     0,     0,     0,     0,     0,     0,     0,
     225,     0,     0,     0,     0,     0,   226,     0,     0,     0,
       0,     0,   227,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   228,     0,     0,     0,     0,    58,     0,     0,
     229,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     264,     0,     0,     0,     0,   230,     0,     0,     0,     0,
       0,    59,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,     0,     0,     0,     0,     0,
     265,     0,     0,     0,   225,   515,     0,     0,     0,     0,
     226,     0,     0,     0,     0,     0,   227,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   228,     0,     0,    58,
       0,     0,     0,     0,   229,     0,     0,     0,     0,     0,
       0,     0,   264,     0,     0,     0,     0,     0,     0,   230,
       0,     0,     0,   656,     0,    13,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,     0,
       0,     0,   265,     0,    16,     0,   225,     0,     0,     0,
     608,   609,   226,     0,     0,     0,     0,     0,   227,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   228,     0,
       0,     0,     0,    58,     0,     0,   229,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   264,     0,     0,     0,
       0,   230,     0,     0,     0,     0,     0,    59,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,     0,     0,     0,     0,     0,   265,   608,   609,     0,
       0,     0,     0,     0,   610,   611,   612,   613,   614,     0,
       0,   615,   616,   617,   618,     0,   619,   620,   621,   622,
     623,     0,   624,   625,     0,    58,     0,     0,   626,   627,
     628,     0,     0,     0,   629,     0,     0,     0,   264,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   656,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   630,     0,   631,   632,   633,   634,   635,
     636,   637,   638,   639,   640,     0,     0,     0,     0,     0,
       0,     0,   184,   608,   609,   641,   642,     0,   265,   643,
       0,   610,   611,   612,   613,   614,     0,     0,   615,   616,
     617,   618,     0,   619,   620,   621,   622,   623,     0,   624,
     625,     0,     0,     0,     0,   626,   627,   628,   185,     0,
     186,   629,   187,   188,   189,   190,   191,     0,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,     0,
     203,   204,   205,     0,     0,   206,   207,   208,   209,     0,
     630,     0,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,     0,     0,   210,   211,     0,     0,     0,   608,
     609,     0,   641,   642,     0,     0,   663,   610,   611,   612,
     613,   614,     0,     0,   615,   616,   617,   618,     0,   619,
     620,   621,   622,   623,     0,   624,   625,     0,     0,     0,
       0,   626,   627,   628,     0,     0,     0,   629,     0,   212,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   630,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,     0,     0,
       0,     0,     0,     0,     0,   608,   609,     0,   641,   642,
       0,     0,   852,   610,   611,   612,   613,   614,     0,     0,
     615,   616,   617,   618,     0,   619,   620,   621,   622,   623,
       0,   624,   625,     0,     0,     0,     0,   626,   627,   628,
       0,     0,     0,   629,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   630,     0,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,     0,     0,     0,     0,     0,     0,
       0,   608,   609,     0,   641,   642,     0,     0,   930,   610,
     611,   612,   613,   614,     0,     0,   615,   616,   617,   618,
       0,   619,   620,   621,   622,   623,     0,   624,   625,     0,
       0,     0,     0,   626,   627,   628,     0,     0,     0,   629,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   630,     0,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
       0,     0,     0,     0,     0,     0,     0,   608,   609,     0,
     641,   642,     0,     0,   933,   610,   611,   612,   613,   614,
       0,     0,   615,   616,   617,   618,     0,   619,   620,   621,
     622,   623,     0,   624,   625,     0,     0,     0,     0,   626,
     627,   628,     0,     0,     0,   629,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   630,     0,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,     0,     0,     0,     0,
       0,     0,     0,   608,   609,     0,   641,   642,     0,     0,
     935,   610,   611,   612,   613,   614,     0,     0,   615,   616,
     617,   618,     0,   619,   620,   621,   622,   623,     0,   624,
     625,     0,     0,     0,     0,   626,   627,   628,     0,     0,
       0,   629,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     630,     0,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,     0,     0,     0,     0,     0,     0,     0,   608,
     609,     0,   641,   642,     0,     0,   943,   610,   611,   612,
     613,   614,     0,     0,   615,   616,   617,   618,     0,   619,
     620,   621,   622,   623,     0,   624,   625,     0,     0,     0,
       0,   626,   627,   628,     0,     0,     0,   629,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   630,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,     0,     0,
       0,     0,     0,     0,     0,   608,   609,     0,   641,   642,
       0,     0,   944,   610,   611,   612,   613,   614,     0,     0,
     615,   616,   617,   618,     0,   619,   620,   621,   622,   623,
       0,   624,   625,     0,     0,     0,     0,   626,   627,   628,
       0,     0,     0,   629,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   630,     0,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,     0,     0,     0,     0,     0,     0,
       0,   608,   609,     0,   641,   642,     0,     0,   945,   610,
     611,   612,   613,   614,     0,     0,   615,   616,   617,   618,
       0,   619,   620,   621,   622,   623,     0,   624,   625,     0,
       0,     0,     0,   626,   627,   628,     0,     0,     0,   629,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   630,     0,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
       0,     0,     0,     0,     0,     0,     0,   608,   609,     0,
     641,   642,     0,     0,   946,   610,   611,   612,   613,   614,
       0,     0,   615,   616,   617,   618,     0,   619,   620,   621,
     622,   623,     0,   624,   625,     0,     0,     0,     0,   626,
     627,   628,     0,     0,     0,   629,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   630,     0,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,     0,     0,     0,     0,
       0,     0,     0,   608,   609,     0,   641,   642,     0,     0,
     947,   610,   611,   612,   613,   614,     0,     0,   615,   616,
     617,   618,     0,   619,   620,   621,   622,   623,     0,   624,
     625,     0,     0,     0,     0,   626,   627,   628,     0,     0,
       0,   629,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     630,     0,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,     0,     0,     0,     0,     0,     0,     0,   608,
     609,     0,   641,   642,     0,     0,   948,   610,   611,   612,
     613,   614,     0,     0,   615,   616,   617,   618,     0,   619,
     620,   621,   622,   623,     0,   624,   625,     0,     0,     0,
       0,   626,   627,   628,     0,     0,     0,   629,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   630,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,     0,     0,
       0,     0,     0,     0,     0,   608,   609,     0,   641,   642,
       0,     0,  1093,   610,   611,   612,   613,   614,     0,     0,
     615,   616,   617,   618,     0,   619,   620,   621,   622,   623,
       0,   624,   625,     0,     0,     0,     0,   626,   627,   628,
       0,     0,     0,   629,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   630,     0,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,     0,     0,     0,     0,     0,     0,
       0,   608,   609,     0,   641,   642,     0,     0,  1108,   610,
     611,   612,   613,   614,     0,     0,   615,   616,   617,   618,
       0,   619,   620,   621,   622,   623,     0,   624,   625,     0,
       0,     0,     0,   626,   627,   628,     0,     0,     0,   629,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   630,     0,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
       0,     0,     0,     0,     0,     0,     0,   608,   609,     0,
     641,   642,     0,     0,  1186,   610,   611,   612,   613,   614,
       0,     0,   615,   616,   617,   618,     0,   619,   620,   621,
     622,   623,     0,   624,   625,     0,     0,     0,     0,   626,
     627,   628,     0,     0,     0,   629,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   630,     0,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,     0,     0,     0,     0,
       0,     0,     0,   608,   609,     0,   641,   642,     0,     0,
    1260,   610,   611,   612,   613,   614,     0,     0,   615,   616,
     617,   618,     0,   619,   620,   621,   622,   623,     0,   624,
     625,     0,     0,     0,     0,   626,   627,   628,     0,     0,
       0,   629,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     630,     0,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,     0,     0,     0,     0,     0,     0,     0,   608,
     609,     0,   641,   642,     0,     0,  1261,   610,   611,   612,
     613,   614,     0,     0,   615,   616,   617,   618,     0,   619,
     620,   621,   622,   623,     0,   624,   625,     0,     0,     0,
       0,   626,   627,   628,     0,     0,     0,   629,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   630,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,     0,     0,
       0,     0,     0,     0,     0,   608,   609,     0,   641,   642,
       0,     0,  1265,   610,   611,   612,   613,   614,     0,     0,
     615,   616,   617,   618,     0,   619,   620,   621,   622,   623,
       0,   624,   625,     0,     0,     0,     0,   626,   627,   628,
       0,     0,     0,   629,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   630,     0,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,     0,     0,     0,     0,     0,     0,
       0,   608,   609,     0,   641,   642,     0,     0,  1281,   610,
     611,   612,   613,   614,     0,     0,   615,   616,   617,   618,
       0,   619,   620,   621,   622,   623,     0,   624,   625,     0,
       0,     0,     0,   626,   627,   628,     0,     0,     0,   629,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   630,     0,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
       0,     0,     0,     0,     0,     0,     0,   608,   609,     0,
     641,   642,     0,     0,  1283,   610,   611,   612,   613,   614,
       0,     0,   615,   616,   617,   618,     0,   619,   620,   621,
     622,   623,     0,   624,   625,     0,     0,     0,     0,   626,
     627,   628,     0,     0,     0,   629,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   630,     0,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,     0,     0,     0,     0,
       0,     0,     0,   608,   609,     0,   641,   642,     0,     0,
    1285,   610,   611,   612,   613,   614,     0,     0,   615,   616,
     617,   618,     0,   619,   620,   621,   622,   623,     0,   624,
     625,     0,     0,     0,     0,   626,   627,   628,     0,     0,
       0,   629,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     630,     0,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,     0,     0,     0,     0,     0,     0,     0,   608,
     609,     0,   641,   642,     0,     0,  1289,   610,   611,   612,
     613,   614,     0,     0,   615,   616,   617,   618,     0,   619,
     620,   621,   622,   623,     0,   624,   625,     0,     0,     0,
       0,   626,   627,   628,     0,     0,     0,   629,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   630,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,     0,     0,
       0,     0,     0,     0,     0,   608,   609,     0,   641,   642,
       0,     0,  1360,   610,   611,   612,   613,   614,     0,     0,
     615,   616,   617,   618,     0,   619,   620,   621,   622,   623,
       0,   624,   625,     0,     0,     0,     0,   626,   627,   628,
       0,     0,     0,   629,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   630,     0,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,     0,     0,     0,     0,     0,     0,
       0,   608,   609,     0,   641,   642,     0,     0,  1361,   610,
     611,   612,   613,   614,     0,     0,   615,   616,   617,   618,
       0,   619,   620,   621,   622,   623,     0,   624,   625,     0,
       0,     0,     0,   626,   627,   628,     0,     0,     0,   629,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   630,     0,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
       0,     0,     0,     0,     0,     0,     0,   608,   609,     0,
     641,   642,     0,     0,  1363,   610,   611,   612,   613,   614,
       0,     0,   615,   616,   617,   618,     0,   619,   620,   621,
     622,   623,     0,   624,   625,     0,     0,     0,     0,   626,
     627,   628,     0,     0,     0,   629,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   630,     0,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,     0,     0,     0,     0,
       0,     0,     0,   608,   609,     0,   641,   642,     0,     0,
    1365,   610,   611,   612,   613,   614,     0,     0,   615,   616,
     617,   618,     0,   619,   620,   621,   622,   623,     0,   624,
     625,     0,     0,     0,     0,   626,   627,   628,     0,     0,
       0,   629,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     630,     0,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,     0,     0,     0,     0,     0,     0,     0,   608,
     609,     0,   641,   642,     0,     0,  1366,   610,   611,   612,
     613,   614,     0,     0,   615,   616,   617,   618,     0,   619,
     620,   621,   622,   623,     0,   624,   625,     0,     0,     0,
       0,   626,   627,   628,     0,     0,     0,   629,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   630,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,     0,     0,
       0,     0,     0,     0,     0,   608,   609,     0,   641,   642,
       0,     0,  1368,   610,   611,   612,   613,   614,     0,     0,
     615,   616,   617,   618,     0,   619,   620,   621,   622,   623,
       0,   624,   625,     0,     0,     0,     0,   626,   627,   628,
       0,     0,     0,   629,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   630,     0,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,     0,     0,     0,     0,     0,     0,
       0,   608,   609,     0,   641,   642,     0,     0,  1369,   610,
     611,   612,   613,   614,     0,     0,   615,   616,   617,   618,
       0,   619,   620,   621,   622,   623,     0,   624,   625,     0,
       0,     0,     0,   626,   627,   628,     0,     0,     0,   629,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   630,     0,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
       0,     0,     0,     0,     0,     0,     0,   608,   609,     0,
     641,   642,     0,     0,  1383,   610,   611,   612,   613,   614,
       0,     0,   615,   616,   617,   618,     0,   619,   620,   621,
     622,   623,     0,   624,   625,     0,     0,     0,     0,   626,
     627,   628,     0,     0,     0,   629,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   630,     0,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,     0,     0,     0,     0,
       0,     0,     0,   608,   609,     0,   641,   642,     0,     0,
    1394,   610,   611,   612,   613,   614,     0,     0,   615,   616,
     617,   618,     0,   619,   620,   621,   622,   623,     0,   624,
     625,     0,     0,     0,     0,   626,   627,   628,     0,     0,
       0,   629,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     630,     0,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,     0,     0,     0,     0,     0,     0,     0,   608,
     609,     0,   641,   642,     0,     0,  1403,   610,   611,   612,
     613,   614,     0,     0,   615,   616,   617,   618,     0,   619,
     620,   621,   622,   623,     0,   624,   625,     0,     0,     0,
       0,   626,   627,   628,     0,     0,     0,   629,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   630,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,     0,     0,
       0,     0,     0,     0,     0,   608,   609,     0,   641,   642,
       0,     0,  1473,   610,   611,   612,   613,   614,     0,     0,
     615,   616,   617,   618,     0,   619,   620,   621,   622,   623,
       0,   624,   625,     0,     0,     0,     0,   626,   627,   628,
       0,     0,     0,   629,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   630,     0,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,     0,     0,     0,     0,     0,   608,
     609,     0,     0,     0,   641,   642,     0,     0,  1524,   610,
     611,   612,   613,   614,     0,     0,   615,   616,   617,   618,
       0,   619,   620,   621,   622,   623,     0,   624,   625,     0,
       0,     0,     0,   626,   627,   628,     0,     0,     0,   629,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   630,     0,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
       0,     0,     0,   608,   609,     0,     0,     0,     0,     0,
     641,   642,   840,   610,   611,   612,   613,   614,     0,     0,
     615,   616,   617,   618,     0,   619,   620,   621,   622,   623,
       0,   624,   625,     0,     0,     0,     0,   626,   627,   628,
       0,     0,     0,   629,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   630,     0,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,     0,     0,     0,   608,   609,     0,
       0,     0,     0,     0,   641,   642,   985,   610,   611,   612,
     613,   614,     0,     0,   615,   616,   617,   618,     0,   619,
     620,   621,   622,   623,     0,   624,   625,     0,     0,     0,
       0,   626,   627,   628,     0,     0,     0,   629,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   630,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,     0,     0,
       0,   608,   609,     0,     0,     0,     0,     0,   641,   642,
    1001,   610,   611,   612,   613,   614,     0,     0,   615,   616,
     617,   618,     0,   619,   620,   621,   622,   623,     0,   624,
     625,     0,     0,     0,     0,   626,   627,   628,     0,     0,
       0,   629,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     630,     0,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   641,   642,  1165,   610,   611,   612,   613,   614,
     280,   281,   615,   616,   617,   618,     0,   619,   620,   621,
     622,   623,     0,   624,   625,     0,     0,   282,     0,   626,
     627,   628,     0,     0,     0,   629,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   630,     0,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   641,   642,  1171,     0,
       0,     0,     0,   608,   609,     0,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,     0,     0,   301,   302,   303,     0,
       0,   304,   305,   306,   307,   308,     0,     0,   309,   310,
     311,   312,   313,   314,   315,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   773,   316,     0,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,     0,     0,   327,   328,   608,
     609,     0,     0,     0,     0,   329,   330,   610,   611,   612,
     613,   614,     0,     0,   615,   616,   617,   618,     0,   619,
     620,   621,   622,   623,     0,   624,   625,     0,     0,   752,
       0,   626,   627,   628,   231,   232,   233,   629,   235,   236,
     237,   238,   239,   434,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,     0,   253,   254,   255,     0,
       0,   258,   259,   260,   261,     0,   630,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,     0,     0,
       0,     0,   608,   609,     0,     0,     0,     0,   641,   642,
       0,     0,     0,   610,   611,   612,   613,   614,     0,     0,
     615,   616,   617,   618,     0,   619,   620,   621,   622,   623,
       0,   624,   625,     0,     0,     0,   774,   626,   627,   628,
       0,     0,     0,   629,     0,   775,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   630,   990,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,     0,   608,   609,     0,     0,     0,
       0,     0,     0,     0,   641,   642,   610,   611,   612,   613,
     614,     0,     0,   615,   616,   617,   618,     0,   619,   620,
     621,   622,   623,     0,   624,   625,     0,     0,     0,     0,
     626,   627,   628,     0,     0,     0,   629,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1129,     0,     0,     0,   630,     0,   631,   632,   633,
     634,   635,   636,   637,   638,   639,   640,     0,   608,   609,
       0,     0,     0,     0,     0,     0,     0,   641,   642,   610,
     611,   612,   613,   614,     0,     0,   615,   616,   617,   618,
       0,   619,   620,   621,   622,   623,     0,   624,   625,   608,
     609,     0,     0,   626,   627,   628,     0,     0,     0,   629,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   630,     0,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     641,   642,   610,   611,   612,   613,   614,     0,     0,   615,
     616,   617,   618,     0,   619,   620,   621,   622,   623,     0,
     624,   625,   608,   609,     0,     0,   626,   627,   628,     0,
       0,     0,  -741,   610,   611,   612,   613,   614,     0,     0,
     615,   616,   617,   618,     0,   619,   620,   621,   622,   623,
       0,   624,   625,   608,   609,     0,     0,   626,   627,   628,
       0,   630,     0,   631,   632,   633,   634,   635,   636,   637,
     638,   639,   640,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   641,   642,     0,     0,     0,     0,     0,
       0,     0,   630,     0,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   641,   642,   610,   611,   612,   613,
     614,     0,     0,   615,   616,   617,   618,     0,   619,   620,
     621,   622,   623,     0,   624,   625,   608,   609,     0,     0,
     626,     0,   628,     0,     0,     0,     0,   610,   611,   612,
     613,   614,     0,     0,   615,   616,   617,   618,     0,   619,
     620,   621,   622,   623,     0,   624,   625,   608,   609,     0,
       0,   626,     0,     0,     0,     0,     0,   631,   632,   633,
     634,   635,   636,   637,   638,   639,   640,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   641,   642,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   641,   642,
     610,   611,   612,   613,   614,     0,     0,   615,   616,   617,
     618,     0,   619,   620,   621,   622,   623,     0,   624,   625,
     608,   609,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   610,   611,   612,   613,   614,     0,     0,   615,   616,
     617,   618,     0,   619,   620,   621,   622,   623,     0,   624,
     625,   608,   609,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   632,   633,   634,   635,   636,   637,   638,   639,
     640,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   641,   642,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   633,   634,   635,   636,   637,   638,
     639,   640,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   641,   642,   610,   611,   612,   613,   614,     0,
       0,   615,   616,   617,   618,     0,   619,   620,   621,   622,
     623,     0,   624,   625,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   610,   611,   612,   613,   614,
       0,     0,   615,     0,     0,   618,     0,   619,   620,   621,
     622,   623,     0,   624,   625,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   986,     0,   634,   635,
     636,   637,   638,   639,   640,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   641,   642,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   634,
     635,   636,   637,   638,   639,   640,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   641,   642,   231,   232,
     233,     0,   235,   236,   237,   238,   239,   434,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,     0,
     253,   254,   255,     0,     0,   258,   259,   260,   261,   231,
     232,   233,     0,   235,   236,   237,   238,   239,   434,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
       0,   253,   254,   255,     0,     0,   258,   259,   260,   261,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     987,     0,     0,     0,     0,     0,   809,   810,     0,   988,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   811,     0,     0,     0,     0,     0,     0,     0,     0,
     812,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   813,   814
};

static const yytype_int16 yycheck[] =
{
       1,   159,   137,   138,   540,   536,   173,    14,    15,   605,
     682,   672,   753,   602,   864,   601,   741,   595,   519,    20,
      21,    22,   600,    80,   518,   651,   751,   653,   658,   655,
     173,   496,   662,   498,  1068,   500,   461,    22,   708,    15,
      16,     8,   768,    20,   473,  1178,     5,    33,   374,    33,
      51,   464,    20,    19,    20,   126,    63,    64,    65,    46,
      20,   132,    20,   147,   150,    36,   139,   140,   141,  1342,
     125,  1465,    57,   128,   129,  1412,   126,   147,   162,   214,
     505,   155,   132,  1412,   410,   755,   200,   717,   666,   203,
     128,   129,   162,   164,   164,   102,   103,   104,   105,  1412,
     126,   150,   173,  1497,     5,     6,   132,   126,   153,    15,
      16,    36,   162,   132,   200,   199,   202,   162,  1412,   106,
     204,  1458,   708,   173,    25,  1398,   162,   200,   178,  1458,
      31,   126,   203,   203,    33,   205,   561,   132,   716,     7,
     718,   196,   197,   102,   103,  1458,   571,   173,   726,   574,
     563,   729,   164,   202,   173,   200,    34,   693,   196,   197,
     217,    60,    61,   138,  1458,   201,   173,    68,    69,   755,
     164,   126,   758,   147,     5,     6,   601,   606,   173,   155,
     172,   842,    50,   341,   160,    63,   162,   199,   162,   165,
     851,  1324,   178,   164,   178,   172,   162,   164,  1048,   184,
     175,   102,   103,   338,   172,   199,   836,   342,   200,    21,
      22,   196,   172,   197,   172,   216,   942,   126,   173,   644,
     179,   176,   206,   199,   123,  1251,   951,   172,   127,   126,
     197,   374,   179,   400,   153,   132,   562,   138,   205,  1412,
     199,   138,  1382,   162,     8,   915,   413,   414,   415,   155,
     576,  1391,   199,   131,   160,   200,   162,   400,   265,   165,
     138,   162,   126,   172,   173,  1444,   175,   410,   132,   178,
     413,   414,   415,   408,  1308,   174,   173,   138,   179,   178,
     199,   164,   181,   708,   162,  1458,   343,   164,    62,   919,
     138,  1317,  1318,  1433,  1434,   162,   197,   164,   197,   147,
     765,   162,  1481,   204,   152,   117,   118,   206,  1334,   173,
     173,   189,   172,   125,   162,   127,   128,   129,   130,   131,
     203,   199,   205,   175,   491,   492,   203,   126,   495,   915,
     497,   179,   499,   132,   835,   172,   203,   172,   832,    57,
    1190,    21,    22,   172,   153,    63,   847,   172,   491,   492,
     976,   845,   495,   162,   497,   172,   499,   162,   501,  1385,
    1386,   172,   172,   200,  1036,   200,   692,   374,   904,   198,
    1031,   183,   184,   185,   173,   518,    63,    64,    65,   705,
     706,   163,   164,   200,   196,   197,   175,   198,   198,   715,
     172,   180,   172,   400,   720,   721,   172,   723,   180,   725,
     172,   727,   728,   410,   730,  1255,   413,   414,   415,   139,
     199,   141,   172,   420,   421,   102,   103,   104,   105,   562,
     200,   203,   179,   162,   200,   162,   198,     0,  1278,   172,
     163,   164,   172,   576,     7,   115,   116,   117,   118,   172,
     200,   172,   199,   176,   179,   125,   185,   127,   128,   129,
     130,   131,   180,   133,   134,   198,   179,    30,   172,    32,
     200,    34,   138,    36,   199,   608,   609,    40,   172,   200,
     203,   172,  1144,   199,  1199,   203,   199,    50,   163,   164,
     623,   155,   172,    56,   491,   492,   200,   172,   495,   196,
     497,   162,   499,   958,   501,   180,   200,   664,   641,   200,
    1089,   181,   182,   183,   184,   185,  1356,    80,    57,  1135,
     200,   518,   172,   978,    63,   172,   196,   197,   203,   163,
     164,   664,   172,   172,   531,   163,   164,  1153,   172,   102,
     103,    21,    22,   179,   172,    57,   180,   704,   198,   540,
     707,    63,   180,   200,   711,   147,   162,   175,   163,   692,
     200,   200,   180,   199,    47,   562,  1142,   172,   983,   203,
     162,   704,   705,   706,   707,   203,  1416,  1417,   711,   576,
     995,   199,   715,   172,    67,  1000,   175,   720,   721,   178,
     723,  1167,   725,   175,   727,   728,   176,   730,   180,   596,
     180,   164,   138,  1265,   175,   602,   922,   923,   924,   180,
      57,   147,   928,    12,   163,   164,    63,   199,   153,   162,
     163,   153,   185,   172,    23,    24,   162,   162,   199,   172,
     162,   180,   162,   163,   197,   176,   162,   117,   118,   180,
     203,   957,   172,    21,    22,   125,   153,   127,   128,   129,
     130,   131,  1492,   153,   203,   162,   162,   172,   175,    57,
     175,   178,   162,   178,   797,    63,  1506,   664,  1399,    57,
      21,    22,    57,   830,   166,    63,   833,  1245,    63,   204,
     176,   162,  1522,   674,   180,   676,   176,  1527,   176,   162,
     180,   162,   180,   684,   176,   692,    57,   830,   180,   832,
     833,   858,   176,   166,   167,   696,   180,   704,   705,   706,
     707,   173,   845,  1128,   711,  1130,   196,   197,   715,   203,
     204,  1136,   163,   720,   721,   858,   723,   164,   725,   139,
     727,   728,   176,   730,   176,   166,   180,   164,   180,   117,
     118,  1403,   201,   202,   176,   204,   871,   125,   180,   127,
     128,   129,   130,   131,   176,   139,   140,   141,   180,  1335,
     166,   167,   168,  1178,    10,    11,   117,   118,   162,   166,
     167,   168,   169,   202,   125,   175,   767,   128,   129,   130,
     131,    55,    56,    57,  1370,   166,   167,   168,   163,   922,
     923,   924,    35,    35,   199,   928,   106,   162,   202,   179,
     179,    22,   179,   181,   182,   183,   184,   185,   199,   179,
     179,  1387,   199,   179,   204,   179,   179,   179,   196,   197,
     179,   179,  1390,   199,   957,   162,   176,   162,   162,   162,
     204,   204,   175,   830,   162,   832,   833,   162,  1154,   162,
     132,   198,   198,   197,  1259,   196,   197,   162,   845,    75,
    1426,  1372,   204,    79,   845,   199,   847,   204,   175,   179,
    1428,   858,   202,   179,  1021,   179,   179,    93,    94,   199,
     179,   179,    98,    99,   100,   101,   179,   868,   199,   179,
     199,   179,   200,   874,   875,   162,   197,   199,  1021,   162,
     162,    43,   199,   199,   199,  1421,   887,   199,   199,   890,
     891,   892,   893,   894,  1490,   199,   199,   898,   198,  1324,
     199,   180,   199,   904,   179,   906,  1331,   179,   179,   199,
      13,   172,   172,  1499,   199,   922,   923,   924,     4,   162,
     175,   928,   202,   200,   162,   199,    43,  1463,   162,   162,
     162,   180,   199,   198,  1092,   205,   200,   199,   351,   179,
     199,   199,  1367,   172,   179,   162,   200,     1,   361,   199,
     957,   199,   199,   199,   199,   199,   180,   199,   371,   180,
     199,   180,   173,   162,   162,    21,    22,   162,   155,    10,
      13,     9,    42,    66,    43,   205,   180,   180,   200,   180,
     200,    43,   200,   199,   180,   200,   180,   180,   180,  1156,
     200,   180,  1127,   200,   180,   180,   179,   199,    14,   205,
     200,   173,   199,   155,   175,    37,     8,   162,   199,   199,
      67,  1154,   200,  1156,  1021,   199,   162,  1152,   162,   199,
     199,   199,   199,   205,   205,   438,   439,   180,   162,   199,
     199,   180,   180,   180,   200,   180,   199,   162,   199,   162,
     205,   162,   455,   456,   457,   458,   459,   162,   199,   199,
     180,   464,    70,   180,   162,   137,   199,    12,   200,   115,
     116,   117,   118,   119,   199,   204,   122,   123,   124,   125,
     199,   127,   128,   129,   130,   131,   180,   133,   134,   199,
      21,    22,  1089,   139,   140,   141,   199,   203,   200,   145,
     199,   162,   199,   199,  1420,    33,  1097,   199,   203,   200,
     200,   514,   200,   200,   200,   200,   172,    53,   199,   203,
     523,   200,   204,   200,   204,   200,   200,   200,   174,  1286,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     203,   544,   200,   205,   198,   200,   199,   198,   205,   585,
     196,   197,   198,  1286,  1279,   205,    79,  1154,     1,  1156,
     563,   564,    45,   129,   567,    82,   569,  1529,   676,   101,
    1525,   676,   900,  1327,   223,     1,   579,   580,   581,   582,
     583,   584,  1276,   480,   115,   116,   117,   118,   119,  1377,
    1424,   122,   123,   124,   125,  1380,   127,   128,   129,   130,
     131,  1312,   133,   134,   528,  1425,    54,   610,   611,  1031,
     851,   614,   615,   616,   617,   279,   619,   420,   621,   622,
     623,   624,   625,   626,   627,   628,   629,   630,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,  1229,   642,
    1231,  1366,   851,  1368,  1369,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   420,  1387,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   196,   197,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,   684,    -1,    -1,    -1,    -1,   689,  1420,    -1,  1286,
      -1,    -1,   695,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   710,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1308,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1459,    -1,   739,    -1,    -1,    -1,
     743,    -1,    -1,    -1,    -1,  1342,    -1,    -1,    -1,   752,
     753,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1485,    -1,    -1,    -1,   767,    -1,    -1,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,  1514,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,  1524,
      -1,  1398,    -1,    -1,    -1,    -1,    -1,   810,    -1,    -1,
      -1,   814,    -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,
      -1,    -1,  1413,  1420,    -1,    -1,   174,  1418,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,
     853,    -1,    -1,    -1,    -1,   203,    -1,   860,    -1,   862,
      -1,    -1,   865,    -1,    -1,    -1,    -1,   870,    -1,   872,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1491,    -1,   905,    -1,  1495,  1496,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1508,    -1,    -1,
    1511,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,   955,   956,    98,    99,   100,   101,    -1,    -1,
      -1,    -1,    -1,   966,   967,   968,    -1,   970,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   980,    -1,   982,
      -1,   984,    -1,    -1,    -1,    -1,    -1,   990,    -1,    -1,
      -1,   994,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1002,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1030,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1038,    -1,    -1,  1041,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1049,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1070,  1071,  1072,
    1073,  1074,  1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,
    1083,  1084,  1085,  1086,  1087,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1096,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,  1115,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1162,
      -1,  1164,    -1,    -1,    -1,    -1,    -1,  1170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1188,    -1,    -1,    -1,  1192,
      -1,    -1,    -1,    -1,  1197,  1198,    -1,  1200,  1201,    -1,
      -1,    21,    22,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,    -1,  1237,   139,   140,   141,   142,   143,
     144,   145,    -1,    -1,  1247,  1248,  1249,    -1,    -1,    -1,
      -1,  1254,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1268,  1269,  1270,    -1,   173,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   196,   197,    -1,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,  1327,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1345,  1346,    -1,    -1,    -1,    -1,    -1,    -1,
    1353,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   196,   197,    -1,    -1,
      -1,  1384,    -1,   203,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1395,    -1,    -1,    -1,  1399,    -1,    -1,    -1,
      -1,    -1,  1405,    -1,    -1,    -1,    -1,    -1,    -1,  1412,
      -1,    -1,    -1,    -1,    -1,  1418,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1442,
      -1,    -1,  1445,  1446,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1458,    -1,    -1,    -1,     1,
      -1,  1464,    -1,     5,     6,     7,    -1,     9,    10,    11,
      -1,    13,    -1,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,    31,
      -1,  1494,    -1,    -1,    -1,    -1,    38,    39,    -1,  1502,
      42,    -1,    44,    45,  1507,  1508,    48,    -1,    50,    51,
      52,    -1,    54,    55,    -1,    -1,    58,    59,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,    -1,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
     182,   183,    -1,   185,    -1,    -1,   188,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,   199,    -1,   201,
     202,   203,   204,   205,     1,    -1,    -1,    -1,     5,     6,
       7,    -1,     9,    10,    11,    -1,    13,    -1,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    39,    -1,    -1,    42,    -1,    44,    45,    -1,
      -1,    48,    -1,    50,    51,    52,    -1,    54,    55,    -1,
      -1,    58,    59,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,
      -1,   188,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,   199,    -1,   201,   202,   203,   204,   205,     1,
      -1,    -1,    -1,     5,     6,     7,    -1,     9,    10,    11,
      -1,    13,    -1,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,
      42,    -1,    44,    45,    -1,    -1,    48,    -1,    50,    51,
      52,    -1,    54,    55,    -1,    -1,    58,    59,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,    -1,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
     182,   183,    -1,   185,    -1,    -1,   188,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,   199,    -1,   201,
     202,   203,   204,   205,     1,    -1,    -1,    -1,     5,     6,
       7,    -1,     9,    10,    11,    -1,    13,    -1,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    39,    -1,    -1,    42,    -1,    44,    45,    -1,
      -1,    48,    -1,    50,    51,    52,    -1,    54,    55,    -1,
      -1,    58,    59,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,
      -1,   188,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,   199,    -1,   201,   202,   203,   204,   205,     5,
       6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    52,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,    -1,   199,    -1,   201,   202,    -1,   204,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    70,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,    -1,   199,   200,   201,   202,    13,   204,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,    -1,   199,    -1,   201,   202,    13,   204,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,    -1,   199,    -1,   201,   202,    -1,   204,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    70,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,    -1,   199,    -1,   201,   202,    -1,   204,    15,
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
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,   198,   199,    -1,   201,   202,    13,   204,    15,
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
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,    -1,   199,    -1,   201,   202,    -1,   204,    15,
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
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,   198,   199,    -1,   201,   202,    -1,   204,    15,
      16,    17,    18,    19,    -1,    -1,    22,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,    -1,   199,    -1,   201,   202,    -1,   204,    15,
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
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,    -1,   199,   200,   201,   202,    -1,   204,    15,
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
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,    -1,   199,    -1,   201,   202,    -1,   204,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,    -1,   199,    -1,   201,   202,    -1,   204,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,    -1,   199,    -1,   201,   202,    -1,   204,    15,
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
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,    -1,   199,    -1,   201,   202,    -1,   204,    15,
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
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,    -1,   199,   200,   201,   202,    -1,   204,    15,
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
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,    -1,   199,   200,   201,   202,    -1,   204,    15,
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
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,    -1,   199,   200,   201,   202,    -1,   204,    15,
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
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,    -1,   199,   200,   201,   202,    -1,   204,    15,
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
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,    -1,   199,   200,   201,   202,    -1,   204,    15,
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
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,    -1,   199,    -1,   201,   202,    -1,   204,    15,
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
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,     5,
       6,   197,    -1,   199,    -1,   201,   202,    -1,   204,    15,
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
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    19,   165,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,   185,
      41,    -1,   188,   189,    -1,    -1,    -1,    -1,    49,    -1,
      -1,   197,    -1,   199,    -1,   201,   202,    -1,   204,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    -1,    -1,    -1,    -1,   138,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,   162,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
     201,    -1,    -1,    -1,    19,   206,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,   162,    -1,   164,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,   201,    -1,   203,    -1,    19,    -1,    -1,    -1,
      21,    22,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,   162,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,   201,    21,    22,    -1,
      -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    -1,   138,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,   145,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    21,    22,   196,   197,    -1,   201,   200,
      -1,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    71,    -1,
      73,   145,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,   117,   118,    -1,    -1,    -1,    21,
      22,    -1,   196,   197,    -1,    -1,   200,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,   162,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,   196,   197,
      -1,    -1,   200,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,   196,   197,    -1,    -1,   200,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
     196,   197,    -1,    -1,   200,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,   196,   197,    -1,    -1,
     200,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,   196,   197,    -1,    -1,   200,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,   196,   197,
      -1,    -1,   200,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,   196,   197,    -1,    -1,   200,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
     196,   197,    -1,    -1,   200,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,   196,   197,    -1,    -1,
     200,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,   196,   197,    -1,    -1,   200,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,   196,   197,
      -1,    -1,   200,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,   196,   197,    -1,    -1,   200,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
     196,   197,    -1,    -1,   200,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,   196,   197,    -1,    -1,
     200,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,   196,   197,    -1,    -1,   200,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,   196,   197,
      -1,    -1,   200,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,   196,   197,    -1,    -1,   200,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
     196,   197,    -1,    -1,   200,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,   196,   197,    -1,    -1,
     200,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,   196,   197,    -1,    -1,   200,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,   196,   197,
      -1,    -1,   200,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,   196,   197,    -1,    -1,   200,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
     196,   197,    -1,    -1,   200,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,   196,   197,    -1,    -1,
     200,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,   196,   197,    -1,    -1,   200,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,   196,   197,
      -1,    -1,   200,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,   196,   197,    -1,    -1,   200,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
     196,   197,    -1,    -1,   200,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,   196,   197,    -1,    -1,
     200,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,   196,   197,    -1,    -1,   200,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,   196,   197,
      -1,    -1,   200,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,   196,   197,    -1,    -1,   200,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
     196,   197,   198,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,   196,   197,   198,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,   196,   197,
     198,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   196,   197,   198,   115,   116,   117,   118,   119,
      21,    22,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    38,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   196,   197,   198,    -1,
      -1,    -1,    -1,    21,    22,    -1,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,    -1,    -1,   127,   128,   129,    -1,
      -1,   132,   133,   134,   135,   136,    -1,    -1,   139,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,   174,    -1,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,   188,   189,    21,
      22,    -1,    -1,    -1,    -1,   196,   197,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,   137,
      -1,   139,   140,   141,    71,    72,    73,   145,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,    -1,   196,   197,
      -1,    -1,    -1,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,   153,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,   162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   196,   197,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,    -1,    -1,   174,    -1,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    21,
      22,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     196,   197,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,    -1,
     133,   134,    21,    22,    -1,    -1,   139,   140,   141,    -1,
      -1,    -1,   145,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    21,    22,    -1,    -1,   139,   140,   141,
      -1,   174,    -1,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   196,   197,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   196,   197,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    21,    22,    -1,    -1,
     139,    -1,   141,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    21,    22,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,    -1,   133,   134,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   196,   197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   196,   197,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,
      -1,    -1,   122,    -1,    -1,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   196,   197,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   196,   197,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,    -1,    -1,    -1,    -1,   128,   129,    -1,   162,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   196,   197
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   208,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   164,   185,   197,   203,   210,   211,   214,
     220,   222,   223,   227,   266,   267,   271,   300,   378,   386,
     393,   403,   450,   455,   460,    19,    20,   162,   258,   259,
     260,   155,   228,   229,   162,   185,   224,   225,    57,    63,
     383,   384,   162,   201,   213,   461,   451,   456,   138,   162,
     288,    34,    63,   131,   189,   199,   262,   263,   264,   265,
     288,   210,   210,   210,    36,     8,   404,    62,   374,   173,
     172,   175,   172,   224,    22,    57,   184,   196,   226,   162,
     210,   374,   383,   383,   383,   162,   138,   221,   264,   264,
     264,   199,   139,   140,   141,   172,   198,   394,    57,    63,
     272,   274,     5,     6,   400,    57,    63,   375,    15,    16,
     155,   160,   162,   165,   199,   216,   259,   155,   229,   162,
     162,   162,   385,    57,    63,   212,   162,   162,   162,   162,
     166,   219,   200,   260,   264,   264,   264,   264,   164,   233,
     234,    57,    63,   387,   275,    57,    63,   401,   204,   379,
     166,   167,   168,   215,    15,    16,   155,   160,   162,   216,
     256,   257,   226,   173,   163,   391,   392,   234,   234,   166,
     200,   164,   162,   388,    35,    71,    73,    75,    76,    77,
      78,    79,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    93,    94,    95,    98,    99,   100,   101,
     117,   118,   162,   270,   273,   405,   376,   202,   261,   347,
     166,   167,   168,   172,   200,    19,    25,    31,    41,    49,
      64,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   151,   201,   288,   408,   410,   411,
     414,   420,   421,   449,   163,   462,   452,   457,   175,   389,
      21,    22,    38,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   127,   128,   129,   132,   133,   134,   135,   136,   139,
     140,   141,   142,   143,   144,   145,   174,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   188,   189,   196,
     197,    35,    35,   199,   268,   234,   106,   398,   399,   205,
     210,   377,   259,   202,   162,   370,   373,   256,   179,   179,
     179,   199,   179,   179,   199,   179,   179,   179,   179,   179,
     179,   199,   288,    33,    60,    61,   123,   127,   174,   178,
     181,   197,   206,   419,   176,   204,   204,   204,    75,    79,
      93,    94,    98,    99,   100,   101,   409,   392,   162,   162,
     162,   198,    22,   162,   198,   150,   200,   347,   357,   358,
     175,   269,   279,   280,   162,   234,   347,   234,   259,   172,
     175,   178,   368,   422,   427,   429,     5,     6,    15,    16,
      17,    18,    19,    25,    27,    31,    39,    45,    48,    51,
      55,    65,    68,    69,    80,   102,   103,   104,   117,   118,
     146,   147,   148,   149,   150,   152,   154,   155,   156,   157,
     158,   159,   160,   161,   165,   181,   182,   183,   188,   189,
     197,   199,   201,   202,   204,   218,   220,   282,   288,   293,
     305,   312,   315,   318,   322,   324,   326,   327,   329,   334,
     337,   338,   345,   346,   408,   465,   473,   483,   486,   498,
     501,   431,   425,   162,   415,   433,   435,   437,   439,   441,
     443,   445,   447,   338,   179,   199,    33,   178,    33,   178,
     197,   206,   198,   338,   197,   206,   420,   463,   453,   458,
     204,   162,   132,   199,     7,    50,   299,   200,   203,   449,
     204,   175,   397,   406,   147,   162,   369,   372,   234,   162,
     420,   126,   132,   173,   367,   449,   449,   418,   449,   179,
     179,   179,   290,   410,   465,   288,   179,     5,   102,   103,
     179,   199,   179,   199,   199,   179,   179,   199,   179,   199,
     179,   199,   179,   179,   199,   179,   179,   338,   338,   199,
     199,   199,   199,   199,   199,   217,   338,   338,   338,   338,
     338,    13,    49,   316,   338,   316,   202,    13,   338,   482,
     497,   199,   197,   266,   202,   204,   318,   323,    21,    22,
     115,   116,   117,   118,   119,   122,   123,   124,   125,   127,
     128,   129,   130,   131,   133,   134,   139,   140,   141,   145,
     174,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   196,   197,   200,   199,   449,   449,   200,   162,   412,
     449,   268,   449,   268,   449,   268,   162,   359,   360,   449,
     162,   362,   363,   200,   424,   285,   316,   198,   198,   198,
     338,   162,   413,   359,   361,   362,   364,   395,   338,   147,
     162,   366,   407,   357,   284,   288,   203,   204,   402,   199,
      43,   172,   175,   368,   210,   367,   338,   180,   180,   180,
     163,   172,   209,   210,   417,   474,   476,   291,   199,   179,
     199,   313,   179,   179,   179,   493,   316,   420,   497,   338,
     306,   308,   338,   310,   338,   495,   316,   480,   484,   316,
     478,   420,   338,   338,   338,   338,   338,   338,   168,   169,
     215,   199,    13,   172,   500,   500,   147,   152,   179,   288,
     328,   199,   137,   172,   500,    70,   153,   162,   197,   200,
     316,   466,   468,   265,     4,   321,   202,   284,   266,    19,
     153,   162,   408,    19,   153,   162,   408,   338,   338,   338,
     338,   338,   338,   162,   338,   153,   162,   338,   338,   338,
     408,   338,   338,   338,   338,   338,   338,    22,   338,   338,
     338,   338,   338,   338,   338,   338,   338,   338,   338,   128,
     129,   153,   162,   196,   197,   335,   408,   338,   200,   316,
     180,   180,   180,   203,   180,   269,   180,   269,   180,   269,
     175,   180,   417,   175,   180,   417,   287,   449,   200,   500,
     198,   209,   381,   390,   210,   239,   240,   239,   240,   162,
     380,   382,   200,   199,    43,   126,   172,   173,   175,   178,
     365,     1,    26,    28,    29,    38,    44,    52,    54,    58,
      59,    65,   105,   210,   230,   231,   238,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   281,   283,
     289,   294,   295,   296,   297,   298,   300,   304,   325,   338,
     353,   338,   162,   162,   420,   367,   338,   210,   423,   428,
     430,   449,   420,   420,   449,    70,   316,   468,   472,   162,
     338,   449,   487,   489,   491,   420,   500,   180,   417,   500,
     200,   420,   420,   200,   420,   200,   420,   500,   420,   420,
     500,   420,   180,   200,   200,   200,   200,   200,   200,   338,
     407,   199,   338,   198,   200,   199,   199,   330,   332,   407,
     338,   482,   205,   162,   200,   468,   199,   132,   365,   466,
     172,   200,   172,   200,   198,   199,   268,   205,   321,   179,
     199,   179,   199,   199,   199,   198,    19,   153,   162,   408,
     175,   153,   162,   338,   199,   199,   153,   162,   338,     1,
     199,   198,   172,   200,   432,   426,   416,   162,   434,   180,
     438,   180,   442,   180,   449,   446,   359,   449,   448,   362,
     180,   417,   162,   209,   464,   210,   359,   454,   362,   459,
     173,   381,   390,   338,   162,   162,   449,   338,   126,   338,
     279,    61,   338,   162,   210,   155,    58,   338,   234,   126,
     338,   210,   210,    10,    10,    11,   236,    13,     9,    42,
     210,   205,   210,   210,   210,   210,   210,    66,   301,   210,
     107,   108,   109,   110,   111,   112,   113,   114,   120,   121,
     126,   132,   135,   136,   142,   143,   144,   173,   164,   197,
     205,   267,   354,   200,    43,   210,   367,   338,   210,   180,
     180,   180,   468,   200,   200,   200,   180,   417,   200,   180,
     420,   420,   420,   180,   200,   199,   420,   200,   180,   180,
     180,   180,   200,   180,   180,   200,   180,   321,   199,   170,
      20,   407,   338,   338,   420,   268,    20,   200,   338,   338,
     338,   198,   197,   153,   162,   126,   132,   162,   173,   178,
     319,   320,   269,   268,   339,   338,   341,   338,   200,   316,
     338,   179,   199,   338,   199,   198,   338,   197,   200,   316,
     199,   198,   336,   200,   316,   436,   440,   444,   199,   449,
     205,   205,   205,   338,   380,   396,   200,    43,   365,   338,
      14,   338,   173,   175,   155,   279,   338,   199,   199,   199,
     199,   199,    37,   276,   199,   204,   303,   372,   338,   338,
     338,   338,   338,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   338,   265,     8,   347,   352,
     162,   338,   210,   475,   477,   292,   200,   199,   162,   314,
     180,   180,   180,   494,   286,   497,   180,   307,   309,   311,
     496,   481,   485,   479,   199,   234,   200,   316,   316,    20,
     200,   200,   180,   269,   316,   200,   200,   466,   199,   132,
     365,   162,   162,   199,   162,   162,   172,   200,   234,   269,
     420,   200,   449,   200,   200,   200,   343,   338,   338,   200,
     466,   200,   338,   200,   285,   205,   162,   338,   279,   338,
     338,   338,   407,   338,   338,   277,   162,   302,   371,   198,
      57,    63,   350,    67,   351,   210,   210,   199,   199,   338,
     180,   488,   490,   492,   199,   200,   500,   199,   338,   338,
     338,   199,    70,   472,   199,   199,   200,   338,   279,   200,
     200,   316,   331,   180,   200,   132,   365,   198,   338,   338,
     338,   162,   319,   137,   279,   317,   234,   180,   180,   449,
     200,   200,   198,   200,   200,   200,   200,    20,   200,   200,
     204,   172,   200,   205,   210,   372,    47,   351,    46,   106,
     348,   472,   472,   200,   199,   199,   199,   199,   285,   286,
     316,   472,   466,   467,   200,   203,   200,   328,   333,   203,
     338,   338,   200,   200,   200,   126,   338,   279,   340,   342,
     180,   200,    12,   237,   234,   316,   234,   234,   284,   162,
     175,   368,    33,   349,   348,   350,   199,   466,   469,   470,
     471,   471,   338,   472,   472,   467,   200,   200,   500,   471,
     200,   338,   203,   328,   482,   132,   365,   338,   344,   238,
     294,   295,   296,   297,   338,   210,   241,   242,   244,   200,
     279,   279,   278,   420,   367,   355,   349,   366,   466,   172,
     500,   200,   200,   200,   471,   471,   200,   200,   200,   203,
     499,   338,   499,   338,   338,    12,    23,    24,   232,   235,
     204,   238,   234,   205,   367,   338,   273,   356,   200,   199,
     200,   200,    53,   198,   499,   205,   234,   199,   284,   210,
     279,   338,   210,   210,   273,   466,   338,   198,   279,   338,
     243,   210,   234,   200,   200,   205,   279,   234,   276,   279,
     235
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   207,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   209,   209,   210,
     210,   211,   212,   212,   212,   213,   213,   214,   215,   215,
     215,   215,   216,   216,   217,   217,   217,   218,   219,   219,
     221,   220,   222,   223,   224,   224,   224,   224,   225,   225,
     226,   226,   227,   228,   228,   229,   229,   230,   231,   231,
     232,   232,   233,   233,   234,   234,   235,   235,   235,   236,
     236,   237,   237,   238,   238,   238,   238,   238,   239,   239,
     240,   240,   242,   243,   241,   244,   241,   246,   245,   247,
     249,   248,   250,   252,   251,   254,   253,   255,   256,   256,
     256,   256,   256,   256,   257,   257,   258,   258,   258,   259,
     259,   259,   259,   259,   259,   259,   259,   260,   260,   261,
     261,   262,   262,   262,   263,   263,   264,   264,   264,   264,
     264,   264,   264,   265,   265,   266,   266,   267,   267,   268,
     268,   268,   269,   269,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   271,
     272,   272,   272,   273,   275,   274,   276,   277,   278,   276,
     280,   281,   279,   282,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   284,   284,   284,   285,   285,   286,   286,
     287,   287,   288,   288,   288,   289,   289,   291,   292,   290,
     290,   293,   293,   293,   293,   293,   293,   294,   295,   296,
     296,   296,   297,   297,   298,   299,   299,   299,   300,   300,
     301,   301,   302,   302,   303,   303,   304,   304,   304,   306,
     307,   305,   308,   309,   305,   310,   311,   305,   313,   314,
     312,   315,   315,   315,   316,   316,   317,   317,   317,   318,
     318,   318,   319,   319,   319,   319,   319,   320,   320,   321,
     321,   322,   323,   323,   324,   324,   324,   324,   324,   324,
     324,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     326,   326,   327,   327,   328,   328,   329,   330,   331,   329,
     332,   333,   329,   334,   334,   334,   334,   334,   334,   334,
     335,   336,   334,   337,   337,   337,   337,   337,   337,   337,
     338,   338,   338,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   339,   340,   338,   338,   338,
     338,   341,   342,   338,   338,   338,   343,   344,   338,   338,
     338,   338,   338,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   345,   345,   345,   346,   346,   346,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   347,   347,   348,   348,   348,   349,   349,   350,   350,
     350,   351,   351,   352,   353,   353,   354,   353,   355,   353,
     356,   353,   357,   357,   358,   358,   359,   359,   360,   360,
     361,   361,   361,   362,   363,   363,   364,   364,   364,   365,
     365,   366,   366,   366,   366,   366,   367,   367,   367,   368,
     368,   369,   369,   369,   369,   369,   370,   370,   371,   371,
     371,   372,   372,   372,   373,   373,   373,   374,   374,   375,
     375,   375,   376,   376,   377,   376,   378,   379,   378,   380,
     380,   381,   381,   382,   382,   382,   383,   383,   383,   385,
     384,   386,   387,   387,   387,   388,   389,   389,   390,   390,
     391,   391,   392,   392,   394,   395,   396,   393,   397,   397,
     398,   398,   399,   400,   400,   401,   401,   401,   402,   402,
     404,   405,   406,   403,   407,   407,   407,   407,   407,   408,
     408,   408,   408,   408,   408,   408,   408,   408,   408,   408,
     408,   408,   408,   408,   408,   408,   408,   408,   408,   408,
     408,   408,   408,   408,   408,   408,   409,   409,   409,   409,
     409,   409,   409,   409,   410,   411,   411,   411,   412,   412,
     413,   413,   413,   415,   416,   414,   417,   417,   418,   418,
     419,   419,   419,   419,   420,   420,   421,   421,   421,   421,
     422,   423,   421,   421,   421,   424,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   425,
     426,   421,   421,   427,   428,   421,   429,   430,   421,   431,
     432,   421,   421,   433,   434,   421,   435,   436,   421,   421,
     437,   438,   421,   439,   440,   421,   421,   441,   442,   421,
     443,   444,   421,   445,   446,   421,   447,   448,   421,   449,
     449,   449,   451,   452,   453,   454,   450,   456,   457,   458,
     459,   455,   461,   462,   463,   464,   460,   465,   465,   465,
     465,   465,   466,   466,   466,   466,   466,   466,   466,   466,
     467,   468,   469,   469,   470,   470,   471,   471,   472,   472,
     474,   475,   473,   476,   477,   473,   478,   479,   473,   480,
     481,   473,   482,   482,   483,   484,   485,   483,   486,   487,
     488,   486,   489,   490,   486,   491,   492,   486,   486,   493,
     494,   486,   486,   495,   496,   486,   497,   497,   498,   498,
     498,   498,   499,   499,   500,   500,   501,   501,   501
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     3,
       3,     2,     2,     2,     2,     2,     2,     1,     1,     1,
       1,     2,     0,     1,     1,     1,     1,     4,     1,     1,
       2,     2,     3,     2,     0,     2,     4,     3,     1,     2,
       0,     4,     2,     2,     1,     2,     3,     3,     2,     4,
       0,     1,     2,     1,     3,     1,     3,     3,     3,     2,
       1,     1,     1,     2,     0,     1,     0,     3,     7,     1,
       1,     0,     2,     1,     1,     1,     1,     1,     1,     2,
       0,     1,     0,     0,     6,     0,     3,     0,     8,     7,
       0,     9,     3,     0,     7,     0,     7,     4,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     1,     5,     1,     3,     3,
       4,     1,     1,     1,     1,     4,     1,     2,     3,     3,
       3,     3,     2,     1,     3,     0,     3,     0,     4,     0,
       2,     3,     0,     2,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     3,     3,
       2,     2,     3,     4,     3,     2,     2,     2,     2,     2,
       3,     3,     3,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       0,     1,     1,     3,     0,     5,     0,     0,     0,     6,
       0,     0,     6,     2,     1,     2,     2,     1,     1,     1,
       1,     2,     1,     2,     2,     2,     2,     1,     1,     1,
       2,     2,     2,     0,     2,     2,     0,     2,     0,     2,
       1,     3,     1,     3,     2,     2,     3,     0,     0,     5,
       1,     2,     5,     5,     5,     6,     2,     1,     1,     1,
       2,     3,     2,     3,     4,     1,     1,     0,     1,     1,
       1,     0,     1,     3,     8,     7,     3,     3,     5,     0,
       0,     7,     0,     0,     7,     0,     0,     7,     0,     0,
       6,     5,     8,    10,     1,     3,     1,     2,     3,     1,
       1,     2,     2,     2,     2,     2,     4,     1,     3,     0,
       4,     7,     7,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       6,     8,     5,     6,     1,     4,     3,     0,     0,     8,
       0,     0,     9,     3,     4,     5,     6,     8,     5,     6,
       0,     0,     5,     3,     4,     4,     5,     4,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       4,     4,     5,     4,     5,     3,     4,     1,     1,     2,
       4,     4,     1,     3,     5,     0,     0,     8,     3,     3,
       3,     0,     0,     8,     3,     4,     0,     0,     9,     4,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       1,     4,     7,     8,     7,     4,     4,     4,     4,     4,
       1,     6,     7,     6,     6,     7,     7,     6,     7,     6,
       6,     0,     1,     0,     1,     1,     0,     1,     0,     1,
       1,     0,     1,     5,     0,     2,     0,     4,     0,     9,
       0,    11,     3,     4,     1,     3,     1,     3,     1,     3,
       0,     1,     3,     3,     1,     3,     0,     1,     3,     1,
       1,     1,     2,     3,     5,     3,     1,     1,     1,     0,
       1,     1,     4,     3,     3,     5,     1,     3,     0,     2,
       2,     4,     6,     5,     4,     6,     5,     0,     1,     0,
       1,     1,     0,     2,     0,     4,     6,     0,     6,     1,
       3,     1,     2,     0,     1,     3,     0,     1,     1,     0,
       5,     3,     0,     1,     1,     1,     0,     2,     0,     1,
       1,     2,     0,     1,     0,     0,     0,    13,     0,     2,
       0,     1,     3,     1,     1,     0,     1,     1,     1,     3,
       0,     0,     0,     9,     1,     4,     3,     3,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     1,     3,
       0,     1,     3,     0,     0,     6,     1,     1,     1,     3,
       3,     2,     4,     3,     1,     2,     1,     1,     1,     1,
       0,     0,     6,     4,     5,     0,     9,     4,     2,     2,
       3,     2,     3,     2,     2,     3,     3,     3,     2,     0,
       0,     6,     2,     0,     0,     6,     0,     0,     6,     0,
       0,     6,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     0,     0,     6,     0,     0,     6,     1,
       3,     3,     0,     0,     0,     0,    12,     0,     0,     0,
       0,    12,     0,     0,     0,     0,    12,     1,     1,     1,
       1,     1,     3,     3,     5,     5,     6,     6,     8,     8,
       1,     1,     3,     5,     1,     2,     1,     0,     0,     1,
       0,     0,    10,     0,     0,    10,     0,     0,     9,     0,
       0,     7,     3,     1,     5,     0,     0,    10,     4,     0,
       0,    11,     0,     0,    11,     0,     0,    10,     5,     0,
       0,     9,     5,     0,     0,    10,     1,     3,     4,     5,
       7,     9,     0,     3,     0,     1,    11,    12,    11
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = DAS2_YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == DAS2_YYEMPTY)                                        \
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
   Use DAS2_YYerror or DAS2_YYUNDEF. */
#define YYERRCODE DAS2_YYUNDEF

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
#if DAS2_YYDEBUG

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

#  elif defined DAS2_YYLTYPE_IS_TRIVIAL && DAS2_YYLTYPE_IS_TRIVIAL

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
#else /* !DAS2_YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !DAS2_YYDEBUG */


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

    case YYSYMBOL_expression_if_block: /* expression_if_block  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_if_then_else: /* expression_if_then_else  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_if_then_else_oneliner: /* expression_if_then_else_oneliner  */
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

    case YYSYMBOL_optional_annotation_list_with_emit_semis: /* optional_annotation_list_with_emit_semis  */
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

    case YYSYMBOL_expression_block_finally: /* expression_block_finally  */
            { delete ((*yyvaluep).pExpression); }
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

    case YYSYMBOL_optional_expr_list: /* optional_expr_list  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_optional_expr_map_tuple_list: /* optional_expr_map_tuple_list  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_type_declaration_no_options_list: /* type_declaration_no_options_list  */
            { deleteTypeDeclarationList(((*yyvaluep).pTypeDeclList)); }
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

    case YYSYMBOL_expression_return: /* expression_return  */
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

    case YYSYMBOL_expr_generator: /* expr_generator  */
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

    case YYSYMBOL_global_let_variable_name_with_pos_list: /* global_let_variable_name_with_pos_list  */
            { delete ((*yyvaluep).pNameWithPosList); }
        break;

    case YYSYMBOL_variable_declaration_list: /* variable_declaration_list  */
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
        break;

    case YYSYMBOL_let_variable_declaration: /* let_variable_declaration  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_global_let_variable_declaration: /* global_let_variable_declaration  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_global_variable_declaration_list: /* global_variable_declaration_list  */
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
        break;

    case YYSYMBOL_enum_expression: /* enum_expression  */
            { delete ((*yyvaluep).pEnumPair); }
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

    case YYSYMBOL_type_declaration_no_options_no_dim: /* type_declaration_no_options_no_dim  */
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

    case YYSYMBOL_make_struct_dim_list: /* make_struct_dim_list  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_struct_dim_decl: /* make_struct_dim_decl  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_optional_make_struct_dim_decl: /* optional_make_struct_dim_decl  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_struct_decl: /* make_struct_decl  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_map_tuple: /* make_map_tuple  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_tuple_call: /* make_tuple_call  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_dim_decl: /* make_dim_decl  */
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
# if defined DAS2_YYLTYPE_IS_TRIVIAL && DAS2_YYLTYPE_IS_TRIVIAL
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

  yychar = DAS2_YYEMPTY; /* Cause a token to be read.  */

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
  if (yychar == DAS2_YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= DAS2_YYEOF)
    {
      yychar = DAS2_YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == DAS2_YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = DAS2_YYUNDEF;
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
  yychar = DAS2_YYEMPTY;
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
                das2_yyerror(scanner,"module name has to be first declaration",tokAt(scanner,(yylsp[0])), CompilationError::syntax_error);
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

  case 21: /* top_level_reader_macro: expr_reader SEMICOLON  */
                                   {
        delete (yyvsp[-1].pExpression);    // we do nothing, we don't even attemp to 'visit'
    }
    break;

  case 22: /* optional_public_or_private_module: %empty  */
                        { (yyval.b) = yyextra->g_Program->policies.default_module_public; }
    break;

  case 23: /* optional_public_or_private_module: "public"  */
                        { (yyval.b) = true; }
    break;

  case 24: /* optional_public_or_private_module: "private"  */
                        { (yyval.b) = false; }
    break;

  case 25: /* module_name: '$'  */
                    { (yyval.s) = new string("$"); }
    break;

  case 26: /* module_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 27: /* module_declaration: "module" module_name optional_shared optional_public_or_private_module  */
                                                                                                      {
        yyextra->g_Program->thisModuleName = *(yyvsp[-2].s);
        yyextra->g_Program->thisModule->isPublic = (yyvsp[0].b);
        yyextra->g_Program->thisModule->isModule = true;
        if ( yyextra->g_Program->thisModule->name.empty() ) {
            yyextra->g_Program->thisModule->name = *(yyvsp[-2].s);
        } else if ( yyextra->g_Program->thisModule->name != *(yyvsp[-2].s) ){
            das2_yyerror(scanner,"this module already has a name " + yyextra->g_Program->thisModule->name,tokAt(scanner,(yylsp[-2])),
                CompilationError::module_already_has_a_name);
        }
        if ( !yyextra->g_Program->policies.ignore_shared_modules ) {
            yyextra->g_Program->promoteToBuiltin = (yyvsp[-1].b);
        }
        delete (yyvsp[-2].s);
    }
    break;

  case 28: /* character_sequence: STRING_CHARACTER  */
                                                                                  { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
    break;

  case 29: /* character_sequence: STRING_CHARACTER_ESC  */
                                                                                  { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
    break;

  case 30: /* character_sequence: character_sequence STRING_CHARACTER  */
                                                                                  { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
    break;

  case 31: /* character_sequence: character_sequence STRING_CHARACTER_ESC  */
                                                                                  { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
    break;

  case 32: /* string_constant: "start of the string" character_sequence "end of the string"  */
                                                           { (yyval.s) = (yyvsp[-1].s); }
    break;

  case 33: /* string_constant: "start of the string" "end of the string"  */
                                                           { (yyval.s) = new string(); }
    break;

  case 34: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 35: /* string_builder_body: string_builder_body character_sequence  */
                                                                                  {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das2_yyerror(scanner,"invalid escape sequence",tokAt(scanner,(yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt(scanner,(yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 36: /* string_builder_body: string_builder_body "{" expr "}"  */
                                                                                                        {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 37: /* string_builder: "start of the string" string_builder_body "end of the string"  */
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

  case 38: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das2_yyend_reader(scanner);
        }
    }
    break;

  case 39: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das2_yyend_reader(scanner);
        }
    }
    break;

  case 40: /* $@1: %empty  */
                                        {
        auto macros = yyextra->g_Program->getReaderMacro(*(yyvsp[0].s));
        if ( macros.size()==0 ) {
            das2_yyerror(scanner,"reader macro " + *(yyvsp[0].s) + " not found",tokAt(scanner,(yylsp[0])),
                CompilationError::unsupported_read_macro);
        } else if ( macros.size()>1 ) {
            string options;
            for ( auto & x : macros ) {
                options += "\t" + x->module->name + "::" + x->name + "\n";
            }
            das2_yyerror(scanner,"too many options for the reader macro " + *(yyvsp[0].s) +  "\n" + options, tokAt(scanner,(yylsp[0])),
                CompilationError::unsupported_read_macro);
        } else if ( yychar != '~' ) {
            das2_yyerror(scanner,"expecting ~ after the reader macro", tokAt(scanner,(yylsp[0])),
                CompilationError::syntax_error);
        } else {
            yyextra->g_ReaderMacro = macros.back().get();
            yyextra->g_ReaderExpr = new ExprReader(tokAt(scanner,(yylsp[-1])),yyextra->g_ReaderMacro);
            yyclearin ;
            das2_yybegin_reader(scanner);
        }
    }
    break;

  case 41: /* expr_reader: '%' name_in_namespace $@1 reader_character_sequence  */
                                     {
        yyextra->g_ReaderExpr->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = yyextra->g_ReaderExpr;
        int thisLine = 0;
        FileInfo * info = nullptr;
        if ( auto seqt = yyextra->g_ReaderMacro->suffix(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, thisLine, info, tokAt(scanner,(yylsp[0]))) ) {
            das2_accept_sequence(scanner,seqt,strlen(seqt),thisLine,info);
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

  case 42: /* options_declaration: "options" annotation_argument_list  */
                                                   {
        if ( yyextra->g_Program->options.size() ) {
            for ( auto & opt : *(yyvsp[0].aaList) ) {
                if ( yyextra->g_Access->isOptionAllowed(opt.name, yyextra->g_Program->thisModuleName) ) {
                    yyextra->g_Program->options.push_back(opt);
                } else {
                    das2_yyerror(scanner,"option " + opt.name + " is not allowed here",
                        tokAt(scanner,(yylsp[0])), CompilationError::invalid_option);
                }
            }
        } else {
            swap ( yyextra->g_Program->options, *(yyvsp[0].aaList) );
        }
        delete (yyvsp[0].aaList);
    }
    break;

  case 44: /* require_module_name: "name"  */
                   {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 45: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 46: /* require_module_name: require_module_name '.' "name"  */
                                                {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 47: /* require_module_name: require_module_name '/' "name"  */
                                                {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 48: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 49: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 50: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 51: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 55: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 56: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 57: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 58: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 59: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 60: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 61: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 66: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 67: /* expression_else: "else" optional_emit_semis expression_block  */
                                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 68: /* expression_else: elif_or_static_elif '(' expr ')' optional_emit_semis expression_block expression_else  */
                                                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-4].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-6].b);
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

  case 72: /* expression_else_one_liner: "else" expression_if_one_liner  */
                                                      {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 73: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 74: /* expression_if_one_liner: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 75: /* expression_if_one_liner: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 76: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 77: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 82: /* $@2: %empty  */
        {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 83: /* $@3: %empty  */
                                      {
        yyextra->pop_nesteds();
    }
    break;

  case 84: /* expression_if_block: $@2 '{' expressions $@3 '}' expression_block_finally  */
                                        {
        (yyval.pExpression) = (yyvsp[-3].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-4]),(yylsp[0]));
        if ( (yyvsp[0].pExpression) ) {
            auto pF = (ExprBlock *) (yyvsp[0].pExpression);
            auto pB = (ExprBlock *) (yyval.pExpression);
            swap ( pB->finalList, pF->list );
            delete pF;
        }
    }
    break;

  case 85: /* $@4: %empty  */
       {
        yyextra->das_keyword = false;
    }
    break;

  case 86: /* expression_if_block: $@4 expression_if_one_liner SEMICOLON  */
                                               {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 87: /* $@5: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 88: /* expression_if_then_else: $@5 if_or_static_if '(' expr ')' optional_emit_semis expression_if_block expression_else  */
                                                                                                                   {
        yyextra->das_keyword = false;
        if ( !(yyvsp[-1].pExpression)->rtti_isBlock() && (yyvsp[0].pExpression)) {
            das2_yyerror(scanner,"if one-liner can't have else clause",tokAt(scanner,(yylsp[-6])), CompilationError::syntax_error);
        }
        auto blk = (yyvsp[-1].pExpression)->rtti_isBlock() ? static_cast<ExprBlock *>((yyvsp[-1].pExpression)) : ast_wrapInBlock((yyvsp[-1].pExpression));
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-4].pExpression)),
            blk,ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-6].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 89: /* expression_if_then_else_oneliner: expression_if_one_liner "if" '(' expr ')' expression_else_one_liner SEMICOLON  */
                                                                                                                      {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr(ast_wrapInBlock((yyvsp[-6].pExpression))),(yyvsp[-1].pExpression) ? ExpressionPtr(ast_wrapInBlock((yyvsp[-1].pExpression))) : nullptr);
    }
    break;

  case 90: /* $@6: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 91: /* expression_for_loop: $@6 "for" '(' variable_name_with_pos_list "in" expr_list ')' optional_emit_semis expression_block  */
                                                                                                                                 {
        yyextra->das_keyword = false;
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-5].pNameWithPosList),(yyvsp[-3].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 92: /* expression_unsafe: "unsafe" optional_emit_semis expression_block  */
                                                                    {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-2])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 93: /* $@7: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 94: /* expression_while_loop: $@7 "while" '(' expr ')' optional_emit_semis expression_block  */
                                                                                         {
        yyextra->das_keyword = false;
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-5])));
        pWhile->cond = ExpressionPtr((yyvsp[-3].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 95: /* $@8: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 96: /* expression_with: $@8 "with" '(' expr ')' optional_emit_semis expression_block  */
                                                                                   {
        yyextra->das_keyword = false;
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-5])));
        pWith->with = ExpressionPtr((yyvsp[-3].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
    break;

  case 97: /* expression_with_alias: "assume" "name" '=' expr  */
                                                      {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-3])), *(yyvsp[-2].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-2].s);
    }
    break;

  case 98: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 99: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 100: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 101: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 102: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 103: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 104: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 105: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 106: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 107: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 108: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 109: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 110: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 111: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 112: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 113: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 114: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 115: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 116: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 117: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 118: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 119: /* metadata_argument_list: '@' annotation_argument optional_emit_semis  */
                                                         {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[-1].aa));
    }
    break;

  case 120: /* metadata_argument_list: metadata_argument_list '@' annotation_argument optional_emit_semis  */
                                                                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-3].aaList),(yyvsp[-1].aa));
    }
    break;

  case 121: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 122: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 123: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 124: /* annotation_declaration_basic: annotation_declaration_name  */
                                          {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[0]));
        if ( yyextra->g_Access->isAnnotationAllowed(*(yyvsp[0].s), yyextra->g_Program->thisModuleName) ) {
            if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
                (yyval.fa)->annotation = ann;
            }
        } else {
            das2_yyerror(scanner,"annotation " + *(yyvsp[0].s) + " is not allowed here",
                        tokAt(scanner,(yylsp[0])), CompilationError::invalid_annotation);
        }
        delete (yyvsp[0].s);
    }
    break;

  case 125: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
                                                                                 {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[-3]));
        if ( yyextra->g_Access->isAnnotationAllowed(*(yyvsp[-3].s), yyextra->g_Program->thisModuleName) ) {
            if ( auto ann = findAnnotation(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3]))) ) {
                (yyval.fa)->annotation = ann;
            }
        } else {
            das2_yyerror(scanner,"annotation " + *(yyvsp[-3].s) + " is not allowed here",
                        tokAt(scanner,(yylsp[-3])), CompilationError::invalid_annotation);
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
    break;

  case 126: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 127: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 128: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
                                                                              {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation); }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::And,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 129: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
                                                                            {
        if ( !(yyvsp[-2].fa)->annotation || !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation); }
        if ( !(yyvsp[0].fa)->annotation || !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Or,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 130: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
                                                                              {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation); }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Xor,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 131: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 132: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 133: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 134: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 135: /* optional_annotation_list: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 136: /* optional_annotation_list: '[' annotation_list ']'  */
                                       { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 137: /* optional_annotation_list_with_emit_semis: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 138: /* optional_annotation_list_with_emit_semis: '[' annotation_list ']' optional_emit_semis  */
                                                          { (yyval.faList) = (yyvsp[-2].faList); }
    break;

  case 139: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 140: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 141: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 142: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 143: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 144: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 145: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 146: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 147: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 148: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 149: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 150: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 151: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 152: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 153: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 154: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 155: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 156: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 157: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 158: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 159: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 160: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 161: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 162: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 163: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 164: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 165: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 166: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 167: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 168: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 169: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 170: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 171: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 172: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 173: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 174: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 175: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 176: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 177: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 178: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 179: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 180: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 181: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 182: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 183: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 184: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 185: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 186: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 187: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 188: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 189: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 190: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 191: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 192: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 193: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 194: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 195: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 196: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 197: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 198: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 199: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 200: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 201: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 202: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 203: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 204: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 205: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 206: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 207: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 208: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 209: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 210: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 211: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 212: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 213: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 214: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 215: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 216: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 217: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 218: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 219: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 220: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 221: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 222: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 223: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 224: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 225: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 226: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 227: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 228: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 229: /* global_function_declaration: optional_annotation_list_with_emit_semis "def" function_declaration  */
                                                                                                {
        (yyvsp[0].pFuncDecl)->atDecl = tokRangeAt(scanner,(yylsp[-1]),(yylsp[0]));
        assignDefaultArguments((yyvsp[0].pFuncDecl));
        runFunctionAnnotations(scanner, yyextra, (yyvsp[0].pFuncDecl), (yyvsp[-2].faList), tokAt(scanner,(yylsp[-2])));
        if ( (yyvsp[0].pFuncDecl)->isGeneric() ) {
            implAddGenericFunction(scanner,(yyvsp[0].pFuncDecl));
        } else {
            if ( !yyextra->g_Program->addFunction((yyvsp[0].pFuncDecl)) ) {
                das2_yyerror(scanner,"function is already defined " +
                    (yyvsp[0].pFuncDecl)->getMangledName(),(yyvsp[0].pFuncDecl)->at,
                        CompilationError::function_already_declared);
            }
        }
        (yyvsp[0].pFuncDecl)->delRef();
    }
    break;

  case 230: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 231: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 232: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 233: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 234: /* $@9: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 235: /* function_declaration: optional_public_or_private_function $@9 function_declaration_header optional_emit_semis expression_block  */
                                                                                    {
        (yyvsp[-2].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyvsp[-2].pFuncDecl)->privateFunction = !(yyvsp[-4].b);
        (yyval.pFuncDecl) = (yyvsp[-2].pFuncDecl);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
        }
    }
    break;

  case 236: /* expression_block_finally: %empty  */
        {
        (yyval.pExpression) = nullptr;
    }
    break;

  case 237: /* $@10: %empty  */
                  {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 238: /* $@11: %empty  */
                             {
        yyextra->pop_nesteds();
    }
    break;

  case 239: /* expression_block_finally: "finally" $@10 '{' expressions $@11 '}'  */
          {
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 240: /* $@12: %empty  */
        {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 241: /* $@13: %empty  */
                                      {
        yyextra->pop_nesteds();
    }
    break;

  case 242: /* expression_block: $@12 '{' expressions $@13 '}' expression_block_finally  */
                                        {
        (yyval.pExpression) = (yyvsp[-3].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-4]),(yylsp[0]));
        if ( (yyvsp[0].pExpression) ) {
            auto pF = (ExprBlock *) (yyvsp[0].pExpression);
            auto pB = (ExprBlock *) (yyval.pExpression);
            swap ( pB->finalList, pF->list );
            delete pF;
        }
    }
    break;

  case 243: /* expr_call_pipe: expr_call expr_full_block_assumed_piped  */
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

  case 244: /* expression_any: SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 245: /* expression_any: expr_assign SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 246: /* expression_any: expression_delete SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 247: /* expression_any: expression_let  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 248: /* expression_any: expression_while_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 249: /* expression_any: expression_unsafe  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 250: /* expression_any: expression_with  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 251: /* expression_any: expression_with_alias SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 252: /* expression_any: expression_for_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 253: /* expression_any: expression_break SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 254: /* expression_any: expression_continue SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 255: /* expression_any: expression_return SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 256: /* expression_any: expression_yield SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 257: /* expression_any: expression_if_then_else  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 258: /* expression_any: expression_if_then_else_oneliner  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 259: /* expression_any: expression_try_catch  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 260: /* expression_any: expression_label SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 261: /* expression_any: expression_goto SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 262: /* expression_any: "pass" SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 263: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 264: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 265: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 266: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 267: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
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

  case 272: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 273: /* name_in_namespace: "name" "::" "name"  */
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

  case 274: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 275: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 276: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 277: /* $@14: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 278: /* $@15: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 279: /* new_type_declaration: '<' $@14 type_declaration '>' $@15  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 280: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 281: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 282: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 283: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 284: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 285: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 286: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 287: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 288: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 289: /* expression_return: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 290: /* expression_return: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 291: /* expression_return: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 292: /* expression_yield: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 293: /* expression_yield: "yield" "<-" expr  */
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

  case 304: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 305: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr SEMICOLON  */
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

  case 308: /* expression_let: kwd_let optional_in_scope '{' variable_declaration_list '}'  */
                                                                               {
        (yyval.pExpression) = ast_LetList(scanner,(yyvsp[-4].b),(yyvsp[-3].b),*(yyvsp[-1].pVarDeclList),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 309: /* $@16: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 310: /* $@17: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 311: /* expr_cast: "cast" '<' $@16 type_declaration_no_options '>' $@17 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 312: /* $@18: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 313: /* $@19: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 314: /* expr_cast: "upcast" '<' $@18 type_declaration_no_options '>' $@19 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 315: /* $@20: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 316: /* $@21: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 317: /* expr_cast: "reinterpret" '<' $@20 type_declaration_no_options '>' $@21 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 318: /* $@22: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 319: /* $@23: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 320: /* expr_type_decl: "type" '<' $@22 type_declaration '>' $@23  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 321: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 322: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 323: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" c_or_s "name" '>' '(' expr ')'  */
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

  case 324: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 325: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 326: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 327: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 328: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 329: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 330: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 331: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 332: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 333: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 334: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 335: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 336: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 337: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 338: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 339: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 340: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 341: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis block_or_simple_block  */
                                                                                                                {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 342: /* expr_full_block_assumed_piped: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis expression_block  */
                                                                                                           {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 343: /* expr_full_block_assumed_piped: '{' expressions '}'  */
                                   {
        (yyval.pExpression) = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,new TypeDecl(Type::autoinfer),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[-1])));
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

  case 370: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 371: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 372: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 373: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 374: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 375: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 376: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 377: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 378: /* $@25: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 379: /* func_addr_expr: '@' '@' '<' $@24 type_declaration_no_options '>' $@25 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 380: /* $@26: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 381: /* $@27: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 382: /* func_addr_expr: '@' '@' '<' $@26 optional_function_argument_list optional_function_type '>' $@27 func_addr_name  */
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

  case 383: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 384: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 385: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 386: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 387: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
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

  case 388: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 389: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 390: /* $@28: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 391: /* $@29: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 392: /* expr_field: expr '.' $@28 error $@29  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 393: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 394: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
                                                          {
            auto dd = new ExprMakeStruct(tokAt(scanner,(yylsp[-3])));
            dd->at = tokAt(scanner,(yylsp[-3]));
            dd->makeType = new TypeDecl(Type::alias);
            dd->makeType->alias = *(yyvsp[-3].s);
            dd->useInitializer = false;
            dd->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = dd;
    }
    break;

  case 395: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 396: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = new TypeDecl(Type::alias);
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType->alias = *(yyvsp[-4].s);
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 397: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 398: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 399: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 400: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 401: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 402: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 403: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 404: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 405: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 406: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 407: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 408: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 409: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 410: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 411: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 412: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 413: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 414: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 415: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 416: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 417: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 418: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 419: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 420: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 421: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 422: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 423: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 424: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 425: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 426: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 427: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 428: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 429: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 430: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 431: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 432: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 433: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 434: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 435: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 436: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 437: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 438: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 439: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 440: /* expr: '(' expr_list optional_comma ')'  */
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

  case 441: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 442: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 443: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 444: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 445: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 446: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 447: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 448: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 449: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 450: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 451: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 452: /* expr: expr_generator  */
                                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 453: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 454: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 455: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 456: /* $@31: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 457: /* expr: expr "is" "type" '<' $@30 type_declaration_no_options '>' $@31  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 458: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 459: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 460: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 461: /* $@32: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 462: /* $@33: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 463: /* expr: expr "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 464: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 465: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 466: /* $@34: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 467: /* $@35: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 468: /* expr: expr '?' "as" "type" '<' $@34 type_declaration '>' $@35  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 469: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 470: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 471: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 472: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 473: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 474: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 475: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 476: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 477: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 478: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 479: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 480: /* expr: expr_call_pipe  */
                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 481: /* expr: "unsafe" '(' expr ')'  */
                                         {
            (yyvsp[-1].pExpression)->alwaysSafe = true;
            (yyvsp[-1].pExpression)->userSaidItsSafe = true;
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    break;

  case 482: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 483: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 484: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list optional_emit_semis expression_block  */
                                                                                                                                                  {
        auto closure = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        ((ExprBlock *)(yyvsp[0].pExpression))->returnType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),closure,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 485: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 486: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 487: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 488: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 489: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 490: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 491: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 492: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 493: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 494: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 495: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 496: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 497: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 498: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 499: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 500: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 501: /* optional_field_annotation: %empty  */
                                      { (yyval.aaList) = nullptr; }
    break;

  case 502: /* optional_field_annotation: metadata_argument_list  */
                                      { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 503: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 504: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 505: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 506: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 507: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 508: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 509: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 510: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 511: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 512: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 513: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 514: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 515: /* struct_variable_declaration_list: struct_variable_declaration_list "new line, semicolon"  */
                                                                 { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 516: /* $@36: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 517: /* struct_variable_declaration_list: struct_variable_declaration_list $@36 structure_variable_declaration SEMICOLON  */
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

  case 518: /* $@37: %empty  */
                                                                                                                     {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 519: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable "abstract" optional_constant $@37 function_declaration_header SEMICOLON  */
                                                          {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 520: /* $@38: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 521: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@38 function_declaration_header optional_emit_semis expression_block  */
                                                                                            {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-10].pVarDeclList),(yyvsp[-9].faList),(yyvsp[-6].b),(yyvsp[-7].b),(yyvsp[-5].i),(yyvsp[-4].b),(yyvsp[-2].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-8]),(yylsp[0])),tokAt(scanner,(yylsp[-9])));
            }
    break;

  case 522: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 523: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 524: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 525: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 526: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 527: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 528: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 529: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 530: /* tuple_alias_type_list: %empty  */
      {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 531: /* tuple_alias_type_list: tuple_type  */
                       {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 532: /* tuple_alias_type_list: tuple_alias_type_list semis tuple_type  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tokName = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) {
                for ( const auto & nl : *((yyvsp[0].pVarDecl)->pNameList) ) {
                    crd->afterTupleEntry(nl.name.c_str(), nl.at);
                }
            }
        }
    }
    break;

  case 533: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 534: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 535: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 536: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 537: /* variant_alias_type_list: variant_type  */
                         {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 538: /* variant_alias_type_list: variant_alias_type_list semis variant_type  */
                                                               {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tokName = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) {
                for ( const auto & nl : *((yyvsp[0].pVarDecl)->pNameList) ) {
                    crd->afterVariantEntry(nl.name.c_str(), nl.at);
                }
            }
        }
    }
    break;

  case 539: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 540: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 541: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 542: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 543: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 544: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 545: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 546: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 547: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 548: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 549: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 550: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 551: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 552: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 553: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 554: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 555: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 556: /* global_let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 557: /* global_let_variable_name_with_pos_list: global_let_variable_name_with_pos_list ',' "name"  */
                                                                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 558: /* variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 559: /* variable_declaration_list: variable_declaration_list SEMICOLON  */
                                                  {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 560: /* variable_declaration_list: variable_declaration_list let_variable_declaration  */
                                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 561: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 562: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 563: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 564: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                         {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 565: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                               {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 566: /* global_let_variable_declaration: global_let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 567: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 568: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 569: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 570: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 571: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 572: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 573: /* global_variable_declaration_list: global_variable_declaration_list SEMICOLON  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 574: /* $@39: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 575: /* global_variable_declaration_list: global_variable_declaration_list $@39 optional_field_annotation let_variable_declaration  */
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

  case 576: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 577: /* $@40: %empty  */
                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 578: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@40 optional_field_annotation global_let_variable_declaration  */
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

  case 579: /* enum_expression: "name"  */
                   {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        delete (yyvsp[0].s);
    }
    break;

  case 580: /* enum_expression: "name" '=' expr  */
                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[-2].s),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-2])));
        delete (yyvsp[-2].s);
    }
    break;

  case 583: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 584: /* enum_list: enum_expression  */
                            {
        (yyval.pEnum) = new Enumeration();
        if ( !(yyval.pEnum)->add((yyvsp[0].pEnumPair)->name,(yyvsp[0].pEnumPair)->expr,(yyvsp[0].pEnumPair)->at) ) {
            das2_yyerror(scanner,"enumeration already declared " + (yyvsp[0].pEnumPair)->name, (yyvsp[0].pEnumPair)->at,
                CompilationError::enumeration_value_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            for ( auto & crd : yyextra->g_CommentReaders ) {
                crd->afterEnumerationEntry((yyvsp[0].pEnumPair)->name.c_str(), (yyvsp[0].pEnumPair)->at);
            }
        }
        delete (yyvsp[0].pEnumPair);
    }
    break;

  case 585: /* enum_list: enum_list commas enum_expression  */
                                                 {
        if ( !(yyvsp[-2].pEnum)->add((yyvsp[0].pEnumPair)->name,(yyvsp[0].pEnumPair)->expr,(yyvsp[0].pEnumPair)->at) ) {
            das2_yyerror(scanner,"enumeration already declared " + (yyvsp[0].pEnumPair)->name, (yyvsp[0].pEnumPair)->at,
                CompilationError::enumeration_value_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            for ( auto & crd : yyextra->g_CommentReaders ) {
                crd->afterEnumerationEntry((yyvsp[0].pEnumPair)->name.c_str(), (yyvsp[0].pEnumPair)->at);
            }
        }
        delete (yyvsp[0].pEnumPair);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
    break;

  case 586: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 587: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 588: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 589: /* $@41: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 590: /* single_alias: optional_public_or_private_alias "name" $@41 '=' type_declaration  */
                                  {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        (yyvsp[0].pTypeDecl)->isPrivateAlias = !(yyvsp[-4].b);
        if ( (yyvsp[0].pTypeDecl)->baseType == Type::alias ) {
            das2_yyerror(scanner,"alias cannot be defined in terms of another alias "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::invalid_type);
        }
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !yyextra->g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterAlias((yyvsp[-3].s)->c_str(),pubename);
        }
        delete (yyvsp[-3].s);
    }
    break;

  case 592: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 593: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 594: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 595: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 596: /* optional_enum_basic_type_declaration: %empty  */
        {
        (yyval.type) = Type::tInt;
    }
    break;

  case 597: /* optional_enum_basic_type_declaration: ':' enum_basic_type_declaration  */
                                              {
        (yyval.type) = (yyvsp[0].type);
    }
    break;

  case 604: /* $@42: %empty  */
                                                     {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 605: /* $@43: %empty  */
                                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 606: /* $@44: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 607: /* enum_declaration: optional_annotation_list "enum" $@42 optional_public_or_private_enum enum_name optional_enum_basic_type_declaration optional_emit_commas '{' $@43 enum_list optional_commas $@44 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-8].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-12].faList),tokAt(scanner,(yylsp[-12])),(yyvsp[-9].b),(yyvsp[-8].pEnum),(yyvsp[-3].pEnum),(yyvsp[-7].type));
    }
    break;

  case 608: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 609: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 610: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 611: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 612: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 613: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 614: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 615: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 616: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 617: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 618: /* optional_struct_variable_declaration_list: ';'  */
            {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 619: /* optional_struct_variable_declaration_list: '{' struct_variable_declaration_list '}'  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 620: /* $@45: %empty  */
                                                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 621: /* $@46: %empty  */
                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 622: /* $@47: %empty  */
                                             { if ( (yyvsp[-1].pStructure) ) { (yyvsp[-1].pStructure)->isClass = (yyvsp[-4].b); (yyvsp[-1].pStructure)->privateStructure = !(yyvsp[-3].b); } }
    break;

  case 623: /* structure_declaration: optional_annotation_list_with_emit_semis $@45 class_or_struct optional_public_or_private_structure $@46 structure_name optional_emit_semis $@47 optional_struct_variable_declaration_list  */
                                                                                                                                                                        {
        yyextra->pop_nesteds();
        if ( (yyvsp[-3].pStructure) ) {
            ast_structureDeclaration ( scanner, (yyvsp[-8].faList), tokAt(scanner,(yylsp[-6])), (yyvsp[-3].pStructure), tokAt(scanner,(yylsp[-3])), (yyvsp[0].pVarDeclList) );
            if ( !yyextra->g_CommentReaders.empty() ) {
                auto tak = tokAt(scanner,(yylsp[-6]));
                for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructure((yyvsp[-3].pStructure),tak);
            }
        } else {
            deleteVariableDeclarationList((yyvsp[0].pVarDeclList));
        }
    }
    break;

  case 624: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 625: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 626: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 627: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 628: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 629: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 630: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 631: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 632: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 633: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 634: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 635: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 636: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 637: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 638: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 639: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 640: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 641: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 642: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 643: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 644: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 645: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 646: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 647: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 648: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 649: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 650: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 651: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 652: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 653: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 654: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 655: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 656: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 657: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 658: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 659: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 660: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 661: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 662: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 663: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 664: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 665: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 666: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 667: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 668: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 669: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 670: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 671: /* bitfield_alias_bits: "name"  */
                   {
        (yyval.pNameList) = new vector<string>();
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[0].s)->c_str(),atvname);
        }
        delete (yyvsp[0].s);
    }
    break;

  case 672: /* bitfield_alias_bits: bitfield_alias_bits commas "name"  */
                                                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[0].s)->c_str(),atvname);
        }
        delete (yyvsp[0].s);
    }
    break;

  case 673: /* $@48: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 674: /* $@49: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 675: /* bitfield_type_declaration: "bitfield" '<' $@48 bitfield_bits '>' $@49  */
                                                                                                                             {
            (yyval.pTypeDecl) = new TypeDecl(Type::tBitfield);
            (yyval.pTypeDecl)->argNames = *(yyvsp[-2].pNameList);
            if ( (yyval.pTypeDecl)->argNames.size()>32 ) {
                das2_yyerror(scanner,"only 32 different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-5])),
                    CompilationError::invalid_type);
            }
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
            delete (yyvsp[-2].pNameList);
    }
    break;

  case 678: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 679: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 680: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 681: /* dim_list: '[' ']'  */
                {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 682: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 683: /* dim_list: dim_list '[' ']'  */
                              {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 684: /* type_declaration_no_options: type_declaration_no_options_no_dim  */
                                                     {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 685: /* type_declaration_no_options: type_declaration_no_options_no_dim dim_list  */
                                                                       {
        if ( (yyvsp[-1].pTypeDecl)->baseType==Type::typeDecl ) {
            das2_yyerror(scanner,"type declaration can`t be used as array base type",tokAt(scanner,(yylsp[-1])),
                CompilationError::invalid_type);
        } else if ( (yyvsp[-1].pTypeDecl)->baseType==Type::typeMacro ) {
            das2_yyerror(scanner,"macro can`t be used as array base type",tokAt(scanner,(yylsp[-1])),
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

  case 686: /* type_declaration_no_options_no_dim: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 687: /* type_declaration_no_options_no_dim: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 688: /* type_declaration_no_options_no_dim: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 689: /* type_declaration_no_options_no_dim: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 690: /* $@50: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 691: /* $@51: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 692: /* type_declaration_no_options_no_dim: "type" '<' $@50 type_declaration '>' $@51  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 693: /* type_declaration_no_options_no_dim: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 694: /* type_declaration_no_options_no_dim: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 695: /* $@52: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 696: /* type_declaration_no_options_no_dim: '$' name_in_namespace '<' $@52 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 697: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' '[' ']'  */
                                                                 {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 698: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "explicit"  */
                                                                  {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 699: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "const"  */
                                                               {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 700: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' "const"  */
                                                                   {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 701: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '&'  */
                                                         {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 702: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' '&'  */
                                                             {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 703: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '#'  */
                                                         {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 704: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "implicit"  */
                                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 705: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' '#'  */
                                                             {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 706: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "==" "const"  */
                                                                      {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 707: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "==" '&'  */
                                                                {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 708: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '?'  */
                                                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 709: /* $@53: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 710: /* $@54: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 711: /* type_declaration_no_options_no_dim: "smart_ptr" '<' $@53 type_declaration '>' $@54  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 712: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "??"  */
                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 713: /* $@55: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 714: /* $@56: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 715: /* type_declaration_no_options_no_dim: "array" '<' $@55 type_declaration '>' $@56  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 716: /* $@57: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 717: /* $@58: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 718: /* type_declaration_no_options_no_dim: "table" '<' $@57 table_type_pair '>' $@58  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 719: /* $@59: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 720: /* $@60: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 721: /* type_declaration_no_options_no_dim: "iterator" '<' $@59 type_declaration '>' $@60  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 722: /* type_declaration_no_options_no_dim: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 723: /* $@61: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 724: /* $@62: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 725: /* type_declaration_no_options_no_dim: "block" '<' $@61 type_declaration '>' $@62  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 726: /* $@63: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 727: /* $@64: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 728: /* type_declaration_no_options_no_dim: "block" '<' $@63 optional_function_argument_list optional_function_type '>' $@64  */
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

  case 729: /* type_declaration_no_options_no_dim: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 730: /* $@65: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 731: /* $@66: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 732: /* type_declaration_no_options_no_dim: "function" '<' $@65 type_declaration '>' $@66  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 733: /* $@67: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 734: /* $@68: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 735: /* type_declaration_no_options_no_dim: "function" '<' $@67 optional_function_argument_list optional_function_type '>' $@68  */
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

  case 736: /* type_declaration_no_options_no_dim: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 737: /* $@69: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 738: /* $@70: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 739: /* type_declaration_no_options_no_dim: "lambda" '<' $@69 type_declaration '>' $@70  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 740: /* $@71: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 741: /* $@72: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 742: /* type_declaration_no_options_no_dim: "lambda" '<' $@71 optional_function_argument_list optional_function_type '>' $@72  */
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

  case 743: /* $@73: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 744: /* $@74: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 745: /* type_declaration_no_options_no_dim: "tuple" '<' $@73 tuple_type_list '>' $@74  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 746: /* $@75: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 747: /* $@76: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 748: /* type_declaration_no_options_no_dim: "variant" '<' $@75 variant_type_list '>' $@76  */
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

  case 752: /* $@77: %empty  */
                   {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 753: /* $@78: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 754: /* $@79: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 755: /* $@80: %empty  */
                                                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 756: /* tuple_alias_declaration: "tuple" $@77 optional_public_or_private_alias "name" optional_emit_semis $@78 '{' $@79 tuple_alias_type_list optional_semis $@80 '}'  */
          {
        auto vtype = make_smart<TypeDecl>(Type::tTuple);
        vtype->alias = *(yyvsp[-8].s);
        vtype->at = tokAt(scanner,(yylsp[-8]));
        vtype->isPrivateAlias = !(yyvsp[-9].b);
        varDeclToTypeDecl(scanner, vtype.get(), (yyvsp[-3].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-8].s),tokAt(scanner,(yylsp[-8])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-8]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTuple((yyvsp[-8].s)->c_str(),atvname);
        }
        delete (yyvsp[-8].s);
    }
    break;

  case 757: /* $@81: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 758: /* $@82: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 759: /* $@83: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 760: /* $@84: %empty  */
                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 761: /* variant_alias_declaration: "variant" $@81 optional_public_or_private_alias "name" optional_emit_semis $@82 '{' $@83 variant_alias_type_list optional_semis $@84 '}'  */
          {
        auto vtype = make_smart<TypeDecl>(Type::tVariant);
        vtype->alias = *(yyvsp[-8].s);
        vtype->at = tokAt(scanner,(yylsp[-8]));
        vtype->isPrivateAlias = !(yyvsp[-9].b);
        varDeclToTypeDecl(scanner, vtype.get(), (yyvsp[-3].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-8].s),tokAt(scanner,(yylsp[-8])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-8]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariant((yyvsp[-8].s)->c_str(),atvname);
        }
        delete (yyvsp[-8].s);
    }
    break;

  case 762: /* $@85: %empty  */
                      {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 763: /* $@86: %empty  */
                                                                              {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 764: /* $@87: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 765: /* $@88: %empty  */
                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 766: /* bitfield_alias_declaration: "bitfield" $@85 optional_public_or_private_alias "name" optional_emit_commas $@86 '{' $@87 bitfield_alias_bits optional_commas $@88 '}'  */
          {
        auto btype = make_smart<TypeDecl>(Type::tBitfield);
        btype->alias = *(yyvsp[-8].s);
        btype->at = tokAt(scanner,(yylsp[-8]));
        btype->argNames = *(yyvsp[-3].pNameList);
        btype->isPrivateAlias = !(yyvsp[-9].b);
        if ( btype->argNames.size()>32 ) {
            das2_yyerror(scanner,"only 32 different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-8])),
                CompilationError::invalid_type);
        }
        if ( !yyextra->g_Program->addAlias(btype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-8].s),tokAt(scanner,(yylsp[-8])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-8]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfield((yyvsp[-8].s)->c_str(),atvname);
        }
        delete (yyvsp[-8].s);
        delete (yyvsp[-3].pNameList);
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

  case 780: /* make_variant_dim: make_struct_fields  */
                                {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 781: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 782: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 783: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 784: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 785: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 786: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 787: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 788: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 789: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 790: /* $@89: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 791: /* $@90: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 792: /* make_struct_decl: "struct" '<' $@89 type_declaration_no_options '>' $@90 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 793: /* $@91: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 794: /* $@92: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 795: /* make_struct_decl: "class" '<' $@91 type_declaration_no_options '>' $@92 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 796: /* $@93: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 797: /* $@94: %empty  */
                                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 798: /* make_struct_decl: "variant" '<' $@93 type_declaration_no_options '>' $@94 '(' make_variant_dim ')'  */
                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 799: /* $@95: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 800: /* $@96: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 801: /* make_struct_decl: "default" '<' $@95 type_declaration_no_options '>' $@96 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 802: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 803: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 804: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 805: /* $@97: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 806: /* $@98: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 807: /* make_tuple_call: "tuple" '<' $@97 type_declaration_no_options '>' $@98 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 808: /* make_dim_decl: '[' expr_list optional_comma ']'  */
                                                          {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 809: /* $@99: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 810: /* $@100: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 811: /* make_dim_decl: "array" "struct" '<' $@99 type_declaration_no_options '>' $@100 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 812: /* $@101: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 813: /* $@102: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 814: /* make_dim_decl: "array" "tuple" '<' $@101 type_declaration_no_options '>' $@102 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 815: /* $@103: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 816: /* $@104: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 817: /* make_dim_decl: "array" "variant" '<' $@103 type_declaration_no_options '>' $@104 '(' make_variant_dim ')'  */
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

  case 818: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 819: /* $@105: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 820: /* $@106: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 821: /* make_dim_decl: "array" '<' $@105 type_declaration_no_options '>' $@106 '(' optional_expr_list ')'  */
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

  case 822: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 823: /* $@107: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 824: /* $@108: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 825: /* make_dim_decl: "fixed_array" '<' $@107 type_declaration_no_options '>' $@108 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 826: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 827: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 828: /* make_table_decl: '{' expr_map_tuple_list optional_comma '}'  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 829: /* make_table_decl: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 830: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 831: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 832: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 833: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 834: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 835: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 836: /* array_comprehension: '[' "for" '(' variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                           {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 837: /* array_comprehension: '[' "iterator" "for" '(' variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                                        {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 838: /* array_comprehension: '{' "for" '(' variable_name_with_pos_list "in" expr_list ')' ';' make_map_tuple array_comprehension_where '}'  */
                                                                                                                                                                     {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
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
  yytoken = yychar == DAS2_YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
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

      if (yychar <= DAS2_YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == DAS2_YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, scanner);
          yychar = DAS2_YYEMPTY;
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
  if (yychar != DAS2_YYEMPTY)
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



void das2_yyfatalerror ( DAS2_YYLTYPE * lloc, yyscan_t scanner, const string & error, CompilationError cerr ) {
    yyextra->g_Program->error(error,"","",LineInfo(yyextra->g_FileAccessStack.back(),
        lloc->first_column,lloc->first_line,lloc->last_column,lloc->last_line),cerr);
}

void das2_yyerror ( DAS2_YYLTYPE * lloc, yyscan_t scanner, const string & error ) {
    if ( !yyextra->das_suppress_errors ) {
        yyextra->g_Program->error(error,"","",LineInfo(yyextra->g_FileAccessStack.back(),
            lloc->first_column,lloc->first_line,lloc->last_column,lloc->last_line),
                CompilationError::syntax_error);
    }
}

LineInfo tokAt ( yyscan_t scanner, const struct DAS2_YYLTYPE & li ) {
    return LineInfo(yyextra->g_FileAccessStack.back(),
        li.first_column,li.first_line,
        li.last_column,li.last_line);
}

LineInfo tokRangeAt ( yyscan_t scanner, const struct DAS2_YYLTYPE & li, const struct DAS2_YYLTYPE & lie ) {
    return LineInfo(yyextra->g_FileAccessStack.back(),
        li.first_column,li.first_line,
        lie.last_column,lie.last_line);
}


