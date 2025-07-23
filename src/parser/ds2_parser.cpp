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
    void das2_strfmt ( yyscan_t yyscanner );

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
  YYSYMBOL_INTEGER = 156,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 157,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 158,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 159,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 160,            /* "unsigned int8 constant"  */
  YYSYMBOL_DAS_FLOAT = 161,                /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 162,                   /* "double constant"  */
  YYSYMBOL_NAME = 163,                     /* "name"  */
  YYSYMBOL_DAS_EMIT_COMMA = 164,           /* "new line, comma"  */
  YYSYMBOL_DAS_EMIT_SEMICOLON = 165,       /* "new line, semicolon"  */
  YYSYMBOL_BEGIN_STRING = 166,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 167,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 168,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 169,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 170,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 171,          /* "}"  */
  YYSYMBOL_END_OF_READ = 172,              /* "end of failed eader macro"  */
  YYSYMBOL_173_ = 173,                     /* ','  */
  YYSYMBOL_174_ = 174,                     /* '='  */
  YYSYMBOL_175_ = 175,                     /* '?'  */
  YYSYMBOL_176_ = 176,                     /* ':'  */
  YYSYMBOL_177_ = 177,                     /* '|'  */
  YYSYMBOL_178_ = 178,                     /* '^'  */
  YYSYMBOL_179_ = 179,                     /* '&'  */
  YYSYMBOL_180_ = 180,                     /* '<'  */
  YYSYMBOL_181_ = 181,                     /* '>'  */
  YYSYMBOL_182_ = 182,                     /* '-'  */
  YYSYMBOL_183_ = 183,                     /* '+'  */
  YYSYMBOL_184_ = 184,                     /* '*'  */
  YYSYMBOL_185_ = 185,                     /* '/'  */
  YYSYMBOL_186_ = 186,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 187,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 188,               /* UNARY_PLUS  */
  YYSYMBOL_189_ = 189,                     /* '~'  */
  YYSYMBOL_190_ = 190,                     /* '!'  */
  YYSYMBOL_PRE_INC = 191,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 192,                  /* PRE_DEC  */
  YYSYMBOL_LLPIPE = 193,                   /* LLPIPE  */
  YYSYMBOL_POST_INC = 194,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 195,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 196,                    /* DEREF  */
  YYSYMBOL_197_ = 197,                     /* '.'  */
  YYSYMBOL_198_ = 198,                     /* '['  */
  YYSYMBOL_199_ = 199,                     /* ']'  */
  YYSYMBOL_200_ = 200,                     /* '('  */
  YYSYMBOL_201_ = 201,                     /* ')'  */
  YYSYMBOL_202_ = 202,                     /* '$'  */
  YYSYMBOL_203_ = 203,                     /* '@'  */
  YYSYMBOL_204_ = 204,                     /* ';'  */
  YYSYMBOL_205_ = 205,                     /* '{'  */
  YYSYMBOL_206_ = 206,                     /* '}'  */
  YYSYMBOL_207_ = 207,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 208,                 /* $accept  */
  YYSYMBOL_program = 209,                  /* program  */
  YYSYMBOL_COMMA = 210,                    /* COMMA  */
  YYSYMBOL_SEMICOLON = 211,                /* SEMICOLON  */
  YYSYMBOL_top_level_reader_macro = 212,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 213, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 214,              /* module_name  */
  YYSYMBOL_module_declaration = 215,       /* module_declaration  */
  YYSYMBOL_character_sequence = 216,       /* character_sequence  */
  YYSYMBOL_string_constant = 217,          /* string_constant  */
  YYSYMBOL_format_string = 218,            /* format_string  */
  YYSYMBOL_optional_format_string = 219,   /* optional_format_string  */
  YYSYMBOL_220_1 = 220,                    /* $@1  */
  YYSYMBOL_string_builder_body = 221,      /* string_builder_body  */
  YYSYMBOL_string_builder = 222,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 223, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 224,              /* expr_reader  */
  YYSYMBOL_225_2 = 225,                    /* $@2  */
  YYSYMBOL_options_declaration = 226,      /* options_declaration  */
  YYSYMBOL_require_declaration = 227,      /* require_declaration  */
  YYSYMBOL_require_module_name = 228,      /* require_module_name  */
  YYSYMBOL_require_module = 229,           /* require_module  */
  YYSYMBOL_is_public_module = 230,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 231,       /* expect_declaration  */
  YYSYMBOL_expect_list = 232,              /* expect_list  */
  YYSYMBOL_expect_error = 233,             /* expect_error  */
  YYSYMBOL_expression_label = 234,         /* expression_label  */
  YYSYMBOL_expression_goto = 235,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 236,      /* elif_or_static_elif  */
  YYSYMBOL_emit_semis = 237,               /* emit_semis  */
  YYSYMBOL_optional_emit_semis = 238,      /* optional_emit_semis  */
  YYSYMBOL_expression_else = 239,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 240,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 241, /* expression_else_one_liner  */
  YYSYMBOL_expression_if_one_liner = 242,  /* expression_if_one_liner  */
  YYSYMBOL_semis = 243,                    /* semis  */
  YYSYMBOL_optional_semis = 244,           /* optional_semis  */
  YYSYMBOL_expression_if_block = 245,      /* expression_if_block  */
  YYSYMBOL_246_3 = 246,                    /* $@3  */
  YYSYMBOL_247_4 = 247,                    /* $@4  */
  YYSYMBOL_248_5 = 248,                    /* $@5  */
  YYSYMBOL_expression_else_block = 249,    /* expression_else_block  */
  YYSYMBOL_250_6 = 250,                    /* $@6  */
  YYSYMBOL_251_7 = 251,                    /* $@7  */
  YYSYMBOL_252_8 = 252,                    /* $@8  */
  YYSYMBOL_expression_if_then_else = 253,  /* expression_if_then_else  */
  YYSYMBOL_254_9 = 254,                    /* $@9  */
  YYSYMBOL_expression_if_then_else_oneliner = 255, /* expression_if_then_else_oneliner  */
  YYSYMBOL_for_variable_name_with_pos_list = 256, /* for_variable_name_with_pos_list  */
  YYSYMBOL_expression_for_loop = 257,      /* expression_for_loop  */
  YYSYMBOL_258_10 = 258,                   /* $@10  */
  YYSYMBOL_expression_unsafe = 259,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 260,    /* expression_while_loop  */
  YYSYMBOL_261_11 = 261,                   /* $@11  */
  YYSYMBOL_expression_with = 262,          /* expression_with  */
  YYSYMBOL_263_12 = 263,                   /* $@12  */
  YYSYMBOL_expression_with_alias = 264,    /* expression_with_alias  */
  YYSYMBOL_annotation_argument_value = 265, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 266, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 267, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 268,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 269, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 270,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 271, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 272, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 273,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 274,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 275, /* optional_annotation_list  */
  YYSYMBOL_optional_annotation_list_with_emit_semis = 276, /* optional_annotation_list_with_emit_semis  */
  YYSYMBOL_optional_function_argument_list = 277, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 278,   /* optional_function_type  */
  YYSYMBOL_function_name = 279,            /* function_name  */
  YYSYMBOL_global_function_declaration = 280, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 281, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 282, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 283,     /* function_declaration  */
  YYSYMBOL_284_13 = 284,                   /* $@13  */
  YYSYMBOL_expression_block_finally = 285, /* expression_block_finally  */
  YYSYMBOL_286_14 = 286,                   /* $@14  */
  YYSYMBOL_287_15 = 287,                   /* $@15  */
  YYSYMBOL_expression_block = 288,         /* expression_block  */
  YYSYMBOL_289_16 = 289,                   /* $@16  */
  YYSYMBOL_290_17 = 290,                   /* $@17  */
  YYSYMBOL_expr_call_pipe = 291,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 292,           /* expression_any  */
  YYSYMBOL_expressions = 293,              /* expressions  */
  YYSYMBOL_optional_expr_list = 294,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_map_tuple_list = 295, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 296, /* type_declaration_no_options_list  */
  YYSYMBOL_name_in_namespace = 297,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 298,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 299,     /* new_type_declaration  */
  YYSYMBOL_300_18 = 300,                   /* $@18  */
  YYSYMBOL_301_19 = 301,                   /* $@19  */
  YYSYMBOL_expr_new = 302,                 /* expr_new  */
  YYSYMBOL_expression_break = 303,         /* expression_break  */
  YYSYMBOL_expression_continue = 304,      /* expression_continue  */
  YYSYMBOL_expression_return = 305,        /* expression_return  */
  YYSYMBOL_expression_yield = 306,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 307,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 308,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 309,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 310,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 311,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 312, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 313,           /* expression_let  */
  YYSYMBOL_expr_cast = 314,                /* expr_cast  */
  YYSYMBOL_315_20 = 315,                   /* $@20  */
  YYSYMBOL_316_21 = 316,                   /* $@21  */
  YYSYMBOL_317_22 = 317,                   /* $@22  */
  YYSYMBOL_318_23 = 318,                   /* $@23  */
  YYSYMBOL_319_24 = 319,                   /* $@24  */
  YYSYMBOL_320_25 = 320,                   /* $@25  */
  YYSYMBOL_expr_type_decl = 321,           /* expr_type_decl  */
  YYSYMBOL_322_26 = 322,                   /* $@26  */
  YYSYMBOL_323_27 = 323,                   /* $@27  */
  YYSYMBOL_expr_type_info = 324,           /* expr_type_info  */
  YYSYMBOL_expr_list = 325,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 326,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 327,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 328,            /* capture_entry  */
  YYSYMBOL_capture_list = 329,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 330,    /* optional_capture_list  */
  YYSYMBOL_expr_full_block = 331,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 332, /* expr_full_block_assumed_piped  */
  YYSYMBOL_expr_numeric_const = 333,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 334,              /* expr_assign  */
  YYSYMBOL_expr_named_call = 335,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 336,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 337,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 338,           /* func_addr_expr  */
  YYSYMBOL_339_28 = 339,                   /* $@28  */
  YYSYMBOL_340_29 = 340,                   /* $@29  */
  YYSYMBOL_341_30 = 341,                   /* $@30  */
  YYSYMBOL_342_31 = 342,                   /* $@31  */
  YYSYMBOL_expr_field = 343,               /* expr_field  */
  YYSYMBOL_344_32 = 344,                   /* $@32  */
  YYSYMBOL_345_33 = 345,                   /* $@33  */
  YYSYMBOL_expr_call = 346,                /* expr_call  */
  YYSYMBOL_expr = 347,                     /* expr  */
  YYSYMBOL_348_34 = 348,                   /* $@34  */
  YYSYMBOL_349_35 = 349,                   /* $@35  */
  YYSYMBOL_350_36 = 350,                   /* $@36  */
  YYSYMBOL_351_37 = 351,                   /* $@37  */
  YYSYMBOL_352_38 = 352,                   /* $@38  */
  YYSYMBOL_353_39 = 353,                   /* $@39  */
  YYSYMBOL_expr_generator = 354,           /* expr_generator  */
  YYSYMBOL_expr_mtag = 355,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 356, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 357,        /* optional_override  */
  YYSYMBOL_optional_constant = 358,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 359, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 360, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 361, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 362, /* struct_variable_declaration_list  */
  YYSYMBOL_363_40 = 363,                   /* $@40  */
  YYSYMBOL_364_41 = 364,                   /* $@41  */
  YYSYMBOL_365_42 = 365,                   /* $@42  */
  YYSYMBOL_function_argument_declaration_no_type = 366, /* function_argument_declaration_no_type  */
  YYSYMBOL_function_argument_declaration_type = 367, /* function_argument_declaration_type  */
  YYSYMBOL_function_argument_list = 368,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 369,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 370,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 371,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 372,             /* variant_type  */
  YYSYMBOL_variant_type_list = 373,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 374,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 375,             /* copy_or_move  */
  YYSYMBOL_variable_declaration_no_type = 376, /* variable_declaration_no_type  */
  YYSYMBOL_variable_declaration_type = 377, /* variable_declaration_type  */
  YYSYMBOL_variable_declaration = 378,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 379,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 380,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 381, /* let_variable_name_with_pos_list  */
  YYSYMBOL_global_let_variable_name_with_pos_list = 382, /* global_let_variable_name_with_pos_list  */
  YYSYMBOL_variable_declaration_list = 383, /* variable_declaration_list  */
  YYSYMBOL_let_variable_declaration = 384, /* let_variable_declaration  */
  YYSYMBOL_global_let_variable_declaration = 385, /* global_let_variable_declaration  */
  YYSYMBOL_optional_shared = 386,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 387, /* optional_public_or_private_variable  */
  YYSYMBOL_global_variable_declaration_list = 388, /* global_variable_declaration_list  */
  YYSYMBOL_389_43 = 389,                   /* $@43  */
  YYSYMBOL_global_let = 390,               /* global_let  */
  YYSYMBOL_391_44 = 391,                   /* $@44  */
  YYSYMBOL_enum_expression = 392,          /* enum_expression  */
  YYSYMBOL_commas = 393,                   /* commas  */
  YYSYMBOL_enum_list = 394,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 395, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 396,             /* single_alias  */
  YYSYMBOL_397_45 = 397,                   /* $@45  */
  YYSYMBOL_alias_declaration = 398,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 399, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 400,                /* enum_name  */
  YYSYMBOL_optional_enum_basic_type_declaration = 401, /* optional_enum_basic_type_declaration  */
  YYSYMBOL_optional_commas = 402,          /* optional_commas  */
  YYSYMBOL_emit_commas = 403,              /* emit_commas  */
  YYSYMBOL_optional_emit_commas = 404,     /* optional_emit_commas  */
  YYSYMBOL_enum_declaration = 405,         /* enum_declaration  */
  YYSYMBOL_406_46 = 406,                   /* $@46  */
  YYSYMBOL_407_47 = 407,                   /* $@47  */
  YYSYMBOL_408_48 = 408,                   /* $@48  */
  YYSYMBOL_optional_structure_parent = 409, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 410,          /* optional_sealed  */
  YYSYMBOL_structure_name = 411,           /* structure_name  */
  YYSYMBOL_class_or_struct = 412,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 413, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 414, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 415,    /* structure_declaration  */
  YYSYMBOL_416_49 = 416,                   /* $@49  */
  YYSYMBOL_417_50 = 417,                   /* $@50  */
  YYSYMBOL_418_51 = 418,                   /* $@51  */
  YYSYMBOL_variable_name_with_pos_list = 419, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 420,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 421, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 422, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 423,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 424,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 425,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 426, /* bitfield_type_declaration  */
  YYSYMBOL_427_52 = 427,                   /* $@52  */
  YYSYMBOL_428_53 = 428,                   /* $@53  */
  YYSYMBOL_c_or_s = 429,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 430,          /* table_type_pair  */
  YYSYMBOL_dim_list = 431,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 432, /* type_declaration_no_options  */
  YYSYMBOL_type_declaration_no_options_no_dim = 433, /* type_declaration_no_options_no_dim  */
  YYSYMBOL_434_54 = 434,                   /* $@54  */
  YYSYMBOL_435_55 = 435,                   /* $@55  */
  YYSYMBOL_436_56 = 436,                   /* $@56  */
  YYSYMBOL_437_57 = 437,                   /* $@57  */
  YYSYMBOL_438_58 = 438,                   /* $@58  */
  YYSYMBOL_439_59 = 439,                   /* $@59  */
  YYSYMBOL_440_60 = 440,                   /* $@60  */
  YYSYMBOL_441_61 = 441,                   /* $@61  */
  YYSYMBOL_442_62 = 442,                   /* $@62  */
  YYSYMBOL_443_63 = 443,                   /* $@63  */
  YYSYMBOL_444_64 = 444,                   /* $@64  */
  YYSYMBOL_445_65 = 445,                   /* $@65  */
  YYSYMBOL_446_66 = 446,                   /* $@66  */
  YYSYMBOL_447_67 = 447,                   /* $@67  */
  YYSYMBOL_448_68 = 448,                   /* $@68  */
  YYSYMBOL_449_69 = 449,                   /* $@69  */
  YYSYMBOL_450_70 = 450,                   /* $@70  */
  YYSYMBOL_451_71 = 451,                   /* $@71  */
  YYSYMBOL_452_72 = 452,                   /* $@72  */
  YYSYMBOL_453_73 = 453,                   /* $@73  */
  YYSYMBOL_454_74 = 454,                   /* $@74  */
  YYSYMBOL_455_75 = 455,                   /* $@75  */
  YYSYMBOL_456_76 = 456,                   /* $@76  */
  YYSYMBOL_457_77 = 457,                   /* $@77  */
  YYSYMBOL_458_78 = 458,                   /* $@78  */
  YYSYMBOL_459_79 = 459,                   /* $@79  */
  YYSYMBOL_460_80 = 460,                   /* $@80  */
  YYSYMBOL_type_declaration = 461,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 462,  /* tuple_alias_declaration  */
  YYSYMBOL_463_81 = 463,                   /* $@81  */
  YYSYMBOL_464_82 = 464,                   /* $@82  */
  YYSYMBOL_465_83 = 465,                   /* $@83  */
  YYSYMBOL_466_84 = 466,                   /* $@84  */
  YYSYMBOL_variant_alias_declaration = 467, /* variant_alias_declaration  */
  YYSYMBOL_468_85 = 468,                   /* $@85  */
  YYSYMBOL_469_86 = 469,                   /* $@86  */
  YYSYMBOL_470_87 = 470,                   /* $@87  */
  YYSYMBOL_471_88 = 471,                   /* $@88  */
  YYSYMBOL_bitfield_alias_declaration = 472, /* bitfield_alias_declaration  */
  YYSYMBOL_473_89 = 473,                   /* $@89  */
  YYSYMBOL_474_90 = 474,                   /* $@90  */
  YYSYMBOL_475_91 = 475,                   /* $@91  */
  YYSYMBOL_476_92 = 476,                   /* $@92  */
  YYSYMBOL_make_decl = 477,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 478,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 479,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 480,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim_list = 481,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 482,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 483, /* optional_make_struct_dim_decl  */
  YYSYMBOL_use_initializer = 484,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 485,         /* make_struct_decl  */
  YYSYMBOL_486_93 = 486,                   /* $@93  */
  YYSYMBOL_487_94 = 487,                   /* $@94  */
  YYSYMBOL_488_95 = 488,                   /* $@95  */
  YYSYMBOL_489_96 = 489,                   /* $@96  */
  YYSYMBOL_490_97 = 490,                   /* $@97  */
  YYSYMBOL_491_98 = 491,                   /* $@98  */
  YYSYMBOL_492_99 = 492,                   /* $@99  */
  YYSYMBOL_493_100 = 493,                  /* $@100  */
  YYSYMBOL_make_map_tuple = 494,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 495,          /* make_tuple_call  */
  YYSYMBOL_496_101 = 496,                  /* $@101  */
  YYSYMBOL_497_102 = 497,                  /* $@102  */
  YYSYMBOL_make_dim_decl = 498,            /* make_dim_decl  */
  YYSYMBOL_499_103 = 499,                  /* $@103  */
  YYSYMBOL_500_104 = 500,                  /* $@104  */
  YYSYMBOL_501_105 = 501,                  /* $@105  */
  YYSYMBOL_502_106 = 502,                  /* $@106  */
  YYSYMBOL_503_107 = 503,                  /* $@107  */
  YYSYMBOL_504_108 = 504,                  /* $@108  */
  YYSYMBOL_505_109 = 505,                  /* $@109  */
  YYSYMBOL_506_110 = 506,                  /* $@110  */
  YYSYMBOL_507_111 = 507,                  /* $@111  */
  YYSYMBOL_508_112 = 508,                  /* $@112  */
  YYSYMBOL_expr_map_tuple_list = 509,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 510,          /* make_table_decl  */
  YYSYMBOL_511_113 = 511,                  /* $@113  */
  YYSYMBOL_array_comprehension_where = 512, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 513,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 514       /* array_comprehension  */
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
#define YYLAST   11977

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  208
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  307
/* YYNRULES -- Number of rules.  */
#define YYNRULES  878
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1586

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   435


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
       2,     2,     2,   190,     2,   207,   202,   186,   179,     2,
     200,   201,   184,   183,   173,   182,   197,   185,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   176,   204,
     180,   174,   181,   175,   203,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   198,     2,   199,   178,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   205,   177,   206,   189,     2,     2,     2,
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
     165,   166,   167,   168,   169,   170,   171,   172,   187,   188,
     191,   192,   193,   194,   195,   196
};

