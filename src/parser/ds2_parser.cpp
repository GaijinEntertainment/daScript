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
  YYSYMBOL_optional_not_required = 215,    /* optional_not_required  */
  YYSYMBOL_module_declaration = 216,       /* module_declaration  */
  YYSYMBOL_character_sequence = 217,       /* character_sequence  */
  YYSYMBOL_string_constant = 218,          /* string_constant  */
  YYSYMBOL_format_string = 219,            /* format_string  */
  YYSYMBOL_optional_format_string = 220,   /* optional_format_string  */
  YYSYMBOL_221_1 = 221,                    /* $@1  */
  YYSYMBOL_string_builder_body = 222,      /* string_builder_body  */
  YYSYMBOL_string_builder = 223,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 224, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 225,              /* expr_reader  */
  YYSYMBOL_226_2 = 226,                    /* $@2  */
  YYSYMBOL_options_declaration = 227,      /* options_declaration  */
  YYSYMBOL_require_declaration = 228,      /* require_declaration  */
  YYSYMBOL_require_module_name = 229,      /* require_module_name  */
  YYSYMBOL_optional_require_guard = 230,   /* optional_require_guard  */
  YYSYMBOL_require_module = 231,           /* require_module  */
  YYSYMBOL_is_public_module = 232,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 233,       /* expect_declaration  */
  YYSYMBOL_expect_list = 234,              /* expect_list  */
  YYSYMBOL_expect_error = 235,             /* expect_error  */
  YYSYMBOL_expression_label = 236,         /* expression_label  */
  YYSYMBOL_expression_goto = 237,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 238,      /* elif_or_static_elif  */
  YYSYMBOL_emit_semis = 239,               /* emit_semis  */
  YYSYMBOL_optional_emit_semis = 240,      /* optional_emit_semis  */
  YYSYMBOL_expression_else = 241,          /* expression_else  */
  YYSYMBOL_242_3 = 242,                    /* $@3  */
  YYSYMBOL_243_4 = 243,                    /* $@4  */
  YYSYMBOL_if_or_static_if = 244,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 245, /* expression_else_one_liner  */
  YYSYMBOL_expression_if_one_liner = 246,  /* expression_if_one_liner  */
  YYSYMBOL_semis = 247,                    /* semis  */
  YYSYMBOL_optional_semis = 248,           /* optional_semis  */
  YYSYMBOL_expression_if_block = 249,      /* expression_if_block  */
  YYSYMBOL_250_5 = 250,                    /* $@5  */
  YYSYMBOL_251_6 = 251,                    /* $@6  */
  YYSYMBOL_252_7 = 252,                    /* $@7  */
  YYSYMBOL_expression_else_block = 253,    /* expression_else_block  */
  YYSYMBOL_254_8 = 254,                    /* $@8  */
  YYSYMBOL_255_9 = 255,                    /* $@9  */
  YYSYMBOL_256_10 = 256,                   /* $@10  */
  YYSYMBOL_expression_if_then_else = 257,  /* expression_if_then_else  */
  YYSYMBOL_258_11 = 258,                   /* $@11  */
  YYSYMBOL_259_12 = 259,                   /* $@12  */
  YYSYMBOL_expression_if_then_else_oneliner = 260, /* expression_if_then_else_oneliner  */
  YYSYMBOL_for_variable_name_with_pos_list = 261, /* for_variable_name_with_pos_list  */
  YYSYMBOL_expression_for_loop = 262,      /* expression_for_loop  */
  YYSYMBOL_263_13 = 263,                   /* $@13  */
  YYSYMBOL_expression_unsafe = 264,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 265,    /* expression_while_loop  */
  YYSYMBOL_266_14 = 266,                   /* $@14  */
  YYSYMBOL_expression_with = 267,          /* expression_with  */
  YYSYMBOL_268_15 = 268,                   /* $@15  */
  YYSYMBOL_expression_with_alias = 269,    /* expression_with_alias  */
  YYSYMBOL_annotation_argument_value = 270, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 271, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 272, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 273,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 274, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 275,   /* metadata_argument_list  */
  YYSYMBOL_optional_for_annotations = 276, /* optional_for_annotations  */
  YYSYMBOL_annotation_declaration_name = 277, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 278, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 279,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 280,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 281, /* optional_annotation_list  */
  YYSYMBOL_optional_annotation_list_with_emit_semis = 282, /* optional_annotation_list_with_emit_semis  */
  YYSYMBOL_optional_function_argument_list = 283, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 284,   /* optional_function_type  */
  YYSYMBOL_function_name = 285,            /* function_name  */
  YYSYMBOL_das_type_name = 286,            /* das_type_name  */
  YYSYMBOL_optional_template = 287,        /* optional_template  */
  YYSYMBOL_global_function_declaration = 288, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 289, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 290, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 291,     /* function_declaration  */
  YYSYMBOL_292_16 = 292,                   /* $@16  */
  YYSYMBOL_expression_block_finally = 293, /* expression_block_finally  */
  YYSYMBOL_294_17 = 294,                   /* $@17  */
  YYSYMBOL_295_18 = 295,                   /* $@18  */
  YYSYMBOL_expression_block = 296,         /* expression_block  */
  YYSYMBOL_297_19 = 297,                   /* $@19  */
  YYSYMBOL_298_20 = 298,                   /* $@20  */
  YYSYMBOL_expr_call_pipe_no_bracket = 299, /* expr_call_pipe_no_bracket  */
  YYSYMBOL_expression_any = 300,           /* expression_any  */
  YYSYMBOL_301_21 = 301,                   /* $@21  */
  YYSYMBOL_302_22 = 302,                   /* $@22  */
  YYSYMBOL_expressions = 303,              /* expressions  */
  YYSYMBOL_optional_expr_list = 304,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_map_tuple_list = 305, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 306, /* type_declaration_no_options_list  */
  YYSYMBOL_name_in_namespace = 307,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 308,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 309,     /* new_type_declaration  */
  YYSYMBOL_310_23 = 310,                   /* $@23  */
  YYSYMBOL_311_24 = 311,                   /* $@24  */
  YYSYMBOL_expr_new = 312,                 /* expr_new  */
  YYSYMBOL_expression_break = 313,         /* expression_break  */
  YYSYMBOL_expression_continue = 314,      /* expression_continue  */
  YYSYMBOL_expression_return = 315,        /* expression_return  */
  YYSYMBOL_expression_yield = 316,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 317,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 318,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 319,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 320,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 321,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 322, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 323,           /* expression_let  */
  YYSYMBOL_expr_cast = 324,                /* expr_cast  */
  YYSYMBOL_325_25 = 325,                   /* $@25  */
  YYSYMBOL_326_26 = 326,                   /* $@26  */
  YYSYMBOL_327_27 = 327,                   /* $@27  */
  YYSYMBOL_328_28 = 328,                   /* $@28  */
  YYSYMBOL_329_29 = 329,                   /* $@29  */
  YYSYMBOL_330_30 = 330,                   /* $@30  */
  YYSYMBOL_expr_type_decl = 331,           /* expr_type_decl  */
  YYSYMBOL_332_31 = 332,                   /* $@31  */
  YYSYMBOL_333_32 = 333,                   /* $@32  */
  YYSYMBOL_expr_type_info = 334,           /* expr_type_info  */
  YYSYMBOL_expr_list = 335,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 336,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 337,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 338,            /* capture_entry  */
  YYSYMBOL_capture_list = 339,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 340,    /* optional_capture_list  */
  YYSYMBOL_expr_full_block = 341,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 342, /* expr_full_block_assumed_piped  */
  YYSYMBOL_expr_numeric_const = 343,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign_no_bracket = 344,   /* expr_assign_no_bracket  */
  YYSYMBOL_expr_named_call = 345,          /* expr_named_call  */
  YYSYMBOL_expr_method_call_no_bracket = 346, /* expr_method_call_no_bracket  */
  YYSYMBOL_func_addr_name = 347,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 348,           /* func_addr_expr  */
  YYSYMBOL_349_33 = 349,                   /* $@33  */
  YYSYMBOL_350_34 = 350,                   /* $@34  */
  YYSYMBOL_351_35 = 351,                   /* $@35  */
  YYSYMBOL_352_36 = 352,                   /* $@36  */
  YYSYMBOL_expr_field_no_bracket = 353,    /* expr_field_no_bracket  */
  YYSYMBOL_354_37 = 354,                   /* $@37  */
  YYSYMBOL_355_38 = 355,                   /* $@38  */
  YYSYMBOL_expr_call = 356,                /* expr_call  */
  YYSYMBOL_expr = 357,                     /* expr  */
  YYSYMBOL_expr_no_bracket = 358,          /* expr_no_bracket  */
  YYSYMBOL_359_39 = 359,                   /* $@39  */
  YYSYMBOL_360_40 = 360,                   /* $@40  */
  YYSYMBOL_361_41 = 361,                   /* $@41  */
  YYSYMBOL_362_42 = 362,                   /* $@42  */
  YYSYMBOL_363_43 = 363,                   /* $@43  */
  YYSYMBOL_364_44 = 364,                   /* $@44  */
  YYSYMBOL_expr_generator = 365,           /* expr_generator  */
  YYSYMBOL_expr_mtag_no_bracket = 366,     /* expr_mtag_no_bracket  */
  YYSYMBOL_optional_field_annotation = 367, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 368,        /* optional_override  */
  YYSYMBOL_optional_constant = 369,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 370, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 371, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 372, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 373, /* struct_variable_declaration_list  */
  YYSYMBOL_374_45 = 374,                   /* $@45  */
  YYSYMBOL_375_46 = 375,                   /* $@46  */
  YYSYMBOL_376_47 = 376,                   /* $@47  */
  YYSYMBOL_function_argument_declaration_no_type = 377, /* function_argument_declaration_no_type  */
  YYSYMBOL_function_argument_declaration_type = 378, /* function_argument_declaration_type  */
  YYSYMBOL_function_argument_list = 379,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 380,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 381,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 382,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 383,             /* variant_type  */
  YYSYMBOL_variant_type_list = 384,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 385,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 386,             /* copy_or_move  */
  YYSYMBOL_variable_declaration_no_type = 387, /* variable_declaration_no_type  */
  YYSYMBOL_variable_declaration_type = 388, /* variable_declaration_type  */
  YYSYMBOL_variable_declaration = 389,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 390,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 391,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 392, /* let_variable_name_with_pos_list  */
  YYSYMBOL_global_let_variable_name_with_pos_list = 393, /* global_let_variable_name_with_pos_list  */
  YYSYMBOL_variable_declaration_list = 394, /* variable_declaration_list  */
  YYSYMBOL_let_variable_declaration = 395, /* let_variable_declaration  */
  YYSYMBOL_global_let_variable_declaration = 396, /* global_let_variable_declaration  */
  YYSYMBOL_optional_shared = 397,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 398, /* optional_public_or_private_variable  */
  YYSYMBOL_global_variable_declaration_list = 399, /* global_variable_declaration_list  */
  YYSYMBOL_400_48 = 400,                   /* $@48  */
  YYSYMBOL_global_let = 401,               /* global_let  */
  YYSYMBOL_402_49 = 402,                   /* $@49  */
  YYSYMBOL_enum_expression = 403,          /* enum_expression  */
  YYSYMBOL_commas = 404,                   /* commas  */
  YYSYMBOL_enum_list = 405,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 406, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 407,             /* single_alias  */
  YYSYMBOL_408_50 = 408,                   /* $@50  */
  YYSYMBOL_alias_declaration = 409,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 410, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 411,                /* enum_name  */
  YYSYMBOL_optional_enum_basic_type_declaration = 412, /* optional_enum_basic_type_declaration  */
  YYSYMBOL_optional_commas = 413,          /* optional_commas  */
  YYSYMBOL_emit_commas = 414,              /* emit_commas  */
  YYSYMBOL_optional_emit_commas = 415,     /* optional_emit_commas  */
  YYSYMBOL_enum_declaration = 416,         /* enum_declaration  */
  YYSYMBOL_417_51 = 417,                   /* $@51  */
  YYSYMBOL_418_52 = 418,                   /* $@52  */
  YYSYMBOL_419_53 = 419,                   /* $@53  */
  YYSYMBOL_optional_structure_parent = 420, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 421,          /* optional_sealed  */
  YYSYMBOL_structure_name = 422,           /* structure_name  */
  YYSYMBOL_class_or_struct = 423,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 424, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 425, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 426,    /* structure_declaration  */
  YYSYMBOL_427_54 = 427,                   /* $@54  */
  YYSYMBOL_428_55 = 428,                   /* $@55  */
  YYSYMBOL_429_56 = 429,                   /* $@56  */
  YYSYMBOL_variable_name_with_pos_list = 430, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 431,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 432, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 433, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 434,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 435,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 436,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_basic_type_declaration = 437, /* bitfield_basic_type_declaration  */
  YYSYMBOL_bitfield_type_declaration = 438, /* bitfield_type_declaration  */
  YYSYMBOL_439_57 = 439,                   /* $@57  */
  YYSYMBOL_440_58 = 440,                   /* $@58  */
  YYSYMBOL_c_or_s = 441,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 442,          /* table_type_pair  */
  YYSYMBOL_dim_list = 443,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 444, /* type_declaration_no_options  */
  YYSYMBOL_optional_expr_list_in_braces = 445, /* optional_expr_list_in_braces  */
  YYSYMBOL_type_declaration_no_options_no_dim = 446, /* type_declaration_no_options_no_dim  */
  YYSYMBOL_447_59 = 447,                   /* $@59  */
  YYSYMBOL_448_60 = 448,                   /* $@60  */
  YYSYMBOL_449_61 = 449,                   /* $@61  */
  YYSYMBOL_450_62 = 450,                   /* $@62  */
  YYSYMBOL_451_63 = 451,                   /* $@63  */
  YYSYMBOL_452_64 = 452,                   /* $@64  */
  YYSYMBOL_453_65 = 453,                   /* $@65  */
  YYSYMBOL_454_66 = 454,                   /* $@66  */
  YYSYMBOL_455_67 = 455,                   /* $@67  */
  YYSYMBOL_456_68 = 456,                   /* $@68  */
  YYSYMBOL_457_69 = 457,                   /* $@69  */
  YYSYMBOL_458_70 = 458,                   /* $@70  */
  YYSYMBOL_459_71 = 459,                   /* $@71  */
  YYSYMBOL_460_72 = 460,                   /* $@72  */
  YYSYMBOL_461_73 = 461,                   /* $@73  */
  YYSYMBOL_462_74 = 462,                   /* $@74  */
  YYSYMBOL_463_75 = 463,                   /* $@75  */
  YYSYMBOL_464_76 = 464,                   /* $@76  */
  YYSYMBOL_465_77 = 465,                   /* $@77  */
  YYSYMBOL_466_78 = 466,                   /* $@78  */
  YYSYMBOL_467_79 = 467,                   /* $@79  */
  YYSYMBOL_468_80 = 468,                   /* $@80  */
  YYSYMBOL_469_81 = 469,                   /* $@81  */
  YYSYMBOL_470_82 = 470,                   /* $@82  */
  YYSYMBOL_471_83 = 471,                   /* $@83  */
  YYSYMBOL_472_84 = 472,                   /* $@84  */
  YYSYMBOL_473_85 = 473,                   /* $@85  */
  YYSYMBOL_474_86 = 474,                   /* $@86  */
  YYSYMBOL_type_declaration = 475,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 476,  /* tuple_alias_declaration  */
  YYSYMBOL_477_87 = 477,                   /* $@87  */
  YYSYMBOL_478_88 = 478,                   /* $@88  */
  YYSYMBOL_479_89 = 479,                   /* $@89  */
  YYSYMBOL_480_90 = 480,                   /* $@90  */
  YYSYMBOL_variant_alias_declaration = 481, /* variant_alias_declaration  */
  YYSYMBOL_482_91 = 482,                   /* $@91  */
  YYSYMBOL_483_92 = 483,                   /* $@92  */
  YYSYMBOL_484_93 = 484,                   /* $@93  */
  YYSYMBOL_485_94 = 485,                   /* $@94  */
  YYSYMBOL_bitfield_alias_declaration = 486, /* bitfield_alias_declaration  */
  YYSYMBOL_487_95 = 487,                   /* $@95  */
  YYSYMBOL_488_96 = 488,                   /* $@96  */
  YYSYMBOL_489_97 = 489,                   /* $@97  */
  YYSYMBOL_490_98 = 490,                   /* $@98  */
  YYSYMBOL_make_decl = 491,                /* make_decl  */
  YYSYMBOL_make_decl_no_bracket = 492,     /* make_decl_no_bracket  */
  YYSYMBOL_make_struct_fields = 493,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 494,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 495,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim_list = 496,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 497,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 498, /* optional_make_struct_dim_decl  */
  YYSYMBOL_use_initializer = 499,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 500,         /* make_struct_decl  */
  YYSYMBOL_501_99 = 501,                   /* $@99  */
  YYSYMBOL_502_100 = 502,                  /* $@100  */
  YYSYMBOL_503_101 = 503,                  /* $@101  */
  YYSYMBOL_504_102 = 504,                  /* $@102  */
  YYSYMBOL_505_103 = 505,                  /* $@103  */
  YYSYMBOL_506_104 = 506,                  /* $@104  */
  YYSYMBOL_507_105 = 507,                  /* $@105  */
  YYSYMBOL_508_106 = 508,                  /* $@106  */
  YYSYMBOL_509_107 = 509,                  /* $@107  */
  YYSYMBOL_510_108 = 510,                  /* $@108  */
  YYSYMBOL_make_tuple_call = 511,          /* make_tuple_call  */
  YYSYMBOL_512_109 = 512,                  /* $@109  */
  YYSYMBOL_513_110 = 513,                  /* $@110  */
  YYSYMBOL_make_dim_decl = 514,            /* make_dim_decl  */
  YYSYMBOL_515_111 = 515,                  /* $@111  */
  YYSYMBOL_516_112 = 516,                  /* $@112  */
  YYSYMBOL_517_113 = 517,                  /* $@113  */
  YYSYMBOL_518_114 = 518,                  /* $@114  */
  YYSYMBOL_519_115 = 519,                  /* $@115  */
  YYSYMBOL_520_116 = 520,                  /* $@116  */
  YYSYMBOL_521_117 = 521,                  /* $@117  */
  YYSYMBOL_522_118 = 522,                  /* $@118  */
  YYSYMBOL_523_119 = 523,                  /* $@119  */
  YYSYMBOL_524_120 = 524,                  /* $@120  */
  YYSYMBOL_expr_map_tuple_list = 525,      /* expr_map_tuple_list  */
  YYSYMBOL_push_table_nesting = 526,       /* push_table_nesting  */
  YYSYMBOL_make_table_decl = 527,          /* make_table_decl  */
  YYSYMBOL_make_table_call = 528,          /* make_table_call  */
  YYSYMBOL_array_comprehension_where = 529, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 530,           /* optional_comma  */
  YYSYMBOL_table_comprehension = 531,      /* table_comprehension  */
  YYSYMBOL_array_comprehension = 532       /* array_comprehension  */
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
#define YYLAST   9588

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  208
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  325
/* YYNRULES -- Number of rules.  */
#define YYNRULES  952
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1700

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
       0,   568,   568,   569,   574,   575,   576,   577,   578,   579,
     580,   581,   582,   583,   584,   585,   586,   590,   591,   595,
     596,   600,   606,   607,   608,   612,   613,   617,   618,   622,
     641,   642,   643,   644,   648,   649,   653,   654,   658,   659,
     659,   663,   668,   677,   692,   708,   713,   721,   721,   760,
     778,   782,   785,   789,   793,   797,   801,   807,   816,   817,
     821,   824,   830,   831,   835,   839,   840,   844,   847,   853,
     859,   862,   868,   869,   873,   874,   878,   879,   883,   884,
     884,   888,   888,   897,   898,   902,   903,   909,   910,   911,
     912,   913,   917,   918,   922,   923,   927,   929,   927,   941,
     941,   949,   951,   949,   963,   963,   971,   973,   971,   984,
     991,   998,  1003,  1012,  1020,  1026,  1034,  1044,  1044,  1053,
    1061,  1061,  1075,  1075,  1087,  1091,  1098,  1099,  1100,  1101,
    1102,  1103,  1107,  1112,  1120,  1121,  1122,  1126,  1127,  1128,
    1129,  1130,  1131,  1132,  1133,  1134,  1140,  1143,  1149,  1152,
    1158,  1161,  1164,  1170,  1171,  1172,  1173,  1177,  1195,  1218,
    1221,  1231,  1246,  1261,  1276,  1279,  1286,  1290,  1297,  1298,
    1302,  1303,  1307,  1308,  1309,  1313,  1316,  1320,  1327,  1331,
    1332,  1333,  1334,  1335,  1336,  1337,  1338,  1339,  1340,  1341,
    1342,  1343,  1344,  1345,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,  1353,  1354,  1355,  1356,  1357,  1358,  1359,  1360,  1361,
    1362,  1363,  1364,  1365,  1366,  1367,  1368,  1369,  1370,  1371,
    1372,  1373,  1374,  1375,  1376,  1377,  1378,  1379,  1380,  1381,
    1382,  1383,  1384,  1385,  1386,  1387,  1388,  1389,  1390,  1391,
    1392,  1393,  1394,  1395,  1396,  1397,  1398,  1399,  1400,  1401,
    1402,  1403,  1404,  1405,  1406,  1407,  1408,  1409,  1410,  1411,
    1412,  1413,  1414,  1415,  1416,  1417,  1418,  1422,  1423,  1424,
    1425,  1426,  1427,  1428,  1429,  1430,  1431,  1432,  1433,  1434,
    1435,  1436,  1437,  1438,  1439,  1440,  1441,  1442,  1443,  1444,
    1445,  1446,  1450,  1451,  1455,  1474,  1475,  1476,  1480,  1486,
    1486,  1503,  1506,  1508,  1506,  1516,  1518,  1516,  1533,  1542,
    1551,  1564,  1565,  1566,  1567,  1568,  1569,  1570,  1571,  1572,
    1573,  1574,  1575,  1576,  1577,  1578,  1579,  1580,  1581,  1582,
    1583,  1585,  1583,  1600,  1605,  1611,  1617,  1618,  1622,  1623,
    1627,  1631,  1638,  1639,  1650,  1654,  1657,  1665,  1665,  1665,
    1668,  1674,  1677,  1681,  1685,  1692,  1699,  1705,  1709,  1713,
    1716,  1719,  1727,  1730,  1738,  1744,  1745,  1746,  1750,  1751,
    1755,  1756,  1760,  1765,  1773,  1779,  1791,  1794,  1797,  1803,
    1803,  1803,  1806,  1806,  1806,  1811,  1811,  1811,  1819,  1819,
    1819,  1825,  1835,  1846,  1861,  1864,  1867,  1870,  1876,  1877,
    1884,  1895,  1896,  1897,  1901,  1902,  1903,  1904,  1905,  1909,
    1914,  1922,  1923,  1927,  1934,  1938,  1944,  1945,  1946,  1947,
    1948,  1949,  1950,  1954,  1955,  1956,  1957,  1958,  1959,  1960,
    1961,  1962,  1963,  1964,  1965,  1966,  1967,  1968,  1969,  1970,
    1971,  1972,  1973,  1974,  1978,  1984,  1995,  2001,  2012,  2016,
    2023,  2026,  2026,  2026,  2031,  2031,  2031,  2044,  2048,  2052,
    2058,  2066,  2074,  2080,  2088,  2088,  2088,  2095,  2099,  2108,
    2116,  2124,  2128,  2131,  2139,  2140,  2141,  2148,  2149,  2150,
    2151,  2152,  2153,  2154,  2155,  2156,  2157,  2158,  2159,  2160,
    2161,  2162,  2163,  2164,  2165,  2166,  2167,  2168,  2169,  2170,
    2171,  2172,  2173,  2174,  2175,  2176,  2177,  2178,  2179,  2180,
    2181,  2182,  2183,  2189,  2190,  2191,  2192,  2193,  2208,  2217,
    2218,  2219,  2220,  2221,  2222,  2223,  2224,  2225,  2226,  2227,
    2228,  2229,  2230,  2233,  2233,  2233,  2236,  2241,  2245,  2249,
    2249,  2249,  2254,  2257,  2261,  2261,  2261,  2266,  2269,  2270,
    2271,  2272,  2273,  2274,  2275,  2276,  2277,  2279,  2283,  2284,
    2289,  2295,  2301,  2310,  2313,  2316,  2325,  2326,  2327,  2328,
    2329,  2330,  2331,  2335,  2339,  2343,  2347,  2351,  2355,  2359,
    2363,  2367,  2374,  2375,  2379,  2380,  2381,  2385,  2386,  2390,
    2391,  2392,  2396,  2397,  2401,  2413,  2416,  2417,  2421,  2421,
    2440,  2439,  2454,  2453,  2470,  2482,  2491,  2501,  2502,  2503,
    2504,  2505,  2509,  2512,  2521,  2522,  2526,  2529,  2533,  2546,
    2555,  2556,  2560,  2563,  2567,  2580,  2581,  2585,  2591,  2597,
    2606,  2609,  2616,  2619,  2625,  2626,  2627,  2631,  2632,  2636,
    2643,  2648,  2657,  2663,  2674,  2681,  2690,  2693,  2696,  2703,
    2706,  2711,  2722,  2725,  2730,  2742,  2743,  2747,  2748,  2749,
    2753,  2756,  2759,  2759,  2779,  2782,  2782,  2800,  2805,  2813,
    2814,  2818,  2821,  2834,  2851,  2852,  2853,  2858,  2858,  2884,
    2888,  2889,  2890,  2894,  2904,  2907,  2913,  2914,  2918,  2919,
    2923,  2924,  2928,  2930,  2935,  2928,  2951,  2952,  2956,  2957,
    2961,  2967,  2968,  2969,  2970,  2974,  2975,  2976,  2980,  2983,
    2989,  2991,  2996,  2989,  3017,  3024,  3029,  3038,  3044,  3055,
    3056,  3057,  3058,  3059,  3060,  3061,  3062,  3063,  3064,  3065,
    3066,  3067,  3068,  3069,  3070,  3071,  3072,  3073,  3074,  3075,
    3076,  3077,  3078,  3079,  3080,  3081,  3085,  3086,  3087,  3088,
    3089,  3090,  3091,  3092,  3096,  3107,  3111,  3118,  3130,  3137,
    3143,  3152,  3157,  3167,  3177,  3187,  3200,  3201,  3202,  3203,
    3204,  3208,  3212,  3212,  3212,  3226,  3227,  3231,  3235,  3242,
    3246,  3250,  3254,  3261,  3264,  3282,  3283,  3287,  3288,  3289,
    3290,  3291,  3291,  3291,  3295,  3300,  3307,  3314,  3314,  3321,
    3321,  3328,  3332,  3336,  3341,  3346,  3351,  3356,  3360,  3364,
    3369,  3373,  3377,  3382,  3382,  3382,  3388,  3395,  3395,  3395,
    3400,  3400,  3400,  3406,  3406,  3406,  3411,  3416,  3416,  3416,
    3421,  3421,  3421,  3430,  3435,  3435,  3435,  3440,  3440,  3440,
    3449,  3454,  3454,  3454,  3459,  3459,  3459,  3468,  3468,  3468,
    3474,  3474,  3474,  3483,  3486,  3497,  3513,  3515,  3520,  3525,
    3513,  3551,  3553,  3558,  3564,  3551,  3590,  3592,  3597,  3602,
    3590,  3643,  3644,  3645,  3646,  3647,  3648,  3649,  3653,  3654,
    3655,  3656,  3657,  3661,  3668,  3675,  3681,  3687,  3694,  3701,
    3707,  3716,  3719,  3725,  3733,  3738,  3745,  3750,  3756,  3757,
    3761,  3762,  3766,  3766,  3766,  3774,  3774,  3774,  3781,  3781,
    3781,  3792,  3792,  3792,  3799,  3799,  3799,  3810,  3816,  3816,
    3816,  3830,  3849,  3849,  3849,  3859,  3859,  3859,  3873,  3873,
    3873,  3887,  3896,  3896,  3896,  3916,  3923,  3923,  3923,  3933,
    3936,  3947,  3953,  3976,  3984,  4004,  4029,  4030,  4034,  4035,
    4040,  4043,  4053
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
  "optional_public_or_private_module", "module_name",
  "optional_not_required", "module_declaration", "character_sequence",
  "string_constant", "format_string", "optional_format_string", "$@1",
  "string_builder_body", "string_builder", "reader_character_sequence",
  "expr_reader", "$@2", "options_declaration", "require_declaration",
  "require_module_name", "optional_require_guard", "require_module",
  "is_public_module", "expect_declaration", "expect_list", "expect_error",
  "expression_label", "expression_goto", "elif_or_static_elif",
  "emit_semis", "optional_emit_semis", "expression_else", "$@3", "$@4",
  "if_or_static_if", "expression_else_one_liner",
  "expression_if_one_liner", "semis", "optional_semis",
  "expression_if_block", "$@5", "$@6", "$@7", "expression_else_block",
  "$@8", "$@9", "$@10", "expression_if_then_else", "$@11", "$@12",
  "expression_if_then_else_oneliner", "for_variable_name_with_pos_list",
  "expression_for_loop", "$@13", "expression_unsafe",
  "expression_while_loop", "$@14", "expression_with", "$@15",
  "expression_with_alias", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "metadata_argument_list", "optional_for_annotations",
  "annotation_declaration_name", "annotation_declaration_basic",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
  "optional_annotation_list_with_emit_semis",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "das_type_name", "optional_template",
  "global_function_declaration", "optional_public_or_private_function",
  "function_declaration_header", "function_declaration", "$@16",
  "expression_block_finally", "$@17", "$@18", "expression_block", "$@19",
  "$@20", "expr_call_pipe_no_bracket", "expression_any", "$@21", "$@22",
  "expressions", "optional_expr_list", "optional_expr_map_tuple_list",
  "type_declaration_no_options_list", "name_in_namespace",
  "expression_delete", "new_type_declaration", "$@23", "$@24", "expr_new",
  "expression_break", "expression_continue", "expression_return",
  "expression_yield", "expression_try_catch", "kwd_let_var_or_nothing",
  "kwd_let", "optional_in_scope", "tuple_expansion",
  "tuple_expansion_variable_declaration", "expression_let", "expr_cast",
  "$@25", "$@26", "$@27", "$@28", "$@29", "$@30", "expr_type_decl", "$@31",
  "$@32", "expr_type_info", "expr_list", "block_or_simple_block",
  "block_or_lambda", "capture_entry", "capture_list",
  "optional_capture_list", "expr_full_block",
  "expr_full_block_assumed_piped", "expr_numeric_const",
  "expr_assign_no_bracket", "expr_named_call",
  "expr_method_call_no_bracket", "func_addr_name", "func_addr_expr",
  "$@33", "$@34", "$@35", "$@36", "expr_field_no_bracket", "$@37", "$@38",
  "expr_call", "expr", "expr_no_bracket", "$@39", "$@40", "$@41", "$@42",
  "$@43", "$@44", "expr_generator", "expr_mtag_no_bracket",
  "optional_field_annotation", "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@45", "$@46", "$@47",
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
  "global_variable_declaration_list", "$@48", "global_let", "$@49",
  "enum_expression", "commas", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@50",
  "alias_declaration", "optional_public_or_private_enum", "enum_name",
  "optional_enum_basic_type_declaration", "optional_commas", "emit_commas",
  "optional_emit_commas", "enum_declaration", "$@51", "$@52", "$@53",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@54", "$@55", "$@56", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_alias_bits", "bitfield_basic_type_declaration",
  "bitfield_type_declaration", "$@57", "$@58", "c_or_s", "table_type_pair",
  "dim_list", "type_declaration_no_options",
  "optional_expr_list_in_braces", "type_declaration_no_options_no_dim",
  "$@59", "$@60", "$@61", "$@62", "$@63", "$@64", "$@65", "$@66", "$@67",
  "$@68", "$@69", "$@70", "$@71", "$@72", "$@73", "$@74", "$@75", "$@76",
  "$@77", "$@78", "$@79", "$@80", "$@81", "$@82", "$@83", "$@84", "$@85",
  "$@86", "type_declaration", "tuple_alias_declaration", "$@87", "$@88",
  "$@89", "$@90", "variant_alias_declaration", "$@91", "$@92", "$@93",
  "$@94", "bitfield_alias_declaration", "$@95", "$@96", "$@97", "$@98",
  "make_decl", "make_decl_no_bracket", "make_struct_fields",
  "make_variant_dim", "make_struct_single", "make_struct_dim_list",
  "make_struct_dim_decl", "optional_make_struct_dim_decl",
  "use_initializer", "make_struct_decl", "$@99", "$@100", "$@101", "$@102",
  "$@103", "$@104", "$@105", "$@106", "$@107", "$@108", "make_tuple_call",
  "$@109", "$@110", "make_dim_decl", "$@111", "$@112", "$@113", "$@114",
  "$@115", "$@116", "$@117", "$@118", "$@119", "$@120",
  "expr_map_tuple_list", "push_table_nesting", "make_table_decl",
  "make_table_call", "array_comprehension_where", "optional_comma",
  "table_comprehension", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1522)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-845)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1522,    70, -1522, -1522,    59,   -55,    15,   324, -1522,   -87,
   -1522, -1522, -1522, -1522,   298,    24, -1522, -1522, -1522, -1522,
     134,   134,   134, -1522,    89, -1522,   153, -1522, -1522, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522,    49, -1522,    88,
      95,   110, -1522,   176,   171, -1522, -1522, -1522,   186,   134,
   -1522, -1522,   153,   324,   324,   324,   235,   177, -1522, -1522,
   -1522, -1522,    24,    24,    24,   160, -1522,   851,   191, -1522,
   -1522, -1522, -1522,   312, -1522,   102, -1522,   605,   142,    59,
     301,   -55, -1522,   290, -1522,   280,   346,    35, -1522, -1522,
     669,   375,   448,   462, -1522,   480,   388, -1522, -1522,   -59,
      59,    24,    24,    24,    24,   518, -1522,   726,   727,   569,
     601,   739, -1522, -1522,   546, -1522, -1522, -1522, -1522, -1522,
     842,   162,   566, -1522, -1522, -1522, -1522,   171,   171,   577,
     171,   628, -1522,   687,   694, -1522,   651, -1522, -1522,   697,
     712,   518,   518, -1522, -1522,   677, -1522,    -4, -1522,   292,
     758,   851, -1522,   735, -1522, -1522, -1522, -1522, -1522, -1522,
   -1522,   742, -1522, -1522, -1522, -1522, -1522, -1522,   703, -1522,
   -1522, -1522,   856, -1522, -1522, -1522, -1522, -1522, -1522, -1522,
      72,   749,   577,   577,   577,   872, -1522, -1522,  8446,   879,
   -1522,   602,   783, -1522, -1522, -1522, -1522, -1522,  9216, -1522,
     780,   854,   360,    59,   759,   802, -1522, -1522, -1522,   162,
   -1522, -1522, -1522,   788,   794,   800,   782,   804,   825, -1522,
   -1522, -1522,   789, -1522, -1522, -1522, -1522, -1522,   288, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522,
     827, -1522, -1522, -1522,   837,   838, -1522, -1522, -1522, -1522,
     846,   848,   795,   298,    -6, -1522, -1522, -1522, -1522,   426,
     831,   865, -1522, -1522, -1522, -1522, -1522, -1522,   867, -1522,
     797,   843,  8634, -1522, -1522, -1522, -1522, -1522, -1522, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522,  1012,  1016,
   -1522,   853, -1522,   518,   586,   783, -1522,   906,   518, -1522,
   -1522,   703,   518,    59, -1522,   490, -1522, -1522, -1522, -1522,
   -1522,  7353, -1522, -1522,   907,   891,   -63,   -60,   -52, -1522,
   -1522,  7353,   203, -1522,  5252, -1522, -1522, -1522,    12, -1522,
   -1522, -1522,     5, -1522,  5443,   875,  8257, -1522,   869, -1522,
   -1522,  9354,  9393, -1522, -1522, -1522, -1522, -1522, -1522, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522,
   -1522, -1522, -1522,   912,   877, -1522, -1522, -1522, -1522, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522,  1056, -1522, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522,
     917,   882, -1522, -1522,   -84,    53,   944, -1522, -1522, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522,   878,   908, -1522,   -49,
   -1522,   518,   923,  8446, -1522,   197,  8446,  8446,  8446,   913,
     920, -1522, -1522,   253,   298,   925,    41, -1522,   361,   892,
     934,   935,   916,   937,   921,   363,   942, -1522,   438,    25,
     945,  8111,  8111,   927,   928,   929,   930,   931,   932, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522,  8111,  8111,
    8111,  8111,  8111,  3724,  4106, -1522,   933, -1522, -1522, -1522,
   -1522,   938, -1522, -1522, -1522, -1522,   936, -1522, -1522, -1522,
     715, -1522,   715,   715,   940,  1151, -1522, -1522,   939, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522,  8446,  8446,   941,
     952,  8446,   853,  8446,   853,  8446,   853,  8539,   980,   943,
   -1522,  5252, -1522,  8446,  7353,   947,   972, -1522, -1522, -1522,
   -1522, -1522,   948, -1522, -1522,   950,  5634, -1522,   426, -1522,
    8539,   980, -1522, -1522, -1522, -1522, -1522, -1522,  9425,   946,
     781,   954, -1522,   131,   962,    91,   949,  8446,  8446, -1522,
    7733, -1522,   963, -1522, -1522,   298, -1522,   486,   967,  1103,
     585, -1522, -1522, -1522,   122, -1522, -1522, -1522,  7353,   530,
     654,   971,   395, -1522, -1522, -1522, -1522,   969, -1522, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522,   439, -1522,   990,
     994,   997, -1522,  5252,  8446,  7353,  7353, -1522, -1522,  7353,
   -1522,  7353, -1522,  5252, -1522, -1522,  5252,   998, -1522,  8446,
     491,   491,  7353,  7353,  7353,  7353,  7353,  7353,   809,   491,
     491,   113,   491,   491,   979,  1158,   981,   982,   167,   972,
    1008,   985,   519,   518,  3342,    24,  1183,   987, -1522,   936,
   -1522, -1522, -1522, -1522,  1838,  8628,  8111,  8111, -1522, -1522,
    8111,  8111,  8111,  8111,  1025,  8111,   539,  7353,  8111,  8111,
    8111,  8111,  7353,  8111,  8111,  8111,  8111,  7922,  8111,  8111,
    8111,  8111,  8111,  8111,  8111,  8111,  8111,  8111,  9265,  7353,
    4297,   662,   668, -1522, -1522,  1028,   678,    53,   679,    53,
     681,    53,   -17, -1522,   549,   865,  1017, -1522,   556, -1522,
    8446,   972,   563,   865, -1522, -1522,  5825, -1522, -1522, -1522,
   -1522,  1000,  1037, -1522,   134, -1522,   134, -1522, -1522, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522,
   -1522, -1522, -1522, -1522, -1522,  7353, -1522, -1522,   440,   -56,
     -56,   -56, -1522,   865,   865,  8111,  8710, -1522,  1038, -1522,
   -1522, -1522, -1522,  7353,  1039,  1040,  8446,   197, -1522,  7353,
     134, -1522, -1522, -1522, -1522, -1522, -1522, -1522,  8446,  8446,
    8446,  8446,  3915,  1043,  7353,  8446, -1522, -1522, -1522,  8446,
     972,   220, -1522,  1034,  1007,  8446,  8446,  1013,  8446,  1014,
    8446,   972,  8446,  8539,   972, -1522,   980,   791,  1015,  1018,
    1019,  1020,  1021,  1022, -1522,  7353,   642,   -54,  1009, -1522,
    7353, -1522,  7353, -1522,  7353,  1023,   580, -1522, -1522,  1026,
    1027,   -46, -1522, -1522,  6016,   173,  3533, -1522,    84,  1024,
     271,  1029,   853, -1522,  2061,  1183,  1033,  1030, -1522, -1522,
    1048,  1031, -1522, -1522,  1066,  1066,  1190,  1190,  1641,  1641,
    1036,   310,  1041, -1522,  1035,   319,   319,   939,  1066,  1066,
    8710, -1522, -1522,  8859,  8744,  8832,  8710,  9127,   688,  8947,
    8974,  9062,  1190,  1190,   451,   451,   310,   310,   310,   593,
    7353,  1045,  1046,   617,  7353,  1217,  1047,  1054, -1522,   164,
   -1522, -1522, -1522,   274, -1522,  1058, -1522,  1074, -1522,  1075,
    8446, -1522,  8539,  8446, -1522,   980,   673,  1057,  1060,  8446,
    7353, -1522, -1522,  1087,   307, -1522,  8350, -1522,   214, -1522,
    1064,  1072,  1223, -1522, -1522,   103, -1522, -1522, -1522,   978,
    2315,  1099, -1522,   307,    33,  1086, -1522,  1251,   122,  7353,
     134, -1522, -1522, -1522, -1522,   865,   815,  1172,   701,   618,
     190,  1094,  1097,   757,  1098,   702,  8446,  8539,   980,  1177,
    1101,  1112,  8446,  7353,  1102, -1522,  1370,  1383, -1522,  1404,
   -1522,  1413,  1122,  1442,   763,  1126,  8446,   767,  1183, -1522,
   -1522, -1522, -1522, -1522,  1113,  1162,  1139,  1298,  1179,    40,
     -54,  1142, -1522, -1522, -1522,  1144,   329,  7353,  7353,  8446,
     853,  1145,  1140,  1034,   355, -1522,  1149,   294,  6207, -1522,
   -1522, -1522,   415,    53, -1522,  6398, -1522, -1522,  6589,  1192,
    1194, -1522,   134,  1204,  6780,   -67,  6971, -1522, -1522, -1522,
     134,   134,  1351, -1522,   822, -1522, -1522,  1349, -1522, -1522,
    1354, -1522,  1322,   134, -1522,   134,   134,   134,   134,   134,
   -1522,  1300, -1522,   134,  1840,   853, -1522,  7353, -1522,  7353,
    4488,  7353, -1522,  1187,  1168, -1522, -1522,  8111,  1170, -1522,
    1178,  7353,  4679,  1171, -1522,  1181, -1522,  4870, -1522,  5825,
   -1522, -1522, -1522,  1215, -1522,  1218, -1522, -1522, -1522, -1522,
   -1522, -1522,   865, -1522, -1522,   865, -1522, -1522,  1060, -1522,
   -1522,   865, -1522,  7353, -1522,   575, -1522, -1522, -1522,  1176,
   -1522,  1180, -1522,  7353,  1222,  1226,  8446, -1522,  7353,  1184,
    7353,   582, -1522,  1228, -1522, -1522,  1384,   703, -1522,  1231,
   -1522,  7353,   134, -1522, -1522, -1522, -1522,  1195, -1522, -1522,
   -1522,  1197,  1232, -1522, -1522,  1475,   777,   839, -1522, -1522,
    7353,  1609, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522,
   -1522, -1522,  1625, -1522,   107,  5061, -1522,  1227,  7353,  1236,
   -1522,   262,  5252,     4,    44,   356,  7353,  7353,  7353,  1199,
    1200,  1874,    53,   -54, -1522, -1522, -1522,   580,  1201,  3533,
    1241,  1242,  1206,  1244,  1245, -1522,   273,   518,  7353, -1522,
    1395,  7353, -1522,  1237,  1238, -1522,  1239,  1258, -1522, -1522,
    7353, -1522, -1522, -1522, -1522,  1219, -1522, -1522,  1220,   -92,
     -92,  1221, -1522, -1522, -1522, -1522, -1522, -1522, -1522,   -77,
   -1522,  8111,  8111,  8111,  8111,  8111,  8111,  8111,  8111,  8111,
    8111,  7353,  8111,  8111,  8111,  8111,  8111,  8111,  8111,    53,
    8446,  1224,  8446,  1233, -1522,   308,  1234, -1522,  7353,   978,
    7353, -1522,  1235,  3533, -1522,   317,  7353, -1522, -1522, -1522,
     350, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522,  1248,
   -1522,  1212, -1522, -1522,  1240, -1522,  1380,   135, -1522,  1387,
   -1522, -1522,  1243,  1255,   764,  1365,   134, -1522,   134, -1522,
    1247,  1250, -1522, -1522,  7353,  1252, -1522, -1522, -1522, -1522,
    1253,  1254,  1257,  8111,  8111,  8111,  1259,  1369,  1262, -1522,
    1263,  7162, -1522, -1522,   366, -1522, -1522,  1265, -1522,  1277,
   -1522,   373,  1399,  1179,  5252,  7353,  7353,  1267, -1522, -1522,
   -1522, -1522, -1522,  1270,    45, -1522,   341, -1522, -1522,  1289,
   -1522, -1522,   415, -1522,   944, -1522, -1522, -1522,  8446,  7353,
   -1522, -1522, -1522, -1522,  2521,  7353,  7353,    59,   759,  1269,
    1271,  7353,  1179, -1522, -1522, -1522,  1151,  1151,  1151,  1151,
    1151,  1151,  1151,  1151,  1151,  1151,  1151, -1522, -1522,  1151,
    1151,  1151,  1151,  1151,  1151,  1151,   518,  3275, -1522,   720,
   -1522, -1522, -1522,  8446,  1275,  1276, -1522,   443, -1522,  1278,
   -1522,  7353, -1522, -1522,  1295,  7353, -1522, -1522, -1522,  8446,
   -1522, -1522,   579, -1522,     6, -1522, -1522,  1369,  1369,  1279,
    1272,  1281,  1282,  1283,  5252, -1522,  7353,  1190,  1190,  1190,
    5252, -1522, -1522,  1369,  1284,  1369, -1522,  1285, -1522, -1522,
    1303, -1522, -1522,  1274,  1325,   389,   392, -1522, -1522,   358,
     511, -1522,  5252,  1291,  1292, -1522, -1522, -1522,   865, -1522,
    1293,  1299,  1304,   458,   -54,  7353,  1305,   403,   246,   944,
   -1522, -1522,   722, -1522, -1522,  1317, -1522, -1522, -1522, -1522,
    1280,   -72,  1457,     6, -1522, -1522,   764,   204,   204, -1522,
    7353,  1369,  1369,   618,  1318,  1319,   972,   204,  1369,   618,
   -1522, -1522,  7353, -1522, -1522,  1320,  7353,  7353, -1522,   511,
     411, -1522, -1522,  1387,  1492,   518, -1522,    52,  1321,   518,
     357, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522,
   -1522, -1522, -1522,  1457,   440,   618,  1348,  1352, -1522,  1326,
    1329,  1330,   204,   204,  1348,  1333, -1522, -1522,  1337,  1338,
     618,  1339,  1340,  7353, -1522, -1522, -1522,  1342, -1522,  7544,
     134, -1522,  5252,   518, -1522,  8446,   197, -1522,  2728,  9216,
   -1522, -1522, -1522, -1522,   412,  1347, -1522, -1522, -1522, -1522,
    1341,  1353, -1522, -1522, -1522,  1355, -1522,  1500,  1358,  1340,
    7353, -1522, -1522, -1522, -1522, -1522,  1151, -1522,  1362,   413,
   -1522, -1522,   591,  7353,  1357,   134,  9216, -1522,   618, -1522,
   -1522, -1522,  7353, -1522,  1360,  1340, -1522,   693,  7544,   518,
   -1522,  7353,   134, -1522, -1522,   518,   414, -1522, -1522,  1363,
   -1522,   518, -1522, -1522,  1368, -1522,   134, -1522,   134, -1522,
   -1522, -1522, -1522,  2934, -1522,  7353, -1522, -1522, -1522, -1522,
    1366,  1371,  1359,  1387, -1522, -1522,  7544,   518, -1522, -1522,
     134, -1522,  3140, -1522,  1371,  1367,   693,  1387, -1522, -1522
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   170,     1,   368,     0,     0,    58,   674,   369,     0,
     866,   856,   861,    20,     0,     0,    19,    16,    15,     3,
       0,     0,     0,     8,   710,     7,   655,     6,    11,     5,
       4,    13,    12,    14,   135,   136,   134,   144,   146,    49,
      67,    64,    65,     0,     0,    50,   676,   675,     0,     0,
      26,    25,   655,   674,   674,   674,     0,   342,    47,   154,
     155,   156,     0,     0,     0,   157,   159,   166,     0,   153,
      21,    10,     9,   292,   692,     0,   656,   657,     0,     0,
       0,     0,    59,     0,    51,     0,     0,    62,   677,   679,
      22,     0,     0,     0,   344,     0,     0,   165,   160,     0,
       0,     0,     0,     0,     0,    76,   293,   295,   680,   702,
     701,   705,   659,   658,   665,   142,   143,   140,   141,   138,
       0,     0,     0,   137,   147,    68,    66,     0,     0,    52,
       0,     0,    63,     0,     0,    60,     0,    23,    24,    27,
     766,    76,    76,   343,    45,    48,   164,     0,   161,   162,
     163,   167,    74,    77,   171,   297,   296,   299,   294,   682,
     681,     0,   704,   703,   707,   706,   711,   660,   582,    30,
      31,    35,     0,   130,   131,   128,   129,   127,   126,   132,
       0,     0,    54,    55,    53,    62,    57,    56,     0,     0,
      29,     0,   690,   857,   862,    46,   158,    75,     0,   683,
     684,   698,   662,     0,   583,     0,    32,    33,    34,     0,
     145,   139,    61,     0,     0,     0,     0,     0,     0,   719,
     739,   720,   755,   721,   725,   726,   727,   728,   745,   732,
     733,   734,   735,   736,   737,   738,   740,   741,   742,   743,
     826,   724,   731,   744,   833,   840,   722,   729,   723,   730,
       0,     0,     0,     0,   754,   787,   790,   788,   789,   853,
     783,   678,    28,   769,   770,   767,   768,   688,   691,   867,
       0,     0,     0,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,     0,     0,
     178,   172,   266,    76,     0,   690,   699,     0,    76,   664,
     661,   582,    76,     0,   644,   637,   666,   133,   791,   817,
     820,     0,   823,   813,     0,     0,   827,   834,   841,   847,
     850,     0,   785,   797,   336,   803,   808,   802,     0,   816,
     812,   805,     0,   807,     0,   784,     0,   689,     0,   858,
     863,   257,   258,   255,   181,   182,   184,   183,   185,   186,
     187,   188,   214,   215,   212,   213,   205,   216,   217,   206,
     203,   204,   256,   239,     0,   254,   218,   219,   220,   221,
     192,   193,   194,   189,   190,   191,   202,     0,   208,   209,
     207,   200,   201,   196,   195,   197,   198,   199,   180,   179,
     238,     0,   210,   211,   582,   175,   305,   746,   749,   752,
     753,   747,   750,   748,   751,   685,     0,   696,   712,     0,
     148,    76,     0,     0,   638,     0,     0,     0,     0,     0,
       0,   483,   484,     0,     0,     0,     0,   477,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   745,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   571,
     416,   418,   417,   419,   420,   421,   422,    41,     0,     0,
       0,     0,     0,   336,     0,   401,   402,   941,   481,   480,
     558,   478,   551,   550,   549,   548,   168,   554,   479,   553,
     552,   525,   485,   526,     0,   474,   530,   486,     0,   482,
     878,   880,   879,   475,   882,   881,   476,     0,     0,     0,
     772,     0,   172,     0,   172,     0,   172,     0,     0,     0,
     799,     0,   796,     0,     0,     0,   948,   394,   810,   811,
     804,   806,     0,   809,   780,     0,     0,   855,   854,   868,
     616,   622,   259,   261,   260,   262,   253,   237,   263,   240,
     222,     0,   173,   367,   607,   608,     0,     0,     0,   298,
       0,   398,     0,   300,   693,     0,   700,     0,     0,   639,
     637,   663,   149,   645,     0,   635,   636,   634,     0,     0,
       0,     0,   777,   902,   905,   347,   754,   351,   350,   356,
     871,   877,   872,   873,   874,   876,   875,     0,   388,     0,
       0,     0,   932,     0,     0,     0,     0,   379,   382,     0,
     385,     0,   936,     0,   914,   918,     0,     0,   908,     0,
     513,   514,     0,     0,     0,     0,     0,     0,     0,   490,
     489,   527,   488,   487,     0,     0,     0,     0,   342,   948,
     948,     0,   403,    76,     0,     0,   411,   402,   333,   168,
     309,   310,   308,   794,     0,     0,     0,     0,   515,   516,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   464,     0,
       0,     0,     0,   756,   771,     0,     0,   175,     0,   175,
       0,   175,   342,   614,     0,   612,     0,   620,     0,   757,
       0,   948,     0,   340,   395,   795,   949,   337,   801,   779,
     782,     0,   761,   617,    94,   623,    94,   264,   265,   242,
     243,   245,   244,   246,   247,   248,   249,   241,   250,   251,
     252,   226,   227,   229,   228,   230,   231,   232,   233,   224,
     225,   234,   235,   236,   223,     0,   365,   366,     0,   582,
     582,   582,   174,   177,   176,     0,   399,   333,   671,   697,
     708,   595,   713,     0,     0,     0,     0,     0,   652,     0,
       0,   792,   818,   821,    18,    17,   775,   776,     0,     0,
       0,     0,   900,     0,     0,     0,   922,   925,   928,     0,
     948,     0,   939,   948,     0,     0,     0,     0,     0,     0,
       0,   948,     0,     0,   948,   911,     0,     0,     0,     0,
       0,     0,     0,     0,    44,     0,    42,     0,     0,   921,
       0,   626,     0,   625,     0,     0,   949,   893,   518,     0,
       0,   451,   448,   450,   338,     0,   336,   467,     0,     0,
       0,     0,   172,   403,     0,   411,     0,     0,   537,   536,
       0,     0,   538,   542,   491,   492,   504,   505,   502,   503,
       0,   531,     0,   523,     0,   555,   556,   557,   493,   494,
     560,   561,   562,   509,   510,   511,   512,     0,     0,   507,
     508,   506,   500,   501,   496,   495,   497,   498,   499,     0,
       0,     0,   457,     0,     0,     0,     0,     0,   472,     0,
     824,   814,   758,     0,   828,     0,   835,     0,   842,     0,
       0,   848,     0,     0,   851,     0,     0,     0,   785,     0,
       0,   396,   781,   762,   686,    92,    95,   859,    95,   864,
       0,     0,   714,   604,   605,   627,   609,   611,   610,   400,
       0,   667,   672,   686,   598,     0,   641,   642,     0,     0,
       0,   654,   793,   819,   822,   778,     0,     0,     0,   901,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   949,     0,   528,     0,     0,   529,     0,
     559,     0,     0,     0,     0,     0,     0,     0,   411,   566,
     567,   568,   569,   570,     0,    38,     0,   110,     0,     0,
       0,     0,   884,   883,   517,     0,     0,     0,     0,     0,
     172,     0,     0,   948,     0,   468,     0,     0,     0,   471,
     469,   169,     0,   175,   335,   359,   357,   305,     0,     0,
       0,   358,     0,     0,     0,    76,     0,   330,   415,   311,
       0,     0,     0,   324,     0,   325,   319,     0,   316,   315,
       0,   317,     0,     0,   334,     0,    90,    91,    88,    89,
     326,   371,   314,     0,   423,   172,   533,     0,   539,     0,
       0,     0,   521,     0,     0,   543,   547,     0,     0,   524,
       0,     0,     0,     0,   458,     0,   465,     0,   519,     0,
     473,   825,   815,     0,   773,     0,   829,   831,   836,   838,
     843,   845,   613,   849,   615,   619,   852,   621,   785,   786,
     798,   341,   397,     0,   669,   687,   869,    93,   618,     0,
     624,     0,   606,     0,     0,     0,     0,   628,     0,     0,
       0,   687,   694,     0,   596,   709,     0,   582,   640,     0,
     649,     0,     0,   653,   903,   906,   348,     0,   353,   354,
     352,     0,     0,   391,   389,     0,     0,     0,   933,   931,
     338,     0,   940,   943,   380,   383,   386,   937,   935,   915,
     919,   917,     0,   909,    76,     0,    39,     0,     0,     0,
     372,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   175,     0,   942,   339,   470,     0,     0,   336,
       0,     0,     0,     0,     0,   409,     0,    76,     0,   360,
       0,     0,   345,     0,     0,   329,     0,     0,    71,   305,
       0,   362,   333,   327,   328,     0,    83,    84,     0,   150,
     150,     0,   318,   313,   320,   321,   322,   323,   370,     0,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   175,
       0,     0,     0,     0,   446,     0,     0,   544,     0,   532,
       0,   522,     0,   336,   459,     0,     0,   520,   466,   462,
       0,   760,   774,   759,   832,   839,   846,   800,   763,   764,
     670,     0,   860,   865,     0,   716,   717,   630,   629,   301,
     668,   673,     0,     0,   589,   592,     0,   643,     0,   651,
       0,     0,   349,   355,     0,     0,   390,   923,   926,   929,
       0,     0,     0,     0,     0,     0,     0,   900,     0,   912,
       0,     0,   305,   572,     0,    36,    43,     0,   112,     0,
     113,     0,   114,     0,     0,     0,     0,     0,   886,   885,
     449,   581,   452,     0,     0,   444,     0,   406,   407,     0,
     405,   404,     0,   412,   305,   361,   305,   346,     0,     0,
      69,    70,   119,   363,     0,     0,     0,     0,   152,     0,
       0,     0,     0,   646,   377,   376,   435,   436,   438,   437,
     439,   429,   430,   431,   440,   441,   425,   426,   427,   428,
     442,   443,   432,   433,   434,   424,    76,     0,   580,     0,
     578,   447,   575,     0,     0,     0,   574,     0,   460,     0,
     463,     0,   870,   715,     0,     0,   302,   307,   695,     0,
     590,   591,   592,   593,   584,   599,   650,   900,   900,     0,
       0,     0,     0,     0,   336,   944,   338,   381,   384,   387,
       0,   901,   916,   900,     0,   900,   563,     0,   565,   573,
      40,   111,   373,     0,     0,     0,     0,   888,   887,     0,
       0,   455,     0,     0,     0,   410,   413,   364,   125,   124,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   305,
     534,   540,     0,   579,   577,     0,   576,   765,   718,   631,
       0,     0,   587,   584,   585,   586,   589,   899,   899,   392,
       0,   900,   900,   891,     0,     0,   948,   899,   900,   891,
     564,    37,     0,   115,   116,     0,     0,     0,   453,     0,
       0,   445,   408,   301,    85,    76,   151,     0,     0,    76,
     637,   378,   647,   648,   414,   535,   541,   545,   461,   333,
     597,   588,   600,   587,     0,     0,   896,   948,   898,     0,
       0,     0,   899,   899,   892,     0,   934,   945,     0,     0,
     891,     0,   946,     0,   890,   889,   456,     0,   332,     0,
       0,   107,     0,    76,   305,     0,     0,   546,     0,     0,
     602,   633,   632,   594,     0,   949,   897,   904,   907,   393,
       0,     0,   930,   938,   920,     0,   910,     0,     0,   946,
       0,    86,    90,    91,    88,    89,    87,   109,    99,     0,
     305,   123,     0,     0,     0,     0,     0,   894,     0,   924,
     927,   913,     0,   950,     0,   946,    96,    78,     0,    76,
     121,     0,     0,   304,   601,    76,     0,   947,   951,     0,
     333,    76,    72,    73,     0,   108,     0,   305,     0,   375,
     305,   895,   952,     0,    79,     0,   100,   118,   374,   603,
       0,   104,     0,   301,   101,    80,     0,    76,    98,   333,
       0,    81,     0,   105,   104,     0,    78,   301,    82,   103
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1522, -1522,  -897,    -1, -1522, -1522, -1522, -1522, -1522,   953,
    1493, -1522, -1522, -1522, -1522, -1522, -1522,  1573, -1522, -1522,
   -1522,   369, -1522, -1522,  1390, -1522, -1522,  1497, -1522, -1522,
   -1522, -1522,  -132,  -112, -1522, -1522, -1522, -1522, -1521,   861,
     870, -1522, -1522, -1522, -1522,  -103, -1522, -1522, -1522, -1522,
   -1522, -1522, -1522,  -993, -1522, -1522, -1522, -1522, -1522, -1522,
   -1522, -1522,  1391, -1522, -1522,   -43,   -97,  -359,   353, -1522,
   -1522,   494,   956,   960,   648,  -483,  -667, -1522,  -312, -1522,
   -1522, -1522, -1229, -1522, -1522, -1488, -1522, -1522, -1023, -1522,
   -1522, -1522, -1522, -1522, -1522,  -756,  -330, -1154,   895,   -13,
   -1522, -1522, -1522, -1522, -1522, -1486, -1400, -1396, -1335, -1522,
   -1522,  1611, -1522,  -992, -1522, -1522, -1522, -1522, -1522, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522,  -452, -1313,   461,
     238, -1522,  -796, -1522,   521, -1522, -1522, -1522, -1522, -1405,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522,   410,
    1044, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522,  -162,
     100,    51,    99,   174, -1522, -1522, -1522, -1522, -1522, -1522,
   -1522,   279,  -505,  -770, -1522,  -513,  -762, -1522,  -932,    54,
      55, -1522,  -569,  -563, -1522, -1522, -1522, -1223, -1522,  1570,
   -1522, -1522, -1522, -1522, -1522,   484,   674, -1522,   991, -1522,
   -1522, -1522, -1522, -1522, -1522,   676, -1522,  1327, -1522, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522,
   -1522, -1522, -1522,  -135, -1522,  1193, -1522, -1522, -1522,  1402,
   -1522, -1522, -1522,  -567, -1522, -1522,  -284,  -881, -1522, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522,  -161, -1522, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522, -1522,
   -1522, -1522, -1522, -1522, -1522,  -823, -1444,  -626, -1522, -1522,
   -1258, -1295,  1202, -1522, -1522, -1522, -1522, -1522, -1522, -1522,
   -1522, -1522, -1522,  1203, -1522, -1522,  1205, -1522, -1522, -1522,
   -1522, -1522, -1522, -1522, -1522, -1522, -1522,  1032, -1522,  -425,
    1208, -1166,  -615,  1210,  -421
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   786,   787,    18,   139,    52,   190,    19,   172,
     178,  1470,  1187,  1345,   628,   478,   145,   479,    96,    21,
      22,    87,    44,    45,   135,    23,    41,    42,  1050,  1051,
    1664,   153,   154,  1665,  1681,  1694,  1238,  1590,  1052,   936,
     937,  1647,  1660,  1680,  1648,  1685,  1689,  1695,  1686,  1053,
    1054,  1628,  1055,  1009,  1056,  1057,  1058,  1059,  1060,  1061,
    1062,  1063,   179,   180,    37,    38,    39,   204,  1389,    65,
      66,    67,    68,   646,    24,   405,   559,   301,   302,   107,
      25,   157,   303,   158,   198,  1437,  1510,  1634,   561,   562,
    1139,   480,  1064,  1232,  1490,   854,   636,  1022,   712,   481,
    1065,   587,   791,  1322,   482,  1066,  1067,  1068,  1069,  1070,
     758,  1071,  1249,  1191,  1394,  1072,   483,   805,  1333,   806,
    1334,   808,  1335,   484,   795,  1326,   485,   526,   563,   486,
    1215,  1216,   852,   487,   650,   488,  1073,   489,   490,   843,
     491,  1019,  1480,  1020,  1539,   492,   905,  1288,   493,   527,
     495,  1270,  1555,  1272,  1556,  1423,  1597,   496,   497,   553,
    1516,  1562,  1442,  1444,  1316,   954,  1147,  1599,  1636,   554,
     555,   556,   703,   704,   724,   707,   708,   726,   834,   943,
     944,  1603,   578,   425,   570,   315,  1498,   571,   316,    77,
     114,   202,   311,    27,   168,   952,  1125,   953,    48,    49,
     136,    28,   161,   200,   305,  1126,   268,   269,    29,   108,
     768,  1312,   566,   307,   308,   111,   166,   772,    30,    75,
     201,   567,   945,   498,   415,   256,   257,   913,   934,   192,
     258,   695,  1292,   922,   581,   345,   259,   522,   260,   426,
     962,   523,   710,   508,  1102,   427,   963,   428,   964,   507,
    1101,   511,  1106,   512,  1294,   513,  1108,   514,  1295,   515,
    1110,   516,  1296,   517,  1113,   518,  1116,   705,    31,    54,
     270,   540,  1129,    32,    55,   271,   541,  1131,    33,    53,
     348,   722,  1301,   589,   499,   640,  1575,   641,  1567,  1568,
    1569,   972,   500,   789,  1320,   790,  1321,   816,  1340,   996,
    1464,   812,  1337,   501,   813,  1338,   502,   976,  1451,   977,
    1452,   978,  1453,   799,  1330,   810,  1336,  1023,   643,   503,
     504,  1618,   717,   505,   506
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,    58,    69,   147,   525,   779,   205,   777,   593,   193,
     194,   950,   596,  1138,  1220,   788,  1331,  1194,   849,    70,
      71,    72,   639,  1027,   835,   837,  1395,   261,   725,   697,
     915,   699,   917,   701,   919,   723,   124,  1124,   530,   543,
     545,  -170,  1462,   994,   617,   528,   599,  1120,    89,    69,
      69,    69,  1514,   255,   997,  1588,  1124,   131,    59,  1075,
    1192,  1486,   538,  -830,  1354,  1482,  -837,   551,  1621,   711,
       2,   568,  1592,  1143,  -844,  1538,    50,     3,    34,    35,
    -454,   101,   102,   103,  1198,  1581,   569,    60,    69,    69,
      69,    69,   132,    13,  1006,   551,   927,    73,   152,   568,
       4,    40,     5,  1622,     6,   346,  1387,   109,   110,  1007,
       7,   203,  1515,  -830,   569,    51,  -837,   552,  -830,   203,
       8,  -837,    95,  1392,  -844,    74,     9,  1666,  1393,  -844,
    -454,    61,    16,   611,  1586,  -454,  1615,  -830,   756,   574,
    -837,   925,   146,   600,   601,   929,  1008,   203,  -844,   419,
      10,   800,  1517,  1518,  -454,   335,    62,   115,   116,   920,
     312,   811,  1622,    56,   814,  1690,   971,  1352,  1527,    79,
    1529,   406,    11,    12,   333,   254,   418,   173,   174,   557,
     420,   757,   336,   337,   531,   980,  1554,    57,   984,  1623,
      43,   529,   848,  1624,   334,  1688,   992,   196,  1144,   995,
    1622,   310,  1184,   532,  1353,   618,  1382,  1166,   959,  1699,
    1364,   255,   533,  1193,    63,    76,  1167,  1193,  1193,  1026,
     133,   602,    36,    78,    64,  1193,  1572,  1573,  1300,   558,
     831,    15,   134,  1580,   982,    13,   728,  1297,   909,  1145,
     332,   603,   666,   667,  1300,   209,   338,   881,  1623,   575,
     339,   882,  1624,   335,  1625,   576,    14,  1028,   429,   430,
    1570,    79,   831,  1356,   760,   579,   580,   582,    15,  1579,
     421,    80,   152,   210,    16,  1553,  1135,   833,   436,  1136,
     336,   337,  1137,    81,   438,  1029,  1623,    13,   255,   572,
    1624,   255,   255,   255,   831,   761,   577,   340,   117,    13,
     832,   341,  1525,   118,   342,   119,    95,  1341,   120,   833,
     688,   689,   346,  1625,  1610,  1611,    95,    83,   175,  1468,
     801,   445,   446,   176,   575,   177,    16,   637,   120,   343,
     576,   654,   655,   254,    84,   817,  1024,  1099,    16,    82,
     970,   833,   121,  1157,   338,   122,   691,   692,   339,    88,
     696,  1625,   698,  1487,   700,   448,   449,    85,   637,   929,
     100,  1475,   713,  1099,   104,  1100,  1217,  1024,    86,  1033,
    1635,   577,   255,   255,  1025,  1435,   255,   706,   255,    13,
     255,    46,   255,   520,   784,    13,  1366,    47,   255,  1151,
     105,  1158,    56,   785,   568,   340,   763,   764,    94,   341,
    1497,   981,   342,   521,  1565,   255,  1162,  1655,  1205,   569,
     254,    13,  1117,   254,   254,   254,    57,  1114,    16,   106,
     586,   597,   255,   255,    16,  1130,    83,   343,   658,   659,
     925,  1128,   101,   585,   103,  1349,   664,    56,   665,   666,
     667,   668,   669,    84,   104,   664,  1372,  1103,   666,   667,
      16,   473,  1551,  1644,   129,  1104,   831,   125,   477,   335,
    1427,    57,  1197,  1350,   191,   128,    85,  1207,  -766,   255,
    1031,   784,   654,   655,  1373,   127,  1384,    86,  1105,  1659,
     785,  1099,   831,   831,   255,   831,   336,   337,   832,  1355,
    1099,  1536,   958,  1208,   254,   254,   182,   183,   254,   184,
     254,  1547,   254,   833,   254,   966,   967,   688,   689,  1421,
     254,   844,   654,   655,  1207,   979,   688,   689,  1428,   859,
     863,   986,   987,  1099,   989,    13,   991,   254,   993,   833,
     833,   130,   833,  1595,   877,  1363,   424,  1202,   140,  1099,
    1483,   604,  1210,   612,   254,   254,  1099,  1537,  1211,   713,
     338,  1430,   769,   906,   339,   144,    97,    98,    99,   784,
      13,   605,  1349,   613,    16,  1099,   309,  1469,   785,   658,
     659,  1631,   346,   778,  1473,   255,  1349,   664,  1212,   665,
     666,   667,   668,   669,  1099,  1207,  1099,  1207,   941,  1213,
    1534,   254,  1269,  1535,  1214,   148,   149,   150,   151,    16,
     925,   340,  1416,   942,  1550,   341,   254,  1650,   342,   658,
     659,   141,  1587,  1637,  1649,  1671,  1207,   664,   615,   793,
     666,   667,   668,   669,   335,   142,  1512,   965,  1275,   842,
     968,    79,    69,   343,   975,   685,   686,   687,   616,   794,
    1285,   255,  1505,   143,  1677,  1290,  1443,  1679,   688,   689,
      56,   336,   337,   255,   255,   255,   255,  1546,    56,   839,
     255,   407,   112,   422,   255,   408,   423,   839,   113,   424,
     255,   255,   840,   255,    57,   255,   162,   255,   255,   409,
     410,   263,    57,   152,   411,   412,   413,   414,   688,   689,
     770,   771,  1165,   872,  1566,  1566,   264,   254,  1171,   841,
    1574,   265,   873,   266,  1566,  1661,  1574,   346,   163,   654,
     655,   781,  1182,   784,    13,   338,  1662,  1663,   575,   339,
     784,    13,   785,   935,   576,   935,   137,   784,    13,   785,
     921,   494,   138,  1344,  1015,  1201,   785,   924,  1299,   784,
    1351,   519,  1604,  1016,   928,   951,   784,  1088,   785,  1566,
    1566,   167,  1086,    16,   535,   785,  1089,  1574,   775,  1112,
      16,   776,  1115,   254,   424,   577,   340,    16,  1121,   181,
     341,  1093,   637,   342,   134,   254,   254,   254,   254,   961,
    1094,  1024,   254,   155,   159,   255,   254,   255,   255,   156,
     160,   185,   254,   254,   255,   254,   164,   254,   343,   254,
     254,   255,   165,  1598,   656,   657,   658,   659,   660,   206,
     207,   661,   662,   663,   664,  1656,   665,   666,   667,   668,
     669,  1440,   670,   671,   335,   188,   672,  1441,   673,   674,
     675,   346,  1236,  1237,   676,   782,  1407,   784,    13,   346,
    1408,   255,   255,   910,   195,   346,   785,   255,   335,   911,
     186,   336,   337,  1049,  1118,   346,   346,   187,   346,   914,
     916,   255,   918,   677,  1087,   678,   679,   680,   681,   682,
     683,   684,   685,   686,   687,   336,   337,    16,   346,   346,
    1388,  1388,  1156,  1164,   255,   688,   689,   189,   191,   741,
     742,   743,   744,   745,   746,   747,   748,   346,   101,   346,
     197,  1501,  1476,  1557,  1673,   199,   203,   254,   749,   254,
     254,  1578,   211,  1229,   750,   338,   254,   475,   647,   339,
     648,   784,    13,   254,   751,   752,   753,   784,    13,   132,
     785,   784,    13,  1692,   714,  1127,   785,  1127,  1161,   338,
     785,   784,    13,   339,  1180,   262,   721,   267,  1183,  1049,
     785,   649,  1606,   649,   649,   754,   304,  1150,  1328,  1153,
     306,    16,   313,   254,   254,   314,   340,    16,   318,   254,
     341,    16,   998,   342,   319,  1307,   169,   170,   824,   825,
     320,    16,   321,   254,   322,  1315,  1417,  1596,   780,   324,
     340,   101,   102,   103,   341,   331,  1154,   342,   343,   654,
     655,   255,   349,   784,    13,   323,   254,   326,  1526,   169,
     170,   171,   785,   651,   652,   802,   804,   327,   328,   807,
    1329,   809,   343,   206,   207,   208,   329,  1633,   330,   344,
    1540,   347,   818,   819,   820,   821,   822,   823,   946,   947,
     948,  1225,   346,    16,    91,    92,    93,   402,   350,  1233,
    1234,   403,  1342,   404,   729,   730,   731,   732,   733,   734,
     735,   736,  1242,  1651,  1243,  1244,  1245,  1246,  1247,   417,
     509,   510,  1250,   536,   539,   546,   547,   874,   548,   737,
     549,   550,   560,   564,   565,  1374,   573,   654,   655,   738,
     739,   740,   606,   583,   656,   657,   658,   659,   660,   907,
     584,   661,   662,   663,   664,   598,   665,   666,   667,   668,
     669,  1419,   670,   671,   607,   608,   609,   610,   673,   674,
     675,   611,   614,   254,  1524,   619,   931,   622,   623,   624,
     625,   626,   627,   694,   645,   255,   642,   255,   644,   690,
    1629,   653,   693,   706,   709,   716,   774,   718,   715,   719,
     762,  1319,   783,   677,   755,   678,   679,   680,   681,   682,
     683,   684,   685,   686,   687,   940,   759,   773,   767,   792,
     796,   828,   654,   655,   797,   688,   689,   798,   815,   827,
     829,   836,   830,   955,   658,   659,   838,   851,   870,   960,
     853,   912,   664,   923,   665,   666,   667,   668,   669,   932,
     933,   951,   956,   957,   974,   335,   973,   983,   985,  1010,
     335,   654,   655,  1076,   988,   990,   999,  1488,  1096,  1000,
    1001,  1002,  1003,  1004,  1014,  1030,  1017,  1018,  1078,  1032,
    1077,  1079,   336,   337,  1082,  1005,  1080,   336,   337,  1107,
    1011,  1081,  1012,   255,  1013,  1091,  1092,  1097,   683,   684,
     685,   686,   687,  1098,   802,  1109,  1111,   254,  1119,   254,
     521,  1123,  1502,   688,   689,  1132,  1134,   656,   657,   658,
     659,   660,  1133,  1140,   661,   662,   663,   664,  1511,   665,
     666,   667,   668,   669,  1499,   670,   671,  1148,   255,   672,
    1493,   673,   674,   675,  1149,  1159,   338,   676,  1160,  1163,
     339,   338,  1169,  1173,   255,   339,   656,   657,   658,   659,
    1090,  1632,  1170,  1185,  1095,  1445,   664,  1446,   665,   666,
     667,   668,   669,  1178,   670,   671,   677,  1181,   678,   679,
     680,   681,   682,   683,   684,   685,   686,   687,  1186,  1188,
    1122,  1189,  1190,  1195,  1196,  1203,  1204,   340,   688,   689,
    1206,   341,   340,  1155,   342,  1223,   341,  1224,  1168,   342,
    1226,  1235,  1239,  1240,  1241,   254,  1248,  1277,  1278,  1152,
    1280,  1286,   683,   684,   685,   686,   687,  1281,  1291,   343,
    1287,  1293,  1302,  1049,   343,  1305,  1303,   688,   689,  1306,
    1309,  1313,  1314,  1172,  1317,  1325,  1323,  1324,  1346,  1348,
    1360,  1361,  1365,   335,  1367,  1368,  1369,  1370,  1371,  1376,
     254,  1378,  1379,  1591,  1381,  1380,   335,  1594,  1432,  1385,
    1386,  1391,  1431,  1434,  1436,  1418,   254,  1199,  1200,  1439,
     336,   337,  1443,  1450,  1420,  1422,  1426,   335,   931,  1461,
    1472,  1433,  1474,   336,   337,  1219,   335,  1447,  1222,  1438,
    1448,  1481,  1484,  1454,  1228,  1455,  1231,  1456,  1508,  1460,
     255,  1630,  1463,  1465,   336,   337,  1471,   842,  1479,  1494,
    1531,  1495,  1520,   336,   337,   335,  1503,  1504,  1532,  1506,
    1519,  1521,  1522,  1523,  1528,  1559,  1530,  1271,  1533,  1273,
    1561,  1276,  1541,  1542,   338,   620,   621,  1552,   339,  1543,
    1544,  1282,   336,   337,  1589,  1545,  1549,   338,   335,   931,
    1560,   339,   629,   630,   631,   632,   633,  1667,  1558,  1576,
    1577,  1207,  1593,  1670,  1583,  1605,   842,  1607,   338,  1674,
    1608,  1609,   339,  1298,  1612,   336,   337,   338,  1613,  1614,
    1616,   339,  1639,  1304,  1617,   340,  1620,  1638,  1308,   341,
    1310,  1174,   342,  1642,  1640,  1691,  1641,  1643,   340,  1658,
    1687,  1318,   341,  1653,  1175,   342,   338,  1646,  1675,  1672,
     339,   123,  1683,  1697,    20,   212,  1684,   343,   126,   340,
     802,   826,   254,   341,  1698,  1176,   342,   938,   340,  1627,
     343,  1696,   341,  1390,  1177,   342,   939,  1049,  1347,   338,
     317,   850,  1146,   339,   766,   926,  1357,  1358,  1359,   855,
    1485,   343,    26,  1563,  1600,  1564,  1513,   340,  1601,  1602,
     343,   341,    90,  1179,   342,  1311,   588,  1141,  1375,  1142,
     325,  1377,   416,     0,  1654,   590,   591,   803,   592,     0,
    1383,   594,   335,   595,     0,     0,     0,     0,     0,   343,
     340,  1669,     0,     0,   341,     0,  1327,   342,   335,     0,
       0,     0,   654,   655,     0,  1676,     0,  1678,     0,   336,
     337,     0,  1049,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   343,     0,     0,   336,   337,     0,  1424,  1693,
    1425,  1049,     0,     0,     0,     0,  1429,     0,     0,     0,
     864,   865,     0,     0,   866,   867,   868,   869,     0,   871,
       0,     0,   875,   876,   878,   879,   880,   883,   884,   885,
     886,   888,   889,   890,   891,   892,   893,   894,   895,   896,
     897,   898,     0,   338,  1449,     0,     0,   339,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   338,
       0,  1467,     0,   339,     0,     0,     0,   656,   657,   658,
     659,   660,     0,     0,   661,  1477,  1478,   664,     0,   665,
     666,   667,   668,   669,     0,   670,   671,     0,     0,     0,
       0,     0,     0,     0,   340,     0,     0,     0,   341,  1489,
    1332,   342,     0,     0,     0,  1491,  1492,     0,     0,     0,
     340,  1496,     0,     0,   341,     0,  1339,   342,     0,   949,
       0,     0,     0,     0,     0,     0,   343,     0,     0,     0,
       0,   681,   682,   683,   684,   685,   686,   687,     0,     0,
       0,     0,   343,     0,     0,     0,     0,     0,   688,   689,
       0,  1507,     0,     0,     0,  1509,     0,     0,     0,     0,
     -87,     0,     0,     0,     0,     0,     0,   856,     0,     0,
       0,   654,   655,     0,     0,     0,   802,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1074,     0,
       0,     0,     0,     0,     0,  1548,     0,   335,     0,   219,
     220,   221,     0,   223,   224,   225,   226,   227,   447,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
    1571,   241,   242,   243,   336,   337,   246,   247,   248,   249,
       0,     0,  1582,     0,     0,     0,  1584,  1585,  1251,  1252,
    1253,  1254,  1255,  1256,  1257,  1258,   656,   657,   658,   659,
     660,  1259,  1260,   661,   662,   663,   664,  1261,   665,   666,
     667,   668,   669,  1262,   670,   671,  1263,  1264,   672,     0,
     673,   674,   675,  1265,  1266,  1267,   676,     0,     0,     0,
       0,     0,   857,  1619,  1074,     0,     0,     0,   338,     0,
       0,   858,   339,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1268,   677,     0,   678,   679,   680,
     681,   682,   683,   684,   685,   686,   687,     0,     0,     0,
    1645,     0,     0,     0,     0,     0,     0,   688,   689,     0,
       0,     0,     0,  1652,     0,     0,     0,     0,     0,   340,
       0,     0,  1657,   341,     0,  1362,   342,     0,     0,     0,
       0,  1668,  1034,     0,     0,     0,   429,   430,     3,     0,
    -120,  -106,  -106,     0,  -117,     0,   431,   432,   433,   434,
     435,   343,     0,     0,     0,  1682,   436,  1035,   437,  1036,
    1037,     0,   438,     0,     0,     0,     0,     0,     0,  1038,
     439,  1039,     0,  -122,     0,  1040,   440,     0,     0,   441,
       0,     8,   442,  1041,     0,  1042,   443,     0,     0,  1043,
    1044,     0,     0,     0,     0,     0,  1045,     0,     0,   445,
     446,  1279,   219,   220,   221,     0,   223,   224,   225,   226,
     227,   447,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,     0,   241,   242,   243,     0,     0,   246,
     247,   248,   249,   448,   449,   450,  1046,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   451,
     452,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,   453,   454,
     455,   456,   457,     0,   458,     0,   459,   460,   461,   462,
     463,   464,   465,   466,    57,     0,    13,   467,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,   470,     0,    14,     0,     0,
     471,   472,     0,     0,     0,     0,     0,     0,     0,   473,
       0,   474,     0,   475,   476,    16,  1047,  1048,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1396,  1397,  1398,  1399,  1400,
    1401,  1402,  1403,  1404,  1405,  1406,  1409,  1410,  1411,  1412,
    1413,  1414,  1415,     0,     0,     0,  1034,     0,     0,     0,
     429,   430,     3,     0,  -120,  -106,  -106,     0,  -117,     0,
     431,   432,   433,   434,   435,     0,     0,     0,     0,     0,
     436,  1035,   437,  1036,  1037,     0,   438,     0,     0,     0,
       0,     0,     0,  1038,   439,  1039,     0,  -122,     0,  1040,
     440,     0,     0,   441,     0,     8,   442,  1041,     0,  1042,
     443,     0,     0,  1043,  1044,     0,     0,  1457,  1458,  1459,
    1045,     0,     0,   445,   446,     0,   219,   220,   221,     0,
     223,   224,   225,   226,   227,   447,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,     0,   241,   242,
     243,     0,     0,   246,   247,   248,   249,   448,   449,   450,
    1046,     0,     0,     0,     0,     0,     0,     0,  1074,     0,
       0,     0,     0,   451,   452,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,   453,   454,   455,   456,   457,     0,   458,     0,
     459,   460,   461,   462,   463,   464,   465,   466,    57,     0,
      13,   467,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   468,   469,   470,
       0,    14,     0,     0,   471,   472,     0,     0,     0,     0,
       0,     0,     0,   473,     0,   474,     0,   475,   476,    16,
    1047,  -306,  1034,     0,     0,     0,   429,   430,     3,     0,
    -120,  -106,  -106,     0,  -117,     0,   431,   432,   433,   434,
     435,     0,     0,     0,     0,     0,   436,  1035,   437,  1036,
    1037,     0,   438,     0,     0,     0,     0,     0,     0,  1038,
     439,  1039,     0,  -122,     0,  1040,   440,     0,     0,   441,
       0,     8,   442,  1041,     0,  1042,   443,     0,     0,  1043,
    1044,     0,     0,     0,     0,     0,  1045,     0,     0,   445,
     446,     0,   219,   220,   221,     0,   223,   224,   225,   226,
     227,   447,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,     0,   241,   242,   243,     0,     0,   246,
     247,   248,   249,   448,   449,   450,  1046,     0,     0,     0,
       0,     0,     0,  1626,     0,     0,     0,     0,     0,   451,
     452,     0,  1074,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,   453,   454,
     455,   456,   457,     0,   458,     0,   459,   460,   461,   462,
     463,   464,   465,   466,    57,     0,    13,   467,     0,     0,
       0,     0,  1626,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,   470,     0,    14,     0,     0,
     471,   472,     0,     0,     0,     0,     0,  1074,     0,   473,
       0,   474,     0,   475,   476,    16,  1047,  -331,     0,  1034,
    1626,     0,     0,   429,   430,     3,  1074,  -120,  -106,  -106,
       0,  -117,     0,   431,   432,   433,   434,   435,     0,     0,
       0,     0,     0,   436,  1035,   437,  1036,  1037,     0,   438,
       0,     0,     0,     0,     0,     0,  1038,   439,  1039,     0,
    -122,     0,  1040,   440,     0,     0,   441,     0,     8,   442,
    1041,     0,  1042,   443,     0,     0,  1043,  1044,     0,     0,
       0,     0,     0,  1045,     0,     0,   445,   446,     0,   219,
     220,   221,     0,   223,   224,   225,   226,   227,   447,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
       0,   241,   242,   243,     0,     0,   246,   247,   248,   249,
     448,   449,   450,  1046,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   451,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,   453,   454,   455,   456,   457,
       0,   458,     0,   459,   460,   461,   462,   463,   464,   465,
     466,    57,     0,    13,   467,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,     0,    14,     0,     0,   471,   472,     0,
       0,     0,     0,     0,     0,     0,   473,     0,   474,     0,
     475,   476,    16,  1047,  -303,  1034,     0,     0,     0,   429,
     430,     3,     0,  -120,  -106,  -106,     0,  -117,     0,   431,
     432,   433,   434,   435,     0,     0,     0,     0,     0,   436,
    1035,   437,  1036,  1037,     0,   438,     0,     0,     0,     0,
       0,     0,  1038,   439,  1039,     0,  -122,     0,  1040,   440,
       0,     0,   441,     0,     8,   442,  1041,     0,  1042,   443,
       0,     0,  1043,  1044,     0,     0,     0,     0,     0,  1045,
       0,     0,   445,   446,     0,   219,   220,   221,     0,   223,
     224,   225,   226,   227,   447,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,     0,   241,   242,   243,
       0,     0,   246,   247,   248,   249,   448,   449,   450,  1046,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   451,   452,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,   453,   454,   455,   456,   457,     0,   458,     0,   459,
     460,   461,   462,   463,   464,   465,   466,    57,     0,    13,
     467,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   468,   469,   470,     0,
      14,     0,     0,   471,   472,     0,     0,     0,     0,     0,
       0,     0,   473,     0,   474,     0,   475,   476,    16,  1047,
     -97,  1034,     0,     0,     0,   429,   430,     3,     0,  -120,
    -106,  -106,     0,  -117,     0,   431,   432,   433,   434,   435,
       0,     0,     0,     0,     0,   436,  1035,   437,  1036,  1037,
       0,   438,     0,     0,     0,     0,     0,     0,  1038,   439,
    1039,     0,  -122,     0,  1040,   440,     0,     0,   441,     0,
       8,   442,  1041,     0,  1042,   443,     0,     0,  1043,  1044,
       0,     0,     0,     0,     0,  1045,     0,     0,   445,   446,
       0,   219,   220,   221,     0,   223,   224,   225,   226,   227,
     447,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,     0,   241,   242,   243,     0,     0,   246,   247,
     248,   249,   448,   449,   450,  1046,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   451,   452,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,   453,   454,   455,
     456,   457,     0,   458,     0,   459,   460,   461,   462,   463,
     464,   465,   466,    57,     0,    13,   467,     0,   335,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   468,   469,   470,     0,    14,     0,     0,   471,
     472,     0,     0,     0,     0,   336,   337,     0,   473,     0,
     474,     0,   475,   476,    16,  1047,  -102,   429,   430,     0,
       0,     0,     0,     0,     0,     0,     0,   431,   432,   433,
     434,   435,     0,     0,     0,     0,     0,   436,     0,   437,
       0,     0,     0,   438,     0,     0,     0,     0,     0,     0,
       0,   439,     0,     0,     0,     0,     0,   440,     0,     0,
     441,     0,     0,   442,     0,     0,     0,   443,     0,   338,
       0,     0,     0,   339,     0,     0,     0,   444,     0,     0,
     445,   446,   845,   219,   220,   221,     0,   223,   224,   225,
     226,   227,   447,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,     0,   241,   242,   243,     0,     0,
     246,   247,   248,   249,   448,   449,   450,     0,     0,     0,
     340,     0,     0,     0,   341,     0,  1500,   342,     0,     0,
     451,   452,     0,     0,     0,     0,     0,     0,     0,   524,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,   343,     0,     0,     0,     0,     0,     0,   453,
     454,   455,   456,   457,     0,   458,   637,   459,   460,   461,
     462,   463,   464,   465,   466,   638,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   468,   469,   470,     0,    14,     0,
       0,   471,   472,     0,     0,     0,     0,     0,   429,   430,
     846,     0,   474,   847,   475,   476,   634,   477,   431,   432,
     433,   434,   435,     0,     0,     0,     0,     0,   436,     0,
     437,     0,     0,     0,   438,     0,     0,     0,     0,     0,
       0,     0,   439,     0,     0,     0,     0,     0,   440,     0,
       0,   441,   635,     0,   442,     0,     0,     0,   443,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   444,     0,
       0,   445,   446,     0,   219,   220,   221,     0,   223,   224,
     225,   226,   227,   447,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,     0,   241,   242,   243,     0,
       0,   246,   247,   248,   249,   448,   449,   450,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   451,   452,     0,     0,     0,     0,     0,     0,     0,
     524,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
     453,   454,   455,   456,   457,     0,   458,   637,   459,   460,
     461,   462,   463,   464,   465,   466,   638,     0,     0,   467,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,   469,   470,     0,    14,
       0,     0,   471,   472,     0,     0,     0,     0,     0,   429,
     430,   473,     0,   474,     0,   475,   476,   634,   477,   431,
     432,   433,   434,   435,     0,     0,     0,     0,     0,   436,
       0,   437,     0,     0,     0,   438,     0,     0,     0,     0,
       0,     0,     0,   439,     0,     0,     0,     0,     0,   440,
       0,     0,   441,   635,     0,   442,     0,     0,     0,   443,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   444,
       0,     0,   445,   446,     0,   219,   220,   221,     0,   223,
     224,   225,   226,   227,   447,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,     0,   241,   242,   243,
       0,     0,   246,   247,   248,   249,   448,   449,   450,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   451,   452,     0,     0,     0,     0,     0,     0,
       0,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,   453,   454,   455,   456,   457,     0,   458,     0,   459,
     460,   461,   462,   463,   464,   465,   466,    57,     0,     0,
     467,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   468,   469,   470,     0,
      14,     0,     0,   471,   472,     0,     0,     0,     0,     0,
     429,   430,   473,     0,   474,     0,   475,   476,     0,   477,
     431,   432,   433,   434,   435,     0,     0,     0,     0,     0,
     436,     0,   437,     0,     0,     0,   438,     0,     0,     0,
       0,     0,     0,     0,   439,     0,     0,     0,     0,     0,
     440,     0,     0,   441,     0,     0,   442,     0,     0,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,   445,   446,   969,   219,   220,   221,     0,
     223,   224,   225,   226,   227,   447,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,     0,   241,   242,
     243,     0,     0,   246,   247,   248,   249,   448,   449,   450,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   451,   452,     0,     0,     0,     0,     0,
       0,     0,   524,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,   453,   454,   455,   456,   457,     0,   458,   637,
     459,   460,   461,   462,   463,   464,   465,   466,   638,     0,
       0,   467,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   468,   469,   470,
       0,    14,     0,     0,   471,   472,     0,     0,     0,     0,
       0,   429,   430,   473,     0,   474,     0,   475,   476,     0,
     477,   431,   432,   433,   434,   435,     0,     0,     0,     0,
       0,   436,     0,   437,     0,     0,     0,   438,     0,     0,
       0,     0,     0,     0,     0,   439,     0,     0,     0,     0,
       0,   440,     0,     0,   441,     0,     0,   442,     0,     0,
       0,   443,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   444,     0,     0,   445,   446,     0,   219,   220,   221,
       0,   223,   224,   225,   226,   227,   447,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,     0,   241,
     242,   243,     0,     0,   246,   247,   248,   249,   448,   449,
     450,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   451,   452,     0,     0,     0,     0,
       0,     0,     0,   524,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,   456,   457,     0,   458,
     637,   459,   460,   461,   462,   463,   464,   465,   466,   638,
       0,     0,   467,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   468,   469,
     470,     0,    14,     0,     0,   471,   472,     0,     0,     0,
       0,     0,   429,   430,   473,     0,   474,     0,   475,   476,
       0,   477,   431,   432,   433,   434,   435,     0,     0,     0,
       0,     0,   436,     0,   437,     0,     0,     0,   438,     0,
       0,     0,     0,     0,     0,     0,   439,     0,     0,     0,
       0,     0,   440,     0,     0,   441,     0,     0,   442,     0,
       0,     0,   443,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   444,     0,     0,   445,   446,     0,   219,   220,
     221,     0,   223,   224,   225,   226,   227,   447,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,     0,
     241,   242,   243,     0,     0,   246,   247,   248,   249,   448,
     449,   450,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   451,   452,     0,     0,     0,
       0,     0,     0,     0,   524,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,   453,   454,   455,   456,   457,     0,
     458,     0,   459,   460,   461,   462,   463,   464,   465,   466,
      57,     0,     0,   467,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   468,
     469,   470,     0,    14,     0,     0,   471,   472,     0,     0,
       0,     0,     0,   429,   430,   473,     0,   474,   908,   475,
     476,     0,   477,   431,   432,   433,   434,   435,     0,     0,
       0,     0,     0,   436,     0,   437,     0,     0,     0,   438,
       0,     0,     0,     0,     0,     0,     0,   439,     0,     0,
       0,     0,     0,   440,     0,     0,   441,     0,     0,   442,
       0,     0,     0,   443,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   444,     0,     0,   445,   446,     0,   219,
     220,   221,     0,   223,   224,   225,   226,   227,   447,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
       0,   241,   242,   243,     0,     0,   246,   247,   248,   249,
     448,   449,   450,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   451,   452,     0,     0,
       0,     0,     0,     0,     0,   524,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,   453,   454,   455,   456,   457,
       0,   458,     0,   459,   460,   461,   462,   463,   464,   465,
     466,    57,     0,     0,   467,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,     0,    14,     0,     0,   471,   472,     0,
       0,     0,     0,     0,   429,   430,   473,     0,   474,  1274,
     475,   476,     0,   477,   431,   432,   433,   434,   435,     0,
       0,     0,     0,     0,   436,     0,   437,     0,     0,     0,
     438,     0,     0,     0,     0,     0,     0,     0,   439,     0,
       0,     0,     0,     0,   440,     0,     0,   441,     0,     0,
     442,     0,     0,     0,   443,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   444,     0,     0,   445,   446,     0,
     219,   220,   221,     0,   223,   224,   225,   226,   227,   447,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,     0,   241,   242,   243,     0,     0,   246,   247,   248,
     249,   448,   449,   450,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   451,   452,     0,
       0,     0,     0,     0,     0,     0,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,   453,   454,   455,   456,
     457,     0,   458,     0,   459,   460,   461,   462,   463,   464,
     465,   466,    57,     0,     0,   467,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   468,   469,   470,     0,    14,     0,     0,   471,   472,
       0,     0,     0,     0,     0,   429,   430,  1283,     0,   474,
    1284,   475,   476,     0,   477,   431,   432,   433,   434,   435,
       0,     0,     0,     0,     0,   436,     0,   437,     0,     0,
       0,   438,     0,     0,     0,     0,     0,     0,     0,   439,
       0,     0,     0,     0,     0,   440,     0,     0,   441,     0,
       0,   442,     0,     0,     0,   443,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   444,     0,     0,   445,   446,
       0,   219,   220,   221,     0,   223,   224,   225,   226,   227,
     447,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,     0,   241,   242,   243,     0,     0,   246,   247,
     248,   249,   448,   449,   450,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   451,   452,
       0,     0,     0,     0,     0,     0,     0,   524,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,   453,   454,   455,
     456,   457,     0,   458,     0,   459,   460,   461,   462,   463,
     464,   465,   466,    57,     0,     0,   467,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   468,   469,   470,     0,    14,     0,     0,   471,
     472,     0,     0,     0,     0,     0,   429,   430,   473,     0,
     474,  1289,   475,   476,     0,   477,   431,   432,   433,   434,
     435,     0,     0,     0,     0,     0,   436,     0,   437,     0,
       0,     0,   438,     0,     0,     0,     0,     0,     0,     0,
     439,     0,     0,     0,     0,     0,   440,     0,     0,   441,
       0,     0,   442,     0,     0,     0,   443,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,   445,
     446,     0,   219,   220,   221,     0,   223,   224,   225,   226,
     227,   447,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,     0,   241,   242,   243,     0,     0,   246,
     247,   248,   249,   448,   449,   450,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   451,
     452,     0,     0,     0,     0,     0,     0,     0,   524,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,   453,   454,
     455,   456,   457,     0,   458,     0,   459,   460,   461,   462,
     463,   464,   465,   466,    57,     0,     0,   467,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,   470,     0,    14,     0,     0,
     471,   472,     0,     0,     0,     0,     0,   429,   430,   473,
       0,   474,  1343,   475,   476,     0,   477,   431,   432,   433,
     434,   435,     0,     0,     0,     0,     0,   436,     0,   437,
       0,     0,     0,   438,     0,     0,     0,     0,     0,     0,
       0,   439,     0,     0,     0,     0,     0,   440,     0,     0,
     441,     0,     0,   442,     0,     0,     0,   443,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   444,     0,     0,
     445,   446,     0,   219,   220,   221,     0,   223,   224,   225,
     226,   227,   447,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,     0,   241,   242,   243,     0,     0,
     246,   247,   248,   249,   448,   449,   450,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     451,   452,     0,     0,     0,     0,     0,     0,     0,   524,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,   453,
     454,   455,   456,   457,     0,   458,     0,   459,   460,   461,
     462,   463,   464,   465,   466,    57,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   468,   469,   470,     0,    14,     0,
       0,   471,   472,     0,     0,     0,     0,     0,   429,   430,
     473,     0,   474,     0,   475,   476,     0,   477,   431,   432,
     433,   434,   435,     0,     0,     0,     0,     0,   436,     0,
     437,     0,     0,     0,   438,     0,     0,     0,     0,     0,
       0,     0,   439,     0,     0,     0,     0,     0,   440,     0,
       0,   441,     0,     0,   442,     0,     0,     0,   443,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   444,     0,
       0,   445,   446,     0,   219,   220,   221,     0,   223,   224,
     225,   226,   227,   447,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,     0,   241,   242,   243,     0,
       0,   246,   247,   248,   249,   448,   449,   450,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   451,   452,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
     453,   454,   455,   456,   457,     0,   458,     0,   459,   460,
     461,   462,   463,   464,   465,   466,    57,     0,     0,   467,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,   469,   470,     0,    14,
       0,     0,   471,   472,     0,     0,     0,     0,     0,   429,
     430,   473,   534,   474,     0,   475,   476,     0,   477,   431,
     432,   433,   434,   435,     0,     0,     0,     0,     0,   436,
       0,   437,     0,     0,     0,   438,     0,     0,     0,     0,
       0,     0,     0,   439,     0,     0,     0,     0,     0,   440,
       0,     0,   441,     0,     0,   442,     0,     0,     0,   443,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   444,
       0,     0,   445,   446,     0,   219,   220,   221,     0,   223,
     224,   225,   226,   227,   447,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,     0,   241,   242,   243,
       0,     0,   246,   247,   248,   249,   448,   449,   450,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   451,   452,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,   453,   454,   455,   456,   457,     0,   458,     0,   459,
     460,   461,   462,   463,   464,   465,   466,    57,     0,     0,
     467,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   468,   469,   470,     0,
      14,     0,     0,   471,   472,     0,     0,     0,     0,     0,
     429,   430,   473,   720,   474,     0,   475,   476,     0,   477,
     431,   432,   433,   434,   435,     0,     0,     0,     0,     0,
     436,     0,   437,     0,     0,     0,   438,     0,     0,     0,
       0,     0,     0,     0,   439,     0,     0,     0,     0,     0,
     440,     0,     0,   441,     0,     0,   442,     0,     0,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,   445,   446,     0,   219,   220,   221,     0,
     223,   224,   225,   226,   227,   447,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,     0,   241,   242,
     243,     0,     0,   246,   247,   248,   249,   448,   449,   450,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   451,   452,     0,     0,     0,     0,     0,
       0,     0,   930,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,   453,   454,   455,   456,   457,     0,   458,     0,
     459,   460,   461,   462,   463,   464,   465,   466,    57,     0,
       0,   467,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   468,   469,   470,
       0,    14,     0,     0,   471,   472,     0,     0,     0,     0,
       0,   429,   430,   473,     0,   474,     0,   475,   476,  1021,
     477,   431,   432,   433,   434,   435,     0,     0,     0,     0,
       0,   436,     0,   437,     0,     0,     0,   438,     0,     0,
       0,     0,     0,     0,     0,   439,     0,     0,     0,     0,
       0,   440,     0,     0,   441,     0,     0,   442,     0,     0,
       0,   443,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   444,     0,     0,   445,   446,     0,   219,   220,   221,
       0,   223,   224,   225,   226,   227,   447,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,     0,   241,
     242,   243,     0,     0,   246,   247,   248,   249,   448,   449,
     450,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   451,   452,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,   456,   457,     0,   458,
       0,   459,   460,   461,   462,   463,   464,   465,   466,    57,
       0,     0,   467,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   468,   469,
     470,     0,    14,     0,     0,   471,   472,     0,     0,     0,
       0,     0,   429,   430,   473,     0,   474,     0,   475,   476,
       0,   477,   431,   432,   433,   434,   435,     0,     0,     0,
       0,     0,   436,     0,   437,     0,     0,     0,   438,     0,
       0,     0,     0,     0,     0,     0,   439,     0,     0,     0,
       0,     0,   440,     0,     0,   441,     0,     0,   442,     0,
       0,     0,   443,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   444,     0,     0,   445,   446,     0,   219,   220,
     221,     0,   223,   224,   225,   226,   227,   447,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,     0,
     241,   242,   243,     0,     0,   246,   247,   248,   249,   448,
     449,   450,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   451,   452,     0,     0,     0,
       0,     0,     0,     0,   930,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,   453,   454,   455,   456,   457,     0,
     458,     0,   459,   460,   461,   462,   463,   464,   465,   466,
      57,     0,     0,   467,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   468,
     469,   470,     0,    14,     0,     0,   471,   472,     0,     0,
       0,     0,     0,   429,   430,  1209,     0,   474,     0,   475,
     476,     0,   477,   431,   432,   433,   434,   435,     0,     0,
       0,     0,     0,   436,     0,   437,     0,     0,     0,   438,
       0,     0,     0,     0,     0,     0,     0,   439,     0,     0,
       0,     0,     0,   440,     0,     0,   441,     0,     0,   442,
       0,     0,     0,   443,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   444,     0,     0,   445,   446,     0,   219,
     220,   221,     0,   223,   224,   225,   226,   227,   447,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
       0,   241,   242,   243,     0,     0,   246,   247,   248,   249,
     448,   449,   450,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   451,   452,     0,     0,
       0,     0,     0,     0,     0,  1218,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,   453,   454,   455,   456,   457,
       0,   458,     0,   459,   460,   461,   462,   463,   464,   465,
     466,    57,     0,     0,   467,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,     0,    14,     0,     0,   471,   472,     0,
       0,     0,     0,     0,   429,   430,   473,     0,   474,     0,
     475,   476,     0,   477,   431,   432,   433,   434,   435,     0,
       0,     0,     0,     0,   436,     0,   437,     0,     0,     0,
     438,     0,     0,     0,     0,     0,     0,     0,   439,     0,
       0,     0,     0,     0,   440,     0,     0,   441,     0,     0,
     442,     0,     0,     0,   443,     0,     0,     0,     0,     0,
    1221,     0,     0,     0,   444,     0,     0,   445,   446,     0,
     219,   220,   221,     0,   223,   224,   225,   226,   227,   447,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,     0,   241,   242,   243,     0,     0,   246,   247,   248,
     249,   448,   449,   450,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   451,   452,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,   453,   454,   455,   456,
     457,     0,   458,     0,   459,   460,   461,   462,   463,   464,
     465,   466,    57,     0,     0,   467,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   468,   469,   470,     0,    14,     0,     0,   471,   472,
       0,     0,     0,     0,     0,   429,   430,   473,     0,   474,
       0,   475,   476,     0,   477,   431,   432,   433,   434,   435,
       0,     0,     0,     0,     0,   436,     0,   437,     0,     0,
       0,   438,     0,     0,     0,     0,     0,     0,     0,   439,
       0,     0,     0,     0,     0,   440,     0,     0,   441,     0,
       0,   442,     0,     0,     0,   443,     0,     0,  1227,     0,
       0,     0,     0,     0,     0,   444,     0,     0,   445,   446,
       0,   219,   220,   221,     0,   223,   224,   225,   226,   227,
     447,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,     0,   241,   242,   243,     0,     0,   246,   247,
     248,   249,   448,   449,   450,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   451,   452,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,   453,   454,   455,
     456,   457,     0,   458,     0,   459,   460,   461,   462,   463,
     464,   465,   466,    57,     0,     0,   467,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   468,   469,   470,     0,    14,     0,     0,   471,
     472,     0,     0,     0,     0,     0,   429,   430,   473,     0,
     474,     0,   475,   476,     0,   477,   431,   432,   433,   434,
     435,     0,     0,     0,     0,     0,   436,     0,   437,     0,
       0,     0,   438,     0,     0,     0,     0,     0,     0,     0,
     439,     0,     0,     0,     0,     0,   440,     0,     0,   441,
       0,     0,   442,     0,     0,     0,   443,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,   445,
     446,     0,   219,   220,   221,     0,   223,   224,   225,   226,
     227,   447,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,     0,   241,   242,   243,     0,     0,   246,
     247,   248,   249,   448,   449,   450,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   451,
     452,     0,     0,     0,     0,     0,     0,     0,  1230,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,   453,   454,
     455,   456,   457,     0,   458,     0,   459,   460,   461,   462,
     463,   464,   465,   466,    57,     0,     0,   467,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,   470,     0,    14,     0,     0,
     471,   472,     0,     0,     0,     0,     0,   429,   430,   473,
       0,   474,     0,   475,   476,     0,   477,   431,   432,   433,
     434,   435,     0,     0,     0,     0,     0,   436,     0,   437,
       0,     0,     0,   438,     0,     0,     0,     0,     0,     0,
       0,   439,     0,     0,     0,     0,     0,   440,     0,     0,
     441,     0,     0,   442,     0,     0,     0,   443,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   444,     0,     0,
     445,   446,     0,   219,   220,   221,     0,   223,   224,   225,
     226,   227,   447,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,     0,   241,   242,   243,     0,     0,
     246,   247,   248,   249,   448,   449,   450,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     451,   452,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,   453,
     454,   455,   456,   457,     0,   458,     0,   459,   460,   461,
     462,   463,   464,   465,   466,    57,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   468,   469,   470,     0,    14,     0,
       0,   471,   472,     0,     0,     0,     0,     0,   429,   430,
     473,     0,   474,  1466,   475,   476,     0,   477,   431,   432,
     433,   434,   435,     0,     0,     0,     0,     0,   436,     0,
     437,     0,     0,     0,   438,     0,     0,     0,     0,     0,
       0,     0,   439,     0,     0,     0,     0,     0,   440,     0,
       0,   441,     0,     0,   442,     0,     0,     0,   443,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   444,     0,
       0,   445,   446,     0,   219,   220,   221,     0,   223,   224,
     225,   226,   227,   447,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,     0,   241,   242,   243,     0,
       0,   246,   247,   248,   249,   448,   449,   450,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   451,   452,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
     453,   454,   455,   456,   457,     0,   458,     0,   459,   460,
     461,   462,   463,   464,   465,   466,    57,     0,     0,   467,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,   469,   470,     0,    14,
       0,     0,   471,   472,     0,     0,     0,     0,     0,   429,
     430,   473,     0,   474,     0,   475,   476,     0,   477,   431,
     432,   433,   434,   435,     0,     0,     0,     0,     0,   436,
    1035,   437,  1036,     0,     0,   438,     0,     0,     0,     0,
       0,     0,     0,   439,     0,     0,     0,     0,     0,   440,
       0,     0,   441,     0,     0,   442,  1041,     0,     0,   443,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   444,
       0,     0,   445,   446,     0,   219,   220,   221,     0,   223,
     224,   225,   226,   227,   447,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,     0,   241,   242,   243,
       0,     0,   246,   247,   248,   249,   448,   449,   450,  1046,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   451,   452,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,   453,   454,   455,   456,   457,     0,   458,     0,   459,
     460,   461,   462,   463,   464,   465,   466,    57,     0,     0,
     467,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   468,   469,   470,     0,
      14,     0,     0,   471,   472,     0,     0,     0,   429,   430,
       0,     0,   473,     0,   474,     0,   475,   476,   431,   432,
     433,   434,   435,     0,     0,     0,     0,     0,   436,     0,
     437,     0,     0,     0,   438,     0,     0,     0,     0,     0,
       0,     0,   439,     0,     0,     0,     0,     0,   440,     0,
       0,   441,     0,     0,   442,     0,     0,     0,   443,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   444,     0,
       0,   445,   446,     0,   219,   220,   221,     0,   223,   224,
     225,   226,   227,   447,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,     0,   241,   242,   243,     0,
       0,   246,   247,   248,   249,   448,   449,   450,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   451,   452,     0,     0,     0,     0,     0,     0,     0,
     765,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
     453,   454,   455,   456,   457,     0,   458,     0,   459,   460,
     461,   462,   463,   464,   465,   466,    57,     0,     0,   467,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,   469,   470,     0,    14,
       0,     0,   471,   472,     0,     0,     0,   429,   430,     0,
       0,   473,     0,   474,     0,   475,   476,   431,   432,   433,
     434,   435,     0,     0,   887,     0,     0,   436,     0,   437,
       0,     0,     0,   438,     0,     0,     0,     0,     0,     0,
       0,   439,     0,     0,     0,     0,     0,   440,     0,     0,
     441,     0,     0,   442,     0,     0,     0,   443,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   444,     0,     0,
     445,   446,     0,   219,   220,   221,     0,   223,   224,   225,
     226,   227,   447,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,     0,   241,   242,   243,     0,     0,
     246,   247,   248,   249,   448,   449,   450,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     451,   452,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,   453,
     454,   455,   456,   457,     0,   458,     0,   459,   460,   461,
     462,   463,   464,   465,   466,    57,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   468,   469,   470,     0,    14,     0,
       0,   471,   472,     0,     0,     0,   429,   430,     0,     0,
     473,     0,   474,     0,   475,   476,   431,   432,   433,   434,
     435,     0,     0,     0,     0,     0,   436,     0,   437,     0,
       0,     0,   438,     0,     0,     0,     0,     0,     0,     0,
     439,     0,     0,     0,     0,     0,   440,     0,     0,   441,
       0,     0,   442,     0,     0,     0,   443,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,   445,
     446,     0,   219,   220,   221,     0,   223,   224,   225,   226,
     227,   447,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,     0,   241,   242,   243,     0,     0,   246,
     247,   248,   249,   448,   449,   450,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   451,
     452,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,   453,   454,
     455,   456,   457,     0,   458,     0,   459,   460,   461,   462,
     463,   464,   465,   466,    57,     0,   213,   467,     0,     0,
       0,     0,   214,     0,     0,     0,     0,     0,   215,     0,
       0,     0,     0,   468,   469,   470,     0,    14,   216,     0,
     471,   472,     0,     0,     0,     0,   217,     0,     0,   473,
       0,   474,     0,   475,   476,     0,     0,     0,     0,     0,
       0,   218,     0,     0,     0,     0,     0,     0,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,     0,     0,     0,     0,     0,     0,     0,     0,   213,
       0,     0,     0,     0,     0,   214,     0,     0,     0,     0,
       0,   215,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   216,     0,     0,     0,     0,    56,     0,     0,   217,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   252,
       0,     0,     0,     0,   218,     0,     0,     0,     0,     0,
      57,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,     0,     0,     0,     0,     0,   253,
       0,     0,     0,     0,   537,   213,     0,     0,     0,     0,
       0,   214,     0,     0,     0,     0,     0,   215,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   216,     0,    56,
       0,     0,     0,     0,     0,   217,     0,     0,     0,     0,
       0,     0,   252,     0,     0,     0,     0,     0,     0,     0,
     218,     0,     0,   702,     0,    13,     0,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
       0,     0,   253,     0,    16,     0,     0,     0,   213,     0,
       0,     0,     0,     0,   214,     0,     0,     0,     0,     0,
     215,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     216,     0,     0,     0,     0,    56,     0,     0,   217,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   252,     0,
       0,     0,     0,   218,     0,     0,     0,     0,     0,    57,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,     0,     0,     0,     0,   860,   253,     0,
       0,     0,     0,     0,     0,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   353,     0,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   252,     0,     0,     0,     0,     0,     0,     0,   219,
     220,   221,   702,   223,   224,   225,   226,   227,   447,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
       0,   241,   242,   243,     0,     0,   246,   247,   248,   249,
       0,   654,   655,     0,     0,     0,     0,     0,     0,     0,
       0,   253,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
       0,     0,   372,   373,   374,   654,   655,   375,   376,   377,
     378,   379,     0,     0,   380,   381,   382,   383,   384,   385,
     386,     0,   861,     0,     0,     0,     0,     0,     0,     0,
       0,   862,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   387,
       0,   388,   389,   390,   391,   392,   393,   394,   395,   396,
     397,     0,     0,   398,   399,     0,   656,   657,   658,   659,
     660,   400,   401,   661,   662,   663,   664,     0,   665,   666,
     667,   668,   669,     0,   670,   671,     0,     0,  -845,     0,
     673,   674,   675,   654,   655,     0,  -845,     0,     0,     0,
     656,   657,   658,   659,   660,     0,     0,   661,   662,   663,
     664,     0,   665,   666,   667,   668,   669,     0,   670,   671,
     654,   655,     0,     0,   673,   677,   675,   678,   679,   680,
     681,   682,   683,   684,   685,   686,   687,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   688,   689,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   678,   679,   680,   681,   682,   683,   684,   685,   686,
     687,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   688,   689,     0,     0,     0,     0,     0,   656,   657,
     658,   659,   660,     0,     0,   661,   662,   663,   664,     0,
     665,   666,   667,   668,   669,     0,   670,   671,   654,   655,
       0,     0,   673,     0,     0,   656,   657,   658,   659,   660,
       0,     0,   661,   662,   663,   664,     0,   665,   666,   667,
     668,   669,     0,   670,   671,   654,   655,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   678,
     679,   680,   681,   682,   683,   684,   685,   686,   687,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   688,
     689,     0,     0,     0,     0,     0,   678,   679,   680,   681,
     682,   683,   684,   685,   686,   687,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   688,   689,     0,     0,
       0,     0,     0,   656,   657,   658,   659,   660,     0,     0,
     661,   662,   663,   664,     0,   665,   666,   667,   668,   669,
       0,   670,   671,   654,   655,     0,     0,     0,     0,     0,
     656,   657,   658,   659,   660,     0,     0,   661,   662,   663,
     664,     0,   665,   666,   667,   668,   669,     0,   670,   671,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   679,   680,   681,   682,   683,
     684,   685,   686,   687,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   688,   689,  1083,     0,     0,     0,
       0,     0,     0,   680,   681,   682,   683,   684,   685,   686,
     687,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   688,   689,     0,     0,     0,     0,     0,   656,   657,
     658,   659,   660,     0,     0,   661,   662,   663,   664,     0,
     665,   666,   667,   668,   669,     0,   670,   671,   219,   220,
     221,     0,   223,   224,   225,   226,   227,   447,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,     0,
     241,   242,   243,     0,     0,   246,   247,   248,   249,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   681,   682,   683,   684,   685,   686,   687,     0,
       0,   272,     0,     0,     0,     0,     0,     0,     0,   688,
     689,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1084,     0,     0,     0,     0,     0,   273,     0,   274,
    1085,   275,   276,   277,   278,   279,     0,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,     0,   291,
     292,   293,     0,     0,   294,   295,   296,   297,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   298,   299,   219,   220,   221,     0,
     223,   224,   225,   226,   227,   447,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,     0,   241,   242,
     243,     0,     0,   246,   247,   248,   249,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   300,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   899,   900,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   901,
       0,     0,     0,     0,     0,   273,     0,   274,   902,   275,
     276,   277,   278,   279,     0,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,     0,   291,   292,   293,
       0,     0,   294,   295,   296,   297,     0,     0,     0,     0,
       0,     0,   903,   904,   273,     0,   274,     0,   275,   276,
     277,   278,   279,     0,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,     0,   291,   292,   293,     0,
       0,   294,   295,   296,   297,     0,   273,     0,   274,     0,
     275,   276,   277,   278,   279,     0,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   542,   291,   292,
     293,     0,     0,   294,   295,   296,   297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   544,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   727
};

