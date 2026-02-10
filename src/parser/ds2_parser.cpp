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
  YYSYMBOL_require_module = 230,           /* require_module  */
  YYSYMBOL_is_public_module = 231,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 232,       /* expect_declaration  */
  YYSYMBOL_expect_list = 233,              /* expect_list  */
  YYSYMBOL_expect_error = 234,             /* expect_error  */
  YYSYMBOL_expression_label = 235,         /* expression_label  */
  YYSYMBOL_expression_goto = 236,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 237,      /* elif_or_static_elif  */
  YYSYMBOL_emit_semis = 238,               /* emit_semis  */
  YYSYMBOL_optional_emit_semis = 239,      /* optional_emit_semis  */
  YYSYMBOL_expression_else = 240,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 241,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 242, /* expression_else_one_liner  */
  YYSYMBOL_expression_if_one_liner = 243,  /* expression_if_one_liner  */
  YYSYMBOL_semis = 244,                    /* semis  */
  YYSYMBOL_optional_semis = 245,           /* optional_semis  */
  YYSYMBOL_expression_if_block = 246,      /* expression_if_block  */
  YYSYMBOL_247_3 = 247,                    /* $@3  */
  YYSYMBOL_248_4 = 248,                    /* $@4  */
  YYSYMBOL_249_5 = 249,                    /* $@5  */
  YYSYMBOL_expression_else_block = 250,    /* expression_else_block  */
  YYSYMBOL_251_6 = 251,                    /* $@6  */
  YYSYMBOL_252_7 = 252,                    /* $@7  */
  YYSYMBOL_253_8 = 253,                    /* $@8  */
  YYSYMBOL_expression_if_then_else = 254,  /* expression_if_then_else  */
  YYSYMBOL_255_9 = 255,                    /* $@9  */
  YYSYMBOL_expression_if_then_else_oneliner = 256, /* expression_if_then_else_oneliner  */
  YYSYMBOL_for_variable_name_with_pos_list = 257, /* for_variable_name_with_pos_list  */
  YYSYMBOL_expression_for_loop = 258,      /* expression_for_loop  */
  YYSYMBOL_259_10 = 259,                   /* $@10  */
  YYSYMBOL_expression_unsafe = 260,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 261,    /* expression_while_loop  */
  YYSYMBOL_262_11 = 262,                   /* $@11  */
  YYSYMBOL_expression_with = 263,          /* expression_with  */
  YYSYMBOL_264_12 = 264,                   /* $@12  */
  YYSYMBOL_expression_with_alias = 265,    /* expression_with_alias  */
  YYSYMBOL_annotation_argument_value = 266, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 267, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 268, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 269,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 270, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 271,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 272, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 273, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 274,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 275,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 276, /* optional_annotation_list  */
  YYSYMBOL_optional_annotation_list_with_emit_semis = 277, /* optional_annotation_list_with_emit_semis  */
  YYSYMBOL_optional_function_argument_list = 278, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 279,   /* optional_function_type  */
  YYSYMBOL_function_name = 280,            /* function_name  */
  YYSYMBOL_optional_template = 281,        /* optional_template  */
  YYSYMBOL_global_function_declaration = 282, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 283, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 284, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 285,     /* function_declaration  */
  YYSYMBOL_286_13 = 286,                   /* $@13  */
  YYSYMBOL_expression_block_finally = 287, /* expression_block_finally  */
  YYSYMBOL_288_14 = 288,                   /* $@14  */
  YYSYMBOL_289_15 = 289,                   /* $@15  */
  YYSYMBOL_expression_block = 290,         /* expression_block  */
  YYSYMBOL_291_16 = 291,                   /* $@16  */
  YYSYMBOL_292_17 = 292,                   /* $@17  */
  YYSYMBOL_expr_call_pipe = 293,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 294,           /* expression_any  */
  YYSYMBOL_expressions = 295,              /* expressions  */
  YYSYMBOL_optional_expr_list = 296,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_map_tuple_list = 297, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 298, /* type_declaration_no_options_list  */
  YYSYMBOL_name_in_namespace = 299,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 300,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 301,     /* new_type_declaration  */
  YYSYMBOL_302_18 = 302,                   /* $@18  */
  YYSYMBOL_303_19 = 303,                   /* $@19  */
  YYSYMBOL_expr_new = 304,                 /* expr_new  */
  YYSYMBOL_expression_break = 305,         /* expression_break  */
  YYSYMBOL_expression_continue = 306,      /* expression_continue  */
  YYSYMBOL_expression_return = 307,        /* expression_return  */
  YYSYMBOL_expression_yield = 308,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 309,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 310,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 311,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 312,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 313,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 314, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 315,           /* expression_let  */
  YYSYMBOL_expr_cast = 316,                /* expr_cast  */
  YYSYMBOL_317_20 = 317,                   /* $@20  */
  YYSYMBOL_318_21 = 318,                   /* $@21  */
  YYSYMBOL_319_22 = 319,                   /* $@22  */
  YYSYMBOL_320_23 = 320,                   /* $@23  */
  YYSYMBOL_321_24 = 321,                   /* $@24  */
  YYSYMBOL_322_25 = 322,                   /* $@25  */
  YYSYMBOL_expr_type_decl = 323,           /* expr_type_decl  */
  YYSYMBOL_324_26 = 324,                   /* $@26  */
  YYSYMBOL_325_27 = 325,                   /* $@27  */
  YYSYMBOL_expr_type_info = 326,           /* expr_type_info  */
  YYSYMBOL_expr_list = 327,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 328,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 329,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 330,            /* capture_entry  */
  YYSYMBOL_capture_list = 331,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 332,    /* optional_capture_list  */
  YYSYMBOL_expr_full_block = 333,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 334, /* expr_full_block_assumed_piped  */
  YYSYMBOL_expr_numeric_const = 335,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 336,              /* expr_assign  */
  YYSYMBOL_expr_named_call = 337,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 338,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 339,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 340,           /* func_addr_expr  */
  YYSYMBOL_341_28 = 341,                   /* $@28  */
  YYSYMBOL_342_29 = 342,                   /* $@29  */
  YYSYMBOL_343_30 = 343,                   /* $@30  */
  YYSYMBOL_344_31 = 344,                   /* $@31  */
  YYSYMBOL_expr_field = 345,               /* expr_field  */
  YYSYMBOL_346_32 = 346,                   /* $@32  */
  YYSYMBOL_347_33 = 347,                   /* $@33  */
  YYSYMBOL_expr_call = 348,                /* expr_call  */
  YYSYMBOL_expr = 349,                     /* expr  */
  YYSYMBOL_350_34 = 350,                   /* $@34  */
  YYSYMBOL_351_35 = 351,                   /* $@35  */
  YYSYMBOL_352_36 = 352,                   /* $@36  */
  YYSYMBOL_353_37 = 353,                   /* $@37  */
  YYSYMBOL_354_38 = 354,                   /* $@38  */
  YYSYMBOL_355_39 = 355,                   /* $@39  */
  YYSYMBOL_expr_generator = 356,           /* expr_generator  */
  YYSYMBOL_expr_mtag = 357,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 358, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 359,        /* optional_override  */
  YYSYMBOL_optional_constant = 360,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 361, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 362, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 363, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 364, /* struct_variable_declaration_list  */
  YYSYMBOL_365_40 = 365,                   /* $@40  */
  YYSYMBOL_366_41 = 366,                   /* $@41  */
  YYSYMBOL_367_42 = 367,                   /* $@42  */
  YYSYMBOL_function_argument_declaration_no_type = 368, /* function_argument_declaration_no_type  */
  YYSYMBOL_function_argument_declaration_type = 369, /* function_argument_declaration_type  */
  YYSYMBOL_function_argument_list = 370,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 371,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 372,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 373,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 374,             /* variant_type  */
  YYSYMBOL_variant_type_list = 375,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 376,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 377,             /* copy_or_move  */
  YYSYMBOL_variable_declaration_no_type = 378, /* variable_declaration_no_type  */
  YYSYMBOL_variable_declaration_type = 379, /* variable_declaration_type  */
  YYSYMBOL_variable_declaration = 380,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 381,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 382,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 383, /* let_variable_name_with_pos_list  */
  YYSYMBOL_global_let_variable_name_with_pos_list = 384, /* global_let_variable_name_with_pos_list  */
  YYSYMBOL_variable_declaration_list = 385, /* variable_declaration_list  */
  YYSYMBOL_let_variable_declaration = 386, /* let_variable_declaration  */
  YYSYMBOL_global_let_variable_declaration = 387, /* global_let_variable_declaration  */
  YYSYMBOL_optional_shared = 388,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 389, /* optional_public_or_private_variable  */
  YYSYMBOL_global_variable_declaration_list = 390, /* global_variable_declaration_list  */
  YYSYMBOL_391_43 = 391,                   /* $@43  */
  YYSYMBOL_global_let = 392,               /* global_let  */
  YYSYMBOL_393_44 = 393,                   /* $@44  */
  YYSYMBOL_enum_expression = 394,          /* enum_expression  */
  YYSYMBOL_commas = 395,                   /* commas  */
  YYSYMBOL_enum_list = 396,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 397, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 398,             /* single_alias  */
  YYSYMBOL_399_45 = 399,                   /* $@45  */
  YYSYMBOL_alias_declaration = 400,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 401, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 402,                /* enum_name  */
  YYSYMBOL_optional_enum_basic_type_declaration = 403, /* optional_enum_basic_type_declaration  */
  YYSYMBOL_optional_commas = 404,          /* optional_commas  */
  YYSYMBOL_emit_commas = 405,              /* emit_commas  */
  YYSYMBOL_optional_emit_commas = 406,     /* optional_emit_commas  */
  YYSYMBOL_enum_declaration = 407,         /* enum_declaration  */
  YYSYMBOL_408_46 = 408,                   /* $@46  */
  YYSYMBOL_409_47 = 409,                   /* $@47  */
  YYSYMBOL_410_48 = 410,                   /* $@48  */
  YYSYMBOL_optional_structure_parent = 411, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 412,          /* optional_sealed  */
  YYSYMBOL_structure_name = 413,           /* structure_name  */
  YYSYMBOL_class_or_struct = 414,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 415, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 416, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 417,    /* structure_declaration  */
  YYSYMBOL_418_49 = 418,                   /* $@49  */
  YYSYMBOL_419_50 = 419,                   /* $@50  */
  YYSYMBOL_420_51 = 420,                   /* $@51  */
  YYSYMBOL_variable_name_with_pos_list = 421, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 422,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 423, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 424, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 425,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 426,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 427,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_basic_type_declaration = 428, /* bitfield_basic_type_declaration  */
  YYSYMBOL_bitfield_type_declaration = 429, /* bitfield_type_declaration  */
  YYSYMBOL_430_52 = 430,                   /* $@52  */
  YYSYMBOL_431_53 = 431,                   /* $@53  */
  YYSYMBOL_c_or_s = 432,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 433,          /* table_type_pair  */
  YYSYMBOL_dim_list = 434,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 435, /* type_declaration_no_options  */
  YYSYMBOL_optional_expr_list_in_braces = 436, /* optional_expr_list_in_braces  */
  YYSYMBOL_type_declaration_no_options_no_dim = 437, /* type_declaration_no_options_no_dim  */
  YYSYMBOL_438_54 = 438,                   /* $@54  */
  YYSYMBOL_439_55 = 439,                   /* $@55  */
  YYSYMBOL_440_56 = 440,                   /* $@56  */
  YYSYMBOL_441_57 = 441,                   /* $@57  */
  YYSYMBOL_442_58 = 442,                   /* $@58  */
  YYSYMBOL_443_59 = 443,                   /* $@59  */
  YYSYMBOL_444_60 = 444,                   /* $@60  */
  YYSYMBOL_445_61 = 445,                   /* $@61  */
  YYSYMBOL_446_62 = 446,                   /* $@62  */
  YYSYMBOL_447_63 = 447,                   /* $@63  */
  YYSYMBOL_448_64 = 448,                   /* $@64  */
  YYSYMBOL_449_65 = 449,                   /* $@65  */
  YYSYMBOL_450_66 = 450,                   /* $@66  */
  YYSYMBOL_451_67 = 451,                   /* $@67  */
  YYSYMBOL_452_68 = 452,                   /* $@68  */
  YYSYMBOL_453_69 = 453,                   /* $@69  */
  YYSYMBOL_454_70 = 454,                   /* $@70  */
  YYSYMBOL_455_71 = 455,                   /* $@71  */
  YYSYMBOL_456_72 = 456,                   /* $@72  */
  YYSYMBOL_457_73 = 457,                   /* $@73  */
  YYSYMBOL_458_74 = 458,                   /* $@74  */
  YYSYMBOL_459_75 = 459,                   /* $@75  */
  YYSYMBOL_460_76 = 460,                   /* $@76  */
  YYSYMBOL_461_77 = 461,                   /* $@77  */
  YYSYMBOL_462_78 = 462,                   /* $@78  */
  YYSYMBOL_463_79 = 463,                   /* $@79  */
  YYSYMBOL_464_80 = 464,                   /* $@80  */
  YYSYMBOL_465_81 = 465,                   /* $@81  */
  YYSYMBOL_type_declaration = 466,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 467,  /* tuple_alias_declaration  */
  YYSYMBOL_468_82 = 468,                   /* $@82  */
  YYSYMBOL_469_83 = 469,                   /* $@83  */
  YYSYMBOL_470_84 = 470,                   /* $@84  */
  YYSYMBOL_471_85 = 471,                   /* $@85  */
  YYSYMBOL_variant_alias_declaration = 472, /* variant_alias_declaration  */
  YYSYMBOL_473_86 = 473,                   /* $@86  */
  YYSYMBOL_474_87 = 474,                   /* $@87  */
  YYSYMBOL_475_88 = 475,                   /* $@88  */
  YYSYMBOL_476_89 = 476,                   /* $@89  */
  YYSYMBOL_bitfield_alias_declaration = 477, /* bitfield_alias_declaration  */
  YYSYMBOL_478_90 = 478,                   /* $@90  */
  YYSYMBOL_479_91 = 479,                   /* $@91  */
  YYSYMBOL_480_92 = 480,                   /* $@92  */
  YYSYMBOL_481_93 = 481,                   /* $@93  */
  YYSYMBOL_make_decl = 482,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 483,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 484,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 485,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim_list = 486,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 487,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 488, /* optional_make_struct_dim_decl  */
  YYSYMBOL_use_initializer = 489,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 490,         /* make_struct_decl  */
  YYSYMBOL_491_94 = 491,                   /* $@94  */
  YYSYMBOL_492_95 = 492,                   /* $@95  */
  YYSYMBOL_493_96 = 493,                   /* $@96  */
  YYSYMBOL_494_97 = 494,                   /* $@97  */
  YYSYMBOL_495_98 = 495,                   /* $@98  */
  YYSYMBOL_496_99 = 496,                   /* $@99  */
  YYSYMBOL_497_100 = 497,                  /* $@100  */
  YYSYMBOL_498_101 = 498,                  /* $@101  */
  YYSYMBOL_499_102 = 499,                  /* $@102  */
  YYSYMBOL_500_103 = 500,                  /* $@103  */
  YYSYMBOL_make_map_tuple = 501,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 502,          /* make_tuple_call  */
  YYSYMBOL_503_104 = 503,                  /* $@104  */
  YYSYMBOL_504_105 = 504,                  /* $@105  */
  YYSYMBOL_make_dim_decl = 505,            /* make_dim_decl  */
  YYSYMBOL_506_106 = 506,                  /* $@106  */
  YYSYMBOL_507_107 = 507,                  /* $@107  */
  YYSYMBOL_508_108 = 508,                  /* $@108  */
  YYSYMBOL_509_109 = 509,                  /* $@109  */
  YYSYMBOL_510_110 = 510,                  /* $@110  */
  YYSYMBOL_511_111 = 511,                  /* $@111  */
  YYSYMBOL_512_112 = 512,                  /* $@112  */
  YYSYMBOL_513_113 = 513,                  /* $@113  */
  YYSYMBOL_514_114 = 514,                  /* $@114  */
  YYSYMBOL_515_115 = 515,                  /* $@115  */
  YYSYMBOL_expr_map_tuple_list = 516,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 517,          /* make_table_decl  */
  YYSYMBOL_518_116 = 518,                  /* $@116  */
  YYSYMBOL_array_comprehension_where = 519, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 520,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 521       /* array_comprehension  */
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
#define YYLAST   12684

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  208
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  314
/* YYNRULES -- Number of rules.  */
#define YYNRULES  919
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1655

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
       0,   566,   566,   567,   572,   573,   574,   575,   576,   577,
     578,   579,   580,   581,   582,   583,   584,   588,   589,   593,
     594,   598,   604,   605,   606,   610,   611,   615,   616,   620,
     639,   640,   641,   642,   646,   647,   651,   652,   656,   657,
     657,   661,   666,   675,   690,   706,   711,   719,   719,   758,
     772,   776,   779,   783,   789,   798,   801,   807,   808,   812,
     816,   817,   821,   824,   830,   836,   839,   845,   846,   850,
     851,   855,   856,   860,   861,   862,   870,   871,   875,   876,
     882,   883,   884,   885,   886,   890,   891,   895,   896,   900,
     902,   900,   914,   914,   922,   924,   922,   936,   936,   944,
     944,   956,   963,   970,   975,   984,   992,   998,  1006,  1016,
    1016,  1025,  1033,  1033,  1046,  1046,  1058,  1062,  1068,  1069,
    1070,  1071,  1072,  1073,  1077,  1082,  1090,  1091,  1092,  1096,
    1097,  1098,  1099,  1100,  1101,  1102,  1103,  1104,  1110,  1113,
    1119,  1122,  1128,  1129,  1130,  1131,  1135,  1153,  1176,  1179,
    1189,  1204,  1219,  1234,  1237,  1244,  1248,  1255,  1256,  1260,
    1261,  1265,  1266,  1267,  1271,  1274,  1278,  1285,  1289,  1290,
    1291,  1292,  1293,  1294,  1295,  1296,  1297,  1298,  1299,  1300,
    1301,  1302,  1303,  1304,  1305,  1306,  1307,  1308,  1309,  1310,
    1311,  1312,  1313,  1314,  1315,  1316,  1317,  1318,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,  1326,  1327,  1328,  1329,  1330,
    1331,  1332,  1333,  1334,  1335,  1336,  1337,  1338,  1339,  1340,
    1341,  1342,  1343,  1344,  1345,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,  1353,  1354,  1355,  1356,  1357,  1358,  1359,  1360,
    1361,  1362,  1363,  1364,  1365,  1366,  1367,  1368,  1369,  1370,
    1371,  1372,  1373,  1374,  1375,  1376,  1377,  1378,  1379,  1380,
    1381,  1382,  1383,  1384,  1385,  1386,  1387,  1388,  1389,  1390,
    1391,  1392,  1393,  1394,  1395,  1396,  1397,  1401,  1402,  1406,
    1425,  1426,  1427,  1431,  1437,  1437,  1454,  1457,  1459,  1457,
    1467,  1469,  1467,  1484,  1497,  1498,  1499,  1500,  1501,  1502,
    1503,  1504,  1505,  1506,  1507,  1508,  1509,  1510,  1511,  1512,
    1513,  1514,  1515,  1519,  1524,  1530,  1536,  1537,  1541,  1542,
    1546,  1550,  1557,  1558,  1569,  1573,  1576,  1584,  1584,  1584,
    1587,  1593,  1596,  1600,  1604,  1611,  1618,  1624,  1628,  1632,
    1635,  1638,  1646,  1649,  1657,  1663,  1664,  1665,  1669,  1670,
    1674,  1675,  1679,  1684,  1692,  1698,  1710,  1713,  1716,  1722,
    1722,  1722,  1725,  1725,  1725,  1730,  1730,  1730,  1738,  1738,
    1738,  1744,  1754,  1765,  1780,  1783,  1786,  1789,  1795,  1796,
    1803,  1814,  1815,  1816,  1820,  1821,  1822,  1823,  1824,  1828,
    1833,  1841,  1842,  1846,  1853,  1857,  1863,  1864,  1865,  1866,
    1867,  1868,  1869,  1873,  1874,  1875,  1876,  1877,  1878,  1879,
    1880,  1881,  1882,  1883,  1884,  1885,  1886,  1887,  1888,  1889,
    1890,  1891,  1895,  1902,  1914,  1919,  1929,  1933,  1940,  1943,
    1943,  1943,  1948,  1948,  1948,  1961,  1965,  1969,  1974,  1981,
    1990,  1995,  2002,  2002,  2002,  2009,  2013,  2022,  2030,  2038,
    2042,  2045,  2051,  2052,  2053,  2054,  2055,  2056,  2057,  2058,
    2059,  2060,  2061,  2062,  2063,  2064,  2065,  2066,  2067,  2068,
    2069,  2070,  2071,  2072,  2073,  2074,  2075,  2076,  2077,  2078,
    2079,  2080,  2081,  2082,  2083,  2084,  2085,  2086,  2092,  2093,
    2094,  2095,  2096,  2109,  2118,  2119,  2120,  2121,  2122,  2123,
    2124,  2125,  2126,  2127,  2128,  2129,  2130,  2131,  2134,  2134,
    2134,  2137,  2142,  2146,  2150,  2150,  2150,  2155,  2158,  2162,
    2162,  2162,  2167,  2170,  2171,  2172,  2173,  2174,  2175,  2176,
    2177,  2178,  2180,  2184,  2185,  2193,  2196,  2199,  2208,  2209,
    2210,  2211,  2212,  2213,  2214,  2218,  2222,  2226,  2230,  2234,
    2238,  2242,  2246,  2250,  2257,  2258,  2262,  2263,  2264,  2268,
    2269,  2273,  2274,  2275,  2279,  2280,  2284,  2296,  2299,  2300,
    2304,  2304,  2323,  2322,  2337,  2336,  2353,  2365,  2374,  2384,
    2385,  2386,  2387,  2388,  2392,  2395,  2404,  2405,  2409,  2412,
    2416,  2429,  2438,  2439,  2443,  2446,  2450,  2463,  2464,  2468,
    2474,  2480,  2489,  2492,  2499,  2502,  2508,  2509,  2510,  2514,
    2515,  2519,  2526,  2531,  2540,  2546,  2557,  2564,  2573,  2576,
    2579,  2586,  2589,  2594,  2605,  2608,  2613,  2625,  2626,  2630,
    2631,  2632,  2636,  2639,  2642,  2642,  2662,  2665,  2665,  2683,
    2688,  2696,  2697,  2701,  2704,  2717,  2734,  2735,  2736,  2741,
    2741,  2767,  2771,  2772,  2773,  2777,  2787,  2790,  2796,  2797,
    2801,  2802,  2806,  2807,  2811,  2813,  2818,  2811,  2834,  2835,
    2839,  2840,  2844,  2850,  2851,  2852,  2853,  2857,  2858,  2859,
    2863,  2866,  2872,  2874,  2879,  2872,  2900,  2907,  2912,  2921,
    2927,  2938,  2939,  2940,  2941,  2942,  2943,  2944,  2945,  2946,
    2947,  2948,  2949,  2950,  2951,  2952,  2953,  2954,  2955,  2956,
    2957,  2958,  2959,  2960,  2961,  2962,  2963,  2964,  2968,  2969,
    2970,  2971,  2972,  2973,  2974,  2975,  2979,  2990,  2994,  3001,
    3013,  3020,  3026,  3035,  3040,  3050,  3060,  3070,  3083,  3084,
    3085,  3086,  3087,  3091,  3095,  3095,  3095,  3109,  3110,  3114,
    3118,  3125,  3129,  3133,  3137,  3144,  3147,  3166,  3167,  3171,
    3172,  3173,  3174,  3175,  3175,  3175,  3179,  3184,  3191,  3198,
    3198,  3205,  3205,  3212,  3216,  3220,  3225,  3230,  3235,  3240,
    3244,  3248,  3253,  3257,  3261,  3266,  3266,  3266,  3272,  3279,
    3279,  3279,  3284,  3284,  3284,  3290,  3290,  3290,  3295,  3300,
    3300,  3300,  3305,  3305,  3305,  3314,  3319,  3319,  3319,  3324,
    3324,  3324,  3333,  3338,  3338,  3338,  3343,  3343,  3343,  3352,
    3352,  3352,  3358,  3358,  3358,  3367,  3370,  3381,  3397,  3399,
    3404,  3409,  3397,  3435,  3437,  3442,  3448,  3435,  3474,  3476,
    3481,  3486,  3474,  3527,  3528,  3529,  3530,  3531,  3535,  3542,
    3549,  3555,  3561,  3568,  3575,  3581,  3590,  3593,  3599,  3607,
    3612,  3619,  3624,  3630,  3631,  3635,  3636,  3640,  3640,  3640,
    3648,  3648,  3648,  3655,  3655,  3655,  3666,  3666,  3666,  3673,
    3673,  3673,  3684,  3690,  3696,  3702,  3702,  3702,  3716,  3735,
    3735,  3735,  3745,  3745,  3745,  3759,  3759,  3759,  3773,  3782,
    3782,  3782,  3802,  3809,  3809,  3809,  3819,  3822,  3828,  3828,
    3850,  3858,  3878,  3903,  3904,  3908,  3909,  3913,  3916,  3919
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
  "require_module_name", "require_module", "is_public_module",
  "expect_declaration", "expect_list", "expect_error", "expression_label",
  "expression_goto", "elif_or_static_elif", "emit_semis",
  "optional_emit_semis", "expression_else", "if_or_static_if",
  "expression_else_one_liner", "expression_if_one_liner", "semis",
  "optional_semis", "expression_if_block", "$@3", "$@4", "$@5",
  "expression_else_block", "$@6", "$@7", "$@8", "expression_if_then_else",
  "$@9", "expression_if_then_else_oneliner",
  "for_variable_name_with_pos_list", "expression_for_loop", "$@10",
  "expression_unsafe", "expression_while_loop", "$@11", "expression_with",
  "$@12", "expression_with_alias", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "metadata_argument_list", "annotation_declaration_name",
  "annotation_declaration_basic", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_annotation_list_with_emit_semis",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "optional_template", "global_function_declaration",
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
  "bitfield_alias_bits", "bitfield_basic_type_declaration",
  "bitfield_type_declaration", "$@52", "$@53", "c_or_s", "table_type_pair",
  "dim_list", "type_declaration_no_options",
  "optional_expr_list_in_braces", "type_declaration_no_options_no_dim",
  "$@54", "$@55", "$@56", "$@57", "$@58", "$@59", "$@60", "$@61", "$@62",
  "$@63", "$@64", "$@65", "$@66", "$@67", "$@68", "$@69", "$@70", "$@71",
  "$@72", "$@73", "$@74", "$@75", "$@76", "$@77", "$@78", "$@79", "$@80",
  "$@81", "type_declaration", "tuple_alias_declaration", "$@82", "$@83",
  "$@84", "$@85", "variant_alias_declaration", "$@86", "$@87", "$@88",
  "$@89", "bitfield_alias_declaration", "$@90", "$@91", "$@92", "$@93",
  "make_decl", "make_struct_fields", "make_variant_dim",
  "make_struct_single", "make_struct_dim_list", "make_struct_dim_decl",
  "optional_make_struct_dim_decl", "use_initializer", "make_struct_decl",
  "$@94", "$@95", "$@96", "$@97", "$@98", "$@99", "$@100", "$@101",
  "$@102", "$@103", "make_map_tuple", "make_tuple_call", "$@104", "$@105",
  "make_dim_decl", "$@106", "$@107", "$@108", "$@109", "$@110", "$@111",
  "$@112", "$@113", "$@114", "$@115", "expr_map_tuple_list",
  "make_table_decl", "$@116", "array_comprehension_where",
  "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1475)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-817)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1475,    32, -1475, -1475,    48,   -54,   160,   566, -1475,    91,
   -1475, -1475, -1475, -1475,   187,   708, -1475, -1475, -1475, -1475,
     106,   106,   106, -1475,   356, -1475,   100, -1475, -1475, -1475,
   -1475, -1475, -1475, -1475, -1475, -1475, -1475,     9, -1475,    70,
      75,   133, -1475, -1475,   160,    22, -1475, -1475, -1475,    98,
     106, -1475, -1475,   100,   566,   566,   566,   156,   210, -1475,
   -1475, -1475, -1475,   708,   708,   708,   239, -1475,   677,   332,
   -1475, -1475, -1475, -1475,   286, -1475,   524, -1475,   621,    78,
      48,   288,   -54,   264,   306, -1475,   416,   468, -1475, -1475,
   -1475,   633,   470,   492,   511, -1475,   516,   346, -1475, -1475,
     -12,    48,   708,   708,   708,   708,   327, -1475,   709,   718,
     486,   593,   719, -1475, -1475,   497, -1475, -1475, -1475, -1475,
   -1475,   831,   422,   504, -1475, -1475, -1475, -1475,   657, -1475,
   -1475,   553, -1475, -1475,   540,   577,   327,   327, -1475, -1475,
     589, -1475,   251, -1475,   405,   618,   677, -1475,   612, -1475,
   -1475, -1475, -1475, -1475, -1475, -1475,   644, -1475, -1475, -1475,
   -1475, -1475, -1475,   629, -1475, -1475, -1475,   844, -1475, -1475,
   -1475, -1475, -1475, -1475, -1475,   272,   667, -1475,  1690,   772,
   -1475,   564,   620, -1475, -1475, -1475, -1475, -1475, 11569, -1475,
     659,   740,   -49,    48,   655,   698, -1475, -1475, -1475,   422,
   -1475, -1475,   690,   700,   705,   688,   715,   722, -1475, -1475,
   -1475,   704, -1475, -1475, -1475, -1475, -1475,   -93, -1475, -1475,
   -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,   726,
   -1475, -1475, -1475,   729,   737, -1475, -1475, -1475, -1475,   738,
     742,   725,   187,   278, -1475, -1475, -1475, -1475,    45,   684,
     746, -1475, -1475, -1475, -1475, -1475, -1475,   764, -1475,   727,
     733, 11681, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,
   -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,
   -1475, -1475, -1475, -1475, -1475, -1475, -1475,   914,   919, -1475,
     739,   327,   979,   620, -1475,   792,   327, -1475, -1475,   629,
     327,    48, -1475,   558, -1475, -1475, -1475, -1475, -1475,  7978,
   -1475, -1475,   794,   786,   -73,   -67,   -61, -1475, -1475,  7978,
     311, -1475,  5686, -1475, -1475, -1475,     5, -1475, -1475, -1475,
      77, -1475,  5877,   765,  1050, -1475,   763, -1475, -1475,   809,
     812, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,
   -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,
   -1475,   813,   780, -1475, -1475, -1475, -1475, -1475, -1475, -1475,
   -1475, -1475, -1475, -1475, -1475,   969, -1475, -1475, -1475, -1475,
   -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,   830,   796,
   -1475, -1475,   -47,   -80, -1475, -1475, -1475, -1475, -1475, -1475,
   -1475, -1475, -1475, -1475,   791,   825, -1475,   -63, -1475,   327,
     840,  1690, -1475,   200,  1690,  1690,  1690,   826,   827, -1475,
   -1475,    92,   187,   834,    35, -1475,   368,   815,   838,   839,
     843,   847,   845,   377,   850, -1475,   415,    26,   864,  7978,
    7978,   846,   856,   857,   860,   863,   865, -1475, -1475, -1475,
   -1475, -1475, -1475, -1475, -1475, -1475,  7978,  7978,  7978,  7978,
    7978,  4158,  4540, -1475,   867,  1034, -1475, -1475, -1475,   868,
   -1475, -1475, -1475, -1475,   869, -1475, -1475, -1475, -1475, -1475,
   -1475,   607,  8151, -1475, -1475,   876, -1475, -1475, -1475, -1475,
   -1475, -1475,  1690,  1690,   881,   902,  1690,   739,  1690,   739,
    1690,   739,  2371,   921,  8237, -1475,  5686, -1475,  1690,  7978,
     884,   913, 12013, -1475, -1475, -1475, -1475,   889, -1475, -1475,
   11161,  6068, -1475,    45, -1475,  2371,   921, -1475, -1475, -1475,
   -1475,   929,  1161,  1798,   895, -1475,    63,   892,    60,   897,
    1690,  1690, -1475, -1475,   898, -1475,   187, -1475,   588,   900,
    1061,   581, -1475, -1475, -1475,   302, -1475, -1475, -1475,  7978,
     127,   259,   924,   545, -1475, -1475, -1475, -1475,   906, -1475,
   -1475,   430, -1475,   927,   928,   930, -1475,  5686,  1690,  7978,
    7978, -1475, -1475,  7978, -1475,  7978, -1475,  5686, -1475, -1475,
    5686,   931, -1475,  1690,   541,   541,  7978,  7978,  7978,  7978,
    7978,  7978,   776,   541,   541,   162,   541,   541,   909,  1099,
     955,   920,   155,   913,   940,   954,   472,   956,   327,  3585,
     708,  1154,   960, -1475,   869, -1475,  2220,  8140,  7978,  7978,
   -1475, -1475,  7978,  7978,  7978,  7978,   996,  7978,   380,  7978,
    7978,  7978,  7978,  7978,  7978,  7978,  7978,  7978,  6259,  7978,
    7978,  7978,  7978,  7978,  7978,  7978,  7978,  7978,  7978, 12486,
    7978, -1475,  4731,   627,   656, -1475, -1475,  1001,   664,   -80,
     682,   -80,   683,   -80,   111, -1475,   223,   746,   992, -1475,
     560, -1475,  1690,   913,   658,   746, 12013, -1475,  6450, -1475,
   -1475, -1475, -1475, 11245,  1004, -1475,   106, -1475,   106, -1475,
   -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,
   -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,
   -1475, -1475, -1475, -1475, -1475, -1475,  7978, -1475, -1475,    18,
     -78,   -78,   -78, -1475,   746,   746, -1475,  1008, -1475, -1475,
   -1475, -1475,  7978,  1011,  1012,  1690,   200, -1475,  7978,  1833,
   -1475, -1475, -1475, -1475, -1475, -1475, -1475,  1690,  1690,  1690,
    1690,  4349,  1013,  7978,  1690, -1475, -1475, -1475,  1690,   913,
     783, 11764, -1475,  1005,  8323,  1690,  1690,  8409,  1690,  8495,
    1690,   913,  1690,  2371,   913, -1475,   921,    88,  8581,  8667,
    8753,  8839,  8925,  9011, -1475,  7978,   732,    65,   981, -1475,
    7978, -1475,  7978, -1475,  7978,   978,   523, -1475, -1475,   982,
     983,   -57, -1475, -1475,    65,  7978,   221,  3776, -1475,   274,
     984,   362,   987,   739, -1475,  2559,  1154,  1006,   988, -1475,
   -1475,  1010,   993, -1475, -1475,   389,   389,   183,   183, 12358,
   12358,   994,   519,   995, -1475, 11329,   150,   150,   876,   389,
     389,   671, 12128, 12211, 12096, 11836, 11847,  2225, 12243, 12326,
     183,   183,   852,   852,   519,   519,   519,   550,  7978,   997,
     999,   565,  7978,  1190,  1000, 11413, -1475,   281, -1475, -1475,
   -1475,   -97, -1475,  1015, -1475,  1020, -1475,  1022,  1690, -1475,
    2371,  1690, -1475,   921,   661,  1003,  1007,  1690,  7978, 12013,
   -1475,  1018,   586, -1475,  1262, -1475,  -105, -1475,  9097,  1009,
    1162, -1475, -1475,   363, -1475, -1475, -1475,  2765,  1032, -1475,
     586,    34,  9183, -1475,  1167,   302,  7978,  1833, -1475, -1475,
   -1475, -1475,   746,   318,   880,   702,   598,   287,  1017,  1024,
     756,  9269,   712,  1690,  2371,   921,   991,  1026,  1019,  1690,
    7978,  7978,  1027, -1475,  1033,  1041, -1475,  1056, -1475,  1253,
    1028,  1258,   769,  1029,  1690,   788,  1154, -1475, -1475, -1475,
   -1475, -1475,  1021, 11930,  1036,  1168,  1071,    23,    65,  9355,
   12013, 12013, -1475,  1039,   243,  7978,  7978,  1690,   739,    27,
    1035,  1005,   267, -1475,  1043,   418,  6641, -1475, -1475, -1475,
      13,   -80, -1475,  6832, -1475, -1475,  7023,  1079,  1082, -1475,
     106,  1091,  7214,   206,  7405, -1475, -1475,   106,   106,  1239,
   -1475,   904, -1475, -1475,  1237, -1475, -1475,  1242, -1475,  1212,
     106, -1475,   106,   106,   106,   106,   106, -1475,  1189, -1475,
     106,  2086,   739, -1475,  7978, -1475,  7978,  4922,  7978, -1475,
    1076,  1058, -1475, -1475,  7978,  1059, -1475, 11497,  7978,  5113,
    1060, -1475, 11581, -1475,  5304, -1475,  6450, -1475, -1475, -1475,
    1098, -1475,  1101, -1475, -1475, -1475, -1475, -1475, -1475,   746,
   -1475, -1475,   746, -1475, -1475,  1007, -1475, -1475,   746, 12013,
    7978, -1475,   493, -1475, -1475, -1475,  1072, -1475,  1073, -1475,
    7978,  1102,  1104,  1690, -1475,  7978,  1074,  7978,   548, -1475,
    1114, -1475, -1475,  1254,   629, -1475,  1120, -1475,  7978,  1833,
   -1475, -1475, -1475, -1475,  1084, -1475, -1475, -1475,  1088,  1126,
   -1475, -1475,  1347,   821,   824, -1475, -1475,  7978,  1421, 12013,
   -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,
    1477, -1475,   249,  5495, -1475,  1119,  7978,  1129, -1475,   293,
    5686,   190,    36,   285,  7978,  7978,  7978,  9441,  9527,  1482,
     -80,  5686, -1475, -1475, -1475,   523,  1094,  3776,  1133,  1134,
    1100,  1135,  1136, -1475,   295,   327,  7978, 12013,  1287,  7978,
   12013,  1128,  1138, -1475,  1131,  1153, 12013, -1475,  7978, 12013,
   -1475, -1475,  1110, -1475, -1475,  1115,  1116,  1117,  1121, -1475,
   -1475, -1475, -1475, -1475, -1475, -1475,   265, -1475,  7978,  7978,
    7978,  7978,  7978,  7978,  7978,  7978,  7978,  7978,  7978,  7978,
    7978,  7978,  7978,  7978,  7978,  7978,   -80,  1690,  9613,  1690,
    9699, -1475,   301,  9785, -1475,  7978,  1696,  7978, -1475,  9871,
    3776, -1475,   325,  7978, -1475, -1475, -1475,   331, -1475, -1475,
   -1475, -1475, -1475, -1475, -1475, 12013,  1146, -1475,  1122, -1475,
   -1475,  9957, -1475,  1279,   209, 12013,  1286, 12013, -1475,  1123,
    1151,   723,  1260,   106, -1475,  1833, -1475,  1130,  1166, -1475,
   -1475,  7978,  1150, -1475, -1475, -1475, -1475,  1169,  1170,  1175,
    7978,  7978,  7978,  1183,  1297,  1184, -1475,  1185,  7596, -1475,
   -1475,   337, -1475, -1475, 10043, -1475,  1205, -1475,   350,  1327,
    1071,  5686,  7978,  7978, 10129, 12013, 12013, -1475, -1475, -1475,
    1206,   383, -1475,   443, -1475, -1475,  1226, -1475, -1475,    13,
   -1475,  1257, 12013, -1475, 12013,  1690,  7978, -1475, -1475, -1475,
   12013,  7978,  7978,    65,  7978,  7978,  1071, -1475, -1475, -1475,
   12013, 12013, 12013, 12013, 12013, 12013, 12013, 12013, 12013, 12013,
   12013, 12013, 12013, 12013, 12013, 12013, 12013, 12013,   327,  1492,
   -1475,   713, -1475, -1475, -1475,  1690, 10215, 10301, -1475,   454,
   -1475, 10387, -1475,  7978, -1475, -1475,  1233,  7978, -1475, -1475,
   -1475,  1690, -1475, -1475,   579, -1475,    25, -1475, -1475,  1297,
    1297, 10473,  1197,  1198,  1199,  1200,  5686, -1475,  7978,   183,
     183,   183,  5686, -1475, -1475,  1297,  1202,  1297, -1475, 10559,
   -1475, -1475,  1236, -1475, -1475,  1201,  1241,   408,   431, 12013,
   12013,   329,   407, -1475,  1207,  1208,  1209, -1475,  7787, -1475,
   -1475, -1475,   746, 12013, 10645, 10731,    37, 10817, 10903,   433,
     193, -1475, -1475, -1475,   720, -1475, -1475,  1211, -1475, 12013,
   -1475, 12013,  1210,   -24,  1373,    25, -1475, -1475,   723,   142,
     142, -1475,  7978,  1297,  1297,   598,  1215,  1216,   913,   142,
    1297,   598, -1475, -1475,  7978, -1475, -1475,  1214,  7978,  7978,
   -1475,   407,  7978, -1475, -1475,  7978, 12096,  1401,   327,  5686,
     327,   327,    -4, -1475, -1475, -1475, -1475, -1475, -1475, -1475,
   -1475, -1475, -1475, -1475, -1475,  1373,    18,   598,  1246,  1247,
   -1475,  1220,  1221, 10989,   142,   142,  1246,  1222, -1475, -1475,
    1223,  1225,   598,  1229,  8068,  7978, 12013, 12013, -1475,  1227,
    1696,  3967,   106,  1231,   435, -1475, -1475,  1690,   200, -1475,
    2971, 11569, -1475, -1475, -1475, -1475,   439,  1238, -1475, -1475,
   -1475, -1475,  1240,  1243, -1475, -1475, -1475,  1244, -1475,  1389,
    1248,  8068,  1245, -1475, -1475, -1475, -1475, -1475, 12013, -1475,
     652,  1251,  3967,   327, -1475, -1475,   561,  7978,  1252,   106,
   11569, -1475,   598, -1475, -1475, -1475,  7978, -1475,  1249, -1475,
     327, -1475, -1475,  1250, -1475, -1475,   106, -1475,  7978,  1833,
   -1475, -1475,   327,   440, 12013, -1475,  1263,  7978,  3177, -1475,
   -1475,  1833, -1475, -1475, -1475, -1475,  1264,  3967, 11075,  1256,
   -1475, -1475, -1475,   106,   327,  1286,  3383, -1475,  1263, -1475,
    1261,   652,  1286, -1475, -1475
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   159,     1,   348,     0,     0,     0,   646,   349,     0,
     838,   828,   833,    20,     0,     0,    19,    16,    15,     3,
       0,     0,     0,     8,   682,     7,   627,     6,    11,     5,
       4,    13,    12,    14,   127,   128,   126,   136,   138,    49,
      62,    59,    60,    51,     0,    57,    50,   648,   647,     0,
       0,    26,    25,   627,   646,   646,   646,     0,   322,    47,
     143,   144,   145,     0,     0,     0,   146,   148,   155,     0,
     142,    21,    10,     9,   277,   664,     0,   628,   629,     0,
       0,     0,     0,    52,     0,    58,     0,     0,    55,   649,
     651,    22,     0,     0,     0,   324,     0,     0,   154,   149,
       0,     0,     0,     0,     0,     0,    71,   278,   280,   652,
     674,   673,   677,   631,   630,   637,   134,   135,   132,   133,
     130,     0,     0,     0,   129,   139,    63,    61,    57,    54,
      53,     0,    23,    24,    27,   738,    71,    71,   323,    45,
      48,   153,     0,   150,   151,   152,   156,    69,    72,   160,
     282,   281,   284,   279,   654,   653,     0,   676,   675,   679,
     678,   683,   632,   554,    30,    31,    35,     0,   122,   123,
     120,   121,   119,   118,   124,     0,     0,    56,     0,     0,
      29,     0,   662,   829,   834,    46,   147,    70,     0,   655,
     656,   670,   634,     0,   555,     0,    32,    33,    34,     0,
     137,   131,     0,     0,     0,     0,     0,     0,   691,   711,
     692,   727,   693,   697,   698,   699,   700,   717,   704,   705,
     706,   707,   708,   709,   710,   712,   713,   714,   715,   798,
     696,   703,   716,   805,   812,   694,   701,   695,   702,     0,
       0,     0,     0,   726,   759,   762,   760,   761,   825,   755,
     650,    28,   741,   742,   739,   740,   660,   663,   839,     0,
       0,     0,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,     0,     0,   167,
     161,    71,     0,   662,   671,     0,    71,   636,   633,   554,
      71,     0,   616,   609,   638,   125,   763,   789,   792,     0,
     795,   785,     0,     0,   799,   806,   813,   819,   822,     0,
     757,   769,   316,   775,   780,   774,     0,   788,   784,   777,
       0,   779,     0,   756,     0,   661,     0,   830,   835,   246,
     247,   244,   170,   171,   173,   172,   174,   175,   176,   177,
     203,   204,   201,   202,   194,   205,   206,   195,   192,   193,
     245,   228,     0,   243,   207,   208,   209,   210,   181,   182,
     183,   178,   179,   180,   191,     0,   197,   198,   196,   189,
     190,   185,   184,   186,   187,   188,   169,   168,   227,     0,
     199,   200,   554,   164,   290,   718,   721,   724,   725,   719,
     722,   720,   723,   657,     0,   668,   684,     0,   140,    71,
       0,     0,   610,     0,     0,     0,     0,     0,     0,   458,
     459,     0,     0,     0,     0,   452,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   717,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   543,   396,   398,
     397,   399,   400,   401,   402,    41,     0,     0,     0,     0,
       0,   316,     0,   381,   382,   908,   456,   455,   533,   453,
     526,   525,   524,   523,   157,   529,   454,   528,   527,   500,
     460,   501,     0,   505,   461,     0,   457,   843,   847,   844,
     845,   846,     0,     0,     0,   744,     0,   161,     0,   161,
       0,   161,     0,     0,     0,   771,     0,   768,     0,     0,
       0,   915,   374,   782,   783,   776,   778,     0,   781,   752,
       0,     0,   827,   826,   840,   588,   594,   248,   249,   242,
     226,   250,   229,   211,     0,   162,   347,   579,   580,     0,
       0,     0,   283,   285,     0,   665,     0,   672,     0,     0,
     611,   609,   635,   141,   617,     0,   607,   608,   606,     0,
       0,     0,     0,   749,   867,   870,   327,   726,   331,   330,
     336,     0,   368,     0,     0,     0,   899,     0,     0,     0,
       0,   359,   362,     0,   365,     0,   903,     0,   879,   885,
       0,     0,   873,     0,   488,   489,     0,     0,     0,     0,
       0,     0,     0,   465,   464,   502,   463,   462,     0,     0,
       0,     0,   322,   915,   915,     0,   383,     0,    71,     0,
       0,   391,   382,   313,   157,   293,     0,     0,     0,     0,
     490,   491,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   442,
       0,   766,     0,     0,     0,   728,   743,     0,     0,   164,
       0,   164,     0,   164,   322,   586,     0,   584,     0,   592,
       0,   729,     0,   915,     0,   320,   375,   767,   916,   317,
     773,   751,   754,     0,   733,   589,    87,   595,    87,   251,
     231,   232,   234,   233,   235,   236,   237,   238,   230,   239,
     240,   241,   215,   216,   218,   217,   219,   220,   221,   222,
     213,   214,   223,   224,   225,   212,     0,   345,   346,     0,
     554,   554,   554,   163,   166,   165,   313,   643,   669,   680,
     567,   685,     0,     0,     0,     0,     0,   624,     0,     0,
     764,   790,   793,    18,    17,   747,   748,     0,     0,     0,
       0,   865,     0,     0,     0,   889,   892,   895,     0,   915,
       0,   883,   906,   915,     0,     0,     0,     0,     0,     0,
       0,   915,     0,     0,   915,   876,     0,     0,     0,     0,
       0,     0,     0,     0,    44,     0,    42,     0,     0,   888,
       0,   598,     0,   597,     0,     0,   916,   858,   493,     0,
       0,   429,   426,   428,     0,   318,     0,   316,   445,     0,
       0,     0,     0,   161,   383,     0,   391,     0,     0,   512,
     511,     0,     0,   513,   517,   466,   467,   479,   480,   477,
     478,     0,   506,     0,   498,     0,   530,   531,   532,   468,
     469,   484,   485,   486,   487,     0,     0,   482,   483,   481,
     475,   476,   471,   470,   472,   473,   474,     0,     0,     0,
     435,     0,     0,     0,     0,     0,   450,     0,   796,   786,
     730,     0,   800,     0,   807,     0,   814,     0,     0,   820,
       0,     0,   823,     0,     0,     0,   757,     0,     0,   376,
     753,   734,   658,    85,    88,   831,    88,   836,     0,     0,
     686,   576,   577,   599,   581,   583,   582,     0,   639,   644,
     658,   570,     0,   613,   614,     0,     0,     0,   626,   765,
     791,   794,   750,     0,     0,     0,   866,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   916,     0,   503,     0,     0,   504,     0,   534,     0,
       0,     0,     0,     0,     0,     0,   391,   538,   539,   540,
     541,   542,     0,    38,     0,   102,     0,     0,     0,     0,
     849,   848,   492,     0,     0,     0,     0,     0,   161,     0,
       0,   915,     0,   446,     0,     0,     0,   449,   447,   158,
       0,   164,   315,   339,   337,   290,     0,     0,     0,   338,
       0,     0,     0,    71,     0,   395,   294,     0,     0,     0,
     307,     0,   308,   302,     0,   299,   298,     0,   300,     0,
       0,   314,     0,    83,    84,    81,    82,   309,   351,   297,
       0,   403,   161,   508,     0,   514,     0,     0,     0,   496,
       0,     0,   518,   522,     0,     0,   499,     0,     0,     0,
       0,   436,     0,   443,     0,   494,     0,   451,   797,   787,
       0,   745,     0,   801,   803,   808,   810,   815,   817,   585,
     821,   587,   591,   824,   593,   757,   758,   770,   321,   377,
       0,   641,   659,   841,    86,   590,     0,   596,     0,   578,
       0,     0,     0,     0,   600,     0,     0,     0,   659,   666,
       0,   568,   681,     0,   554,   612,     0,   621,     0,     0,
     625,   868,   871,   328,     0,   333,   334,   332,     0,     0,
     371,   369,     0,     0,     0,   900,   898,   318,     0,   882,
     907,   910,   360,   363,   366,   904,   902,   880,   886,   884,
       0,   874,    71,     0,    39,     0,     0,     0,   352,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     164,     0,   909,   319,   448,     0,     0,   316,     0,     0,
       0,     0,     0,   389,     0,    71,     0,   340,     0,     0,
     325,     0,     0,   312,     0,     0,    66,   290,     0,   342,
     310,   311,     0,    76,    77,     0,     0,     0,     0,   301,
     296,   303,   304,   305,   306,   350,     0,   295,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   164,     0,     0,     0,
       0,   424,     0,     0,   519,     0,   507,     0,   497,     0,
     316,   437,     0,     0,   495,   444,   440,     0,   732,   746,
     731,   804,   811,   818,   772,   735,   736,   642,     0,   832,
     837,     0,   688,   689,   602,   601,   286,   640,   645,     0,
       0,   561,   564,     0,   615,     0,   623,     0,     0,   329,
     335,     0,     0,   370,   890,   893,   896,     0,     0,     0,
       0,     0,     0,     0,   865,     0,   877,     0,     0,   290,
     544,     0,    36,    43,     0,   104,     0,   105,     0,   106,
       0,     0,     0,     0,     0,   851,   850,   427,   553,   430,
       0,     0,   422,     0,   386,   387,     0,   385,   384,     0,
     392,   290,   341,   290,   326,     0,     0,    64,    65,   111,
     343,     0,     0,     0,     0,     0,     0,   618,   357,   356,
     413,   414,   416,   415,   417,   407,   408,   409,   418,   419,
     405,   406,   420,   421,   410,   411,   412,   404,    71,     0,
     552,     0,   550,   425,   547,     0,     0,     0,   546,     0,
     438,     0,   441,     0,   842,   687,     0,     0,   287,   292,
     667,     0,   562,   563,   564,   565,   556,   571,   622,   865,
     865,     0,     0,     0,     0,     0,   316,   911,   318,   361,
     364,   367,     0,   866,   881,   865,     0,   865,   535,     0,
     537,   545,    40,   103,   353,     0,     0,     0,     0,   853,
     852,     0,     0,   433,     0,     0,     0,   390,     0,   378,
     393,   344,   117,   116,     0,     0,     0,     0,     0,     0,
       0,   290,   509,   515,     0,   551,   549,     0,   548,   737,
     690,   603,     0,     0,   559,   556,   557,   558,   561,   864,
     864,   372,     0,   865,   865,   856,     0,     0,   915,   864,
     865,   856,   536,    37,     0,   107,   108,     0,     0,     0,
     431,     0,     0,   423,   388,     0,   379,    78,    71,     0,
      71,    71,   609,   358,   619,   620,   394,   510,   516,   520,
     439,   313,   569,   560,   572,   559,     0,     0,   861,   915,
     863,     0,     0,     0,   864,   864,   857,     0,   901,   912,
       0,     0,   856,     0,   913,     0,   855,   854,   434,   913,
     380,     0,     0,    92,     0,   290,   290,     0,     0,   521,
       0,     0,   574,   605,   604,   566,     0,   916,   862,   869,
     872,   373,     0,     0,   897,   905,   887,     0,   875,     0,
       0,   913,     0,    79,    83,    84,    81,    82,    80,   101,
      73,     0,     0,    71,   113,   115,     0,     0,     0,     0,
       0,   859,     0,   891,   894,   878,     0,   917,     0,   919,
      71,    67,    68,     0,   100,   313,     0,   290,     0,     0,
     289,   573,    71,     0,   914,   918,    97,     0,     0,    93,
     110,     0,   355,   290,   860,    74,     0,     0,     0,     0,
     354,   575,   313,     0,    71,   286,     0,    98,    97,    91,
       0,    73,   286,    75,    96
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1475, -1475,  -866,    -1, -1475, -1475, -1475, -1475, -1475,   851,
    1367, -1475, -1475, -1475, -1475, -1475, -1475,  1448, -1475, -1475,
   -1475,  1408, -1475,  1329, -1475, -1475,  1381, -1475, -1475, -1475,
   -1475,  -132,  -181, -1475, -1475, -1474,   775,   778, -1475, -1475,
   -1475, -1475,  -174, -1475, -1475, -1475, -1475, -1475, -1475,  -800,
   -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,  1278, -1475,
   -1475,   -43,  1378, -1475, -1475, -1475,   495,   866,   859,   563,
    -466,  -646, -1475, -1475, -1475, -1475, -1278, -1475, -1475,  -991,
   -1475, -1475,  -989, -1475, -1475, -1475, -1475,  -723,  -319, -1115,
     803,   -13, -1475, -1475, -1475, -1475, -1475, -1415, -1399, -1330,
   -1306, -1475, -1475,  1486, -1475, -1071, -1475, -1475, -1475, -1475,
   -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,  -445,
   -1475,  1014,   152, -1475,  -778, -1475, -1475, -1475, -1475, -1475,
   -1475, -1379, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,
   -1475,   934, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,
    -155,    14,   -33,    16,    93, -1475, -1475, -1475, -1475, -1475,
   -1475, -1475,   291,  -499,  -742, -1475,  -511,  -731, -1475,  -889,
     -30,   -28, -1475,  -548,  -545, -1475, -1475, -1475, -1187, -1475,
    1446, -1475, -1475, -1475, -1475, -1475,   392,   582, -1475,   986,
   -1475, -1475, -1475, -1475, -1475, -1475,   583, -1475,  1213, -1475,
   -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,
   -1475, -1475, -1475, -1475,   -77, -1475,  1080, -1475, -1475, -1475,
    1288, -1475, -1475, -1475,  -533, -1475, -1475,  -325,  -862, -1475,
   -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,
   -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,
   -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,  -168, -1475,
   -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,
   -1475, -1475, -1475, -1475,  1083,  -706, -1416,  -591, -1475, -1475,
   -1191, -1224, -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,
   -1475, -1475, -1475,  -933, -1475, -1475, -1475, -1475, -1475, -1475,
   -1475, -1475, -1475, -1475, -1475, -1475, -1475, -1475,   932, -1475,
   -1475, -1391,  -602, -1475
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   755,   756,    18,   134,    53,   180,    19,   167,
     173,  1432,  1155,  1312,   602,   466,   140,   467,    97,    21,
      22,    45,    46,    88,    23,    41,    42,  1017,  1018,  1613,
     148,   149,  1614,  1205,  1552,  1019,   904,   905,  1590,  1591,
    1639,  1592,  1635,  1636,  1650,  1637,  1020,  1021,  1022,   977,
    1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,   174,   175,
      37,    38,    39,   194,    66,    67,    68,    69,   621,    24,
     393,   542,   290,   108,    25,   152,   291,   153,   188,  1399,
    1472,  1598,   543,   544,  1106,   468,  1031,   825,   610,   990,
     684,   469,  1032,   568,   760,  1289,   470,  1033,  1034,  1035,
    1036,  1037,   729,  1038,  1216,  1159,  1358,  1039,   471,   775,
    1300,   776,  1301,   778,  1302,   472,   764,  1293,   473,   511,
    1450,   474,  1183,  1184,   823,   475,   625,   476,  1040,   477,
     478,   813,   479,   987,  1442,   988,  1501,   480,   873,  1255,
     481,   512,  1237,  1517,  1239,  1518,  1385,  1559,   483,   484,
     536,  1478,  1524,  1404,  1406,  1283,   921,  1114,  1561,  1600,
     537,   538,   539,   675,   676,   696,   679,   680,   698,   804,
     911,   912,  1565,   559,   413,   551,   303,  1460,   552,   304,
      78,   115,   192,   299,    27,   163,   919,  1092,   920,    49,
      50,   131,    28,   156,   190,   293,  1093,   257,   258,    29,
     109,   737,  1279,   547,   295,   296,   112,   161,   741,    30,
      76,   191,   548,   913,   485,   403,   245,   246,   881,   902,
     182,   247,   667,  1259,   890,   562,   333,   248,   507,   249,
     414,   929,   508,   682,   493,  1069,   415,   930,   416,   931,
     492,  1068,   496,  1073,   497,  1261,   498,  1075,   499,  1262,
     500,  1077,   501,  1263,   502,  1080,   503,  1083,   677,    31,
      55,   259,   525,  1096,    32,    56,   260,   526,  1098,    33,
      54,   336,   694,  1268,   486,   614,  1537,   615,  1529,  1530,
    1531,   939,   487,   758,  1287,   759,  1288,   786,  1307,   964,
    1426,   782,  1304,   772,   488,   783,  1305,   489,   943,  1413,
     944,  1414,   945,  1415,   768,  1297,   780,  1303,   991,   490,
     618,  1580,   689,   491
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,    59,    70,   510,   183,   184,   746,   748,   195,   523,
     250,   805,   807,   917,   989,   697,  1188,   613,  1140,    71,
      72,    73,  1298,   883,  1105,   885,   695,   887,   820,  1359,
     757,   669,     2,   671,  1087,   673,  1091,   125,   513,     3,
     573,   962,  -159,  1160,    84,   591,   540,  1171,  1042,    90,
      70,    70,    70,  -802,  1091,   965,  1321,  1509,   678,  -809,
      13,   683,     4,  1500,     5,  -816,     6,    34,    35,  -432,
     727,  1476,     7,   534,  1110,  1543,  1070,  1583,   323,    85,
    1424,   895,     8,   181,  1071,   549,   555,  -738,     9,    70,
      70,    70,    70,   116,   117,  1166,   541,   417,   418,    16,
     550,   244,    40,  -802,   534,   324,   325,  1072,  -802,  -809,
     515,   995,    10,   728,  -809,  -816,    13,   424,  1616,  -432,
    -816,   323,  1548,   426,  -432,   193,  1577,  -802,   102,   103,
     104,  1477,   769,  -809,    11,    12,  1584,   574,   575,  -816,
    1178,    13,   781,  -432,   407,   784,  1179,   893,   324,   325,
     300,   897,  1585,   334,   535,    16,   193,   297,  1582,   394,
     433,   434,    77,  1643,   406,   243,   909,   947,   408,   326,
     938,   952,  1557,   327,   819,   412,  1180,  1584,  1162,   960,
      16,   910,   963,    79,   514,  1479,  1480,  1181,  1152,   141,
    1608,   298,  1182,  1585,   436,   437,  1161,    13,   926,  1111,
    1161,  1489,  1133,  1491,   626,   627,   592,    86,  1349,  1161,
    1161,    36,   326,   974,  1134,   576,   327,   877,    14,    87,
     328,  1586,  1584,  1264,   329,   994,  1267,   330,   975,   320,
      15,    57,    15,   731,   118,   577,    16,   949,  1585,   119,
    1112,   120,  1267,    80,   121,  1587,   560,   561,   563,  1437,
      96,    81,   331,   770,    51,    58,   516,   244,   409,  1534,
    1535,    89,  1586,   328,   732,   976,  1542,   329,   787,   966,
     330,    13,   566,  1515,  1323,   517,   636,   553,   122,   638,
     639,   123,   801,  1599,   518,  1459,  1587,   888,   802,  1532,
     461,   638,   639,    52,    96,   331,   611,   465,  1541,   628,
     629,   630,   631,  1487,   334,   992,    82,  1586,   750,   636,
      16,   637,   638,   639,   640,   641,   937,   642,   643,    95,
    1430,   243,  1622,    43,   663,   664,    57,   556,   668,   803,
     670,  1587,   672,   557,   244,   323,   801,   244,   244,   244,
     685,   549,  1527,  1572,  1573,  1124,    44,   659,   660,    96,
      58,   323,  1449,  1319,  1451,  1185,   550,  1001,    13,   659,
     660,   897,   324,   325,    74,   654,   655,   656,   657,   658,
     801,   147,   734,   735,   558,   611,  1165,  1118,   324,   325,
     659,   660,  1084,   803,   992,  1397,   334,   753,    13,  1173,
    1320,  1081,    75,   107,   801,  1097,   754,    16,   243,  1513,
     802,   243,   243,   243,   889,  1095,   585,  1129,   567,   571,
     626,   627,   801,   549,   147,   244,   244,   803,  1322,   244,
     925,   244,   993,   244,    80,   244,   326,    16,   550,   556,
     327,   244,   893,   933,   934,   557,   334,   168,   169,   101,
     751,   803,   326,   946,   126,   199,   327,   996,   244,  1308,
     954,   955,   186,   957,  1066,   959,   801,   961,   321,   803,
    1066,    87,  1498,   244,   244,  1356,  1316,    13,  1339,   128,
    1357,  1333,  1516,   200,  1066,   997,   558,   328,   322,   243,
     243,   329,  1067,   243,   330,   243,   815,   243,  1125,   243,
     801,   505,   147,   328,  1317,   243,  1340,   329,  1066,  1121,
     330,   244,  1383,   803,  1066,   105,    16,   630,   631,   331,
    1066,   506,   243,   139,   685,   636,   244,   637,   638,   639,
     640,   641,  1170,  1066,  1330,   331,  1390,   243,   243,   110,
     111,   106,  1392,   738,   843,   105,  1102,   803,  1431,  1103,
     626,   627,  1104,   844,  1389,   102,    57,   104,   578,   830,
     834,  1435,  1499,  1456,   747,   809,  1066,   586,    98,    99,
     100,   999,   626,   627,   848,   243,  1594,  1595,   579,  1549,
      58,   654,   655,   656,   657,   658,  1236,   587,   170,   129,
     243,  1316,   874,   171,  1444,   172,   659,   660,   121,   932,
    1378,  1175,   935,   157,   323,   589,   942,   143,   144,   145,
     146,   893,  1242,   812,  1066,   244,  1316,    70,  1066,  1496,
     762,    57,  1175,  1175,  1252,   590,  1175,  1176,  1132,  1257,
     809,   324,   325,    47,  1138,   810,  1474,  1175,  1630,    48,
     763,   130,  1497,   135,  1512,    58,  1593,   630,   631,  1150,
    1601,  1634,  1445,   252,  1641,   636,  1405,   637,   638,   639,
     640,   641,   811,  1467,  1649,   136,  1266,   753,   253,   630,
     631,  1654,  1169,   254,  1610,   255,   754,   636,   244,   243,
     638,   639,   640,   641,   137,  1611,  1612,   983,   113,   138,
     244,   244,   244,   244,   114,   326,   984,   244,   556,   327,
     132,   244,   626,   627,   557,   903,   133,   903,   244,   244,
     158,   244,   162,   244,  1055,   244,   244,   176,  1311,   753,
      13,   918,   753,  1056,    85,  1318,   659,   660,   754,  1060,
    1079,   754,   334,  1082,   753,    13,  1331,   178,  1061,  1088,
     179,   410,   243,   754,   411,   558,   328,   412,   659,   660,
     329,   892,    60,   330,   243,   243,   243,   243,   928,    16,
     753,   243,   611,   181,   744,   243,   185,   745,   102,   754,
     412,   992,   243,   243,    16,   243,   150,   243,   331,   243,
     243,    61,   151,  1528,  1528,   154,   159,   187,  1053,  1536,
    1402,   155,   160,  1528,   256,  1536,  1403,   628,   629,   630,
     631,   632,   739,   740,   633,   634,   635,   636,  1560,   637,
     638,   639,   640,   641,   334,   642,   643,   189,   878,   463,
     622,   244,   623,   244,   244,    62,   323,   102,   103,   104,
     244,  1566,   753,    13,  1016,   753,    13,   244,  1528,  1528,
     201,   754,   193,   334,   754,   292,  1536,   879,   251,   896,
      63,   334,  1085,   324,   325,   882,   294,    57,   649,   650,
     651,   652,   653,   654,   655,   656,   657,   658,   301,   334,
     334,   302,    16,   884,   886,    16,   244,   244,   659,   660,
     306,    58,   244,   626,   627,   243,  1438,   243,   243,   334,
     307,  1197,   332,  1123,   243,   308,  1540,   244,   309,   334,
     334,   243,  1628,  1131,  1463,   310,  1623,   334,    64,   196,
     197,  1519,   311,  1094,   312,  1094,   314,   326,    65,   315,
     244,   327,  1379,   323,  1203,  1204,  1016,   316,   317,  1646,
     753,    13,   318,   334,  1117,   319,  1120,  1568,   335,   754,
     243,   243,   337,   753,    13,  1274,   243,  1128,   338,   392,
     324,   325,   754,   164,   165,   794,   795,   753,    13,   390,
    1148,   243,   753,    13,   391,   405,   754,   494,   328,  1282,
      16,   754,   329,   521,   948,   330,   495,  1558,   524,  1151,
     630,   631,   527,    16,   243,   528,   529,  1488,   636,   530,
     637,   638,   639,   640,   641,   753,    13,    16,   753,    13,
     331,   531,    16,   532,   754,   533,   545,   754,   164,   165,
     166,   546,  1295,   554,   326,  1296,   564,   565,   327,  1193,
    1597,   196,   197,   198,   572,   580,  1200,  1201,   581,   582,
    1309,   914,   915,   916,   323,    16,   244,   584,    16,  1209,
     588,  1210,  1211,  1212,  1213,  1214,   656,   657,   658,  1217,
      92,    93,    94,   583,   593,   585,   596,   617,  1618,   659,
     660,   324,   325,  1341,   395,   328,   597,   598,   396,   329,
     599,  1122,   330,   600,  1554,   601,   323,   620,   619,   202,
     616,  1381,   397,   398,   323,   203,   662,   399,   400,   401,
     402,   204,   665,   666,   678,   687,   688,   331,   690,   323,
     243,   205,   699,   324,   325,   726,   730,  1486,   733,   206,
     742,   324,   325,   736,   743,   752,   761,   765,   766,   797,
     767,   785,   798,   806,   207,   326,   324,   325,  1286,   327,
     800,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   799,   808,   814,   326,   822,   841,
     244,   327,   244,   824,   880,   326,   328,   901,   891,   327,
     329,   918,  1135,   330,   923,   924,   940,  1452,   951,   982,
     326,   978,   985,   986,   327,   998,  1043,  1000,  1044,    57,
    1045,  1063,  1090,  1046,  1047,  1048,  1074,  1058,   331,  1059,
    1064,  1076,   241,  1078,  1086,  1101,  1107,   506,   328,  1100,
    1116,  1157,   329,    58,  1142,   330,   328,  1464,  1126,  1137,
     329,  1153,  1143,   330,   243,  1127,   243,  1136,  1141,  1146,
    1149,   328,  1596,  1473,  1158,   329,  1156,  1144,   330,  1164,
     331,  1172,  1191,   482,  1174,  1192,  1461,  1194,   331,  1202,
    1206,  1207,   242,   504,  1208,  1215,  1244,   522,  1245,  1247,
    1253,  1258,  1281,   331,  1260,  1272,   520,  1273,   244,   700,
     701,   702,   703,   704,   705,   706,   707,  1280,  1269,  1270,
    1276,   202,  1407,  1284,  1408,  1290,   323,   203,  1291,  1292,
    1313,   323,  1315,   204,   708,  1332,  1334,  1335,  1337,  1338,
    1336,  1343,  1345,   205,   709,   710,   711,  1347,   244,  1348,
    1351,   206,  1346,   324,   325,  1352,  1353,  1354,   324,   325,
    1393,  1355,  1396,  1398,   244,  1401,   207,  1405,  1394,  1400,
    1409,  1412,   243,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,  1410,  1423,  1434,  1416,
    1436,  1417,   243,   594,   595,  1418,  1553,   326,  1555,  1556,
     323,   327,   326,  1422,  1425,  1427,   327,  1443,   243,  1446,
     603,   604,   605,   606,   607,  1448,  1470,  1482,  1483,  1484,
    1485,    57,  1490,  1493,  1495,  1494,  1523,   324,   325,  1503,
    1504,  1502,  1520,  1551,   241,  1521,  1538,  1539,  1545,  1175,
    1567,  1569,  1570,  1574,  1575,   674,  1576,    13,   328,   812,
    1578,  1579,   329,   328,  1145,   330,   -89,   329,  1602,  1147,
     330,  1603,  1606,   686,  1604,  1605,   124,  1607,  1625,    20,
    1627,  1609,    83,   796,   323,   693,  1615,   177,  1620,  1514,
     331,  1617,  1645,   127,   242,   331,    16,  1652,   -94,  1642,
    1653,   326,  1522,   906,  1651,   327,   907,   305,  1626,   142,
     244,   324,   325,   826,  1113,   894,   821,    26,   812,  1525,
    1633,  1447,  1562,   749,  1526,   624,  1563,  1475,  1564,    91,
    1278,   569,  1108,  1109,   570,   313,   404,     0,     0,     0,
     323,   773,  1648,   771,   774,   323,     0,   777,     0,   779,
       0,     0,   328,     0,     0,   323,   329,     0,  1294,   330,
     788,   789,   790,   791,   792,   793,     0,   324,   325,     0,
       0,     0,   324,   325,   243,   326,     0,     0,     0,   327,
       0,  1589,   324,   325,   331,     0,     0,     0,     0,  1016,
       0,     0,   835,   836,     0,     0,   837,   838,   839,   840,
       0,   842,     0,   845,   846,   847,   849,   850,   851,   852,
     853,   854,   856,   857,   858,   859,   860,   861,   862,   863,
     864,   865,   866,     0,   875,     0,   328,     0,  1621,     0,
     329,   326,  1299,   330,     0,   327,   326,     0,     0,     0,
     327,     0,     0,     0,     0,  1629,   326,     0,  1632,     0,
     327,     0,   899,     0,     0,     0,     0,  1016,   331,     0,
    1640,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1647,     0,     0,  1016,     0,     0,     0,     0,
       0,     0,   328,     0,     0,     0,   329,   328,  1306,   330,
     908,   329,     0,  1329,   330,     0,     0,   328,     0,     0,
       0,   329,     0,  1462,   330,     0,   922,     0,     0,     0,
       0,     0,   927,     0,   331,     0,     0,     0,     0,   331,
       0,     0,     0,     0,     0,     0,     0,   941,     0,   331,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   202,
       0,     0,     0,     0,     0,   203,     0,   626,   627,     0,
       0,   204,     0,     0,     0,     0,     0,     0,     0,   973,
       0,   205,     0,     0,   979,     0,   980,     0,   981,   206,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   771,
       0,     0,     0,     0,   207,     0,     0,     0,     0,  1041,
       0,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,     0,     0,     0,     0,     0,     0,
       0,     0,  1057,     0,     0,     0,  1062,     0,     0,     0,
       0,     0,   628,   629,   630,   631,   632,     0,     0,   633,
     634,   635,   636,     0,   637,   638,   639,   640,   641,    57,
     642,   643,  1089,     0,     0,     0,   644,   645,   646,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
       0,  1041,     0,    58,   626,   627,     0,     0,     0,     0,
    1119,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   648,     0,   649,   650,   651,   652,   653,   654,   655,
     656,   657,   658,     0,  1139,   771,     0,     0,     0,     0,
       0,     0,   242,   659,   660,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   712,   713,   714,   715,
     716,   717,   718,   719,     0,     0,     0,     0,     0,  1167,
    1168,     0,     0,     0,     0,   720,     0,     0,     0,     0,
     899,   721,     0,     0,     0,     0,     0,  1187,     0,     0,
    1190,   722,   723,   724,     0,     0,  1196,     0,  1199,   628,
     629,   630,   631,   632,     0,     0,   633,   634,   635,   636,
       0,   637,   638,   639,   640,   641,     0,   642,   643,     0,
       0,     0,   725,   644,   645,   646,     0,     0,  1238,   647,
    1240,     0,  1243,     0,     0,     0,     0,     0,  1246,     0,
       0,     0,  1249,     0,     0,     0,     0,     0,    13,     0,
     899,     0,     0,     0,     0,     0,     0,     0,   648,     0,
     649,   650,   651,   652,   653,   654,   655,   656,   657,   658,
       0,     0,     0,     0,  1265,     0,     0,     0,     0,     0,
     659,   660,     0,     0,  1271,     0,     0,    16,     0,  1275,
       0,  1277,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1285,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   771,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1314,     0,     0,     0,     0,     0,   -80,     0,  1324,  1325,
    1326,     0,     0,     0,     0,     0,     0,   626,   627,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1342,     0,     0,  1344,     0,     0,     0,     0,     0,     0,
       0,     0,  1350,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1360,  1361,  1362,  1363,  1364,  1365,  1366,  1367,
    1368,  1369,  1370,  1371,  1372,  1373,  1374,  1375,  1376,  1377,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1386,
       0,  1387,     0,     0,     0,     0,     0,  1391,     0,     0,
       0,     0,     0,     0,  1218,  1219,  1220,  1221,  1222,  1223,
    1224,  1225,   628,   629,   630,   631,   632,  1226,  1227,   633,
     634,   635,   636,  1228,   637,   638,   639,   640,   641,  1229,
     642,   643,  1230,  1231,     0,  1411,   644,   645,   646,  1232,
    1233,  1234,   647,     0,  1419,  1420,  1421,     0,     0,   827,
       0,     0,  1429,     0,     0,     0,   626,   627,     0,     0,
       0,     0,     0,     0,     0,     0,  1439,  1440,     0,     0,
    1235,   648,     0,   649,   650,   651,   652,   653,   654,   655,
     656,   657,   658,     0,     0,     0,     0,     0,     0,     0,
    1453,     0,     0,   659,   660,  1454,  1455,     0,  1457,  1458,
       0,   208,   209,   210,     0,   212,   213,   214,   215,   216,
     435,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,     0,   230,   231,   232,     0,     0,   235,   236,
     237,   238,     0,     0,     0,     0,     0,  1469,     0,     0,
       0,  1471,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   628,   629,   630,   631,   632,     0,     0,   633,   634,
     635,   636,   771,   637,   638,   639,   640,   641,     0,   642,
     643,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   828,     0,     0,     0,     0,     0,
       0,     0,  1506,   829,     0,     0,     0,     0,     0,     0,
     202,     0,     0,     0,     0,     0,   203,     0,     0,     0,
       0,     0,   204,   650,   651,   652,   653,   654,   655,   656,
     657,   658,   205,     0,     0,     0,  1533,     0,     0,     0,
     206,     0,   659,   660,     0,     0,     0,     0,  1544,     0,
       0,     0,  1546,  1547,     0,   207,   771,     0,     0,  1550,
       0,     0,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,     0,     0,     0,     0,  1581,
       0,     0,     0,     0,     0,  1588,     0,     0,     0,     0,
       0,     0,     0,     0,  1041,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   241,     0,     0,  1588,     0,     0,     0,
       0,  1619,     0,     0,   674,     0,     0,     0,     0,     0,
    1624,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1631,     0,     0,     0,     0,     0,     0,     0,
    1002,  1638,  1041,     0,   417,   418,     3,     0,  -112,   -99,
     -99,  1588,  -109,   242,   419,   420,   421,   422,   423,     0,
    1041,     0,     0,     0,   424,  1003,   425,  1004,  1005,     0,
     426,     0,     0,     0,     0,     0,     0,  1006,   427,  1007,
       0,  -114,     0,  1008,   428,     0,     0,   429,     0,     8,
     430,  1009,     0,  1010,   431,     0,     0,  1011,  1012,     0,
       0,     0,     0,     0,  1013,     0,     0,   433,   434,     0,
     208,   209,   210,     0,   212,   213,   214,   215,   216,   435,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,     0,   230,   231,   232,     0,     0,   235,   236,   237,
     238,   436,   437,   438,  1014,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   439,   440,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,   441,   442,   443,   444,
     445,     0,   446,     0,   447,   448,   449,   450,   451,   452,
     453,   454,    58,     0,    13,   455,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   456,   457,   458,     0,    14,     0,     0,   459,   460,
       0,     0,     0,     0,     0,     0,     0,   461,     0,   462,
       0,   463,   464,    16,   465,  1015,  1002,     0,     0,     0,
     417,   418,     3,     0,  -112,   -99,   -99,     0,  -109,     0,
     419,   420,   421,   422,   423,     0,     0,     0,     0,     0,
     424,  1003,   425,  1004,  1005,     0,   426,     0,     0,     0,
       0,     0,     0,  1006,   427,  1007,     0,  -114,     0,  1008,
     428,     0,     0,   429,     0,     8,   430,  1009,     0,  1010,
     431,     0,     0,  1011,  1012,     0,     0,     0,     0,     0,
    1013,     0,     0,   433,   434,     0,   208,   209,   210,     0,
     212,   213,   214,   215,   216,   435,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,     0,   230,   231,
     232,     0,     0,   235,   236,   237,   238,   436,   437,   438,
    1014,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   439,   440,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,   441,   442,   443,   444,   445,     0,   446,     0,
     447,   448,   449,   450,   451,   452,   453,   454,    58,     0,
      13,   455,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   456,   457,   458,
       0,    14,     0,     0,   459,   460,     0,     0,     0,     0,
       0,     0,     0,   461,     0,   462,     0,   463,   464,    16,
     465,  -291,  1002,     0,     0,     0,   417,   418,     3,     0,
    -112,   -99,   -99,     0,  -109,     0,   419,   420,   421,   422,
     423,     0,     0,     0,     0,     0,   424,  1003,   425,  1004,
    1005,     0,   426,     0,     0,     0,     0,     0,     0,  1006,
     427,  1007,     0,  -114,     0,  1008,   428,     0,     0,   429,
       0,     8,   430,  1009,     0,  1010,   431,     0,     0,  1011,
    1012,     0,     0,     0,     0,     0,  1013,     0,     0,   433,
     434,     0,   208,   209,   210,     0,   212,   213,   214,   215,
     216,   435,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,     0,   230,   231,   232,     0,     0,   235,
     236,   237,   238,   436,   437,   438,  1014,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   439,
     440,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,   441,   442,
     443,   444,   445,     0,   446,     0,   447,   448,   449,   450,
     451,   452,   453,   454,    58,     0,    13,   455,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   456,   457,   458,     0,    14,     0,     0,
     459,   460,     0,     0,     0,     0,     0,     0,     0,   461,
       0,   462,     0,   463,   464,    16,   465,  -288,  1002,     0,
       0,     0,   417,   418,     3,     0,  -112,   -99,   -99,     0,
    -109,     0,   419,   420,   421,   422,   423,     0,     0,     0,
       0,     0,   424,  1003,   425,  1004,  1005,     0,   426,     0,
       0,     0,     0,     0,     0,  1006,   427,  1007,     0,  -114,
       0,  1008,   428,     0,     0,   429,     0,     8,   430,  1009,
       0,  1010,   431,     0,     0,  1011,  1012,     0,     0,     0,
       0,     0,  1013,     0,     0,   433,   434,     0,   208,   209,
     210,     0,   212,   213,   214,   215,   216,   435,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,     0,
     230,   231,   232,     0,     0,   235,   236,   237,   238,   436,
     437,   438,  1014,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   439,   440,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,   441,   442,   443,   444,   445,     0,
     446,     0,   447,   448,   449,   450,   451,   452,   453,   454,
      58,     0,    13,   455,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   456,
     457,   458,     0,    14,     0,     0,   459,   460,     0,     0,
       0,     0,     0,     0,     0,   461,     0,   462,     0,   463,
     464,    16,   465,   -90,  1002,     0,     0,     0,   417,   418,
       3,     0,  -112,   -99,   -99,     0,  -109,     0,   419,   420,
     421,   422,   423,     0,     0,     0,     0,     0,   424,  1003,
     425,  1004,  1005,     0,   426,     0,     0,     0,     0,     0,
       0,  1006,   427,  1007,     0,  -114,     0,  1008,   428,     0,
       0,   429,     0,     8,   430,  1009,     0,  1010,   431,     0,
       0,  1011,  1012,     0,     0,     0,     0,     0,  1013,     0,
       0,   433,   434,     0,   208,   209,   210,     0,   212,   213,
     214,   215,   216,   435,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,     0,   230,   231,   232,     0,
       0,   235,   236,   237,   238,   436,   437,   438,  1014,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   439,   440,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
     441,   442,   443,   444,   445,     0,   446,     0,   447,   448,
     449,   450,   451,   452,   453,   454,    58,     0,    13,   455,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   456,   457,   458,     0,    14,
       0,     0,   459,   460,     0,     0,     0,     0,     0,     0,
       0,   461,     0,   462,     0,   463,   464,    16,   465,   -95,
     417,   418,     0,     0,     0,     0,     0,     0,     0,     0,
     419,   420,   421,   422,   423,     0,     0,     0,     0,     0,
     424,     0,   425,     0,     0,     0,   426,     0,     0,     0,
       0,     0,     0,     0,   427,     0,     0,     0,     0,     0,
     428,     0,     0,   429,     0,     0,   430,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     432,     0,     0,   433,   434,   816,   208,   209,   210,     0,
     212,   213,   214,   215,   216,   435,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,     0,   230,   231,
     232,     0,     0,   235,   236,   237,   238,   436,   437,   438,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   439,   440,     0,     0,     0,     0,     0,
       0,     0,   509,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,   441,   442,   443,   444,   445,     0,   446,   611,
     447,   448,   449,   450,   451,   452,   453,   454,   612,     0,
       0,   455,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   456,   457,   458,
       0,    14,     0,     0,   459,   460,     0,     0,     0,     0,
       0,   417,   418,   817,     0,   462,   818,   463,   464,   608,
     465,   419,   420,   421,   422,   423,     0,     0,     0,     0,
       0,   424,     0,   425,     0,     0,     0,   426,     0,     0,
       0,     0,     0,     0,     0,   427,     0,     0,     0,     0,
       0,   428,     0,     0,   429,   609,     0,   430,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,     0,     0,   433,   434,     0,   208,   209,   210,
       0,   212,   213,   214,   215,   216,   435,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,     0,   230,
     231,   232,     0,     0,   235,   236,   237,   238,   436,   437,
     438,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,   440,     0,     0,     0,     0,
       0,     0,     0,   509,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,   441,   442,   443,   444,   445,     0,   446,
     611,   447,   448,   449,   450,   451,   452,   453,   454,   612,
       0,     0,   455,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   456,   457,
     458,     0,    14,     0,     0,   459,   460,     0,     0,     0,
       0,     0,   417,   418,   461,     0,   462,     0,   463,   464,
       0,   465,   419,   420,   421,   422,   423,     0,     0,     0,
       0,     0,   424,  1003,   425,  1004,     0,     0,   426,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,   428,     0,     0,   429,     0,     0,   430,  1009,
       0,     0,   431,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432,     0,     0,   433,   434,     0,   208,   209,
     210,     0,   212,   213,   214,   215,   216,   435,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,     0,
     230,   231,   232,     0,     0,   235,   236,   237,   238,   436,
     437,   438,  1014,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   439,   440,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,   441,   442,   443,   444,   445,     0,
     446,     0,   447,   448,   449,   450,   451,   452,   453,   454,
      58,     0,     0,   455,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   456,
     457,   458,     0,    14,     0,     0,   459,   460,     0,     0,
       0,     0,     0,   417,   418,   461,     0,   462,     0,   463,
     464,   608,   465,   419,   420,   421,   422,   423,     0,     0,
       0,     0,     0,   424,     0,   425,     0,     0,     0,   426,
       0,     0,     0,     0,     0,     0,     0,   427,     0,     0,
       0,     0,     0,   428,     0,     0,   429,   609,     0,   430,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   432,     0,     0,   433,   434,     0,   208,
     209,   210,     0,   212,   213,   214,   215,   216,   435,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
       0,   230,   231,   232,     0,     0,   235,   236,   237,   238,
     436,   437,   438,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   439,   440,     0,     0,
       0,     0,     0,     0,     0,   509,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,   441,   442,   443,   444,   445,
       0,   446,     0,   447,   448,   449,   450,   451,   452,   453,
     454,    58,     0,     0,   455,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     456,   457,   458,     0,    14,     0,     0,   459,   460,     0,
       0,     0,     0,     0,   417,   418,   461,     0,   462,     0,
     463,   464,     0,   465,   419,   420,   421,   422,   423,     0,
       0,     0,     0,     0,   424,     0,   425,     0,     0,     0,
     426,     0,     0,     0,     0,     0,     0,     0,   427,     0,
       0,     0,     0,     0,   428,     0,     0,   429,     0,     0,
     430,     0,     0,     0,   431,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   432,     0,     0,   433,   434,   936,
     208,   209,   210,     0,   212,   213,   214,   215,   216,   435,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,     0,   230,   231,   232,     0,     0,   235,   236,   237,
     238,   436,   437,   438,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   439,   440,     0,
       0,     0,     0,     0,     0,     0,   509,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,   441,   442,   443,   444,
     445,     0,   446,   611,   447,   448,   449,   450,   451,   452,
     453,   454,   612,     0,     0,   455,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   456,   457,   458,     0,    14,     0,     0,   459,   460,
       0,     0,     0,     0,     0,   417,   418,   461,     0,   462,
       0,   463,   464,     0,   465,   419,   420,   421,   422,   423,
       0,     0,     0,     0,     0,   424,     0,   425,     0,     0,
       0,   426,     0,     0,     0,     0,     0,     0,     0,   427,
       0,     0,     0,     0,     0,   428,     0,     0,   429,     0,
       0,   430,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   432,     0,     0,   433,   434,
       0,   208,   209,   210,     0,   212,   213,   214,   215,   216,
     435,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,     0,   230,   231,   232,     0,     0,   235,   236,
     237,   238,   436,   437,   438,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   439,   440,
       0,     0,     0,     0,     0,     0,     0,   509,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,   441,   442,   443,
     444,   445,     0,   446,   611,   447,   448,   449,   450,   451,
     452,   453,   454,   612,     0,     0,   455,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   456,   457,   458,     0,    14,     0,     0,   459,
     460,     0,     0,     0,     0,     0,   417,   418,   461,     0,
     462,     0,   463,   464,     0,   465,   419,   420,   421,   422,
     423,     0,     0,     0,     0,     0,   424,     0,   425,     0,
       0,     0,   426,     0,     0,     0,     0,     0,     0,     0,
     427,     0,     0,     0,     0,     0,   428,     0,     0,   429,
       0,     0,   430,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   432,     0,     0,   433,
     434,     0,   208,   209,   210,     0,   212,   213,   214,   215,
     216,   435,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,     0,   230,   231,   232,     0,     0,   235,
     236,   237,   238,   436,   437,   438,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   439,
     440,     0,     0,     0,     0,     0,     0,     0,   509,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,   441,   442,
     443,   444,   445,     0,   446,     0,   447,   448,   449,   450,
     451,   452,   453,   454,    58,     0,     0,   455,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   456,   457,   458,     0,    14,     0,     0,
     459,   460,     0,     0,     0,     0,     0,   417,   418,   461,
       0,   462,   876,   463,   464,     0,   465,   419,   420,   421,
     422,   423,     0,     0,     0,     0,     0,   424,     0,   425,
       0,     0,     0,   426,     0,     0,     0,     0,     0,     0,
       0,   427,     0,     0,     0,     0,     0,   428,     0,     0,
     429,     0,     0,   430,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,     0,     0,
     433,   434,     0,   208,   209,   210,     0,   212,   213,   214,
     215,   216,   435,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,     0,   230,   231,   232,     0,     0,
     235,   236,   237,   238,   436,   437,   438,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     439,   440,     0,     0,     0,     0,     0,     0,     0,   509,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,   441,
     442,   443,   444,   445,     0,   446,     0,   447,   448,   449,
     450,   451,   452,   453,   454,    58,     0,     0,   455,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   456,   457,   458,     0,    14,     0,
       0,   459,   460,     0,     0,     0,     0,     0,   417,   418,
     461,     0,   462,  1241,   463,   464,     0,   465,   419,   420,
     421,   422,   423,     0,     0,     0,     0,     0,   424,     0,
     425,     0,     0,     0,   426,     0,     0,     0,     0,     0,
       0,     0,   427,     0,     0,     0,     0,     0,   428,     0,
       0,   429,     0,     0,   430,     0,     0,     0,   431,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   432,     0,
       0,   433,   434,     0,   208,   209,   210,     0,   212,   213,
     214,   215,   216,   435,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,     0,   230,   231,   232,     0,
       0,   235,   236,   237,   238,   436,   437,   438,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   439,   440,     0,     0,     0,     0,     0,     0,     0,
     509,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
     441,   442,   443,   444,   445,     0,   446,     0,   447,   448,
     449,   450,   451,   452,   453,   454,    58,     0,     0,   455,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   456,   457,   458,     0,    14,
       0,     0,   459,   460,     0,     0,     0,     0,     0,   417,
     418,  1250,     0,   462,  1251,   463,   464,     0,   465,   419,
     420,   421,   422,   423,     0,     0,     0,     0,     0,   424,
       0,   425,     0,     0,     0,   426,     0,     0,     0,     0,
       0,     0,     0,   427,     0,     0,     0,     0,     0,   428,
       0,     0,   429,     0,     0,   430,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   432,
       0,     0,   433,   434,     0,   208,   209,   210,     0,   212,
     213,   214,   215,   216,   435,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,     0,   230,   231,   232,
       0,     0,   235,   236,   237,   238,   436,   437,   438,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   439,   440,     0,     0,     0,     0,     0,     0,
       0,   509,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,   441,   442,   443,   444,   445,     0,   446,     0,   447,
     448,   449,   450,   451,   452,   453,   454,    58,     0,     0,
     455,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   456,   457,   458,     0,
      14,     0,     0,   459,   460,     0,     0,     0,     0,     0,
     417,   418,   461,     0,   462,  1256,   463,   464,     0,   465,
     419,   420,   421,   422,   423,     0,     0,     0,     0,     0,
     424,     0,   425,     0,     0,     0,   426,     0,     0,     0,
       0,     0,     0,     0,   427,     0,     0,     0,     0,     0,
     428,     0,     0,   429,     0,     0,   430,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     432,     0,     0,   433,   434,     0,   208,   209,   210,     0,
     212,   213,   214,   215,   216,   435,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,     0,   230,   231,
     232,     0,     0,   235,   236,   237,   238,   436,   437,   438,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   439,   440,     0,     0,     0,     0,     0,
       0,     0,   509,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,   441,   442,   443,   444,   445,     0,   446,     0,
     447,   448,   449,   450,   451,   452,   453,   454,    58,     0,
       0,   455,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   456,   457,   458,
       0,    14,     0,     0,   459,   460,     0,     0,     0,     0,
       0,   417,   418,   461,     0,   462,  1310,   463,   464,     0,
     465,   419,   420,   421,   422,   423,     0,     0,     0,     0,
       0,   424,     0,   425,     0,     0,     0,   426,     0,     0,
       0,     0,     0,     0,     0,   427,     0,     0,     0,     0,
       0,   428,     0,     0,   429,     0,     0,   430,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,     0,     0,   433,   434,     0,   208,   209,   210,
       0,   212,   213,   214,   215,   216,   435,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,     0,   230,
     231,   232,     0,     0,   235,   236,   237,   238,   436,   437,
     438,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,   440,     0,     0,     0,     0,
       0,     0,     0,   509,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,   441,   442,   443,   444,   445,     0,   446,
       0,   447,   448,   449,   450,   451,   452,   453,   454,    58,
       0,     0,   455,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   456,   457,
     458,     0,    14,     0,     0,   459,   460,     0,     0,     0,
       0,     0,   417,   418,   461,     0,   462,     0,   463,   464,
       0,   465,   419,   420,   421,   422,   423,     0,     0,     0,
       0,     0,   424,     0,   425,     0,     0,     0,   426,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,   428,     0,     0,   429,     0,     0,   430,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432,     0,     0,   433,   434,     0,   208,   209,
     210,     0,   212,   213,   214,   215,   216,   435,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,     0,
     230,   231,   232,     0,     0,   235,   236,   237,   238,   436,
     437,   438,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   439,   440,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,   441,   442,   443,   444,   445,     0,
     446,     0,   447,   448,   449,   450,   451,   452,   453,   454,
      58,     0,     0,   455,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   456,
     457,   458,     0,    14,     0,     0,   459,   460,     0,     0,
       0,     0,     0,   417,   418,   461,   519,   462,     0,   463,
     464,     0,   465,   419,   420,   421,   422,   423,     0,     0,
       0,     0,     0,   424,     0,   425,     0,     0,     0,   426,
       0,     0,     0,     0,     0,     0,     0,   427,     0,     0,
       0,     0,     0,   428,     0,     0,   429,     0,     0,   430,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   432,     0,     0,   433,   434,     0,   208,
     209,   210,     0,   212,   213,   214,   215,   216,   435,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
       0,   230,   231,   232,     0,     0,   235,   236,   237,   238,
     436,   437,   438,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   439,   440,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,   441,   442,   443,   444,   445,
       0,   446,     0,   447,   448,   449,   450,   451,   452,   453,
     454,    58,     0,     0,   455,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     456,   457,   458,     0,    14,     0,     0,   459,   460,     0,
       0,     0,     0,     0,   417,   418,   461,   692,   462,     0,
     463,   464,     0,   465,   419,   420,   421,   422,   423,     0,
       0,   855,     0,     0,   424,     0,   425,     0,     0,     0,
     426,     0,     0,     0,     0,     0,     0,     0,   427,     0,
       0,     0,     0,     0,   428,     0,     0,   429,     0,     0,
     430,     0,     0,     0,   431,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   432,     0,     0,   433,   434,     0,
     208,   209,   210,     0,   212,   213,   214,   215,   216,   435,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,     0,   230,   231,   232,     0,     0,   235,   236,   237,
     238,   436,   437,   438,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   439,   440,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,   441,   442,   443,   444,
     445,     0,   446,     0,   447,   448,   449,   450,   451,   452,
     453,   454,    58,     0,     0,   455,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   456,   457,   458,     0,    14,     0,     0,   459,   460,
       0,     0,     0,     0,     0,   417,   418,   461,     0,   462,
       0,   463,   464,     0,   465,   419,   420,   421,   422,   423,
       0,     0,     0,     0,     0,   424,     0,   425,     0,     0,
       0,   426,     0,     0,     0,     0,     0,     0,     0,   427,
       0,     0,     0,     0,     0,   428,     0,     0,   429,     0,
       0,   430,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   432,     0,     0,   433,   434,
       0,   208,   209,   210,     0,   212,   213,   214,   215,   216,
     435,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,     0,   230,   231,   232,     0,     0,   235,   236,
     237,   238,   436,   437,   438,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   439,   440,
       0,     0,     0,     0,     0,     0,     0,   898,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,   441,   442,   443,
     444,   445,     0,   446,     0,   447,   448,   449,   450,   451,
     452,   453,   454,    58,     0,     0,   455,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   456,   457,   458,     0,    14,     0,     0,   459,
     460,     0,     0,     0,     0,     0,   417,   418,   461,     0,
     462,     0,   463,   464,     0,   465,   419,   420,   421,   422,
     423,     0,     0,     0,     0,     0,   424,     0,   425,     0,
       0,     0,   426,     0,     0,     0,     0,     0,     0,     0,
     427,     0,     0,     0,     0,     0,   428,     0,     0,   429,
       0,     0,   430,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   432,     0,     0,   433,
     434,     0,   208,   209,   210,     0,   212,   213,   214,   215,
     216,   435,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,     0,   230,   231,   232,     0,     0,   235,
     236,   237,   238,   436,   437,   438,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   439,
     440,     0,     0,     0,     0,     0,     0,     0,   898,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,   441,   442,
     443,   444,   445,     0,   446,     0,   447,   448,   449,   450,
     451,   452,   453,   454,    58,     0,     0,   455,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   456,   457,   458,     0,    14,     0,     0,
     459,   460,     0,     0,     0,     0,     0,   417,   418,  1177,
       0,   462,     0,   463,   464,     0,   465,   419,   420,   421,
     422,   423,     0,     0,     0,     0,     0,   424,     0,   425,
       0,     0,     0,   426,     0,     0,     0,     0,     0,     0,
       0,   427,     0,     0,     0,     0,     0,   428,     0,     0,
     429,     0,     0,   430,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,     0,     0,
     433,   434,     0,   208,   209,   210,     0,   212,   213,   214,
     215,   216,   435,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,     0,   230,   231,   232,     0,     0,
     235,   236,   237,   238,   436,   437,   438,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     439,   440,     0,     0,     0,     0,     0,     0,     0,  1186,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,   441,
     442,   443,   444,   445,     0,   446,     0,   447,   448,   449,
     450,   451,   452,   453,   454,    58,     0,     0,   455,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   456,   457,   458,     0,    14,     0,
       0,   459,   460,     0,     0,     0,     0,     0,   417,   418,
     461,     0,   462,     0,   463,   464,     0,   465,   419,   420,
     421,   422,   423,     0,     0,     0,     0,     0,   424,     0,
     425,     0,     0,     0,   426,     0,     0,     0,     0,     0,
       0,     0,   427,     0,     0,     0,     0,     0,   428,     0,
       0,   429,     0,     0,   430,     0,     0,     0,   431,     0,
       0,     0,     0,     0,  1189,     0,     0,     0,   432,     0,
       0,   433,   434,     0,   208,   209,   210,     0,   212,   213,
     214,   215,   216,   435,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,     0,   230,   231,   232,     0,
       0,   235,   236,   237,   238,   436,   437,   438,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   439,   440,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
     441,   442,   443,   444,   445,     0,   446,     0,   447,   448,
     449,   450,   451,   452,   453,   454,    58,     0,     0,   455,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   456,   457,   458,     0,    14,
       0,     0,   459,   460,     0,     0,     0,     0,     0,   417,
     418,   461,     0,   462,     0,   463,   464,     0,   465,   419,
     420,   421,   422,   423,     0,     0,     0,     0,     0,   424,
       0,   425,     0,     0,     0,   426,     0,     0,     0,     0,
       0,     0,     0,   427,     0,     0,     0,     0,     0,   428,
       0,     0,   429,     0,     0,   430,     0,     0,     0,   431,
       0,     0,  1195,     0,     0,     0,     0,     0,     0,   432,
       0,     0,   433,   434,     0,   208,   209,   210,     0,   212,
     213,   214,   215,   216,   435,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,     0,   230,   231,   232,
       0,     0,   235,   236,   237,   238,   436,   437,   438,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   439,   440,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,   441,   442,   443,   444,   445,     0,   446,     0,   447,
     448,   449,   450,   451,   452,   453,   454,    58,     0,     0,
     455,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   456,   457,   458,     0,
      14,     0,     0,   459,   460,     0,     0,     0,     0,     0,
     417,   418,   461,     0,   462,     0,   463,   464,     0,   465,
     419,   420,   421,   422,   423,     0,     0,     0,     0,     0,
     424,     0,   425,     0,     0,     0,   426,     0,     0,     0,
       0,     0,     0,     0,   427,     0,     0,     0,     0,     0,
     428,     0,     0,   429,     0,     0,   430,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     432,     0,     0,   433,   434,     0,   208,   209,   210,     0,
     212,   213,   214,   215,   216,   435,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,     0,   230,   231,
     232,     0,     0,   235,   236,   237,   238,   436,   437,   438,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   439,   440,     0,     0,     0,     0,     0,
       0,     0,  1198,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,   441,   442,   443,   444,   445,     0,   446,     0,
     447,   448,   449,   450,   451,   452,   453,   454,    58,     0,
       0,   455,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   456,   457,   458,
       0,    14,     0,     0,   459,   460,     0,     0,     0,     0,
       0,   417,   418,   461,     0,   462,     0,   463,   464,     0,
     465,   419,   420,   421,   422,   423,     0,     0,     0,     0,
       0,   424,     0,   425,     0,     0,     0,   426,     0,     0,
       0,     0,     0,     0,     0,   427,     0,     0,     0,     0,
       0,   428,     0,     0,   429,     0,     0,   430,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,     0,     0,   433,   434,     0,   208,   209,   210,
       0,   212,   213,   214,   215,   216,   435,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,     0,   230,
     231,   232,     0,     0,   235,   236,   237,   238,   436,   437,
     438,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,   440,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,   441,   442,   443,   444,   445,     0,   446,
       0,   447,   448,   449,   450,   451,   452,   453,   454,    58,
       0,     0,   455,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   456,   457,
     458,     0,    14,     0,     0,   459,   460,     0,     0,     0,
       0,     0,   417,   418,   461,     0,   462,  1428,   463,   464,
       0,   465,   419,   420,   421,   422,   423,     0,     0,     0,
       0,     0,   424,     0,   425,     0,     0,     0,   426,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,   428,     0,     0,   429,     0,     0,   430,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432,     0,     0,   433,   434,     0,   208,   209,
     210,     0,   212,   213,   214,   215,   216,   435,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,     0,
     230,   231,   232,     0,     0,   235,   236,   237,   238,   436,
     437,   438,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   439,   440,     0,     0,     0,
       0,     0,     0,     0,  1505,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,   441,   442,   443,   444,   445,     0,
     446,     0,   447,   448,   449,   450,   451,   452,   453,   454,
      58,     0,     0,   455,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   456,
     457,   458,     0,    14,     0,     0,   459,   460,     0,     0,
       0,     0,     0,   417,   418,   461,     0,   462,     0,   463,
     464,     0,   465,   419,   420,   421,   422,   423,     0,     0,
       0,     0,     0,   424,     0,   425,     0,     0,     0,   426,
       0,     0,     0,     0,     0,     0,     0,   427,     0,     0,
       0,     0,     0,   428,     0,     0,   429,     0,     0,   430,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   432,     0,     0,   433,   434,     0,   208,
     209,   210,     0,   212,   213,   214,   215,   216,   435,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
       0,   230,   231,   232,     0,     0,   235,   236,   237,   238,
     436,   437,   438,     0,     0,     0,     0,     0,     0,   626,
     627,     0,     0,     0,     0,     0,   439,   440,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,   441,   442,   443,   444,   445,
       0,   446,     0,   447,   448,   449,   450,   451,   452,   453,
     454,    58,     0,     0,   455,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   831,
     456,   457,   458,     0,    14,     0,     0,   459,   460,     0,
       0,     0,   626,   627,     0,     0,   461,     0,   462,     0,
     463,   464,     0,   465,   628,   629,   630,   631,   632,     0,
       0,   633,   634,   635,   636,     0,   637,   638,   639,   640,
     641,     0,   642,   643,     0,     0,     0,     0,   644,   645,
     646,   208,   209,   210,   647,   212,   213,   214,   215,   216,
     435,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,     0,   230,   231,   232,     0,     0,   235,   236,
     237,   238,     0,   648,     0,   649,   650,   651,   652,   653,
     654,   655,   656,   657,   658,     0,     0,     0,   626,   627,
       0,     0,     0,     0,     0,   659,   660,   628,   629,   630,
     631,   632,  1579,     0,   633,   634,   635,   636,     0,   637,
     638,   639,   640,   641,     0,   642,   643,     0,     0,     0,
       0,   644,   645,   646,   832,     0,     0,   647,     0,     0,
       0,     0,     0,   833,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   648,     0,   649,   650,
     651,   652,   653,   654,   655,   656,   657,   658,     0,     0,
       0,     0,     0,     0,   626,   627,     0,     0,   659,   660,
       0,     0,   661,   628,   629,   630,   631,   632,     0,     0,
     633,   634,   635,   636,     0,   637,   638,   639,   640,   641,
       0,   642,   643,     0,     0,     0,     0,   644,   645,   646,
       0,     0,     0,   647,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   648,     0,   649,   650,   651,   652,   653,   654,
     655,   656,   657,   658,     0,     0,     0,     0,     0,     0,
     626,   627,     0,     0,   659,   660,     0,     0,   681,   628,
     629,   630,   631,   632,     0,     0,   633,   634,   635,   636,
       0,   637,   638,   639,   640,   641,     0,   642,   643,     0,
       0,     0,     0,   644,   645,   646,     0,     0,     0,   647,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   648,     0,
     649,   650,   651,   652,   653,   654,   655,   656,   657,   658,
       0,     0,     0,     0,     0,     0,   626,   627,     0,     0,
     659,   660,     0,     0,   953,   628,   629,   630,   631,   632,
       0,     0,   633,   634,   635,   636,     0,   637,   638,   639,
     640,   641,     0,   642,   643,     0,     0,     0,     0,   644,
     645,   646,     0,     0,     0,   647,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   648,     0,   649,   650,   651,   652,
     653,   654,   655,   656,   657,   658,     0,     0,     0,     0,
       0,     0,   626,   627,     0,     0,   659,   660,     0,     0,
     956,   628,   629,   630,   631,   632,     0,     0,   633,   634,
     635,   636,     0,   637,   638,   639,   640,   641,     0,   642,
     643,     0,     0,     0,     0,   644,   645,   646,     0,     0,
       0,   647,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     648,     0,   649,   650,   651,   652,   653,   654,   655,   656,
     657,   658,     0,     0,     0,     0,     0,     0,   626,   627,
       0,     0,   659,   660,     0,     0,   958,   628,   629,   630,
     631,   632,     0,     0,   633,   634,   635,   636,     0,   637,
     638,   639,   640,   641,     0,   642,   643,     0,     0,     0,
       0,   644,   645,   646,     0,     0,     0,   647,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   648,     0,   649,   650,
     651,   652,   653,   654,   655,   656,   657,   658,     0,     0,
       0,     0,     0,     0,   626,   627,     0,     0,   659,   660,
       0,     0,   967,   628,   629,   630,   631,   632,     0,     0,
     633,   634,   635,   636,     0,   637,   638,   639,   640,   641,
       0,   642,   643,     0,     0,     0,     0,   644,   645,   646,
       0,     0,     0,   647,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   648,     0,   649,   650,   651,   652,   653,   654,
     655,   656,   657,   658,     0,     0,     0,     0,     0,     0,
     626,   627,     0,     0,   659,   660,     0,     0,   968,   628,
     629,   630,   631,   632,     0,     0,   633,   634,   635,   636,
       0,   637,   638,   639,   640,   641,     0,   642,   643,     0,
       0,     0,     0,   644,   645,   646,     0,     0,     0,   647,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   648,     0,
     649,   650,   651,   652,   653,   654,   655,   656,   657,   658,
       0,     0,     0,     0,     0,     0,   626,   627,     0,     0,
     659,   660,     0,     0,   969,   628,   629,   630,   631,   632,
       0,     0,   633,   634,   635,   636,     0,   637,   638,   639,
     640,   641,     0,   642,   643,     0,     0,     0,     0,   644,
     645,   646,     0,     0,     0,   647,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   648,     0,   649,   650,   651,   652,
     653,   654,   655,   656,   657,   658,     0,     0,     0,     0,
       0,     0,   626,   627,     0,     0,   659,   660,     0,     0,
     970,   628,   629,   630,   631,   632,     0,     0,   633,   634,
     635,   636,     0,   637,   638,   639,   640,   641,     0,   642,
     643,     0,     0,     0,     0,   644,   645,   646,     0,     0,
       0,   647,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     648,     0,   649,   650,   651,   652,   653,   654,   655,   656,
     657,   658,     0,     0,     0,     0,     0,     0,   626,   627,
       0,     0,   659,   660,     0,     0,   971,   628,   629,   630,
     631,   632,     0,     0,   633,   634,   635,   636,     0,   637,
     638,   639,   640,   641,     0,   642,   643,     0,     0,     0,
       0,   644,   645,   646,     0,     0,     0,   647,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   648,     0,   649,   650,
     651,   652,   653,   654,   655,   656,   657,   658,     0,     0,
       0,     0,     0,     0,   626,   627,     0,     0,   659,   660,
       0,     0,   972,   628,   629,   630,   631,   632,     0,     0,
     633,   634,   635,   636,     0,   637,   638,   639,   640,   641,
       0,   642,   643,     0,     0,     0,     0,   644,   645,   646,
       0,     0,     0,   647,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   648,     0,   649,   650,   651,   652,   653,   654,
     655,   656,   657,   658,     0,     0,     0,     0,     0,     0,
     626,   627,     0,     0,   659,   660,     0,     0,  1099,   628,
     629,   630,   631,   632,     0,     0,   633,   634,   635,   636,
       0,   637,   638,   639,   640,   641,     0,   642,   643,     0,
       0,     0,     0,   644,   645,   646,     0,     0,     0,   647,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   648,     0,
     649,   650,   651,   652,   653,   654,   655,   656,   657,   658,
       0,     0,     0,     0,     0,     0,   626,   627,     0,     0,
     659,   660,     0,     0,  1115,   628,   629,   630,   631,   632,
       0,     0,   633,   634,   635,   636,     0,   637,   638,   639,
     640,   641,     0,   642,   643,     0,     0,     0,     0,   644,
     645,   646,     0,     0,     0,   647,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   648,     0,   649,   650,   651,   652,
     653,   654,   655,   656,   657,   658,     0,     0,     0,     0,
       0,     0,   626,   627,     0,     0,   659,   660,     0,     0,
    1130,   628,   629,   630,   631,   632,     0,     0,   633,   634,
     635,   636,     0,   637,   638,   639,   640,   641,     0,   642,
     643,     0,     0,     0,     0,   644,   645,   646,     0,     0,
       0,   647,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     648,     0,   649,   650,   651,   652,   653,   654,   655,   656,
     657,   658,     0,     0,     0,     0,     0,     0,   626,   627,
       0,     0,   659,   660,     0,     0,  1163,   628,   629,   630,
     631,   632,     0,     0,   633,   634,   635,   636,     0,   637,
     638,   639,   640,   641,     0,   642,   643,     0,     0,     0,
       0,   644,   645,   646,     0,     0,     0,   647,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   648,     0,   649,   650,
     651,   652,   653,   654,   655,   656,   657,   658,     0,     0,
       0,     0,     0,     0,   626,   627,     0,     0,   659,   660,
       0,     0,  1327,   628,   629,   630,   631,   632,     0,     0,
     633,   634,   635,   636,     0,   637,   638,   639,   640,   641,
       0,   642,   643,     0,     0,     0,     0,   644,   645,   646,
       0,     0,     0,   647,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   648,     0,   649,   650,   651,   652,   653,   654,
     655,   656,   657,   658,     0,     0,     0,     0,     0,     0,
     626,   627,     0,     0,   659,   660,     0,     0,  1328,   628,
     629,   630,   631,   632,     0,     0,   633,   634,   635,   636,
       0,   637,   638,   639,   640,   641,     0,   642,   643,     0,
       0,     0,     0,   644,   645,   646,     0,     0,     0,   647,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   648,     0,
     649,   650,   651,   652,   653,   654,   655,   656,   657,   658,
       0,     0,     0,     0,     0,     0,   626,   627,     0,     0,
     659,   660,     0,     0,  1380,   628,   629,   630,   631,   632,
       0,     0,   633,   634,   635,   636,     0,   637,   638,   639,
     640,   641,     0,   642,   643,     0,     0,     0,     0,   644,
     645,   646,     0,     0,     0,   647,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   648,     0,   649,   650,   651,   652,
     653,   654,   655,   656,   657,   658,     0,     0,     0,     0,
       0,     0,   626,   627,     0,     0,   659,   660,     0,     0,
    1382,   628,   629,   630,   631,   632,     0,     0,   633,   634,
     635,   636,     0,   637,   638,   639,   640,   641,     0,   642,
     643,     0,     0,     0,     0,   644,   645,   646,     0,     0,
       0,   647,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     648,     0,   649,   650,   651,   652,   653,   654,   655,   656,
     657,   658,     0,     0,     0,     0,     0,     0,   626,   627,
       0,     0,   659,   660,     0,     0,  1384,   628,   629,   630,
     631,   632,     0,     0,   633,   634,   635,   636,     0,   637,
     638,   639,   640,   641,     0,   642,   643,     0,     0,     0,
       0,   644,   645,   646,     0,     0,     0,   647,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   648,     0,   649,   650,
     651,   652,   653,   654,   655,   656,   657,   658,     0,     0,
       0,     0,     0,     0,   626,   627,     0,     0,   659,   660,
       0,     0,  1388,   628,   629,   630,   631,   632,     0,     0,
     633,   634,   635,   636,     0,   637,   638,   639,   640,   641,
       0,   642,   643,     0,     0,     0,     0,   644,   645,   646,
       0,     0,     0,   647,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   648,     0,   649,   650,   651,   652,   653,   654,
     655,   656,   657,   658,     0,     0,     0,     0,     0,     0,
     626,   627,     0,     0,   659,   660,     0,     0,  1395,   628,
     629,   630,   631,   632,     0,     0,   633,   634,   635,   636,
       0,   637,   638,   639,   640,   641,     0,   642,   643,     0,
       0,     0,     0,   644,   645,   646,     0,     0,     0,   647,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   648,     0,
     649,   650,   651,   652,   653,   654,   655,   656,   657,   658,
       0,     0,     0,     0,     0,     0,   626,   627,     0,     0,
     659,   660,     0,     0,  1433,   628,   629,   630,   631,   632,
       0,     0,   633,   634,   635,   636,     0,   637,   638,   639,
     640,   641,     0,   642,   643,     0,     0,     0,     0,   644,
     645,   646,     0,     0,     0,   647,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   648,     0,   649,   650,   651,   652,
     653,   654,   655,   656,   657,   658,     0,     0,     0,     0,
       0,     0,   626,   627,     0,     0,   659,   660,     0,     0,
    1441,   628,   629,   630,   631,   632,     0,     0,   633,   634,
     635,   636,     0,   637,   638,   639,   640,   641,     0,   642,
     643,     0,     0,     0,     0,   644,   645,   646,     0,     0,
       0,   647,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     648,     0,   649,   650,   651,   652,   653,   654,   655,   656,
     657,   658,     0,     0,     0,     0,     0,     0,   626,   627,
       0,     0,   659,   660,     0,     0,  1465,   628,   629,   630,
     631,   632,     0,     0,   633,   634,   635,   636,     0,   637,
     638,   639,   640,   641,     0,   642,   643,     0,     0,     0,
       0,   644,   645,   646,     0,     0,     0,   647,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   648,     0,   649,   650,
     651,   652,   653,   654,   655,   656,   657,   658,     0,     0,
       0,     0,     0,     0,   626,   627,     0,     0,   659,   660,
       0,     0,  1466,   628,   629,   630,   631,   632,     0,     0,
     633,   634,   635,   636,     0,   637,   638,   639,   640,   641,
       0,   642,   643,     0,     0,     0,     0,   644,   645,   646,
       0,     0,     0,   647,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   648,     0,   649,   650,   651,   652,   653,   654,
     655,   656,   657,   658,     0,     0,     0,     0,     0,     0,
     626,   627,     0,     0,   659,   660,     0,     0,  1468,   628,
     629,   630,   631,   632,     0,     0,   633,   634,   635,   636,
       0,   637,   638,   639,   640,   641,     0,   642,   643,     0,
       0,     0,     0,   644,   645,   646,     0,     0,     0,   647,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   648,     0,
     649,   650,   651,   652,   653,   654,   655,   656,   657,   658,
       0,     0,     0,     0,     0,     0,   626,   627,     0,     0,
     659,   660,     0,     0,  1481,   628,   629,   630,   631,   632,
       0,     0,   633,   634,   635,   636,     0,   637,   638,   639,
     640,   641,     0,   642,   643,     0,     0,     0,     0,   644,
     645,   646,     0,     0,     0,   647,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   648,     0,   649,   650,   651,   652,
     653,   654,   655,   656,   657,   658,     0,     0,     0,     0,
       0,     0,   626,   627,     0,     0,   659,   660,     0,     0,
    1492,   628,   629,   630,   631,   632,     0,     0,   633,   634,
     635,   636,     0,   637,   638,   639,   640,   641,     0,   642,
     643,     0,     0,     0,     0,   644,   645,   646,     0,     0,
       0,   647,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     648,     0,   649,   650,   651,   652,   653,   654,   655,   656,
     657,   658,     0,     0,     0,     0,     0,     0,   626,   627,
       0,     0,   659,   660,     0,     0,  1507,   628,   629,   630,
     631,   632,     0,     0,   633,   634,   635,   636,     0,   637,
     638,   639,   640,   641,     0,   642,   643,     0,     0,     0,
       0,   644,   645,   646,     0,     0,     0,   647,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   648,     0,   649,   650,
     651,   652,   653,   654,   655,   656,   657,   658,     0,     0,
       0,     0,     0,     0,   626,   627,     0,     0,   659,   660,
       0,     0,  1508,   628,   629,   630,   631,   632,     0,     0,
     633,   634,   635,   636,     0,   637,   638,   639,   640,   641,
       0,   642,   643,     0,     0,     0,     0,   644,   645,   646,
       0,     0,     0,   647,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   648,     0,   649,   650,   651,   652,   653,   654,
     655,   656,   657,   658,     0,     0,     0,     0,     0,     0,
     626,   627,     0,     0,   659,   660,     0,     0,  1510,   628,
     629,   630,   631,   632,     0,     0,   633,   634,   635,   636,
       0,   637,   638,   639,   640,   641,     0,   642,   643,     0,
       0,     0,     0,   644,   645,   646,     0,     0,     0,   647,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   648,     0,
     649,   650,   651,   652,   653,   654,   655,   656,   657,   658,
       0,     0,     0,     0,     0,     0,   626,   627,     0,     0,
     659,   660,     0,     0,  1511,   628,   629,   630,   631,   632,
       0,     0,   633,   634,   635,   636,     0,   637,   638,   639,
     640,   641,     0,   642,   643,     0,     0,     0,     0,   644,
     645,   646,     0,     0,     0,   647,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   648,     0,   649,   650,   651,   652,
     653,   654,   655,   656,   657,   658,     0,     0,     0,     0,
       0,     0,   626,   627,     0,     0,   659,   660,     0,     0,
    1571,   628,   629,   630,   631,   632,     0,     0,   633,   634,
     635,   636,     0,   637,   638,   639,   640,   641,     0,   642,
     643,     0,     0,     0,     0,   644,   645,   646,     0,     0,
       0,   647,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     648,     0,   649,   650,   651,   652,   653,   654,   655,   656,
     657,   658,     0,     0,     0,     0,   626,   627,     0,     0,
       0,     0,   659,   660,     0,     0,  1644,   628,   629,   630,
     631,   632,     0,     0,   633,   634,   635,   636,     0,   637,
     638,   639,   640,   641,     0,   642,   643,     0,     0,     0,
       0,   644,   645,   646,     0,     0,     0,   647,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   648,     0,   649,   650,
     651,   652,   653,   654,   655,   656,   657,   658,     0,     0,
     626,   627,     0,     0,     0,     0,     0,     0,   659,   660,
     691,   628,   629,   630,   631,   632,     0,     0,   633,   634,
     635,   636,     0,   637,   638,   639,   640,   641,     0,   642,
     643,     0,     0,     0,     0,   644,   645,   646,     0,     0,
       0,   647,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     648,     0,   649,   650,   651,   652,   653,   654,   655,   656,
     657,   658,     0,     0,   626,   627,     0,     0,     0,     0,
       0,     0,   659,   660,   900,   628,   629,   630,   631,   632,
       0,     0,   633,   634,   635,   636,     0,   637,   638,   639,
     640,   641,     0,   642,   643,     0,     0,     0,     0,   644,
     645,   646,     0,     0,     0,   647,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   648,     0,   649,   650,   651,   652,
     653,   654,   655,   656,   657,   658,     0,     0,   626,   627,
       0,     0,     0,     0,     0,     0,   659,   660,  1049,   628,
     629,   630,   631,   632,     0,     0,   633,   634,   635,   636,
       0,   637,   638,   639,   640,   641,     0,   642,   643,     0,
       0,     0,     0,   644,   645,   646,     0,     0,     0,   647,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   648,     0,
     649,   650,   651,   652,   653,   654,   655,   656,   657,   658,
       0,     0,   626,   627,   261,     0,     0,     0,     0,     0,
     659,   660,  1065,   628,   629,   630,   631,   632,     0,     0,
     633,   634,   635,   636,     0,   637,   638,   639,   640,   641,
       0,   642,   643,     0,     0,     0,     0,   644,   645,   646,
     262,     0,   263,   647,   264,   265,   266,   267,   268,     0,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,     0,   280,   281,   282,     0,     0,   283,   284,   285,
     286,     0,   648,     0,   649,   650,   651,   652,   653,   654,
     655,   656,   657,   658,     0,     0,     0,   287,   288,     0,
       0,     0,     0,     0,   659,   660,  1248,   628,   629,   630,
     631,   632,   339,   340,   633,   634,   635,   636,     0,   637,
     638,   639,   640,   641,     0,   642,   643,     0,     0,   341,
       0,   644,   645,   646,     0,     0,     0,   647,     0,     0,
       0,     0,   289,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   648,     0,   649,   650,
     651,   652,   653,   654,   655,   656,   657,   658,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   659,   660,
    1254,     0,     0,     0,     0,   626,   627,     0,     0,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,     0,     0,   360,
     361,   362,     0,     0,   363,   364,   365,   366,   367,     0,
       0,   368,   369,   370,   371,   372,   373,   374,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1050,   375,     0,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   626,   627,
     386,   387,     0,     0,     0,     0,     0,     0,   388,   389,
     628,   629,   630,   631,   632,     0,     0,   633,   634,   635,
     636,     0,   637,   638,   639,   640,   641,     0,   642,   643,
       0,     0,   950,     0,   644,   645,   646,   208,   209,   210,
     647,   212,   213,   214,   215,   216,   435,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,     0,   230,
     231,   232,     0,     0,   235,   236,   237,   238,     0,   648,
       0,   649,   650,   651,   652,   653,   654,   655,   656,   657,
     658,   626,   627,     0,     0,     0,     0,     0,     0,     0,
       0,   659,   660,   628,   629,   630,   631,   632,     0,     0,
     633,   634,   635,   636,     0,   637,   638,   639,   640,   641,
       0,   642,   643,     0,     0,     0,     0,   644,   645,   646,
    1051,     0,     0,   647,     0,     0,     0,     0,     0,  1052,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   648,  1054,   649,   650,   651,   652,   653,   654,
     655,   656,   657,   658,   626,   627,     0,     0,     0,     0,
       0,     0,     0,     0,   659,   660,   628,   629,   630,   631,
     632,     0,     0,   633,   634,   635,   636,     0,   637,   638,
     639,   640,   641,     0,   642,   643,     0,     0,     0,     0,
     644,   645,   646,     0,     0,     0,   647,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   648,  1154,   649,   650,   651,
     652,   653,   654,   655,   656,   657,   658,   626,   627,     0,
       0,     0,     0,     0,     0,     0,     0,   659,   660,   628,
     629,   630,   631,   632,     0,     0,   633,   634,   635,   636,
       0,   637,   638,   639,   640,   641,     0,   642,   643,   626,
     627,     0,     0,   644,   645,   646,     0,     0,     0,   647,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   648,     0,
     649,   650,   651,   652,   653,   654,   655,   656,   657,   658,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     659,   660,   628,   629,   630,   631,   632,     0,     0,   633,
     634,   635,   636,     0,   637,   638,   639,   640,   641,     0,
     642,   643,   626,   627,     0,     0,   644,   645,   646,     0,
       0,     0,  -817,     0,   628,   629,   630,   631,   632,     0,
       0,   633,   634,   635,   636,     0,   637,   638,   639,   640,
     641,     0,   642,   643,   626,   627,     0,     0,   644,     0,
     646,   648,     0,   649,   650,   651,   652,   653,   654,   655,
     656,   657,   658,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   659,   660,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   649,   650,   651,   652,   653,
     654,   655,   656,   657,   658,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   659,   660,   628,   629,   630,
     631,   632,     0,     0,   633,   634,   635,   636,     0,   637,
     638,   639,   640,   641,     0,   642,   643,   626,   627,     0,
       0,   644,     0,     0,     0,     0,     0,     0,     0,   628,
     629,   630,   631,   632,     0,     0,   633,   634,   635,   636,
       0,   637,   638,   639,   640,   641,     0,   642,   643,   626,
     627,     0,     0,     0,     0,     0,     0,     0,   649,   650,
     651,   652,   653,   654,   655,   656,   657,   658,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   659,   660,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   651,   652,   653,   654,   655,   656,   657,   658,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     659,   660,   628,   629,   630,   631,   632,     0,     0,   633,
     634,   635,   636,     0,   637,   638,   639,   640,   641,     0,
     642,   643,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   628,   629,   630,   631,   632,     0,
       0,   633,     0,     0,   636,     0,   637,   638,   639,   640,
     641,     0,   642,   643,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   652,   653,   654,   655,
     656,   657,   658,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   659,   660,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   652,   653,
     654,   655,   656,   657,   658,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   659,   660,   208,   209,   210,
       0,   212,   213,   214,   215,   216,   435,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,     0,   230,
     231,   232,     0,     0,   235,   236,   237,   238,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   867,   868,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     869,     0,     0,     0,     0,     0,     0,     0,     0,   870,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   871,   872
};