#if DAS2_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   559,   559,   560,   565,   566,   567,   568,   569,   570,
     571,   572,   573,   574,   575,   576,   577,   581,   582,   586,
     587,   591,   597,   598,   599,   603,   604,   608,   626,   627,
     628,   629,   633,   634,   638,   639,   643,   644,   644,   648,
     653,   662,   677,   693,   698,   706,   706,   745,   763,   767,
     770,   774,   780,   789,   792,   798,   799,   803,   807,   808,
     812,   815,   821,   827,   830,   836,   837,   841,   842,   846,
     847,   851,   852,   853,   861,   862,   866,   867,   873,   874,
     875,   876,   877,   881,   882,   886,   887,   891,   893,   891,
     905,   905,   913,   915,   913,   927,   927,   935,   935,   947,
     954,   961,   966,   975,   983,   989,   997,  1007,  1007,  1016,
    1024,  1024,  1037,  1037,  1049,  1056,  1057,  1058,  1059,  1060,
    1061,  1065,  1070,  1078,  1079,  1080,  1084,  1085,  1086,  1087,
    1088,  1089,  1090,  1091,  1097,  1100,  1106,  1109,  1115,  1116,
    1117,  1118,  1122,  1135,  1153,  1156,  1164,  1175,  1186,  1197,
    1200,  1207,  1211,  1218,  1219,  1223,  1224,  1228,  1229,  1230,
    1234,  1237,  1244,  1248,  1249,  1250,  1251,  1252,  1253,  1254,
    1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,  1264,
    1265,  1266,  1267,  1268,  1269,  1270,  1271,  1272,  1273,  1274,
    1275,  1276,  1277,  1278,  1279,  1280,  1281,  1282,  1283,  1284,
    1285,  1286,  1287,  1288,  1289,  1290,  1291,  1292,  1293,  1294,
    1295,  1296,  1297,  1298,  1299,  1300,  1301,  1302,  1303,  1304,
    1305,  1306,  1307,  1308,  1309,  1310,  1311,  1312,  1313,  1314,
    1315,  1316,  1317,  1318,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,  1326,  1327,  1328,  1329,  1330,  1331,  1332,  1333,  1334,
    1335,  1336,  1337,  1338,  1339,  1340,  1341,  1342,  1347,  1365,
    1366,  1367,  1371,  1377,  1377,  1394,  1397,  1399,  1397,  1407,
    1409,  1407,  1424,  1437,  1438,  1439,  1440,  1441,  1442,  1443,
    1444,  1445,  1446,  1447,  1448,  1449,  1450,  1451,  1452,  1453,
    1454,  1455,  1459,  1464,  1470,  1476,  1477,  1481,  1482,  1486,
    1490,  1497,  1498,  1509,  1513,  1516,  1524,  1524,  1524,  1527,
    1533,  1536,  1540,  1544,  1551,  1558,  1564,  1568,  1572,  1575,
    1578,  1586,  1589,  1597,  1603,  1604,  1605,  1609,  1610,  1614,
    1615,  1619,  1624,  1632,  1638,  1650,  1653,  1656,  1662,  1662,
    1662,  1665,  1665,  1665,  1670,  1670,  1670,  1678,  1678,  1678,
    1684,  1694,  1705,  1720,  1723,  1729,  1730,  1737,  1748,  1749,
    1750,  1754,  1755,  1756,  1757,  1758,  1762,  1767,  1775,  1776,
    1780,  1787,  1791,  1797,  1798,  1799,  1800,  1801,  1802,  1803,
    1807,  1808,  1809,  1810,  1811,  1812,  1813,  1814,  1815,  1816,
    1817,  1818,  1819,  1820,  1821,  1822,  1823,  1824,  1825,  1829,
    1836,  1848,  1853,  1863,  1867,  1874,  1877,  1877,  1877,  1882,
    1882,  1882,  1895,  1899,  1903,  1908,  1915,  1924,  1929,  1936,
    1936,  1936,  1943,  1947,  1956,  1964,  1972,  1976,  1979,  1985,
    1986,  1987,  1988,  1989,  1990,  1991,  1992,  1993,  1994,  1995,
    1996,  1997,  1998,  1999,  2000,  2001,  2002,  2003,  2004,  2005,
    2006,  2007,  2008,  2009,  2010,  2011,  2012,  2013,  2014,  2015,
    2016,  2017,  2018,  2019,  2020,  2026,  2027,  2028,  2029,  2030,
    2043,  2052,  2053,  2054,  2055,  2056,  2057,  2058,  2059,  2060,
    2061,  2062,  2063,  2064,  2065,  2068,  2068,  2068,  2071,  2076,
    2080,  2084,  2084,  2084,  2089,  2092,  2096,  2096,  2096,  2101,
    2104,  2105,  2106,  2107,  2108,  2109,  2110,  2111,  2112,  2114,
    2118,  2119,  2127,  2130,  2133,  2142,  2143,  2144,  2145,  2146,
    2147,  2148,  2152,  2156,  2160,  2164,  2168,  2172,  2176,  2180,
    2184,  2191,  2192,  2196,  2197,  2198,  2202,  2203,  2207,  2208,
    2209,  2213,  2214,  2218,  2229,  2232,  2233,  2233,  2252,  2251,
    2266,  2265,  2282,  2294,  2303,  2313,  2314,  2315,  2316,  2317,
    2321,  2324,  2333,  2334,  2338,  2341,  2345,  2359,  2368,  2369,
    2373,  2376,  2380,  2394,  2395,  2399,  2405,  2411,  2420,  2423,
    2430,  2433,  2439,  2440,  2441,  2445,  2446,  2450,  2457,  2462,
    2471,  2477,  2488,  2495,  2504,  2507,  2510,  2517,  2520,  2525,
    2536,  2539,  2544,  2556,  2557,  2561,  2562,  2563,  2567,  2570,
    2573,  2573,  2593,  2596,  2596,  2614,  2619,  2627,  2628,  2632,
    2635,  2648,  2665,  2666,  2667,  2672,  2672,  2698,  2702,  2703,
    2704,  2708,  2718,  2721,  2727,  2728,  2732,  2733,  2737,  2738,
    2742,  2744,  2749,  2742,  2765,  2766,  2770,  2771,  2775,  2781,
    2782,  2783,  2784,  2788,  2789,  2790,  2794,  2797,  2803,  2805,
    2810,  2803,  2831,  2838,  2843,  2852,  2858,  2869,  2870,  2871,
    2872,  2873,  2874,  2875,  2876,  2877,  2878,  2879,  2880,  2881,
    2882,  2883,  2884,  2885,  2886,  2887,  2888,  2889,  2890,  2891,
    2892,  2893,  2894,  2895,  2899,  2900,  2901,  2902,  2903,  2904,
    2905,  2906,  2910,  2921,  2925,  2932,  2944,  2951,  2960,  2965,
    2975,  2988,  2988,  2988,  3001,  3002,  3006,  3010,  3017,  3021,
    3025,  3029,  3036,  3039,  3057,  3058,  3059,  3060,  3061,  3061,
    3061,  3065,  3070,  3077,  3077,  3084,  3088,  3092,  3097,  3102,
    3107,  3112,  3116,  3120,  3125,  3129,  3133,  3138,  3138,  3138,
    3144,  3151,  3151,  3151,  3156,  3156,  3156,  3162,  3162,  3162,
    3167,  3171,  3171,  3171,  3176,  3176,  3176,  3185,  3189,  3189,
    3189,  3194,  3194,  3194,  3203,  3207,  3207,  3207,  3212,  3212,
    3212,  3221,  3221,  3221,  3227,  3227,  3227,  3236,  3239,  3250,
    3266,  3268,  3273,  3278,  3266,  3304,  3306,  3311,  3317,  3304,
    3343,  3345,  3350,  3355,  3343,  3386,  3387,  3388,  3389,  3390,
    3394,  3401,  3408,  3414,  3420,  3427,  3434,  3440,  3449,  3452,
    3458,  3466,  3471,  3478,  3483,  3489,  3490,  3494,  3495,  3499,
    3499,  3499,  3507,  3507,  3507,  3514,  3514,  3514,  3525,  3525,
    3525,  3536,  3542,  3548,  3554,  3554,  3554,  3568,  3587,  3587,
    3587,  3597,  3597,  3597,  3611,  3611,  3611,  3625,  3634,  3634,
    3634,  3654,  3661,  3661,  3661,  3671,  3674,  3680,  3680,  3702,
    3710,  3730,  3755,  3756,  3760,  3761,  3765,  3768,  3771
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
  "\"<-\"", "\"??\"", "\"?.\"", "\"?[\"", "\"<|\"", "\"|>\"", "\":=\"",
  "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"::\"",
  "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"", "\"||=\"", "\"^^=\"", "\"..\"",
  "\"$$\"", "\"$i\"", "\"$v\"", "\"$b\"", "\"$a\"", "\"$t\"", "\"$c\"",
  "\"$f\"", "\"...\"", "\"integer constant\"", "\"long integer constant\"",
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
  "character_sequence", "string_constant", "format_string",
  "optional_format_string", "$@1", "string_builder_body", "string_builder",
  "reader_character_sequence", "expr_reader", "$@2", "options_declaration",
  "require_declaration", "require_module_name", "require_module",
  "is_public_module", "expect_declaration", "expect_list", "expect_error",
  "expression_label", "expression_goto", "elif_or_static_elif",
  "emit_semis", "optional_emit_semis", "expression_else",
  "if_or_static_if", "expression_else_one_liner",
  "expression_if_one_liner", "semis", "optional_semis",
  "expression_if_block", "$@3", "$@4", "$@5", "expression_else_block",
  "$@6", "$@7", "$@8", "expression_if_then_else", "$@9",
  "expression_if_then_else_oneliner", "for_variable_name_with_pos_list",
  "expression_for_loop", "$@10", "expression_unsafe",
  "expression_while_loop", "$@11", "expression_with", "$@12",
  "expression_with_alias", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "metadata_argument_list", "annotation_declaration_name",
  "annotation_declaration_basic", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_annotation_list_with_emit_semis",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@13", "expression_block_finally", "$@14",
  "$@15", "expression_block", "$@16", "$@17", "expr_call_pipe",
  "expression_any", "expressions", "optional_expr_list",
  "optional_expr_map_tuple_list", "type_declaration_no_options_list",
  "name_in_namespace", "expression_delete", "new_type_declaration", "$@18",
  "$@19", "expr_new", "expression_break", "expression_continue",
  "expression_return", "expression_yield", "expression_try_catch",
  "kwd_let_var_or_nothing", "kwd_let", "optional_in_scope",
  "tuple_expansion", "tuple_expansion_variable_declaration",
  "expression_let", "expr_cast", "$@20", "$@21", "$@22", "$@23", "$@24",
  "$@25", "expr_type_decl", "$@26", "$@27", "expr_type_info", "expr_list",
  "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_full_block",
  "expr_full_block_assumed_piped", "expr_numeric_const", "expr_assign",
  "expr_named_call", "expr_method_call", "func_addr_name",
  "func_addr_expr", "$@28", "$@29", "$@30", "$@31", "expr_field", "$@32",
  "$@33", "expr_call", "expr", "$@34", "$@35", "$@36", "$@37", "$@38",
  "$@39", "expr_generator", "expr_mtag", "optional_field_annotation",
  "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@40", "$@41", "$@42",
  "function_argument_declaration_no_type",
  "function_argument_declaration_type", "function_argument_list",
  "tuple_type", "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
  "variable_declaration_no_type", "variable_declaration_type",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list",
  "global_let_variable_name_with_pos_list", "variable_declaration_list",
  "let_variable_declaration", "global_let_variable_declaration",
  "optional_shared", "optional_public_or_private_variable",
  "global_variable_declaration_list", "$@43", "global_let", "$@44",
  "enum_expression", "commas", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@45",
  "alias_declaration", "optional_public_or_private_enum", "enum_name",
  "optional_enum_basic_type_declaration", "optional_commas", "emit_commas",
  "optional_emit_commas", "enum_declaration", "$@46", "$@47", "$@48",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@49", "$@50", "$@51", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_alias_bits", "bitfield_type_declaration", "$@52", "$@53",
  "c_or_s", "table_type_pair", "dim_list", "type_declaration_no_options",
  "type_declaration_no_options_no_dim", "$@54", "$@55", "$@56", "$@57",
  "$@58", "$@59", "$@60", "$@61", "$@62", "$@63", "$@64", "$@65", "$@66",
  "$@67", "$@68", "$@69", "$@70", "$@71", "$@72", "$@73", "$@74", "$@75",
  "$@76", "$@77", "$@78", "$@79", "$@80", "type_declaration",
  "tuple_alias_declaration", "$@81", "$@82", "$@83", "$@84",
  "variant_alias_declaration", "$@85", "$@86", "$@87", "$@88",
  "bitfield_alias_declaration", "$@89", "$@90", "$@91", "$@92",
  "make_decl", "make_struct_fields", "make_variant_dim",
  "make_struct_single", "make_struct_dim_list", "make_struct_dim_decl",
  "optional_make_struct_dim_decl", "use_initializer", "make_struct_decl",
  "$@93", "$@94", "$@95", "$@96", "$@97", "$@98", "$@99", "$@100",
  "make_map_tuple", "make_tuple_call", "$@101", "$@102", "make_dim_decl",
  "$@103", "$@104", "$@105", "$@106", "$@107", "$@108", "$@109", "$@110",
  "$@111", "$@112", "expr_map_tuple_list", "make_table_decl", "$@113",
  "array_comprehension_where", "optional_comma", "array_comprehension", YY_NULLPTR
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