static const yytype_int16 yycheck[] =
{
       1,    14,    15,   100,   334,   574,   168,   570,   433,   141,
     142,   767,   433,   945,  1037,   582,  1170,  1010,   644,    20,
      21,    22,   474,   846,   639,   640,  1249,   188,   541,   512,
     697,   514,   699,   516,   701,   540,    79,   934,    33,   351,
     352,     8,  1337,   813,    19,    33,     5,   928,    49,    62,
      63,    64,    46,   188,   816,  1543,   953,    22,    34,   855,
      20,  1374,   346,   126,    20,    20,   126,   151,  1589,   521,
       0,   148,    20,    40,   126,  1480,   163,     7,    19,    20,
     126,   140,   141,   142,  1016,  1529,   163,    63,   101,   102,
     103,   104,    57,   165,   148,   151,   711,     8,   165,   148,
      30,   156,    32,  1589,    34,   177,   198,     5,     6,   163,
      40,   203,   106,   176,   163,   202,   176,   201,   181,   203,
      50,   181,   139,   200,   176,    36,    56,  1648,   205,   181,
     176,   107,   204,   200,  1539,   181,  1580,   200,     7,   423,
     200,   708,   201,   102,   103,   712,   200,   203,   200,   311,
      80,   603,  1447,  1448,   200,    33,   132,    15,    16,   176,
     203,   613,  1648,   139,   616,  1686,   792,   163,  1463,   173,
    1465,   303,   102,   103,   180,   188,   308,    15,    16,   126,
     312,    50,    60,    61,   179,   800,  1499,   163,   803,  1589,
     175,   179,   644,  1589,   200,  1683,   811,   201,   165,   814,
    1686,   202,   998,   198,   200,   180,  1229,   977,   777,  1697,
    1203,   346,   207,   173,   190,    62,   978,   173,   173,   845,
     185,   180,   163,   174,   200,   173,  1521,  1522,  1125,   176,
     127,   198,   197,  1528,   801,   165,   548,  1118,   690,   206,
     253,   200,   129,   130,  1141,   173,   124,   672,  1648,   127,
     128,   672,  1648,    33,  1589,   133,   186,   173,     5,     6,
    1518,   173,   127,  1195,   173,   426,   427,   428,   198,  1527,
     313,   176,   165,   201,   204,  1498,   173,   174,    25,   176,
      60,    61,   179,   173,    31,   201,  1686,   165,   423,   421,
    1686,   426,   427,   428,   127,   204,   174,   175,   156,   165,
     133,   179,  1456,   161,   182,   163,   139,   200,   166,   174,
     197,   198,   177,  1648,  1572,  1573,   139,   146,   156,  1342,
     604,    68,    69,   161,   127,   163,   204,   154,   166,   207,
     133,    21,    22,   346,   163,   619,   163,   173,   204,   163,
     792,   174,   200,   969,   124,   203,   507,   508,   128,   163,
     511,  1686,   513,  1376,   515,   102,   103,   186,   154,   926,
     200,  1353,   523,   173,   173,   201,  1033,   163,   197,   852,
    1599,   174,   507,   508,   201,  1307,   511,   163,   513,   165,
     515,    57,   517,   180,   164,   165,  1209,    63,   523,   958,
     199,   201,   139,   173,   148,   175,   557,   558,   163,   179,
    1392,   181,   182,   200,   200,   540,   973,  1636,  1023,   163,
     423,   165,   925,   426,   427,   428,   163,   922,   204,   107,
     433,   434,   557,   558,   204,   938,   146,   207,   118,   119,
     997,   936,   140,   180,   142,   173,   126,   139,   128,   129,
     130,   131,   132,   163,   173,   126,   173,   173,   129,   130,
     204,   198,   206,  1619,    85,   181,   127,   156,   205,    33,
    1283,   163,   133,   201,   176,   185,   186,   173,   180,   604,
     199,   164,    21,    22,   201,   185,  1232,   197,   204,  1645,
     173,   173,   127,   127,   619,   127,    60,    61,   133,   133,
     173,   133,   776,   199,   507,   508,   127,   128,   511,   130,
     513,  1494,   515,   174,   517,   789,   790,   197,   198,   201,
     523,   643,    21,    22,   173,   799,   197,   198,   201,   654,
     655,   805,   806,   173,   808,   165,   810,   540,   812,   174,
     174,   185,   174,   176,   669,  1202,   179,  1020,   163,   173,
     199,   180,   127,   180,   557,   558,   173,  1479,   133,   710,
     124,   201,   565,   688,   128,   167,    62,    63,    64,   164,
     165,   200,   173,   200,   204,   173,   206,   201,   173,   118,
     119,  1594,   177,   574,   201,   710,   173,   126,   163,   128,
     129,   130,   131,   132,   173,   173,   173,   173,   148,   174,
     201,   604,  1075,   201,   179,   101,   102,   103,   104,   204,
    1167,   175,  1269,   163,   201,   179,   619,  1630,   182,   118,
     119,   163,   201,   201,   201,   201,   173,   126,   180,   180,
     129,   130,   131,   132,    33,   163,    47,   788,  1080,   642,
     791,   173,   645,   207,   795,   184,   185,   186,   200,   200,
    1092,   776,   199,   163,  1667,  1097,    67,  1670,   197,   198,
     139,    60,    61,   788,   789,   790,   791,   199,   139,   148,
     795,    75,    57,   173,   799,    79,   176,   148,    63,   179,
     805,   806,   153,   808,   163,   810,   107,   812,   813,    93,
      94,    79,   163,   165,    98,    99,   100,   101,   197,   198,
     204,   205,   976,   154,  1517,  1518,    94,   710,   982,   180,
    1523,    99,   163,   101,  1527,    12,  1529,   177,   107,    21,
      22,   181,   996,   164,   165,   124,    23,    24,   127,   128,
     164,   165,   173,   724,   133,   726,    57,   164,   165,   173,
     181,   321,    63,  1185,   154,  1019,   173,   181,   163,   164,
    1192,   331,  1565,   163,   181,   163,   164,   154,   173,  1572,
    1573,   205,   887,   204,   344,   173,   163,  1580,   173,   920,
     204,   176,   923,   776,   179,   174,   175,   204,   929,   203,
     179,   154,   154,   182,   197,   788,   789,   790,   791,   780,
     163,   163,   795,    57,    57,   920,   799,   922,   923,    63,
      63,   163,   805,   806,   929,   808,    57,   810,   207,   812,
     813,   936,    63,  1559,   116,   117,   118,   119,   120,   167,
     168,   123,   124,   125,   126,  1638,   128,   129,   130,   131,
     132,    57,   134,   135,    33,   174,   138,    63,   140,   141,
     142,   177,    10,    11,   146,   181,  1261,   164,   165,   177,
    1261,   976,   977,   181,   167,   177,   173,   982,    33,   181,
     163,    60,    61,   854,   181,   177,   177,   163,   177,   181,
     181,   996,   181,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    60,    61,   204,   177,   177,
    1239,  1240,   181,   181,  1019,   197,   198,   190,   176,   108,
     109,   110,   111,   112,   113,   114,   115,   177,   140,   177,
     165,   181,  1354,   181,  1660,   163,   203,   920,   127,   922,
     923,  1526,   163,  1045,   133,   124,   929,   202,   203,   128,
     205,   164,   165,   936,   143,   144,   145,   164,   165,    57,
     173,   164,   165,  1689,   524,   936,   173,   938,   181,   124,
     173,   164,   165,   128,   181,    66,   536,   164,   181,   950,
     173,   490,  1567,   492,   493,   174,   176,   958,   181,   960,
     106,   204,   203,   976,   977,   163,   175,   204,   180,   982,
     179,   204,   181,   182,   180,  1136,   167,   168,   169,   170,
     180,   204,   200,   996,   180,  1147,  1270,  1550,   578,   200,
     175,   140,   141,   142,   179,   200,   181,   182,   207,    21,
      22,  1136,   205,   164,   165,   180,  1019,   180,  1460,   167,
     168,   169,   173,   492,   493,   605,   606,   180,   180,   609,
     181,   611,   207,   167,   168,   169,   180,  1596,   180,   198,
    1482,   164,   622,   623,   624,   625,   626,   627,   759,   760,
     761,  1042,   177,   204,    53,    54,    55,    35,   205,  1050,
    1051,    35,  1184,   200,   108,   109,   110,   111,   112,   113,
     114,   115,  1063,  1632,  1065,  1066,  1067,  1068,  1069,   163,
     163,   180,  1073,   198,   205,   163,   199,   667,    22,   133,
     163,   199,   138,   205,   176,  1217,   163,    21,    22,   143,
     144,   145,   200,   180,   116,   117,   118,   119,   120,   689,
     180,   123,   124,   125,   126,   180,   128,   129,   130,   131,
     132,  1272,   134,   135,   180,   180,   200,   180,   140,   141,
     142,   200,   180,  1136,  1454,   180,   716,   200,   200,   200,
     200,   200,   200,   181,   198,  1270,   203,  1272,   200,   200,
    1592,   201,   201,   163,   201,   173,    43,   199,   201,   199,
     201,  1152,   181,   175,   200,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   755,   204,   200,   205,   200,
     180,    13,    21,    22,   180,   197,   198,   180,   180,   200,
     199,   173,   200,   773,   118,   119,   201,     4,   163,   779,
     203,   163,   126,   176,   128,   129,   130,   131,   132,   199,
     163,   163,   163,   163,   794,    33,   163,   173,   201,   200,
      33,    21,    22,   180,   201,   201,   201,  1378,     1,   201,
     201,   201,   201,   201,   201,   201,   200,   200,   180,   200,
     200,   200,    60,    61,   199,   825,   200,    60,    61,   181,
     830,   200,   832,  1378,   834,   200,   200,   200,   182,   183,
     184,   185,   186,   199,   844,   181,   181,  1270,   201,  1272,
     200,   174,  1423,   197,   198,   201,    43,   116,   117,   118,
     119,   120,   200,   174,   123,   124,   125,   126,  1439,   128,
     129,   130,   131,   132,  1416,   134,   135,   201,  1423,   138,
    1387,   140,   141,   142,    43,   201,   124,   146,   201,   201,
     128,   124,   201,   201,  1439,   128,   116,   117,   118,   119,
     900,  1595,   200,   200,   904,  1316,   126,  1318,   128,   129,
     130,   131,   132,   201,   134,   135,   175,   201,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   176,   200,
     930,    43,   163,   201,   200,   200,   206,   175,   197,   198,
     201,   179,   175,   181,   182,   163,   179,   163,   181,   182,
     156,    10,    13,     9,    42,  1378,    66,   180,   200,   959,
     200,   200,   182,   183,   184,   185,   186,   199,   163,   207,
     199,   163,   206,  1384,   207,   163,   206,   197,   198,   163,
     206,   163,     8,   983,   163,   163,   201,   200,   171,   163,
     201,   201,   201,    33,   163,   163,   200,   163,   163,    14,
    1423,   174,   174,  1545,   156,   176,    33,  1549,   206,   200,
     200,   200,   174,    43,    37,   201,  1439,  1017,  1018,   174,
      60,    61,    67,   181,   201,   201,   201,    33,  1028,    70,
     163,   201,    43,    60,    61,  1035,    33,   200,  1038,   206,
     200,   181,   163,   200,  1044,   201,  1046,   200,   163,   200,
    1595,  1593,   200,   200,    60,    61,   201,  1480,   201,   200,
     167,   200,   200,    60,    61,    33,   201,   201,   204,   201,
     201,   200,   200,   200,   200,   205,   201,  1077,   163,  1079,
      33,  1081,   201,   201,   124,   451,   452,  1498,   128,   206,
     201,  1091,    60,    61,    12,   201,   201,   124,    33,  1099,
    1511,   128,   468,   469,   470,   471,   472,  1649,   201,   201,
     201,   173,   201,  1655,   204,   173,  1539,   201,   124,  1661,
     201,   201,   128,  1123,   201,    60,    61,   124,   201,   201,
     201,   128,   201,  1133,   204,   175,   204,   200,  1138,   179,
    1140,   181,   182,    53,   201,  1687,   201,   199,   175,   199,
     201,  1151,   179,   206,   181,   182,   124,   205,   200,   206,
     128,    78,   206,   206,     1,   185,   205,   207,    81,   175,
    1170,   628,  1595,   179,  1696,   181,   182,   726,   175,  1590,
     207,  1694,   179,  1240,   181,   182,   726,  1598,  1188,   124,
     209,   645,   954,   128,   560,   710,  1196,  1197,  1198,   649,
    1372,   207,     1,  1513,  1563,  1516,  1442,   175,  1564,  1564,
     207,   179,    52,   181,   182,  1141,   433,   953,  1218,   953,
     228,  1221,   305,    -1,  1635,   433,   433,   605,   433,    -1,
    1230,   433,    33,   433,    -1,    -1,    -1,    -1,    -1,   207,
     175,  1652,    -1,    -1,   179,    -1,   181,   182,    33,    -1,
      -1,    -1,    21,    22,    -1,  1666,    -1,  1668,    -1,    60,
      61,    -1,  1673,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   207,    -1,    -1,    60,    61,    -1,  1278,  1690,
    1280,  1692,    -1,    -1,    -1,    -1,  1286,    -1,    -1,    -1,
     656,   657,    -1,    -1,   660,   661,   662,   663,    -1,   665,
      -1,    -1,   668,   669,   670,   671,   672,   673,   674,   675,
     676,   677,   678,   679,   680,   681,   682,   683,   684,   685,
     686,   687,    -1,   124,  1324,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,  1341,    -1,   128,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,  1355,  1356,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,   179,  1379,
     181,   182,    -1,    -1,    -1,  1385,  1386,    -1,    -1,    -1,
     175,  1391,    -1,    -1,   179,    -1,   181,   182,    -1,   765,
      -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,
      -1,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,   197,   198,
      -1,  1431,    -1,    -1,    -1,  1435,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,  1456,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   854,    -1,
      -1,    -1,    -1,    -1,    -1,  1495,    -1,    33,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
    1520,    93,    94,    95,    60,    61,    98,    99,   100,   101,
      -1,    -1,  1532,    -1,    -1,    -1,  1536,  1537,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,   154,  1583,   950,    -1,    -1,    -1,   124,    -1,
      -1,   163,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,   175,    -1,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    -1,    -1,    -1,
    1620,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,
      -1,    -1,    -1,  1633,    -1,    -1,    -1,    -1,    -1,   175,
      -1,    -1,  1642,   179,    -1,   181,   182,    -1,    -1,    -1,
      -1,  1651,     1,    -1,    -1,    -1,     5,     6,     7,    -1,
       9,    10,    11,    -1,    13,    -1,    15,    16,    17,    18,
      19,   207,    -1,    -1,    -1,  1675,    25,    26,    27,    28,
      29,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      39,    40,    -1,    42,    -1,    44,    45,    -1,    -1,    48,
      -1,    50,    51,    52,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,  1087,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,
     149,   150,   151,    -1,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   182,   183,   184,    -1,   186,    -1,    -1,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,
      -1,   200,    -1,   202,   203,   204,   205,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1251,  1252,  1253,  1254,  1255,
    1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,  1264,  1265,
    1266,  1267,  1268,    -1,    -1,    -1,     1,    -1,    -1,    -1,
       5,     6,     7,    -1,     9,    10,    11,    -1,    13,    -1,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    39,    40,    -1,    42,    -1,    44,
      45,    -1,    -1,    48,    -1,    50,    51,    52,    -1,    54,
      55,    -1,    -1,    58,    59,    -1,    -1,  1333,  1334,  1335,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1384,    -1,
      -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,   148,   149,   150,   151,    -1,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,
      -1,   186,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   198,    -1,   200,    -1,   202,   203,   204,
     205,   206,     1,    -1,    -1,    -1,     5,     6,     7,    -1,
       9,    10,    11,    -1,    13,    -1,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      39,    40,    -1,    42,    -1,    44,    45,    -1,    -1,    48,
      -1,    50,    51,    52,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,  1589,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,  1598,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,
     149,   150,   151,    -1,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,   165,   166,    -1,    -1,
      -1,    -1,  1648,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   182,   183,   184,    -1,   186,    -1,    -1,
     189,   190,    -1,    -1,    -1,    -1,    -1,  1673,    -1,   198,
      -1,   200,    -1,   202,   203,   204,   205,   206,    -1,     1,
    1686,    -1,    -1,     5,     6,     7,  1692,     9,    10,    11,
      -1,    13,    -1,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    40,    -1,
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
      -1,    -1,    38,    39,    40,    -1,    42,    -1,    44,    45,
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
      40,    -1,    42,    -1,    44,    45,    -1,    -1,    48,    -1,
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
     160,   161,   162,   163,    -1,   165,   166,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   182,   183,   184,    -1,   186,    -1,    -1,   189,
     190,    -1,    -1,    -1,    -1,    60,    61,    -1,   198,    -1,
     200,    -1,   202,   203,   204,   205,   206,     5,     6,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,   124,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    70,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
     175,    -1,    -1,    -1,   179,    -1,   181,   182,    -1,    -1,
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   207,    -1,    -1,    -1,    -1,    -1,    -1,   147,
     148,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   182,   183,   184,    -1,   186,    -1,
      -1,   189,   190,    -1,    -1,    -1,    -1,    -1,     5,     6,
     198,    -1,   200,   201,   202,   203,    13,   205,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,   186,
      -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,     5,
       6,   198,    -1,   200,    -1,   202,   203,    13,   205,    15,
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
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    70,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,
      -1,   186,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,
      -1,     5,     6,   198,    -1,   200,    -1,   202,   203,    -1,
     205,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
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
      -1,    -1,    -1,     5,     6,   198,    -1,   200,   201,   202,
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
      -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,
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
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
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
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
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
       6,   198,   199,   200,    -1,   202,   203,    -1,   205,    15,
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
       5,     6,   198,   199,   200,    -1,   202,   203,    -1,   205,
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
      -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,   148,   149,   150,   151,    -1,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,
      -1,   186,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,
      -1,     5,     6,   198,    -1,   200,    -1,   202,   203,    13,
     205,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,
      -1,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     182,   183,   184,    -1,   186,    -1,    -1,   189,   190,    -1,
      -1,    -1,    -1,    -1,     5,     6,   198,    -1,   200,    -1,
     202,   203,    -1,   205,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
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
      -1,   202,   203,    -1,   205,    15,    16,    17,    18,    19,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,
     150,   151,    -1,   153,    -1,   155,   156,   157,   158,   159,
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
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,
     149,   150,   151,    -1,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   182,   183,   184,    -1,   186,    -1,    -1,
     189,   190,    -1,    -1,    -1,    -1,    -1,     5,     6,   198,
      -1,   200,    -1,   202,   203,    -1,   205,    15,    16,    17,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,   150,   151,    -1,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,   186,
      -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,     5,
       6,   198,    -1,   200,    -1,   202,   203,    -1,   205,    15,
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
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,   148,   149,   150,   151,    -1,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,
     186,    -1,    -1,   189,   190,    -1,    -1,    -1,     5,     6,
      -1,    -1,   198,    -1,   200,    -1,   202,   203,    15,    16,
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
      -1,    -1,   189,   190,    -1,    -1,    -1,     5,     6,    -1,
      -1,   198,    -1,   200,    -1,   202,   203,    15,    16,    17,
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
      -1,   189,   190,    -1,    -1,    -1,     5,     6,    -1,    -1,
     198,    -1,   200,    -1,   202,   203,    15,    16,    17,    18,
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
     159,   160,   161,   162,   163,    -1,    19,   166,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,   182,   183,   184,    -1,   186,    41,    -1,
     189,   190,    -1,    -1,    -1,    -1,    49,    -1,    -1,   198,
      -1,   200,    -1,   202,   203,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    -1,   139,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
     163,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,   202,
      -1,    -1,    -1,    -1,   207,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,   163,    -1,   165,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,   202,    -1,   204,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    -1,   139,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   163,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    19,   202,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,   163,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,    -1,   128,   129,   130,    21,    22,   133,   134,   135,
     136,   137,    -1,    -1,   140,   141,   142,   143,   144,   145,
     146,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
      -1,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,   189,   190,    -1,   116,   117,   118,   119,
     120,   197,   198,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,   132,    -1,   134,   135,    -1,    -1,   138,    -1,
     140,   141,   142,    21,    22,    -1,   146,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,   132,    -1,   134,   135,
      21,    22,    -1,    -1,   140,   175,   142,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,   132,    -1,   134,   135,    21,    22,
      -1,    -1,   140,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,    -1,    -1,    -1,    -1,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    21,    22,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,   132,    -1,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,   198,    19,    -1,    -1,    -1,
      -1,    -1,    -1,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,   132,    -1,   134,   135,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   181,   182,   183,   184,   185,   186,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   154,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,
     163,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    73,   163,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,    71,    -1,    73,    -1,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,    -1,    71,    -1,    73,    -1,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,   163,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   209,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   165,   186,   198,   204,   211,   212,   216,
     225,   227,   228,   233,   282,   288,   319,   401,   409,   416,
     426,   476,   481,   486,    19,    20,   163,   272,   273,   274,
     156,   234,   235,   175,   230,   231,    57,    63,   406,   407,
     163,   202,   214,   487,   477,   482,   139,   163,   307,    34,
      63,   107,   132,   190,   200,   277,   278,   279,   280,   307,
     211,   211,   211,     8,    36,   427,    62,   397,   174,   173,
     176,   173,   163,   146,   163,   186,   197,   229,   163,   211,
     397,   406,   406,   406,   163,   139,   226,   279,   279,   279,
     200,   140,   141,   142,   173,   199,   107,   287,   417,     5,
       6,   423,    57,    63,   398,    15,    16,   156,   161,   163,
     166,   200,   203,   218,   273,   156,   235,   185,   185,   229,
     185,    22,    57,   185,   197,   232,   408,    57,    63,   213,
     163,   163,   163,   163,   167,   224,   201,   274,   279,   279,
     279,   279,   165,   239,   240,    57,    63,   289,   291,    57,
      63,   410,   107,   107,    57,    63,   424,   205,   402,   167,
     168,   169,   217,    15,    16,   156,   161,   163,   218,   270,
     271,   203,   229,   229,   229,   163,   163,   163,   174,   190,
     215,   176,   437,   240,   240,   167,   201,   165,   292,   163,
     411,   428,   399,   203,   275,   367,   167,   168,   169,   173,
     201,   163,   232,    19,    25,    31,    41,    49,    64,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   152,   202,   307,   431,   433,   434,   438,   444,
     446,   475,    66,    79,    94,    99,   101,   164,   414,   415,
     478,   483,    35,    71,    73,    75,    76,    77,    78,    79,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    93,    94,    95,    98,    99,   100,   101,   118,   119,
     163,   285,   286,   290,   176,   412,   106,   421,   422,   206,
     211,   400,   273,   203,   163,   393,   396,   270,   180,   180,
     180,   200,   180,   180,   200,   437,   180,   180,   180,   180,
     180,   200,   307,   180,   200,    33,    60,    61,   124,   128,
     175,   179,   182,   207,   198,   443,   177,   164,   488,   205,
     205,    21,    22,    38,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   128,   129,   130,   133,   134,   135,   136,   137,
     140,   141,   142,   143,   144,   145,   146,   175,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   189,   190,
     197,   198,    35,    35,   200,   283,   240,    75,    79,    93,
      94,    98,    99,   100,   101,   432,   415,   163,   240,   367,
     240,   273,   173,   176,   179,   391,   447,   453,   455,     5,
       6,    15,    16,    17,    18,    19,    25,    27,    31,    39,
      45,    48,    51,    55,    65,    68,    69,    80,   102,   103,
     104,   118,   119,   147,   148,   149,   150,   151,   153,   155,
     156,   157,   158,   159,   160,   161,   162,   166,   182,   183,
     184,   189,   190,   198,   200,   202,   203,   205,   223,   225,
     299,   307,   312,   324,   331,   334,   337,   341,   343,   345,
     346,   348,   353,   356,   357,   358,   365,   366,   431,   492,
     500,   511,   514,   527,   528,   531,   532,   457,   451,   163,
     180,   459,   461,   463,   465,   467,   469,   471,   473,   357,
     180,   200,   445,   449,   127,   304,   335,   357,    33,   179,
      33,   179,   198,   207,   199,   357,   198,   207,   444,   205,
     479,   484,   163,   286,   163,   286,   163,   199,    22,   163,
     199,   151,   201,   367,   377,   378,   379,   126,   176,   284,
     138,   296,   297,   336,   205,   176,   420,   429,   148,   163,
     392,   395,   240,   163,   444,   127,   133,   174,   390,   475,
     475,   442,   475,   180,   180,   180,   307,   309,   433,   491,
     500,   511,   514,   527,   528,   531,   532,   307,   180,     5,
     102,   103,   180,   200,   180,   200,   200,   180,   180,   200,
     180,   200,   180,   200,   180,   180,   200,    19,   180,   180,
     358,   358,   200,   200,   200,   200,   200,   200,   222,   358,
     358,   358,   358,   358,    13,    49,   304,   154,   163,   335,
     493,   495,   203,   526,   200,   198,   281,   203,   205,   337,
     342,   342,   342,   201,    21,    22,   116,   117,   118,   119,
     120,   123,   124,   125,   126,   128,   129,   130,   131,   132,
     134,   135,   138,   140,   141,   142,   146,   175,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   197,   198,
     200,   475,   475,   201,   181,   439,   475,   283,   475,   283,
     475,   283,   163,   380,   381,   475,   163,   383,   384,   201,
     450,   335,   306,   475,   357,   201,   173,   530,   199,   199,
     199,   357,   489,   380,   382,   383,   385,   163,   286,   108,
     109,   110,   111,   112,   113,   114,   115,   133,   143,   144,
     145,   108,   109,   110,   111,   112,   113,   114,   115,   127,
     133,   143,   144,   145,   174,   200,     7,    50,   318,   204,
     173,   204,   201,   475,   475,   127,   358,   205,   418,   307,
     204,   205,   425,   200,    43,   173,   176,   391,   211,   390,
     357,   181,   181,   181,   164,   173,   210,   211,   441,   501,
     503,   310,   200,   180,   200,   332,   180,   180,   180,   521,
     335,   444,   357,   525,   357,   325,   327,   357,   329,   357,
     523,   335,   509,   512,   335,   180,   505,   444,   357,   357,
     357,   357,   357,   357,   169,   170,   217,   200,    13,   199,
     200,   127,   133,   174,   386,   530,   173,   530,   201,   148,
     153,   180,   307,   347,   240,    70,   198,   201,   335,   495,
     280,     4,   340,   203,   303,   281,    19,   154,   163,   431,
      19,   154,   163,   431,   358,   358,   358,   358,   358,   358,
     163,   358,   154,   163,   357,   358,   358,   431,   358,   358,
     358,   527,   532,   358,   358,   358,   358,    22,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   129,
     130,   154,   163,   197,   198,   354,   431,   357,   201,   335,
     181,   181,   163,   435,   181,   284,   181,   284,   181,   284,
     176,   181,   441,   176,   181,   441,   306,   530,   181,   441,
     127,   357,   199,   163,   436,   211,   247,   248,   247,   248,
     357,   148,   163,   387,   388,   430,   379,   379,   379,   358,
     303,   163,   403,   405,   373,   357,   163,   163,   444,   390,
     357,   211,   448,   454,   456,   475,   444,   444,   475,    70,
     335,   495,   499,   163,   357,   475,   515,   517,   519,   444,
     530,   181,   441,   173,   530,   201,   444,   444,   201,   444,
     201,   444,   530,   444,   381,   530,   507,   384,   181,   201,
     201,   201,   201,   201,   201,   357,   148,   163,   200,   261,
     200,   357,   357,   357,   201,   154,   163,   200,   200,   349,
     351,    13,   305,   525,   163,   201,   495,   493,   173,   201,
     201,   199,   200,   283,     1,    26,    28,    29,    38,    40,
      44,    52,    54,    58,    59,    65,   105,   205,   206,   211,
     236,   237,   246,   257,   258,   260,   262,   263,   264,   265,
     266,   267,   268,   269,   300,   308,   313,   314,   315,   316,
     317,   319,   323,   344,   358,   340,   180,   200,   180,   200,
     200,   200,   199,    19,   154,   163,   431,   176,   154,   163,
     357,   200,   200,   154,   163,   357,     1,   200,   199,   173,
     201,   458,   452,   173,   181,   204,   460,   181,   464,   181,
     468,   181,   475,   472,   380,   475,   474,   383,   181,   201,
     445,   475,   357,   174,   210,   404,   413,   211,   380,   480,
     383,   485,   201,   200,    43,   173,   176,   179,   386,   298,
     174,   404,   413,    40,   165,   206,   282,   374,   201,    43,
     211,   390,   357,   211,   181,   181,   181,   495,   201,   201,
     201,   181,   441,   201,   181,   444,   381,   384,   181,   201,
     200,   444,   357,   201,   181,   181,   181,   181,   201,   181,
     181,   201,   444,   181,   340,   200,   176,   220,   200,    43,
     163,   321,    20,   173,   261,   201,   200,   133,   386,   357,
     357,   444,   283,   200,   206,   530,   201,   173,   199,   198,
     127,   133,   163,   174,   179,   338,   339,   284,   127,   357,
     296,    61,   357,   163,   163,   211,   156,    58,   357,   240,
     127,   357,   301,   211,   211,    10,    10,    11,   244,    13,
       9,    42,   211,   211,   211,   211,   211,   211,    66,   320,
     211,   108,   109,   110,   111,   112,   113,   114,   115,   121,
     122,   127,   133,   136,   137,   143,   144,   145,   174,   283,
     359,   357,   361,   357,   201,   335,   357,   180,   200,   358,
     200,   199,   357,   198,   201,   335,   200,   199,   355,   201,
     335,   163,   440,   163,   462,   466,   470,   445,   357,   163,
     210,   490,   206,   206,   357,   163,   163,   475,   357,   206,
     357,   403,   419,   163,     8,   367,   372,   163,   357,   211,
     502,   504,   311,   201,   200,   163,   333,   181,   181,   181,
     522,   305,   181,   326,   328,   330,   524,   510,   513,   181,
     506,   200,   240,   201,   335,   221,   171,   357,   163,   173,
     201,   335,   163,   200,    20,   133,   386,   357,   357,   357,
     201,   201,   181,   284,   261,   201,   493,   163,   163,   200,
     163,   163,   173,   201,   240,   357,    14,   357,   174,   174,
     176,   156,   296,   357,   303,   200,   200,   198,   275,   276,
     276,   200,   200,   205,   322,   395,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   527,   532,   358,
     358,   358,   358,   358,   358,   358,   284,   444,   201,   475,
     201,   201,   201,   363,   357,   357,   201,   493,   201,   357,
     201,   174,   206,   201,    43,   386,    37,   293,   206,   174,
      57,    63,   370,    67,   371,   211,   211,   200,   200,   357,
     181,   516,   518,   520,   200,   201,   200,   358,   358,   358,
     200,    70,   499,   200,   508,   200,   201,   357,   296,   201,
     219,   201,   163,   201,    43,   321,   335,   357,   357,   201,
     350,   181,    20,   199,   163,   338,   336,   296,   475,   357,
     302,   357,   357,   274,   200,   200,   357,   321,   394,   240,
     181,   181,   475,   201,   201,   199,   201,   357,   163,   357,
     294,   475,    47,   371,    46,   106,   368,   499,   499,   201,
     200,   200,   200,   200,   304,   305,   335,   499,   200,   499,
     201,   167,   204,   163,   201,   201,   133,   386,   347,   352,
     335,   201,   201,   206,   201,   201,   199,   261,   357,   201,
     201,   206,   211,   395,   336,   360,   362,   181,   201,   205,
     211,    33,   369,   368,   370,   200,   493,   496,   497,   498,
     498,   357,   499,   499,   493,   494,   201,   201,   530,   498,
     499,   494,   357,   204,   357,   357,   347,   201,   293,    12,
     245,   240,    20,   201,   240,   176,   391,   364,   303,   375,
     369,   387,   388,   389,   493,   173,   530,   201,   201,   201,
     498,   498,   201,   201,   201,   494,   201,   204,   529,   357,
     204,   246,   313,   314,   315,   316,   358,   211,   259,   335,
     240,   296,   444,   390,   295,   290,   376,   201,   200,   201,
     201,   201,    53,   199,   529,   357,   205,   249,   252,   201,
     296,   390,   357,   206,   211,   290,   493,   357,   199,   529,
     250,    12,    23,    24,   238,   241,   246,   240,   357,   211,
     240,   201,   206,   303,   240,   200,   211,   296,   211,   296,
     251,   242,   357,   206,   205,   253,   256,   201,   293,   254,
     246,   240,   303,   211,   243,   255,   253,   206,   241,   293
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   208,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   210,   210,   211,
     211,   212,   213,   213,   213,   214,   214,   215,   215,   216,
     217,   217,   217,   217,   218,   218,   219,   219,   220,   221,
     220,   222,   222,   222,   223,   224,   224,   226,   225,   227,
     228,   229,   229,   229,   229,   229,   229,   229,   230,   230,
     231,   231,   232,   232,   233,   234,   234,   235,   235,   236,
     237,   237,   238,   238,   239,   239,   240,   240,   241,   242,
     241,   243,   241,   244,   244,   245,   245,   246,   246,   246,
     246,   246,   247,   247,   248,   248,   250,   251,   249,   252,
     249,   254,   255,   253,   256,   253,   258,   259,   257,   260,
     261,   261,   261,   261,   261,   261,   261,   263,   262,   264,
     266,   265,   268,   267,   269,   269,   270,   270,   270,   270,
     270,   270,   271,   271,   272,   272,   272,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   274,   274,   275,   275,
     276,   276,   276,   277,   277,   277,   277,   278,   278,   279,
     279,   279,   279,   279,   279,   279,   280,   280,   281,   281,
     282,   282,   283,   283,   283,   284,   284,   284,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   287,   287,   288,   289,   289,   289,   290,   292,
     291,   293,   294,   295,   293,   297,   298,   296,   299,   299,
     299,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     301,   302,   300,   303,   303,   303,   304,   304,   305,   305,
     306,   306,   307,   307,   307,   308,   308,   310,   311,   309,
     309,   312,   312,   312,   312,   312,   312,   313,   314,   315,
     315,   315,   316,   316,   317,   318,   318,   318,   319,   319,
     320,   320,   321,   321,   322,   322,   323,   323,   323,   325,
     326,   324,   327,   328,   324,   329,   330,   324,   332,   333,
     331,   334,   334,   334,   335,   335,   335,   335,   336,   336,
     336,   337,   337,   337,   338,   338,   338,   338,   338,   339,
     339,   340,   340,   341,   342,   342,   343,   343,   343,   343,
     343,   343,   343,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   344,   345,   345,   346,   346,   347,   347,
     348,   349,   350,   348,   351,   352,   348,   353,   353,   353,
     353,   353,   353,   353,   354,   355,   353,   356,   356,   356,
     356,   356,   356,   356,   357,   357,   357,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   359,   360,   358,   358,   358,   358,   361,
     362,   358,   358,   358,   363,   364,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   365,   365,   365,   366,   366,   366,   366,
     366,   366,   366,   366,   366,   366,   366,   366,   366,   366,
     366,   366,   367,   367,   368,   368,   368,   369,   369,   370,
     370,   370,   371,   371,   372,   373,   373,   373,   374,   373,
     375,   373,   376,   373,   377,   378,   378,   379,   379,   379,
     379,   379,   380,   380,   381,   381,   382,   382,   382,   383,
     384,   384,   385,   385,   385,   386,   386,   387,   387,   387,
     388,   388,   389,   389,   390,   390,   390,   391,   391,   392,
     392,   392,   392,   392,   393,   393,   394,   394,   394,   395,
     395,   395,   396,   396,   396,   397,   397,   398,   398,   398,
     399,   399,   400,   399,   401,   402,   401,   403,   403,   404,
     404,   405,   405,   405,   406,   406,   406,   408,   407,   409,
     410,   410,   410,   411,   412,   412,   413,   413,   414,   414,
     415,   415,   417,   418,   419,   416,   420,   420,   421,   421,
     422,   423,   423,   423,   423,   424,   424,   424,   425,   425,
     427,   428,   429,   426,   430,   430,   430,   430,   430,   431,
     431,   431,   431,   431,   431,   431,   431,   431,   431,   431,
     431,   431,   431,   431,   431,   431,   431,   431,   431,   431,
     431,   431,   431,   431,   431,   431,   432,   432,   432,   432,
     432,   432,   432,   432,   433,   434,   434,   434,   435,   435,
     435,   436,   436,   436,   436,   436,   437,   437,   437,   437,
     437,   438,   439,   440,   438,   441,   441,   442,   442,   443,
     443,   443,   443,   444,   444,   445,   445,   446,   446,   446,
     446,   447,   448,   446,   446,   446,   446,   449,   446,   450,
     446,   446,   446,   446,   446,   446,   446,   446,   446,   446,
     446,   446,   446,   451,   452,   446,   446,   453,   454,   446,
     455,   456,   446,   457,   458,   446,   446,   459,   460,   446,
     461,   462,   446,   446,   463,   464,   446,   465,   466,   446,
     446,   467,   468,   446,   469,   470,   446,   471,   472,   446,
     473,   474,   446,   475,   475,   475,   477,   478,   479,   480,
     476,   482,   483,   484,   485,   481,   487,   488,   489,   490,
     486,   491,   491,   491,   491,   491,   491,   491,   492,   492,
     492,   492,   492,   493,   493,   493,   493,   493,   493,   493,
     493,   494,   494,   495,   496,   496,   497,   497,   498,   498,
     499,   499,   501,   502,   500,   503,   504,   500,   505,   506,
     500,   507,   508,   500,   509,   510,   500,   511,   512,   513,
     511,   514,   515,   516,   514,   517,   518,   514,   519,   520,
     514,   514,   521,   522,   514,   514,   523,   524,   514,   525,
     525,   526,   527,   528,   528,   528,   529,   529,   530,   530,
     531,   531,   532
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     3,
       3,     2,     2,     2,     2,     2,     2,     1,     1,     1,
       1,     2,     0,     1,     1,     1,     1,     0,     2,     5,
       1,     1,     2,     2,     3,     2,     0,     2,     0,     0,
       3,     0,     2,     5,     3,     1,     2,     0,     4,     2,
       2,     1,     2,     3,     3,     3,     3,     3,     0,     2,
       3,     5,     0,     1,     2,     1,     3,     1,     3,     3,
       3,     2,     1,     1,     1,     2,     0,     1,     0,     0,
       4,     0,     8,     1,     1,     0,     2,     1,     1,     1,
       1,     1,     1,     2,     0,     1,     0,     0,     6,     0,
       3,     0,     0,     6,     0,     3,     0,     0,     9,     7,
       1,     4,     3,     3,     3,     5,     5,     0,    10,     3,
       0,     8,     0,     7,     4,     4,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     1,     3,     3,     5,
       3,     3,     3,     3,     1,     5,     1,     3,     3,     4,
       0,     3,     1,     1,     1,     1,     1,     1,     4,     1,
       2,     3,     3,     3,     3,     2,     1,     3,     0,     3,
       0,     4,     0,     2,     3,     0,     2,     2,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     3,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     3,     2,     2,
       3,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     3,     2,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     4,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     4,     0,     1,     1,     3,     0,
       5,     0,     0,     0,     6,     0,     0,     6,     2,     2,
       2,     1,     2,     2,     1,     1,     1,     1,     2,     1,
       2,     2,     2,     2,     1,     1,     1,     2,     2,     2,
       0,     0,     6,     0,     2,     2,     0,     2,     0,     2,
       1,     3,     1,     3,     2,     2,     3,     0,     0,     5,
       1,     2,     5,     5,     5,     6,     2,     1,     1,     1,
       2,     3,     2,     3,     4,     1,     1,     0,     1,     1,
       1,     0,     1,     3,     8,     7,     3,     3,     5,     0,
       0,     7,     0,     0,     7,     0,     0,     7,     0,     0,
       6,     5,     8,    10,     1,     2,     3,     4,     1,     2,
       3,     1,     1,     2,     2,     2,     2,     2,     4,     1,
       3,     0,     4,     7,     7,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     6,     8,     5,     6,     1,     4,
       3,     0,     0,     8,     0,     0,     9,     3,     4,     5,
       6,     8,     5,     6,     0,     0,     5,     3,     4,     4,
       5,     4,     3,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     4,     3,     4,
       5,     4,     5,     3,     4,     1,     1,     2,     4,     4,
       1,     3,     5,     0,     0,     8,     3,     3,     3,     0,
       0,     8,     3,     4,     0,     0,     9,     4,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     1,     4,
       3,     3,     3,     7,     8,     7,     4,     4,     4,     4,
       4,     1,     6,     7,     6,     6,     7,     7,     6,     7,
       6,     6,     0,     1,     0,     1,     1,     0,     1,     0,
       1,     1,     0,     1,     5,     0,     2,     6,     0,     4,
       0,     9,     0,    11,     3,     3,     4,     1,     1,     3,
       3,     3,     1,     3,     1,     3,     0,     1,     3,     3,
       1,     3,     0,     1,     3,     1,     1,     1,     2,     3,
       3,     5,     1,     1,     1,     1,     1,     0,     1,     1,
       4,     3,     3,     5,     1,     3,     0,     2,     2,     4,
       6,     5,     4,     6,     5,     0,     1,     0,     1,     1,
       0,     2,     0,     4,     6,     0,     6,     1,     3,     1,
       2,     0,     1,     3,     0,     1,     1,     0,     5,     3,
       0,     1,     1,     1,     0,     2,     0,     1,     1,     2,
       0,     1,     0,     0,     0,    13,     0,     2,     0,     1,
       3,     1,     1,     2,     2,     0,     1,     1,     1,     3,
       0,     0,     0,     9,     1,     4,     3,     3,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     1,     3,
       3,     0,     1,     3,     3,     5,     0,     2,     2,     2,
       2,     4,     0,     0,     7,     1,     1,     1,     3,     3,
       2,     4,     3,     1,     2,     0,     4,     1,     1,     1,
       1,     0,     0,     6,     4,     4,     3,     0,     6,     0,
       7,     4,     2,     2,     3,     2,     3,     2,     2,     3,
       3,     3,     2,     0,     0,     6,     2,     0,     0,     6,
       0,     0,     6,     0,     0,     6,     1,     0,     0,     6,
       0,     0,     7,     1,     0,     0,     6,     0,     0,     7,
       1,     0,     0,     6,     0,     0,     7,     0,     0,     6,
       0,     0,     6,     1,     3,     3,     0,     0,     0,     0,
      12,     0,     0,     0,     0,    12,     0,     0,     0,     0,
      13,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     5,     5,     6,     6,     8,
       8,     0,     1,     2,     3,     5,     1,     2,     1,     0,
       0,     1,     0,     0,    10,     0,     0,    10,     0,     0,
      10,     0,     0,    11,     0,     0,     7,     5,     0,     0,
      10,     3,     0,     0,    11,     0,     0,    11,     0,     0,
      10,     5,     0,     0,     9,     5,     0,     0,    10,     1,
       3,     0,     5,     5,     7,     9,     0,     3,     0,     1,
      11,    12,    13
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
            { /* gc_node; */ }
        break;

    case YYSYMBOL_string_builder: /* string_builder  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_reader: /* expr_reader  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_require_module_name: /* require_module_name  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_optional_require_guard: /* optional_require_guard  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_expression_label: /* expression_label  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_goto: /* expression_goto  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_else: /* expression_else  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_else_one_liner: /* expression_else_one_liner  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_if_one_liner: /* expression_if_one_liner  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_if_block: /* expression_if_block  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_else_block: /* expression_else_block  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_if_then_else: /* expression_if_then_else  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_if_then_else_oneliner: /* expression_if_then_else_oneliner  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_for_variable_name_with_pos_list: /* for_variable_name_with_pos_list  */
            { delete ((*yyvaluep).pNameWithPosList); }
        break;

    case YYSYMBOL_expression_for_loop: /* expression_for_loop  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_unsafe: /* expression_unsafe  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_while_loop: /* expression_while_loop  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_with: /* expression_with  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_with_alias: /* expression_with_alias  */
            { /* gc_node; */ }
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

    case YYSYMBOL_optional_for_annotations: /* optional_for_annotations  */
            { delete ((*yyvaluep).aaList); }
        break;

    case YYSYMBOL_annotation_declaration_name: /* annotation_declaration_name  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_annotation_declaration_basic: /* annotation_declaration_basic  */
            { /* gc owns AnnotationDeclaration */ }
        break;

    case YYSYMBOL_annotation_declaration: /* annotation_declaration  */
            { /* gc owns AnnotationDeclaration */ }
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
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_function_name: /* function_name  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_das_type_name: /* das_type_name  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_function_declaration_header: /* function_declaration_header  */
            { ((*yyvaluep).pFuncDecl)->delRef(); }
        break;

    case YYSYMBOL_function_declaration: /* function_declaration  */
            { ((*yyvaluep).pFuncDecl)->delRef(); }
        break;

    case YYSYMBOL_expression_block_finally: /* expression_block_finally  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_block: /* expression_block  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_call_pipe_no_bracket: /* expr_call_pipe_no_bracket  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_any: /* expression_any  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expressions: /* expressions  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_optional_expr_list: /* optional_expr_list  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_optional_expr_map_tuple_list: /* optional_expr_map_tuple_list  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_type_declaration_no_options_list: /* type_declaration_no_options_list  */
            { deleteTypeDeclarationList(((*yyvaluep).pTypeDeclList)); }
        break;

    case YYSYMBOL_name_in_namespace: /* name_in_namespace  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_expression_delete: /* expression_delete  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_new_type_declaration: /* new_type_declaration  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_expr_new: /* expr_new  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_break: /* expression_break  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_continue: /* expression_continue  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_return: /* expression_return  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_yield: /* expression_yield  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_try_catch: /* expression_try_catch  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_tuple_expansion: /* tuple_expansion  */
            { delete ((*yyvaluep).pNameList); }
        break;

    case YYSYMBOL_tuple_expansion_variable_declaration: /* tuple_expansion_variable_declaration  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_expression_let: /* expression_let  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_cast: /* expr_cast  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_type_decl: /* expr_type_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_type_info: /* expr_type_info  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_list: /* expr_list  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_block_or_simple_block: /* block_or_simple_block  */
            { /* gc_node; */ }
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
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_full_block_assumed_piped: /* expr_full_block_assumed_piped  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_numeric_const: /* expr_numeric_const  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_assign_no_bracket: /* expr_assign_no_bracket  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_named_call: /* expr_named_call  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_method_call_no_bracket: /* expr_method_call_no_bracket  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_func_addr_name: /* func_addr_name  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_func_addr_expr: /* func_addr_expr  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_field_no_bracket: /* expr_field_no_bracket  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_call: /* expr_call  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr: /* expr  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_no_bracket: /* expr_no_bracket  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_generator: /* expr_generator  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_mtag_no_bracket: /* expr_mtag_no_bracket  */
            { /* gc_node; */ }
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
            { /* gc owns Enumeration */ }
        break;

    case YYSYMBOL_enum_name: /* enum_name  */
            { /* $$->delRef(); // if enum rule returns, module already has the link */ }
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
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_auto_type_declaration: /* auto_type_declaration  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_bitfield_bits: /* bitfield_bits  */
            { delete ((*yyvaluep).pNameList); }
        break;

    case YYSYMBOL_bitfield_alias_bits: /* bitfield_alias_bits  */
            { deleteNameExprList(((*yyvaluep).pNameExprList)); }
        break;

    case YYSYMBOL_bitfield_type_declaration: /* bitfield_type_declaration  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_table_type_pair: /* table_type_pair  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_dim_list: /* dim_list  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_type_declaration_no_options: /* type_declaration_no_options  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_optional_expr_list_in_braces: /* optional_expr_list_in_braces  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_type_declaration_no_options_no_dim: /* type_declaration_no_options_no_dim  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_type_declaration: /* type_declaration  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_make_decl: /* make_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_decl_no_bracket: /* make_decl_no_bracket  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_struct_fields: /* make_struct_fields  */
            { /* gc owns MakeStruct */ }
        break;

    case YYSYMBOL_make_variant_dim: /* make_variant_dim  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_struct_single: /* make_struct_single  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_struct_dim_list: /* make_struct_dim_list  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_struct_dim_decl: /* make_struct_dim_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_optional_make_struct_dim_decl: /* optional_make_struct_dim_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_struct_decl: /* make_struct_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_tuple_call: /* make_tuple_call  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_dim_decl: /* make_dim_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_map_tuple_list: /* expr_map_tuple_list  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_table_decl: /* make_table_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_table_call: /* make_table_call  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_array_comprehension_where: /* array_comprehension_where  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_table_comprehension: /* table_comprehension  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_array_comprehension: /* array_comprehension  */
            { /* gc_node; */ }
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
                das2_yyerror(scanner,"module name has to be first declaration",tokAt(scanner,(yylsp[0])), CompilationError::invalid_module);
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
        (void)(yyvsp[-1].pExpression); // gc_node — Expression, don't delete
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

  case 27: /* optional_not_required: %empty  */
        { (yyval.b) = false; }
    break;

  case 28: /* optional_not_required: '!' "inscope"  */
                        { (yyval.b) = true; }
    break;

  case 29: /* module_declaration: "module" module_name optional_shared optional_public_or_private_module optional_not_required  */
                                                                                                                                    {
        yyextra->g_Program->thisModuleName = *(yyvsp[-3].s);
        yyextra->g_Program->thisModule->isPublic = (yyvsp[-1].b);
        yyextra->g_Program->thisModule->isModule = true;
        yyextra->g_Program->thisModule->visibleEverywhere = (yyvsp[0].b);
        if ( yyextra->g_Program->thisModule->name.empty() ) {
            yyextra->g_Program->library.renameModule(yyextra->g_Program->thisModule.get(),*(yyvsp[-3].s));
        } else if ( yyextra->g_Program->thisModule->name != *(yyvsp[-3].s) ){
            das2_yyerror(scanner,"this module already has a name " + yyextra->g_Program->thisModule->name,tokAt(scanner,(yylsp[-3])),
                CompilationError::already_declared_module_name);
        }
        if ( !yyextra->g_Program->policies.ignore_shared_modules ) {
            yyextra->g_Program->promoteToBuiltin = (yyvsp[-2].b);
        }
        delete (yyvsp[-3].s);
    }
    break;

  case 30: /* character_sequence: STRING_CHARACTER  */
                                                                                  { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
    break;

  case 31: /* character_sequence: STRING_CHARACTER_ESC  */
                                                                                  { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
    break;

  case 32: /* character_sequence: character_sequence STRING_CHARACTER  */
                                                                                  { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
    break;

  case 33: /* character_sequence: character_sequence STRING_CHARACTER_ESC  */
                                                                                  { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
    break;

  case 34: /* string_constant: "start of the string" character_sequence "end of the string"  */
                                                           { (yyval.s) = (yyvsp[-1].s); }
    break;

  case 35: /* string_constant: "start of the string" "end of the string"  */
                                                           { (yyval.s) = new string(); }
    break;

  case 36: /* format_string: %empty  */
        { (yyval.s) = new string(); }
    break;

  case 37: /* format_string: format_string STRING_CHARACTER  */
                                                 { (yyval.s) = (yyvsp[-1].s); (yyvsp[-1].s)->push_back((yyvsp[0].ch)); }
    break;

  case 38: /* optional_format_string: %empty  */
        { (yyval.s) = new string(""); }
    break;

  case 39: /* $@1: %empty  */
            { das2_strfmt(scanner); }
    break;

  case 40: /* optional_format_string: ':' $@1 format_string  */
                                                         { (yyval.s) = (yyvsp[0].s); }
    break;

  case 41: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 42: /* string_builder_body: string_builder_body character_sequence  */
                                                                                  {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das2_yyerror(scanner,"invalid escape sequence",tokAt(scanner,(yylsp[-1])), CompilationError::invalid_escape);
        auto sc = new ExprConstString(tokAt(scanner,(yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 43: /* string_builder_body: string_builder_body "{" expr optional_format_string "}"  */
                                                                                                                                     {
        auto se = (yyvsp[-2].pExpression);
        if ( !(yyvsp[-1].s)->empty() ) {
            auto call_fmt = new ExprCall(tokAt(scanner,(yylsp[-1])), "_::fmt");
            call_fmt->arguments.push_back(new ExprConstString(tokAt(scanner,(yylsp[-1])),":" + *(yyvsp[-1].s)));
            call_fmt->arguments.push_back(se);
            se = call_fmt;
        }
        static_cast<ExprStringBuilder *>((yyvsp[-4].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-4].pExpression);
        delete (yyvsp[-1].s);
    }
    break;

  case 44: /* string_builder: "start of the string" string_builder_body "end of the string"  */
                                                                   {
        auto strb = static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression));
        if ( strb->elements.size()==0 ) {
            (yyval.pExpression) = new ExprConstString(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),"");
            // gc_node — don't delete $sb
        } else if ( strb->elements.size()==1 && strb->elements[0]->rtti_isStringConstant() ) {
            auto sconst = static_cast<ExprConstString*>(strb->elements[0]);
            (yyval.pExpression) = new ExprConstString(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),sconst->text);
            // gc_node — don't delete $sb
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    }
    break;

  case 45: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das2_yyend_reader(scanner);
        }
    }
    break;

  case 46: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das2_yyend_reader(scanner);
        }
    }
    break;

  case 47: /* $@2: %empty  */
                                        {
        auto macros = yyextra->g_Program->getReaderMacro(*(yyvsp[0].s));
        if ( macros.size()==0 ) {
            das2_yyerror(scanner,"reader macro " + *(yyvsp[0].s) + " not found",tokAt(scanner,(yylsp[0])),
                CompilationError::lookup_macro);
        } else if ( macros.size()>1 ) {
            string options;
            for ( auto & x : macros ) {
                options += "\t" + x->module->name + "::" + x->name + "\n";
            }
            das2_yyerror(scanner,"too many options for the reader macro " + *(yyvsp[0].s) +  "\n" + options, tokAt(scanner,(yylsp[0])),
                CompilationError::ambiguous_macro);
        } else if ( yychar != '~' ) {
            das2_yyerror(scanner,"expecting ~ after the reader macro", tokAt(scanner,(yylsp[0])),
                CompilationError::invalid_macro);
        } else {
            yyextra->g_ReaderMacro = macros.back();
            yyextra->g_ReaderExpr = new ExprReader(tokAt(scanner,(yylsp[-1])),yyextra->g_ReaderMacro);
            yyclearin ;
            das2_yybegin_reader(scanner);
        }
    }
    break;

  case 48: /* expr_reader: '%' name_in_namespace $@2 reader_character_sequence  */
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

  case 49: /* options_declaration: "options" annotation_argument_list  */
                                                   {
        for ( auto & opt : *(yyvsp[0].aaList) ) {
            if ( yyextra->g_Access->isOptionAllowed(opt.name, yyextra->g_Program->thisModule->fileName) ) {
                if ( yyextra->g_Access->isOptionBlocked(opt.name, yyextra->g_Program->thisModule->fileName) ) {
                    // blocked: ok to write, silently ignored (not applied)
                } else {
                    yyextra->g_Program->options.push_back(opt);
                }
            } else {
                das2_yyerror(scanner,"option " + opt.name + " is not allowed here",
                    tokAt(scanner,(yylsp[0])), CompilationError::invalid_options);
            }
        }
        delete (yyvsp[0].aaList);
    }
    break;

  case 51: /* require_module_name: "name"  */
                   {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 52: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 53: /* require_module_name: '.' '/' require_module_name  */
                                         {
        *(yyvsp[0].s) = "./" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 54: /* require_module_name: ".." '/' require_module_name  */
                                            {
        *(yyvsp[0].s) = "../" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 55: /* require_module_name: '%' '/' require_module_name  */
                                         {
        *(yyvsp[0].s) = "%/" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 56: /* require_module_name: require_module_name '.' "name"  */
                                                {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 57: /* require_module_name: require_module_name '/' "name"  */
                                                {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 58: /* optional_require_guard: %empty  */
                            { (yyval.s) = nullptr; }
    break;

  case 59: /* optional_require_guard: '?' "name"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 60: /* require_module: optional_require_guard require_module_name is_public_module  */
                                                                                       {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])),(yyvsp[-2].s));
    }
    break;

  case 61: /* require_module: optional_require_guard require_module_name "as" "name" is_public_module  */
                                                                                                            {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])),(yyvsp[-4].s));
    }
    break;

  case 62: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 63: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 67: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 68: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 69: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 70: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 71: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 72: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 73: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 78: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 79: /* $@3: %empty  */
                                           {
    }
    break;

  case 80: /* expression_else: "else" optional_emit_semis $@3 expression_else_block  */
                                   {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 81: /* $@4: %empty  */
                                                                        {
    }
    break;

  case 82: /* expression_else: elif_or_static_elif '(' expr ')' optional_emit_semis $@4 expression_else_block expression_else  */
                                                         {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-7])),(yyvsp[-5].pExpression),(yyvsp[-1].pExpression),(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-7].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 83: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 84: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 85: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 86: /* expression_else_one_liner: "else" expression_if_one_liner  */
                                                      {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 87: /* expression_if_one_liner: expr_no_bracket  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 88: /* expression_if_one_liner: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 89: /* expression_if_one_liner: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 90: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 91: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 96: /* $@5: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 97: /* $@6: %empty  */
                         {
        yyextra->pop_nesteds();
    }
    break;

  case 98: /* expression_if_block: '{' $@5 expressions $@6 '}' expression_block_finally  */
                                        {
        (yyval.pExpression) = (yyvsp[-3].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-5]),(yylsp[0]));
        if ( (yyvsp[0].pExpression) ) {
            auto pF = (ExprBlock *) (yyvsp[0].pExpression);
            auto pB = (ExprBlock *) (yyval.pExpression);
            swap ( pB->finalList, pF->list );
            // gc_node — don't delete Expression
        }
    }
    break;

  case 99: /* $@7: %empty  */
       {
        yyextra->das_keyword = false;
    }
    break;

  case 100: /* expression_if_block: $@7 expression_if_one_liner SEMICOLON  */
                                               {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 101: /* $@8: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 102: /* $@9: %empty  */
                         {
        yyextra->pop_nesteds();
    }
    break;

  case 103: /* expression_else_block: '{' $@8 expressions $@9 '}' expression_block_finally  */
                                        {
        (yyval.pExpression) = (yyvsp[-3].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-5]),(yylsp[0]));
        if ( (yyvsp[0].pExpression) ) {
            auto pF = (ExprBlock *) (yyvsp[0].pExpression);
            auto pB = (ExprBlock *) (yyval.pExpression);
            swap ( pB->finalList, pF->list );
            // gc_node — don't delete Expression
        }
    }
    break;

  case 104: /* $@10: %empty  */
       {
        yyextra->das_keyword = false;
    }
    break;

  case 105: /* expression_else_block: $@10 expression_if_one_liner SEMICOLON  */
                                               {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 106: /* $@11: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 107: /* $@12: %empty  */
                                                                  {
    }
    break;

  case 108: /* expression_if_then_else: $@11 if_or_static_if '(' expr ')' optional_emit_semis $@12 expression_if_block expression_else  */
                                                       {
        yyextra->das_keyword = false;
        auto blk = (yyvsp[-1].pExpression)->rtti_isBlock() ? static_cast<ExprBlock *>((yyvsp[-1].pExpression)) : ast_wrapInBlock((yyvsp[-1].pExpression));
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-7])),(yyvsp[-5].pExpression),blk,(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-7].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 109: /* expression_if_then_else_oneliner: expression_if_one_liner "if" '(' expr ')' expression_else_one_liner SEMICOLON  */
                                                                                                                      {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ast_wrapInBlock((yyvsp[-6].pExpression)),(yyvsp[-1].pExpression) ? ast_wrapInBlock((yyvsp[-1].pExpression)) : nullptr);
    }
    break;

  case 110: /* for_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 111: /* for_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 112: /* for_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 113: /* for_variable_name_with_pos_list: '(' tuple_expansion ')'  */
                                       {
        auto pSL = new vector<VariableNameAndPosition>();
        for ( auto & x : *(yyvsp[-1].pNameList) ) {
            das_checkName(scanner,x,tokAt(scanner,(yylsp[-1])));
        }
        pSL->push_back(VariableNameAndPosition((yyvsp[-1].pNameList),tokAt(scanner,(yylsp[-1]))));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 114: /* for_variable_name_with_pos_list: for_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 115: /* for_variable_name_with_pos_list: for_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 116: /* for_variable_name_with_pos_list: for_variable_name_with_pos_list ',' '(' tuple_expansion ')'  */
                                                                                 {
        for ( auto & x : *(yyvsp[-1].pNameList) ) {
            das_checkName(scanner,x,tokAt(scanner,(yylsp[-1])));
        }
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition((yyvsp[-1].pNameList),tokAt(scanner,(yylsp[-1]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
    }
    break;

  case 117: /* $@13: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 118: /* expression_for_loop: $@13 "for" optional_for_annotations '(' for_variable_name_with_pos_list "in" expr_list ')' optional_emit_semis expression_block  */
                                                                                                                                                                    {
        yyextra->das_keyword = false;
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-5].pNameWithPosList),(yyvsp[-3].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-8])),tokAt(scanner,(yylsp[0])),(yyvsp[-7].aaList));
    }
    break;

  case 119: /* expression_unsafe: "unsafe" optional_emit_semis expression_block  */
                                                                    {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-2])));
        pUnsafe->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 120: /* $@14: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 121: /* expression_while_loop: $@14 "while" optional_for_annotations '(' expr ')' optional_emit_semis expression_block  */
                                                                                                                        {
        yyextra->das_keyword = false;
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-6])));
        pWhile->cond = (yyvsp[-3].pExpression);
        pWhile->body = (yyvsp[0].pExpression);
        if ( (yyvsp[-5].aaList) ) { pWhile->annotations = move(*(yyvsp[-5].aaList)); delete (yyvsp[-5].aaList); }
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 122: /* $@15: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 123: /* expression_with: $@15 "with" '(' expr ')' optional_emit_semis expression_block  */
                                                                                   {
        yyextra->das_keyword = false;
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-5])));
        pWith->with = (yyvsp[-3].pExpression);
        pWith->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pWith;
    }
    break;

  case 124: /* expression_with_alias: "assume" "name" '=' expr  */
                                                      {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-3])), *(yyvsp[-2].s), ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-2].s);
    }
    break;

  case 125: /* expression_with_alias: "typedef" "name" '=' type_declaration  */
                                                                {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-3])), *(yyvsp[-2].s), TypeDeclPtr((yyvsp[0].pTypeDecl)));
        delete (yyvsp[-2].s);
    }
    break;

  case 126: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 127: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 128: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 129: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 130: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 131: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 132: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 133: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 134: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 135: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 136: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 137: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 138: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 139: /* annotation_argument: annotation_argument_name '=' '@' '@' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[0].s); delete (yyvsp[-4].s); }
    break;

  case 140: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 141: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 142: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 143: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 144: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 145: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 146: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 147: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 148: /* metadata_argument_list: '@' annotation_argument optional_emit_semis  */
                                                         {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[-1].aa));
    }
    break;

  case 149: /* metadata_argument_list: metadata_argument_list '@' annotation_argument optional_emit_semis  */
                                                                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-3].aaList),(yyvsp[-1].aa));
    }
    break;

  case 150: /* optional_for_annotations: %empty  */
                    {
        (yyval.aaList) = nullptr;
    }
    break;

  case 151: /* optional_for_annotations: '[' annotation_argument_list ']'  */
                                               {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 152: /* optional_for_annotations: metadata_argument_list  */
                                     {
        (yyval.aaList) = (yyvsp[0].aaList);
    }
    break;

  case 153: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 154: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 155: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 156: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 157: /* annotation_declaration_basic: annotation_declaration_name  */
                                          {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[0]));
        if ( yyextra->g_Access->isAnnotationAllowed(*(yyvsp[0].s), yyextra->g_Program->thisModuleName) ) {
            if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
                (yyval.fa)->annotation = ann;
            } else {
                (yyval.fa)->annotation = new Annotation(*(yyvsp[0].s));
                das2_yyerror(scanner,"annotation " + *(yyvsp[0].s) + " is not found",
                            tokAt(scanner,(yylsp[0])), CompilationError::lookup_annotation);
            }
        } else {
            (yyval.fa)->annotation = new Annotation(*(yyvsp[0].s));
            das2_yyerror(scanner,"annotation " + *(yyvsp[0].s) + " is not allowed here",
                        tokAt(scanner,(yylsp[0])), CompilationError::invalid_annotation);
        }
        delete (yyvsp[0].s);
    }
    break;

  case 158: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
                                                                                 {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[-3]));
        if ( yyextra->g_Access->isAnnotationAllowed(*(yyvsp[-3].s), yyextra->g_Program->thisModuleName) ) {
            if ( auto ann = findAnnotation(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3]))) ) {
                (yyval.fa)->annotation = ann;
            } else {
                (yyval.fa)->annotation = new Annotation(*(yyvsp[-3].s));
                das2_yyerror(scanner,"annotation " + *(yyvsp[-3].s) + " is not found",
                            tokAt(scanner,(yylsp[-3])), CompilationError::lookup_annotation);
            }
        } else {
            (yyval.fa)->annotation = new Annotation(*(yyvsp[-3].s));
            das2_yyerror(scanner,"annotation " + *(yyvsp[-3].s) + " is not allowed here",
                        tokAt(scanner,(yylsp[-3])), CompilationError::invalid_annotation);
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
    break;

  case 159: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 160: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            delete (yyvsp[0].fa); (yyvsp[0].fa) = nullptr;
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 161: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
                                                                              {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation);
            delete (yyvsp[-2].fa); (yyvsp[-2].fa) = nullptr;
        }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            delete (yyvsp[-2].fa); (yyvsp[-2].fa) = nullptr;
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::And,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 162: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
                                                                            {
        if ( !(yyvsp[-2].fa)->annotation || !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation);
            delete (yyvsp[-2].fa); (yyvsp[-2].fa) = nullptr;
        }
        if ( !(yyvsp[0].fa)->annotation || !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            delete (yyvsp[-2].fa); (yyvsp[-2].fa) = nullptr;
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Or,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 163: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
                                                                              {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation);
            delete (yyvsp[-2].fa); (yyvsp[-2].fa) = nullptr;
        }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            delete (yyvsp[-2].fa); (yyvsp[-2].fa) = nullptr;
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Xor,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 164: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 165: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 166: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 167: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 168: /* optional_annotation_list: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 169: /* optional_annotation_list: '[' annotation_list ']'  */
                                       { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 170: /* optional_annotation_list_with_emit_semis: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 171: /* optional_annotation_list_with_emit_semis: '[' annotation_list ']' optional_emit_semis  */
                                                          { (yyval.faList) = (yyvsp[-2].faList); }
    break;

  case 172: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 173: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 174: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 175: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 176: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 177: /* optional_function_type: "->" type_declaration  */
                                           {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 178: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 179: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 180: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 181: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 182: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 183: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 184: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 185: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 186: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 187: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 188: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 189: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 190: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 191: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 192: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 193: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 194: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 195: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 196: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 197: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 198: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 199: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 200: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 201: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 202: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 203: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 204: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 205: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 206: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 207: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 208: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 209: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 210: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 211: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 212: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 213: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 214: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 215: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 216: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 217: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 218: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 219: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 220: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 221: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 222: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 223: /* function_name: "operator" '[' ']' '='  */
                                 { (yyval.s) = new string("[]="); }
    break;

  case 224: /* function_name: "operator" '[' ']' "<-"  */
                                    { (yyval.s) = new string("[]<-"); }
    break;

  case 225: /* function_name: "operator" '[' ']' ":="  */
                                      { (yyval.s) = new string("[]:="); }
    break;

  case 226: /* function_name: "operator" '[' ']' "+="  */
                                     { (yyval.s) = new string("[]+="); }
    break;

  case 227: /* function_name: "operator" '[' ']' "-="  */
                                     { (yyval.s) = new string("[]-="); }
    break;

  case 228: /* function_name: "operator" '[' ']' "*="  */
                                     { (yyval.s) = new string("[]*="); }
    break;

  case 229: /* function_name: "operator" '[' ']' "/="  */
                                     { (yyval.s) = new string("[]/="); }
    break;

  case 230: /* function_name: "operator" '[' ']' "%="  */
                                     { (yyval.s) = new string("[]%="); }
    break;

  case 231: /* function_name: "operator" '[' ']' "&="  */
                                     { (yyval.s) = new string("[]&="); }
    break;

  case 232: /* function_name: "operator" '[' ']' "|="  */
                                     { (yyval.s) = new string("[]|="); }
    break;

  case 233: /* function_name: "operator" '[' ']' "^="  */
                                     { (yyval.s) = new string("[]^="); }
    break;

  case 234: /* function_name: "operator" '[' ']' "&&="  */
                                        { (yyval.s) = new string("[]&&="); }
    break;

  case 235: /* function_name: "operator" '[' ']' "||="  */
                                        { (yyval.s) = new string("[]||="); }
    break;

  case 236: /* function_name: "operator" '[' ']' "^^="  */
                                        { (yyval.s) = new string("[]^^="); }
    break;

  case 237: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 238: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 239: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 240: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 241: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 242: /* function_name: "operator" '.' "name" "+="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`+="); delete (yyvsp[-1].s); }
    break;

  case 243: /* function_name: "operator" '.' "name" "-="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`-="); delete (yyvsp[-1].s); }
    break;

  case 244: /* function_name: "operator" '.' "name" "*="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`*="); delete (yyvsp[-1].s); }
    break;

  case 245: /* function_name: "operator" '.' "name" "/="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`/="); delete (yyvsp[-1].s); }
    break;

  case 246: /* function_name: "operator" '.' "name" "%="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`%="); delete (yyvsp[-1].s); }
    break;

  case 247: /* function_name: "operator" '.' "name" "&="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&="); delete (yyvsp[-1].s); }
    break;

  case 248: /* function_name: "operator" '.' "name" "|="  */
                                          { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`|="); delete (yyvsp[-1].s); }
    break;

  case 249: /* function_name: "operator" '.' "name" "^="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^="); delete (yyvsp[-1].s); }
    break;

  case 250: /* function_name: "operator" '.' "name" "&&="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&&="); delete (yyvsp[-1].s); }
    break;

  case 251: /* function_name: "operator" '.' "name" "||="  */
                                            { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`||="); delete (yyvsp[-1].s); }
    break;

  case 252: /* function_name: "operator" '.' "name" "^^="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^^="); delete (yyvsp[-1].s); }
    break;

  case 253: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 254: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 255: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 256: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 257: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 258: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 259: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 260: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 261: /* function_name: "operator" "is" das_type_name  */
                                                { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 262: /* function_name: "operator" "as" das_type_name  */
                                                { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 263: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 264: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 265: /* function_name: "operator" '?' "as" das_type_name  */
                                                    { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 266: /* function_name: das_type_name  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 267: /* das_type_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 268: /* das_type_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 269: /* das_type_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 270: /* das_type_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 271: /* das_type_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 272: /* das_type_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 273: /* das_type_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 274: /* das_type_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 275: /* das_type_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 276: /* das_type_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 277: /* das_type_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 278: /* das_type_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 279: /* das_type_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 280: /* das_type_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 281: /* das_type_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 282: /* das_type_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 283: /* das_type_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 284: /* das_type_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 285: /* das_type_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 286: /* das_type_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 287: /* das_type_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 288: /* das_type_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 289: /* das_type_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 290: /* das_type_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 291: /* das_type_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 292: /* optional_template: %empty  */
                                        { (yyval.b) = false; }
    break;

  case 293: /* optional_template: "template"  */
                                        { (yyval.b) = true; }
    break;

  case 294: /* global_function_declaration: optional_annotation_list_with_emit_semis "def" optional_template function_declaration  */
                                                                                                                              {
        (yyvsp[0].pFuncDecl)->atDecl = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
        (yyvsp[0].pFuncDecl)->isTemplate = (yyvsp[-1].b);
        assignDefaultArguments((yyvsp[0].pFuncDecl));
        runFunctionAnnotations(scanner, yyextra, (yyvsp[0].pFuncDecl), (yyvsp[-3].faList), tokAt(scanner,(yylsp[-3])));
        if ( (yyvsp[0].pFuncDecl)->isGeneric() ) {
            implAddGenericFunction(scanner,(yyvsp[0].pFuncDecl));
        } else {
            if ( !yyextra->g_Program->addFunction((yyvsp[0].pFuncDecl)) ) {
                das2_yyerror(scanner,"function is already defined " +
                    (yyvsp[0].pFuncDecl)->getMangledName(),(yyvsp[0].pFuncDecl)->at,
                        CompilationError::already_declared_function);
            }
        }
        (yyvsp[0].pFuncDecl)->delRef();
    }
    break;

  case 295: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 296: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 297: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 298: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 299: /* $@16: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 300: /* function_declaration: optional_public_or_private_function $@16 function_declaration_header optional_emit_semis block_or_simple_block  */
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

  case 301: /* expression_block_finally: %empty  */
        {
        (yyval.pExpression) = nullptr;
    }
    break;

  case 302: /* $@17: %empty  */
                  {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 303: /* $@18: %empty  */
                             {
        yyextra->pop_nesteds();
    }
    break;

  case 304: /* expression_block_finally: "finally" $@17 '{' expressions $@18 '}'  */
          {
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 305: /* $@19: %empty  */
        {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 306: /* $@20: %empty  */
                                      {
        yyextra->pop_nesteds();
    }
    break;

  case 307: /* expression_block: $@19 '{' expressions $@20 '}' expression_block_finally  */
                                        {
        (yyval.pExpression) = (yyvsp[-3].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-4]),(yylsp[0]));
        if ( (yyvsp[0].pExpression) ) {
            auto pF = (ExprBlock *) (yyvsp[0].pExpression);
            auto pB = (ExprBlock *) (yyval.pExpression);
            swap ( pB->finalList, pF->list );
            // gc_node — don't delete Expression
        }
    }
    break;

  case 308: /* expr_call_pipe_no_bracket: expr_call expr_full_block_assumed_piped  */
                                                           {
        if ( (yyvsp[-1].pExpression)->rtti_isCallLikeExpr() ) {
            ((ExprLooksLikeCall *)(yyvsp[-1].pExpression))->arguments.push_back((yyvsp[0].pExpression));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
            // gc_node — don't delete Expression
        }
    }
    break;

  case 309: /* expr_call_pipe_no_bracket: expr_method_call_no_bracket expr_full_block_assumed_piped  */
                                                                             {
        if ( (yyvsp[-1].pExpression)->rtti_isCallLikeExpr() ) {
            ((ExprLooksLikeCall *)(yyvsp[-1].pExpression))->arguments.push_back((yyvsp[0].pExpression));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
            // gc_node — don't delete Expression
        }
    }
    break;

  case 310: /* expr_call_pipe_no_bracket: expr_field_no_bracket expr_full_block_assumed_piped  */
                                                                       {
        if ( (yyvsp[-1].pExpression)->rtti_isCallLikeExpr() ) {
            ((ExprLooksLikeCall *)(yyvsp[-1].pExpression))->arguments.push_back((yyvsp[0].pExpression));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
            // gc_node — don't delete Expression
        }
    }
    break;

  case 311: /* expression_any: SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 312: /* expression_any: expr_assign_no_bracket SEMICOLON  */
                                                    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 313: /* expression_any: expression_delete SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 314: /* expression_any: expression_let  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 315: /* expression_any: expression_while_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 316: /* expression_any: expression_unsafe  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 317: /* expression_any: expression_with  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 318: /* expression_any: expression_with_alias SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 319: /* expression_any: expression_for_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 320: /* expression_any: expression_break SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 321: /* expression_any: expression_continue SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 322: /* expression_any: expression_return SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 323: /* expression_any: expression_yield SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 324: /* expression_any: expression_if_then_else  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 325: /* expression_any: expression_if_then_else_oneliner  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 326: /* expression_any: expression_try_catch  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 327: /* expression_any: expression_label SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 328: /* expression_any: expression_goto SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 329: /* expression_any: "pass" SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 330: /* $@21: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 331: /* $@22: %empty  */
                         {
        yyextra->pop_nesteds();
    }
    break;

  case 332: /* expression_any: '{' $@21 expressions $@22 '}' expression_block_finally  */
                                        {
        (yyval.pExpression) = (yyvsp[-3].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-5]),(yylsp[0]));
        if ( (yyvsp[0].pExpression) ) {
            auto pF = (ExprBlock *) (yyvsp[0].pExpression);
            auto pB = (ExprBlock *) (yyval.pExpression);
            swap ( pB->finalList, pF->list );
            // gc_node — don't delete Expression
        }
    }
    break;

  case 333: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 334: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 335: /* expressions: expressions error  */
                                 {
        (void)(yyvsp[-1].pExpression); /* gc_node — don't delete Expression */ (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 336: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 337: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 338: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 339: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 340: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 341: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 342: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 343: /* name_in_namespace: "name" "::" "name"  */
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

  case 344: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 345: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 346: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 347: /* $@23: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 348: /* $@24: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 349: /* new_type_declaration: '<' $@23 type_declaration '>' $@24  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 350: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 351: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 352: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 353: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 354: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 355: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 356: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 357: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 358: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 359: /* expression_return: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 360: /* expression_return: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 361: /* expression_return: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 362: /* expression_yield: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 363: /* expression_yield: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 364: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 365: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 366: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 367: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 368: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 369: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 370: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 371: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 372: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 373: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 374: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 375: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr SEMICOLON  */
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

  case 376: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 377: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 378: /* expression_let: kwd_let optional_in_scope '{' variable_declaration_list '}'  */
                                                                               {
        (yyval.pExpression) = ast_LetList(scanner,(yyvsp[-4].b),(yyvsp[-3].b),*(yyvsp[-1].pVarDeclList),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 379: /* $@25: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 380: /* $@26: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 381: /* expr_cast: "cast" '<' $@25 type_declaration_no_options '>' $@26 expr_no_bracket  */
                                                                                                                                                           {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
    }
    break;

  case 382: /* $@27: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 383: /* $@28: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 384: /* expr_cast: "upcast" '<' $@27 type_declaration_no_options '>' $@28 expr_no_bracket  */
                                                                                                                                                             {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 385: /* $@29: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 386: /* $@30: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 387: /* expr_cast: "reinterpret" '<' $@29 type_declaration_no_options '>' $@30 expr_no_bracket  */
                                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 388: /* $@31: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 389: /* $@32: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 390: /* expr_type_decl: "type" '<' $@31 type_declaration '>' $@32  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 391: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
                                                                          {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-3].s),ptd->typeexpr);
                // gc_node — don't delete Expression
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-3].s),(yyvsp[-1].pExpression));
            }
            delete (yyvsp[-3].s);
    }
    break;

  case 392: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
                                                                                                {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-6].s),ptd->typeexpr,*(yyvsp[-4].s));
                // gc_node — don't delete Expression
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-6].s),(yyvsp[-1].pExpression),*(yyvsp[-4].s));
            }
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
    break;

  case 393: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" c_or_s "name" '>' '(' expr ')'  */
                                                                                                                        {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-8].s),ptd->typeexpr,*(yyvsp[-6].s),*(yyvsp[-4].s));
                // gc_node — don't delete Expression
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-8].s),(yyvsp[-1].pExpression),*(yyvsp[-6].s),*(yyvsp[-4].s));
            }
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
    break;

  case 394: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 395: /* expr_list: "<-" expr  */
                             {
            (yyval.pExpression) = ast_makeMoveArgument(scanner, (yyvsp[0].pExpression), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 396: /* expr_list: expr_list ',' expr  */
                                        {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 397: /* expr_list: expr_list ',' "<-" expr  */
                                                   {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-3])),(yyvsp[-3].pExpression),ast_makeMoveArgument(scanner, (yyvsp[0].pExpression), tokAt(scanner,(yylsp[0]))));
    }
    break;

  case 398: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 399: /* block_or_simple_block: "=>" expr_no_bracket  */
                                                   {
            auto retE = new ExprReturn(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 400: /* block_or_simple_block: "=>" "<-" expr_no_bracket  */
                                                          {
            auto retE = new ExprReturn(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 401: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 402: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 403: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 404: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 405: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 406: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 407: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 408: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 409: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 410: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 411: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 412: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 413: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis block_or_simple_block  */
                                                                                                                {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 414: /* expr_full_block_assumed_piped: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis block_or_simple_block  */
                                                                                                                {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 415: /* expr_full_block_assumed_piped: '{' expressions '}'  */
                                   {
        (yyval.pExpression) = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,new TypeDecl(Type::autoinfer),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[-1])),LineInfo());
    }
    break;

  case 416: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 417: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 418: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 419: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 420: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 421: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 422: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 423: /* expr_assign_no_bracket: expr_no_bracket  */
                                                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 424: /* expr_assign_no_bracket: expr_no_bracket '=' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 425: /* expr_assign_no_bracket: expr_no_bracket "<-" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 426: /* expr_assign_no_bracket: expr_no_bracket "<-" make_table_decl  */
                                                                   { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 427: /* expr_assign_no_bracket: expr_no_bracket "<-" array_comprehension  */
                                                                     { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 428: /* expr_assign_no_bracket: expr_no_bracket ":=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 429: /* expr_assign_no_bracket: expr_no_bracket "&=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 430: /* expr_assign_no_bracket: expr_no_bracket "|=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 431: /* expr_assign_no_bracket: expr_no_bracket "^=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 432: /* expr_assign_no_bracket: expr_no_bracket "&&=" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 433: /* expr_assign_no_bracket: expr_no_bracket "||=" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 434: /* expr_assign_no_bracket: expr_no_bracket "^^=" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 435: /* expr_assign_no_bracket: expr_no_bracket "+=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 436: /* expr_assign_no_bracket: expr_no_bracket "-=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 437: /* expr_assign_no_bracket: expr_no_bracket "*=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 438: /* expr_assign_no_bracket: expr_no_bracket "/=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 439: /* expr_assign_no_bracket: expr_no_bracket "%=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 440: /* expr_assign_no_bracket: expr_no_bracket "<<=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 441: /* expr_assign_no_bracket: expr_no_bracket ">>=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 442: /* expr_assign_no_bracket: expr_no_bracket "<<<=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 443: /* expr_assign_no_bracket: expr_no_bracket ">>>=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 444: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 445: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 446: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' ')'  */
                                                                    {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 447: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' expr_list ')'  */
                                                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 448: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 449: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 450: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 451: /* $@33: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 452: /* $@34: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 453: /* func_addr_expr: '@' '@' '<' $@33 type_declaration_no_options '>' $@34 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 454: /* $@35: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 455: /* $@36: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 456: /* func_addr_expr: '@' '@' '<' $@35 optional_function_argument_list optional_function_type '>' $@36 func_addr_name  */
                                                                                                                                                                                     {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value));
        expr->funcType = new TypeDecl(Type::tFunction);
        expr->funcType->firstType = (yyvsp[-3].pTypeDecl);
        if ( (yyvsp[-4].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, expr->funcType, (yyvsp[-4].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-4].pVarDeclList));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 457: /* expr_field_no_bracket: expr_no_bracket '.' "name"  */
                                                         {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 458: /* expr_field_no_bracket: expr_no_bracket '.' '.' "name"  */
                                                             {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 459: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' ')'  */
                                                                 {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 460: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' expr_list ')'  */
                                                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 461: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->methodCall = true;
        nc->arguments = (yyvsp[-2].pMakeStruct);
        nc->nonNamedArguments.push_back((yyvsp[-7].pExpression));
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 462: /* expr_field_no_bracket: expr_no_bracket '.' basic_type_declaration '(' ')'  */
                                                                                   {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 463: /* expr_field_no_bracket: expr_no_bracket '.' basic_type_declaration '(' expr_list ')'  */
                                                                                                        {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 464: /* $@37: %empty  */
                                          { yyextra->das_suppress_errors=true; }
    break;

  case 465: /* $@38: %empty  */
                                                                                       { yyextra->das_suppress_errors=false; }
    break;

  case 466: /* expr_field_no_bracket: expr_no_bracket '.' $@37 error $@38  */
                                                                                                                               {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 467: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 468: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 469: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 470: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-4])),*(yyvsp[-4].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 471: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 472: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 473: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 474: /* expr: expr_no_bracket  */
                                       { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 475: /* expr: make_table_decl  */
                                     { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 476: /* expr: array_comprehension  */
                                     { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 477: /* expr_no_bracket: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 478: /* expr_no_bracket: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 479: /* expr_no_bracket: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 480: /* expr_no_bracket: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 481: /* expr_no_bracket: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 482: /* expr_no_bracket: make_decl_no_bracket  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 483: /* expr_no_bracket: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 484: /* expr_no_bracket: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 485: /* expr_no_bracket: expr_field_no_bracket  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 486: /* expr_no_bracket: expr_mtag_no_bracket  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 487: /* expr_no_bracket: '!' expr_no_bracket  */
                                                         { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",(yyvsp[0].pExpression)); }
    break;

  case 488: /* expr_no_bracket: '~' expr_no_bracket  */
                                                         { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",(yyvsp[0].pExpression)); }
    break;

  case 489: /* expr_no_bracket: '+' expr_no_bracket  */
                                                             { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",(yyvsp[0].pExpression)); }
    break;

  case 490: /* expr_no_bracket: '-' expr_no_bracket  */
                                                             { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",(yyvsp[0].pExpression)); }
    break;

  case 491: /* expr_no_bracket: expr_no_bracket "<<" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 492: /* expr_no_bracket: expr_no_bracket ">>" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 493: /* expr_no_bracket: expr_no_bracket "<<<" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 494: /* expr_no_bracket: expr_no_bracket ">>>" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 495: /* expr_no_bracket: expr_no_bracket '+' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 496: /* expr_no_bracket: expr_no_bracket '-' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 497: /* expr_no_bracket: expr_no_bracket '*' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 498: /* expr_no_bracket: expr_no_bracket '/' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 499: /* expr_no_bracket: expr_no_bracket '%' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 500: /* expr_no_bracket: expr_no_bracket '<' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 501: /* expr_no_bracket: expr_no_bracket '>' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 502: /* expr_no_bracket: expr_no_bracket "==" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 503: /* expr_no_bracket: expr_no_bracket "!=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 504: /* expr_no_bracket: expr_no_bracket "<=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 505: /* expr_no_bracket: expr_no_bracket ">=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 506: /* expr_no_bracket: expr_no_bracket '&' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 507: /* expr_no_bracket: expr_no_bracket '|' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 508: /* expr_no_bracket: expr_no_bracket '^' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 509: /* expr_no_bracket: expr_no_bracket "&&" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 510: /* expr_no_bracket: expr_no_bracket "||" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 511: /* expr_no_bracket: expr_no_bracket "^^" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 512: /* expr_no_bracket: expr_no_bracket ".." expr_no_bracket  */
                                                                   {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 513: /* expr_no_bracket: "++" expr_no_bracket  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", (yyvsp[0].pExpression)); }
    break;

  case 514: /* expr_no_bracket: "--" expr_no_bracket  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", (yyvsp[0].pExpression)); }
    break;

  case 515: /* expr_no_bracket: expr_no_bracket "++"  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 516: /* expr_no_bracket: expr_no_bracket "--"  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 517: /* expr_no_bracket: '(' expr_list optional_comma ')'  */
                                                         {
            if ( (yyvsp[-2].pExpression)->rtti_isSequence() ) {
                auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-2])));
                mkt->values = sequenceToList((yyvsp[-2].pExpression));
                mkt->shorthandRecordNames = ast_tupleCollectShorthandNames(mkt->values);
                (yyval.pExpression) = mkt;
            } else if ( (yyvsp[-1].b) ) {
                auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-2])));
                mkt->values.push_back((yyvsp[-2].pExpression));
                mkt->shorthandRecordNames = ast_tupleCollectShorthandNames(mkt->values);
                (yyval.pExpression) = mkt;
            } else {
                (yyval.pExpression) = (yyvsp[-2].pExpression);
            }
        }
    break;

  case 518: /* expr_no_bracket: '(' make_struct_single ')'  */
                                      {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        for ( auto & arg : *(((ExprMakeStruct *)(yyvsp[-1].pExpression))->structs.back()) ) {
            mkt->values.push_back(arg->value);
            mkt->recordNames.push_back(arg->name);
        }
        // gc_node — don't delete Expression
        (yyval.pExpression) = mkt;
    }
    break;

  case 519: /* expr_no_bracket: expr_no_bracket '[' expr ']'  */
                                                            { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 520: /* expr_no_bracket: expr_no_bracket '.' '[' expr ']'  */
                                                                { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 521: /* expr_no_bracket: expr_no_bracket "?[" expr ']'  */
                                                            { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 522: /* expr_no_bracket: expr_no_bracket '.' "?[" expr ']'  */
                                                                { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 523: /* expr_no_bracket: expr_no_bracket "?." "name"  */
                                                            { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 524: /* expr_no_bracket: expr_no_bracket '.' "?." "name"  */
                                                                { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 525: /* expr_no_bracket: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 526: /* expr_no_bracket: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 527: /* expr_no_bracket: '*' expr_no_bracket  */
                                                              { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 528: /* expr_no_bracket: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 529: /* expr_no_bracket: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 530: /* expr_no_bracket: expr_generator  */
                                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 531: /* expr_no_bracket: expr_no_bracket "??" expr_no_bracket  */
                                                                         { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 532: /* expr_no_bracket: expr_no_bracket '?' expr_no_bracket ':' expr_no_bracket  */
                                                                                           {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 533: /* $@39: %empty  */
                                                          { yyextra->das_arrow_depth ++; }
    break;

  case 534: /* $@40: %empty  */
                                                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 535: /* expr_no_bracket: expr_no_bracket "is" "type" '<' $@39 type_declaration_no_options '>' $@40  */
                                                                                                                                                                  {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 536: /* expr_no_bracket: expr_no_bracket "is" basic_type_declaration  */
                                                                          {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 537: /* expr_no_bracket: expr_no_bracket "is" "name"  */
                                                         {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 538: /* expr_no_bracket: expr_no_bracket "as" "name"  */
                                                         {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 539: /* $@41: %empty  */
                                                          { yyextra->das_arrow_depth ++; }
    break;

  case 540: /* $@42: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 541: /* expr_no_bracket: expr_no_bracket "as" "type" '<' $@41 type_declaration '>' $@42  */
                                                                                                                                                       {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 542: /* expr_no_bracket: expr_no_bracket "as" basic_type_declaration  */
                                                                          {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 543: /* expr_no_bracket: expr_no_bracket '?' "as" "name"  */
                                                             {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 544: /* $@43: %empty  */
                                                              { yyextra->das_arrow_depth ++; }
    break;

  case 545: /* $@44: %empty  */
                                                                                                                          { yyextra->das_arrow_depth --; }
    break;

  case 546: /* expr_no_bracket: expr_no_bracket '?' "as" "type" '<' $@43 type_declaration '>' $@44  */
                                                                                                                                                           {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 547: /* expr_no_bracket: expr_no_bracket '?' "as" basic_type_declaration  */
                                                                              {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 548: /* expr_no_bracket: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 549: /* expr_no_bracket: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 550: /* expr_no_bracket: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 551: /* expr_no_bracket: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 552: /* expr_no_bracket: expr_method_call_no_bracket  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 553: /* expr_no_bracket: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 554: /* expr_no_bracket: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 555: /* expr_no_bracket: expr_no_bracket "<|" expr_no_bracket  */
                                                                      { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 556: /* expr_no_bracket: expr_no_bracket "|>" expr_no_bracket  */
                                                                      { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 557: /* expr_no_bracket: expr_no_bracket "|>" basic_type_declaration  */
                                                                     {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 558: /* expr_no_bracket: expr_call_pipe_no_bracket  */
                                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 559: /* expr_no_bracket: "unsafe" '(' expr ')'  */
                                         {
            (yyvsp[-1].pExpression)->alwaysSafe = true;
            (yyvsp[-1].pExpression)->userSaidItsSafe = true;
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    break;

  case 560: /* expr_no_bracket: expr_no_bracket "=>" expr_no_bracket  */
                                                               {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 561: /* expr_no_bracket: expr_no_bracket "=>" make_table_decl  */
                                                               {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 562: /* expr_no_bracket: expr_no_bracket "=>" array_comprehension  */
                                                                   {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 563: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 564: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 565: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list optional_emit_semis expression_block  */
                                                                                                                                                  {
        auto closure = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),(yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->returnType = new TypeDecl(Type::autoinfer);
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),closure,tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 566: /* expr_mtag_no_bracket: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 567: /* expr_mtag_no_bracket: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 568: /* expr_mtag_no_bracket: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 569: /* expr_mtag_no_bracket: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 570: /* expr_mtag_no_bracket: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 571: /* expr_mtag_no_bracket: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 572: /* expr_mtag_no_bracket: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 573: /* expr_mtag_no_bracket: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 574: /* expr_mtag_no_bracket: expr_no_bracket '.' "$f" '(' expr ')'  */
                                                                           {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 575: /* expr_mtag_no_bracket: expr_no_bracket "?." "$f" '(' expr ')'  */
                                                                            {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 576: /* expr_mtag_no_bracket: expr_no_bracket '.' '.' "$f" '(' expr ')'  */
                                                                               {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 577: /* expr_mtag_no_bracket: expr_no_bracket '.' "?." "$f" '(' expr ')'  */
                                                                                {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 578: /* expr_mtag_no_bracket: expr_no_bracket "as" "$f" '(' expr ')'  */
                                                                              {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 579: /* expr_mtag_no_bracket: expr_no_bracket '?' "as" "$f" '(' expr ')'  */
                                                                                  {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 580: /* expr_mtag_no_bracket: expr_no_bracket "is" "$f" '(' expr ')'  */
                                                                              {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 581: /* expr_mtag_no_bracket: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 582: /* optional_field_annotation: %empty  */
                                      { (yyval.aaList) = nullptr; }
    break;

  case 583: /* optional_field_annotation: metadata_argument_list  */
                                      { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 584: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 585: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 586: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 587: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 588: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 589: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 590: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 591: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 592: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 593: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 594: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 595: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 596: /* struct_variable_declaration_list: struct_variable_declaration_list "new line, semicolon"  */
                                                                 { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 597: /* struct_variable_declaration_list: struct_variable_declaration_list "typedef" "name" '=' type_declaration SEMICOLON  */
                                                                                                                {
        (yyval.pVarDeclList) = (yyvsp[-5].pVarDeclList);
        ast_structureAlias(scanner,(yyvsp[-3].s),(yyvsp[-1].pTypeDecl),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 598: /* $@45: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 599: /* struct_variable_declaration_list: struct_variable_declaration_list $@45 structure_variable_declaration SEMICOLON  */
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

  case 600: /* $@46: %empty  */
                                                                                                                     {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 601: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable "abstract" optional_constant $@46 function_declaration_header SEMICOLON  */
                                                          {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 602: /* $@47: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 603: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@47 function_declaration_header optional_emit_semis expression_block  */
                                                                                            {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-10].pVarDeclList),(yyvsp[-9].faList),(yyvsp[-6].b),(yyvsp[-7].b),(yyvsp[-5].i),(yyvsp[-4].b),(yyvsp[-2].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-8]),(yylsp[0])),tokAt(scanner,(yylsp[-9])));
            }
    break;

  case 604: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
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

  case 605: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
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

  case 606: /* function_argument_declaration_type: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))));
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 607: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 608: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 609: /* function_argument_list: function_argument_declaration_no_type ';' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 610: /* function_argument_list: function_argument_declaration_type ';' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 611: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 612: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 613: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 614: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 615: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 616: /* tuple_alias_type_list: %empty  */
      {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 617: /* tuple_alias_type_list: tuple_type  */
                       {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 618: /* tuple_alias_type_list: tuple_alias_type_list semis tuple_type  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
        if ( !yyextra->g_CommentReaders.empty() ) {
            for ( auto & crd : yyextra->g_CommentReaders ) {
                for ( const auto & nl : *((yyvsp[0].pVarDecl)->pNameList) ) {
                    crd->afterTupleEntry(nl.name.c_str(), nl.at);
                }
            }
        }
    }
    break;

  case 619: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 620: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 621: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 622: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 623: /* variant_alias_type_list: variant_type  */
                         {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 624: /* variant_alias_type_list: variant_alias_type_list semis variant_type  */
                                                               {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
        if ( !yyextra->g_CommentReaders.empty() ) {
            for ( auto & crd : yyextra->g_CommentReaders ) {
                for ( const auto & nl : *((yyvsp[0].pVarDecl)->pNameList) ) {
                    crd->afterVariantEntry(nl.name.c_str(), nl.at);
                }
            }
        }
    }
    break;

  case 625: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 626: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 627: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 628: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 629: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 630: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 631: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 632: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 633: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 634: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 635: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 636: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 637: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 638: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 639: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 640: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 641: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 642: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 643: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 644: /* global_let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 645: /* global_let_variable_name_with_pos_list: global_let_variable_name_with_pos_list ',' "name"  */
                                                                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 646: /* variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 647: /* variable_declaration_list: variable_declaration_list SEMICOLON  */
                                                  {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 648: /* variable_declaration_list: variable_declaration_list let_variable_declaration  */
                                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 649: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 650: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 651: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 652: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                         {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 653: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                               {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 654: /* global_let_variable_declaration: global_let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 655: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 656: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 657: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 658: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 659: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 660: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 661: /* global_variable_declaration_list: global_variable_declaration_list SEMICOLON  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 662: /* $@48: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 663: /* global_variable_declaration_list: global_variable_declaration_list $@48 optional_field_annotation let_variable_declaration  */
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

  case 664: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 665: /* $@49: %empty  */
                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 666: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@49 optional_field_annotation global_let_variable_declaration  */
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

  case 667: /* enum_expression: "name"  */
                   {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        delete (yyvsp[0].s);
    }
    break;

  case 668: /* enum_expression: "name" '=' expr  */
                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[-2].s),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-2])));
        delete (yyvsp[-2].s);
    }
    break;

  case 671: /* enum_list: %empty  */
        {
        (yyval.pEnumList) = new Enumeration();
    }
    break;

  case 672: /* enum_list: enum_expression  */
                            {
        (yyval.pEnumList) = new Enumeration();
        if ( !(yyval.pEnumList)->add((yyvsp[0].pEnumPair)->name,(yyvsp[0].pEnumPair)->expr,(yyvsp[0].pEnumPair)->at) ) {
            das2_yyerror(scanner,"enumeration already declared " + (yyvsp[0].pEnumPair)->name, (yyvsp[0].pEnumPair)->at,
                CompilationError::already_declared_enumerator);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            for ( auto & crd : yyextra->g_CommentReaders ) {
                crd->afterEnumerationEntry((yyvsp[0].pEnumPair)->name.c_str(), (yyvsp[0].pEnumPair)->at);
            }
        }
        delete (yyvsp[0].pEnumPair);
    }
    break;

  case 673: /* enum_list: enum_list commas enum_expression  */
                                                 {
        if ( !(yyvsp[-2].pEnumList)->add((yyvsp[0].pEnumPair)->name,(yyvsp[0].pEnumPair)->expr,(yyvsp[0].pEnumPair)->at) ) {
            das2_yyerror(scanner,"enumeration already declared " + (yyvsp[0].pEnumPair)->name, (yyvsp[0].pEnumPair)->at,
                CompilationError::already_declared_enumerator);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            for ( auto & crd : yyextra->g_CommentReaders ) {
                crd->afterEnumerationEntry((yyvsp[0].pEnumPair)->name.c_str(), (yyvsp[0].pEnumPair)->at);
            }
        }
        delete (yyvsp[0].pEnumPair);
        (yyval.pEnumList) = (yyvsp[-2].pEnumList);
    }
    break;

  case 674: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 675: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 676: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 677: /* $@50: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 678: /* single_alias: optional_public_or_private_alias "name" $@50 '=' type_declaration  */
                                  {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        (yyvsp[0].pTypeDecl)->isPrivateAlias = !(yyvsp[-4].b);
        if ( (yyvsp[0].pTypeDecl)->baseType == Type::alias ) {
            das2_yyerror(scanner,"alias cannot be defined in terms of another alias "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::invalid_type_alias);
        }
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !yyextra->g_Program->addAlias((yyvsp[0].pTypeDecl)) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::already_declared_type_alias);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterAlias((yyvsp[-3].s)->c_str(),pubename);
        }
        delete (yyvsp[-3].s);
    }
    break;

  case 680: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 681: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 682: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 683: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 684: /* optional_enum_basic_type_declaration: %empty  */
        {
        (yyval.type) = Type::tInt;
    }
    break;

  case 685: /* optional_enum_basic_type_declaration: ':' enum_basic_type_declaration  */
                                              {
        (yyval.type) = (yyvsp[0].type);
    }
    break;

  case 692: /* $@51: %empty  */
                                                                     {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 693: /* $@52: %empty  */
                                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 694: /* $@53: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 695: /* enum_declaration: optional_annotation_list_with_emit_semis "enum" $@51 optional_public_or_private_enum enum_name optional_enum_basic_type_declaration optional_emit_commas '{' $@52 enum_list optional_commas $@53 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-8].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-12].faList),tokAt(scanner,(yylsp[-12])),(yyvsp[-9].b),(yyvsp[-8].pEnum),(yyvsp[-3].pEnumList),(yyvsp[-7].type));
    }
    break;

  case 696: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 697: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 698: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 699: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 700: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 701: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 702: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 703: /* class_or_struct: "class" "template"  */
                                  { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 704: /* class_or_struct: "struct" "template"  */
                                  { (yyval.i) = CorS_StructTemplate; }
    break;

  case 705: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 706: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 707: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 708: /* optional_struct_variable_declaration_list: ';'  */
            {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 709: /* optional_struct_variable_declaration_list: '{' struct_variable_declaration_list '}'  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 710: /* $@54: %empty  */
                                                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 711: /* $@55: %empty  */
                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 712: /* $@56: %empty  */
                                             {
        if ( (yyvsp[-1].pStructure) ) {
            (yyvsp[-1].pStructure)->isClass = (yyvsp[-4].i)==CorS_Class || (yyvsp[-4].i)==CorS_ClassTemplate;
            (yyvsp[-1].pStructure)->isTemplate = (yyvsp[-4].i)==CorS_ClassTemplate || (yyvsp[-4].i)==CorS_StructTemplate;
            (yyvsp[-1].pStructure)->privateStructure = !(yyvsp[-3].b);
        }
    }
    break;

  case 713: /* structure_declaration: optional_annotation_list_with_emit_semis $@54 class_or_struct optional_public_or_private_structure $@55 structure_name optional_emit_semis $@56 optional_struct_variable_declaration_list  */
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

  case 714: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 715: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 716: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 717: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 718: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 719: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 720: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 721: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 722: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 723: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 724: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 725: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 726: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 727: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 728: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 729: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 730: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 731: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 732: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 733: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 734: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 735: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 736: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 737: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 738: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 739: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 740: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 741: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 742: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 743: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 744: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 745: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 746: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 747: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 748: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 749: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 750: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 751: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 752: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 753: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 754: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 755: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 756: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 757: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 758: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 759: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 760: /* bitfield_bits: bitfield_bits ',' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 761: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<tuple<string,Expression *>>();
        (yyval.pNameExprList) = pSL;

    }
    break;

  case 762: /* bitfield_alias_bits: "name"  */
                   {
        (yyval.pNameExprList) = new vector<tuple<string,Expression *>>();
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pNameExprList)->emplace_back(*(yyvsp[0].s),nullptr);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[0].s)->c_str(),atvname);
        }
        delete (yyvsp[0].s);
    }
    break;

  case 763: /* bitfield_alias_bits: "name" '=' expr  */
                                   {
        (yyval.pNameExprList) = new vector<tuple<string,Expression *>>();
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyval.pNameExprList)->emplace_back(*(yyvsp[-2].s),(yyvsp[0].pExpression));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[-2].s)->c_str(),atvname);
        }
        delete (yyvsp[-2].s);
    }
    break;

  case 764: /* bitfield_alias_bits: bitfield_alias_bits commas "name"  */
                                                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameExprList)->emplace_back(*(yyvsp[0].s),nullptr);
        (yyval.pNameExprList) = (yyvsp[-2].pNameExprList);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[0].s)->c_str(),atvname);
        }
        delete (yyvsp[0].s);
    }
    break;

  case 765: /* bitfield_alias_bits: bitfield_alias_bits commas "name" '=' expr  */
                                                                    {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyvsp[-4].pNameExprList)->emplace_back(*(yyvsp[-2].s),(yyvsp[0].pExpression));
        (yyval.pNameExprList) = (yyvsp[-4].pNameExprList);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[-2].s)->c_str(),atvname);
        }
        delete (yyvsp[-2].s);
    }
    break;

  case 766: /* bitfield_basic_type_declaration: %empty  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 767: /* bitfield_basic_type_declaration: ':' "uint8"  */
                             { (yyval.type) = Type::tBitfield8; }
    break;

  case 768: /* bitfield_basic_type_declaration: ':' "uint16"  */
                             { (yyval.type) = Type::tBitfield16; }
    break;

  case 769: /* bitfield_basic_type_declaration: ':' "uint"  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 770: /* bitfield_basic_type_declaration: ':' "uint64"  */
                             { (yyval.type) = Type::tBitfield64; }
    break;

  case 771: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' '>'  */
                                                                          {
            (yyval.pTypeDecl) = new TypeDecl((yyvsp[-2].type));
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-2]));
    }
    break;

  case 772: /* $@57: %empty  */
                                                                     { yyextra->das_arrow_depth ++; }
    break;

  case 773: /* $@58: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 774: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' $@57 bitfield_bits '>' $@58  */
                                                                                                                                                             {
            (yyval.pTypeDecl) = new TypeDecl((yyvsp[-5].type));
            (yyval.pTypeDecl)->argNames = *(yyvsp[-2].pNameList);
            auto maxBits = (yyval.pTypeDecl)->maxBitfieldBits();
            if ( (yyval.pTypeDecl)->argNames.size()>maxBits ) {
                das_yyerror(scanner,"only " + to_string(maxBits) + " different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-5])),
                    CompilationError::exceeds_bitfield);
            }
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
            delete (yyvsp[-2].pNameList);
    }
    break;

  case 777: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 778: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 779: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 780: /* dim_list: '[' ']'  */
                {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 781: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 782: /* dim_list: dim_list '[' ']'  */
                              {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 783: /* type_declaration_no_options: type_declaration_no_options_no_dim  */
                                                     {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 784: /* type_declaration_no_options: type_declaration_no_options_no_dim dim_list  */
                                                                       {
        if ( (yyvsp[-1].pTypeDecl)->baseType==Type::typeDecl ) {
            das2_yyerror(scanner,"type declaration can`t be used as array base type",tokAt(scanner,(yylsp[-1])),
                CompilationError::invalid_array_type);
        } else if ( (yyvsp[-1].pTypeDecl)->baseType==Type::typeMacro ) {
            das2_yyerror(scanner,"macro can`t be used as array base type",tokAt(scanner,(yylsp[-1])),
                CompilationError::invalid_array_type);
        }
        (yyvsp[-1].pTypeDecl)->dim.insert((yyvsp[-1].pTypeDecl)->dim.begin(), (yyvsp[0].pTypeDecl)->dim.begin(), (yyvsp[0].pTypeDecl)->dim.end());
        (yyvsp[-1].pTypeDecl)->dimExpr.insert((yyvsp[-1].pTypeDecl)->dimExpr.begin(), (yyvsp[0].pTypeDecl)->dimExpr.begin(), (yyvsp[0].pTypeDecl)->dimExpr.end());
        (yyvsp[-1].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyvsp[0].pTypeDecl)->dimExpr.clear();
    }
    break;

  case 785: /* optional_expr_list_in_braces: %empty  */
            { (yyval.pExpression) = nullptr; }
    break;

  case 786: /* optional_expr_list_in_braces: '(' expr_list optional_comma ')'  */
                                                { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 787: /* type_declaration_no_options_no_dim: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 788: /* type_declaration_no_options_no_dim: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 789: /* type_declaration_no_options_no_dim: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 790: /* type_declaration_no_options_no_dim: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 791: /* $@59: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 792: /* $@60: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 793: /* type_declaration_no_options_no_dim: "type" '<' $@59 type_declaration '>' $@60  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 794: /* type_declaration_no_options_no_dim: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 795: /* type_declaration_no_options_no_dim: name_in_namespace '(' optional_expr_list ')'  */
                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 796: /* type_declaration_no_options_no_dim: '$' name_in_namespace optional_expr_list_in_braces  */
                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-1]), (yylsp[0]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[0].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-1])), *(yyvsp[-1].s)));
        delete (yyvsp[-1].s);
    }
    break;

  case 797: /* $@61: %empty  */
                                    { yyextra->das_arrow_depth ++; }
    break;

  case 798: /* type_declaration_no_options_no_dim: name_in_namespace '<' $@61 type_declaration_no_options_list '>' optional_expr_list_in_braces  */
                                                                                                                                                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-5]), (yylsp[0]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-2].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-5])), *(yyvsp[-5].s)));
        delete (yyvsp[-5].s);
    }
    break;

  case 799: /* $@62: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 800: /* type_declaration_no_options_no_dim: '$' name_in_namespace '<' $@62 type_declaration_no_options_list '>' optional_expr_list_in_braces  */
                                                                                                                                                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-5]), (yylsp[0]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-2].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-5])), *(yyvsp[-5].s)));
        delete (yyvsp[-5].s);
    }
    break;

  case 801: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 802: /* type_declaration_no_options_no_dim: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 803: /* type_declaration_no_options_no_dim: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 804: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 805: /* type_declaration_no_options_no_dim: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 806: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 807: /* type_declaration_no_options_no_dim: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 808: /* type_declaration_no_options_no_dim: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 809: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 810: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 811: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 812: /* type_declaration_no_options_no_dim: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 813: /* $@63: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 814: /* $@64: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 815: /* type_declaration_no_options_no_dim: "smart_ptr" '<' $@63 type_declaration '>' $@64  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 816: /* type_declaration_no_options_no_dim: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 817: /* $@65: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 818: /* $@66: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 819: /* type_declaration_no_options_no_dim: "array" '<' $@65 type_declaration '>' $@66  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 820: /* $@67: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 821: /* $@68: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 822: /* type_declaration_no_options_no_dim: "table" '<' $@67 table_type_pair '>' $@68  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 823: /* $@69: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 824: /* $@70: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 825: /* type_declaration_no_options_no_dim: "iterator" '<' $@69 type_declaration '>' $@70  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 826: /* type_declaration_no_options_no_dim: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 827: /* $@71: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 828: /* $@72: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 829: /* type_declaration_no_options_no_dim: "block" '<' $@71 type_declaration '>' $@72  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 830: /* $@73: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 831: /* $@74: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 832: /* type_declaration_no_options_no_dim: "block" '<' $@73 optional_function_argument_list optional_function_type '>' $@74  */
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

  case 833: /* type_declaration_no_options_no_dim: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 834: /* $@75: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 835: /* $@76: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 836: /* type_declaration_no_options_no_dim: "function" '<' $@75 type_declaration '>' $@76  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 837: /* $@77: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 838: /* $@78: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 839: /* type_declaration_no_options_no_dim: "function" '<' $@77 optional_function_argument_list optional_function_type '>' $@78  */
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

  case 840: /* type_declaration_no_options_no_dim: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 841: /* $@79: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 842: /* $@80: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 843: /* type_declaration_no_options_no_dim: "lambda" '<' $@79 type_declaration '>' $@80  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 844: /* $@81: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 845: /* $@82: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 846: /* type_declaration_no_options_no_dim: "lambda" '<' $@81 optional_function_argument_list optional_function_type '>' $@82  */
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

  case 847: /* $@83: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 848: /* $@84: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 849: /* type_declaration_no_options_no_dim: "tuple" '<' $@83 tuple_type_list '>' $@84  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 850: /* $@85: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 851: /* $@86: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 852: /* type_declaration_no_options_no_dim: "variant" '<' $@85 variant_type_list '>' $@86  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 853: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 854: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 855: /* type_declaration: type_declaration '|' '#'  */
                                             {
        if ( (yyvsp[-2].pTypeDecl)->baseType==Type::option ) {
            (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
            (yyval.pTypeDecl)->argTypes.push_back(new TypeDecl(*(yyvsp[-2].pTypeDecl)->argTypes.back()));
            (yyvsp[-2].pTypeDecl)->argTypes.back()->temporary ^= true;
        } else {
            (yyval.pTypeDecl) = new TypeDecl(Type::option);
            (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
            (yyval.pTypeDecl)->argTypes.push_back((yyvsp[-2].pTypeDecl));
            (yyval.pTypeDecl)->argTypes.push_back(new TypeDecl(*(yyvsp[-2].pTypeDecl)));
            (yyval.pTypeDecl)->argTypes.back()->temporary ^= true;
        }
    }
    break;

  case 856: /* $@87: %empty  */
                   {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 857: /* $@88: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 858: /* $@89: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 859: /* $@90: %empty  */
                                                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 860: /* tuple_alias_declaration: "tuple" $@87 optional_public_or_private_alias "name" optional_emit_semis $@88 '{' $@89 tuple_alias_type_list optional_semis $@90 '}'  */
          {
        auto vtype = new TypeDecl(Type::tTuple);
        vtype->alias = *(yyvsp[-8].s);
        vtype->at = tokAt(scanner,(yylsp[-8]));
        vtype->isPrivateAlias = !(yyvsp[-9].b);
        varDeclToTypeDecl(scanner, vtype, (yyvsp[-3].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-8].s),tokAt(scanner,(yylsp[-8])),
                CompilationError::already_declared_type_alias);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-8]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTuple((yyvsp[-8].s)->c_str(),atvname);
        }
        delete (yyvsp[-8].s);
    }
    break;

  case 861: /* $@91: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 862: /* $@92: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 863: /* $@93: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 864: /* $@94: %empty  */
                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 865: /* variant_alias_declaration: "variant" $@91 optional_public_or_private_alias "name" optional_emit_semis $@92 '{' $@93 variant_alias_type_list optional_semis $@94 '}'  */
          {
        auto vtype = new TypeDecl(Type::tVariant);
        vtype->alias = *(yyvsp[-8].s);
        vtype->at = tokAt(scanner,(yylsp[-8]));
        vtype->isPrivateAlias = !(yyvsp[-9].b);
        varDeclToTypeDecl(scanner, vtype, (yyvsp[-3].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-8].s),tokAt(scanner,(yylsp[-8])),
                CompilationError::already_declared_type_alias);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-8]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariant((yyvsp[-8].s)->c_str(),atvname);
        }
        delete (yyvsp[-8].s);
    }
    break;

  case 866: /* $@95: %empty  */
                      {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 867: /* $@96: %empty  */
                                                                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 868: /* $@97: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 869: /* $@98: %empty  */
                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 870: /* bitfield_alias_declaration: "bitfield" $@95 optional_public_or_private_alias "name" bitfield_basic_type_declaration optional_emit_commas $@96 '{' $@97 bitfield_alias_bits optional_commas $@98 '}'  */
          {
        auto btype = new TypeDecl((yyvsp[-8].type));
        btype->alias = *(yyvsp[-9].s);
        btype->at = tokAt(scanner,(yylsp[-9]));
        btype->isPrivateAlias = !(yyvsp[-10].b);
        for ( auto & p : *(yyvsp[-3].pNameExprList) ) {
            if ( !get<1>(p) ) {
                btype->argNames.push_back(get<0>(p));
            }
        }
        auto maxBits = btype->maxBitfieldBits();
        if ( btype->argNames.size()>maxBits ) {
            das_yyerror(scanner,"only " + to_string(maxBits) + " different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-9])),
                CompilationError::exceeds_bitfield);
        }
        for ( auto & p : *(yyvsp[-3].pNameExprList) ) {
            if ( get<1>(p) ) {
                ast_globalBitfieldConst ( scanner, btype, (yyvsp[-10].b), get<0>(p), get<1>(p) );
            }
        }
        if ( !yyextra->g_Program->addAlias(btype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-9].s),tokAt(scanner,(yylsp[-9])),
                CompilationError::already_declared_type_alias);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-9]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfield((yyvsp[-9].s)->c_str(),atvname);
        }
        delete (yyvsp[-9].s);
        delete (yyvsp[-3].pNameExprList);
    }
    break;

  case 871: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 872: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 873: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 874: /* make_decl: make_table_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 875: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 876: /* make_decl: table_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 877: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 878: /* make_decl_no_bracket: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 879: /* make_decl_no_bracket: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 880: /* make_decl_no_bracket: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 881: /* make_decl_no_bracket: table_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 882: /* make_decl_no_bracket: make_table_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 883: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 884: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 885: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 886: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 887: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 888: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 889: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 890: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 891: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 892: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 893: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 894: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 895: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 896: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 897: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 898: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 899: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 900: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 901: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 902: /* $@99: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 903: /* $@100: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 904: /* make_struct_decl: "struct" '<' $@99 type_declaration_no_options '>' $@100 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 905: /* $@101: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 906: /* $@102: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 907: /* make_struct_decl: "class" '<' $@101 type_declaration_no_options '>' $@102 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 908: /* $@103: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 909: /* $@104: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 910: /* make_struct_decl: "variant" '<' $@103 variant_type_list '>' $@104 '(' use_initializer make_variant_dim ')'  */
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

  case 911: /* $@105: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 912: /* $@106: %empty  */
                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 913: /* make_struct_decl: "variant" "type" '<' $@105 type_declaration_no_options '>' $@106 '(' use_initializer make_variant_dim ')'  */
                                                                                                                                                                                                    {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-10]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 914: /* $@107: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 915: /* $@108: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 916: /* make_struct_decl: "default" '<' $@107 type_declaration_no_options '>' $@108 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 917: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = new TypeDecl(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 918: /* $@109: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 919: /* $@110: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 920: /* make_tuple_call: "tuple" '<' $@109 tuple_type_list '>' $@110 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 921: /* make_dim_decl: '[' optional_expr_list ']'  */
                                                  {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-2])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = new TypeDecl(Type::autoinfer);
            mka->gen2 = true;
            auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),"to_array_move");
            tam->arguments.push_back(mka);
            (yyval.pExpression) = tam;
        } else {
            auto mks = new ExprMakeStruct();
            mks->at = tokAt(scanner,(yylsp[-2]));
            mks->makeType = new TypeDecl(Type::tArray);
            mks->makeType->firstType = new TypeDecl(Type::autoinfer);
            mks->useInitializer = true;
            mks->alwaysUseInitializer = true;
            (yyval.pExpression) = mks;
        }
    }
    break;

  case 922: /* $@111: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 923: /* $@112: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 924: /* make_dim_decl: "array" "struct" '<' $@111 type_declaration_no_options '>' $@112 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 925: /* $@113: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 926: /* $@114: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 927: /* make_dim_decl: "array" "tuple" '<' $@113 tuple_type_list '>' $@114 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 928: /* $@115: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 929: /* $@116: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 930: /* make_dim_decl: "array" "variant" '<' $@115 variant_type_list '>' $@116 '(' make_variant_dim ')'  */
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

  case 931: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer);
        mka->gen2 = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 932: /* $@117: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 933: /* $@118: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 934: /* make_dim_decl: "array" '<' $@117 type_declaration_no_options '>' $@118 '(' optional_expr_list ')'  */
                                                                                                                                                                        {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-8])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = (yyvsp[-5].pTypeDecl);
            mka->gen2 = true;
            auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_array_move");
            tam->arguments.push_back(mka);
            (yyval.pExpression) = tam;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->makeType = new TypeDecl(Type::tArray);
            msd->makeType->firstType = (yyvsp[-5].pTypeDecl);
            msd->at = tokAt(scanner,(yylsp[-5]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 935: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 936: /* $@119: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 937: /* $@120: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 938: /* make_dim_decl: "fixed_array" '<' $@119 type_declaration_no_options '>' $@120 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 939: /* expr_map_tuple_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 940: /* expr_map_tuple_list: expr_map_tuple_list ',' expr  */
                                                      {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 941: /* push_table_nesting: %empty  */
                    {
        yyextra->das_nested_parentheses ++;
    }
    break;

  case 942: /* make_table_decl: '{' push_table_nesting optional_emit_semis optional_expr_map_tuple_list '}'  */
                                                                                                     {
        yyextra->das_nested_parentheses --;
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = new TypeDecl(Type::autoinfer);
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto mks = new ExprMakeStruct();
            mks->at = tokAt(scanner,(yylsp[-4]));
            mks->makeType = new TypeDecl(Type::tTable);
            mks->makeType->firstType = new TypeDecl(Type::autoinfer);
            mks->makeType->secondType = new TypeDecl(Type::autoinfer);
            mks->useInitializer = true;
            mks->alwaysUseInitializer = true;
            (yyval.pExpression) = mks;
        }
    }
    break;

  case 943: /* make_table_call: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 944: /* make_table_call: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
                                                                                                                 {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-6])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = (yyvsp[-4].pTypeDecl);
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-6]));
            msd->makeType = new TypeDecl(Type::tTable);
            msd->makeType->firstType = (yyvsp[-4].pTypeDecl);
            msd->makeType->secondType = new TypeDecl(Type::tVoid);
            msd->at = tokAt(scanner,(yylsp[-6]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 945: /* make_table_call: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
                                                                                                                                                             {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-8])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = new TypeDecl(Type::tTuple);
            mka->makeType->argTypes.push_back((yyvsp[-6].pTypeDecl));
            mka->makeType->argTypes.push_back((yyvsp[-4].pTypeDecl));
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->makeType = new TypeDecl(Type::tTable);
            msd->makeType->firstType = (yyvsp[-6].pTypeDecl);
            msd->makeType->secondType = (yyvsp[-4].pTypeDecl);
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 946: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 947: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 948: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 949: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 950: /* table_comprehension: '[' "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                               {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 951: /* table_comprehension: '[' "iterator" "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 952: /* array_comprehension: '{' push_table_nesting optional_emit_semis "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where '}'  */
                                                                                                                                                                                                      {
        yyextra->das_nested_parentheses --;
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
                CompilationError::invalid_expression);
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