static const yytype_int16 yycheck[] =
{
       1,    14,    15,   322,   136,   137,   551,   555,   163,   334,
     178,   613,   614,   736,   814,   526,  1005,   462,   951,    20,
      21,    22,  1137,   669,   913,   671,   525,   673,   619,  1216,
     563,   497,     0,   499,   896,   501,   902,    80,    33,     7,
       5,   783,     8,    20,    22,    19,   126,    20,   826,    50,
      63,    64,    65,   126,   920,   786,    20,    20,   163,   126,
     165,   506,    30,  1442,    32,   126,    34,    19,    20,   126,
       7,    46,    40,   151,    40,  1491,   173,  1551,    33,    57,
    1304,   683,    50,   176,   181,   148,   411,   180,    56,   102,
     103,   104,   105,    15,    16,   984,   176,     5,     6,   204,
     163,   178,   156,   176,   151,    60,    61,   204,   181,   176,
      33,   817,    80,    50,   181,   176,   165,    25,  1592,   176,
     181,    33,  1501,    31,   181,   203,  1542,   200,   140,   141,
     142,   106,   577,   200,   102,   103,  1551,   102,   103,   200,
     127,   165,   587,   200,   299,   590,   133,   680,    60,    61,
     193,   684,  1551,   177,   201,   204,   203,   206,  1549,   291,
      68,    69,    62,  1637,   296,   178,   148,   769,   300,   124,
     761,   773,   176,   128,   619,   179,   163,  1592,   978,   781,
     204,   163,   784,   174,   179,  1409,  1410,   174,   966,   201,
    1581,   192,   179,  1592,   102,   103,   173,   165,   746,   165,
     173,  1425,   944,  1427,    21,    22,   180,   185,  1197,   173,
     173,   163,   124,   148,   945,   180,   128,   662,   186,   197,
     175,  1551,  1637,  1085,   179,   816,  1092,   182,   163,   242,
     198,   139,   198,   173,   156,   200,   204,   770,  1637,   161,
     206,   163,  1108,   173,   166,  1551,   414,   415,   416,  1320,
     139,   176,   207,   578,   163,   163,   179,   334,   301,  1483,
    1484,   163,  1592,   175,   204,   200,  1490,   179,   593,   181,
     182,   165,   180,  1460,  1163,   198,   126,   409,   200,   129,
     130,   203,   127,  1561,   207,  1356,  1592,   176,   133,  1480,
     198,   129,   130,   202,   139,   207,   154,   205,  1489,   116,
     117,   118,   119,  1418,   177,   163,   173,  1637,   181,   126,
     204,   128,   129,   130,   131,   132,   761,   134,   135,   163,
    1309,   334,  1600,   163,   492,   493,   139,   127,   496,   174,
     498,  1637,   500,   133,   411,    33,   127,   414,   415,   416,
     508,   148,   200,  1534,  1535,   936,   186,   197,   198,   139,
     163,    33,  1341,   163,  1343,  1001,   163,   823,   165,   197,
     198,   894,    60,    61,     8,   182,   183,   184,   185,   186,
     127,   165,   540,   541,   174,   154,   133,   925,    60,    61,
     197,   198,   893,   174,   163,  1274,   177,   164,   165,   991,
     200,   890,    36,   107,   127,   906,   173,   204,   411,   206,
     133,   414,   415,   416,   181,   904,   200,   940,   421,   422,
      21,    22,   127,   148,   165,   492,   493,   174,   133,   496,
     745,   498,   201,   500,   173,   502,   124,   204,   163,   127,
     128,   508,   965,   758,   759,   133,   177,    15,    16,   200,
     181,   174,   124,   768,   156,   173,   128,   173,   525,   200,
     775,   776,   201,   778,   173,   780,   127,   782,   180,   174,
     173,   197,   133,   540,   541,   200,   173,   165,   173,   163,
     205,  1177,  1461,   201,   173,   201,   174,   175,   200,   492,
     493,   179,   201,   496,   182,   498,   618,   500,   201,   502,
     127,   180,   165,   175,   201,   508,   201,   179,   173,   181,
     182,   578,   201,   174,   173,   173,   204,   118,   119,   207,
     173,   200,   525,   167,   682,   126,   593,   128,   129,   130,
     131,   132,   988,   173,  1170,   207,   201,   540,   541,     5,
       6,   199,   201,   546,   154,   173,   173,   174,   201,   176,
      21,    22,   179,   163,  1250,   140,   139,   142,   180,   626,
     627,   201,  1441,  1353,   555,   148,   173,   180,    63,    64,
      65,   199,    21,    22,   641,   578,  1555,  1556,   200,  1502,
     163,   182,   183,   184,   185,   186,  1042,   200,   156,   163,
     593,   173,   659,   161,   201,   163,   197,   198,   166,   757,
    1236,   173,   760,   107,    33,   180,   764,   102,   103,   104,
     105,  1134,  1047,   616,   173,   682,   173,   620,   173,   201,
     180,   139,   173,   173,  1059,   200,   173,   199,   943,  1064,
     148,    60,    61,    57,   949,   153,    47,   173,  1617,    63,
     200,   163,   201,   163,   201,   163,   201,   118,   119,   964,
     201,   201,   199,    79,  1633,   126,    67,   128,   129,   130,
     131,   132,   180,   199,  1645,   163,   163,   164,    94,   118,
     119,  1652,   987,    99,    12,   101,   173,   126,   745,   682,
     129,   130,   131,   132,   163,    23,    24,   154,    57,   163,
     757,   758,   759,   760,    63,   124,   163,   764,   127,   128,
      57,   768,    21,    22,   133,   696,    63,   698,   775,   776,
     107,   778,   205,   780,   154,   782,   783,   203,  1153,   164,
     165,   163,   164,   163,    57,  1160,   197,   198,   173,   154,
     888,   173,   177,   891,   164,   165,  1171,   174,   163,   897,
     190,   173,   745,   173,   176,   174,   175,   179,   197,   198,
     179,   181,    34,   182,   757,   758,   759,   760,   749,   204,
     164,   764,   154,   176,   173,   768,   167,   176,   140,   173,
     179,   163,   775,   776,   204,   778,    57,   780,   207,   782,
     783,    63,    63,  1479,  1480,    57,    57,   165,   855,  1485,
      57,    63,    63,  1489,   164,  1491,    63,   116,   117,   118,
     119,   120,   204,   205,   123,   124,   125,   126,  1521,   128,
     129,   130,   131,   132,   177,   134,   135,   163,   181,   202,
     203,   888,   205,   890,   891,   107,    33,   140,   141,   142,
     897,  1527,   164,   165,   825,   164,   165,   904,  1534,  1535,
     163,   173,   203,   177,   173,   176,  1542,   181,    66,   181,
     132,   177,   181,    60,    61,   181,   106,   139,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   203,   177,
     177,   163,   204,   181,   181,   204,   943,   944,   197,   198,
     180,   163,   949,    21,    22,   888,  1321,   890,   891,   177,
     180,  1013,   198,   181,   897,   180,  1488,   964,   200,   177,
     177,   904,  1615,   181,   181,   180,  1602,   177,   190,   167,
     168,   181,   180,   904,   200,   906,   180,   124,   200,   180,
     987,   128,  1237,    33,    10,    11,   917,   180,   180,  1642,
     164,   165,   180,   177,   925,   200,   927,  1529,   164,   173,
     943,   944,   205,   164,   165,  1103,   949,   181,   205,   200,
      60,    61,   173,   167,   168,   169,   170,   164,   165,    35,
     181,   964,   164,   165,    35,   163,   173,   163,   175,  1114,
     204,   173,   179,   198,   181,   182,   180,  1512,   205,   181,
     118,   119,   163,   204,   987,   163,   163,  1422,   126,   199,
     128,   129,   130,   131,   132,   164,   165,   204,   164,   165,
     207,    22,   204,   163,   173,   199,   205,   173,   167,   168,
     169,   176,   181,   163,   124,   181,   180,   180,   128,  1010,
    1558,   167,   168,   169,   180,   200,  1017,  1018,   180,   180,
    1152,   730,   731,   732,    33,   204,  1103,   180,   204,  1030,
     180,  1032,  1033,  1034,  1035,  1036,   184,   185,   186,  1040,
      54,    55,    56,   200,   180,   200,   200,    13,  1596,   197,
     198,    60,    61,  1185,    75,   175,   200,   200,    79,   179,
     200,   181,   182,   200,  1509,   200,    33,   198,   200,    19,
     203,  1239,    93,    94,    33,    25,   200,    98,    99,   100,
     101,    31,   201,   181,   163,   201,   173,   207,   199,    33,
    1103,    41,   163,    60,    61,   200,   204,  1416,   201,    49,
     200,    60,    61,   205,    43,   181,   200,   180,   180,   200,
     180,   180,    13,   173,    64,   124,    60,    61,  1119,   128,
     200,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   199,   201,   200,   124,     4,   163,
    1237,   128,  1239,   203,   163,   124,   175,   163,   176,   128,
     179,   163,   181,   182,   163,   163,   163,  1345,   173,   201,
     124,   200,   200,   200,   128,   201,   180,   200,   200,   139,
     180,     1,   174,   200,   200,   200,   181,   200,   207,   200,
     200,   181,   152,   181,   201,    43,   174,   200,   175,   200,
      43,    43,   179,   163,   181,   182,   175,  1385,   201,   200,
     179,   200,   181,   182,  1237,   201,  1239,   201,   201,   201,
     201,   175,  1557,  1401,   163,   179,   200,   181,   182,   200,
     207,   206,   163,   309,   201,   163,  1378,   156,   207,    10,
      13,     9,   202,   319,    42,    66,   180,   207,   200,   200,
     200,   163,     8,   207,   163,   163,   332,   163,  1345,   108,
     109,   110,   111,   112,   113,   114,   115,   163,   206,   206,
     206,    19,  1283,   163,  1285,   201,    33,    25,   200,   163,
     171,    33,   163,    31,   133,   201,   163,   163,   163,   163,
     200,    14,   174,    41,   143,   144,   145,   176,  1385,   156,
     200,    49,   174,    60,    61,   200,   200,   200,    60,    61,
     174,   200,    43,    37,  1401,   174,    64,    67,   206,   206,
     200,   181,  1345,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   200,    70,   163,   200,
      43,   201,  1385,   439,   440,   200,  1508,   124,  1510,  1511,
      33,   128,   124,   200,   200,   200,   128,   181,  1401,   163,
     456,   457,   458,   459,   460,   138,   163,   200,   200,   200,
     200,   139,   200,   167,   163,   204,    33,    60,    61,   201,
     201,   204,   201,    12,   152,   205,   201,   201,   204,   173,
     173,   201,   201,   201,   201,   163,   201,   165,   175,  1442,
     201,   204,   179,   175,   181,   182,   205,   179,   200,   181,
     182,   201,    53,   509,   201,   201,    79,   199,   199,     1,
     200,   206,    44,   602,    33,   521,   205,   128,   206,  1460,
     207,  1593,   206,    82,   202,   207,   204,   206,   205,   205,
    1651,   124,  1473,   698,  1648,   128,   698,   199,  1610,   101,
    1557,    60,    61,   624,   921,   682,   620,     1,  1501,  1475,
    1622,  1339,  1525,   559,  1478,   481,  1526,  1404,  1526,    53,
    1108,   421,   920,   920,   421,   217,   293,    -1,    -1,    -1,
      33,   579,  1644,   579,   580,    33,    -1,   583,    -1,   585,
      -1,    -1,   175,    -1,    -1,    33,   179,    -1,   181,   182,
     596,   597,   598,   599,   600,   601,    -1,    60,    61,    -1,
      -1,    -1,    60,    61,  1557,   124,    -1,    -1,    -1,   128,
      -1,  1552,    60,    61,   207,    -1,    -1,    -1,    -1,  1560,
      -1,    -1,   628,   629,    -1,    -1,   632,   633,   634,   635,
      -1,   637,    -1,   639,   640,   641,   642,   643,   644,   645,
     646,   647,   648,   649,   650,   651,   652,   653,   654,   655,
     656,   657,   658,    -1,   660,    -1,   175,    -1,  1599,    -1,
     179,   124,   181,   182,    -1,   128,   124,    -1,    -1,    -1,
     128,    -1,    -1,    -1,    -1,  1616,   124,    -1,  1619,    -1,
     128,    -1,   688,    -1,    -1,    -1,    -1,  1628,   207,    -1,
    1631,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1643,    -1,    -1,  1646,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,    -1,    -1,   179,   175,   181,   182,
     726,   179,    -1,   181,   182,    -1,    -1,   175,    -1,    -1,
      -1,   179,    -1,   181,   182,    -1,   742,    -1,    -1,    -1,
      -1,    -1,   748,    -1,   207,    -1,    -1,    -1,    -1,   207,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   763,    -1,   207,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    21,    22,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   795,
      -1,    41,    -1,    -1,   800,    -1,   802,    -1,   804,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   815,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,   825,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   868,    -1,    -1,    -1,   872,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,   139,
     134,   135,   898,    -1,    -1,    -1,   140,   141,   142,    -1,
      -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   917,    -1,   163,    21,    22,    -1,    -1,    -1,    -1,
     926,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,   950,   951,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   197,   198,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,   109,   110,   111,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,   985,
     986,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
     996,   133,    -1,    -1,    -1,    -1,    -1,  1003,    -1,    -1,
    1006,   143,   144,   145,    -1,    -1,  1012,    -1,  1014,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    -1,   174,   140,   141,   142,    -1,    -1,  1044,   146,
    1046,    -1,  1048,    -1,    -1,    -1,    -1,    -1,  1054,    -1,
      -1,    -1,  1058,    -1,    -1,    -1,    -1,    -1,   165,    -1,
    1066,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,  1090,    -1,    -1,    -1,    -1,    -1,
     197,   198,    -1,    -1,  1100,    -1,    -1,   204,    -1,  1105,
      -1,  1107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1156,    -1,    -1,    -1,    -1,    -1,    10,    -1,  1164,  1165,
    1166,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1186,    -1,    -1,  1189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1218,  1219,  1220,  1221,  1222,  1223,  1224,  1225,
    1226,  1227,  1228,  1229,  1230,  1231,  1232,  1233,  1234,  1235,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1245,
      -1,  1247,    -1,    -1,    -1,    -1,    -1,  1253,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,  1291,   140,   141,   142,   143,
     144,   145,   146,    -1,  1300,  1301,  1302,    -1,    -1,    19,
      -1,    -1,  1308,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1322,  1323,    -1,    -1,
     174,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1346,    -1,    -1,   197,   198,  1351,  1352,    -1,  1354,  1355,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    -1,  1393,    -1,    -1,
      -1,  1397,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,  1418,   128,   129,   130,   131,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1448,   163,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    41,    -1,    -1,    -1,  1482,    -1,    -1,    -1,
      49,    -1,   197,   198,    -1,    -1,    -1,    -1,  1494,    -1,
      -1,    -1,  1498,  1499,    -1,    64,  1502,    -1,    -1,  1505,
      -1,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,  1545,
      -1,    -1,    -1,    -1,    -1,  1551,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1560,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,  1592,    -1,    -1,    -1,
      -1,  1597,    -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,
    1606,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1618,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       1,  1627,  1628,    -1,     5,     6,     7,    -1,     9,    10,
      11,  1637,    13,   202,    15,    16,    17,    18,    19,    -1,
    1646,    -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    40,
      -1,    42,    -1,    44,    45,    -1,    -1,    48,    -1,    50,
      51,    52,    -1,    54,    55,    -1,    -1,    58,    59,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,
     151,    -1,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   182,   183,   184,    -1,   186,    -1,    -1,   189,   190,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,   200,
      -1,   202,   203,   204,   205,   206,     1,    -1,    -1,    -1,
       5,     6,     7,    -1,     9,    10,    11,    -1,    13,    -1,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    39,    40,    -1,    42,    -1,    44,
      45,    -1,    -1,    48,    -1,    50,    51,    52,    -1,    54,
      55,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,
     149,   150,   151,    -1,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   182,   183,   184,    -1,   186,    -1,    -1,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,
      -1,   200,    -1,   202,   203,   204,   205,   206,     1,    -1,
      -1,    -1,     5,     6,     7,    -1,     9,    10,    11,    -1,
      13,    -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    29,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    39,    40,    -1,    42,
      -1,    44,    45,    -1,    -1,    48,    -1,    50,    51,    52,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,
     183,   184,    -1,   186,    -1,    -1,   189,   190,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   198,    -1,   200,    -1,   202,
     203,   204,   205,   206,     1,    -1,    -1,    -1,     5,     6,
       7,    -1,     9,    10,    11,    -1,    13,    -1,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    39,    40,    -1,    42,    -1,    44,    45,    -1,
      -1,    48,    -1,    50,    51,    52,    -1,    54,    55,    -1,
      -1,    58,    59,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,   150,   151,    -1,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,   186,
      -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    -1,   200,    -1,   202,   203,   204,   205,   206,
       5,     6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,   183,
     184,    -1,   186,    -1,    -1,   189,   190,    -1,    -1,    -1,
      -1,    -1,     5,     6,   198,    -1,   200,    -1,   202,   203,
      -1,   205,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    52,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,
     183,   184,    -1,   186,    -1,    -1,   189,   190,    -1,    -1,
      -1,    -1,    -1,     5,     6,   198,    -1,   200,    -1,   202,
     203,    13,   205,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
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
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    70,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   182,   183,   184,    -1,   186,    -1,    -1,   189,   190,
      -1,    -1,    -1,    -1,    -1,     5,     6,   198,    -1,   200,
      -1,   202,   203,    -1,   205,    15,    16,    17,    18,    19,
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
       5,     6,   198,    -1,   200,   201,   202,   203,    -1,   205,
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
      -1,     5,     6,   198,    -1,   200,   201,   202,   203,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,
     183,   184,    -1,   186,    -1,    -1,   189,   190,    -1,    -1,
      -1,    -1,    -1,     5,     6,   198,   199,   200,    -1,   202,
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
      -1,    -1,    -1,    -1,     5,     6,   198,   199,   200,    -1,
     202,   203,    -1,   205,    15,    16,    17,    18,    19,    -1,
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
      -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    65,    -1,
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
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,
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
      -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,     5,     6,   198,    -1,   200,   201,   202,   203,
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
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,
      -1,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
     182,   183,   184,    -1,   186,    -1,    -1,   189,   190,    -1,
      -1,    -1,    21,    22,    -1,    -1,   198,    -1,   200,    -1,
     202,   203,    -1,   205,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,
     142,    71,    72,    73,   146,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,    -1,   175,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,   197,   198,   116,   117,   118,
     119,   120,   204,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,   154,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,   198,
      -1,    -1,   201,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
     197,   198,    -1,    -1,   201,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,   197,   198,    -1,    -1,
     201,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,   198,
      -1,    -1,   201,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
     197,   198,    -1,    -1,   201,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,   197,   198,    -1,    -1,
     201,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,   198,
      -1,    -1,   201,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
     197,   198,    -1,    -1,   201,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,   197,   198,    -1,    -1,
     201,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,   198,
      -1,    -1,   201,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
     197,   198,    -1,    -1,   201,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,   197,   198,    -1,    -1,
     201,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,   198,
      -1,    -1,   201,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
     197,   198,    -1,    -1,   201,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,   197,   198,    -1,    -1,
     201,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,   198,
      -1,    -1,   201,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
     197,   198,    -1,    -1,   201,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,   197,   198,    -1,    -1,
     201,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,   198,
      -1,    -1,   201,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
     197,   198,    -1,    -1,   201,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,   197,   198,    -1,    -1,
     201,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     199,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,   199,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    21,    22,    35,    -1,    -1,    -1,    -1,    -1,
     197,   198,   199,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      71,    -1,    73,   146,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,   118,   119,    -1,
      -1,    -1,    -1,    -1,   197,   198,   199,   116,   117,   118,
     119,   120,    21,    22,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    38,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     199,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,    -1,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    21,    22,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,   132,    -1,   134,   135,
      -1,    -1,   138,    -1,   140,   141,   142,    71,    72,    73,
     146,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,    -1,   175,
      -1,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
     154,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,   198,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    21,
      22,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,    -1,
     134,   135,    21,    22,    -1,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,    -1,   134,   135,    21,    22,    -1,    -1,   140,    -1,
     142,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    21,    22,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,    -1,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,    -1,    -1,   126,    -1,   128,   129,   130,   131,
     132,    -1,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   209,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   165,   186,   198,   204,   211,   212,   216,
     225,   227,   228,   232,   277,   282,   311,   392,   400,   407,
     417,   467,   472,   477,    19,    20,   163,   268,   269,   270,
     156,   233,   234,   163,   186,   229,   230,    57,    63,   397,
     398,   163,   202,   214,   478,   468,   473,   139,   163,   299,
      34,    63,   107,   132,   190,   200,   272,   273,   274,   275,
     299,   211,   211,   211,     8,    36,   418,    62,   388,   174,
     173,   176,   173,   229,    22,    57,   185,   197,   231,   163,
     211,   388,   397,   397,   397,   163,   139,   226,   274,   274,
     274,   200,   140,   141,   142,   173,   199,   107,   281,   408,
       5,     6,   414,    57,    63,   389,    15,    16,   156,   161,
     163,   166,   200,   203,   218,   269,   156,   234,   163,   163,
     163,   399,    57,    63,   213,   163,   163,   163,   163,   167,
     224,   201,   270,   274,   274,   274,   274,   165,   238,   239,
      57,    63,   283,   285,    57,    63,   401,   107,   107,    57,
      63,   415,   205,   393,   167,   168,   169,   217,    15,    16,
     156,   161,   163,   218,   266,   267,   203,   231,   174,   190,
     215,   176,   428,   239,   239,   167,   201,   165,   286,   163,
     402,   419,   390,   203,   271,   358,   167,   168,   169,   173,
     201,   163,    19,    25,    31,    41,    49,    64,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   152,   202,   299,   422,   424,   425,   429,   435,   437,
     466,    66,    79,    94,    99,   101,   164,   405,   406,   469,
     474,    35,    71,    73,    75,    76,    77,    78,    79,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      93,    94,    95,    98,    99,   100,   101,   118,   119,   163,
     280,   284,   176,   403,   106,   412,   413,   206,   211,   391,
     269,   203,   163,   384,   387,   266,   180,   180,   180,   200,
     180,   180,   200,   428,   180,   180,   180,   180,   180,   200,
     299,   180,   200,    33,    60,    61,   124,   128,   175,   179,
     182,   207,   198,   434,   177,   164,   479,   205,   205,    21,
      22,    38,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     128,   129,   130,   133,   134,   135,   136,   137,   140,   141,
     142,   143,   144,   145,   146,   175,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   189,   190,   197,   198,
      35,    35,   200,   278,   239,    75,    79,    93,    94,    98,
      99,   100,   101,   423,   406,   163,   239,   358,   239,   269,
     173,   176,   179,   382,   438,   444,   446,     5,     6,    15,
      16,    17,    18,    19,    25,    27,    31,    39,    45,    48,
      51,    55,    65,    68,    69,    80,   102,   103,   104,   118,
     119,   147,   148,   149,   150,   151,   153,   155,   156,   157,
     158,   159,   160,   161,   162,   166,   182,   183,   184,   189,
     190,   198,   200,   202,   203,   205,   223,   225,   293,   299,
     304,   316,   323,   326,   329,   333,   335,   337,   338,   340,
     345,   348,   349,   356,   357,   422,   482,   490,   502,   505,
     517,   521,   448,   442,   163,   180,   450,   452,   454,   456,
     458,   460,   462,   464,   349,   180,   200,   436,   440,   127,
     296,   327,   349,    33,   179,    33,   179,   198,   207,   199,
     349,   198,   207,   435,   205,   470,   475,   163,   163,   163,
     199,    22,   163,   199,   151,   201,   358,   368,   369,   370,
     126,   176,   279,   290,   291,   205,   176,   411,   420,   148,
     163,   383,   386,   239,   163,   435,   127,   133,   174,   381,
     466,   466,   433,   466,   180,   180,   180,   299,   301,   424,
     482,   299,   180,     5,   102,   103,   180,   200,   180,   200,
     200,   180,   180,   200,   180,   200,   180,   200,   180,   180,
     200,    19,   180,   180,   349,   349,   200,   200,   200,   200,
     200,   200,   222,   349,   349,   349,   349,   349,    13,    49,
     296,   154,   163,   327,   483,   485,   203,    13,   518,   200,
     198,   276,   203,   205,   329,   334,    21,    22,   116,   117,
     118,   119,   120,   123,   124,   125,   126,   128,   129,   130,
     131,   132,   134,   135,   140,   141,   142,   146,   175,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   197,
     198,   201,   200,   466,   466,   201,   181,   430,   466,   278,
     466,   278,   466,   278,   163,   371,   372,   466,   163,   374,
     375,   201,   441,   327,   298,   466,   349,   201,   173,   520,
     199,   199,   199,   349,   480,   371,   373,   374,   376,   163,
     108,   109,   110,   111,   112,   113,   114,   115,   133,   143,
     144,   145,   108,   109,   110,   111,   112,   113,   114,   115,
     127,   133,   143,   144,   145,   174,   200,     7,    50,   310,
     204,   173,   204,   201,   466,   466,   205,   409,   299,   204,
     205,   416,   200,    43,   173,   176,   382,   211,   381,   349,
     181,   181,   181,   164,   173,   210,   211,   432,   491,   493,
     302,   200,   180,   200,   324,   180,   180,   180,   512,   327,
     435,   349,   501,   516,   349,   317,   319,   349,   321,   349,
     514,   327,   499,   503,   327,   180,   495,   435,   349,   349,
     349,   349,   349,   349,   169,   170,   217,   200,    13,   199,
     200,   127,   133,   174,   377,   520,   173,   520,   201,   148,
     153,   180,   299,   339,   200,   239,    70,   198,   201,   327,
     485,   275,     4,   332,   203,   295,   276,    19,   154,   163,
     422,    19,   154,   163,   422,   349,   349,   349,   349,   349,
     349,   163,   349,   154,   163,   349,   349,   349,   422,   349,
     349,   349,   349,   349,   349,    22,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   129,   130,   154,
     163,   197,   198,   346,   422,   349,   201,   327,   181,   181,
     163,   426,   181,   279,   181,   279,   181,   279,   176,   181,
     432,   176,   181,   432,   298,   520,   181,   432,   127,   349,
     199,   163,   427,   211,   244,   245,   244,   245,   349,   148,
     163,   378,   379,   421,   370,   370,   370,   295,   163,   394,
     396,   364,   349,   163,   163,   435,   381,   349,   211,   439,
     445,   447,   466,   435,   435,   466,    70,   327,   485,   489,
     163,   349,   466,   506,   508,   510,   435,   520,   181,   432,
     138,   173,   520,   201,   435,   435,   201,   435,   201,   435,
     520,   435,   372,   520,   497,   375,   181,   201,   201,   201,
     201,   201,   201,   349,   148,   163,   200,   257,   200,   349,
     349,   349,   201,   154,   163,   200,   200,   341,   343,   257,
     297,   516,   163,   201,   485,   483,   173,   201,   201,   199,
     200,   278,     1,    26,    28,    29,    38,    40,    44,    52,
      54,    58,    59,    65,   105,   206,   211,   235,   236,   243,
     254,   255,   256,   258,   259,   260,   261,   262,   263,   264,
     265,   294,   300,   305,   306,   307,   308,   309,   311,   315,
     336,   349,   332,   180,   200,   180,   200,   200,   200,   199,
      19,   154,   163,   422,   176,   154,   163,   349,   200,   200,
     154,   163,   349,     1,   200,   199,   173,   201,   449,   443,
     173,   181,   204,   451,   181,   455,   181,   459,   181,   466,
     463,   371,   466,   465,   374,   181,   201,   436,   466,   349,
     174,   210,   395,   404,   211,   371,   471,   374,   476,   201,
     200,    43,   173,   176,   179,   377,   292,   174,   395,   404,
      40,   165,   206,   277,   365,   201,    43,   211,   381,   349,
     211,   181,   181,   181,   485,   201,   201,   201,   181,   432,
     201,   181,   435,   372,   375,   181,   201,   200,   435,   349,
     501,   201,   181,   181,   181,   181,   201,   181,   181,   201,
     435,   181,   332,   200,   176,   220,   200,    43,   163,   313,
      20,   173,   257,   201,   200,   133,   377,   349,   349,   435,
     278,    20,   206,   520,   201,   173,   199,   198,   127,   133,
     163,   174,   179,   330,   331,   279,   127,   349,   290,    61,
     349,   163,   163,   211,   156,    58,   349,   239,   127,   349,
     211,   211,    10,    10,    11,   241,    13,     9,    42,   211,
     211,   211,   211,   211,   211,    66,   312,   211,   108,   109,
     110,   111,   112,   113,   114,   115,   121,   122,   127,   133,
     136,   137,   143,   144,   145,   174,   278,   350,   349,   352,
     349,   201,   327,   349,   180,   200,   349,   200,   199,   349,
     198,   201,   327,   200,   199,   347,   201,   327,   163,   431,
     163,   453,   457,   461,   436,   349,   163,   210,   481,   206,
     206,   349,   163,   163,   466,   349,   206,   349,   394,   410,
     163,     8,   358,   363,   163,   349,   211,   492,   494,   303,
     201,   200,   163,   325,   181,   181,   181,   513,   297,   181,
     318,   320,   322,   515,   500,   504,   181,   496,   200,   239,
     201,   327,   221,   171,   349,   163,   173,   201,   327,   163,
     200,    20,   133,   377,   349,   349,   349,   201,   201,   181,
     279,   327,   201,   483,   163,   163,   200,   163,   163,   173,
     201,   239,   349,    14,   349,   174,   174,   176,   156,   290,
     349,   200,   200,   200,   200,   200,   200,   205,   314,   386,
     349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   279,   435,
     201,   466,   201,   201,   201,   354,   349,   349,   201,   483,
     201,   349,   201,   174,   206,   201,    43,   377,    37,   287,
     206,   174,    57,    63,   361,    67,   362,   211,   211,   200,
     200,   349,   181,   507,   509,   511,   200,   201,   200,   349,
     349,   349,   200,    70,   489,   200,   498,   200,   201,   349,
     290,   201,   219,   201,   163,   201,    43,   313,   327,   349,
     349,   201,   342,   181,   201,   199,   163,   330,   138,   290,
     328,   290,   466,   349,   349,   349,   257,   349,   349,   313,
     385,   239,   181,   181,   466,   201,   201,   199,   201,   349,
     163,   349,   288,   466,    47,   362,    46,   106,   359,   489,
     489,   201,   200,   200,   200,   200,   296,   297,   327,   489,
     200,   489,   201,   167,   204,   163,   201,   201,   133,   377,
     339,   344,   204,   201,   201,   127,   349,   201,   201,    20,
     201,   201,   201,   206,   211,   386,   290,   351,   353,   181,
     201,   205,   211,    33,   360,   359,   361,   200,   483,   486,
     487,   488,   488,   349,   489,   489,   483,   484,   201,   201,
     520,   488,   489,   484,   349,   204,   349,   349,   339,   501,
     349,    12,   242,   239,   327,   239,   239,   176,   382,   355,
     295,   366,   360,   378,   379,   380,   483,   173,   520,   201,
     201,   201,   488,   488,   201,   201,   201,   484,   201,   204,
     519,   349,   519,   243,   305,   306,   307,   308,   349,   211,
     246,   247,   249,   201,   290,   290,   435,   381,   289,   284,
     367,   201,   200,   201,   201,   201,    53,   199,   519,   206,
      12,    23,    24,   237,   240,   205,   243,   239,   381,   349,
     206,   211,   284,   483,   349,   199,   239,   200,   295,   211,
     290,   349,   211,   239,   201,   250,   251,   253,   349,   248,
     211,   290,   205,   243,   201,   206,   295,   211,   239,   287,
     252,   250,   206,   240,   287
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   208,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   210,   210,   211,
     211,   212,   213,   213,   213,   214,   214,   215,   215,   216,
     217,   217,   217,   217,   218,   218,   219,   219,   220,   221,
     220,   222,   222,   222,   223,   224,   224,   226,   225,   227,
     228,   229,   229,   229,   229,   230,   230,   231,   231,   232,
     233,   233,   234,   234,   235,   236,   236,   237,   237,   238,
     238,   239,   239,   240,   240,   240,   241,   241,   242,   242,
     243,   243,   243,   243,   243,   244,   244,   245,   245,   247,
     248,   246,   249,   246,   251,   252,   250,   253,   250,   255,
     254,   256,   257,   257,   257,   257,   257,   257,   257,   259,
     258,   260,   262,   261,   264,   263,   265,   265,   266,   266,
     266,   266,   266,   266,   267,   267,   268,   268,   268,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   270,   270,
     271,   271,   272,   272,   272,   272,   273,   273,   274,   274,
     274,   274,   274,   274,   274,   275,   275,   276,   276,   277,
     277,   278,   278,   278,   279,   279,   279,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   281,   281,   282,
     283,   283,   283,   284,   286,   285,   287,   288,   289,   287,
     291,   292,   290,   293,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   295,   295,   295,   296,   296,   297,   297,
     298,   298,   299,   299,   299,   300,   300,   302,   303,   301,
     301,   304,   304,   304,   304,   304,   304,   305,   306,   307,
     307,   307,   308,   308,   309,   310,   310,   310,   311,   311,
     312,   312,   313,   313,   314,   314,   315,   315,   315,   317,
     318,   316,   319,   320,   316,   321,   322,   316,   324,   325,
     323,   326,   326,   326,   327,   327,   327,   327,   328,   328,
     328,   329,   329,   329,   330,   330,   330,   330,   330,   331,
     331,   332,   332,   333,   334,   334,   335,   335,   335,   335,
     335,   335,   335,   336,   336,   336,   336,   336,   336,   336,
     336,   336,   336,   336,   336,   336,   336,   336,   336,   336,
     336,   336,   337,   337,   338,   338,   339,   339,   340,   341,
     342,   340,   343,   344,   340,   345,   345,   345,   345,   345,
     345,   345,   346,   347,   345,   348,   348,   348,   348,   348,
     348,   348,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   350,   351,
     349,   349,   349,   349,   352,   353,   349,   349,   349,   354,
     355,   349,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   356,   356,   356,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   358,   358,   359,   359,   359,   360,
     360,   361,   361,   361,   362,   362,   363,   364,   364,   364,
     365,   364,   366,   364,   367,   364,   368,   369,   369,   370,
     370,   370,   370,   370,   371,   371,   372,   372,   373,   373,
     373,   374,   375,   375,   376,   376,   376,   377,   377,   378,
     378,   378,   379,   379,   380,   380,   381,   381,   381,   382,
     382,   383,   383,   383,   383,   383,   384,   384,   385,   385,
     385,   386,   386,   386,   387,   387,   387,   388,   388,   389,
     389,   389,   390,   390,   391,   390,   392,   393,   392,   394,
     394,   395,   395,   396,   396,   396,   397,   397,   397,   399,
     398,   400,   401,   401,   401,   402,   403,   403,   404,   404,
     405,   405,   406,   406,   408,   409,   410,   407,   411,   411,
     412,   412,   413,   414,   414,   414,   414,   415,   415,   415,
     416,   416,   418,   419,   420,   417,   421,   421,   421,   421,
     421,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   423,   423,
     423,   423,   423,   423,   423,   423,   424,   425,   425,   425,
     426,   426,   426,   427,   427,   427,   427,   427,   428,   428,
     428,   428,   428,   429,   430,   431,   429,   432,   432,   433,
     433,   434,   434,   434,   434,   435,   435,   436,   436,   437,
     437,   437,   437,   438,   439,   437,   437,   437,   437,   440,
     437,   441,   437,   437,   437,   437,   437,   437,   437,   437,
     437,   437,   437,   437,   437,   442,   443,   437,   437,   444,
     445,   437,   446,   447,   437,   448,   449,   437,   437,   450,
     451,   437,   452,   453,   437,   437,   454,   455,   437,   456,
     457,   437,   437,   458,   459,   437,   460,   461,   437,   462,
     463,   437,   464,   465,   437,   466,   466,   466,   468,   469,
     470,   471,   467,   473,   474,   475,   476,   472,   478,   479,
     480,   481,   477,   482,   482,   482,   482,   482,   483,   483,
     483,   483,   483,   483,   483,   483,   484,   484,   485,   486,
     486,   487,   487,   488,   488,   489,   489,   491,   492,   490,
     493,   494,   490,   495,   496,   490,   497,   498,   490,   499,
     500,   490,   501,   501,   502,   503,   504,   502,   505,   506,
     507,   505,   508,   509,   505,   510,   511,   505,   505,   512,
     513,   505,   505,   514,   515,   505,   516,   516,   518,   517,
     517,   517,   517,   519,   519,   520,   520,   521,   521,   521
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     3,
       3,     2,     2,     2,     2,     2,     2,     1,     1,     1,
       1,     2,     0,     1,     1,     1,     1,     0,     2,     5,
       1,     1,     2,     2,     3,     2,     0,     2,     0,     0,
       3,     0,     2,     5,     3,     1,     2,     0,     4,     2,
       2,     1,     2,     3,     3,     2,     4,     0,     1,     2,
       1,     3,     1,     3,     3,     3,     2,     1,     1,     1,
       2,     0,     1,     0,     3,     7,     1,     1,     0,     2,
       1,     1,     1,     1,     1,     1,     2,     0,     1,     0,
       0,     6,     0,     3,     0,     0,     6,     0,     3,     0,
       8,     7,     1,     4,     3,     3,     3,     5,     5,     0,
       9,     3,     0,     7,     0,     7,     4,     4,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     3,
       3,     5,     3,     3,     3,     3,     1,     5,     1,     3,
       3,     4,     1,     1,     1,     1,     1,     4,     1,     2,
       3,     3,     3,     3,     2,     1,     3,     0,     3,     0,
       4,     0,     2,     3,     0,     2,     2,     1,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     3,     2,     2,     3,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     3,     2,     2,     2,     2,     2,     3,     3,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     1,     4,
       0,     1,     1,     3,     0,     5,     0,     0,     0,     6,
       0,     0,     6,     2,     1,     2,     2,     1,     1,     1,
       1,     2,     1,     2,     2,     2,     2,     1,     1,     1,
       2,     2,     2,     0,     2,     2,     0,     2,     0,     2,
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
       3,     3,     6,     8,     5,     6,     1,     4,     3,     0,
       0,     8,     0,     0,     9,     3,     4,     5,     6,     8,
       5,     6,     0,     0,     5,     3,     4,     4,     5,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     4,     3,     4,     5,     4,     5,     3,     4,
       1,     1,     2,     4,     4,     1,     3,     5,     0,     0,
       8,     3,     3,     3,     0,     0,     8,     3,     4,     0,
       0,     9,     4,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     1,     4,     7,     8,     7,     4,     4,
       4,     4,     4,     1,     6,     7,     6,     6,     7,     7,
       6,     7,     6,     6,     0,     1,     0,     1,     1,     0,
       1,     0,     1,     1,     0,     1,     5,     0,     2,     6,
       0,     4,     0,     9,     0,    11,     3,     3,     4,     1,
       1,     3,     3,     3,     1,     3,     1,     3,     0,     1,
       3,     3,     1,     3,     0,     1,     3,     1,     1,     1,
       2,     3,     3,     5,     1,     1,     1,     1,     1,     0,
       1,     1,     4,     3,     3,     5,     1,     3,     0,     2,
       2,     4,     6,     5,     4,     6,     5,     0,     1,     0,
       1,     1,     0,     2,     0,     4,     6,     0,     6,     1,
       3,     1,     2,     0,     1,     3,     0,     1,     1,     0,
       5,     3,     0,     1,     1,     1,     0,     2,     0,     1,
       1,     2,     0,     1,     0,     0,     0,    13,     0,     2,
       0,     1,     3,     1,     1,     2,     2,     0,     1,     1,
       1,     3,     0,     0,     0,     9,     1,     4,     3,     3,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       1,     3,     3,     0,     1,     3,     3,     5,     0,     2,
       2,     2,     2,     4,     0,     0,     7,     1,     1,     1,
       3,     3,     2,     4,     3,     1,     2,     0,     4,     1,
       1,     1,     1,     0,     0,     6,     4,     4,     3,     0,
       6,     0,     7,     4,     2,     2,     3,     2,     3,     2,
       2,     3,     3,     3,     2,     0,     0,     6,     2,     0,
       0,     6,     0,     0,     6,     0,     0,     6,     1,     0,
       0,     6,     0,     0,     7,     1,     0,     0,     6,     0,
       0,     7,     1,     0,     0,     6,     0,     0,     7,     0,
       0,     6,     0,     0,     6,     1,     3,     3,     0,     0,
       0,     0,    12,     0,     0,     0,     0,    12,     0,     0,
       0,     0,    13,     1,     1,     1,     1,     1,     3,     3,
       5,     5,     6,     6,     8,     8,     0,     1,     2,     3,
       5,     1,     2,     1,     0,     0,     1,     0,     0,    10,
       0,     0,    10,     0,     0,    10,     0,     0,    11,     0,
       0,     7,     3,     1,     5,     0,     0,    10,     3,     0,
       0,    11,     0,     0,    11,     0,     0,    10,     5,     0,
       0,     9,     5,     0,     0,    10,     1,     3,     0,     5,
       5,     7,     9,     0,     3,     0,     1,    11,    12,    11
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
            { deleteNameExprList(((*yyvaluep).pNameExprList)); }
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

    case YYSYMBOL_optional_expr_list_in_braces: /* optional_expr_list_in_braces  */
            { delete ((*yyvaluep).pExpression); }
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
                CompilationError::module_already_has_a_name);
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
        if ( err ) das2_yyerror(scanner,"invalid escape sequence",tokAt(scanner,(yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt(scanner,(yylsp[0])),esconst);
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
            call_fmt->arguments.push_back(make_smart<ExprConstString>(tokAt(scanner,(yylsp[-1])),":" + *(yyvsp[-1].s)));
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
                yyextra->g_Program->options.push_back(opt);
            } else {
                das2_yyerror(scanner,"option " + opt.name + " is not allowed here",
                    tokAt(scanner,(yylsp[0])), CompilationError::invalid_option);
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

  case 53: /* require_module_name: require_module_name '.' "name"  */
                                                {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 54: /* require_module_name: require_module_name '/' "name"  */
                                                {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 55: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 56: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 57: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 58: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 62: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 63: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 64: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 65: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 66: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 67: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 68: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 73: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 74: /* expression_else: "else" optional_emit_semis expression_else_block  */
                                                                     { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 75: /* expression_else: elif_or_static_elif '(' expr ')' optional_emit_semis expression_else_block expression_else  */
                                                                                                                           {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),(yyvsp[-1].pExpression),(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-6].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 76: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 77: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 78: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 79: /* expression_else_one_liner: "else" expression_if_one_liner  */
                                                      {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 80: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 81: /* expression_if_one_liner: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 82: /* expression_if_one_liner: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 83: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 84: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 89: /* $@3: %empty  */
        {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 90: /* $@4: %empty  */
                                      {
        yyextra->pop_nesteds();
    }
    break;

  case 91: /* expression_if_block: $@3 '{' expressions $@4 '}' expression_block_finally  */
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

  case 92: /* $@5: %empty  */
       {
        yyextra->das_keyword = false;
    }
    break;

  case 93: /* expression_if_block: $@5 expression_if_one_liner SEMICOLON  */
                                               {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 94: /* $@6: %empty  */
        {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 95: /* $@7: %empty  */
                                      {
        yyextra->pop_nesteds();
    }
    break;

  case 96: /* expression_else_block: $@6 '{' expressions $@7 '}' expression_block_finally  */
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

  case 97: /* $@8: %empty  */
       {
        yyextra->das_keyword = false;
    }
    break;

  case 98: /* expression_else_block: $@8 expression_if_one_liner SEMICOLON  */
                                               {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 99: /* $@9: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 100: /* expression_if_then_else: $@9 if_or_static_if '(' expr ')' optional_emit_semis expression_if_block expression_else  */
                                                                                                                   {
        yyextra->das_keyword = false;
        auto blk = (yyvsp[-1].pExpression)->rtti_isBlock() ? static_cast<ExprBlock *>((yyvsp[-1].pExpression)) : ast_wrapInBlock((yyvsp[-1].pExpression));
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),blk,(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-6].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 101: /* expression_if_then_else_oneliner: expression_if_one_liner "if" '(' expr ')' expression_else_one_liner SEMICOLON  */
                                                                                                                      {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ast_wrapInBlock((yyvsp[-6].pExpression)),(yyvsp[-1].pExpression) ? ast_wrapInBlock((yyvsp[-1].pExpression)) : nullptr);
    }
    break;

  case 102: /* for_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 103: /* for_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 104: /* for_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 105: /* for_variable_name_with_pos_list: '(' tuple_expansion ')'  */
                                       {
        auto pSL = new vector<VariableNameAndPosition>();
        for ( auto & x : *(yyvsp[-1].pNameList) ) {
            das_checkName(scanner,x,tokAt(scanner,(yylsp[-1])));
        }
        pSL->push_back(VariableNameAndPosition((yyvsp[-1].pNameList),tokAt(scanner,(yylsp[-1]))));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 106: /* for_variable_name_with_pos_list: for_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 107: /* for_variable_name_with_pos_list: for_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 108: /* for_variable_name_with_pos_list: for_variable_name_with_pos_list ',' '(' tuple_expansion ')'  */
                                                                                 {
        for ( auto & x : *(yyvsp[-1].pNameList) ) {
            das_checkName(scanner,x,tokAt(scanner,(yylsp[-1])));
        }
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition((yyvsp[-1].pNameList),tokAt(scanner,(yylsp[-1]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
    }
    break;

  case 109: /* $@10: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 110: /* expression_for_loop: $@10 "for" '(' for_variable_name_with_pos_list "in" expr_list ')' optional_emit_semis expression_block  */
                                                                                                                                     {
        yyextra->das_keyword = false;
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-5].pNameWithPosList),(yyvsp[-3].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 111: /* expression_unsafe: "unsafe" optional_emit_semis expression_block  */
                                                                    {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-2])));
        pUnsafe->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 112: /* $@11: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 113: /* expression_while_loop: $@11 "while" '(' expr ')' optional_emit_semis expression_block  */
                                                                                         {
        yyextra->das_keyword = false;
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-5])));
        pWhile->cond = (yyvsp[-3].pExpression);
        pWhile->body = (yyvsp[0].pExpression);
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 114: /* $@12: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 115: /* expression_with: $@12 "with" '(' expr ')' optional_emit_semis expression_block  */
                                                                                   {
        yyextra->das_keyword = false;
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-5])));
        pWith->with = (yyvsp[-3].pExpression);
        pWith->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pWith;
    }
    break;

  case 116: /* expression_with_alias: "assume" "name" '=' expr  */
                                                      {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-3])), *(yyvsp[-2].s), ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-2].s);
    }
    break;

  case 117: /* expression_with_alias: "typedef" "name" '=' type_declaration  */
                                                                {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-3])), *(yyvsp[-2].s), TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
    break;

  case 118: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 119: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 120: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 121: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 122: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 123: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 124: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 125: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 126: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 127: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 128: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 129: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 130: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 131: /* annotation_argument: annotation_argument_name '=' '@' '@' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[0].s); delete (yyvsp[-4].s); }
    break;

  case 132: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 133: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 134: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 135: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 136: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 137: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 138: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 139: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 140: /* metadata_argument_list: '@' annotation_argument optional_emit_semis  */
                                                         {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[-1].aa));
    }
    break;

  case 141: /* metadata_argument_list: metadata_argument_list '@' annotation_argument optional_emit_semis  */
                                                                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-3].aaList),(yyvsp[-1].aa));
    }
    break;

  case 142: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 143: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 144: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 145: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 146: /* annotation_declaration_basic: annotation_declaration_name  */
                                          {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[0]));
        if ( yyextra->g_Access->isAnnotationAllowed(*(yyvsp[0].s), yyextra->g_Program->thisModuleName) ) {
            if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
                (yyval.fa)->annotation = ann;
            } else {
                (yyval.fa)->annotation = new Annotation(*(yyvsp[0].s));
                das2_yyerror(scanner,"annotation " + *(yyvsp[0].s) + " is not found",
                            tokAt(scanner,(yylsp[0])), CompilationError::invalid_annotation);
            }
        } else {
            (yyval.fa)->annotation = new Annotation(*(yyvsp[0].s));
            das2_yyerror(scanner,"annotation " + *(yyvsp[0].s) + " is not allowed here",
                        tokAt(scanner,(yylsp[0])), CompilationError::invalid_annotation);
        }
        delete (yyvsp[0].s);
    }
    break;

  case 147: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
                                                                                 {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[-3]));
        if ( yyextra->g_Access->isAnnotationAllowed(*(yyvsp[-3].s), yyextra->g_Program->thisModuleName) ) {
            if ( auto ann = findAnnotation(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3]))) ) {
                (yyval.fa)->annotation = ann;
            } else {
                (yyval.fa)->annotation = new Annotation(*(yyvsp[-3].s));
                das2_yyerror(scanner,"annotation " + *(yyvsp[-3].s) + " is not found",
                            tokAt(scanner,(yylsp[-3])), CompilationError::invalid_annotation);
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

  case 148: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 149: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            delete (yyvsp[0].fa); (yyvsp[0].fa) = nullptr;
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 150: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
                                                                              {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation);
            delete (yyvsp[-2].fa); (yyvsp[-2].fa) = nullptr;
        }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            delete (yyvsp[-2].fa); (yyvsp[-2].fa) = nullptr;
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::And,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 151: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
                                                                            {
        if ( !(yyvsp[-2].fa)->annotation || !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation);
            delete (yyvsp[-2].fa); (yyvsp[-2].fa) = nullptr;
        }
        if ( !(yyvsp[0].fa)->annotation || !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            delete (yyvsp[-2].fa); (yyvsp[-2].fa) = nullptr;
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Or,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 152: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
                                                                              {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation);
            delete (yyvsp[-2].fa); (yyvsp[-2].fa) = nullptr;
        }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            delete (yyvsp[-2].fa); (yyvsp[-2].fa) = nullptr;
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Xor,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 153: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 154: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 155: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 156: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 157: /* optional_annotation_list: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 158: /* optional_annotation_list: '[' annotation_list ']'  */
                                       { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 159: /* optional_annotation_list_with_emit_semis: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 160: /* optional_annotation_list_with_emit_semis: '[' annotation_list ']' optional_emit_semis  */
                                                          { (yyval.faList) = (yyvsp[-2].faList); }
    break;

  case 161: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 162: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 163: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 164: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 165: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 166: /* optional_function_type: "->" type_declaration  */
                                           {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 167: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 168: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 169: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 170: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 171: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 172: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 173: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 174: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 175: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 176: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 177: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 178: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 179: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 180: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 181: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 182: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 183: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 184: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 185: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 186: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 187: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 188: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 189: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 190: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 191: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 192: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 193: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 194: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 195: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 196: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 197: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 198: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 199: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 200: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 201: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 202: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 203: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 204: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 205: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 206: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 207: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 208: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 209: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 210: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 211: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 212: /* function_name: "operator" '[' ']' '='  */
                                 { (yyval.s) = new string("[]="); }
    break;

  case 213: /* function_name: "operator" '[' ']' "<-"  */
                                    { (yyval.s) = new string("[]<-"); }
    break;

  case 214: /* function_name: "operator" '[' ']' ":="  */
                                      { (yyval.s) = new string("[]:="); }
    break;

  case 215: /* function_name: "operator" '[' ']' "+="  */
                                     { (yyval.s) = new string("[]+="); }
    break;

  case 216: /* function_name: "operator" '[' ']' "-="  */
                                     { (yyval.s) = new string("[]-="); }
    break;

  case 217: /* function_name: "operator" '[' ']' "*="  */
                                     { (yyval.s) = new string("[]*="); }
    break;

  case 218: /* function_name: "operator" '[' ']' "/="  */
                                     { (yyval.s) = new string("[]/="); }
    break;

  case 219: /* function_name: "operator" '[' ']' "%="  */
                                     { (yyval.s) = new string("[]%="); }
    break;

  case 220: /* function_name: "operator" '[' ']' "&="  */
                                     { (yyval.s) = new string("[]&="); }
    break;

  case 221: /* function_name: "operator" '[' ']' "|="  */
                                     { (yyval.s) = new string("[]|="); }
    break;

  case 222: /* function_name: "operator" '[' ']' "^="  */
                                     { (yyval.s) = new string("[]^="); }
    break;

  case 223: /* function_name: "operator" '[' ']' "&&="  */
                                        { (yyval.s) = new string("[]&&="); }
    break;

  case 224: /* function_name: "operator" '[' ']' "||="  */
                                        { (yyval.s) = new string("[]||="); }
    break;

  case 225: /* function_name: "operator" '[' ']' "^^="  */
                                        { (yyval.s) = new string("[]^^="); }
    break;

  case 226: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 227: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 228: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 229: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 230: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 231: /* function_name: "operator" '.' "name" "+="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`+="); delete (yyvsp[-1].s); }
    break;

  case 232: /* function_name: "operator" '.' "name" "-="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`-="); delete (yyvsp[-1].s); }
    break;

  case 233: /* function_name: "operator" '.' "name" "*="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`*="); delete (yyvsp[-1].s); }
    break;

  case 234: /* function_name: "operator" '.' "name" "/="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`/="); delete (yyvsp[-1].s); }
    break;

  case 235: /* function_name: "operator" '.' "name" "%="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`%="); delete (yyvsp[-1].s); }
    break;

  case 236: /* function_name: "operator" '.' "name" "&="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&="); delete (yyvsp[-1].s); }
    break;

  case 237: /* function_name: "operator" '.' "name" "|="  */
                                          { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`|="); delete (yyvsp[-1].s); }
    break;

  case 238: /* function_name: "operator" '.' "name" "^="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^="); delete (yyvsp[-1].s); }
    break;

  case 239: /* function_name: "operator" '.' "name" "&&="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&&="); delete (yyvsp[-1].s); }
    break;

  case 240: /* function_name: "operator" '.' "name" "||="  */
                                            { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`||="); delete (yyvsp[-1].s); }
    break;

  case 241: /* function_name: "operator" '.' "name" "^^="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^^="); delete (yyvsp[-1].s); }
    break;

  case 242: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 243: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 244: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 245: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 246: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 247: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 248: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 249: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 250: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 251: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 252: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 253: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 254: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 255: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 256: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 257: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 258: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 259: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 260: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 261: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 262: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 263: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 264: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 265: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 266: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 267: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 268: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 269: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 270: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 271: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 272: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 273: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 274: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 275: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 276: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 277: /* optional_template: %empty  */
                                        { (yyval.b) = false; }
    break;

  case 278: /* optional_template: "template"  */
                                        { (yyval.b) = true; }
    break;

  case 279: /* global_function_declaration: optional_annotation_list_with_emit_semis "def" optional_template function_declaration  */
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
                        CompilationError::function_already_declared);
            }
        }
        (yyvsp[0].pFuncDecl)->delRef();
    }
    break;

  case 280: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 281: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 282: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 283: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 284: /* $@13: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 285: /* function_declaration: optional_public_or_private_function $@13 function_declaration_header optional_emit_semis expression_block  */
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

  case 286: /* expression_block_finally: %empty  */
        {
        (yyval.pExpression) = nullptr;
    }
    break;

  case 287: /* $@14: %empty  */
                  {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 288: /* $@15: %empty  */
                             {
        yyextra->pop_nesteds();
    }
    break;

  case 289: /* expression_block_finally: "finally" $@14 '{' expressions $@15 '}'  */
          {
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 290: /* $@16: %empty  */
        {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 291: /* $@17: %empty  */
                                      {
        yyextra->pop_nesteds();
    }
    break;

  case 292: /* expression_block: $@16 '{' expressions $@17 '}' expression_block_finally  */
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

  case 293: /* expr_call_pipe: expr_call expr_full_block_assumed_piped  */
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

  case 294: /* expression_any: SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 295: /* expression_any: expr_assign SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 296: /* expression_any: expression_delete SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 297: /* expression_any: expression_let  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 298: /* expression_any: expression_while_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 299: /* expression_any: expression_unsafe  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 300: /* expression_any: expression_with  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 301: /* expression_any: expression_with_alias SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 302: /* expression_any: expression_for_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 303: /* expression_any: expression_break SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 304: /* expression_any: expression_continue SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 305: /* expression_any: expression_return SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 306: /* expression_any: expression_yield SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 307: /* expression_any: expression_if_then_else  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 308: /* expression_any: expression_if_then_else_oneliner  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 309: /* expression_any: expression_try_catch  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 310: /* expression_any: expression_label SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 311: /* expression_any: expression_goto SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 312: /* expression_any: "pass" SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 313: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 314: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 315: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 316: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 317: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 318: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 319: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 320: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 321: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 322: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 323: /* name_in_namespace: "name" "::" "name"  */
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

  case 324: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 325: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 326: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 327: /* $@18: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 328: /* $@19: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 329: /* new_type_declaration: '<' $@18 type_declaration '>' $@19  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 330: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 331: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 332: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 333: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 334: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 335: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 336: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 337: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 338: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 339: /* expression_return: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 340: /* expression_return: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 341: /* expression_return: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 342: /* expression_yield: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 343: /* expression_yield: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 344: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 345: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 346: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 347: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 348: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 349: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 350: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 351: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 352: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 353: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 354: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 355: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr SEMICOLON  */
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

  case 356: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 357: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 358: /* expression_let: kwd_let optional_in_scope '{' variable_declaration_list '}'  */
                                                                               {
        (yyval.pExpression) = ast_LetList(scanner,(yyvsp[-4].b),(yyvsp[-3].b),*(yyvsp[-1].pVarDeclList),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 359: /* $@20: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 360: /* $@21: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 361: /* expr_cast: "cast" '<' $@20 type_declaration_no_options '>' $@21 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
    }
    break;

  case 362: /* $@22: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 363: /* $@23: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 364: /* expr_cast: "upcast" '<' $@22 type_declaration_no_options '>' $@23 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 365: /* $@24: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 366: /* $@25: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 367: /* expr_cast: "reinterpret" '<' $@24 type_declaration_no_options '>' $@25 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 368: /* $@26: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 369: /* $@27: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 370: /* expr_type_decl: "type" '<' $@26 type_declaration '>' $@27  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 371: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 372: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 373: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" c_or_s "name" '>' '(' expr ')'  */
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

  case 374: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 375: /* expr_list: "<-" expr  */
                             {
            (yyval.pExpression) = ast_makeMoveArgument(scanner, (yyvsp[0].pExpression), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 376: /* expr_list: expr_list ',' expr  */
                                        {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 377: /* expr_list: expr_list ',' "<-" expr  */
                                                   {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-3])),(yyvsp[-3].pExpression),ast_makeMoveArgument(scanner, (yyvsp[0].pExpression), tokAt(scanner,(yylsp[0]))));
    }
    break;

  case 378: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 379: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 380: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 381: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 382: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 383: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 384: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 385: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 386: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 387: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 388: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 389: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 390: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 391: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 392: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 393: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis block_or_simple_block  */
                                                                                                                {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 394: /* expr_full_block_assumed_piped: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis expression_block  */
                                                                                                           {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 395: /* expr_full_block_assumed_piped: '{' expressions '}'  */
                                   {
        (yyval.pExpression) = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,new TypeDecl(Type::autoinfer),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 396: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 397: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 398: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 399: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 400: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 401: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 402: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 403: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 404: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 405: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 406: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 407: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 408: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 409: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 410: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 411: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 412: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 413: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 414: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 415: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 416: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 417: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 418: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 419: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 420: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 421: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 422: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 423: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 424: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 425: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 426: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 427: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 428: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 429: /* $@28: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 430: /* $@29: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 431: /* func_addr_expr: '@' '@' '<' $@28 type_declaration_no_options '>' $@29 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 432: /* $@30: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 433: /* $@31: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 434: /* func_addr_expr: '@' '@' '<' $@30 optional_function_argument_list optional_function_type '>' $@31 func_addr_name  */
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

  case 435: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 436: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 437: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 438: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 439: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
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

  case 440: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 441: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 442: /* $@32: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 443: /* $@33: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 444: /* expr_field: expr '.' $@32 error $@33  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 445: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 446: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 447: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 448: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-4])),*(yyvsp[-4].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 449: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 450: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 451: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 452: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 453: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 454: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 455: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 456: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 457: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 458: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 459: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 460: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 461: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 462: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",(yyvsp[0].pExpression)); }
    break;

  case 463: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",(yyvsp[0].pExpression)); }
    break;

  case 464: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",(yyvsp[0].pExpression)); }
    break;

  case 465: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",(yyvsp[0].pExpression)); }
    break;

  case 466: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 467: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 468: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 469: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 470: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 471: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 472: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 473: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 474: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 475: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 476: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 477: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 478: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 479: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 480: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 481: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 482: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 483: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 484: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 485: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 486: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 487: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 488: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", (yyvsp[0].pExpression)); }
    break;

  case 489: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", (yyvsp[0].pExpression)); }
    break;

  case 490: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 491: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 492: /* expr: '(' expr_list optional_comma ')'  */
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

  case 493: /* expr: '(' make_struct_single ')'  */
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

  case 494: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 495: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 496: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 497: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 498: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 499: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 500: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 501: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 502: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 503: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 504: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 505: /* expr: expr_generator  */
                                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 506: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 507: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 508: /* $@34: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 509: /* $@35: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 510: /* expr: expr "is" "type" '<' $@34 type_declaration_no_options '>' $@35  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 511: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 512: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 513: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 514: /* $@36: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 515: /* $@37: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 516: /* expr: expr "as" "type" '<' $@36 type_declaration '>' $@37  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 517: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 518: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 519: /* $@38: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 520: /* $@39: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 521: /* expr: expr '?' "as" "type" '<' $@38 type_declaration '>' $@39  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 522: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 523: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 524: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 525: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 526: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 527: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 528: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 529: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 530: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 531: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 532: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 533: /* expr: expr_call_pipe  */
                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 534: /* expr: "unsafe" '(' expr ')'  */
                                         {
            (yyvsp[-1].pExpression)->alwaysSafe = true;
            (yyvsp[-1].pExpression)->userSaidItsSafe = true;
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    break;

  case 535: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 536: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 537: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list optional_emit_semis expression_block  */
                                                                                                                                                  {
        auto closure = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),(yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->returnType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),closure,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 538: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 539: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 540: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 541: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 542: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 543: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 544: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 545: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 546: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 547: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 548: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 549: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 550: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 551: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 552: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 553: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 554: /* optional_field_annotation: %empty  */
                                      { (yyval.aaList) = nullptr; }
    break;

  case 555: /* optional_field_annotation: metadata_argument_list  */
                                      { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 556: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 557: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 558: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 559: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 560: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 561: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 562: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 563: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 564: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 565: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 566: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 567: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 568: /* struct_variable_declaration_list: struct_variable_declaration_list "new line, semicolon"  */
                                                                 { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 569: /* struct_variable_declaration_list: struct_variable_declaration_list "typedef" "name" '=' type_declaration SEMICOLON  */
                                                                                                                {
        (yyval.pVarDeclList) = (yyvsp[-5].pVarDeclList);
        ast_structureAlias(scanner,(yyvsp[-3].s),(yyvsp[-1].pTypeDecl),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 570: /* $@40: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 571: /* struct_variable_declaration_list: struct_variable_declaration_list $@40 structure_variable_declaration SEMICOLON  */
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

  case 572: /* $@41: %empty  */
                                                                                                                     {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 573: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable "abstract" optional_constant $@41 function_declaration_header SEMICOLON  */
                                                          {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 574: /* $@42: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 575: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@42 function_declaration_header optional_emit_semis expression_block  */
                                                                                            {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-10].pVarDeclList),(yyvsp[-9].faList),(yyvsp[-6].b),(yyvsp[-7].b),(yyvsp[-5].i),(yyvsp[-4].b),(yyvsp[-2].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-8]),(yylsp[0])),tokAt(scanner,(yylsp[-9])));
            }
    break;

  case 576: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
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

  case 577: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
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

  case 578: /* function_argument_declaration_type: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))));
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 579: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 580: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 581: /* function_argument_list: function_argument_declaration_no_type ';' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 582: /* function_argument_list: function_argument_declaration_type ';' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 583: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 584: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 585: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 586: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 587: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 588: /* tuple_alias_type_list: %empty  */
      {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 589: /* tuple_alias_type_list: tuple_type  */
                       {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 590: /* tuple_alias_type_list: tuple_alias_type_list semis tuple_type  */
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

  case 591: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 592: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 593: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 594: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 595: /* variant_alias_type_list: variant_type  */
                         {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 596: /* variant_alias_type_list: variant_alias_type_list semis variant_type  */
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

  case 597: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 598: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 599: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 600: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 601: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 602: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 603: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 604: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 605: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 606: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 607: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 608: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 609: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 610: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 611: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 612: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 613: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 614: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 615: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 616: /* global_let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 617: /* global_let_variable_name_with_pos_list: global_let_variable_name_with_pos_list ',' "name"  */
                                                                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 618: /* variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 619: /* variable_declaration_list: variable_declaration_list SEMICOLON  */
                                                  {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 620: /* variable_declaration_list: variable_declaration_list let_variable_declaration  */
                                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 621: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 622: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 623: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 624: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                         {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 625: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                               {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 626: /* global_let_variable_declaration: global_let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 627: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 628: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 629: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 630: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 631: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 632: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 633: /* global_variable_declaration_list: global_variable_declaration_list SEMICOLON  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 634: /* $@43: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 635: /* global_variable_declaration_list: global_variable_declaration_list $@43 optional_field_annotation let_variable_declaration  */
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

  case 636: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 637: /* $@44: %empty  */
                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 638: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@44 optional_field_annotation global_let_variable_declaration  */
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

  case 639: /* enum_expression: "name"  */
                   {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        delete (yyvsp[0].s);
    }
    break;

  case 640: /* enum_expression: "name" '=' expr  */
                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[-2].s),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-2])));
        delete (yyvsp[-2].s);
    }
    break;

  case 643: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 644: /* enum_list: enum_expression  */
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

  case 645: /* enum_list: enum_list commas enum_expression  */
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

  case 646: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 647: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 648: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 649: /* $@45: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 650: /* single_alias: optional_public_or_private_alias "name" $@45 '=' type_declaration  */
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

  case 652: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 653: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 654: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 655: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 656: /* optional_enum_basic_type_declaration: %empty  */
        {
        (yyval.type) = Type::tInt;
    }
    break;

  case 657: /* optional_enum_basic_type_declaration: ':' enum_basic_type_declaration  */
                                              {
        (yyval.type) = (yyvsp[0].type);
    }
    break;

  case 664: /* $@46: %empty  */
                                                                     {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 665: /* $@47: %empty  */
                                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 666: /* $@48: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 667: /* enum_declaration: optional_annotation_list_with_emit_semis "enum" $@46 optional_public_or_private_enum enum_name optional_enum_basic_type_declaration optional_emit_commas '{' $@47 enum_list optional_commas $@48 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-8].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-12].faList),tokAt(scanner,(yylsp[-12])),(yyvsp[-9].b),(yyvsp[-8].pEnum),(yyvsp[-3].pEnum),(yyvsp[-7].type));
    }
    break;

  case 668: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 669: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 670: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 671: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 672: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 673: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 674: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 675: /* class_or_struct: "class" "template"  */
                                  { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 676: /* class_or_struct: "struct" "template"  */
                                  { (yyval.i) = CorS_StructTemplate; }
    break;

  case 677: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 678: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 679: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 680: /* optional_struct_variable_declaration_list: ';'  */
            {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 681: /* optional_struct_variable_declaration_list: '{' struct_variable_declaration_list '}'  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 682: /* $@49: %empty  */
                                                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 683: /* $@50: %empty  */
                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 684: /* $@51: %empty  */
                                             {
        if ( (yyvsp[-1].pStructure) ) {
            (yyvsp[-1].pStructure)->isClass = (yyvsp[-4].i)==CorS_Class || (yyvsp[-4].i)==CorS_ClassTemplate;
            (yyvsp[-1].pStructure)->isTemplate = (yyvsp[-4].i)==CorS_ClassTemplate || (yyvsp[-4].i)==CorS_StructTemplate;
            (yyvsp[-1].pStructure)->privateStructure = !(yyvsp[-3].b);
        }
    }
    break;

  case 685: /* structure_declaration: optional_annotation_list_with_emit_semis $@49 class_or_struct optional_public_or_private_structure $@50 structure_name optional_emit_semis $@51 optional_struct_variable_declaration_list  */
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

  case 686: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 687: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 688: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 689: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 690: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 691: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 692: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 693: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 694: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 695: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 696: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 697: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 698: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 699: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 700: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 701: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 702: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 703: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 704: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 705: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 706: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 707: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 708: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 709: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 710: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 711: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 712: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 713: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 714: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 715: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 716: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 717: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 718: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 719: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 720: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 721: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 722: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 723: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 724: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 725: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 726: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 727: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 728: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 729: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 730: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 731: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 732: /* bitfield_bits: bitfield_bits ',' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 733: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<tuple<string,Expression *>>();
        (yyval.pNameExprList) = pSL;

    }
    break;

  case 734: /* bitfield_alias_bits: "name"  */
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

  case 735: /* bitfield_alias_bits: "name" '=' expr  */
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

  case 736: /* bitfield_alias_bits: bitfield_alias_bits commas "name"  */
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

  case 737: /* bitfield_alias_bits: bitfield_alias_bits commas "name" '=' expr  */
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

  case 738: /* bitfield_basic_type_declaration: %empty  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 739: /* bitfield_basic_type_declaration: ':' "uint8"  */
                             { (yyval.type) = Type::tBitfield8; }
    break;

  case 740: /* bitfield_basic_type_declaration: ':' "uint16"  */
                             { (yyval.type) = Type::tBitfield16; }
    break;

  case 741: /* bitfield_basic_type_declaration: ':' "uint"  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 742: /* bitfield_basic_type_declaration: ':' "uint64"  */
                             { (yyval.type) = Type::tBitfield64; }
    break;

  case 743: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' '>'  */
                                                                          {
            (yyval.pTypeDecl) = new TypeDecl((yyvsp[-2].type));
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-2]));
    }
    break;

  case 744: /* $@52: %empty  */
                                                                     { yyextra->das_arrow_depth ++; }
    break;

  case 745: /* $@53: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 746: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' $@52 bitfield_bits '>' $@53  */
                                                                                                                                                             {
            (yyval.pTypeDecl) = new TypeDecl((yyvsp[-5].type));
            (yyval.pTypeDecl)->argNames = *(yyvsp[-2].pNameList);
            auto maxBits = (yyval.pTypeDecl)->maxBitfieldBits();
            if ( (yyval.pTypeDecl)->argNames.size()>maxBits ) {
                das_yyerror(scanner,"only " + to_string(maxBits) + " different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-5])),
                    CompilationError::invalid_type);
            }
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
            delete (yyvsp[-2].pNameList);
    }
    break;

  case 749: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 750: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 751: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 752: /* dim_list: '[' ']'  */
                {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 753: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 754: /* dim_list: dim_list '[' ']'  */
                              {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 755: /* type_declaration_no_options: type_declaration_no_options_no_dim  */
                                                     {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 756: /* type_declaration_no_options: type_declaration_no_options_no_dim dim_list  */
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

  case 757: /* optional_expr_list_in_braces: %empty  */
            { (yyval.pExpression) = nullptr; }
    break;

  case 758: /* optional_expr_list_in_braces: '(' expr_list optional_comma ')'  */
                                                { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 759: /* type_declaration_no_options_no_dim: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 760: /* type_declaration_no_options_no_dim: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 761: /* type_declaration_no_options_no_dim: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 762: /* type_declaration_no_options_no_dim: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 763: /* $@54: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 764: /* $@55: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 765: /* type_declaration_no_options_no_dim: "type" '<' $@54 type_declaration '>' $@55  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 766: /* type_declaration_no_options_no_dim: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 767: /* type_declaration_no_options_no_dim: name_in_namespace '(' optional_expr_list ')'  */
                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 768: /* type_declaration_no_options_no_dim: '$' name_in_namespace optional_expr_list_in_braces  */
                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-1]), (yylsp[0]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[0].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-1])), *(yyvsp[-1].s)));
        delete (yyvsp[-1].s);
    }
    break;

  case 769: /* $@56: %empty  */
                                    { yyextra->das_arrow_depth ++; }
    break;

  case 770: /* type_declaration_no_options_no_dim: name_in_namespace '<' $@56 type_declaration_no_options_list '>' optional_expr_list_in_braces  */
                                                                                                                                                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-5]), (yylsp[0]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-2].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-5])), *(yyvsp[-5].s)));
        delete (yyvsp[-5].s);
    }
    break;

  case 771: /* $@57: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 772: /* type_declaration_no_options_no_dim: '$' name_in_namespace '<' $@57 type_declaration_no_options_list '>' optional_expr_list_in_braces  */
                                                                                                                                                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-5]), (yylsp[0]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-2].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-5])), *(yyvsp[-5].s)));
        delete (yyvsp[-5].s);
    }
    break;

  case 773: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 774: /* type_declaration_no_options_no_dim: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 775: /* type_declaration_no_options_no_dim: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 776: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 777: /* type_declaration_no_options_no_dim: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 778: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 779: /* type_declaration_no_options_no_dim: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 780: /* type_declaration_no_options_no_dim: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 781: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 782: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 783: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 784: /* type_declaration_no_options_no_dim: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 785: /* $@58: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 786: /* $@59: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 787: /* type_declaration_no_options_no_dim: "smart_ptr" '<' $@58 type_declaration '>' $@59  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 788: /* type_declaration_no_options_no_dim: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 789: /* $@60: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 790: /* $@61: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 791: /* type_declaration_no_options_no_dim: "array" '<' $@60 type_declaration '>' $@61  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 792: /* $@62: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 793: /* $@63: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 794: /* type_declaration_no_options_no_dim: "table" '<' $@62 table_type_pair '>' $@63  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 795: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 796: /* $@65: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 797: /* type_declaration_no_options_no_dim: "iterator" '<' $@64 type_declaration '>' $@65  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 798: /* type_declaration_no_options_no_dim: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 799: /* $@66: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 800: /* $@67: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 801: /* type_declaration_no_options_no_dim: "block" '<' $@66 type_declaration '>' $@67  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 802: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 803: /* $@69: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 804: /* type_declaration_no_options_no_dim: "block" '<' $@68 optional_function_argument_list optional_function_type '>' $@69  */
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

  case 805: /* type_declaration_no_options_no_dim: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 806: /* $@70: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 807: /* $@71: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 808: /* type_declaration_no_options_no_dim: "function" '<' $@70 type_declaration '>' $@71  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 809: /* $@72: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 810: /* $@73: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 811: /* type_declaration_no_options_no_dim: "function" '<' $@72 optional_function_argument_list optional_function_type '>' $@73  */
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

  case 812: /* type_declaration_no_options_no_dim: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 813: /* $@74: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 814: /* $@75: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 815: /* type_declaration_no_options_no_dim: "lambda" '<' $@74 type_declaration '>' $@75  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 816: /* $@76: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 817: /* $@77: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 818: /* type_declaration_no_options_no_dim: "lambda" '<' $@76 optional_function_argument_list optional_function_type '>' $@77  */
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

  case 819: /* $@78: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 820: /* $@79: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 821: /* type_declaration_no_options_no_dim: "tuple" '<' $@78 tuple_type_list '>' $@79  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 822: /* $@80: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 823: /* $@81: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 824: /* type_declaration_no_options_no_dim: "variant" '<' $@80 variant_type_list '>' $@81  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 825: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 826: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 827: /* type_declaration: type_declaration '|' '#'  */
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

  case 828: /* $@82: %empty  */
                   {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 829: /* $@83: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 830: /* $@84: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 831: /* $@85: %empty  */
                                                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 832: /* tuple_alias_declaration: "tuple" $@82 optional_public_or_private_alias "name" optional_emit_semis $@83 '{' $@84 tuple_alias_type_list optional_semis $@85 '}'  */
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

  case 833: /* $@86: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 834: /* $@87: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 835: /* $@88: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 836: /* $@89: %empty  */
                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 837: /* variant_alias_declaration: "variant" $@86 optional_public_or_private_alias "name" optional_emit_semis $@87 '{' $@88 variant_alias_type_list optional_semis $@89 '}'  */
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

  case 838: /* $@90: %empty  */
                      {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 839: /* $@91: %empty  */
                                                                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 840: /* $@92: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 841: /* $@93: %empty  */
                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 842: /* bitfield_alias_declaration: "bitfield" $@90 optional_public_or_private_alias "name" bitfield_basic_type_declaration optional_emit_commas $@91 '{' $@92 bitfield_alias_bits optional_commas $@93 '}'  */
          {
        auto btype = make_smart<TypeDecl>((yyvsp[-8].type));
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
                CompilationError::invalid_type);
        }
        for ( auto & p : *(yyvsp[-3].pNameExprList) ) {
            if ( get<1>(p) ) {
                ast_globalBitfieldConst ( scanner, btype, (yyvsp[-10].b), get<0>(p), get<1>(p) );
            }
        }
        if ( !yyextra->g_Program->addAlias(btype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-9].s),tokAt(scanner,(yylsp[-9])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-9]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfield((yyvsp[-9].s)->c_str(),atvname);
        }
        delete (yyvsp[-9].s);
        delete (yyvsp[-3].pNameExprList);
    }
    break;

  case 843: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 844: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 845: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 846: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 847: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 848: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 849: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 850: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 851: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 852: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 853: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 854: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 855: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 856: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 857: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 858: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 859: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 860: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 861: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 862: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 863: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 864: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 865: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 866: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 867: /* $@94: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 868: /* $@95: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 869: /* make_struct_decl: "struct" '<' $@94 type_declaration_no_options '>' $@95 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 870: /* $@96: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 871: /* $@97: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 872: /* make_struct_decl: "class" '<' $@96 type_declaration_no_options '>' $@97 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 873: /* $@98: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 874: /* $@99: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 875: /* make_struct_decl: "variant" '<' $@98 variant_type_list '>' $@99 '(' use_initializer make_variant_dim ')'  */
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

  case 876: /* $@100: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 877: /* $@101: %empty  */
                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 878: /* make_struct_decl: "variant" "type" '<' $@100 type_declaration_no_options '>' $@101 '(' use_initializer make_variant_dim ')'  */
                                                                                                                                                                                                    {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-10]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 879: /* $@102: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 880: /* $@103: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 881: /* make_struct_decl: "default" '<' $@102 type_declaration_no_options '>' $@103 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 882: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 883: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 884: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 885: /* $@104: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 886: /* $@105: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 887: /* make_tuple_call: "tuple" '<' $@104 tuple_type_list '>' $@105 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 888: /* make_dim_decl: '[' optional_expr_list ']'  */
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

  case 889: /* $@106: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 890: /* $@107: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 891: /* make_dim_decl: "array" "struct" '<' $@106 type_declaration_no_options '>' $@107 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 892: /* $@108: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 893: /* $@109: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 894: /* make_dim_decl: "array" "tuple" '<' $@108 tuple_type_list '>' $@109 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 895: /* $@110: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 896: /* $@111: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 897: /* make_dim_decl: "array" "variant" '<' $@110 variant_type_list '>' $@111 '(' make_variant_dim ')'  */
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

  case 898: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 899: /* $@112: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 900: /* $@113: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 901: /* make_dim_decl: "array" '<' $@112 type_declaration_no_options '>' $@113 '(' optional_expr_list ')'  */
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

  case 902: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 903: /* $@114: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 904: /* $@115: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 905: /* make_dim_decl: "fixed_array" '<' $@114 type_declaration_no_options '>' $@115 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 906: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 907: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 908: /* $@116: %empty  */
                  {
        yyextra->das_nested_parentheses ++;
    }
    break;

  case 909: /* make_table_decl: '{' $@116 optional_emit_semis optional_expr_map_tuple_list '}'  */
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

  case 910: /* make_table_decl: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 911: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 912: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 913: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 914: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 915: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 916: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 917: /* array_comprehension: '[' "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                               {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 918: /* array_comprehension: '[' "iterator" "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 919: /* array_comprehension: '{' "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' make_map_tuple array_comprehension_where '}'  */
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