#define YYTABLE_NINF (-779)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1449,    67, -1449, -1449,    62,   -90,   -80,   102, -1449,   -67,
   -1449, -1449, -1449, -1449,   115,   226, -1449, -1449, -1449, -1449,
     -40,   -40,   -40, -1449,   243, -1449,    64, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449,     1, -1449,    24,
      32,    51, -1449, -1449,   -80,    22, -1449, -1449, -1449,    93,
     -40, -1449, -1449,    64,   102,   102,   102,   109,   168, -1449,
   -1449, -1449, -1449,   226,   226,   226,    95, -1449,   734,   103,
   -1449, -1449, -1449, -1449,   572, -1449,    80, -1449,   698,    39,
      62,   180,   -90,   150,   149, -1449,   207,   236, -1449, -1449,
   -1449,   722,   272,   305,   326, -1449,   357,   271, -1449, -1449,
     -65,    62,   226,   226,   226,   226,   358, -1449, -1449, -1449,
   -1449,   727, -1449, -1449,   470,   741, -1449, -1449,   332, -1449,
   -1449, -1449, -1449, -1449,   774,   212, -1449, -1449, -1449, -1449,
     490, -1449, -1449,   407, -1449, -1449, -1449,   367,   358,   358,
   -1449, -1449,   424, -1449,   120, -1449,   444,   454,   734, -1449,
     443, -1449, 10862, -1449, -1449,   459, -1449, -1449, -1449, -1449,
   -1449, -1449,   430, -1449, -1449, -1449,   806, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449,   121, -1449,  7341, -1449,   484, -1449,
   -1449, -1449, -1449, -1449, -1449, 10974, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449,   623,   658, -1449,   503,   358, -1449,   534,   624,   -44,
      62,   532,   579, -1449, -1449, -1449,   212, -1449,   564,   576,
     588,   595,   616,   653, -1449, -1449, -1449,   603, -1449, -1449,
   -1449, -1449, -1449,   656, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449,   659, -1449, -1449, -1449,   678,
     681, -1449, -1449, -1449, -1449,   683,   684,   617,   115, -1449,
   -1449, -1449, -1449, -1449,   138,   621,   689, -1449,   664,   676,
     682,   736,   740, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449,   742,   708, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449,   866, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
     746,   711, -1449, -1449,   -73,   744, -1449,   714,   367, -1449,
     754,   358, -1449, -1449,   430,   358,    62, -1449,   570, -1449,
   -1449, -1449, -1449, -1449,  7004, -1449, -1449,   761, -1449,   324,
     479,   481, -1449, -1449,  7004,   157, -1449, -1449, -1449,     5,
   -1449, -1449, -1449,     6, -1449,  4139,   729,  1429, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449,   762,  1293, -1449,   721, -1449,
      50,   728,    96,   732,  7341, -1449, -1449,   724, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449,   731,   771, -1449,
     455, -1449,   358,   775,  7341, -1449,   125,  7341,  7341,  7341,
     783,   786, -1449, -1449,    40,   115,   787,    35, -1449,   213,
     782,   800,   803,   791,   814,   792,   274,   815, -1449,   314,
     820,   825,  7004,  7004,   810,   821,   827,   828,   833,   834,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,  7004,
    7004,  7004,  7004,  7004,  3757,  4330, -1449,   799,   973, -1449,
   -1449, -1449,   835, -1449, -1449, -1449, -1449,   817, -1449, -1449,
   -1449, -1449, -1449, -1449,   486,  1940, -1449, -1449,   838, -1449,
   -1449, -1449, -1449, -1449, -1449,  7341,  7341,   811,   857,  7341,
     503,  7341,   503,  7341,   503,  7434,   861,  7528, -1449,  7004,
   -1449, -1449, -1449, -1449,   826, -1449, -1449, 10454,  4521, -1449,
     138,   869,  7434,   861, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449,  7004, -1449, -1449,
     456,   -81,   -81,   -81, -1449,   689, -1449, -1449,   115, -1449,
     302,   840,   998,   581, -1449, -1449, -1449,   822, -1449, -1449,
   -1449,  7004,   463,   464,   862,   386, -1449, -1449, -1449,   844,
   -1449, -1449,   321, -1449,   865,   868,   870, -1449,  7004,  7341,
    7004,  7004, -1449, -1449,  7004, -1449,  7004, -1449,  7004, -1449,
   -1449,  7004, -1449,  7341,   111,   111,  7004,  7004,  7004,  7004,
    7004,  7004,   802,   111,   111,   -17,   111,   111,   849,  1038,
     854,   881, 11306,   856,   307,   881,   884,   858,   369,   871,
     358,  3375,   226,  1054,   859, -1449,   817, -1449,  7600, 11129,
    7004,  7004, -1449, -1449,  7004,  7004,  7004,  7004,   898,  7004,
     237,  7004,  7004,  7004,  7004,  7004,  7004,  7004,  7004,  7004,
    4712,  7004,  7004,  7004,  7004,  7004,  7004,  7004,  7004,  7004,
    7004, 11779,  7004, -1449,  4903,   556,   628, -1449, -1449,   -28,
     639,   744,   672,   744,   709,   744,    10, -1449,   379,   689,
     892, -1449,   618, -1449,  7341,   872, -1449, -1449, -1449, 10538,
   -1449,   457, -1449,   -40, -1449,   -40,  7616,   874,  1026, -1449,
   -1449,   249, -1449, -1449, -1449,  2158,   907, -1449, -1449, -1449,
   -1449,  7004,   914,   915,  7341,   125, -1449,  7004,   646, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449,  7341,  7341,  7341,  7341,
    3948,   917,  7004,  7341, -1449, -1449, -1449,  7341,   881,  1159,
   11057, -1449,   942,  7702,  7341,  7341,  7788,  7341,  7874,  7341,
     881,  7341,  7434,   881,   861,   418,  7960,  8046,  8132,  8218,
    8304,  8390, -1449,  7004,   509,   -45,   916, -1449,  7004, -1449,
    7004, -1449,  7004, -1449,  7004,   918,   474, -1449, -1449,   920,
     921,   526, -1449, -1449,   -45,  7004,   160,  3566, -1449,   130,
     924,   175,   928,   503, -1449,  2364,  1054,   937,   931, -1449,
   -1449,   938,   932, -1449, -1449,   156,   156,   313,   313, 11651,
   11651,   933,   554,   935, -1449, 10622,   -15,   -15,   838,   156,
     156, 11536, 11421, 11504, 11389, 11295, 11140, 11619,  1173,  1823,
     313,   313,   341,   341,   554,   554,   554,   512,  7004,   936,
     939,   515,  7004,  1136,   940, 10706, -1449,   142, -1449, -1449,
   -1449,   975, -1449,   961, -1449,   962, -1449,   964,  7341, -1449,
    7434,  7341, -1449,   861,   637,   689, -1449, -1449, -1449,   483,
   -1449, -1449,  1011, -1449,    27, -1449, -1449,  7004,   981,   983,
    7341, -1449,  7004, -1449,  5094, -1449, -1449,  5285,   984, -1449,
     -40,   992,  5476,   -46,  5667, -1449,   -40,   -40,  1139, -1449,
     712, -1449, -1449,  1138, -1449, -1449,  1143, -1449,  1114,   -40,
     951, -1449,   -40,   -40,   -40,   -40,   -40, -1449,  1092, -1449,
     -40,  1739,   985, -1449,   457,    28,  8476, -1449,  1118,   822,
    7004,   646, -1449, -1449, -1449, -1449,   689,   546,   832,   717,
     545,   154,   965,   966,   673,  8562,   720,  7341,  7434,   861,
     948,   967,   969,  7341,  7004,  7004,   970, -1449,  1093,  1209,
   -1449,  1601, -1449,  1634,   971,  1871,   687,   972,   758,  1054,
   -1449, -1449, -1449, -1449, -1449,   977, 11223,   978,  1119,  1001,
      36,   -45, 11306,  8648, 11306, 11306, -1449,   980,   275,  7004,
    7004,  7341,   503,    41,   976,   942,   328, -1449,   974,   196,
    7195, -1449, -1449, -1449,   117,   744, -1449,   503, -1449,  7004,
   -1449,  7004,  5858,  7004, -1449,   990,   986, -1449, -1449,  7004,
     987, -1449, 10790,  7004,  6049,   991, -1449, 10874, -1449,  6240,
   -1449,  7004, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449,   689, -1449, -1449,   689, -1449, -1449,   993,
    7341, -1449, -1449,  1000, -1449, -1449,  1002, -1449,  1003,  8734,
   -1449,  1140,   -29, 11306,  7004, 11306,  1182,  7004, 11306,  1023,
   -1449,  1022,  1043, 11306, -1449,  7004, 11306, -1449, -1449,  1007,
   -1449, -1449,  1010,  1012,  1014,  1016, -1449,  1174, -1449, -1449,
   -1449, -1449, -1449, -1449,   -76, -1449,  7004,  7004,  7004,  7004,
    7004,  7004,  7004,  7004,  7004,  7004,  7004,  7004,  7004,  7004,
    7004,  7004,  7004,  7004,  7004,   577, -1449, -1449, -1449,  1214,
     430, -1449,  1060, -1449,  7004,   646, -1449, -1449, -1449, -1449,
    1024, -1449, -1449, -1449,  1027,  1061, -1449, -1449,  3117,   780,
     812, -1449, -1449,  7004,  3754, 11306, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449,    92,  6431, -1449,
    1055,  7004,  1065, -1449,   176,  7004,   -11,    48,   330,  7004,
    7004,  7004,  8820,  8906,  3945,   744,  7004, -1449, -1449, -1449,
     474,  1028,  3566,  1067,  1068,  1032,  1070,  1073, -1449,   178,
     358,   744,  7341,  8992,  7341,  9078, -1449,   193,  9164, -1449,
    7004,  1557,  7004, -1449,  9250,  3566, -1449,   251,  7004, -1449,
   -1449, -1449,   264, -1449, -1449, -1449,  7004,   689, -1449, -1449,
   -1449, -1449,  1074,  7004, 11306, -1449, 11306,  7004, -1449, -1449,
   -1449, 11306,  7004,  7004,   -45,  7004,  7004, -1449, -1449,  1001,
   -1449, -1449, -1449, 11306, 11306, 11306, 11306, 11306, 11306, 11306,
   11306, 11306, 11306, 11306, 11306, 11306, 11306, 11306, 11306, 11306,
   11306, 11306, -1449,  1071,   743,  1177,   -40, -1449,   646, -1449,
    1080,  1081, -1449, -1449,  7004,  1090, -1449, -1449, -1449, -1449,
    1082,  1072,  1084,  7004,  7004,  7004,  1085,  1216,  1088,  1094,
    6622, -1449, -1449,   283, -1449, -1449,  9336, -1449,  1132, -1449,
     318,  1263,  1001,  7004,  7004,  7004,  9422, 11306, 11306, -1449,
   -1449, -1449,  1129,   339, -1449,   254, -1449, -1449,  1148, -1449,
   -1449,   117, -1449,  1175,   358,  4136, -1449,   723, -1449, -1449,
   -1449,  7341,  9508,  9594, -1449,   263, -1449,  9680, -1449,  1111,
   -1449, 11306, -1449, 11306,  9766,  9852,    49,  9938, 10024,  1109,
     363,   181, -1449, -1449, -1449,   507, -1449,    14, -1449, -1449,
    1216,  1216, 10110,  1116,  1117,  1120,  1122,  7004, -1449,  7004,
     313,   313,   313,  7004, -1449, -1449,  1216,  1216, -1449, 10196,
   -1449, -1449,  1152, -1449, -1449,  1121,  1163,   368,   384, 11306,
   11306,   360,   439, -1449,  1123,  1127,  1128, -1449,  6813, -1449,
   -1449, -1449, -1449, -1449,   725, -1449, -1449,  1130, -1449, -1449,
    1318,   358,  7004,   358,   358, -1449,    94, -1449, -1449, -1449,
    1302,    14, -1449, -1449,   743,   -61,   -61, -1449,  7004,  1216,
    1216,   545,  1141,  1142,   881,   -61,   545, -1449, -1449,  7004,
   -1449, -1449,  1135,  7004,  7004, -1449,   439,  7004, -1449, -1449,
    7004, 11389, -1449, -1449, -1449, -1449, -1449,  3184,   -40,  1144,
     394, -1449, -1449,  2570,  7341,   125, -1449, -1449,  1302,   456,
     545,  1171,  1172, -1449,  1145,  1146, 10282,   -61,   -61,  1171,
    1147, -1449, -1449,  1160,  1161,  1164,  7428,  7004, 11306, 11306,
   -1449,  1156,  1557, -1449, -1449, -1449, -1449, -1449, -1449, 11306,
   -1449,   549,  1168,  3184,   358, -1449, -1449,  1158,   824,  7004,
   10862, -1449, -1449, -1449, -1449,   395,  1180, -1449, -1449, -1449,
   -1449,  1167,  1176, -1449, -1449, -1449, -1449,  1322,  1183,  7428,
    1179,   358, -1449, -1449,  1181, -1449, -1449,   -40, -1449, -1449,
    7004,   646,   -40, 10862, -1449,   545, -1449, -1449,  7004, -1449,
    1184, -1449,  1187,  7004,  2776, -1449, -1449,   646, -1449, -1449,
     358,   397, 11306, -1449, -1449,  1188,  3184, 10368,  1189, -1449,
   -1449, -1449, -1449,   -40,   358,  1174, -1449,  2982, -1449,  1187,
   -1449,  1191,   549,  1174, -1449, -1449
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   155,     1,   327,     0,     0,     0,   622,   328,     0,
     800,   790,   795,    20,     0,     0,    19,    16,    15,     3,
       0,     0,     0,     8,   658,     7,   603,     6,    11,     5,
       4,    13,    12,    14,   124,   125,   123,   132,   134,    47,
      60,    57,    58,    49,     0,    55,    48,   624,   623,     0,
       0,    26,    25,   603,   622,   622,   622,     0,   301,    45,
     139,   140,   141,     0,     0,     0,   142,   144,   151,     0,
     138,    21,    10,     9,   259,   640,     0,   604,   605,     0,
       0,     0,     0,    50,     0,    56,     0,     0,    53,   625,
     627,    22,     0,     0,     0,   303,     0,     0,   150,   145,
       0,     0,     0,     0,     0,     0,    69,   261,   260,   263,
     258,   628,   650,   649,     0,   653,   607,   606,   613,   130,
     131,   128,   129,   127,     0,     0,   126,   135,    61,    59,
      55,    52,    51,     0,    23,    24,    27,   638,    69,    69,
     302,    43,    46,   149,     0,   146,   147,   148,   152,    67,
      70,   156,     0,   630,   629,     0,   652,   651,   655,   654,
     659,   608,   531,    28,    29,    33,     0,   119,   120,   117,
     118,   116,   115,   121,     0,    54,     0,   636,   639,   801,
     791,   796,    44,   143,    68,     0,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,     0,     0,   162,   157,    69,   631,   632,   646,   610,
       0,   532,     0,    30,    31,    32,     0,   133,     0,     0,
       0,     0,     0,     0,   667,   687,   668,   703,   669,   673,
     674,   675,   676,   693,   680,   681,   682,   683,   684,   685,
     686,   688,   689,   690,   691,   760,   672,   679,   692,   767,
     774,   670,   677,   671,   678,     0,     0,     0,     0,   702,
     724,   727,   725,   726,   787,   722,   626,   637,     0,     0,
       0,   227,   228,   225,   165,   166,   168,   167,   169,   170,
     171,   172,   198,   199,   196,   197,   189,   200,   201,   190,
     187,   188,   226,   209,     0,   224,   202,   203,   204,   205,
     176,   177,   178,   173,   174,   175,   186,     0,   192,   193,
     191,   184,   185,   180,   179,   181,   182,   183,   164,   163,
     208,     0,   194,   195,   531,   160,   269,     0,   638,   647,
       0,    69,   612,   609,   531,    69,     0,   592,   585,   614,
     122,   728,   751,   754,     0,   757,   747,     0,   711,   761,
     768,   775,   781,   784,     0,     0,   737,   742,   736,     0,
     750,   746,   739,     0,   741,     0,   723,     0,   802,   792,
     797,   229,   230,   223,   207,   231,   210,   206,     0,   158,
     326,   555,   556,     0,     0,   262,   264,     0,   694,   697,
     700,   701,   695,   698,   696,   699,   633,     0,   644,   660,
       0,   136,    69,     0,     0,   586,     0,     0,     0,     0,
       0,     0,   435,   436,     0,     0,     0,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   693,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     520,   373,   375,   374,   376,   377,   378,   379,    39,     0,
       0,     0,     0,     0,   295,     0,   358,   359,   867,   433,
     432,   510,   430,   503,   502,   501,   500,   153,   506,   431,
     505,   504,   477,   437,   478,     0,   482,   438,     0,   434,
     805,   809,   806,   807,   808,     0,     0,     0,     0,     0,
     157,     0,   157,     0,   157,     0,     0,     0,   733,   295,
     744,   745,   738,   740,     0,   743,   719,     0,     0,   789,
     788,   708,   564,   570,   232,   212,   213,   215,   214,   216,
     217,   218,   219,   211,   220,   221,   222,     0,   324,   325,
       0,   531,   531,   531,   159,   161,   292,   641,     0,   648,
       0,     0,   587,   585,   611,   137,   593,     0,   583,   584,
     582,     0,     0,     0,     0,   716,   829,   832,   306,   310,
     309,   315,     0,   347,     0,     0,     0,   858,     0,     0,
       0,     0,   338,   341,     0,   344,     0,   862,     0,   838,
     844,     0,   835,     0,   465,   466,     0,     0,     0,     0,
       0,     0,     0,   442,   441,   479,   440,   439,     0,     0,
       0,   874,   353,     0,   301,   874,   874,     0,   360,     0,
      69,     0,     0,   368,   359,   292,   153,   272,     0,     0,
       0,     0,   467,   468,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   419,     0,   731,     0,     0,     0,   704,   706,     0,
       0,   160,     0,   160,     0,   160,   301,   562,     0,   560,
       0,   568,     0,   705,     0,     0,   735,   718,   721,     0,
     709,   634,   565,    85,   571,    85,     0,     0,   662,   552,
     553,   575,   557,   559,   558,     0,   619,   645,   656,   544,
     661,     0,     0,     0,     0,     0,   600,     0,     0,   729,
     752,   755,    18,    17,   714,   715,     0,     0,     0,     0,
     827,     0,     0,     0,   848,   851,   854,     0,   874,     0,
     842,   865,   874,     0,     0,     0,     0,     0,     0,     0,
     874,     0,     0,   874,     0,     0,     0,     0,     0,     0,
       0,     0,    42,     0,    40,     0,     0,   847,   875,   296,
       0,   574,     0,   573,     0,     0,   875,   820,   470,     0,
       0,   406,   403,   405,     0,   297,     0,   295,   422,     0,
       0,     0,     0,   157,   360,     0,   368,     0,     0,   489,
     488,     0,     0,   490,   494,   443,   444,   456,   457,   454,
     455,     0,   483,     0,   475,     0,   507,   508,   509,   445,
     446,   461,   462,   463,   464,     0,     0,   459,   460,   458,
     452,   453,   448,   447,   449,   450,   451,     0,     0,     0,
     412,     0,     0,     0,     0,     0,   427,     0,   758,   748,
     712,     0,   762,     0,   769,     0,   776,     0,     0,   782,
       0,     0,   785,     0,     0,   299,   732,   720,   617,   635,
     803,    83,    86,   793,    86,   798,   554,     0,     0,     0,
       0,   576,     0,   294,   318,   316,   269,     0,     0,   317,
       0,     0,     0,    69,     0,   273,     0,     0,     0,   286,
       0,   287,   281,     0,   278,   277,     0,   279,     0,     0,
       0,   293,     0,    81,    82,    79,    80,   288,   330,   276,
       0,   380,   615,   620,   634,   546,     0,   589,   590,     0,
       0,     0,   602,   730,   753,   756,   717,     0,     0,     0,
     828,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   875,     0,   480,     0,     0,
     481,     0,   511,     0,     0,     0,     0,     0,     0,   368,
     515,   516,   517,   518,   519,     0,    36,     0,   100,     0,
       0,     0,   354,     0,   811,   810,   469,     0,     0,     0,
       0,     0,   157,     0,     0,   874,     0,   423,     0,     0,
       0,   426,   424,   154,     0,   160,   372,   157,   485,     0,
     491,     0,     0,     0,   473,     0,     0,   495,   499,     0,
       0,   476,     0,     0,     0,     0,   413,     0,   420,     0,
     471,     0,   428,   759,   749,   713,   707,   763,   765,   770,
     772,   777,   779,   561,   783,   563,   567,   786,   569,     0,
       0,   710,   618,     0,    84,   566,     0,   572,     0,     0,
     664,   665,   578,   577,     0,   319,     0,     0,   304,     0,
     291,     0,     0,    64,   269,     0,   321,   289,   290,     0,
      74,    75,     0,     0,     0,     0,   280,   265,   275,   282,
     283,   284,   285,   329,     0,   274,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   635,   642,   545,   657,     0,
     531,   588,     0,   597,     0,     0,   601,   830,   833,   307,
       0,   312,   313,   311,     0,     0,   350,   348,     0,     0,
       0,   859,   857,   297,     0,   841,   866,   869,   339,   342,
     345,   863,   861,   839,   845,   843,   836,    69,     0,    37,
       0,     0,     0,   331,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   160,     0,   868,   298,   425,
       0,     0,   295,     0,     0,     0,     0,     0,   366,     0,
      69,   160,     0,     0,     0,     0,   401,     0,     0,   496,
       0,   484,     0,   474,     0,   295,   414,     0,     0,   472,
     421,   417,     0,   766,   773,   780,   295,   300,   804,   794,
     799,   663,     0,     0,   320,   269,   305,     0,    62,    63,
     109,   322,     0,     0,     0,     0,     0,   266,   271,     0,
     594,   336,   335,   390,   391,   393,   392,   394,   384,   385,
     386,   395,   396,   382,   383,   397,   398,   387,   388,   389,
     381,   616,   621,     0,   538,   541,     0,   591,     0,   599,
       0,     0,   308,   314,     0,     0,   349,   849,   852,   855,
       0,     0,     0,     0,     0,     0,     0,   827,     0,     0,
       0,   269,   521,     0,    34,    41,     0,   102,     0,   103,
       0,   104,     0,     0,     0,     0,     0,   813,   812,   404,
     530,   407,     0,     0,   399,     0,   363,   364,     0,   362,
     361,     0,   369,   269,    69,     0,   529,     0,   527,   402,
     524,     0,     0,     0,   523,     0,   415,     0,   418,     0,
     666,   579,   323,   114,     0,     0,     0,     0,     0,     0,
       0,     0,   643,   539,   540,   541,   542,   533,   547,   598,
     827,   827,     0,     0,     0,     0,     0,   295,   870,   297,
     340,   343,   346,     0,   828,   840,   827,   827,   512,     0,
     514,   522,    38,   101,   332,     0,     0,     0,     0,   815,
     814,     0,     0,   410,     0,     0,     0,   367,     0,   355,
     370,   269,   486,   492,     0,   528,   526,     0,   525,   734,
      76,    69,     0,    69,    69,   292,   585,   337,   595,   596,
     536,   533,   534,   535,   538,   826,   826,   351,     0,   827,
     827,   818,     0,     0,   874,   826,   818,   513,    35,     0,
     105,   106,     0,     0,     0,   408,     0,     0,   400,   365,
       0,   356,   371,   487,   493,   497,   416,     0,     0,    90,
       0,   269,   269,     0,     0,     0,   537,   548,   536,     0,
       0,   823,   874,   825,     0,     0,     0,   826,   826,   819,
       0,   860,   871,     0,     0,     0,   872,     0,   817,   816,
     411,   872,   357,   498,    77,    81,    82,    79,    80,    78,
      99,    71,     0,     0,    69,   111,   113,     0,     0,     0,
       0,   550,   581,   580,   543,     0,   875,   824,   831,   834,
     352,     0,     0,   856,   864,   846,   837,     0,     0,   872,
       0,    69,    65,    66,     0,    98,   292,     0,   269,   268,
       0,     0,     0,     0,   821,     0,   850,   853,     0,   876,
       0,   878,    95,     0,     0,    91,   108,     0,   334,   549,
      69,     0,   873,   877,    72,     0,     0,     0,     0,   333,
     269,   822,   292,     0,    69,   265,   551,     0,    96,    95,
      89,     0,    71,   265,    73,    94
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1449, -1449,  -657,    -1, -1449, -1449, -1449, -1449,   784,  1308,
   -1449, -1449, -1449, -1449, -1449, -1449,  1393, -1449, -1449, -1449,
    1354, -1449,  1270, -1449, -1449,  1327, -1449, -1449, -1449, -1449,
    -135,  -172, -1449, -1449, -1415,   716,   718, -1449, -1449, -1449,
   -1449,  -167, -1449, -1449, -1449, -1449, -1449, -1449,  -770, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449,  1192, -1449, -1449,
     -48,  1313, -1449, -1449, -1449,   547,   793,   794,   492,  -474,
    -646, -1449, -1449, -1449, -1448, -1449, -1449, -1257, -1449, -1449,
    -873, -1449, -1449, -1449, -1449,  -618,  -497, -1108, -1449,   -13,
   -1449, -1449, -1449, -1449, -1449, -1409, -1399, -1398, -1393, -1449,
   -1449,  1418, -1449, -1186, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449,  -447, -1449,   943,
     100, -1449,  -753, -1449, -1449, -1449, -1449, -1449, -1449, -1319,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,   604,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,  -156,     2,
     -39,    -2,    69, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
     447,  -500,  -715, -1449,  -507,  -713, -1449,  -691,   -41,   -38,
   -1449,  -549,  -544, -1449, -1449, -1449, -1061, -1449,  1377, -1449,
   -1449, -1449, -1449, -1449,   317,   510, -1449,   963, -1449, -1449,
   -1449, -1449, -1449, -1449,   511, -1449,  1102, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449,  -113, -1449,  1017, -1449, -1449, -1449, -1449, -1449,
   -1449,  -548, -1449, -1449,  -330, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449,  -171, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,  1019,
    -772,     8,  -590, -1449, -1449, -1167,  -734, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449,  -944, -1449, -1449, -1449,
   -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449, -1449,
   -1449,   867, -1449, -1449, -1335,  -592, -1449
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   724,   725,    18,   136,    53,    19,   166,   172,
    1382,  1160,  1294,   602,   469,   142,   470,    97,    21,    22,
      45,    46,    88,    23,    41,    42,   896,   897,  1534,   150,
     151,  1535,  1082,  1458,   898,   872,   873,  1501,  1502,  1568,
    1503,  1564,  1565,  1581,  1566,   899,   900,   901,   980,   902,
     903,   904,   905,   906,   907,   908,   909,   173,   174,    37,
      38,    39,   221,    66,    67,    68,    69,   623,    24,   335,
     395,   214,    25,   109,   215,   110,   152,  1238,  1349,  1507,
     396,   397,   910,   471,   911,   705,   610,   994,   864,   472,
     912,   569,   729,  1272,   473,   913,   914,   915,   916,   917,
     540,   918,  1094,  1164,  1241,   919,   474,   744,  1283,   745,
    1284,   747,  1285,   475,   733,  1276,   476,   611,  1400,   477,
    1188,  1189,   793,   478,   627,   479,   920,   480,   481,   783,
     482,   991,  1392,   992,  1446,   483,   843,  1210,   484,   612,
    1192,  1453,  1194,  1454,  1331,  1493,   486,   487,   390,  1424,
    1467,  1355,  1357,  1266,   925,  1120,  1510,  1543,   391,   392,
     393,   677,   678,   693,   681,   682,   695,   774,   699,   700,
    1514,   561,   416,   553,   348,  1351,   554,   349,    78,   118,
     219,   344,    27,   162,   923,   869,   924,    49,    50,   133,
      28,   155,   217,   338,   870,   178,   179,    29,   111,   706,
    1263,   549,   340,   341,   115,   160,   710,    30,    76,   218,
     550,   701,   488,   406,   271,   272,   669,   691,   273,   498,
    1035,   860,   564,   376,   274,   275,   417,   933,   684,   496,
    1034,   418,   934,   419,   935,   495,  1033,   499,  1037,   500,
    1213,   501,  1039,   502,  1214,   503,  1041,   504,  1215,   505,
    1044,   506,  1047,   679,    31,    55,   279,   522,  1056,    32,
      56,   280,   523,  1058,    33,    54,   278,   521,  1053,   489,
     616,  1480,   617,  1472,  1473,  1474,   943,   490,   727,  1270,
     728,  1271,   754,  1289,   751,  1287,   741,   491,   752,  1288,
     492,   947,  1364,   948,  1365,   949,  1366,   737,  1280,   749,
    1286,   995,   493,   620,  1528,   769,   494
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,    59,    70,   180,   181,   276,   222,   795,   717,   715,
     882,  1146,   685,  1066,   993,   999,   694,   726,   615,    71,
      72,    73,   692,   775,   777,   853,   671,   855,   673,   857,
     675,   790,   127,  1242,   868,  1281,  -155,   966,   510,   512,
     574,   968,  1494,  1007,    84,   420,   421,   520,  1495,    90,
      70,    70,    70,  1350,   119,   120,  1165,   538,  1496,  1497,
    1422,  1176,  1542,   270,  1498,   427,    40,     2,  1303,  1412,
     388,   429,   551,  1445,     3,   102,   103,   104,   388,    85,
     336,    34,    35,    43,   557,   112,   113,   552,  1537,    70,
      70,    70,    70,   613,  1495,  1560,    51,     4,   771,     5,
     539,     6,   996,   977,  1496,  1497,    44,     7,   436,   437,
    1498,   638,   640,   641,   640,   641,  1387,     8,   978,   149,
    1423,    13,   220,     9,  1239,    13,    77,  1490,   389,  1240,
     220,   738,   628,   629,   863,    52,   143,   575,   576,  1470,
     942,   750,   439,   440,   753,   773,   951,    10,   377,    96,
     956,  1573,  1301,   850,   586,   979,  1530,  1495,   964,    47,
      16,   967,   342,   269,    16,    48,   930,  1496,  1497,    11,
      12,   366,   345,  1498,   789,    79,   851,   628,   629,    57,
     661,   662,   661,   662,   511,   513,   858,   114,   410,  1302,
     680,   953,    13,  1117,  1550,   121,   998,    80,   367,   368,
     122,  1230,   123,    58,   514,   124,   409,    86,    81,  1166,
     411,  1167,  1052,   515,  1166,   577,  1157,   847,   343,    87,
     568,  1166,  1166,   545,    82,    36,    15,   167,   168,   632,
     633,    16,    13,  1139,  1118,   578,  1140,   638,   464,   125,
     640,   641,   642,   643,  1183,   468,   562,   563,   565,   739,
    1184,    74,   558,    14,    57,   365,    89,   149,   559,  1475,
      60,  1433,   369,   755,   270,    15,   370,   868,  1484,   542,
    1464,    16,    95,   415,   632,   633,   105,   555,    58,    75,
    1185,   270,   638,   941,   639,   640,   641,   642,   643,    61,
    1419,  1186,  1290,    80,   226,   101,  1187,  1171,   412,   560,
     543,   270,   106,  1000,   270,   270,   270,    96,   661,   662,
    1521,  1522,   130,   371,   613,  1031,  1050,   372,  1580,  1005,
     373,   183,   227,   996,   665,   666,  1585,  1031,   670,   551,
     672,  1001,   674,    62,   628,   629,   128,   508,   656,   657,
     658,   659,   660,  1032,   552,   374,    13,    87,   105,  1298,
    1130,  1321,  1342,   661,   662,  1131,  1048,   509,    63,  1190,
    1045,   997,   628,   629,   269,    57,  1031,  1057,   169,  1180,
     131,  1223,  1055,   170,  1003,   171,   771,  1299,   124,  1322,
    1124,   269,   270,   270,   929,    16,   270,  1417,   270,    58,
     270,   813,   270,   579,  1329,  1181,  1135,   937,   938,   132,
     814,   269,   771,  1178,   269,   269,   269,   950,  1170,   270,
    1315,   269,   572,   580,   958,   959,    64,   961,  1380,   963,
     863,   965,   879,   773,  1031,   880,    65,  1180,   881,   630,
     631,   632,   633,  1335,   771,   137,  1180,  1031,   141,   638,
     772,   639,   640,   641,   642,   643,    96,   644,   645,   773,
    1399,   366,  1336,  1395,   587,   771,  1031,   771,  1052,   632,
     633,   772,  1407,  1304,  1346,  1338,   270,   638,   138,   639,
     640,   641,   642,   643,   588,   156,   157,  1305,   367,   368,
     270,   773,   269,   269,  1381,   785,   269,   771,   269,   139,
     269,  1031,   269,  1443,   590,   656,   657,   658,   659,   660,
    -764,   731,   773,  1491,   773,  -764,   708,   709,    57,   269,
     661,   662,  1031,   865,   591,   800,   804,   779,  1175,  1385,
     140,   732,   780,   149,  -764,   658,   659,   660,  1452,  1312,
     818,   177,    58,  1191,   773,   707,  1298,   161,   661,   662,
    1394,  1298,   369,   722,    13,  1324,   370,    85,   844,   781,
     722,    13,   723,  1375,  1420,   936,   716,  1031,   939,   723,
     859,  1531,   946,   377,  1416,  1197,   269,  1031,  1180,  1441,
    1180,   270,  1532,  1533,  1356,   628,   629,  1207,    57,   366,
     269,   176,  1212,    16,   102,  1442,   104,   779,  1505,  1506,
      16,   182,   863,   371,   102,  1504,  1544,   372,  1571,   969,
     373,   270,    58,   551,   697,   782,   367,   368,   184,    70,
      98,    99,   100,   270,   270,   270,   270,  1138,   552,   698,
     270,   722,   216,  1144,   270,   374,  1425,  1426,   987,   107,
     723,   270,   270,   220,   270,   108,   270,   988,   270,   270,
     377,   377,  1435,  1436,   719,   720,  1051,   722,   277,   145,
     146,   147,   148,  1471,  1471,  -771,   723,  -778,   332,  1479,
    -771,  1174,  -778,  1471,  1479,  1556,  1020,   628,   629,  1025,
     369,   269,   632,   633,   370,  1021,   223,   224,  1026,  -771,
     638,  -778,   639,   640,   641,   642,   643,  1043,   466,   624,
    1046,   625,   871,   333,   871,  1477,  1478,  1576,  1515,   613,
    1444,   269,  -409,   334,   895,  1471,  1471,  -409,   996,  1062,
     337,  1293,  1018,   269,   269,   269,   269,   932,  1300,  1339,
     269,   371,  1080,  1081,   269,   372,  -409,  1127,   373,  1313,
     339,   269,   269,   377,   269,   346,   269,   848,   269,   269,
     922,   722,   347,   413,   351,   270,   414,   270,   270,   415,
     723,   661,   662,   374,   713,   116,   352,   714,  1074,   270,
     415,   117,   630,   631,   632,   633,   634,   270,   353,   635,
     636,   637,   638,  1561,   639,   640,   641,   642,   643,   134,
     644,   645,   722,    13,   153,   135,   646,   647,   648,   398,
     154,   723,   649,   399,   895,   354,   355,  1463,   158,   862,
    1353,   722,    13,   357,   159,   377,  1354,   400,   401,   849,
     723,    13,   402,   403,   404,   405,   377,   364,  1049,   375,
     852,   650,    16,   651,   652,   653,   654,   655,   656,   657,
     658,   659,   660,   356,   270,   270,   358,   722,    13,   359,
     270,    16,  1483,   661,   662,   269,   723,   269,   269,   377,
      16,   722,    13,   854,  1134,   366,  1388,   366,   360,   269,
     723,   361,  1325,   362,   363,   366,   377,   269,  1154,   378,
    1432,  1054,  1465,  1054,   102,   103,   104,    16,   270,  1217,
    1517,   379,   367,   368,   367,   368,   377,   380,   385,  1070,
     856,    16,   367,   368,   377,  1077,  1078,   377,  1129,   381,
     377,  1137,   377,   382,  1403,   383,  1455,   384,  1086,   386,
     387,  1088,  1089,  1090,  1091,  1092,  1509,   408,  1554,  1095,
     394,   537,   722,    13,   497,   524,  1434,   518,  1123,   546,
    1126,   723,   541,   544,   269,   269,   547,   270,   556,  1156,
     269,   163,   164,   165,   722,    13,   369,   548,   369,   558,
     370,   558,   370,   723,  1577,   559,   369,   559,   485,  1540,
     370,  1278,    16,   566,  1265,  1460,   567,   573,   507,   163,
     164,   762,   763,   223,   224,   225,   722,    13,   269,   517,
     582,   366,   581,   583,    16,   723,   619,    13,   702,   703,
     704,   584,   586,  1279,   585,   589,   560,   371,   560,   371,
     592,   372,   618,   372,   373,   593,   373,   371,   367,   368,
     596,   372,   667,  1128,   373,   622,    16,    92,    93,    94,
     668,   597,  1291,  1327,   680,   686,    16,   598,   599,   374,
     228,   374,   690,   600,   601,   621,   229,   269,   664,   374,
     711,   712,   230,   721,   730,   734,   594,   595,   735,   765,
     736,   766,   231,   767,   768,  1323,   770,   776,   792,   778,
     232,   811,   794,   603,   604,   605,   606,   607,   861,   878,
     922,   784,   369,   866,   877,   233,   370,   927,   928,   270,
     944,   270,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   955,   981,  1008,  1010,   986,
     989,   990,   689,   371,  1269,  1002,   366,   372,  1004,  1141,
     373,  1009,  1011,  1012,  1508,  1013,  1023,  1028,  1036,  1024,
    1029,   696,  1038,  1040,  1060,  1042,  1061,  1069,  1071,  1079,
      57,  1083,  1084,   367,   368,   374,  1085,  1087,  1093,  1114,
    1404,  1122,  1162,   267,  1163,   718,  1132,  1133,  1142,  1143,
    1199,  1147,  1152,  1155,   676,  1179,    13,  1158,  1161,   269,
    1169,   269,  1177,  1222,   740,   743,  1200,  1202,   746,  1401,
     748,  1208,   366,  1216,   628,   629,  1225,  1227,  1228,  1229,
     756,   757,   758,   759,   760,   761,  1218,  1232,  1219,  1220,
    1233,  1237,  1234,   268,  1235,    16,  1236,   369,   270,   367,
     368,   370,  1264,  1267,  1275,  1273,  1295,  1274,  1297,  1314,
    1316,  1317,  1318,  1319,   805,   806,  1320,  1340,   807,   808,
     809,   810,   366,   812,  1356,   815,   816,   817,   819,   820,
     821,   822,   823,   824,   826,   827,   828,   829,   830,   831,
     832,   833,   834,   835,   836,  1358,   845,  1359,   371,   367,
     368,  1363,   372,  1368,  1148,   373,  1459,  1352,  1461,  1462,
    1360,  1361,  1367,   369,  1369,  1373,  1374,   370,  1376,   630,
     631,   632,   633,   634,  1377,  1384,   635,   636,   637,   638,
     374,   639,   640,   641,   642,   643,  1386,   644,   645,   921,
    1393,  1396,  1409,  1398,  1415,   926,  1428,  1429,   269,  1438,
    1430,   931,  1431,   722,    13,  1439,  1440,  1447,  1448,  1449,
    1457,  1456,   723,   369,   371,  1466,   945,   370,   372,  1487,
     952,   373,  1481,  1482,  1180,  1516,  1518,  1519,  1523,   -87,
    1418,   270,   653,   654,   655,   656,   657,   658,   659,   660,
    1527,  1524,  1525,    16,  1539,  1526,   374,   976,  1546,  1538,
     661,   662,   982,  1536,   983,  1548,   984,  1547,   985,   782,
    1545,  1553,  1549,  1563,   371,  1551,   764,   126,   372,   740,
    1149,   373,   -92,  1572,    20,  1575,  1552,  1583,    83,   921,
     175,   525,   526,   527,   528,   529,   530,   531,   532,   129,
    1584,   874,  1582,   875,   144,   791,   374,  1119,   350,    26,
     796,  1397,  1469,  1468,  1421,  1570,   533,   626,  1512,  1511,
      91,  1513,  1262,   782,  1115,  1116,   534,   535,   536,  1579,
     407,   570,  1022,   571,  1485,     0,  1027,   742,   228,     0,
       0,   269,     0,     0,   229,     0,     0,  1500,     0,     0,
     230,     0,   895,     0,     0,     0,     0,     0,     0,     0,
     231,     0,     0,     0,     0,     0,     0,     0,   232,     0,
       0,  1059,     0,     0,     0,     0,  1063,     0,  1065,     0,
       0,  1068,     0,   233,     0,     0,  1073,     0,  1076,     0,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,     0,  1125,     0,  1555,     0,     0,     0,
    1558,  1559,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   895,     0,     0,  1569,     0,  1145,   740,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,  1578,     0,     0,     0,   895,     0,   628,   629,
       0,   267,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,  1172,  1173,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   982,     0,     0,     0,     0,     0,
       0,     0,     0,  1193,     0,  1195,     0,  1198,     0,     0,
       0,     0,     0,  1201,     0,     0,     0,  1204,     0,     0,
       0,   268,     0,     0,   366,   982,   519,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,   368,     0,     0,     0,     0,   366,  1224,     0,
       0,  1226,     0,   630,   631,   632,   633,   634,     0,  1231,
     635,   636,   637,   638,     0,   639,   640,   641,   642,   643,
       0,   644,   645,     0,   367,   368,     0,   646,   647,   648,
    1243,  1244,  1245,  1246,  1247,  1248,  1249,  1250,  1251,  1252,
    1253,  1254,  1255,  1256,  1257,  1258,  1259,  1260,  1261,     0,
       0,     0,     0,     0,     0,   369,     0,     0,  1268,   370,
       0,     0,   650,     0,   651,   652,   653,   654,   655,   656,
     657,   658,   659,   660,     0,     0,     0,   740,     0,   -78,
       0,     0,     0,     0,   661,   662,     0,     0,   369,     0,
     628,   629,   370,     0,     0,  1296,     0,     0,     0,     0,
       0,     0,     0,  1306,  1307,  1308,   371,     0,     0,     0,
     372,     0,  1150,   373,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1332,     0,  1333,     0,   374,   371,
       0,     0,  1337,   372,     0,  1151,   373,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1341,     0,     0,
       0,  1343,     0,     0,     0,     0,  1344,  1345,     0,  1347,
    1348,   374,     0,     0,   628,   629,     0,  1096,  1097,  1098,
    1099,  1100,  1101,  1102,  1103,   630,   631,   632,   633,   634,
    1104,  1105,   635,   636,   637,   638,  1106,   639,   640,   641,
     642,   643,  1107,   644,   645,  1108,  1109,     0,  1362,   646,
     647,   648,  1110,  1111,  1112,   649,     0,  1370,  1371,  1372,
       0,     0,     0,     0,  1379,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,     0,     0,     0,  1389,  1390,
       0,     0,     0,  1113,   650,     0,   651,   652,   653,   654,
     655,   656,   657,   658,   659,   660,     0,     0,     0,     0,
       0,   367,   368,     0,     0,     0,   661,   662,     0,   630,
     631,   632,   633,   634,     0,     0,   635,   636,   637,   638,
       0,   639,   640,   641,   642,   643,     0,   644,   645,     0,
       0,   628,   629,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   740,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   369,     0,     0,     0,   370,
       0,     0,  1451,   654,   655,   656,   657,   658,   659,   660,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     661,   662,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1476,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1486,     0,     0,   371,  1488,  1489,     0,
     372,   740,  1153,   373,  1492,     0,   630,   631,   632,   633,
     634,  1499,     0,   635,   636,   637,   638,   921,   639,   640,
     641,   642,   643,     0,   644,   645,     0,     0,   374,     0,
     646,   647,   648,     0,     0,     0,   649,     0,     0,     0,
       0,  1529,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1499,     0,     0,
       0,     0,     0,  1541,     0,   650,     0,   651,   652,   653,
     654,   655,   656,   657,   658,   659,   660,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   661,   662,     0,
       0,   663,     0,     0,  1557,     0,     0,     0,     0,     0,
       0,     0,  1562,     0,     0,     0,     0,  1567,   921,   883,
       0,     0,     0,   420,   421,     3,     0,  -110,   -97,   -97,
    1499,  -107,     0,   422,   423,   424,   425,   426,     0,     0,
       0,   921,     0,   427,   884,   428,   885,   886,     0,   429,
       0,     0,     0,     0,     0,     0,   887,   430,     0,     0,
    -112,     0,   888,   431,     0,     0,   432,     0,     8,   433,
     889,     0,   890,   434,     0,     0,   891,   892,     0,     0,
       0,     0,     0,   893,     0,     0,   436,   437,     0,   234,
     235,   236,     0,   238,   239,   240,   241,   242,   438,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
       0,   256,   257,   258,     0,     0,   261,   262,   263,   264,
     439,   440,   441,   894,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   442,   443,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,   444,   445,   446,   447,   448,
       0,   449,     0,   450,   451,   452,   453,   454,   455,   456,
     457,    58,     0,    13,   458,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     459,   460,   461,     0,    14,     0,     0,   462,   463,     0,
       0,     0,     0,     0,     0,     0,   464,     0,   465,     0,
     466,   467,    16,   468,  -270,   883,     0,     0,     0,   420,
     421,     3,     0,  -110,   -97,   -97,     0,  -107,     0,   422,
     423,   424,   425,   426,     0,     0,     0,     0,     0,   427,
     884,   428,   885,   886,     0,   429,     0,     0,     0,     0,
       0,     0,   887,   430,     0,     0,  -112,     0,   888,   431,
       0,     0,   432,     0,     8,   433,   889,     0,   890,   434,
       0,     0,   891,   892,     0,     0,     0,     0,     0,   893,
       0,     0,   436,   437,     0,   234,   235,   236,     0,   238,
     239,   240,   241,   242,   438,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,   256,   257,   258,
       0,     0,   261,   262,   263,   264,   439,   440,   441,   894,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   442,   443,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,   444,   445,   446,   447,   448,     0,   449,     0,   450,
     451,   452,   453,   454,   455,   456,   457,    58,     0,    13,
     458,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   459,   460,   461,     0,
      14,     0,     0,   462,   463,     0,     0,     0,     0,     0,
       0,     0,   464,     0,   465,     0,   466,   467,    16,   468,
    1006,   883,     0,     0,     0,   420,   421,     3,     0,  -110,
     -97,   -97,     0,  -107,     0,   422,   423,   424,   425,   426,
       0,     0,     0,     0,     0,   427,   884,   428,   885,   886,
       0,   429,     0,     0,     0,     0,     0,     0,   887,   430,
       0,     0,  -112,     0,   888,   431,     0,     0,   432,     0,
       8,   433,   889,     0,   890,   434,     0,     0,   891,   892,
       0,     0,     0,     0,     0,   893,     0,     0,   436,   437,
       0,   234,   235,   236,     0,   238,   239,   240,   241,   242,
     438,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,     0,   256,   257,   258,     0,     0,   261,   262,
     263,   264,   439,   440,   441,   894,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   442,   443,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,   444,   445,   446,
     447,   448,     0,   449,     0,   450,   451,   452,   453,   454,
     455,   456,   457,    58,     0,    13,   458,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   459,   460,   461,     0,    14,     0,     0,   462,
     463,     0,     0,     0,     0,     0,     0,     0,   464,     0,
     465,     0,   466,   467,    16,   468,  -267,   883,     0,     0,
       0,   420,   421,     3,     0,  -110,   -97,   -97,     0,  -107,
       0,   422,   423,   424,   425,   426,     0,     0,     0,     0,
       0,   427,   884,   428,   885,   886,     0,   429,     0,     0,
       0,     0,     0,     0,   887,   430,     0,     0,  -112,     0,
     888,   431,     0,     0,   432,     0,     8,   433,   889,     0,
     890,   434,     0,     0,   891,   892,     0,     0,     0,     0,
       0,   893,     0,     0,   436,   437,     0,   234,   235,   236,
       0,   238,   239,   240,   241,   242,   438,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,     0,   256,
     257,   258,     0,     0,   261,   262,   263,   264,   439,   440,
     441,   894,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   442,   443,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,   444,   445,   446,   447,   448,     0,   449,
       0,   450,   451,   452,   453,   454,   455,   456,   457,    58,
       0,    13,   458,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   459,   460,
     461,     0,    14,     0,     0,   462,   463,     0,     0,     0,
       0,     0,     0,     0,   464,     0,   465,     0,   466,   467,
      16,   468,   -88,   883,     0,     0,     0,   420,   421,     3,
       0,  -110,   -97,   -97,     0,  -107,     0,   422,   423,   424,
     425,   426,     0,     0,     0,     0,     0,   427,   884,   428,
     885,   886,     0,   429,     0,     0,     0,     0,     0,     0,
     887,   430,     0,     0,  -112,     0,   888,   431,     0,     0,
     432,     0,     8,   433,   889,     0,   890,   434,     0,     0,
     891,   892,     0,     0,     0,     0,     0,   893,     0,     0,
     436,   437,     0,   234,   235,   236,     0,   238,   239,   240,
     241,   242,   438,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,     0,   256,   257,   258,     0,     0,
     261,   262,   263,   264,   439,   440,   441,   894,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     442,   443,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,   444,
     445,   446,   447,   448,     0,   449,     0,   450,   451,   452,
     453,   454,   455,   456,   457,    58,     0,    13,   458,     0,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   459,   460,   461,     0,    14,     0,
       0,   462,   463,     0,     0,     0,     0,   367,   368,     0,
     464,     0,   465,     0,   466,   467,    16,   468,   -93,   420,
     421,     0,     0,     0,     0,     0,     0,     0,     0,   422,
     423,   424,   425,   426,     0,     0,     0,     0,     0,   427,
     884,   428,   885,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,   431,
       0,     0,   432,     0,     0,   433,   889,     0,     0,   434,
       0,   369,     0,     0,     0,   370,     0,     0,     0,   435,
       0,     0,   436,   437,     0,   234,   235,   236,     0,   238,
     239,   240,   241,   242,   438,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,   256,   257,   258,
       0,     0,   261,   262,   263,   264,   439,   440,   441,   894,
       0,     0,   371,     0,     0,     0,   372,     0,  1277,   373,
       0,     0,   442,   443,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,   374,     0,     0,     0,     0,     0,
       0,   444,   445,   446,   447,   448,     0,   449,     0,   450,
     451,   452,   453,   454,   455,   456,   457,    58,     0,     0,
     458,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   459,   460,   461,     0,
      14,     0,     0,   462,   463,     0,     0,     0,     0,     0,
     420,   421,   464,     0,   465,     0,   466,   467,     0,   468,
     422,   423,   424,   425,   426,     0,     0,     0,     0,     0,
     427,     0,   428,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,     0,     0,
     431,     0,     0,   432,     0,     0,   433,     0,     0,     0,
     434,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     435,     0,     0,   436,   437,   786,   234,   235,   236,     0,
     238,   239,   240,   241,   242,   438,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,   256,   257,
     258,     0,     0,   261,   262,   263,   264,   439,   440,   441,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   442,   443,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,   444,   445,   446,   447,   448,     0,   449,   613,
     450,   451,   452,   453,   454,   455,   456,   457,   614,     0,
       0,   458,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   459,   460,   461,
       0,    14,     0,     0,   462,   463,     0,     0,     0,     0,
       0,   420,   421,   787,     0,   465,   788,   466,   467,   608,
     468,   422,   423,   424,   425,   426,     0,     0,     0,     0,
       0,   427,     0,   428,     0,     0,     0,   429,     0,     0,
       0,     0,     0,     0,     0,   430,     0,     0,     0,     0,
       0,   431,     0,     0,   432,   609,     0,   433,     0,     0,
       0,   434,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   435,     0,     0,   436,   437,     0,   234,   235,   236,
       0,   238,   239,   240,   241,   242,   438,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,     0,   256,
     257,   258,     0,     0,   261,   262,   263,   264,   439,   440,
     441,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   442,   443,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,   444,   445,   446,   447,   448,     0,   449,
     613,   450,   451,   452,   453,   454,   455,   456,   457,   614,
       0,     0,   458,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   459,   460,
     461,     0,    14,     0,     0,   462,   463,     0,     0,     0,
       0,     0,   420,   421,   464,     0,   465,     0,   466,   467,
     608,   468,   422,   423,   424,   425,   426,     0,     0,     0,
       0,     0,   427,     0,   428,     0,     0,   366,   429,     0,
       0,     0,     0,     0,     0,     0,   430,     0,     0,     0,
       0,     0,   431,     0,     0,   432,   609,     0,   433,     0,
       0,     0,   434,     0,   367,   368,     0,     0,     0,     0,
       0,     0,   435,     0,     0,   436,   437,     0,   234,   235,
     236,     0,   238,   239,   240,   241,   242,   438,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,     0,
     256,   257,   258,     0,     0,   261,   262,   263,   264,   439,
     440,   441,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   442,   443,     0,   369,     0,
       0,     0,   370,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,   444,   445,   446,   447,   448,     0,
     449,     0,   450,   451,   452,   453,   454,   455,   456,   457,
      58,     0,     0,   458,     0,     0,     0,     0,     0,   371,
       0,     0,     0,   372,     0,  1282,   373,     0,     0,   459,
     460,   461,     0,    14,     0,     0,   462,   463,     0,     0,
       0,     0,     0,   420,   421,   464,     0,   465,     0,   466,
     467,   374,   468,   422,   423,   424,   425,   426,     0,     0,
       0,     0,     0,   427,     0,   428,     0,     0,   366,   429,
       0,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,     0,     0,   431,     0,     0,   432,     0,     0,   433,
       0,     0,     0,   434,     0,   367,   368,     0,     0,     0,
       0,     0,     0,   435,     0,     0,   436,   437,   940,   234,
     235,   236,     0,   238,   239,   240,   241,   242,   438,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
       0,   256,   257,   258,     0,     0,   261,   262,   263,   264,
     439,   440,   441,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   442,   443,     0,   369,
       0,     0,     0,   370,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,   444,   445,   446,   447,   448,
       0,   449,   613,   450,   451,   452,   453,   454,   455,   456,
     457,   614,     0,     0,   458,     0,     0,     0,     0,     0,
     371,     0,     0,     0,   372,     0,  1311,   373,     0,     0,
     459,   460,   461,     0,    14,     0,     0,   462,   463,     0,
       0,     0,     0,     0,   420,   421,   464,     0,   465,     0,
     466,   467,   374,   468,   422,   423,   424,   425,   426,     0,
       0,     0,     0,     0,   427,     0,   428,     0,     0,   366,
     429,     0,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,     0,     0,   431,     0,     0,   432,     0,     0,
     433,     0,     0,     0,   434,     0,   367,   368,     0,     0,
       0,     0,     0,     0,   435,     0,     0,   436,   437,     0,
     234,   235,   236,     0,   238,   239,   240,   241,   242,   438,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,   256,   257,   258,     0,     0,   261,   262,   263,
     264,   439,   440,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   442,   443,     0,
     369,     0,     0,     0,   370,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,   444,   445,   446,   447,
     448,     0,   449,     0,   450,   451,   452,   453,   454,   455,
     456,   457,    58,     0,     0,   458,     0,     0,     0,     0,
       0,   371,     0,     0,     0,   372,     0,  1402,   373,     0,
       0,   459,   460,   461,     0,    14,     0,     0,   462,   463,
       0,     0,     0,     0,     0,   420,   421,   464,   516,   465,
       0,   466,   467,   374,   468,   422,   423,   424,   425,   426,
       0,     0,     0,     0,     0,   427,     0,   428,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,   431,     0,     0,   432,     0,
       0,   433,     0,     0,     0,   434,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   435,     0,     0,   436,   437,
       0,   234,   235,   236,     0,   238,   239,   240,   241,   242,
     438,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,     0,   256,   257,   258,     0,     0,   261,   262,
     263,   264,   439,   440,   441,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   442,   443,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,   444,   445,   446,
     447,   448,     0,   449,   613,   450,   451,   452,   453,   454,
     455,   456,   457,   614,     0,     0,   458,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   459,   460,   461,     0,    14,     0,     0,   462,
     463,     0,     0,     0,     0,     0,   420,   421,   464,     0,
     465,     0,   466,   467,     0,   468,   422,   423,   424,   425,
     426,     0,     0,     0,     0,     0,   427,     0,   428,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,     0,
     430,     0,     0,     0,     0,     0,   431,     0,     0,   432,
       0,     0,   433,     0,     0,     0,   434,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   435,     0,     0,   436,
     437,     0,   234,   235,   236,     0,   238,   239,   240,   241,
     242,   438,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,   256,   257,   258,     0,     0,   261,
     262,   263,   264,   439,   440,   441,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   442,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,   444,   445,
     446,   447,   448,     0,   449,     0,   450,   451,   452,   453,
     454,   455,   456,   457,    58,     0,     0,   458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   459,   460,   461,     0,    14,     0,     0,
     462,   463,     0,     0,     0,     0,     0,   420,   421,   464,
     688,   465,     0,   466,   467,     0,   468,   422,   423,   424,
     425,   426,     0,     0,   825,     0,     0,   427,     0,   428,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,   430,     0,     0,     0,     0,     0,   431,     0,     0,
     432,     0,     0,   433,     0,     0,     0,   434,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   435,     0,     0,
     436,   437,     0,   234,   235,   236,     0,   238,   239,   240,
     241,   242,   438,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,     0,   256,   257,   258,     0,     0,
     261,   262,   263,   264,   439,   440,   441,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     442,   443,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,   444,
     445,   446,   447,   448,     0,   449,     0,   450,   451,   452,
     453,   454,   455,   456,   457,    58,     0,     0,   458,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   459,   460,   461,     0,    14,     0,
       0,   462,   463,     0,     0,     0,     0,     0,   420,   421,
     464,     0,   465,     0,   466,   467,     0,   468,   422,   423,
     424,   425,   426,     0,     0,     0,     0,     0,   427,     0,
     428,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,   431,     0,
       0,   432,     0,     0,   433,     0,     0,     0,   434,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   435,     0,
       0,   436,   437,     0,   234,   235,   236,     0,   238,   239,
     240,   241,   242,   438,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,     0,   256,   257,   258,     0,
       0,   261,   262,   263,   264,   439,   440,   441,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   442,   443,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
     444,   445,   446,   447,   448,     0,   449,     0,   450,   451,
     452,   453,   454,   455,   456,   457,    58,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   459,   460,   461,     0,    14,
       0,     0,   462,   463,     0,     0,     0,     0,     0,   420,
     421,   464,     0,   465,   846,   466,   467,     0,   468,   422,
     423,   424,   425,   426,     0,     0,     0,     0,     0,   427,
       0,   428,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,   431,
       0,     0,   432,     0,     0,   433,     0,     0,     0,   434,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   435,
       0,     0,   436,   437,     0,   234,   235,   236,     0,   238,
     239,   240,   241,   242,   438,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,   256,   257,   258,
       0,     0,   261,   262,   263,   264,   439,   440,   441,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   442,   443,     0,     0,     0,     0,     0,     0,
       0,  1064,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,   444,   445,   446,   447,   448,     0,   449,     0,   450,
     451,   452,   453,   454,   455,   456,   457,    58,     0,     0,
     458,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   459,   460,   461,     0,
      14,     0,     0,   462,   463,     0,     0,     0,     0,     0,
     420,   421,   464,     0,   465,     0,   466,   467,     0,   468,
     422,   423,   424,   425,   426,     0,     0,     0,     0,     0,
     427,     0,   428,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,     0,     0,
     431,     0,     0,   432,     0,     0,   433,     0,     0,     0,
     434,     0,     0,     0,     0,     0,  1067,     0,     0,     0,
     435,     0,     0,   436,   437,     0,   234,   235,   236,     0,
     238,   239,   240,   241,   242,   438,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,   256,   257,
     258,     0,     0,   261,   262,   263,   264,   439,   440,   441,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   442,   443,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,   444,   445,   446,   447,   448,     0,   449,     0,
     450,   451,   452,   453,   454,   455,   456,   457,    58,     0,
       0,   458,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   459,   460,   461,
       0,    14,     0,     0,   462,   463,     0,     0,     0,     0,
       0,   420,   421,   464,     0,   465,     0,   466,   467,     0,
     468,   422,   423,   424,   425,   426,     0,     0,     0,     0,
       0,   427,     0,   428,     0,     0,     0,   429,     0,     0,
       0,     0,     0,     0,     0,   430,     0,     0,     0,     0,
       0,   431,     0,     0,   432,     0,     0,   433,     0,     0,
       0,   434,     0,     0,  1072,     0,     0,     0,     0,     0,
       0,   435,     0,     0,   436,   437,     0,   234,   235,   236,
       0,   238,   239,   240,   241,   242,   438,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,     0,   256,
     257,   258,     0,     0,   261,   262,   263,   264,   439,   440,
     441,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   442,   443,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,   444,   445,   446,   447,   448,     0,   449,
       0,   450,   451,   452,   453,   454,   455,   456,   457,    58,
       0,     0,   458,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   459,   460,
     461,     0,    14,     0,     0,   462,   463,     0,     0,     0,
       0,     0,   420,   421,   464,     0,   465,     0,   466,   467,
       0,   468,   422,   423,   424,   425,   426,     0,     0,     0,
       0,     0,   427,     0,   428,     0,     0,     0,   429,     0,
       0,     0,     0,     0,     0,     0,   430,     0,     0,     0,
       0,     0,   431,     0,     0,   432,     0,     0,   433,     0,
       0,     0,   434,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   435,     0,     0,   436,   437,     0,   234,   235,
     236,     0,   238,   239,   240,   241,   242,   438,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,     0,
     256,   257,   258,     0,     0,   261,   262,   263,   264,   439,
     440,   441,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   442,   443,     0,     0,     0,
       0,     0,     0,     0,  1075,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,   444,   445,   446,   447,   448,     0,
     449,     0,   450,   451,   452,   453,   454,   455,   456,   457,
      58,     0,     0,   458,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   459,
     460,   461,     0,    14,     0,     0,   462,   463,     0,     0,
       0,     0,     0,   420,   421,   464,     0,   465,     0,   466,
     467,     0,   468,   422,   423,   424,   425,   426,     0,     0,
       0,     0,     0,   427,     0,   428,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,     0,     0,   431,     0,     0,   432,     0,     0,   433,
       0,     0,     0,   434,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   435,     0,     0,   436,   437,     0,   234,
     235,   236,     0,   238,   239,   240,   241,   242,   438,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
       0,   256,   257,   258,     0,     0,   261,   262,   263,   264,
     439,   440,   441,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   442,   443,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,   444,   445,   446,   447,   448,
       0,   449,     0,   450,   451,   452,   453,   454,   455,   456,
     457,    58,     0,     0,   458,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     459,   460,   461,     0,    14,     0,     0,   462,   463,     0,
       0,     0,     0,     0,   420,   421,   464,     0,   465,  1196,
     466,   467,     0,   468,   422,   423,   424,   425,   426,     0,
       0,     0,     0,     0,   427,     0,   428,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,     0,     0,   431,     0,     0,   432,     0,     0,
     433,     0,     0,     0,   434,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   435,     0,     0,   436,   437,     0,
     234,   235,   236,     0,   238,   239,   240,   241,   242,   438,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,   256,   257,   258,     0,     0,   261,   262,   263,
     264,   439,   440,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   442,   443,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,   444,   445,   446,   447,
     448,     0,   449,     0,   450,   451,   452,   453,   454,   455,
     456,   457,    58,     0,     0,   458,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   459,   460,   461,     0,    14,     0,     0,   462,   463,
       0,     0,     0,     0,     0,   420,   421,  1205,     0,   465,
    1206,   466,   467,     0,   468,   422,   423,   424,   425,   426,
       0,     0,     0,     0,     0,   427,     0,   428,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,   431,     0,     0,   432,     0,
       0,   433,     0,     0,     0,   434,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   435,     0,     0,   436,   437,
       0,   234,   235,   236,     0,   238,   239,   240,   241,   242,
     438,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,     0,   256,   257,   258,     0,     0,   261,   262,
     263,   264,   439,   440,   441,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   442,   443,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,   444,   445,   446,
     447,   448,     0,   449,     0,   450,   451,   452,   453,   454,
     455,   456,   457,    58,     0,     0,   458,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   459,   460,   461,     0,    14,     0,     0,   462,
     463,     0,     0,     0,     0,     0,   420,   421,   464,     0,
     465,  1211,   466,   467,     0,   468,   422,   423,   424,   425,
     426,     0,     0,     0,     0,     0,   427,     0,   428,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,     0,
     430,     0,     0,     0,     0,     0,   431,     0,     0,   432,
       0,     0,   433,     0,     0,     0,   434,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   435,     0,     0,   436,
     437,     0,   234,   235,   236,     0,   238,   239,   240,   241,
     242,   438,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,   256,   257,   258,     0,     0,   261,
     262,   263,   264,   439,   440,   441,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   442,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,   444,   445,
     446,   447,   448,     0,   449,     0,   450,   451,   452,   453,
     454,   455,   456,   457,    58,     0,     0,   458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   459,   460,   461,     0,    14,     0,     0,
     462,   463,     0,     0,     0,     0,     0,   420,   421,   464,
       0,   465,  1292,   466,   467,     0,   468,   422,   423,   424,
     425,   426,     0,     0,     0,     0,     0,   427,     0,   428,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,   430,     0,     0,     0,     0,     0,   431,     0,     0,
     432,     0,     0,   433,     0,     0,     0,   434,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   435,     0,     0,
     436,   437,     0,   234,   235,   236,     0,   238,   239,   240,
     241,   242,   438,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,     0,   256,   257,   258,     0,     0,
     261,   262,   263,   264,   439,   440,   441,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     442,   443,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,   444,
     445,   446,   447,   448,     0,   449,     0,   450,   451,   452,
     453,   454,   455,   456,   457,    58,     0,     0,   458,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   459,   460,   461,     0,    14,     0,
       0,   462,   463,     0,     0,     0,     0,     0,   420,   421,
     464,     0,   465,  1378,   466,   467,     0,   468,   422,   423,
     424,   425,   426,     0,     0,     0,     0,     0,   427,     0,
     428,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,   431,     0,
       0,   432,     0,     0,   433,     0,     0,     0,   434,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   435,     0,
       0,   436,   437,     0,   234,   235,   236,     0,   238,   239,
     240,   241,   242,   438,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,     0,   256,   257,   258,     0,
       0,   261,   262,   263,   264,   439,   440,   441,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   442,   443,     0,     0,     0,     0,     0,     0,     0,
    1450,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
     444,   445,   446,   447,   448,     0,   449,     0,   450,   451,
     452,   453,   454,   455,   456,   457,    58,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   459,   460,   461,     0,    14,
       0,     0,   462,   463,     0,     0,     0,     0,     0,   420,
     421,   464,     0,   465,     0,   466,   467,     0,   468,   422,
     423,   424,   425,   426,     0,     0,     0,     0,     0,   427,
       0,   428,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,   431,
       0,     0,   432,     0,     0,   433,     0,     0,     0,   434,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   435,
       0,     0,   436,   437,     0,   234,   235,   236,     0,   238,
     239,   240,   241,   242,   438,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,   256,   257,   258,
       0,     0,   261,   262,   263,   264,   439,   440,   441,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   442,   443,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,   444,   445,   446,   447,   448,     0,   449,     0,   450,
     451,   452,   453,   454,   455,   456,   457,    58,     0,     0,
     458,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   459,   460,   461,     0,
      14,     0,     0,   462,   463,     0,     0,     0,     0,     0,
     420,   421,   464,     0,   465,     0,   466,   467,     0,   468,
     422,   423,   424,   425,   426,     0,     0,     0,     0,     0,
     427,     0,   428,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,     0,     0,
     431,     0,     0,   432,     0,     0,   433,     0,     0,     0,
     434,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     435,     0,     0,   436,   437,     0,   234,   235,   236,     0,
     238,   239,   240,   241,   242,   438,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,   256,   257,
     258,     0,     0,   261,   262,   263,   264,   439,   440,   441,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   442,   443,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,   444,   445,   446,   447,   448,     0,   449,     0,
     450,   451,   452,   453,   454,   455,   456,   457,    58,     0,
     228,   458,     0,     0,     0,     0,   229,     0,     0,     0,
       0,     0,   230,     0,     0,     0,     0,   459,   460,   461,
       0,    14,   231,     0,   462,   463,     0,     0,     0,     0,
     232,     0,     0,  1182,     0,   465,     0,   466,   467,     0,
     468,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,     0,     0,     0,     0,   628,
     629,     0,     0,   228,     0,     0,     0,     0,     0,   229,
       0,     0,     0,     0,     0,   230,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   231,     0,     0,     0,     0,
      57,     0,     0,   232,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   267,     0,     0,     0,     0,   233,     0,
       0,     0,     0,     0,    58,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,     0,     0,
       0,     0,     0,   268,   630,   631,   632,   633,   634,   628,
     629,   635,   636,   637,   638,     0,   639,   640,   641,   642,
     643,     0,   644,   645,     0,     0,     0,     0,   646,   647,
     648,     0,     0,    57,   649,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   267,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   676,     0,     0,
       0,     0,     0,   650,     0,   651,   652,   653,   654,   655,
     656,   657,   658,   659,   660,     0,     0,     0,     0,   797,
       0,     0,     0,     0,     0,   661,   662,     0,     0,     0,
       0,     0,  1527,     0,     0,     0,   268,   628,   629,     0,
       0,     0,     0,     0,   630,   631,   632,   633,   634,     0,
       0,   635,   636,   637,   638,     0,   639,   640,   641,   642,
     643,     0,   644,   645,     0,     0,     0,     0,   646,   647,
     648,   234,   235,   236,   649,   238,   239,   240,   241,   242,
     438,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,     0,   256,   257,   258,     0,     0,   261,   262,
     263,   264,     0,   650,     0,   651,   652,   653,   654,   655,
     656,   657,   658,   659,   660,     0,     0,     0,     0,     0,
       0,     0,     0,   628,   629,   661,   662,     0,     0,   683,
       0,     0,   630,   631,   632,   633,   634,     0,     0,   635,
     636,   637,   638,     0,   639,   640,   641,   642,   643,     0,
     644,   645,     0,     0,   798,     0,   646,   647,   648,     0,
       0,     0,   649,   799,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   650,     0,   651,   652,   653,   654,   655,   656,   657,
     658,   659,   660,     0,     0,     0,     0,     0,     0,   628,
     629,     0,     0,   661,   662,     0,     0,   876,   630,   631,
     632,   633,   634,     0,     0,   635,   636,   637,   638,     0,
     639,   640,   641,   642,   643,     0,   644,   645,     0,     0,
       0,     0,   646,   647,   648,     0,     0,     0,   649,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   650,     0,   651,
     652,   653,   654,   655,   656,   657,   658,   659,   660,     0,
       0,     0,     0,     0,     0,   628,   629,     0,     0,   661,
     662,     0,     0,   957,   630,   631,   632,   633,   634,     0,
       0,   635,   636,   637,   638,     0,   639,   640,   641,   642,
     643,     0,   644,   645,     0,     0,     0,     0,   646,   647,
     648,     0,     0,     0,   649,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   650,     0,   651,   652,   653,   654,   655,
     656,   657,   658,   659,   660,     0,     0,     0,     0,     0,
       0,   628,   629,     0,     0,   661,   662,     0,     0,   960,
     630,   631,   632,   633,   634,     0,     0,   635,   636,   637,
     638,     0,   639,   640,   641,   642,   643,     0,   644,   645,
       0,     0,     0,     0,   646,   647,   648,     0,     0,     0,
     649,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   650,
       0,   651,   652,   653,   654,   655,   656,   657,   658,   659,
     660,     0,     0,     0,     0,     0,     0,   628,   629,     0,
       0,   661,   662,     0,     0,   962,   630,   631,   632,   633,
     634,     0,     0,   635,   636,   637,   638,     0,   639,   640,
     641,   642,   643,     0,   644,   645,     0,     0,     0,     0,
     646,   647,   648,     0,     0,     0,   649,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   650,     0,   651,   652,   653,
     654,   655,   656,   657,   658,   659,   660,     0,     0,     0,
       0,     0,     0,   628,   629,     0,     0,   661,   662,     0,
       0,   970,   630,   631,   632,   633,   634,     0,     0,   635,
     636,   637,   638,     0,   639,   640,   641,   642,   643,     0,
     644,   645,     0,     0,     0,     0,   646,   647,   648,     0,
       0,     0,   649,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   650,     0,   651,   652,   653,   654,   655,   656,   657,
     658,   659,   660,     0,     0,     0,     0,     0,     0,   628,
     629,     0,     0,   661,   662,     0,     0,   971,   630,   631,
     632,   633,   634,     0,     0,   635,   636,   637,   638,     0,
     639,   640,   641,   642,   643,     0,   644,   645,     0,     0,
       0,     0,   646,   647,   648,     0,     0,     0,   649,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   650,     0,   651,
     652,   653,   654,   655,   656,   657,   658,   659,   660,     0,
       0,     0,     0,     0,     0,   628,   629,     0,     0,   661,
     662,     0,     0,   972,   630,   631,   632,   633,   634,     0,
       0,   635,   636,   637,   638,     0,   639,   640,   641,   642,
     643,     0,   644,   645,     0,     0,     0,     0,   646,   647,
     648,     0,     0,     0,   649,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   650,     0,   651,   652,   653,   654,   655,
     656,   657,   658,   659,   660,     0,     0,     0,     0,     0,
       0,   628,   629,     0,     0,   661,   662,     0,     0,   973,
     630,   631,   632,   633,   634,     0,     0,   635,   636,   637,
     638,     0,   639,   640,   641,   642,   643,     0,   644,   645,
       0,     0,     0,     0,   646,   647,   648,     0,     0,     0,
     649,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   650,
       0,   651,   652,   653,   654,   655,   656,   657,   658,   659,
     660,     0,     0,     0,     0,     0,     0,   628,   629,     0,
       0,   661,   662,     0,     0,   974,   630,   631,   632,   633,
     634,     0,     0,   635,   636,   637,   638,     0,   639,   640,
     641,   642,   643,     0,   644,   645,     0,     0,     0,     0,
     646,   647,   648,     0,     0,     0,   649,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   650,     0,   651,   652,   653,
     654,   655,   656,   657,   658,   659,   660,     0,     0,     0,
       0,     0,     0,   628,   629,     0,     0,   661,   662,     0,
       0,   975,   630,   631,   632,   633,   634,     0,     0,   635,
     636,   637,   638,     0,   639,   640,   641,   642,   643,     0,
     644,   645,     0,     0,     0,     0,   646,   647,   648,     0,
       0,     0,   649,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   650,     0,   651,   652,   653,   654,   655,   656,   657,
     658,   659,   660,     0,     0,     0,     0,     0,     0,   628,
     629,     0,     0,   661,   662,     0,     0,  1121,   630,   631,
     632,   633,   634,     0,     0,   635,   636,   637,   638,     0,
     639,   640,   641,   642,   643,     0,   644,   645,     0,     0,
       0,     0,   646,   647,   648,     0,     0,     0,   649,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   650,     0,   651,
     652,   653,   654,   655,   656,   657,   658,   659,   660,     0,
       0,     0,     0,     0,     0,   628,   629,     0,     0,   661,
     662,     0,     0,  1136,   630,   631,   632,   633,   634,     0,
       0,   635,   636,   637,   638,     0,   639,   640,   641,   642,
     643,     0,   644,   645,     0,     0,     0,     0,   646,   647,
     648,     0,     0,     0,   649,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   650,     0,   651,   652,   653,   654,   655,
     656,   657,   658,   659,   660,     0,     0,     0,     0,     0,
       0,   628,   629,     0,     0,   661,   662,     0,     0,  1168,
     630,   631,   632,   633,   634,     0,     0,   635,   636,   637,
     638,     0,   639,   640,   641,   642,   643,     0,   644,   645,
       0,     0,     0,     0,   646,   647,   648,     0,     0,     0,
     649,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   650,
       0,   651,   652,   653,   654,   655,   656,   657,   658,   659,
     660,     0,     0,     0,     0,     0,     0,   628,   629,     0,
       0,   661,   662,     0,     0,  1221,   630,   631,   632,   633,
     634,     0,     0,   635,   636,   637,   638,     0,   639,   640,
     641,   642,   643,     0,   644,   645,     0,     0,     0,     0,
     646,   647,   648,     0,     0,     0,   649,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   650,     0,   651,   652,   653,
     654,   655,   656,   657,   658,   659,   660,     0,     0,     0,
       0,     0,     0,   628,   629,     0,     0,   661,   662,     0,
       0,  1309,   630,   631,   632,   633,   634,     0,     0,   635,
     636,   637,   638,     0,   639,   640,   641,   642,   643,     0,
     644,   645,     0,     0,     0,     0,   646,   647,   648,     0,
       0,     0,   649,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   650,     0,   651,   652,   653,   654,   655,   656,   657,
     658,   659,   660,     0,     0,     0,     0,     0,     0,   628,
     629,     0,     0,   661,   662,     0,     0,  1310,   630,   631,
     632,   633,   634,     0,     0,   635,   636,   637,   638,     0,
     639,   640,   641,   642,   643,     0,   644,   645,     0,     0,
       0,     0,   646,   647,   648,     0,     0,     0,   649,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   650,     0,   651,
     652,   653,   654,   655,   656,   657,   658,   659,   660,     0,
       0,     0,     0,     0,     0,   628,   629,     0,     0,   661,
     662,     0,     0,  1326,   630,   631,   632,   633,   634,     0,
       0,   635,   636,   637,   638,     0,   639,   640,   641,   642,
     643,     0,   644,   645,     0,     0,     0,     0,   646,   647,
     648,     0,     0,     0,   649,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   650,     0,   651,   652,   653,   654,   655,
     656,   657,   658,   659,   660,     0,     0,     0,     0,     0,
       0,   628,   629,     0,     0,   661,   662,     0,     0,  1328,
     630,   631,   632,   633,   634,     0,     0,   635,   636,   637,
     638,     0,   639,   640,   641,   642,   643,     0,   644,   645,
       0,     0,     0,     0,   646,   647,   648,     0,     0,     0,
     649,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   650,
       0,   651,   652,   653,   654,   655,   656,   657,   658,   659,
     660,     0,     0,     0,     0,     0,     0,   628,   629,     0,
       0,   661,   662,     0,     0,  1330,   630,   631,   632,   633,
     634,     0,     0,   635,   636,   637,   638,     0,   639,   640,
     641,   642,   643,     0,   644,   645,     0,     0,     0,     0,
     646,   647,   648,     0,     0,     0,   649,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   650,     0,   651,   652,   653,
     654,   655,   656,   657,   658,   659,   660,     0,     0,     0,
       0,     0,     0,   628,   629,     0,     0,   661,   662,     0,
       0,  1334,   630,   631,   632,   633,   634,     0,     0,   635,
     636,   637,   638,     0,   639,   640,   641,   642,   643,     0,
     644,   645,     0,     0,     0,     0,   646,   647,   648,     0,
       0,     0,   649,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   650,     0,   651,   652,   653,   654,   655,   656,   657,
     658,   659,   660,     0,     0,     0,     0,     0,     0,   628,
     629,     0,     0,   661,   662,     0,     0,  1383,   630,   631,
     632,   633,   634,     0,     0,   635,   636,   637,   638,     0,
     639,   640,   641,   642,   643,     0,   644,   645,     0,     0,
       0,     0,   646,   647,   648,     0,     0,     0,   649,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   650,     0,   651,
     652,   653,   654,   655,   656,   657,   658,   659,   660,     0,
       0,     0,     0,     0,     0,   628,   629,     0,     0,   661,
     662,     0,     0,  1391,   630,   631,   632,   633,   634,     0,
       0,   635,   636,   637,   638,     0,   639,   640,   641,   642,
     643,     0,   644,   645,     0,     0,     0,     0,   646,   647,
     648,     0,     0,     0,   649,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   650,     0,   651,   652,   653,   654,   655,
     656,   657,   658,   659,   660,     0,     0,     0,     0,     0,
       0,   628,   629,     0,     0,   661,   662,     0,     0,  1405,
     630,   631,   632,   633,   634,     0,     0,   635,   636,   637,
     638,     0,   639,   640,   641,   642,   643,     0,   644,   645,
       0,     0,     0,     0,   646,   647,   648,     0,     0,     0,
     649,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   650,
       0,   651,   652,   653,   654,   655,   656,   657,   658,   659,
     660,     0,     0,     0,     0,     0,     0,   628,   629,     0,
       0,   661,   662,     0,     0,  1406,   630,   631,   632,   633,
     634,     0,     0,   635,   636,   637,   638,     0,   639,   640,
     641,   642,   643,     0,   644,   645,     0,     0,     0,     0,
     646,   647,   648,     0,     0,     0,   649,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   650,     0,   651,   652,   653,
     654,   655,   656,   657,   658,   659,   660,     0,     0,     0,
       0,     0,     0,   628,   629,     0,     0,   661,   662,     0,
       0,  1408,   630,   631,   632,   633,   634,     0,     0,   635,
     636,   637,   638,     0,   639,   640,   641,   642,   643,     0,
     644,   645,     0,     0,     0,     0,   646,   647,   648,     0,
       0,     0,   649,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   650,     0,   651,   652,   653,   654,   655,   656,   657,
     658,   659,   660,     0,     0,     0,     0,     0,     0,   628,
     629,     0,     0,   661,   662,     0,     0,  1410,   630,   631,
     632,   633,   634,     0,     0,   635,   636,   637,   638,     0,
     639,   640,   641,   642,   643,     0,   644,   645,     0,     0,
       0,     0,   646,   647,   648,     0,     0,     0,   649,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   650,     0,   651,
     652,   653,   654,   655,   656,   657,   658,   659,   660,     0,
       0,     0,     0,     0,     0,   628,   629,     0,     0,   661,
     662,     0,     0,  1411,   630,   631,   632,   633,   634,     0,
       0,   635,   636,   637,   638,     0,   639,   640,   641,   642,
     643,     0,   644,   645,     0,     0,     0,     0,   646,   647,
     648,     0,     0,     0,   649,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   650,     0,   651,   652,   653,   654,   655,
     656,   657,   658,   659,   660,     0,     0,     0,     0,     0,
       0,   628,   629,     0,     0,   661,   662,     0,     0,  1413,
     630,   631,   632,   633,   634,     0,     0,   635,   636,   637,
     638,     0,   639,   640,   641,   642,   643,     0,   644,   645,
       0,     0,     0,     0,   646,   647,   648,     0,     0,     0,
     649,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   650,
       0,   651,   652,   653,   654,   655,   656,   657,   658,   659,
     660,     0,     0,     0,     0,     0,     0,   628,   629,     0,
       0,   661,   662,     0,     0,  1414,   630,   631,   632,   633,
     634,     0,     0,   635,   636,   637,   638,     0,   639,   640,
     641,   642,   643,     0,   644,   645,     0,     0,     0,     0,
     646,   647,   648,     0,     0,     0,   649,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   650,     0,   651,   652,   653,
     654,   655,   656,   657,   658,   659,   660,     0,     0,     0,
       0,     0,     0,   628,   629,     0,     0,   661,   662,     0,
       0,  1427,   630,   631,   632,   633,   634,     0,     0,   635,
     636,   637,   638,     0,   639,   640,   641,   642,   643,     0,
     644,   645,     0,     0,     0,     0,   646,   647,   648,     0,
       0,     0,   649,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   650,     0,   651,   652,   653,   654,   655,   656,   657,
     658,   659,   660,     0,     0,     0,     0,     0,     0,   628,
     629,     0,     0,   661,   662,     0,     0,  1437,   630,   631,
     632,   633,   634,     0,     0,   635,   636,   637,   638,     0,
     639,   640,   641,   642,   643,     0,   644,   645,     0,     0,
       0,     0,   646,   647,   648,     0,     0,     0,   649,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   650,     0,   651,
     652,   653,   654,   655,   656,   657,   658,   659,   660,     0,
       0,     0,     0,     0,     0,   628,   629,     0,     0,   661,
     662,     0,     0,  1520,   630,   631,   632,   633,   634,     0,
       0,   635,   636,   637,   638,     0,   639,   640,   641,   642,
     643,     0,   644,   645,     0,     0,     0,     0,   646,   647,
     648,     0,     0,     0,   649,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   650,     0,   651,   652,   653,   654,   655,
     656,   657,   658,   659,   660,     0,     0,     0,     0,   628,
     629,     0,     0,     0,     0,   661,   662,     0,     0,  1574,
     630,   631,   632,   633,   634,     0,     0,   635,   636,   637,
     638,     0,   639,   640,   641,   642,   643,     0,   644,   645,
       0,     0,     0,     0,   646,   647,   648,     0,     0,     0,
     649,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   650,
       0,   651,   652,   653,   654,   655,   656,   657,   658,   659,
     660,     0,     0,   628,   629,     0,     0,     0,     0,     0,
       0,   661,   662,   687,   630,   631,   632,   633,   634,     0,
       0,   635,   636,   637,   638,     0,   639,   640,   641,   642,
     643,     0,   644,   645,     0,     0,     0,     0,   646,   647,
     648,     0,     0,     0,   649,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   650,     0,   651,   652,   653,   654,   655,
     656,   657,   658,   659,   660,     0,     0,   628,   629,     0,
       0,     0,     0,     0,     0,   661,   662,   867,   630,   631,
     632,   633,   634,     0,     0,   635,   636,   637,   638,     0,
     639,   640,   641,   642,   643,     0,   644,   645,     0,     0,
       0,     0,   646,   647,   648,     0,     0,     0,   649,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   650,     0,   651,
     652,   653,   654,   655,   656,   657,   658,   659,   660,     0,
       0,   628,   629,     0,     0,     0,     0,     0,     0,   661,
     662,  1014,   630,   631,   632,   633,   634,     0,     0,   635,
     636,   637,   638,     0,   639,   640,   641,   642,   643,     0,
     644,   645,     0,     0,     0,     0,   646,   647,   648,     0,
       0,     0,   649,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   650,     0,   651,   652,   653,   654,   655,   656,   657,
     658,   659,   660,     0,     0,   628,   629,   185,     0,     0,
       0,     0,     0,   661,   662,  1030,   630,   631,   632,   633,
     634,     0,     0,   635,   636,   637,   638,     0,   639,   640,
     641,   642,   643,     0,   644,   645,     0,     0,     0,     0,
     646,   647,   648,   186,     0,   187,   649,   188,   189,   190,
     191,   192,     0,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,     0,   204,   205,   206,     0,     0,
     207,   208,   209,   210,     0,   650,     0,   651,   652,   653,
     654,   655,   656,   657,   658,   659,   660,     0,     0,     0,
     211,   212,     0,     0,     0,     0,     0,   661,   662,  1203,
     630,   631,   632,   633,   634,   281,   282,   635,   636,   637,
     638,     0,   639,   640,   641,   642,   643,     0,   644,   645,
       0,     0,   283,     0,   646,   647,   648,     0,     0,     0,
     649,     0,     0,     0,     0,   213,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   650,
       0,   651,   652,   653,   654,   655,   656,   657,   658,   659,
     660,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   661,   662,  1209,     0,     0,     0,     0,   628,   629,
       0,     0,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
       0,     0,   302,   303,   304,     0,     0,   305,   306,   307,
     308,   309,     0,     0,   310,   311,   312,   313,   314,   315,
     316,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   801,   317,
       0,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   628,   629,   328,   329,     0,     0,     0,     0,     0,
       0,   330,   331,   630,   631,   632,   633,   634,     0,     0,
     635,   636,   637,   638,     0,   639,   640,   641,   642,   643,
       0,   644,   645,     0,     0,   954,     0,   646,   647,   648,
     234,   235,   236,   649,   238,   239,   240,   241,   242,   438,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,   256,   257,   258,     0,     0,   261,   262,   263,
     264,     0,   650,     0,   651,   652,   653,   654,   655,   656,
     657,   658,   659,   660,   628,   629,     0,     0,     0,     0,
       0,     0,     0,     0,   661,   662,   630,   631,   632,   633,
     634,     0,     0,   635,   636,   637,   638,     0,   639,   640,
     641,   642,   643,     0,   644,   645,     0,     0,     0,     0,
     646,   647,   648,   802,     0,     0,   649,     0,     0,     0,
       0,     0,   803,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1015,   650,  1019,   651,   652,   653,
     654,   655,   656,   657,   658,   659,   660,   628,   629,     0,
       0,     0,     0,     0,     0,     0,     0,   661,   662,   630,
     631,   632,   633,   634,     0,     0,   635,   636,   637,   638,
       0,   639,   640,   641,   642,   643,     0,   644,   645,     0,
       0,     0,     0,   646,   647,   648,   234,   235,   236,   649,
     238,   239,   240,   241,   242,   438,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,   256,   257,
     258,     0,     0,   261,   262,   263,   264,     0,   650,  1159,
     651,   652,   653,   654,   655,   656,   657,   658,   659,   660,
     628,   629,     0,     0,     0,     0,     0,     0,     0,     0,
     661,   662,   630,   631,   632,   633,   634,     0,     0,   635,
     636,   637,   638,     0,   639,   640,   641,   642,   643,     0,
     644,   645,   628,   629,     0,     0,   646,   647,   648,  1016,
       0,     0,   649,     0,     0,     0,     0,     0,  1017,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   650,     0,   651,   652,   653,   654,   655,   656,   657,
     658,   659,   660,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   661,   662,   630,   631,   632,   633,   634,
       0,     0,   635,   636,   637,   638,     0,   639,   640,   641,
     642,   643,     0,   644,   645,   628,   629,     0,     0,   646,
     647,   648,     0,     0,     0,  -779,     0,   630,   631,   632,
     633,   634,     0,     0,   635,   636,   637,   638,     0,   639,
     640,   641,   642,   643,     0,   644,   645,   628,   629,     0,
       0,   646,     0,   648,   650,     0,   651,   652,   653,   654,
     655,   656,   657,   658,   659,   660,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   661,   662,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   651,   652,
     653,   654,   655,   656,   657,   658,   659,   660,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   661,   662,
     630,   631,   632,   633,   634,     0,     0,   635,   636,   637,
     638,     0,   639,   640,   641,   642,   643,     0,   644,   645,
     628,   629,     0,     0,   646,     0,     0,     0,     0,     0,
       0,     0,   630,   631,   632,   633,   634,     0,     0,   635,
     636,   637,   638,     0,   639,   640,   641,   642,   643,     0,
     644,   645,   628,   629,     0,     0,     0,     0,     0,     0,
       0,   651,   652,   653,   654,   655,   656,   657,   658,   659,
     660,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   661,   662,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   651,   652,   653,   654,   655,   656,   657,
     658,   659,   660,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   661,   662,   630,   631,   632,   633,   634,
       0,     0,   635,   636,   637,   638,     0,   639,   640,   641,
     642,   643,     0,   644,   645,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   630,   631,   632,
     633,   634,     0,     0,   635,     0,     0,   638,     0,   639,
     640,   641,   642,   643,     0,   644,   645,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   652,   653,   654,
     655,   656,   657,   658,   659,   660,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   661,   662,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   654,   655,   656,   657,   658,   659,   660,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   661,   662,
     234,   235,   236,     0,   238,   239,   240,   241,   242,   438,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,   256,   257,   258,     0,     0,   261,   262,   263,
     264,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   837,   838,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   839,     0,     0,     0,     0,     0,     0,
       0,     0,   840,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   841,   842
};

static const yytype_int16 yycheck[] =
{
       1,    14,    15,   138,   139,   176,   162,   625,   557,   553,
     701,   955,   509,   886,   784,   787,   523,   565,   465,    20,
      21,    22,   522,   615,   616,   671,   500,   673,   502,   675,
     504,   621,    80,  1094,   691,  1143,     8,   752,    33,    33,
       5,   754,  1457,   796,    22,     5,     6,   377,  1457,    50,
      63,    64,    65,  1239,    15,    16,    20,     7,  1457,  1457,
      46,    20,  1510,   176,  1457,    25,   156,     0,    20,    20,
     151,    31,   148,  1392,     7,   140,   141,   142,   151,    57,
     215,    19,    20,   163,   414,     5,     6,   163,  1503,   102,
     103,   104,   105,   154,  1503,  1543,   163,    30,   127,    32,
      50,    34,   163,   148,  1503,  1503,   186,    40,    68,    69,
    1503,   126,   129,   130,   129,   130,  1302,    50,   163,   165,
     106,   165,   203,    56,   200,   165,    62,  1446,   201,   205,
     203,   578,    21,    22,   682,   202,   201,   102,   103,   200,
     730,   588,   102,   103,   591,   174,   738,    80,   177,   139,
     742,  1566,   163,   181,   200,   200,  1491,  1566,   750,    57,
     204,   753,   206,   176,   204,    63,   715,  1566,  1566,   102,
     103,    33,   220,  1566,   621,   174,   204,    21,    22,   139,
     197,   198,   197,   198,   179,   179,   176,   107,   344,   200,
     163,   739,   165,   165,  1529,   156,   786,   173,    60,    61,
     161,  1074,   163,   163,   198,   166,   341,   185,   176,   173,
     345,   981,   869,   207,   173,   180,   969,   664,   219,   197,
     180,   173,   173,   394,   173,   163,   198,    15,    16,   118,
     119,   204,   165,   948,   206,   200,   949,   126,   198,   200,
     129,   130,   131,   132,   127,   205,   417,   418,   419,   579,
     133,     8,   127,   186,   139,   268,   163,   165,   133,  1426,
      34,  1369,   124,   593,   377,   198,   128,   924,  1435,   173,
     176,   204,   163,   179,   118,   119,   173,   412,   163,    36,
     163,   394,   126,   730,   128,   129,   130,   131,   132,    63,
    1351,   174,   200,   173,   173,   200,   179,   988,   346,   174,
     204,   414,   199,   173,   417,   418,   419,   139,   197,   198,
    1477,  1478,   163,   175,   154,   173,   864,   179,  1575,   793,
     182,   201,   201,   163,   495,   496,  1583,   173,   499,   148,
     501,   201,   503,   107,    21,    22,   156,   180,   182,   183,
     184,   185,   186,   201,   163,   207,   165,   197,   173,   173,
     940,   173,  1225,   197,   198,   201,   863,   200,   132,  1005,
     860,   201,    21,    22,   377,   139,   173,   874,   156,   173,
     163,  1062,   872,   161,   199,   163,   127,   201,   166,   201,
     929,   394,   495,   496,   714,   204,   499,   206,   501,   163,
     503,   154,   505,   180,   201,   199,   944,   727,   728,   163,
     163,   414,   127,   995,   417,   418,   419,   737,   133,   522,
    1182,   424,   425,   200,   744,   745,   190,   747,  1291,   749,
     968,   751,   173,   174,   173,   176,   200,   173,   179,   116,
     117,   118,   119,  1205,   127,   163,   173,   173,   167,   126,
     133,   128,   129,   130,   131,   132,   139,   134,   135,   174,
    1323,    33,   201,   199,   180,   127,   173,   127,  1115,   118,
     119,   133,   199,   133,  1234,   201,   579,   126,   163,   128,
     129,   130,   131,   132,   200,     5,     6,  1168,    60,    61,
     593,   174,   495,   496,   201,   620,   499,   127,   501,   163,
     503,   173,   505,   133,   180,   182,   183,   184,   185,   186,
     176,   180,   174,  1447,   174,   181,   204,   205,   139,   522,
     197,   198,   173,   684,   200,   628,   629,   148,   992,   201,
     163,   200,   153,   165,   200,   184,   185,   186,  1401,  1175,
     643,   164,   163,  1007,   174,   548,   173,   205,   197,   198,
     201,   173,   124,   164,   165,  1191,   128,    57,   661,   180,
     164,   165,   173,  1287,    47,   726,   557,   173,   729,   173,
     181,    12,   733,   177,   201,  1012,   579,   173,   173,   201,
     173,   684,    23,    24,    67,    21,    22,  1024,   139,    33,
     593,   174,  1029,   204,   140,   201,   142,   148,  1461,  1462,
     204,   167,  1140,   175,   140,   201,   201,   179,   201,   181,
     182,   714,   163,   148,   148,   618,    60,    61,   165,   622,
      63,    64,    65,   726,   727,   728,   729,   947,   163,   163,
     733,   164,   163,   953,   737,   207,  1360,  1361,   154,    57,
     173,   744,   745,   203,   747,    63,   749,   163,   751,   752,
     177,   177,  1376,  1377,   181,   181,   163,   164,   164,   102,
     103,   104,   105,  1425,  1426,   176,   173,   176,    35,  1431,
     181,   991,   181,  1435,  1436,  1538,   154,    21,    22,   154,
     124,   684,   118,   119,   128,   163,   167,   168,   163,   200,
     126,   200,   128,   129,   130,   131,   132,   858,   202,   203,
     861,   205,   693,    35,   695,  1429,  1430,  1570,  1470,   154,
    1391,   714,   176,   200,   705,  1477,  1478,   181,   163,   880,
     176,  1158,   825,   726,   727,   728,   729,   718,  1165,  1216,
     733,   175,    10,    11,   737,   179,   200,   181,   182,  1176,
     106,   744,   745,   177,   747,   203,   749,   181,   751,   752,
     163,   164,   163,   173,   180,   858,   176,   860,   861,   179,
     173,   197,   198,   207,   173,    57,   180,   176,   893,   872,
     179,    63,   116,   117,   118,   119,   120,   880,   180,   123,
     124,   125,   126,  1545,   128,   129,   130,   131,   132,    57,
     134,   135,   164,   165,    57,    63,   140,   141,   142,    75,
      63,   173,   146,    79,   795,   200,   180,  1415,    57,   181,
      57,   164,   165,   200,    63,   177,    63,    93,    94,   181,
     173,   165,    98,    99,   100,   101,   177,   200,   181,   198,
     181,   175,   204,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   180,   947,   948,   180,   164,   165,   180,
     953,   204,  1434,   197,   198,   858,   173,   860,   861,   177,
     204,   164,   165,   181,   181,    33,  1303,    33,   180,   872,
     173,   180,  1192,   180,   180,    33,   177,   880,   181,   205,
    1367,   872,  1416,   874,   140,   141,   142,   204,   991,  1050,
    1472,   205,    60,    61,    60,    61,   177,   205,    22,   890,
     181,   204,    60,    61,   177,   896,   897,   177,   181,   163,
     177,   181,   177,   163,   181,   163,   181,   199,   909,   163,
     199,   912,   913,   914,   915,   916,  1465,   163,  1536,   920,
     176,   200,   164,   165,   163,   163,  1373,   198,   929,   205,
     931,   173,   204,   201,   947,   948,   205,  1050,   163,   181,
     953,   167,   168,   169,   164,   165,   124,   176,   124,   127,
     128,   127,   128,   173,  1572,   133,   124,   133,   354,  1508,
     128,   181,   204,   180,  1120,  1412,   180,   180,   364,   167,
     168,   169,   170,   167,   168,   169,   164,   165,   991,   375,
     180,    33,   200,   180,   204,   173,    13,   165,   541,   542,
     543,   200,   200,   181,   180,   180,   174,   175,   174,   175,
     180,   179,   203,   179,   182,   180,   182,   175,    60,    61,
     200,   179,   201,   181,   182,   198,   204,    54,    55,    56,
     163,   200,  1157,  1194,   163,   199,   204,   200,   200,   207,
      19,   207,   163,   200,   200,   200,    25,  1050,   200,   207,
     200,    43,    31,   181,   200,   180,   442,   443,   180,   200,
     180,    13,    41,   199,   173,  1190,   200,   173,     4,   201,
      49,   163,   203,   459,   460,   461,   462,   463,   176,    43,
     163,   200,   124,   201,   200,    64,   128,   163,   163,  1192,
     163,  1194,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   173,   200,   180,   180,   201,
     200,   200,   518,   175,  1125,   201,    33,   179,   200,   181,
     182,   200,   200,   200,  1464,   200,   200,     1,   163,   200,
     200,   537,   181,   181,   163,   181,   163,   163,   156,    10,
     139,    13,     9,    60,    61,   207,    42,   206,    66,   174,
    1331,    43,    43,   152,   163,   561,   201,   201,   201,   200,
     180,   201,   201,   201,   163,   201,   165,   200,   200,  1192,
     200,  1194,   206,    43,   580,   581,   200,   200,   584,  1324,
     586,   200,    33,   200,    21,    22,    14,   174,   176,   156,
     596,   597,   598,   599,   600,   601,   206,   200,   206,   206,
     200,    37,   200,   202,   200,   204,   200,   124,  1331,    60,
      61,   128,     8,   163,   163,   201,   171,   200,   163,   201,
     163,   163,   200,   163,   630,   631,   163,   163,   634,   635,
     636,   637,    33,   639,    67,   641,   642,   643,   644,   645,
     646,   647,   648,   649,   650,   651,   652,   653,   654,   655,
     656,   657,   658,   659,   660,  1266,   662,  1268,   175,    60,
      61,   181,   179,   201,   181,   182,  1411,   206,  1413,  1414,
     200,   200,   200,   124,   200,   200,    70,   128,   200,   116,
     117,   118,   119,   120,   200,   163,   123,   124,   125,   126,
     207,   128,   129,   130,   131,   132,    43,   134,   135,   705,
     181,   163,   201,   138,   205,   711,   200,   200,  1331,   167,
     200,   717,   200,   164,   165,   204,   163,   204,   201,   201,
      12,   201,   173,   124,   175,    33,   732,   128,   179,   204,
     181,   182,   201,   201,   173,   173,   201,   201,   201,   205,
    1351,  1464,   179,   180,   181,   182,   183,   184,   185,   186,
     204,   201,   201,   204,   206,   201,   207,   763,   201,  1504,
     197,   198,   768,   205,   770,    53,   772,   201,   774,  1392,
     200,   200,   199,   199,   175,   206,   602,    79,   179,   785,
     181,   182,   205,   205,     1,   206,  1531,   206,    44,   795,
     130,   108,   109,   110,   111,   112,   113,   114,   115,    82,
    1582,   695,  1579,   695,   101,   622,   207,   925,   226,     1,
     626,  1321,  1424,  1421,  1355,  1560,   133,   484,  1469,  1468,
      53,  1469,  1115,  1446,   924,   924,   143,   144,   145,  1574,
     338,   424,   838,   424,  1436,    -1,   842,   580,    19,    -1,
      -1,  1464,    -1,    -1,    25,    -1,    -1,  1458,    -1,    -1,
      31,    -1,  1463,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      -1,   877,    -1,    -1,    -1,    -1,   882,    -1,   884,    -1,
      -1,   887,    -1,    64,    -1,    -1,   892,    -1,   894,    -1,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,   930,    -1,  1537,    -1,    -1,    -1,
    1541,  1542,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1554,    -1,    -1,  1557,    -1,   954,   955,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,  1573,    -1,    -1,    -1,  1577,    -1,    21,    22,
      -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,   989,   990,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1000,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1009,    -1,  1011,    -1,  1013,    -1,    -1,
      -1,    -1,    -1,  1019,    -1,    -1,    -1,  1023,    -1,    -1,
      -1,   202,    -1,    -1,    33,  1031,   207,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    61,    -1,    -1,    -1,    -1,    33,  1064,    -1,
      -1,  1067,    -1,   116,   117,   118,   119,   120,    -1,  1075,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    -1,    60,    61,    -1,   140,   141,   142,
    1096,  1097,  1098,  1099,  1100,  1101,  1102,  1103,  1104,  1105,
    1106,  1107,  1108,  1109,  1110,  1111,  1112,  1113,  1114,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,  1124,   128,
      -1,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,  1143,    -1,    10,
      -1,    -1,    -1,    -1,   197,   198,    -1,    -1,   124,    -1,
      21,    22,   128,    -1,    -1,  1161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1169,  1170,  1171,   175,    -1,    -1,    -1,
     179,    -1,   181,   182,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1200,    -1,  1202,    -1,   207,   175,
      -1,    -1,  1208,   179,    -1,   181,   182,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1223,    -1,    -1,
      -1,  1227,    -1,    -1,    -1,    -1,  1232,  1233,    -1,  1235,
    1236,   207,    -1,    -1,    21,    22,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,  1274,   140,
     141,   142,   143,   144,   145,   146,    -1,  1283,  1284,  1285,
      -1,    -1,    -1,    -1,  1290,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,  1304,  1305,
      -1,    -1,    -1,   174,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    60,    61,    -1,    -1,    -1,   197,   198,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1369,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,   128,
      -1,    -1,  1398,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1428,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1439,    -1,    -1,   175,  1443,  1444,    -1,
     179,  1447,   181,   182,  1450,    -1,   116,   117,   118,   119,
     120,  1457,    -1,   123,   124,   125,   126,  1463,   128,   129,
     130,   131,   132,    -1,   134,   135,    -1,    -1,   207,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,  1487,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1503,    -1,    -1,
      -1,    -1,    -1,  1509,    -1,   175,    -1,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,
      -1,   201,    -1,    -1,  1540,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1548,    -1,    -1,    -1,    -1,  1553,  1554,     1,
      -1,    -1,    -1,     5,     6,     7,    -1,     9,    10,    11,
    1566,    13,    -1,    15,    16,    17,    18,    19,    -1,    -1,
      -1,  1577,    -1,    25,    26,    27,    28,    29,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,
      42,    -1,    44,    45,    -1,    -1,    48,    -1,    50,    51,
      52,    -1,    54,    55,    -1,    -1,    58,    59,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,
      -1,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     182,   183,   184,    -1,   186,    -1,    -1,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,   200,    -1,
     202,   203,   204,   205,   206,     1,    -1,    -1,    -1,     5,
       6,     7,    -1,     9,    10,    11,    -1,    13,    -1,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    39,    -1,    -1,    42,    -1,    44,    45,
      -1,    -1,    48,    -1,    50,    51,    52,    -1,    54,    55,
      -1,    -1,    58,    59,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,   148,   149,   150,   151,    -1,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,
     186,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   198,    -1,   200,    -1,   202,   203,   204,   205,
     206,     1,    -1,    -1,    -1,     5,     6,     7,    -1,     9,
      10,    11,    -1,    13,    -1,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,
      -1,    -1,    42,    -1,    44,    45,    -1,    -1,    48,    -1,
      50,    51,    52,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,
     150,   151,    -1,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   182,   183,   184,    -1,   186,    -1,    -1,   189,
     190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,
     200,    -1,   202,   203,   204,   205,   206,     1,    -1,    -1,
      -1,     5,     6,     7,    -1,     9,    10,    11,    -1,    13,
      -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    29,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    42,    -1,
      44,    45,    -1,    -1,    48,    -1,    50,    51,    52,    -1,
      54,    55,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,   183,
     184,    -1,   186,    -1,    -1,   189,   190,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   198,    -1,   200,    -1,   202,   203,
     204,   205,   206,     1,    -1,    -1,    -1,     5,     6,     7,
      -1,     9,    10,    11,    -1,    13,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    29,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    39,    -1,    -1,    42,    -1,    44,    45,    -1,    -1,
      48,    -1,    50,    51,    52,    -1,    54,    55,    -1,    -1,
      58,    59,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
     148,   149,   150,   151,    -1,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,   165,   166,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   182,   183,   184,    -1,   186,    -1,
      -1,   189,   190,    -1,    -1,    -1,    -1,    60,    61,    -1,
     198,    -1,   200,    -1,   202,   203,   204,   205,   206,     5,
       6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    52,    -1,    -1,    55,
      -1,   124,    -1,    -1,    -1,   128,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
      -1,    -1,   175,    -1,    -1,    -1,   179,    -1,   181,   182,
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   207,    -1,    -1,    -1,    -1,    -1,
      -1,   147,   148,   149,   150,   151,    -1,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,
     186,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,
       5,     6,   198,    -1,   200,    -1,   202,   203,    -1,   205,
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
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,
      -1,   186,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,
      -1,     5,     6,   198,    -1,   200,   201,   202,   203,    13,
     205,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,   183,
     184,    -1,   186,    -1,    -1,   189,   190,    -1,    -1,    -1,
      -1,    -1,     5,     6,   198,    -1,   200,    -1,   202,   203,
      13,   205,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    33,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      -1,    -1,    55,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,    -1,   124,    -1,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,   175,
      -1,    -1,    -1,   179,    -1,   181,   182,    -1,    -1,   182,
     183,   184,    -1,   186,    -1,    -1,   189,   190,    -1,    -1,
      -1,    -1,    -1,     5,     6,   198,    -1,   200,    -1,   202,
     203,   207,   205,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    33,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    70,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,   124,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,
     175,    -1,    -1,    -1,   179,    -1,   181,   182,    -1,    -1,
     182,   183,   184,    -1,   186,    -1,    -1,   189,   190,    -1,
      -1,    -1,    -1,    -1,     5,     6,   198,    -1,   200,    -1,
     202,   203,   207,   205,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    33,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,
     124,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,
     151,    -1,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,   175,    -1,    -1,    -1,   179,    -1,   181,   182,    -1,
      -1,   182,   183,   184,    -1,   186,    -1,    -1,   189,   190,
      -1,    -1,    -1,    -1,    -1,     5,     6,   198,   199,   200,
      -1,   202,   203,   207,   205,    15,    16,    17,    18,    19,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   182,   183,   184,    -1,   186,    -1,    -1,   189,
     190,    -1,    -1,    -1,    -1,    -1,     5,     6,   198,    -1,
     200,    -1,   202,   203,    -1,   205,    15,    16,    17,    18,
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
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,
     149,   150,   151,    -1,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   182,   183,   184,    -1,   186,    -1,    -1,
     189,   190,    -1,    -1,    -1,    -1,    -1,     5,     6,   198,
     199,   200,    -1,   202,   203,    -1,   205,    15,    16,    17,
      18,    19,    -1,    -1,    22,    -1,    -1,    25,    -1,    27,
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
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
     148,   149,   150,   151,    -1,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   182,   183,   184,    -1,   186,    -1,
      -1,   189,   190,    -1,    -1,    -1,    -1,    -1,     5,     6,
     198,    -1,   200,    -1,   202,   203,    -1,   205,    15,    16,
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
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,   150,   151,    -1,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,   186,
      -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,     5,
       6,   198,    -1,   200,   201,   202,   203,    -1,   205,    15,
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
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,   148,   149,   150,   151,    -1,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,
     186,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,
       5,     6,   198,    -1,   200,    -1,   202,   203,    -1,   205,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,   148,   149,   150,   151,    -1,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,
      -1,   186,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,
      -1,     5,     6,   198,    -1,   200,    -1,   202,   203,    -1,
     205,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,   183,
     184,    -1,   186,    -1,    -1,   189,   190,    -1,    -1,    -1,
      -1,    -1,     5,     6,   198,    -1,   200,    -1,   202,   203,
      -1,   205,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,
     183,   184,    -1,   186,    -1,    -1,   189,   190,    -1,    -1,
      -1,    -1,    -1,     5,     6,   198,    -1,   200,    -1,   202,
     203,    -1,   205,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,
      -1,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     182,   183,   184,    -1,   186,    -1,    -1,   189,   190,    -1,
      -1,    -1,    -1,    -1,     5,     6,   198,    -1,   200,   201,
     202,   203,    -1,   205,    15,    16,    17,    18,    19,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,
     151,    -1,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   182,   183,   184,    -1,   186,    -1,    -1,   189,   190,
      -1,    -1,    -1,    -1,    -1,     5,     6,   198,    -1,   200,
     201,   202,   203,    -1,   205,    15,    16,    17,    18,    19,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,
     150,   151,    -1,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   182,   183,   184,    -1,   186,    -1,    -1,   189,
     190,    -1,    -1,    -1,    -1,    -1,     5,     6,   198,    -1,
     200,   201,   202,   203,    -1,   205,    15,    16,    17,    18,
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
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,
     149,   150,   151,    -1,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   182,   183,   184,    -1,   186,    -1,    -1,
     189,   190,    -1,    -1,    -1,    -1,    -1,     5,     6,   198,
      -1,   200,   201,   202,   203,    -1,   205,    15,    16,    17,
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
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
     148,   149,   150,   151,    -1,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   182,   183,   184,    -1,   186,    -1,
      -1,   189,   190,    -1,    -1,    -1,    -1,    -1,     5,     6,
     198,    -1,   200,   201,   202,   203,    -1,   205,    15,    16,
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
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,   150,   151,    -1,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,   186,
      -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,     5,
       6,   198,    -1,   200,    -1,   202,   203,    -1,   205,    15,
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
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,   148,   149,   150,   151,    -1,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,
     186,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,
       5,     6,   198,    -1,   200,    -1,   202,   203,    -1,   205,
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
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,   148,   149,   150,   151,    -1,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      19,   166,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,   182,   183,   184,
      -1,   186,    41,    -1,   189,   190,    -1,    -1,    -1,    -1,
      49,    -1,    -1,   198,    -1,   200,    -1,   202,   203,    -1,
     205,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,   163,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,   202,   116,   117,   118,   119,   120,    21,
      22,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,
     142,    -1,    -1,   139,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,
      -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   197,   198,    -1,    -1,    -1,
      -1,    -1,   204,    -1,    -1,    -1,   202,    21,    22,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,
     142,    71,    72,    73,   146,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,    -1,   175,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,   197,   198,    -1,    -1,   201,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,    -1,
     134,   135,    -1,    -1,   154,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,   163,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,   132,    -1,   134,   135,    -1,    -1,
      -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,
     198,    -1,    -1,   201,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,
     142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,   197,   198,    -1,    -1,   201,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,   132,    -1,   134,   135,
      -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
      -1,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,   197,   198,    -1,    -1,   201,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,   197,   198,    -1,
      -1,   201,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,    -1,
     134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,   132,    -1,   134,   135,    -1,    -1,
      -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,
     198,    -1,    -1,   201,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,
     142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,   197,   198,    -1,    -1,   201,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,   132,    -1,   134,   135,
      -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
      -1,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,   197,   198,    -1,    -1,   201,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,   197,   198,    -1,
      -1,   201,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,    -1,
     134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,   132,    -1,   134,   135,    -1,    -1,
      -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,
     198,    -1,    -1,   201,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,
     142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,   197,   198,    -1,    -1,   201,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,   132,    -1,   134,   135,
      -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
      -1,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,   197,   198,    -1,    -1,   201,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,   197,   198,    -1,
      -1,   201,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,    -1,
     134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,   132,    -1,   134,   135,    -1,    -1,
      -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,
     198,    -1,    -1,   201,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,
     142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,   197,   198,    -1,    -1,   201,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,   132,    -1,   134,   135,
      -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
      -1,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,   197,   198,    -1,    -1,   201,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,   197,   198,    -1,
      -1,   201,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,    -1,
     134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,   132,    -1,   134,   135,    -1,    -1,
      -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,
     198,    -1,    -1,   201,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,
     142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,   197,   198,    -1,    -1,   201,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,   132,    -1,   134,   135,
      -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
      -1,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,   197,   198,    -1,    -1,   201,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,   197,   198,    -1,
      -1,   201,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,    -1,
     134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,   132,    -1,   134,   135,    -1,    -1,
      -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,
     198,    -1,    -1,   201,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,
     142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,   197,   198,    -1,    -1,   201,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,   132,    -1,   134,   135,
      -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
      -1,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,   197,   198,    -1,    -1,   201,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,   197,   198,    -1,
      -1,   201,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,    -1,
     134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,   132,    -1,   134,   135,    -1,    -1,
      -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,
     198,    -1,    -1,   201,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,
     142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    -1,   197,   198,    -1,    -1,   201,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,   132,    -1,   134,   135,
      -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
      -1,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,   199,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,
     142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,   199,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,   132,    -1,   134,   135,    -1,    -1,
      -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,   199,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,    -1,
     134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    21,    22,    35,    -1,    -1,
      -1,    -1,    -1,   197,   198,   199,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,
     140,   141,   142,    71,    -1,    73,   146,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,    -1,   175,    -1,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    -1,    -1,    -1,
     118,   119,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,
     116,   117,   118,   119,   120,    21,    22,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,   132,    -1,   134,   135,
      -1,    -1,    38,    -1,   140,   141,   142,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
      -1,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,   199,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,    -1,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,   175,
      -1,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    21,    22,   189,   190,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    -1,    -1,   138,    -1,   140,   141,   142,
      71,    72,    73,   146,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,   198,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,
     140,   141,   142,   154,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,   140,   141,   142,    71,    72,    73,   146,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,    -1,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,    -1,
     134,   135,    21,    22,    -1,    -1,   140,   141,   142,   154,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,   163,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    21,    22,    -1,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    21,    22,    -1,
      -1,   140,    -1,   142,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,   132,    -1,   134,   135,
      21,    22,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,    -1,
     134,   135,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,    -1,    -1,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   209,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   165,   186,   198,   204,   211,   212,   215,
     224,   226,   227,   231,   276,   280,   309,   390,   398,   405,
     415,   462,   467,   472,    19,    20,   163,   267,   268,   269,
     156,   232,   233,   163,   186,   228,   229,    57,    63,   395,
     396,   163,   202,   214,   473,   463,   468,   139,   163,   297,
      34,    63,   107,   132,   190,   200,   271,   272,   273,   274,
     297,   211,   211,   211,     8,    36,   416,    62,   386,   174,
     173,   176,   173,   228,    22,    57,   185,   197,   230,   163,
     211,   386,   395,   395,   395,   163,   139,   225,   273,   273,
     273,   200,   140,   141,   142,   173,   199,    57,    63,   281,
     283,   406,     5,     6,   107,   412,    57,    63,   387,    15,
      16,   156,   161,   163,   166,   200,   217,   268,   156,   233,
     163,   163,   163,   397,    57,    63,   213,   163,   163,   163,
     163,   167,   223,   201,   269,   273,   273,   273,   273,   165,
     237,   238,   284,    57,    63,   399,     5,     6,    57,    63,
     413,   205,   391,   167,   168,   169,   216,    15,    16,   156,
     161,   163,   217,   265,   266,   230,   174,   164,   403,   404,
     238,   238,   167,   201,   165,    35,    71,    73,    75,    76,
      77,    78,    79,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    93,    94,    95,    98,    99,   100,
     101,   118,   119,   163,   279,   282,   163,   400,   417,   388,
     203,   270,   356,   167,   168,   169,   173,   201,    19,    25,
      31,    41,    49,    64,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   152,   202,   297,
     420,   422,   423,   426,   432,   433,   461,   164,   474,   464,
     469,    21,    22,    38,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   128,   129,   130,   133,   134,   135,   136,   137,
     140,   141,   142,   143,   144,   145,   146,   175,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   189,   190,
     197,   198,    35,    35,   200,   277,   238,   176,   401,   106,
     410,   411,   206,   211,   389,   268,   203,   163,   382,   385,
     265,   180,   180,   180,   200,   180,   180,   200,   180,   180,
     180,   180,   180,   180,   200,   297,    33,    60,    61,   124,
     128,   175,   179,   182,   207,   198,   431,   177,   205,   205,
     205,   163,   163,   163,   199,    22,   163,   199,   151,   201,
     356,   366,   367,   368,   176,   278,   288,   289,    75,    79,
      93,    94,    98,    99,   100,   101,   421,   404,   163,   238,
     356,   238,   268,   173,   176,   179,   380,   434,   439,   441,
       5,     6,    15,    16,    17,    18,    19,    25,    27,    31,
      39,    45,    48,    51,    55,    65,    68,    69,    80,   102,
     103,   104,   118,   119,   147,   148,   149,   150,   151,   153,
     155,   156,   157,   158,   159,   160,   161,   162,   166,   182,
     183,   184,   189,   190,   198,   200,   202,   203,   205,   222,
     224,   291,   297,   302,   314,   321,   324,   327,   331,   333,
     335,   336,   338,   343,   346,   347,   354,   355,   420,   477,
     485,   495,   498,   510,   514,   443,   437,   163,   427,   445,
     447,   449,   451,   453,   455,   457,   459,   347,   180,   200,
      33,   179,    33,   179,   198,   207,   199,   347,   198,   207,
     432,   475,   465,   470,   163,   108,   109,   110,   111,   112,
     113,   114,   115,   133,   143,   144,   145,   200,     7,    50,
     308,   204,   173,   204,   201,   461,   205,   205,   176,   409,
     418,   148,   163,   381,   384,   238,   163,   432,   127,   133,
     174,   379,   461,   461,   430,   461,   180,   180,   180,   299,
     422,   477,   297,   180,     5,   102,   103,   180,   200,   180,
     200,   200,   180,   180,   200,   180,   200,   180,   200,   180,
     180,   200,   180,   180,   347,   347,   200,   200,   200,   200,
     200,   200,   221,   347,   347,   347,   347,   347,    13,    49,
     294,   325,   347,   154,   163,   325,   478,   480,   203,    13,
     511,   200,   198,   275,   203,   205,   327,   332,    21,    22,
     116,   117,   118,   119,   120,   123,   124,   125,   126,   128,
     129,   130,   131,   132,   134,   135,   140,   141,   142,   146,
     175,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   197,   198,   201,   200,   461,   461,   201,   163,   424,
     461,   277,   461,   277,   461,   277,   163,   369,   370,   461,
     163,   372,   373,   201,   436,   294,   199,   199,   199,   347,
     163,   425,   369,   371,   372,   374,   347,   148,   163,   376,
     377,   419,   368,   368,   368,   293,   407,   297,   204,   205,
     414,   200,    43,   173,   176,   380,   211,   379,   347,   181,
     181,   181,   164,   173,   210,   211,   429,   486,   488,   300,
     200,   180,   200,   322,   180,   180,   180,   505,   325,   432,
     347,   494,   509,   347,   315,   317,   347,   319,   347,   507,
     325,   492,   496,   325,   490,   432,   347,   347,   347,   347,
     347,   347,   169,   170,   216,   200,    13,   199,   173,   513,
     200,   127,   133,   174,   375,   513,   173,   513,   201,   148,
     153,   180,   297,   337,   200,   238,    70,   198,   201,   325,
     480,   274,     4,   330,   203,   293,   275,    19,   154,   163,
     420,    19,   154,   163,   420,   347,   347,   347,   347,   347,
     347,   163,   347,   154,   163,   347,   347,   347,   420,   347,
     347,   347,   347,   347,   347,    22,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   129,   130,   154,
     163,   197,   198,   344,   420,   347,   201,   325,   181,   181,
     181,   204,   181,   278,   181,   278,   181,   278,   176,   181,
     429,   176,   181,   429,   296,   461,   201,   199,   210,   393,
     402,   211,   243,   244,   243,   244,   201,   200,    43,   173,
     176,   179,   375,     1,    26,    28,    29,    38,    44,    52,
      54,    58,    59,    65,   105,   211,   234,   235,   242,   253,
     254,   255,   257,   258,   259,   260,   261,   262,   263,   264,
     290,   292,   298,   303,   304,   305,   306,   307,   309,   313,
     334,   347,   163,   392,   394,   362,   347,   163,   163,   432,
     379,   347,   211,   435,   440,   442,   461,   432,   432,   461,
      70,   325,   480,   484,   163,   347,   461,   499,   501,   503,
     432,   513,   181,   429,   138,   173,   513,   201,   432,   432,
     201,   432,   201,   432,   513,   432,   370,   513,   373,   181,
     201,   201,   201,   201,   201,   201,   347,   148,   163,   200,
     256,   200,   347,   347,   347,   347,   201,   154,   163,   200,
     200,   339,   341,   256,   295,   509,   163,   201,   480,   478,
     173,   201,   201,   199,   200,   277,   206,   330,   180,   200,
     180,   200,   200,   200,   199,    19,   154,   163,   420,   176,
     154,   163,   347,   200,   200,   154,   163,   347,     1,   200,
     199,   173,   201,   444,   438,   428,   163,   446,   181,   450,
     181,   454,   181,   461,   458,   369,   461,   460,   372,   181,
     429,   163,   210,   476,   211,   369,   466,   372,   471,   347,
     163,   163,   461,   347,   127,   347,   288,    61,   347,   163,
     211,   156,    58,   347,   238,   127,   347,   211,   211,    10,
      10,    11,   240,    13,     9,    42,   211,   206,   211,   211,
     211,   211,   211,    66,   310,   211,   108,   109,   110,   111,
     112,   113,   114,   115,   121,   122,   127,   133,   136,   137,
     143,   144,   145,   174,   174,   393,   402,   165,   206,   276,
     363,   201,    43,   211,   379,   347,   211,   181,   181,   181,
     480,   201,   201,   201,   181,   429,   201,   181,   432,   370,
     373,   181,   201,   200,   432,   347,   494,   201,   181,   181,
     181,   181,   201,   181,   181,   201,   181,   330,   200,   176,
     219,   200,    43,   163,   311,    20,   173,   256,   201,   200,
     133,   375,   347,   347,   432,   277,    20,   206,   513,   201,
     173,   199,   198,   127,   133,   163,   174,   179,   328,   329,
     278,   277,   348,   347,   350,   347,   201,   325,   347,   180,
     200,   347,   200,   199,   347,   198,   201,   325,   200,   199,
     345,   201,   325,   448,   452,   456,   200,   461,   206,   206,
     206,   201,    43,   375,   347,    14,   347,   174,   176,   156,
     288,   347,   200,   200,   200,   200,   200,    37,   285,   200,
     205,   312,   384,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   392,   408,     8,   356,   361,   163,   347,   211,
     487,   489,   301,   201,   200,   163,   323,   181,   181,   181,
     506,   295,   181,   316,   318,   320,   508,   493,   497,   491,
     200,   238,   201,   325,   220,   171,   347,   163,   173,   201,
     325,   163,   200,    20,   133,   375,   347,   347,   347,   201,
     201,   181,   278,   325,   201,   478,   163,   163,   200,   163,
     163,   173,   201,   238,   278,   432,   201,   461,   201,   201,
     201,   352,   347,   347,   201,   478,   201,   347,   201,   294,
     163,   347,   288,   347,   347,   347,   256,   347,   347,   286,
     311,   383,   206,    57,    63,   359,    67,   360,   211,   211,
     200,   200,   347,   181,   500,   502,   504,   200,   201,   200,
     347,   347,   347,   200,    70,   484,   200,   200,   201,   347,
     288,   201,   218,   201,   163,   201,    43,   311,   325,   347,
     347,   201,   340,   181,   201,   199,   163,   328,   138,   288,
     326,   238,   181,   181,   461,   201,   201,   199,   201,   201,
     201,   201,    20,   201,   201,   205,   201,   206,   211,   384,
      47,   360,    46,   106,   357,   484,   484,   201,   200,   200,
     200,   200,   294,   295,   325,   484,   484,   201,   167,   204,
     163,   201,   201,   133,   375,   337,   342,   204,   201,   201,
     127,   347,   288,   349,   351,   181,   201,    12,   241,   238,
     325,   238,   238,   293,   176,   380,    33,   358,   357,   359,
     200,   478,   481,   482,   483,   483,   347,   484,   484,   478,
     479,   201,   201,   513,   483,   479,   347,   204,   347,   347,
     337,   494,   347,   353,   242,   303,   304,   305,   306,   347,
     211,   245,   246,   248,   201,   288,   288,   287,   432,   379,
     364,   358,   376,   377,   378,   478,   173,   513,   201,   201,
     201,   483,   483,   201,   201,   201,   201,   204,   512,   347,
     512,    12,    23,    24,   236,   239,   205,   242,   238,   206,
     379,   347,   282,   365,   201,   200,   201,   201,    53,   199,
     512,   206,   238,   200,   293,   211,   288,   347,   211,   211,
     282,   478,   347,   199,   249,   250,   252,   347,   247,   211,
     238,   201,   205,   242,   201,   206,   288,   293,   211,   238,
     285,   251,   249,   206,   239,   285
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   208,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   210,   210,   211,
     211,   212,   213,   213,   213,   214,   214,   215,   216,   216,
     216,   216,   217,   217,   218,   218,   219,   220,   219,   221,
     221,   221,   222,   223,   223,   225,   224,   226,   227,   228,
     228,   228,   228,   229,   229,   230,   230,   231,   232,   232,
     233,   233,   234,   235,   235,   236,   236,   237,   237,   238,
     238,   239,   239,   239,   240,   240,   241,   241,   242,   242,
     242,   242,   242,   243,   243,   244,   244,   246,   247,   245,
     248,   245,   250,   251,   249,   252,   249,   254,   253,   255,
     256,   256,   256,   256,   256,   256,   256,   258,   257,   259,
     261,   260,   263,   262,   264,   265,   265,   265,   265,   265,
     265,   266,   266,   267,   267,   267,   268,   268,   268,   268,
     268,   268,   268,   268,   269,   269,   270,   270,   271,   271,
     271,   271,   272,   272,   273,   273,   273,   273,   273,   273,
     273,   274,   274,   275,   275,   276,   276,   277,   277,   277,
     278,   278,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   280,   281,
     281,   281,   282,   284,   283,   285,   286,   287,   285,   289,
     290,   288,   291,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   293,   293,   293,   294,   294,   295,   295,   296,
     296,   297,   297,   297,   298,   298,   300,   301,   299,   299,
     302,   302,   302,   302,   302,   302,   303,   304,   305,   305,
     305,   306,   306,   307,   308,   308,   308,   309,   309,   310,
     310,   311,   311,   312,   312,   313,   313,   313,   315,   316,
     314,   317,   318,   314,   319,   320,   314,   322,   323,   321,
     324,   324,   324,   325,   325,   326,   326,   326,   327,   327,
     327,   328,   328,   328,   328,   328,   329,   329,   330,   330,
     331,   332,   332,   333,   333,   333,   333,   333,   333,   333,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   335,
     335,   336,   336,   337,   337,   338,   339,   340,   338,   341,
     342,   338,   343,   343,   343,   343,   343,   343,   343,   344,
     345,   343,   346,   346,   346,   346,   346,   346,   346,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   348,   349,   347,   347,   347,
     347,   350,   351,   347,   347,   347,   352,   353,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   354,   354,   354,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   356,   356,   357,   357,   357,   358,   358,   359,   359,
     359,   360,   360,   361,   362,   362,   363,   362,   364,   362,
     365,   362,   366,   367,   367,   368,   368,   368,   368,   368,
     369,   369,   370,   370,   371,   371,   371,   372,   373,   373,
     374,   374,   374,   375,   375,   376,   376,   376,   377,   377,
     378,   378,   379,   379,   379,   380,   380,   381,   381,   381,
     381,   381,   382,   382,   383,   383,   383,   384,   384,   384,
     385,   385,   385,   386,   386,   387,   387,   387,   388,   388,
     389,   388,   390,   391,   390,   392,   392,   393,   393,   394,
     394,   394,   395,   395,   395,   397,   396,   398,   399,   399,
     399,   400,   401,   401,   402,   402,   403,   403,   404,   404,
     406,   407,   408,   405,   409,   409,   410,   410,   411,   412,
     412,   412,   412,   413,   413,   413,   414,   414,   416,   417,
     418,   415,   419,   419,   419,   419,   419,   420,   420,   420,
     420,   420,   420,   420,   420,   420,   420,   420,   420,   420,
     420,   420,   420,   420,   420,   420,   420,   420,   420,   420,
     420,   420,   420,   420,   421,   421,   421,   421,   421,   421,
     421,   421,   422,   423,   423,   423,   424,   424,   425,   425,
     425,   427,   428,   426,   429,   429,   430,   430,   431,   431,
     431,   431,   432,   432,   433,   433,   433,   433,   434,   435,
     433,   433,   433,   436,   433,   433,   433,   433,   433,   433,
     433,   433,   433,   433,   433,   433,   433,   437,   438,   433,
     433,   439,   440,   433,   441,   442,   433,   443,   444,   433,
     433,   445,   446,   433,   447,   448,   433,   433,   449,   450,
     433,   451,   452,   433,   433,   453,   454,   433,   455,   456,
     433,   457,   458,   433,   459,   460,   433,   461,   461,   461,
     463,   464,   465,   466,   462,   468,   469,   470,   471,   467,
     473,   474,   475,   476,   472,   477,   477,   477,   477,   477,
     478,   478,   478,   478,   478,   478,   478,   478,   479,   479,
     480,   481,   481,   482,   482,   483,   483,   484,   484,   486,
     487,   485,   488,   489,   485,   490,   491,   485,   492,   493,
     485,   494,   494,   495,   496,   497,   495,   498,   499,   500,
     498,   501,   502,   498,   503,   504,   498,   498,   505,   506,
     498,   498,   507,   508,   498,   509,   509,   511,   510,   510,
     510,   510,   512,   512,   513,   513,   514,   514,   514
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     3,
       3,     2,     2,     2,     2,     2,     2,     1,     1,     1,
       1,     2,     0,     1,     1,     1,     1,     4,     1,     1,
       2,     2,     3,     2,     0,     2,     0,     0,     3,     0,
       2,     5,     3,     1,     2,     0,     4,     2,     2,     1,
       2,     3,     3,     2,     4,     0,     1,     2,     1,     3,
       1,     3,     3,     3,     2,     1,     1,     1,     2,     0,
       1,     0,     3,     7,     1,     1,     0,     2,     1,     1,
       1,     1,     1,     1,     2,     0,     1,     0,     0,     6,
       0,     3,     0,     0,     6,     0,     3,     0,     8,     7,
       1,     4,     3,     3,     3,     5,     5,     0,     9,     3,
       0,     7,     0,     7,     4,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     1,     5,     1,     3,     3,     4,     1,     1,
       1,     1,     1,     4,     1,     2,     3,     3,     3,     3,
       2,     1,     3,     0,     3,     0,     4,     0,     2,     3,
       0,     2,     1,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     3,     3,     2,     2,
       3,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     3,     2,     2,     2,     2,     2,     3,
       3,     3,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       1,     1,     3,     0,     5,     0,     0,     0,     6,     0,
       0,     6,     2,     1,     2,     2,     1,     1,     1,     1,
       2,     1,     2,     2,     2,     2,     1,     1,     1,     2,
       2,     2,     0,     2,     2,     0,     2,     0,     2,     1,
       3,     1,     3,     2,     2,     3,     0,     0,     5,     1,
       2,     5,     5,     5,     6,     2,     1,     1,     1,     2,
       3,     2,     3,     4,     1,     1,     0,     1,     1,     1,
       0,     1,     3,     8,     7,     3,     3,     5,     0,     0,
       7,     0,     0,     7,     0,     0,     7,     0,     0,     6,
       5,     8,    10,     1,     3,     1,     2,     3,     1,     1,
       2,     2,     2,     2,     2,     4,     1,     3,     0,     4,
       7,     7,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     6,
       8,     5,     6,     1,     4,     3,     0,     0,     8,     0,
       0,     9,     3,     4,     5,     6,     8,     5,     6,     0,
       0,     5,     3,     4,     4,     5,     4,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     4,
       3,     4,     5,     4,     5,     3,     4,     1,     1,     2,
       4,     4,     1,     3,     5,     0,     0,     8,     3,     3,
       3,     0,     0,     8,     3,     4,     0,     0,     9,     4,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       1,     4,     7,     8,     7,     4,     4,     4,     4,     4,
       1,     6,     7,     6,     6,     7,     7,     6,     7,     6,
       6,     0,     1,     0,     1,     1,     0,     1,     0,     1,
       1,     0,     1,     5,     0,     2,     0,     4,     0,     9,
       0,    11,     3,     3,     4,     1,     1,     3,     3,     3,
       1,     3,     1,     3,     0,     1,     3,     3,     1,     3,
       0,     1,     3,     1,     1,     1,     2,     3,     3,     5,
       1,     1,     1,     1,     1,     0,     1,     1,     4,     3,
       3,     5,     1,     3,     0,     2,     2,     4,     6,     5,
       4,     6,     5,     0,     1,     0,     1,     1,     0,     2,
       0,     4,     6,     0,     6,     1,     3,     1,     2,     0,
       1,     3,     0,     1,     1,     0,     5,     3,     0,     1,
       1,     1,     0,     2,     0,     1,     1,     2,     0,     1,
       0,     0,     0,    13,     0,     2,     0,     1,     3,     1,
       1,     2,     2,     0,     1,     1,     1,     3,     0,     0,
       0,     9,     1,     4,     3,     3,     5,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     1,     3,     0,     1,
       3,     0,     0,     6,     1,     1,     1,     3,     3,     2,
       4,     3,     1,     2,     1,     1,     1,     1,     0,     0,
       6,     4,     5,     0,     9,     4,     2,     2,     3,     2,
       3,     2,     2,     3,     3,     3,     2,     0,     0,     6,
       2,     0,     0,     6,     0,     0,     6,     0,     0,     6,
       1,     0,     0,     6,     0,     0,     7,     1,     0,     0,
       6,     0,     0,     7,     1,     0,     0,     6,     0,     0,
       7,     0,     0,     6,     0,     0,     6,     1,     3,     3,
       0,     0,     0,     0,    12,     0,     0,     0,     0,    12,
       0,     0,     0,     0,    12,     1,     1,     1,     1,     1,
       3,     3,     5,     5,     6,     6,     8,     8,     0,     1,
       2,     3,     5,     1,     2,     1,     0,     0,     1,     0,
       0,    10,     0,     0,    10,     0,     0,    10,     0,     0,
       7,     3,     1,     5,     0,     0,    10,     3,     0,     0,
      11,     0,     0,    11,     0,     0,    10,     5,     0,     0,
       9,     5,     0,     0,    10,     1,     3,     0,     5,     5,
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

    case YYSYMBOL_expression_else_block: /* expression_else_block  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_if_then_else: /* expression_if_then_else  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_if_then_else_oneliner: /* expression_if_then_else_oneliner  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_for_variable_name_with_pos_list: /* for_variable_name_with_pos_list  */
            { delete ((*yyvaluep).pNameWithPosList); }
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

    case YYSYMBOL_function_argument_declaration_no_type: /* function_argument_declaration_no_type  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_function_argument_declaration_type: /* function_argument_declaration_type  */
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

    case YYSYMBOL_variable_declaration_no_type: /* variable_declaration_no_type  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_variable_declaration_type: /* variable_declaration_type  */
            { delete ((*yyvaluep).pVarDecl); }
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
            yyextra->g_Program->library.renameModule(yyextra->g_Program->thisModule.get(),*(yyvsp[-2].s));
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

  case 34: /* format_string: %empty  */
        { (yyval.s) = new string(); }
    break;

  case 35: /* format_string: format_string STRING_CHARACTER  */
                                                 { (yyval.s) = (yyvsp[-1].s); (yyvsp[-1].s)->push_back((yyvsp[0].ch)); }
    break;

  case 36: /* optional_format_string: %empty  */
        { (yyval.s) = new string(""); }
    break;

  case 37: /* $@1: %empty  */
            { das2_strfmt(scanner); }
    break;

  case 38: /* optional_format_string: ':' $@1 format_string  */
                                                         { (yyval.s) = (yyvsp[0].s); }
    break;

  case 39: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 40: /* string_builder_body: string_builder_body character_sequence  */
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

  case 41: /* string_builder_body: string_builder_body "{" expr optional_format_string "}"  */
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

  case 42: /* string_builder: "start of the string" string_builder_body "end of the string"  */
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

  case 43: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das2_yyend_reader(scanner);
        }
    }
    break;

  case 44: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das2_yyend_reader(scanner);
        }
    }
    break;

  case 45: /* $@2: %empty  */
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

  case 46: /* expr_reader: '%' name_in_namespace $@2 reader_character_sequence  */
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

  case 47: /* options_declaration: "options" annotation_argument_list  */
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

  case 49: /* require_module_name: "name"  */
                   {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 50: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 51: /* require_module_name: require_module_name '.' "name"  */
                                                {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 52: /* require_module_name: require_module_name '/' "name"  */
                                                {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 53: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 54: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 55: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 56: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 60: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 61: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 62: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 63: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 64: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 65: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 66: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 71: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 72: /* expression_else: "else" optional_emit_semis expression_else_block  */
                                                                     { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 73: /* expression_else: elif_or_static_elif '(' expr ')' optional_emit_semis expression_else_block expression_else  */
                                                                                                                           {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),(yyvsp[-1].pExpression),(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-6].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 74: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 75: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 76: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 77: /* expression_else_one_liner: "else" expression_if_one_liner  */
                                                      {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 78: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 79: /* expression_if_one_liner: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 80: /* expression_if_one_liner: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 81: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 82: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 87: /* $@3: %empty  */
        {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 88: /* $@4: %empty  */
                                      {
        yyextra->pop_nesteds();
    }
    break;

  case 89: /* expression_if_block: $@3 '{' expressions $@4 '}' expression_block_finally  */
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

  case 90: /* $@5: %empty  */
       {
        yyextra->das_keyword = false;
    }
    break;

  case 91: /* expression_if_block: $@5 expression_if_one_liner SEMICOLON  */
                                               {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 92: /* $@6: %empty  */
        {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 93: /* $@7: %empty  */
                                      {
        yyextra->pop_nesteds();
    }
    break;

  case 94: /* expression_else_block: $@6 '{' expressions $@7 '}' expression_block_finally  */
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

  case 95: /* $@8: %empty  */
       {
        yyextra->das_keyword = false;
    }
    break;

  case 96: /* expression_else_block: $@8 expression_if_one_liner SEMICOLON  */
                                               {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 97: /* $@9: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 98: /* expression_if_then_else: $@9 if_or_static_if '(' expr ')' optional_emit_semis expression_if_block expression_else  */
                                                                                                                   {
        yyextra->das_keyword = false;
        auto blk = (yyvsp[-1].pExpression)->rtti_isBlock() ? static_cast<ExprBlock *>((yyvsp[-1].pExpression)) : ast_wrapInBlock((yyvsp[-1].pExpression));
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),blk,(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-6].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 99: /* expression_if_then_else_oneliner: expression_if_one_liner "if" '(' expr ')' expression_else_one_liner SEMICOLON  */
                                                                                                                      {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ast_wrapInBlock((yyvsp[-6].pExpression)),(yyvsp[-1].pExpression) ? ast_wrapInBlock((yyvsp[-1].pExpression)) : nullptr);
    }
    break;

  case 100: /* for_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 101: /* for_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 102: /* for_variable_name_with_pos_list: "name" "aka" "name"  */
                                         {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 103: /* for_variable_name_with_pos_list: '(' tuple_expansion ')'  */
                                       {
        auto pSL = new vector<VariableNameAndPosition>();
        for ( auto & x : *(yyvsp[-1].pNameList) ) {
            das_checkName(scanner,x,tokAt(scanner,(yylsp[-1])));
        }
        pSL->push_back(VariableNameAndPosition((yyvsp[-1].pNameList),tokAt(scanner,(yylsp[-1]))));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 104: /* for_variable_name_with_pos_list: for_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 105: /* for_variable_name_with_pos_list: for_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 106: /* for_variable_name_with_pos_list: for_variable_name_with_pos_list ',' '(' tuple_expansion ')'  */
                                                                                 {
        for ( auto & x : *(yyvsp[-1].pNameList) ) {
            das_checkName(scanner,x,tokAt(scanner,(yylsp[-1])));
        }
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition((yyvsp[-1].pNameList),tokAt(scanner,(yylsp[-1]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
    }
    break;

  case 107: /* $@10: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 108: /* expression_for_loop: $@10 "for" '(' for_variable_name_with_pos_list "in" expr_list ')' optional_emit_semis expression_block  */
                                                                                                                                     {
        yyextra->das_keyword = false;
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-5].pNameWithPosList),(yyvsp[-3].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 109: /* expression_unsafe: "unsafe" optional_emit_semis expression_block  */
                                                                    {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-2])));
        pUnsafe->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 110: /* $@11: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 111: /* expression_while_loop: $@11 "while" '(' expr ')' optional_emit_semis expression_block  */
                                                                                         {
        yyextra->das_keyword = false;
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-5])));
        pWhile->cond = (yyvsp[-3].pExpression);
        pWhile->body = (yyvsp[0].pExpression);
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 112: /* $@12: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 113: /* expression_with: $@12 "with" '(' expr ')' optional_emit_semis expression_block  */
                                                                                   {
        yyextra->das_keyword = false;
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-5])));
        pWith->with = (yyvsp[-3].pExpression);
        pWith->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pWith;
    }
    break;

  case 114: /* expression_with_alias: "assume" "name" '=' expr  */
                                                      {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-3])), *(yyvsp[-2].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-2].s);
    }
    break;

  case 115: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 116: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 117: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 118: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 119: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 120: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 121: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 122: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 123: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 124: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 125: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 126: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 127: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 128: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 129: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 130: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 131: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 132: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 133: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 134: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 135: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 136: /* metadata_argument_list: '@' annotation_argument optional_emit_semis  */
                                                         {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[-1].aa));
    }
    break;

  case 137: /* metadata_argument_list: metadata_argument_list '@' annotation_argument optional_emit_semis  */
                                                                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-3].aaList),(yyvsp[-1].aa));
    }
    break;

  case 138: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 139: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 140: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 141: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 142: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 143: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 144: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 145: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 146: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 147: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 148: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 149: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 150: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 151: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 152: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 153: /* optional_annotation_list: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 154: /* optional_annotation_list: '[' annotation_list ']'  */
                                       { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 155: /* optional_annotation_list_with_emit_semis: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 156: /* optional_annotation_list_with_emit_semis: '[' annotation_list ']' optional_emit_semis  */
                                                          { (yyval.faList) = (yyvsp[-2].faList); }
    break;

  case 157: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 158: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 159: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 160: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 161: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 162: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 163: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 164: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 165: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 166: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 167: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 168: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 169: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 170: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 171: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 172: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 173: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 174: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 175: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 176: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 177: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 178: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 179: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 180: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 181: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 182: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 183: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 184: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 185: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 186: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 187: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 188: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 189: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 190: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 191: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 192: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 193: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 194: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 195: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 196: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 197: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 198: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 199: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 200: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 201: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 202: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 203: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 204: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 205: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 206: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 207: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 208: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 209: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 210: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 211: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 212: /* function_name: "operator" '.' "name" "+="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`+="); delete (yyvsp[-1].s); }
    break;

  case 213: /* function_name: "operator" '.' "name" "-="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`-="); delete (yyvsp[-1].s); }
    break;

  case 214: /* function_name: "operator" '.' "name" "*="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`*="); delete (yyvsp[-1].s); }
    break;

  case 215: /* function_name: "operator" '.' "name" "/="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`/="); delete (yyvsp[-1].s); }
    break;

  case 216: /* function_name: "operator" '.' "name" "%="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`%="); delete (yyvsp[-1].s); }
    break;

  case 217: /* function_name: "operator" '.' "name" "&="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&="); delete (yyvsp[-1].s); }
    break;

  case 218: /* function_name: "operator" '.' "name" "|="  */
                                          { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`|="); delete (yyvsp[-1].s); }
    break;

  case 219: /* function_name: "operator" '.' "name" "^="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^="); delete (yyvsp[-1].s); }
    break;

  case 220: /* function_name: "operator" '.' "name" "&&="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&&="); delete (yyvsp[-1].s); }
    break;

  case 221: /* function_name: "operator" '.' "name" "||="  */
                                            { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`||="); delete (yyvsp[-1].s); }
    break;

  case 222: /* function_name: "operator" '.' "name" "^^="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^^="); delete (yyvsp[-1].s); }
    break;

  case 223: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 224: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 225: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 226: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 227: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 228: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 229: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 230: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 231: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 232: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 233: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 234: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 235: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 236: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 237: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 238: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 239: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 240: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 241: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 242: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 243: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 244: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 245: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 246: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 247: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 248: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 249: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 250: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 251: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 252: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 253: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 254: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 255: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 256: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 257: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 258: /* global_function_declaration: optional_annotation_list_with_emit_semis "def" function_declaration  */
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

  case 259: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 260: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 261: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 262: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 263: /* $@13: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 264: /* function_declaration: optional_public_or_private_function $@13 function_declaration_header optional_emit_semis expression_block  */
                                                                                    {
        (yyvsp[-2].pFuncDecl)->body = (yyvsp[0].pExpression);
        (yyvsp[-2].pFuncDecl)->privateFunction = !(yyvsp[-4].b);
        (yyval.pFuncDecl) = (yyvsp[-2].pFuncDecl);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
        }
    }
    break;

  case 265: /* expression_block_finally: %empty  */
        {
        (yyval.pExpression) = nullptr;
    }
    break;

  case 266: /* $@14: %empty  */
                  {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 267: /* $@15: %empty  */
                             {
        yyextra->pop_nesteds();
    }
    break;

  case 268: /* expression_block_finally: "finally" $@14 '{' expressions $@15 '}'  */
          {
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 269: /* $@16: %empty  */
        {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 270: /* $@17: %empty  */
                                      {
        yyextra->pop_nesteds();
    }
    break;

  case 271: /* expression_block: $@16 '{' expressions $@17 '}' expression_block_finally  */
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

  case 272: /* expr_call_pipe: expr_call expr_full_block_assumed_piped  */
                                                           {
        if ( (yyvsp[-1].pExpression)->rtti_isCallLikeExpr() ) {
            ((ExprLooksLikeCall *)(yyvsp[-1].pExpression))->arguments.push_back((yyvsp[0].pExpression));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
            delete (yyvsp[0].pExpression);
        }
    }
    break;

  case 273: /* expression_any: SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 274: /* expression_any: expr_assign SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 275: /* expression_any: expression_delete SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 276: /* expression_any: expression_let  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 277: /* expression_any: expression_while_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 278: /* expression_any: expression_unsafe  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 279: /* expression_any: expression_with  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 280: /* expression_any: expression_with_alias SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 281: /* expression_any: expression_for_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 282: /* expression_any: expression_break SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 283: /* expression_any: expression_continue SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 284: /* expression_any: expression_return SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 285: /* expression_any: expression_yield SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 286: /* expression_any: expression_if_then_else  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 287: /* expression_any: expression_if_then_else_oneliner  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 288: /* expression_any: expression_try_catch  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 289: /* expression_any: expression_label SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 290: /* expression_any: expression_goto SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 291: /* expression_any: "pass" SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 292: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 293: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 294: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 295: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 296: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 297: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 298: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 299: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 300: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 301: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 302: /* name_in_namespace: "name" "::" "name"  */
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

  case 303: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 304: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 305: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 306: /* $@18: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 307: /* $@19: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 308: /* new_type_declaration: '<' $@18 type_declaration '>' $@19  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 309: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 310: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 311: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 312: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 313: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 314: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 315: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 316: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 317: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 318: /* expression_return: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 319: /* expression_return: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 320: /* expression_return: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 321: /* expression_yield: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 322: /* expression_yield: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 323: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 324: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 325: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 326: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 327: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 328: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 329: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 330: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 331: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 332: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 333: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 334: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr SEMICOLON  */
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

  case 335: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 336: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 337: /* expression_let: kwd_let optional_in_scope '{' variable_declaration_list '}'  */
                                                                               {
        (yyval.pExpression) = ast_LetList(scanner,(yyvsp[-4].b),(yyvsp[-3].b),*(yyvsp[-1].pVarDeclList),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 338: /* $@20: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 339: /* $@21: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 340: /* expr_cast: "cast" '<' $@20 type_declaration_no_options '>' $@21 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
    }
    break;

  case 341: /* $@22: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 342: /* $@23: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 343: /* expr_cast: "upcast" '<' $@22 type_declaration_no_options '>' $@23 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 344: /* $@24: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 345: /* $@25: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 346: /* expr_cast: "reinterpret" '<' $@24 type_declaration_no_options '>' $@25 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 347: /* $@26: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 348: /* $@27: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 349: /* expr_type_decl: "type" '<' $@26 type_declaration '>' $@27  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 350: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
                                                                          {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-3].s),ptd->typeexpr);
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-3].s),(yyvsp[-1].pExpression));
            }
            delete (yyvsp[-3].s);
    }
    break;

  case 351: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
                                                                                                {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-6].s),ptd->typeexpr,*(yyvsp[-4].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-6].s),(yyvsp[-1].pExpression),*(yyvsp[-4].s));
            }
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
    break;

  case 352: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" c_or_s "name" '>' '(' expr ')'  */
                                                                                                                        {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-8].s),ptd->typeexpr,*(yyvsp[-6].s),*(yyvsp[-4].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-8].s),(yyvsp[-1].pExpression),*(yyvsp[-6].s),*(yyvsp[-4].s));
            }
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
    break;

  case 353: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 354: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 355: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 356: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 357: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 358: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 359: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 360: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 361: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 362: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 363: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 364: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 365: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 366: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 367: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 368: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 369: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 370: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis block_or_simple_block  */
                                                                                                                {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 371: /* expr_full_block_assumed_piped: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis expression_block  */
                                                                                                           {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 372: /* expr_full_block_assumed_piped: '{' expressions '}'  */
                                   {
        (yyval.pExpression) = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,new TypeDecl(Type::autoinfer),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 373: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 374: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 375: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 376: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 377: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 378: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 379: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 380: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 381: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 382: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 383: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 384: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 385: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 386: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 387: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 388: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 389: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 390: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 391: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 392: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 393: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 394: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 395: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 396: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 397: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 398: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
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

  case 406: /* $@28: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 407: /* $@29: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 408: /* func_addr_expr: '@' '@' '<' $@28 type_declaration_no_options '>' $@29 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 409: /* $@30: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 410: /* $@31: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 411: /* func_addr_expr: '@' '@' '<' $@30 optional_function_argument_list optional_function_type '>' $@31 func_addr_name  */
                                                                                                                                                                                     {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = (yyvsp[-3].pTypeDecl);
        if ( (yyvsp[-4].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, expr->funcType.get(), (yyvsp[-4].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-4].pVarDeclList));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 412: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 413: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
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

  case 416: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
                                                                                       {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->methodCall = true;
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        nc->nonNamedArguments.push_back((yyvsp[-7].pExpression));
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 417: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 418: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 419: /* $@32: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 420: /* $@33: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 421: /* expr_field: expr '.' $@32 error $@33  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 422: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 423: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 424: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 425: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-4])),*(yyvsp[-4].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 426: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 427: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 428: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 429: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 430: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 431: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 432: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 433: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 434: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 435: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 436: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 437: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 438: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 439: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",(yyvsp[0].pExpression)); }
    break;

  case 440: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",(yyvsp[0].pExpression)); }
    break;

  case 441: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",(yyvsp[0].pExpression)); }
    break;

  case 442: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",(yyvsp[0].pExpression)); }
    break;

  case 443: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 444: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 445: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 446: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 447: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 448: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 449: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 450: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 451: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 452: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 453: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 454: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 455: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 456: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 457: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 458: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 459: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 460: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 461: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 462: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 463: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 464: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 465: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", (yyvsp[0].pExpression)); }
    break;

  case 466: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", (yyvsp[0].pExpression)); }
    break;

  case 467: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 468: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 469: /* expr: '(' expr_list optional_comma ')'  */
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

  case 470: /* expr: '(' make_struct_single ')'  */
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

  case 471: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 472: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 473: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 474: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 475: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 476: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 477: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 478: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 479: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 480: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 481: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 482: /* expr: expr_generator  */
                                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 483: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 484: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 485: /* $@34: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 486: /* $@35: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 487: /* expr: expr "is" "type" '<' $@34 type_declaration_no_options '>' $@35  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 488: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 489: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 490: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 491: /* $@36: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 492: /* $@37: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 493: /* expr: expr "as" "type" '<' $@36 type_declaration '>' $@37  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 494: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 495: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 496: /* $@38: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 497: /* $@39: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 498: /* expr: expr '?' "as" "type" '<' $@38 type_declaration '>' $@39  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 499: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 500: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 501: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 502: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 503: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 504: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 505: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 506: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 507: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 508: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 509: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 510: /* expr: expr_call_pipe  */
                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 511: /* expr: "unsafe" '(' expr ')'  */
                                         {
            (yyvsp[-1].pExpression)->alwaysSafe = true;
            (yyvsp[-1].pExpression)->userSaidItsSafe = true;
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    break;

  case 512: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 513: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 514: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list optional_emit_semis expression_block  */
                                                                                                                                                  {
        auto closure = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),(yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->returnType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),closure,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 515: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 516: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 517: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 518: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 519: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 520: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 521: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 522: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 523: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 524: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 525: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 526: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 527: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 528: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 529: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 530: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 531: /* optional_field_annotation: %empty  */
                                      { (yyval.aaList) = nullptr; }
    break;

  case 532: /* optional_field_annotation: metadata_argument_list  */
                                      { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 533: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 534: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 535: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 536: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 537: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 538: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 539: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 540: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 541: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 542: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 543: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 544: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 545: /* struct_variable_declaration_list: struct_variable_declaration_list "new line, semicolon"  */
                                                                 { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 546: /* $@40: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 547: /* struct_variable_declaration_list: struct_variable_declaration_list $@40 structure_variable_declaration SEMICOLON  */
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

  case 548: /* $@41: %empty  */
                                                                                                                     {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 549: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable "abstract" optional_constant $@41 function_declaration_header SEMICOLON  */
                                                          {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 550: /* $@42: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 551: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@42 function_declaration_header optional_emit_semis expression_block  */
                                                                                            {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-10].pVarDeclList),(yyvsp[-9].faList),(yyvsp[-6].b),(yyvsp[-7].b),(yyvsp[-5].i),(yyvsp[-4].b),(yyvsp[-2].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-8]),(yylsp[0])),tokAt(scanner,(yylsp[-9])));
            }
    break;

  case 552: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
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

  case 553: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
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

  case 554: /* function_argument_declaration_type: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))));
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 555: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 556: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 557: /* function_argument_list: function_argument_declaration_no_type ';' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 558: /* function_argument_list: function_argument_declaration_type ';' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 559: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 560: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 561: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 562: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 563: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 564: /* tuple_alias_type_list: %empty  */
      {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 565: /* tuple_alias_type_list: tuple_type  */
                       {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 566: /* tuple_alias_type_list: tuple_alias_type_list semis tuple_type  */
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

  case 567: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 568: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 569: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 570: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 571: /* variant_alias_type_list: variant_type  */
                         {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 572: /* variant_alias_type_list: variant_alias_type_list semis variant_type  */
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

  case 573: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 574: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 575: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 576: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 577: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 578: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 579: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 580: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 581: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 582: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 583: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 584: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 585: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 586: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 587: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 588: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 589: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
                                         {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 590: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 591: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 592: /* global_let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 593: /* global_let_variable_name_with_pos_list: global_let_variable_name_with_pos_list ',' "name"  */
                                                                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 594: /* variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 595: /* variable_declaration_list: variable_declaration_list SEMICOLON  */
                                                  {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 596: /* variable_declaration_list: variable_declaration_list let_variable_declaration  */
                                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 597: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 598: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 599: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 600: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                         {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 601: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                               {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 602: /* global_let_variable_declaration: global_let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 603: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 604: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 605: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 606: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 607: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 608: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 609: /* global_variable_declaration_list: global_variable_declaration_list SEMICOLON  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 610: /* $@43: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 611: /* global_variable_declaration_list: global_variable_declaration_list $@43 optional_field_annotation let_variable_declaration  */
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

  case 612: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 613: /* $@44: %empty  */
                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 614: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@44 optional_field_annotation global_let_variable_declaration  */
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

  case 615: /* enum_expression: "name"  */
                   {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        delete (yyvsp[0].s);
    }
    break;

  case 616: /* enum_expression: "name" '=' expr  */
                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[-2].s),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-2])));
        delete (yyvsp[-2].s);
    }
    break;

  case 619: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 620: /* enum_list: enum_expression  */
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

  case 621: /* enum_list: enum_list commas enum_expression  */
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

  case 622: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 623: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 624: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 625: /* $@45: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 626: /* single_alias: optional_public_or_private_alias "name" $@45 '=' type_declaration  */
                                  {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        (yyvsp[0].pTypeDecl)->isPrivateAlias = !(yyvsp[-4].b);
        if ( (yyvsp[0].pTypeDecl)->baseType == Type::alias ) {
            das2_yyerror(scanner,"alias cannot be defined in terms of another alias "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::invalid_type);
        }
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !yyextra->g_Program->addAlias((yyvsp[0].pTypeDecl)) ) {
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

  case 628: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 629: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 630: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 631: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 632: /* optional_enum_basic_type_declaration: %empty  */
        {
        (yyval.type) = Type::tInt;
    }
    break;

  case 633: /* optional_enum_basic_type_declaration: ':' enum_basic_type_declaration  */
                                              {
        (yyval.type) = (yyvsp[0].type);
    }
    break;

  case 640: /* $@46: %empty  */
                                                                     {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 641: /* $@47: %empty  */
                                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 642: /* $@48: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 643: /* enum_declaration: optional_annotation_list_with_emit_semis "enum" $@46 optional_public_or_private_enum enum_name optional_enum_basic_type_declaration optional_emit_commas '{' $@47 enum_list optional_commas $@48 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-8].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-12].faList),tokAt(scanner,(yylsp[-12])),(yyvsp[-9].b),(yyvsp[-8].pEnum),(yyvsp[-3].pEnum),(yyvsp[-7].type));
    }
    break;

  case 644: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 645: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 646: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 647: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 648: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 649: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 650: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 651: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 652: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 653: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 654: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 655: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 656: /* optional_struct_variable_declaration_list: ';'  */
            {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 657: /* optional_struct_variable_declaration_list: '{' struct_variable_declaration_list '}'  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 658: /* $@49: %empty  */
                                                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 659: /* $@50: %empty  */
                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 660: /* $@51: %empty  */
                                             {
        if ( (yyvsp[-1].pStructure) ) {
            (yyvsp[-1].pStructure)->isClass = (yyvsp[-4].i)==CorS_Class || (yyvsp[-4].i)==CorS_ClassTemplate;
            (yyvsp[-1].pStructure)->isTemplate = (yyvsp[-4].i)==CorS_ClassTemplate || (yyvsp[-4].i)==CorS_StructTemplate;
            (yyvsp[-1].pStructure)->privateStructure = !(yyvsp[-3].b);
        }
    }
    break;

  case 661: /* structure_declaration: optional_annotation_list_with_emit_semis $@49 class_or_struct optional_public_or_private_structure $@50 structure_name optional_emit_semis $@51 optional_struct_variable_declaration_list  */
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

  case 662: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 663: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 664: /* variable_name_with_pos_list: "name" "aka" "name"  */
                                         {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 665: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 666: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 667: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 668: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 669: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 670: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 671: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 672: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 673: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 674: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 675: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 676: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 677: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 678: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 679: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 680: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 681: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 682: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 683: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 684: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 685: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 686: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 687: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 688: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 689: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 690: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 691: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 692: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 693: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 694: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 695: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 696: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 697: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 698: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 699: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 700: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 701: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 702: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 703: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 704: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 705: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 706: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 707: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 708: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 709: /* bitfield_alias_bits: "name"  */
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

  case 710: /* bitfield_alias_bits: bitfield_alias_bits commas "name"  */
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

  case 711: /* $@52: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 712: /* $@53: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 713: /* bitfield_type_declaration: "bitfield" '<' $@52 bitfield_bits '>' $@53  */
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

  case 716: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 717: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 718: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 719: /* dim_list: '[' ']'  */
                {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 720: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 721: /* dim_list: dim_list '[' ']'  */
                              {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 722: /* type_declaration_no_options: type_declaration_no_options_no_dim  */
                                                     {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 723: /* type_declaration_no_options: type_declaration_no_options_no_dim dim_list  */
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

  case 724: /* type_declaration_no_options_no_dim: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 725: /* type_declaration_no_options_no_dim: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 726: /* type_declaration_no_options_no_dim: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 727: /* type_declaration_no_options_no_dim: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 728: /* $@54: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 729: /* $@55: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 730: /* type_declaration_no_options_no_dim: "type" '<' $@54 type_declaration '>' $@55  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 731: /* type_declaration_no_options_no_dim: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 732: /* type_declaration_no_options_no_dim: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 733: /* $@56: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 734: /* type_declaration_no_options_no_dim: '$' name_in_namespace '<' $@56 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 735: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 736: /* type_declaration_no_options_no_dim: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 737: /* type_declaration_no_options_no_dim: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 738: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 739: /* type_declaration_no_options_no_dim: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 740: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 741: /* type_declaration_no_options_no_dim: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 742: /* type_declaration_no_options_no_dim: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 743: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 744: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 745: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 746: /* type_declaration_no_options_no_dim: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 747: /* $@57: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 748: /* $@58: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 749: /* type_declaration_no_options_no_dim: "smart_ptr" '<' $@57 type_declaration '>' $@58  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 750: /* type_declaration_no_options_no_dim: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 751: /* $@59: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 752: /* $@60: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 753: /* type_declaration_no_options_no_dim: "array" '<' $@59 type_declaration '>' $@60  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 754: /* $@61: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 755: /* $@62: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 756: /* type_declaration_no_options_no_dim: "table" '<' $@61 table_type_pair '>' $@62  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 757: /* $@63: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 758: /* $@64: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 759: /* type_declaration_no_options_no_dim: "iterator" '<' $@63 type_declaration '>' $@64  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 760: /* type_declaration_no_options_no_dim: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 761: /* $@65: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 762: /* $@66: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 763: /* type_declaration_no_options_no_dim: "block" '<' $@65 type_declaration '>' $@66  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 764: /* $@67: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 765: /* $@68: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 766: /* type_declaration_no_options_no_dim: "block" '<' $@67 optional_function_argument_list optional_function_type '>' $@68  */
                                                                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 767: /* type_declaration_no_options_no_dim: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 768: /* $@69: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 769: /* $@70: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 770: /* type_declaration_no_options_no_dim: "function" '<' $@69 type_declaration '>' $@70  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 771: /* $@71: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 772: /* $@72: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 773: /* type_declaration_no_options_no_dim: "function" '<' $@71 optional_function_argument_list optional_function_type '>' $@72  */
                                                                                                                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 774: /* type_declaration_no_options_no_dim: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 775: /* $@73: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 776: /* $@74: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 777: /* type_declaration_no_options_no_dim: "lambda" '<' $@73 type_declaration '>' $@74  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 778: /* $@75: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 779: /* $@76: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 780: /* type_declaration_no_options_no_dim: "lambda" '<' $@75 optional_function_argument_list optional_function_type '>' $@76  */
                                                                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 781: /* $@77: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 782: /* $@78: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 783: /* type_declaration_no_options_no_dim: "tuple" '<' $@77 tuple_type_list '>' $@78  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 784: /* $@79: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 785: /* $@80: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 786: /* type_declaration_no_options_no_dim: "variant" '<' $@79 variant_type_list '>' $@80  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 787: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 788: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 789: /* type_declaration: type_declaration '|' '#'  */
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

  case 790: /* $@81: %empty  */
                   {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 791: /* $@82: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 792: /* $@83: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 793: /* $@84: %empty  */
                                                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 794: /* tuple_alias_declaration: "tuple" $@81 optional_public_or_private_alias "name" optional_emit_semis $@82 '{' $@83 tuple_alias_type_list optional_semis $@84 '}'  */
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

  case 795: /* $@85: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 796: /* $@86: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 797: /* $@87: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 798: /* $@88: %empty  */
                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 799: /* variant_alias_declaration: "variant" $@85 optional_public_or_private_alias "name" optional_emit_semis $@86 '{' $@87 variant_alias_type_list optional_semis $@88 '}'  */
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

  case 800: /* $@89: %empty  */
                      {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 801: /* $@90: %empty  */
                                                                              {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 802: /* $@91: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 803: /* $@92: %empty  */
                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 804: /* bitfield_alias_declaration: "bitfield" $@89 optional_public_or_private_alias "name" optional_emit_commas $@90 '{' $@91 bitfield_alias_bits optional_commas $@92 '}'  */
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

  case 805: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 806: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 807: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 808: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 809: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 810: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 811: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 812: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 813: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 814: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 815: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 816: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 817: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 818: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 819: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 820: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 821: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 822: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 823: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 824: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 825: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 826: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 827: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 828: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 829: /* $@93: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 830: /* $@94: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 831: /* make_struct_decl: "struct" '<' $@93 type_declaration_no_options '>' $@94 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 832: /* $@95: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 833: /* $@96: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 834: /* make_struct_decl: "class" '<' $@95 type_declaration_no_options '>' $@96 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 835: /* $@97: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 836: /* $@98: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 837: /* make_struct_decl: "variant" '<' $@97 variant_type_list '>' $@98 '(' use_initializer make_variant_dim ')'  */
                                                                                                                                                                                  {
        auto mkt = new TypeDecl(Type::tVariant);
        mkt->at = tokAt(scanner,(yylsp[-9]));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-6].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-6].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 838: /* $@99: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 839: /* $@100: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 840: /* make_struct_decl: "default" '<' $@99 type_declaration_no_options '>' $@100 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 841: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 842: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 843: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 844: /* $@101: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 845: /* $@102: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 846: /* make_tuple_call: "tuple" '<' $@101 tuple_type_list '>' $@102 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                 {
        auto mkt = new TypeDecl(Type::tTuple);
        mkt->at = tokAt(scanner,(yylsp[-9]));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-6].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-6].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 847: /* make_dim_decl: '[' optional_expr_list ']'  */
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

  case 848: /* $@103: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 849: /* $@104: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 850: /* make_dim_decl: "array" "struct" '<' $@103 type_declaration_no_options '>' $@104 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-10]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-10])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 851: /* $@105: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 852: /* $@106: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 853: /* make_dim_decl: "array" "tuple" '<' $@105 tuple_type_list '>' $@106 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        auto mkt = new TypeDecl(Type::tTuple);
        mkt->at = tokAt(scanner,(yylsp[-10]));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-6].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-6].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-10]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-10])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 854: /* $@107: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 855: /* $@108: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 856: /* make_dim_decl: "array" "variant" '<' $@107 variant_type_list '>' $@108 '(' make_variant_dim ')'  */
                                                                                                                                                                      {
        auto mkt = new TypeDecl(Type::tVariant);
        mkt->at = tokAt(scanner,(yylsp[-9]));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-5].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-5].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
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
        mka->gen2 = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 858: /* $@109: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 859: /* $@110: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 860: /* make_dim_decl: "array" '<' $@109 type_declaration_no_options '>' $@110 '(' optional_expr_list ')'  */
                                                                                                                                                                        {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-8])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = (yyvsp[-5].pTypeDecl);
            mka->gen2 = true;
            auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_array_move");
            tam->arguments.push_back(mka);
            (yyval.pExpression) = tam;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->makeType = make_smart<TypeDecl>(Type::tArray);
            msd->makeType->firstType = (yyvsp[-5].pTypeDecl);
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
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 862: /* $@111: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 863: /* $@112: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 864: /* make_dim_decl: "fixed_array" '<' $@111 type_declaration_no_options '>' $@112 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 865: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 866: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 867: /* $@113: %empty  */
                  {
        yyextra->das_nested_parentheses ++;
    }
    break;

  case 868: /* make_table_decl: '{' $@113 optional_emit_semis optional_expr_map_tuple_list '}'  */
                                                                      {
        yyextra->das_nested_parentheses --;
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto mks = new ExprMakeStruct();
            mks->at = tokAt(scanner,(yylsp[-4]));
            mks->makeType = make_smart<TypeDecl>(Type::tTable);
            mks->makeType->firstType = make_smart<TypeDecl>(Type::autoinfer);
            mks->makeType->secondType = make_smart<TypeDecl>(Type::autoinfer);
            mks->useInitializer = true;
            mks->alwaysUseInitializer = true;
            (yyval.pExpression) = mks;
        }
    }
    break;

  case 869: /* make_table_decl: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 870: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
                                                                                                                 {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-6])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = (yyvsp[-4].pTypeDecl);
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-6]));
            msd->makeType = make_smart<TypeDecl>(Type::tTable);
            msd->makeType->firstType = (yyvsp[-4].pTypeDecl);
            msd->makeType->secondType = make_smart<TypeDecl>(Type::tVoid);
            msd->at = tokAt(scanner,(yylsp[-6]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 871: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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
            msd->makeType->firstType = (yyvsp[-6].pTypeDecl);
            msd->makeType->secondType = (yyvsp[-4].pTypeDecl);
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 872: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 873: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 874: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 875: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 876: /* array_comprehension: '[' "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                               {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 877: /* array_comprehension: '[' "iterator" "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 878: /* array_comprehension: '{' "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' make_map_tuple array_comprehension_where '}'  */
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


