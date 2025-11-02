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
  YYSYMBOL_global_function_declaration = 281, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 282, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 283, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 284,     /* function_declaration  */
  YYSYMBOL_285_13 = 285,                   /* $@13  */
  YYSYMBOL_expression_block_finally = 286, /* expression_block_finally  */
  YYSYMBOL_287_14 = 287,                   /* $@14  */
  YYSYMBOL_288_15 = 288,                   /* $@15  */
  YYSYMBOL_expression_block = 289,         /* expression_block  */
  YYSYMBOL_290_16 = 290,                   /* $@16  */
  YYSYMBOL_291_17 = 291,                   /* $@17  */
  YYSYMBOL_expr_call_pipe = 292,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 293,           /* expression_any  */
  YYSYMBOL_expressions = 294,              /* expressions  */
  YYSYMBOL_optional_expr_list = 295,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_map_tuple_list = 296, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 297, /* type_declaration_no_options_list  */
  YYSYMBOL_name_in_namespace = 298,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 299,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 300,     /* new_type_declaration  */
  YYSYMBOL_301_18 = 301,                   /* $@18  */
  YYSYMBOL_302_19 = 302,                   /* $@19  */
  YYSYMBOL_expr_new = 303,                 /* expr_new  */
  YYSYMBOL_expression_break = 304,         /* expression_break  */
  YYSYMBOL_expression_continue = 305,      /* expression_continue  */
  YYSYMBOL_expression_return = 306,        /* expression_return  */
  YYSYMBOL_expression_yield = 307,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 308,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 309,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 310,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 311,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 312,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 313, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 314,           /* expression_let  */
  YYSYMBOL_expr_cast = 315,                /* expr_cast  */
  YYSYMBOL_316_20 = 316,                   /* $@20  */
  YYSYMBOL_317_21 = 317,                   /* $@21  */
  YYSYMBOL_318_22 = 318,                   /* $@22  */
  YYSYMBOL_319_23 = 319,                   /* $@23  */
  YYSYMBOL_320_24 = 320,                   /* $@24  */
  YYSYMBOL_321_25 = 321,                   /* $@25  */
  YYSYMBOL_expr_type_decl = 322,           /* expr_type_decl  */
  YYSYMBOL_323_26 = 323,                   /* $@26  */
  YYSYMBOL_324_27 = 324,                   /* $@27  */
  YYSYMBOL_expr_type_info = 325,           /* expr_type_info  */
  YYSYMBOL_expr_list = 326,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 327,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 328,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 329,            /* capture_entry  */
  YYSYMBOL_capture_list = 330,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 331,    /* optional_capture_list  */
  YYSYMBOL_expr_full_block = 332,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 333, /* expr_full_block_assumed_piped  */
  YYSYMBOL_expr_numeric_const = 334,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 335,              /* expr_assign  */
  YYSYMBOL_expr_named_call = 336,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 337,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 338,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 339,           /* func_addr_expr  */
  YYSYMBOL_340_28 = 340,                   /* $@28  */
  YYSYMBOL_341_29 = 341,                   /* $@29  */
  YYSYMBOL_342_30 = 342,                   /* $@30  */
  YYSYMBOL_343_31 = 343,                   /* $@31  */
  YYSYMBOL_expr_field = 344,               /* expr_field  */
  YYSYMBOL_345_32 = 345,                   /* $@32  */
  YYSYMBOL_346_33 = 346,                   /* $@33  */
  YYSYMBOL_expr_call = 347,                /* expr_call  */
  YYSYMBOL_expr = 348,                     /* expr  */
  YYSYMBOL_349_34 = 349,                   /* $@34  */
  YYSYMBOL_350_35 = 350,                   /* $@35  */
  YYSYMBOL_351_36 = 351,                   /* $@36  */
  YYSYMBOL_352_37 = 352,                   /* $@37  */
  YYSYMBOL_353_38 = 353,                   /* $@38  */
  YYSYMBOL_354_39 = 354,                   /* $@39  */
  YYSYMBOL_expr_generator = 355,           /* expr_generator  */
  YYSYMBOL_expr_mtag = 356,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 357, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 358,        /* optional_override  */
  YYSYMBOL_optional_constant = 359,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 360, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 361, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 362, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 363, /* struct_variable_declaration_list  */
  YYSYMBOL_364_40 = 364,                   /* $@40  */
  YYSYMBOL_365_41 = 365,                   /* $@41  */
  YYSYMBOL_366_42 = 366,                   /* $@42  */
  YYSYMBOL_function_argument_declaration_no_type = 367, /* function_argument_declaration_no_type  */
  YYSYMBOL_function_argument_declaration_type = 368, /* function_argument_declaration_type  */
  YYSYMBOL_function_argument_list = 369,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 370,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 371,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 372,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 373,             /* variant_type  */
  YYSYMBOL_variant_type_list = 374,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 375,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 376,             /* copy_or_move  */
  YYSYMBOL_variable_declaration_no_type = 377, /* variable_declaration_no_type  */
  YYSYMBOL_variable_declaration_type = 378, /* variable_declaration_type  */
  YYSYMBOL_variable_declaration = 379,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 380,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 381,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 382, /* let_variable_name_with_pos_list  */
  YYSYMBOL_global_let_variable_name_with_pos_list = 383, /* global_let_variable_name_with_pos_list  */
  YYSYMBOL_variable_declaration_list = 384, /* variable_declaration_list  */
  YYSYMBOL_let_variable_declaration = 385, /* let_variable_declaration  */
  YYSYMBOL_global_let_variable_declaration = 386, /* global_let_variable_declaration  */
  YYSYMBOL_optional_shared = 387,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 388, /* optional_public_or_private_variable  */
  YYSYMBOL_global_variable_declaration_list = 389, /* global_variable_declaration_list  */
  YYSYMBOL_390_43 = 390,                   /* $@43  */
  YYSYMBOL_global_let = 391,               /* global_let  */
  YYSYMBOL_392_44 = 392,                   /* $@44  */
  YYSYMBOL_enum_expression = 393,          /* enum_expression  */
  YYSYMBOL_commas = 394,                   /* commas  */
  YYSYMBOL_enum_list = 395,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 396, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 397,             /* single_alias  */
  YYSYMBOL_398_45 = 398,                   /* $@45  */
  YYSYMBOL_alias_declaration = 399,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 400, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 401,                /* enum_name  */
  YYSYMBOL_optional_enum_basic_type_declaration = 402, /* optional_enum_basic_type_declaration  */
  YYSYMBOL_optional_commas = 403,          /* optional_commas  */
  YYSYMBOL_emit_commas = 404,              /* emit_commas  */
  YYSYMBOL_optional_emit_commas = 405,     /* optional_emit_commas  */
  YYSYMBOL_enum_declaration = 406,         /* enum_declaration  */
  YYSYMBOL_407_46 = 407,                   /* $@46  */
  YYSYMBOL_408_47 = 408,                   /* $@47  */
  YYSYMBOL_409_48 = 409,                   /* $@48  */
  YYSYMBOL_optional_structure_parent = 410, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 411,          /* optional_sealed  */
  YYSYMBOL_structure_name = 412,           /* structure_name  */
  YYSYMBOL_class_or_struct = 413,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 414, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 415, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 416,    /* structure_declaration  */
  YYSYMBOL_417_49 = 417,                   /* $@49  */
  YYSYMBOL_418_50 = 418,                   /* $@50  */
  YYSYMBOL_419_51 = 419,                   /* $@51  */
  YYSYMBOL_variable_name_with_pos_list = 420, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 421,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 422, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 423, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 424,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 425,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 426,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_basic_type_declaration = 427, /* bitfield_basic_type_declaration  */
  YYSYMBOL_bitfield_type_declaration = 428, /* bitfield_type_declaration  */
  YYSYMBOL_429_52 = 429,                   /* $@52  */
  YYSYMBOL_430_53 = 430,                   /* $@53  */
  YYSYMBOL_c_or_s = 431,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 432,          /* table_type_pair  */
  YYSYMBOL_dim_list = 433,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 434, /* type_declaration_no_options  */
  YYSYMBOL_optional_expr_list_in_braces = 435, /* optional_expr_list_in_braces  */
  YYSYMBOL_type_declaration_no_options_no_dim = 436, /* type_declaration_no_options_no_dim  */
  YYSYMBOL_437_54 = 437,                   /* $@54  */
  YYSYMBOL_438_55 = 438,                   /* $@55  */
  YYSYMBOL_439_56 = 439,                   /* $@56  */
  YYSYMBOL_440_57 = 440,                   /* $@57  */
  YYSYMBOL_441_58 = 441,                   /* $@58  */
  YYSYMBOL_442_59 = 442,                   /* $@59  */
  YYSYMBOL_443_60 = 443,                   /* $@60  */
  YYSYMBOL_444_61 = 444,                   /* $@61  */
  YYSYMBOL_445_62 = 445,                   /* $@62  */
  YYSYMBOL_446_63 = 446,                   /* $@63  */
  YYSYMBOL_447_64 = 447,                   /* $@64  */
  YYSYMBOL_448_65 = 448,                   /* $@65  */
  YYSYMBOL_449_66 = 449,                   /* $@66  */
  YYSYMBOL_450_67 = 450,                   /* $@67  */
  YYSYMBOL_451_68 = 451,                   /* $@68  */
  YYSYMBOL_452_69 = 452,                   /* $@69  */
  YYSYMBOL_453_70 = 453,                   /* $@70  */
  YYSYMBOL_454_71 = 454,                   /* $@71  */
  YYSYMBOL_455_72 = 455,                   /* $@72  */
  YYSYMBOL_456_73 = 456,                   /* $@73  */
  YYSYMBOL_457_74 = 457,                   /* $@74  */
  YYSYMBOL_458_75 = 458,                   /* $@75  */
  YYSYMBOL_459_76 = 459,                   /* $@76  */
  YYSYMBOL_460_77 = 460,                   /* $@77  */
  YYSYMBOL_461_78 = 461,                   /* $@78  */
  YYSYMBOL_462_79 = 462,                   /* $@79  */
  YYSYMBOL_463_80 = 463,                   /* $@80  */
  YYSYMBOL_464_81 = 464,                   /* $@81  */
  YYSYMBOL_type_declaration = 465,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 466,  /* tuple_alias_declaration  */
  YYSYMBOL_467_82 = 467,                   /* $@82  */
  YYSYMBOL_468_83 = 468,                   /* $@83  */
  YYSYMBOL_469_84 = 469,                   /* $@84  */
  YYSYMBOL_470_85 = 470,                   /* $@85  */
  YYSYMBOL_variant_alias_declaration = 471, /* variant_alias_declaration  */
  YYSYMBOL_472_86 = 472,                   /* $@86  */
  YYSYMBOL_473_87 = 473,                   /* $@87  */
  YYSYMBOL_474_88 = 474,                   /* $@88  */
  YYSYMBOL_475_89 = 475,                   /* $@89  */
  YYSYMBOL_bitfield_alias_declaration = 476, /* bitfield_alias_declaration  */
  YYSYMBOL_477_90 = 477,                   /* $@90  */
  YYSYMBOL_478_91 = 478,                   /* $@91  */
  YYSYMBOL_479_92 = 479,                   /* $@92  */
  YYSYMBOL_480_93 = 480,                   /* $@93  */
  YYSYMBOL_make_decl = 481,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 482,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 483,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 484,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim_list = 485,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 486,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 487, /* optional_make_struct_dim_decl  */
  YYSYMBOL_use_initializer = 488,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 489,         /* make_struct_decl  */
  YYSYMBOL_490_94 = 490,                   /* $@94  */
  YYSYMBOL_491_95 = 491,                   /* $@95  */
  YYSYMBOL_492_96 = 492,                   /* $@96  */
  YYSYMBOL_493_97 = 493,                   /* $@97  */
  YYSYMBOL_494_98 = 494,                   /* $@98  */
  YYSYMBOL_495_99 = 495,                   /* $@99  */
  YYSYMBOL_496_100 = 496,                  /* $@100  */
  YYSYMBOL_497_101 = 497,                  /* $@101  */
  YYSYMBOL_make_map_tuple = 498,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 499,          /* make_tuple_call  */
  YYSYMBOL_500_102 = 500,                  /* $@102  */
  YYSYMBOL_501_103 = 501,                  /* $@103  */
  YYSYMBOL_make_dim_decl = 502,            /* make_dim_decl  */
  YYSYMBOL_503_104 = 503,                  /* $@104  */
  YYSYMBOL_504_105 = 504,                  /* $@105  */
  YYSYMBOL_505_106 = 505,                  /* $@106  */
  YYSYMBOL_506_107 = 506,                  /* $@107  */
  YYSYMBOL_507_108 = 507,                  /* $@108  */
  YYSYMBOL_508_109 = 508,                  /* $@109  */
  YYSYMBOL_509_110 = 509,                  /* $@110  */
  YYSYMBOL_510_111 = 510,                  /* $@111  */
  YYSYMBOL_511_112 = 511,                  /* $@112  */
  YYSYMBOL_512_113 = 512,                  /* $@113  */
  YYSYMBOL_expr_map_tuple_list = 513,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 514,          /* make_table_decl  */
  YYSYMBOL_515_114 = 515,                  /* $@114  */
  YYSYMBOL_array_comprehension_where = 516, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 517,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 518       /* array_comprehension  */
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
#define YYLAST   12154

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  208
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  311
/* YYNRULES -- Number of rules.  */
#define YYNRULES  894
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1618

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
       0,   564,   564,   565,   570,   571,   572,   573,   574,   575,
     576,   577,   578,   579,   580,   581,   582,   586,   587,   591,
     592,   596,   602,   603,   604,   608,   609,   613,   614,   618,
     637,   638,   639,   640,   644,   645,   649,   650,   654,   655,
     655,   659,   664,   673,   688,   704,   709,   717,   717,   756,
     770,   774,   777,   781,   787,   796,   799,   805,   806,   810,
     814,   815,   819,   822,   828,   834,   837,   843,   844,   848,
     849,   853,   854,   858,   859,   860,   868,   869,   873,   874,
     880,   881,   882,   883,   884,   888,   889,   893,   894,   898,
     900,   898,   912,   912,   920,   922,   920,   934,   934,   942,
     942,   954,   961,   968,   973,   982,   990,   996,  1004,  1014,
    1014,  1023,  1031,  1031,  1044,  1044,  1056,  1060,  1066,  1067,
    1068,  1069,  1070,  1071,  1075,  1080,  1088,  1089,  1090,  1094,
    1095,  1096,  1097,  1098,  1099,  1100,  1101,  1107,  1110,  1116,
    1119,  1125,  1126,  1127,  1128,  1132,  1150,  1173,  1176,  1186,
    1201,  1216,  1231,  1234,  1241,  1245,  1252,  1253,  1257,  1258,
    1262,  1263,  1264,  1268,  1271,  1278,  1282,  1283,  1284,  1285,
    1286,  1287,  1288,  1289,  1290,  1291,  1292,  1293,  1294,  1295,
    1296,  1297,  1298,  1299,  1300,  1301,  1302,  1303,  1304,  1305,
    1306,  1307,  1308,  1309,  1310,  1311,  1312,  1313,  1314,  1315,
    1316,  1317,  1318,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1326,  1327,  1328,  1329,  1330,  1331,  1332,  1333,  1334,  1335,
    1336,  1337,  1338,  1339,  1340,  1341,  1342,  1343,  1344,  1345,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,  1353,  1354,  1355,
    1356,  1357,  1358,  1359,  1360,  1361,  1362,  1363,  1364,  1365,
    1366,  1367,  1368,  1369,  1370,  1371,  1372,  1373,  1374,  1375,
    1376,  1381,  1399,  1400,  1401,  1405,  1411,  1411,  1428,  1431,
    1433,  1431,  1441,  1443,  1441,  1458,  1471,  1472,  1473,  1474,
    1475,  1476,  1477,  1478,  1479,  1480,  1481,  1482,  1483,  1484,
    1485,  1486,  1487,  1488,  1489,  1493,  1498,  1504,  1510,  1511,
    1515,  1516,  1520,  1524,  1531,  1532,  1543,  1547,  1550,  1558,
    1558,  1558,  1561,  1567,  1570,  1574,  1578,  1585,  1592,  1598,
    1602,  1606,  1609,  1612,  1620,  1623,  1631,  1637,  1638,  1639,
    1643,  1644,  1648,  1649,  1653,  1658,  1666,  1672,  1684,  1687,
    1690,  1696,  1696,  1696,  1699,  1699,  1699,  1704,  1704,  1704,
    1712,  1712,  1712,  1718,  1728,  1739,  1754,  1757,  1763,  1764,
    1771,  1782,  1783,  1784,  1788,  1789,  1790,  1791,  1792,  1796,
    1801,  1809,  1810,  1814,  1821,  1825,  1831,  1832,  1833,  1834,
    1835,  1836,  1837,  1841,  1842,  1843,  1844,  1845,  1846,  1847,
    1848,  1849,  1850,  1851,  1852,  1853,  1854,  1855,  1856,  1857,
    1858,  1859,  1863,  1870,  1882,  1887,  1897,  1901,  1908,  1911,
    1911,  1911,  1916,  1916,  1916,  1929,  1933,  1937,  1942,  1949,
    1958,  1963,  1970,  1970,  1970,  1977,  1981,  1990,  1998,  2006,
    2010,  2013,  2019,  2020,  2021,  2022,  2023,  2024,  2025,  2026,
    2027,  2028,  2029,  2030,  2031,  2032,  2033,  2034,  2035,  2036,
    2037,  2038,  2039,  2040,  2041,  2042,  2043,  2044,  2045,  2046,
    2047,  2048,  2049,  2050,  2051,  2052,  2053,  2054,  2060,  2061,
    2062,  2063,  2064,  2077,  2086,  2087,  2088,  2089,  2090,  2091,
    2092,  2093,  2094,  2095,  2096,  2097,  2098,  2099,  2102,  2102,
    2102,  2105,  2110,  2114,  2118,  2118,  2118,  2123,  2126,  2130,
    2130,  2130,  2135,  2138,  2139,  2140,  2141,  2142,  2143,  2144,
    2145,  2146,  2148,  2152,  2153,  2161,  2164,  2167,  2176,  2177,
    2178,  2179,  2180,  2181,  2182,  2186,  2190,  2194,  2198,  2202,
    2206,  2210,  2214,  2218,  2225,  2226,  2230,  2231,  2232,  2236,
    2237,  2241,  2242,  2243,  2247,  2248,  2252,  2264,  2267,  2268,
    2272,  2272,  2291,  2290,  2305,  2304,  2321,  2333,  2342,  2352,
    2353,  2354,  2355,  2356,  2360,  2363,  2372,  2373,  2377,  2380,
    2384,  2398,  2407,  2408,  2412,  2415,  2419,  2433,  2434,  2438,
    2444,  2450,  2459,  2462,  2469,  2472,  2478,  2479,  2480,  2484,
    2485,  2489,  2496,  2501,  2510,  2516,  2527,  2534,  2543,  2546,
    2549,  2556,  2559,  2564,  2575,  2578,  2583,  2595,  2596,  2600,
    2601,  2602,  2606,  2609,  2612,  2612,  2632,  2635,  2635,  2653,
    2658,  2666,  2667,  2671,  2674,  2687,  2704,  2705,  2706,  2711,
    2711,  2737,  2741,  2742,  2743,  2747,  2757,  2760,  2766,  2767,
    2771,  2772,  2776,  2777,  2781,  2783,  2788,  2781,  2804,  2805,
    2809,  2810,  2814,  2820,  2821,  2822,  2823,  2827,  2828,  2829,
    2833,  2836,  2842,  2844,  2849,  2842,  2870,  2877,  2882,  2891,
    2897,  2908,  2909,  2910,  2911,  2912,  2913,  2914,  2915,  2916,
    2917,  2918,  2919,  2920,  2921,  2922,  2923,  2924,  2925,  2926,
    2927,  2928,  2929,  2930,  2931,  2932,  2933,  2934,  2938,  2939,
    2940,  2941,  2942,  2943,  2944,  2945,  2949,  2960,  2964,  2971,
    2983,  2990,  2999,  3004,  3014,  3024,  3034,  3047,  3048,  3049,
    3050,  3051,  3055,  3055,  3055,  3069,  3070,  3074,  3078,  3085,
    3089,  3093,  3097,  3104,  3107,  3126,  3127,  3131,  3132,  3133,
    3134,  3135,  3135,  3135,  3139,  3144,  3151,  3158,  3158,  3165,
    3165,  3172,  3176,  3180,  3185,  3190,  3195,  3200,  3204,  3208,
    3213,  3217,  3221,  3226,  3226,  3226,  3232,  3239,  3239,  3239,
    3244,  3244,  3244,  3250,  3250,  3250,  3255,  3260,  3260,  3260,
    3265,  3265,  3265,  3274,  3279,  3279,  3279,  3284,  3284,  3284,
    3293,  3298,  3298,  3298,  3303,  3303,  3303,  3312,  3312,  3312,
    3318,  3318,  3318,  3327,  3330,  3341,  3357,  3359,  3364,  3369,
    3357,  3395,  3397,  3402,  3408,  3395,  3434,  3436,  3441,  3446,
    3434,  3487,  3488,  3489,  3490,  3491,  3495,  3502,  3509,  3515,
    3521,  3528,  3535,  3541,  3550,  3553,  3559,  3567,  3572,  3579,
    3584,  3590,  3591,  3595,  3596,  3600,  3600,  3600,  3608,  3608,
    3608,  3615,  3615,  3615,  3626,  3626,  3626,  3637,  3643,  3649,
    3655,  3655,  3655,  3669,  3688,  3688,  3688,  3698,  3698,  3698,
    3712,  3712,  3712,  3726,  3735,  3735,  3735,  3755,  3762,  3762,
    3762,  3772,  3775,  3781,  3781,  3803,  3811,  3831,  3856,  3857,
    3861,  3862,  3866,  3869,  3872
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
  "make_map_tuple", "make_tuple_call", "$@102", "$@103", "make_dim_decl",
  "$@104", "$@105", "$@106", "$@107", "$@108", "$@109", "$@110", "$@111",
  "$@112", "$@113", "expr_map_tuple_list", "make_table_decl", "$@114",
  "array_comprehension_where", "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1431)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-795)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1431,   100, -1431, -1431,    35,   -79,   334,   130, -1431,   -19,
   -1431, -1431, -1431, -1431,   206,   274, -1431, -1431, -1431, -1431,
     136,   136,   136, -1431,   233, -1431,   147, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431,    80, -1431,    93,
      97,   154, -1431, -1431,   334,    23, -1431, -1431, -1431,   181,
     136, -1431, -1431,   147,   130,   130,   130,   212,   268, -1431,
   -1431, -1431, -1431,   274,   274,   274,   225, -1431,   689,    13,
   -1431, -1431, -1431, -1431,   471, -1431,    38, -1431,   490,    79,
      35,   285,   -79,   279,   436, -1431,   444,   469, -1431, -1431,
   -1431,   520,   503,   505,   530, -1431,   541,   317, -1431, -1431,
     -66,    35,   274,   274,   274,   274,   326, -1431, -1431, -1431,
   -1431,   585, -1431, -1431,   169,   679, -1431, -1431,   509, -1431,
   -1431, -1431, -1431, -1431,   670,   101, -1431, -1431, -1431, -1431,
     659, -1431, -1431,   566, -1431, -1431,   555,   571,   326,   326,
   -1431, -1431,   592, -1431,    77, -1431,   339,   624,   689, -1431,
     606, -1431, 10924, -1431, -1431,   611, -1431, -1431, -1431, -1431,
   -1431, -1431,   594, -1431, -1431, -1431,   728, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431,   138, -1431,  7403,   721, -1431,   575,
     634, -1431, -1431, -1431, -1431, -1431, 11036, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431,   765,   771, -1431,   610,   326, -1431,   643,   716,
     -34,    35,   620,   669, -1431, -1431, -1431,   101, -1431,   646,
     654,   655,   640,   662,   665, -1431, -1431, -1431,   661, -1431,
   -1431, -1431, -1431, -1431,   -98, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431,   688, -1431, -1431, -1431,
     692,   693, -1431, -1431, -1431, -1431,   697,   700,   685,   206,
    -107, -1431, -1431, -1431, -1431,   500,   683,   714, -1431, -1431,
   -1431, -1431, -1431, -1431,   736, -1431,   702,   703,   746,   752,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
     753,   718, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431,   898, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431,   758,   723, -1431,
   -1431,   -55,   747, -1431,   951,   634, -1431,   761,   326, -1431,
   -1431,   594,   326,    35, -1431,   304, -1431, -1431, -1431, -1431,
   -1431,  6756, -1431, -1431,   778,   763,   351,   376,   394, -1431,
   -1431,  6756,   346, -1431,  6756, -1431, -1431, -1431,     9, -1431,
   -1431, -1431,    65, -1431,  3891,   740,  7214, -1431,   729, -1431,
   -1431, -1431, -1431, -1431, -1431,   781,  1010, -1431,   749, -1431,
      92,   750,     3,   759,  7403, -1431, -1431,   745, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431,   748,   779, -1431,
     261, -1431,   326,   796,  7403, -1431,   128,  7403,  7403,  7403,
     784,   785, -1431, -1431,    34,   206,   786,    36, -1431,   406,
     769,   791,   792,   774,   795,   776,   440,   800, -1431,   441,
     801,   810,  6756,  6756,   798,   803,   804,   811,   816,   827,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,  6756,
    6756,  6756,  6756,  6756,  3509,  4082, -1431,   789,   982, -1431,
   -1431, -1431,   829, -1431, -1431, -1431, -1431,   812, -1431, -1431,
   -1431, -1431, -1431, -1431,   613,  7491, -1431, -1431,   831, -1431,
   -1431, -1431, -1431, -1431, -1431,  7403,  7403,   832, -1431,  7403,
     610,  7403,   610,  7403,   610,  7496,   833,  7591, -1431,  6756,
   -1431,  7403,   834,   859, 11368, -1431, -1431, -1431, -1431,   835,
   -1431, -1431, 10516,  4273, -1431,   500, -1431,  7496,   833, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431,  6756, -1431, -1431,   439,   -50,   -50,   -50, -1431,
     714, -1431, -1431,   206, -1431,   413,   836,   994,   491, -1431,
   -1431, -1431,   498, -1431, -1431, -1431,  6756,   433,   519,   857,
     408, -1431, -1431, -1431, -1431,   839, -1431, -1431,   449, -1431,
     860,   861,   862, -1431,  6756,  7403,  6756,  6756, -1431, -1431,
    6756, -1431,  6756, -1431,  6756, -1431, -1431,  6756, -1431,  7403,
     527,   527,  6756,  6756,  6756,  6756,  6756,  6756,   626,   527,
     527,   -46,   527,   527,   846,  1034,   854,   855,   120,   859,
     881,   856,   382,   858,   326,  1710,   274,  1052,   874, -1431,
     812, -1431,  7663, 11191,  6756,  6756, -1431, -1431,  6756,  6756,
    6756,  6756,   896,  6756,   165,  6756,  6756,  6756,  6756,  6756,
    6756,  6756,  6756,  6756,  4464,  6756,  6756,  6756,  6756,  6756,
    6756,  6756,  6756,  6756,  6756, 11956,  6756, -1431,  4655,   538,
     573, -1431,   900,   581,   747,   584,   747,   589,   747,   -67,
   -1431,   262,   714,   888, -1431,   474, -1431,  7403,   859,   479,
     714, -1431,  6756, -1431, -1431, -1431, -1431, 10600,   902, -1431,
     136, -1431,   136,  7678,   878,  1036, -1431, -1431,   116, -1431,
   -1431, -1431,  2101,   917, -1431, -1431, -1431, -1431,  6756,   918,
     919,  7403,   128, -1431,  6756,  1257, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431,  7403,  7403,  7403,  7403,  3700,   920,  6756,
    7403, -1431, -1431, -1431,  7403,   859,   818, 11119, -1431,   911,
    7764,  7403,  7403,  7850,  7403,  7936,  7403,   859,  7403,  7496,
     859,   833,   124,  8022,  8108,  8194,  8280,  8366,  8452, -1431,
    6756,   518,   -51,   885, -1431,  6756, -1431,  6756, -1431,  6756,
     891,   426, -1431, -1431,   890,   895,   398, -1431, -1431,   -51,
    6756,   146,  3318, -1431,   148,   897,   231,   901,   610, -1431,
    2307,  1052,   916,   903, -1431, -1431,   924,   905, -1431, -1431,
     204,   204,   673,   673, 11828, 11828,   907,   437,   908, -1431,
   10684,    -6,    -6,   831,   204,   204, 11681, 11566, 11598, 11451,
   11357, 11202, 11713,   889, 11796,   673,   673,   321,   321,   437,
     437,   437,   536,  6756,   909,   910,   543,  6756,  1096,   913,
   10768, -1431,   153, -1431, -1431, -1431,   335, -1431,   933, -1431,
     935, -1431,   945,  7403, -1431,  7496,  7403, -1431,   833,   660,
     899,   936,  7403, 11368, -1431,   961,   539, -1431,  7307, -1431,
      64, -1431, -1431,  6756,   974,   975,  7403, -1431,  6756, -1431,
    4846, -1431, -1431,  5037,   977,   978, -1431,   136,   986,  5228,
       1,  5419, -1431,   136,   136,  1134, -1431,   717, -1431, -1431,
    1133, -1431, -1431,  1138, -1431,  1106,   136,   943, -1431,   136,
     136,   136,   136,   136, -1431,  1084, -1431,   136,  7046,   984,
   -1431,   539,    24,  8538, -1431,  1108,   498,  6756,  1257, -1431,
   -1431, -1431, -1431,   714,  1028,  1033,   595,   563,   156,   966,
     968,   663,  8624,   604,  7403,  7496,   833,  1073,   989,   962,
    7403,  6756,  6756,   992, -1431,  1159,  1288, -1431,  1293, -1431,
    1443,   993,  1491,   701,   997,   754,  1052, -1431, -1431, -1431,
   -1431, -1431,   995, 11285,   999,  1161,  1043,    27,   -51,  8710,
   11368, 11368, -1431,  1011,   150,  6756,  6756,  7403,   610,    33,
    1007,   911,   238, -1431,  1017,   232,  6947, -1431, -1431, -1431,
     185,   747, -1431,   610, -1431,  6756, -1431,  6756,  5610,  6756,
   -1431,  1041,  1022, -1431, -1431,  6756,  1023, -1431, 10852,  6756,
    5801,  1024, -1431, 10936, -1431,  5992, -1431,  6756, -1431, -1431,
   -1431, -1431,  1062, -1431, -1431, -1431, -1431, -1431, -1431,   714,
   -1431, -1431,   714, -1431, -1431,   936, -1431, -1431,   714,  6756,
   -1431,   525, -1431, -1431, -1431,  1020, -1431,  1021,  8796, -1431,
    1185,   143, 11368,  6756, 11368,  1216,  6756, 11368,  1057,  1058,
   -1431,  1060,  1077, 11368, -1431,  6756, 11368, -1431, -1431,  1038,
   -1431, -1431,  1042,  1044,  1046,  1047, -1431,  1197, -1431, -1431,
   -1431, -1431, -1431, -1431,   -78, -1431,  6756,  6756,  6756,  6756,
    6756,  6756,  6756,  6756,  6756,  6756,  6756,  6756,  6756,  6756,
    6756,  6756,  6756,  6756,  6756,   528, -1431,  1078, -1431, -1431,
    1242,   594, -1431,  1088, -1431,  6756,  1257, -1431, -1431, -1431,
   -1431,  1055, -1431, -1431, -1431,  1053,  1094, -1431, -1431,  1709,
     775,   797, -1431, -1431,  6756,  1774, 11368, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,    84,  6183,
   -1431,  1087,  6756,  1097, -1431,   173,  6756,    -4,    37,   249,
    6756,  6756,  6756,  8882,  8968,  1864,   747,  6756, -1431, -1431,
   -1431,   426,  1061,  3318,  1098,  1100,  1059,  1102,  1103, -1431,
     177,   326,   747,  7403,  9054,  7403,  9140, -1431,   178,  9226,
   -1431,  6756, 11483,  6756, -1431,  9312,  3318, -1431,   193,  6756,
   -1431, -1431, -1431,   237, -1431, -1431, -1431, -1431, -1431, -1431,
   11368,  1093, -1431,  1064, -1431, -1431, -1431,  1105,  6756, 11368,
   -1431, 11368,  7403,  6756, -1431, -1431, -1431, 11368,  6756,  6756,
     -51,  6756,  6756, -1431, -1431,  1043, -1431, -1431, -1431, 11368,
   11368, 11368, 11368, 11368, 11368, 11368, 11368, 11368, 11368, 11368,
   11368, 11368, 11368, 11368, 11368, 11368, 11368, 11368, -1431,  1065,
    1099,   681,  1205,   136, -1431,  1257, -1431,  1074,  1075, -1431,
   -1431,  6756,  1095, -1431, -1431, -1431, -1431,  1081,  1076,  1082,
    6756,  6756,  6756,  1085,  1214,  1086,  1089,  6374, -1431, -1431,
     247, -1431, -1431,  9398, -1431,  1125, -1431,   281,  1247,  1043,
    6756,  6756,  6756,  9484, 11368, 11368, -1431, -1431, -1431,  1110,
     284, -1431,   287, -1431, -1431,  1130, -1431, -1431,   185, -1431,
    1158,   326,  1902, -1431,   605, -1431, -1431, -1431,  7403,  9570,
    9656, -1431,   295, -1431,  9742, -1431,  6756, -1431, -1431, 11368,
   -1431,   714, 11368,  9828,  9914,    42, 10000, 10086,  1092,   302,
     -44, -1431,  7403, -1431, -1431,   157, -1431,    20, -1431, -1431,
    1214,  1214, 10172,  1101,  1107,  1111,  1112,  6756, -1431,  6756,
     673,   673,   673,  6756, -1431, -1431,  1214,  1214, -1431, 10258,
   -1431, -1431,  1131, -1431, -1431,  1109,  1136,   322,   328, 11368,
   11368,   270,   488, -1431,  1113,  1104,  1115, -1431,  6565, -1431,
   -1431, -1431, -1431, -1431,   607, -1431, -1431,  1117, -1431, 11368,
    1291,   326,  6756,   326,   326, -1431,   201, -1431, -1431, -1431,
      81,  1277,    20, -1431, -1431,   681,   -94,   -94, -1431,  6756,
    1214,  1214,   563,  1118,  1119,   859,   -94,   563, -1431, -1431,
    6756, -1431, -1431,  1120,  6756,  6756, -1431,   488,  6756, -1431,
   -1431,  6756, 11451, -1431, -1431, -1431, -1431, -1431,  3127,   136,
    1122,   340, -1431, -1431,  2513,  7403,   128, -1431, -1431, -1431,
    1277,   439,   563,  1141,  1149, -1431,  1127,  1128, 10344,   -94,
     -94,  1141,  1129, -1431, -1431,  1132,  1135,  1142,  1882,  6756,
   11368, 11368, -1431,  1140, 11483, -1431, -1431, -1431, -1431, -1431,
   -1431, 11368, -1431,   638,  1126,  3127,   326, -1431, -1431,  1144,
     719,  6756, 10924, -1431, -1431, -1431, -1431,   342,  1123, -1431,
   -1431, -1431, -1431,  1145,  1146, -1431, -1431, -1431, -1431,  1279,
    1152,  1882,  1150,   326, -1431, -1431,  1137, -1431, -1431,   136,
   -1431, -1431,  6756,  1257,   136, 10924, -1431,   563, -1431, -1431,
    6756, -1431,  1153, -1431,  1154,  6756,  2719, -1431, -1431,  1257,
   -1431, -1431,   326,   363, 11368, -1431, -1431,  1155,  3127, 10430,
    1151, -1431, -1431, -1431, -1431,   136,   326,  1197, -1431,  2925,
   -1431,  1154, -1431,  1156,   638,  1197, -1431, -1431
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   158,     1,   330,     0,     0,     0,   626,   331,     0,
     816,   806,   811,    20,     0,     0,    19,    16,    15,     3,
       0,     0,     0,     8,   662,     7,   607,     6,    11,     5,
       4,    13,    12,    14,   127,   128,   126,   135,   137,    49,
      62,    59,    60,    51,     0,    57,    50,   628,   627,     0,
       0,    26,    25,   607,   626,   626,   626,     0,   304,    47,
     142,   143,   144,     0,     0,     0,   145,   147,   154,     0,
     141,    21,    10,     9,   262,   644,     0,   608,   609,     0,
       0,     0,     0,    52,     0,    58,     0,     0,    55,   629,
     631,    22,     0,     0,     0,   306,     0,     0,   153,   148,
       0,     0,     0,     0,     0,     0,    71,   264,   263,   266,
     261,   632,   654,   653,     0,   657,   611,   610,   617,   133,
     134,   131,   132,   130,     0,     0,   129,   138,    63,    61,
      57,    54,    53,     0,    23,    24,    27,   717,    71,    71,
     305,    45,    48,   152,     0,   149,   150,   151,   155,    69,
      72,   159,     0,   634,   633,     0,   656,   655,   659,   658,
     663,   612,   534,    30,    31,    35,     0,   122,   123,   120,
     121,   119,   118,   124,     0,    56,     0,     0,    29,     0,
     642,   807,   812,    46,   146,    70,     0,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,     0,     0,   165,   160,    71,   635,   636,   650,
     614,     0,   535,     0,    32,    33,    34,     0,   136,     0,
       0,     0,     0,     0,     0,   671,   691,   672,   707,   673,
     677,   678,   679,   680,   697,   684,   685,   686,   687,   688,
     689,   690,   692,   693,   694,   695,   776,   676,   683,   696,
     783,   790,   674,   681,   675,   682,     0,     0,     0,     0,
     706,   737,   740,   738,   739,   803,   733,   630,    28,   720,
     721,   718,   719,   640,   643,   817,     0,     0,   230,   231,
     228,   168,   169,   171,   170,   172,   173,   174,   175,   201,
     202,   199,   200,   192,   203,   204,   193,   190,   191,   229,
     212,     0,   227,   205,   206,   207,   208,   179,   180,   181,
     176,   177,   178,   189,     0,   195,   196,   194,   187,   188,
     183,   182,   184,   185,   186,   167,   166,   211,     0,   197,
     198,   534,   163,   272,     0,   642,   651,     0,    71,   616,
     613,   534,    71,     0,   596,   589,   618,   125,   741,   767,
     770,     0,   773,   763,     0,     0,   777,   784,   791,   797,
     800,     0,   735,   747,   298,   753,   758,   752,     0,   766,
     762,   755,     0,   757,     0,   734,     0,   641,     0,   808,
     813,   232,   233,   226,   210,   234,   213,   209,     0,   161,
     329,   559,   560,     0,     0,   265,   267,     0,   698,   701,
     704,   705,   699,   702,   700,   703,   637,     0,   648,   664,
       0,   139,    71,     0,     0,   590,     0,     0,     0,     0,
       0,     0,   438,   439,     0,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   697,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,   376,   378,   377,   379,   380,   381,   382,    41,     0,
       0,     0,     0,     0,   298,     0,   361,   362,   883,   436,
     435,   513,   433,   506,   505,   504,   503,   156,   509,   434,
     508,   507,   480,   440,   481,     0,   485,   441,     0,   437,
     821,   825,   822,   823,   824,     0,     0,     0,   722,     0,
     160,     0,   160,     0,   160,     0,     0,     0,   749,     0,
     746,     0,     0,   890,   356,   760,   761,   754,   756,     0,
     759,   730,     0,     0,   805,   804,   818,   568,   574,   235,
     215,   216,   218,   217,   219,   220,   221,   222,   214,   223,
     224,   225,     0,   327,   328,     0,   534,   534,   534,   162,
     164,   295,   645,     0,   652,     0,     0,   591,   589,   615,
     140,   597,     0,   587,   588,   586,     0,     0,     0,     0,
     727,   845,   848,   309,   706,   313,   312,   318,     0,   350,
       0,     0,     0,   874,     0,     0,     0,     0,   341,   344,
       0,   347,     0,   878,     0,   854,   860,     0,   851,     0,
     468,   469,     0,     0,     0,     0,     0,     0,     0,   445,
     444,   482,   443,   442,     0,     0,     0,     0,   304,   890,
     890,     0,   363,     0,    71,     0,     0,   371,   362,   295,
     156,   275,     0,     0,     0,     0,   470,   471,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   422,     0,   744,     0,     0,
       0,   708,     0,     0,   163,     0,   163,     0,   163,   304,
     566,     0,   564,     0,   572,     0,   709,     0,   890,     0,
     302,   745,   891,   299,   751,   729,   732,     0,   712,   569,
      87,   575,    87,     0,     0,   666,   556,   557,   579,   561,
     563,   562,     0,   623,   649,   660,   547,   665,     0,     0,
       0,     0,     0,   604,     0,     0,   742,   768,   771,    18,
      17,   725,   726,     0,     0,     0,     0,   843,     0,     0,
       0,   864,   867,   870,     0,   890,     0,   858,   881,   890,
       0,     0,     0,     0,     0,     0,     0,   890,     0,     0,
     890,     0,     0,     0,     0,     0,     0,     0,     0,    44,
       0,    42,     0,     0,   863,     0,   578,     0,   577,     0,
       0,   891,   836,   473,     0,     0,   409,   406,   408,     0,
     300,     0,   298,   425,     0,     0,     0,     0,   160,   363,
       0,   371,     0,     0,   492,   491,     0,     0,   493,   497,
     446,   447,   459,   460,   457,   458,     0,   486,     0,   478,
       0,   510,   511,   512,   448,   449,   464,   465,   466,   467,
       0,     0,   462,   463,   461,   455,   456,   451,   450,   452,
     453,   454,     0,     0,     0,   415,     0,     0,     0,     0,
       0,   430,     0,   774,   764,   710,     0,   778,     0,   785,
       0,   792,     0,     0,   798,     0,     0,   801,     0,     0,
       0,   735,     0,   357,   731,   713,   638,    85,    88,   809,
      88,   814,   558,     0,     0,     0,     0,   580,     0,   297,
     321,   319,   272,     0,     0,     0,   320,     0,     0,     0,
      71,     0,   276,     0,     0,     0,   289,     0,   290,   284,
       0,   281,   280,     0,   282,     0,     0,     0,   296,     0,
      83,    84,    81,    82,   291,   333,   279,     0,   383,   619,
     624,   638,   550,     0,   593,   594,     0,     0,     0,   606,
     743,   769,   772,   728,     0,     0,     0,   844,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   891,     0,   483,     0,     0,   484,     0,   514,
       0,     0,     0,     0,     0,     0,   371,   518,   519,   520,
     521,   522,     0,    38,     0,   102,     0,     0,     0,     0,
     827,   826,   472,     0,     0,     0,     0,     0,   160,     0,
       0,   890,     0,   426,     0,     0,     0,   429,   427,   157,
       0,   163,   375,   160,   488,     0,   494,     0,     0,     0,
     476,     0,     0,   498,   502,     0,     0,   479,     0,     0,
       0,     0,   416,     0,   423,     0,   474,     0,   431,   775,
     765,   723,     0,   779,   781,   786,   788,   793,   795,   565,
     799,   567,   571,   802,   573,   735,   736,   748,   303,     0,
     621,   639,   819,    86,   570,     0,   576,     0,     0,   668,
     669,   582,   581,     0,   322,     0,     0,   307,     0,     0,
     294,     0,     0,    66,   272,     0,   324,   292,   293,     0,
      76,    77,     0,     0,     0,     0,   283,   268,   278,   285,
     286,   287,   288,   332,     0,   277,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   639,   646,     0,   548,   661,
       0,   534,   592,     0,   601,     0,     0,   605,   846,   849,
     310,     0,   315,   316,   314,     0,     0,   353,   351,     0,
       0,     0,   875,   873,   300,     0,   857,   882,   885,   342,
     345,   348,   879,   877,   855,   861,   859,   852,    71,     0,
      39,     0,     0,     0,   334,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   163,     0,   884,   301,
     428,     0,     0,   298,     0,     0,     0,     0,     0,   369,
       0,    71,   163,     0,     0,     0,     0,   404,     0,     0,
     499,     0,   487,     0,   477,     0,   298,   417,     0,     0,
     475,   424,   420,     0,   724,   711,   782,   789,   796,   750,
     714,   715,   622,     0,   810,   815,   667,     0,     0,   323,
     272,   308,     0,     0,    64,    65,   111,   325,     0,     0,
       0,     0,     0,   269,   274,     0,   598,   339,   338,   393,
     394,   396,   395,   397,   387,   388,   389,   398,   399,   385,
     386,   400,   401,   390,   391,   392,   384,   620,   625,     0,
       0,   541,   544,     0,   595,     0,   603,     0,     0,   311,
     317,     0,     0,   352,   865,   868,   871,     0,     0,     0,
       0,     0,     0,     0,   843,     0,     0,     0,   272,   524,
       0,    36,    43,     0,   104,     0,   105,     0,   106,     0,
       0,     0,     0,     0,   829,   828,   407,   533,   410,     0,
       0,   402,     0,   366,   367,     0,   365,   364,     0,   372,
     272,    71,     0,   532,     0,   530,   405,   527,     0,     0,
       0,   526,     0,   418,     0,   421,     0,   820,   670,   583,
     326,   117,   116,     0,     0,     0,     0,     0,     0,     0,
       0,   647,     0,   542,   543,   544,   545,   536,   551,   602,
     843,   843,     0,     0,     0,     0,     0,   298,   886,   300,
     343,   346,   349,     0,   844,   856,   843,   843,   515,     0,
     517,   525,    40,   103,   335,     0,     0,     0,     0,   831,
     830,     0,     0,   413,     0,     0,     0,   370,     0,   358,
     373,   272,   489,   495,     0,   531,   529,     0,   528,   716,
      78,    71,     0,    71,    71,   295,   589,   340,   599,   600,
       0,   539,   536,   537,   538,   541,   842,   842,   354,     0,
     843,   843,   834,     0,     0,   890,   842,   834,   516,    37,
       0,   107,   108,     0,     0,     0,   411,     0,     0,   403,
     368,     0,   359,   374,   490,   496,   500,   419,     0,     0,
      92,     0,   272,   272,     0,     0,     0,   549,   540,   552,
     539,     0,     0,   839,   890,   841,     0,     0,     0,   842,
     842,   835,     0,   876,   887,     0,     0,     0,   888,     0,
     833,   832,   414,   888,   360,   501,    79,    83,    84,    81,
      82,    80,   101,    73,     0,     0,    71,   113,   115,     0,
       0,     0,     0,   554,   585,   584,   546,     0,   891,   840,
     847,   850,   355,     0,     0,   872,   880,   862,   853,     0,
       0,   888,     0,    71,    67,    68,     0,   100,   295,     0,
     272,   271,     0,     0,     0,     0,   837,     0,   866,   869,
       0,   892,     0,   894,    97,     0,     0,    93,   110,     0,
     337,   553,    71,     0,   889,   893,    74,     0,     0,     0,
       0,   336,   272,   838,   295,     0,    71,   268,   555,     0,
      98,    97,    91,     0,    73,   268,    75,    96
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1431, -1431,  -853,    -1, -1431, -1431, -1431, -1431, -1431,   737,
    1256, -1431, -1431, -1431, -1431, -1431, -1431,  1338, -1431, -1431,
   -1431,  1314, -1431,  1231, -1431, -1431,  1281, -1431, -1431, -1431,
   -1431,  -135,  -250, -1431, -1431, -1430,   653,   658, -1431, -1431,
   -1431, -1431,  -244, -1431, -1431, -1431, -1431, -1431, -1431,  -787,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,  1157, -1431,
   -1431,   -56,  1270, -1431, -1431, -1431,   407,   742,   755,   448,
    -483,  -658, -1431, -1431, -1431, -1381, -1431, -1431, -1179, -1431,
   -1431,  -889, -1431, -1431, -1431, -1431,  -631,  -369, -1139,   696,
     -13, -1431, -1431, -1431, -1431, -1431, -1427, -1421, -1420, -1417,
   -1431, -1431,  1393, -1431, -1219, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,  -440, -1431,
     906,    48, -1431,  -763, -1431, -1431, -1431, -1431, -1431, -1431,
   -1366, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
     515, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,  -152,
     -48,   -95,   -53,    21, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431,   292,  -503,  -731, -1431,  -523,  -735, -1431,  -711,   -91,
     -90, -1431,  -563,  -562, -1431, -1431, -1431, -1092, -1431,  1354,
   -1431, -1431, -1431, -1431, -1431,   288,   473, -1431,   848, -1431,
   -1431, -1431, -1431, -1431, -1431,   478, -1431,  1080, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431,  -153, -1431,   996, -1431, -1431, -1431,  1176,
   -1431, -1431, -1431,  -566, -1431, -1431,  -299,  -844, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431,  -113, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431,  1012,  -689,   -40,  -619, -1431, -1431, -1229,
    -973, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
    -961, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431, -1431,
   -1431, -1431, -1431, -1431, -1431,   837, -1431, -1431, -1382,  -612,
   -1431
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   741,   742,    18,   136,    53,   178,    19,   166,
     172,  1412,  1181,  1321,   618,   479,   142,   480,    97,    21,
      22,    45,    46,    88,    23,    41,    42,   913,   914,  1566,
     150,   151,  1567,  1102,  1489,   915,   888,   889,  1533,  1534,
    1600,  1535,  1596,  1597,  1613,  1598,   916,   917,   918,   997,
     919,   920,   921,   922,   923,   924,   925,   926,   173,   174,
      37,    38,    39,   222,    66,    67,    68,    69,   637,    24,
     342,   405,   215,    25,   109,   216,   110,   152,  1264,  1378,
    1539,   406,   407,   927,   481,   928,   722,   626,  1010,   699,
     482,   929,   585,   746,  1299,   483,   930,   931,   932,   933,
     934,   555,   935,  1114,  1185,  1267,   936,   484,   761,  1310,
     762,  1311,   764,  1312,   485,   750,  1303,   486,   523,  1430,
     487,  1209,  1210,   808,   488,   641,   489,   937,   490,   491,
     798,   492,  1007,  1422,  1008,  1477,   493,   858,  1231,   494,
     524,  1213,  1484,  1215,  1485,  1358,  1525,   496,   497,   400,
    1455,  1499,  1385,  1387,  1293,   942,  1141,  1542,  1575,   401,
     402,   403,   690,   691,   710,   694,   695,   712,   789,   716,
     717,  1546,   576,   426,   568,   355,  1380,   569,   356,    78,
     118,   220,   351,    27,   162,   940,  1071,   941,    49,    50,
     133,    28,   155,   218,   345,  1072,   284,   285,    29,   111,
     723,  1289,   564,   347,   348,   115,   160,   727,    30,    76,
     219,   565,   718,   498,   416,   272,   273,   866,   886,   180,
     274,   682,  1234,   875,   579,   385,   275,   520,   276,   427,
     950,   521,   697,   506,  1050,   428,   951,   429,   952,   505,
    1049,   509,  1053,   510,  1236,   511,  1055,   512,  1237,   513,
    1057,   514,  1238,   515,  1060,   516,  1063,   692,    31,    55,
     286,   537,  1075,    32,    56,   287,   538,  1077,    33,    54,
     388,   708,  1243,   499,   630,  1512,   631,  1504,  1505,  1506,
     960,   500,   744,  1297,   745,  1298,   771,  1316,   768,  1314,
     758,   501,   769,  1315,   502,   964,  1394,   965,  1395,   966,
    1396,   754,  1307,   766,  1313,  1011,   503,   634,  1560,   703,
     504
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,    59,    70,   181,   182,   522,   732,   898,   810,   734,
     223,  1167,  1009,  1085,   743,   711,   805,   790,   792,    71,
      72,    73,  1268,   271,   127,  1308,   868,   684,   870,   686,
     872,   688,  -158,  1070,   709,   629,   985,  1067,   983,   430,
     431,   590,   525,   112,   113,    84,  1379,  1186,  1023,    90,
      70,    70,    70,  1197,    34,    35,  1476,  1330,  1526,   437,
     627,  1527,  1442,   277,  1137,   439,  1453,  1528,  1529,  1012,
     566,  1530,    96,   373,   102,   103,   104,    40,   179,   698,
      85,   343,  -717,   654,   655,   567,   880,   535,  1070,    70,
      70,    70,    70,   374,   119,   120,   398,   994,   527,   553,
       2,   398,   446,   447,   566,  1569,  1502,     3,  1527,   873,
    1417,  1522,   995,  1015,  1528,  1529,   167,   168,  1530,   567,
     652,    13,  1265,   654,   655,   572,  1454,  1266,   959,   878,
       4,    13,     5,   882,     6,   143,   449,   450,   591,   592,
       7,  1562,   554,   968,    51,   114,   399,   973,   221,   996,
       8,   675,   676,   221,   755,   981,     9,   375,   984,  1328,
      16,  1574,  1447,   270,   767,   352,   149,   770,  1605,   947,
      16,  1527,   349,    57,   156,   157,   557,  1528,  1529,  1582,
      10,  1530,  1014,    52,   376,   377,   105,    47,   526,  1138,
     970,   675,   676,    48,  1592,   804,  1329,    58,    36,   420,
    1187,   602,    11,    12,  1451,  1256,  1187,   558,    86,    77,
    1187,  1188,   106,   419,   583,  1187,   593,   421,  1242,   350,
      87,  1239,    15,  1178,  1386,   642,   643,   693,  1507,    13,
    1139,  1161,   474,   271,  1160,   121,   594,  1516,   862,   478,
     122,    74,   123,   786,   528,   124,    13,   786,   378,   149,
      80,   271,   379,   787,    79,   573,   372,   169,   386,    96,
    1464,   574,   170,   529,   171,    13,    80,   124,    16,    75,
     786,   271,   530,    81,   271,   271,   271,   786,   184,   125,
    1553,  1554,  1242,  1191,  1317,    16,    14,   570,  1449,   895,
     788,   560,   896,  1192,   788,   897,   756,   422,    15,   380,
     627,    13,   575,   381,    16,   986,   382,   958,    60,  1012,
     772,   227,  1204,   882,   577,   578,   580,   788,  1205,   828,
     386,  1016,   646,   647,   788,  1021,  1047,    82,   829,  1047,
     652,   383,   653,   654,   655,   656,   657,    61,  1151,   228,
      16,  1405,   642,   643,    89,    57,  1325,  1013,  1206,  1017,
    1348,  1047,   271,   271,  1048,  1064,   271,  1152,   271,  1207,
     271,  1370,   271,  1211,  1208,   786,  1047,  1076,   271,    58,
    1248,   787,  1061,   270,  1326,    95,   786,  1495,  1349,  1356,
     425,    62,  1331,  1145,   271,  1074,   670,   671,   672,   673,
     674,   270,   679,   680,  1363,  1156,   683,   786,   685,  1199,
     687,   675,   676,  1474,   105,  1201,    63,    96,   700,   566,
    1047,   270,   788,    57,   270,   270,   270,  1456,  1457,   878,
    1047,   584,   588,   788,   567,   101,   739,    13,  1612,  1410,
    1019,  1202,   946,  1466,  1467,   740,  1617,    58,  1365,   646,
     647,   128,   271,   874,   788,   954,   955,   652,  1411,   653,
     654,   655,   656,   657,  1047,   967,   271,  1047,   642,   643,
    1201,  1429,   975,   976,    64,   978,    16,   980,  1201,   982,
      98,    99,   100,  1375,    65,  1325,    87,   423,  1332,   102,
     424,   104,  1415,   425,   141,  1424,  1425,  1509,  1510,   815,
     819,   149,   270,   270,  1437,  1325,   270,    43,   270,   800,
     270,  1047,   270,  1446,   833,   672,   673,   674,   270,   145,
     146,   147,   148,  1047,  1342,  1201,  1051,  1523,   675,   676,
      44,    57,   859,  1472,   270,  1196,   518,  -780,   107,  1473,
     794,   375,  -780,   375,   108,   795,  1201,  1362,  1339,  1052,
    1212,  1536,  1483,  1576,   271,    58,   519,   116,   642,   643,
     724,  -780,  -787,   117,  1351,   646,   647,  -787,   376,   377,
     376,   377,   796,   652,  1603,   653,   654,   655,   656,   657,
    -794,   733,   739,    13,  -412,  -794,  -787,   134,   271,  -412,
    1003,   740,   270,   135,   700,   386,   595,   714,  1218,  1004,
     271,   271,   271,   271,  -794,   878,   270,   271,  -412,   130,
    1228,   271,   715,  1537,  1538,  1233,   596,   131,   271,   271,
     386,   271,    16,   271,   736,   271,   271,   725,   726,   797,
     603,   606,   378,    70,   378,   573,   379,    57,   379,   748,
     953,   574,   132,   956,   675,   676,   794,   963,   739,    13,
     604,   607,   153,   739,    13,   646,   647,   740,   154,   749,
    1563,    58,   740,   652,   279,   877,   654,   655,   656,   657,
     881,  1564,  1565,    13,   730,  1159,   137,   731,   138,   280,
     425,  1165,   575,   380,   281,   380,   282,   381,    16,   381,
     382,  1588,   382,    16,   270,   224,   225,  1034,  1241,   739,
    1036,   939,   739,   139,   642,   643,   386,  1041,   740,  1037,
     737,   740,    16,   739,   140,   383,  1042,   383,  1195,   887,
    1475,   887,   740,  1608,   161,   386,    85,   627,   270,   863,
     271,   912,   271,   271,   675,   676,  1012,  1100,  1101,   271,
     270,   270,   270,   270,   949,   271,   158,   270,  1383,  1320,
     176,   270,   159,   271,  1384,   177,  1327,   179,   270,   270,
     386,   270,   375,   270,   864,   270,   270,  1340,   386,   183,
    1059,   386,   867,  1062,   102,   869,   386,  1503,  1503,  1068,
     871,   185,   386,  1511,   217,  1094,  1150,  1503,  1511,   376,
     377,   386,   386,  1081,   386,  1158,  1433,   278,  1486,   644,
     645,   646,   647,   163,   164,   779,   780,   221,   283,   652,
     339,   653,   654,   655,   656,   657,   340,   658,   659,   912,
     341,   271,   271,  1547,  1494,   476,   638,   271,   639,   344,
    1503,  1503,   346,   353,   739,    13,   358,   739,    13,   102,
     103,   104,   354,   740,   359,   360,   740,   163,   164,   165,
     361,  1065,   362,   378,  1155,   363,   573,   379,   719,   720,
     721,   375,   574,  1515,   271,   670,   671,   672,   673,   674,
     270,   364,   270,   270,    16,   739,    13,    16,   366,   270,
     675,   676,   367,   368,   740,   270,   495,   369,   376,   377,
     370,   384,  1175,   270,  1496,   371,   517,  1073,  1593,  1073,
    1418,   386,  1549,   575,   380,   224,   225,   226,   381,   532,
     387,   382,    92,    93,    94,    16,  1090,   389,   390,   391,
     642,   643,  1097,  1098,  1352,   392,   393,   394,   739,    13,
     395,   396,   397,   404,   418,  1106,   383,   740,  1108,  1109,
    1110,  1111,  1112,  1541,   536,  1177,  1115,  1586,   533,   739,
      13,   507,   378,   508,   539,  1144,   379,  1147,   740,   552,
     561,   270,   270,   562,   556,   563,  1305,   270,    16,   571,
     559,   739,    13,  1465,   581,   582,   589,   610,   611,   597,
     740,   598,   599,  1609,   600,   601,   602,  1572,  1306,    16,
     605,   608,   739,    13,   619,   620,   621,   622,   623,  1292,
     609,   740,   632,   380,   270,   633,   693,   381,   612,   969,
     382,    16,  1491,   613,   614,   644,   645,   646,   647,   648,
     636,   615,   649,   650,   651,   652,   616,   653,   654,   655,
     656,   657,    16,   658,   659,   383,   408,   617,  1463,   635,
     409,   678,   702,   681,   704,   701,   728,   729,   738,   747,
     751,   752,   753,  1318,   410,   411,   782,   783,   707,   412,
     413,   414,   415,   784,   791,   785,   807,   793,   799,   826,
     271,   375,   271,   865,   876,   885,   375,   713,   667,   668,
     669,   670,   671,   672,   673,   674,  1350,   809,   893,   894,
     939,   944,   945,   961,   972,   998,   675,   676,   376,   377,
    1005,   735,  1002,   376,   377,  1006,  1024,  1044,  1018,   271,
    1066,  1020,  1354,  1025,  1026,  1027,   375,  1028,  1029,  1039,
    1040,   757,   760,  1045,  1054,   763,  1056,   765,   540,   541,
     542,   543,   544,   545,   546,   547,  1058,   773,   774,   775,
     776,   777,   778,   376,   377,  1069,   519,  1079,  1080,  1371,
    1088,  1089,  1091,   548,  1099,  1296,  1103,  1104,  1105,  1107,
    1113,  1143,   378,   549,   550,   551,   379,   378,  1134,   820,
     821,   379,  1164,   822,   823,   824,   825,  1153,   827,  1154,
     830,   831,   832,   834,   835,   836,   837,   838,   839,   841,
     842,   843,   844,   845,   846,   847,   848,   849,   850,   851,
    1163,   860,   375,  1168,  1173,  1179,  1540,   378,  1176,  1182,
     270,   379,   270,   380,  1183,   271,  1184,   381,   380,  1148,
     382,  1190,   381,  1198,  1149,   382,  1431,   883,  1200,   376,
     377,  1220,  1221,  1223,  1229,  1235,  1244,  1245,  1247,   271,
    1250,  1252,  1253,  1255,  1263,   383,  1254,   938,  1258,   270,
     383,  1290,  1259,   943,  1260,  1434,  1261,  1262,   380,   948,
    1291,  1294,   381,  1301,  1162,   382,  1300,  1302,  1322,  1345,
    1324,  1343,  1341,  1344,   962,  1346,  1347,  1366,  1368,  1450,
    1367,  1381,  1386,  1382,  1390,  1391,  1393,  1398,   642,   643,
     383,  1397,  1399,   378,  1404,  1403,  1406,   379,  1414,  1407,
    1416,  1423,  1388,  1426,  1389,   993,  1428,  1445,  1469,  1471,
     999,  1459,  1000,  1488,  1001,  1479,  1490,  1460,  1492,  1493,
    1498,  1461,  1462,  1470,  1201,   757,  1480,  1478,  1487,  1513,
    1514,   375,  1548,  1577,  1519,   938,   375,   -89,  1550,  1551,
    1555,  1568,  1580,  1556,   380,   126,  1557,  1585,   381,    20,
    1169,   382,   271,  1558,  1559,   270,  1578,  1579,   376,   377,
    1571,  1581,  1595,   376,   377,   781,  1583,  1607,    83,   -94,
    1604,   175,  1615,   129,  1616,   890,   383,  1614,  1038,   270,
     891,   144,  1043,   644,   645,   646,   647,   648,   806,  1448,
     649,   650,   651,   652,   357,   653,   654,   655,   656,   657,
    1140,   658,   659,   879,    26,   811,  1427,   660,   661,   662,
     640,  1570,  1501,   663,  1500,  1543,  1452,    91,  1078,   797,
    1544,  1545,   378,  1082,  1135,  1084,   379,   378,  1087,  1136,
     365,   379,    13,  1288,  1093,   417,  1096,  1517,  1584,     0,
     586,     0,   664,   759,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,     0,     0,   587,     0,     0,  1497,
       0,     0,     0,     0,   675,   676,     0,  1602,     0,     0,
       0,    16,  1146,   380,   797,     0,     0,   381,   380,  1170,
     382,  1611,   381,     0,  1171,   382,   375,     0,     0,     0,
       0,     0,   270,     0,     0,     0,  1166,   757,  1532,     0,
       0,     0,     0,   912,     0,   383,     0,     0,     0,     0,
     383,     0,     0,   376,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1193,  1194,     0,     0,   375,     0,     0,     0,     0,     0,
       0,   883,     0,     0,     0,     0,     0,     0,     0,     0,
    1214,     0,  1216,     0,  1219,     0,     0,     0,     0,     0,
    1222,   376,   377,     0,  1225,     0,     0,     0,     0,     0,
       0,     0,   883,     0,     0,     0,     0,   378,  1587,     0,
       0,   379,  1590,  1591,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1240,   912,     0,     0,  1601,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1249,     0,
       0,  1251,     0,     0,  1610,     0,     0,     0,   912,     0,
    1257,     0,     0,     0,     0,   378,     0,     0,   380,   379,
       0,     0,   381,     0,  1172,   382,     0,     0,     0,     0,
       0,  1269,  1270,  1271,  1272,  1273,  1274,  1275,  1276,  1277,
    1278,  1279,  1280,  1281,  1282,  1283,  1284,  1285,  1286,  1287,
     383,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1295,     0,     0,     0,     0,     0,   380,     0,     0,     0,
     381,     0,  1174,   382,     0,     0,     0,     0,     0,   757,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1323,   383,     0,
       0,     0,     0,     0,     0,  1333,  1334,  1335,     0,     0,
       0,     0,     0,     0,     0,   430,   431,     0,     0,     0,
       0,     0,     0,     0,     0,   432,   433,   434,   435,   436,
       0,     0,     0,     0,     0,   437,  1359,   438,  1360,     0,
       0,   439,   375,     0,  1364,     0,     0,     0,     0,   440,
       0,     0,     0,     0,     0,   441,     0,     0,   442,     0,
       0,   443,     0,  1369,     0,   444,     0,     0,  1372,   376,
     377,     0,     0,  1373,  1374,   445,  1376,  1377,   446,   447,
     801,   235,   236,   237,     0,   239,   240,   241,   242,   243,
     448,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,     0,   257,   258,   259,     0,   375,   262,   263,
     264,   265,   449,   450,   451,     0,  1392,     0,     0,     0,
       0,     0,     0,     0,     0,  1400,  1401,  1402,   452,   453,
       0,     0,  1409,   378,   376,   377,     0,   379,     0,     0,
       0,     0,     0,     0,     0,     0,  1419,  1420,     0,    57,
       0,     0,     0,     0,     0,     0,     0,   454,   455,   456,
     457,   458,     0,   459,   627,   460,   461,   462,   463,   464,
     465,   466,   467,   628,     0,     0,   468,     0,     0,     0,
       0,  1439,     0,     0,   380,     0,     0,     0,   381,     0,
    1304,   382,   469,   470,   471,     0,    14,   375,   378,   472,
     473,     0,   379,   642,   643,     0,     0,     0,   802,     0,
     475,   803,   476,   477,   757,   478,   383,     0,     0,     0,
       0,     0,     0,     0,   376,   377,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,     0,     0,     0,     0,
       0,     0,     0,  1482,     0,     0,     0,     0,     0,   380,
       0,     0,     0,   381,     0,  1309,   382,     0,     0,     0,
       0,     0,   376,   377,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1508,     0,     0,     0,     0,     0,
       0,   383,     0,     0,     0,  1518,     0,     0,   378,  1520,
    1521,     0,   379,   757,     0,     0,  1524,     0,   644,   645,
     646,   647,   648,  1531,     0,   649,   650,   651,   652,   938,
     653,   654,   655,   656,   657,     0,   658,   659,     0,     0,
       0,     0,   660,   661,   662,     0,   378,     0,   663,     0,
     379,     0,     0,     0,  1561,     0,     0,     0,     0,   380,
       0,     0,     0,   381,     0,  1338,   382,     0,     0,     0,
    1531,     0,     0,     0,     0,     0,  1573,   664,     0,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,     0,
       0,   383,     0,     0,     0,     0,     0,   380,     0,   675,
     676,   381,     0,  1432,   382,     0,  1559,  1589,     0,     0,
       0,     0,     0,     0,     0,  1594,     0,     0,     0,     0,
    1599,   938,   899,     0,     0,     0,   430,   431,     3,   383,
    -112,   -99,   -99,  1531,  -109,     0,   432,   433,   434,   435,
     436,     0,     0,     0,   938,     0,   437,   900,   438,   901,
     902,     0,   439,     0,     0,     0,     0,     0,     0,   903,
     440,   904,     0,  -114,     0,   905,   441,     0,     0,   442,
       0,     8,   443,   906,     0,   907,   444,     0,     0,   908,
     909,     0,     0,     0,     0,     0,   910,     0,     0,   446,
     447,     0,   235,   236,   237,     0,   239,   240,   241,   242,
     243,   448,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,     0,   257,   258,   259,     0,     0,   262,
     263,   264,   265,   449,   450,   451,   911,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   452,
     453,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,   454,   455,
     456,   457,   458,     0,   459,     0,   460,   461,   462,   463,
     464,   465,   466,   467,    58,     0,    13,   468,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   469,   470,   471,     0,    14,     0,     0,
     472,   473,     0,     0,     0,     0,     0,     0,     0,   474,
       0,   475,     0,   476,   477,    16,   478,  -273,   899,     0,
       0,     0,   430,   431,     3,     0,  -112,   -99,   -99,     0,
    -109,     0,   432,   433,   434,   435,   436,     0,     0,     0,
       0,     0,   437,   900,   438,   901,   902,     0,   439,     0,
       0,     0,     0,     0,     0,   903,   440,   904,     0,  -114,
       0,   905,   441,     0,     0,   442,     0,     8,   443,   906,
       0,   907,   444,     0,     0,   908,   909,     0,     0,     0,
       0,     0,   910,     0,     0,   446,   447,     0,   235,   236,
     237,     0,   239,   240,   241,   242,   243,   448,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,     0,
     257,   258,   259,     0,     0,   262,   263,   264,   265,   449,
     450,   451,   911,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   452,   453,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,   454,   455,   456,   457,   458,     0,
     459,     0,   460,   461,   462,   463,   464,   465,   466,   467,
      58,     0,    13,   468,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   469,
     470,   471,     0,    14,     0,     0,   472,   473,     0,     0,
       0,     0,     0,     0,     0,   474,     0,   475,     0,   476,
     477,    16,   478,  1022,   899,     0,     0,     0,   430,   431,
       3,     0,  -112,   -99,   -99,     0,  -109,     0,   432,   433,
     434,   435,   436,     0,     0,     0,     0,     0,   437,   900,
     438,   901,   902,     0,   439,     0,     0,     0,     0,     0,
       0,   903,   440,   904,     0,  -114,     0,   905,   441,     0,
       0,   442,     0,     8,   443,   906,     0,   907,   444,     0,
       0,   908,   909,     0,     0,     0,     0,     0,   910,     0,
       0,   446,   447,     0,   235,   236,   237,     0,   239,   240,
     241,   242,   243,   448,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,     0,   257,   258,   259,     0,
       0,   262,   263,   264,   265,   449,   450,   451,   911,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   452,   453,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
     454,   455,   456,   457,   458,     0,   459,     0,   460,   461,
     462,   463,   464,   465,   466,   467,    58,     0,    13,   468,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   469,   470,   471,     0,    14,
       0,     0,   472,   473,     0,     0,     0,     0,     0,     0,
       0,   474,     0,   475,     0,   476,   477,    16,   478,  -270,
     899,     0,     0,     0,   430,   431,     3,     0,  -112,   -99,
     -99,     0,  -109,     0,   432,   433,   434,   435,   436,     0,
       0,     0,     0,     0,   437,   900,   438,   901,   902,     0,
     439,     0,     0,     0,     0,     0,     0,   903,   440,   904,
       0,  -114,     0,   905,   441,     0,     0,   442,     0,     8,
     443,   906,     0,   907,   444,     0,     0,   908,   909,     0,
       0,     0,     0,     0,   910,     0,     0,   446,   447,     0,
     235,   236,   237,     0,   239,   240,   241,   242,   243,   448,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,     0,   257,   258,   259,     0,     0,   262,   263,   264,
     265,   449,   450,   451,   911,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   452,   453,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,   454,   455,   456,   457,
     458,     0,   459,     0,   460,   461,   462,   463,   464,   465,
     466,   467,    58,     0,    13,   468,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   469,   470,   471,     0,    14,     0,     0,   472,   473,
       0,     0,     0,     0,     0,     0,     0,   474,     0,   475,
       0,   476,   477,    16,   478,   -90,   899,     0,     0,     0,
     430,   431,     3,     0,  -112,   -99,   -99,     0,  -109,     0,
     432,   433,   434,   435,   436,     0,     0,     0,     0,     0,
     437,   900,   438,   901,   902,     0,   439,     0,     0,     0,
       0,     0,     0,   903,   440,   904,     0,  -114,     0,   905,
     441,     0,     0,   442,     0,     8,   443,   906,     0,   907,
     444,     0,     0,   908,   909,     0,     0,     0,     0,     0,
     910,     0,     0,   446,   447,     0,   235,   236,   237,     0,
     239,   240,   241,   242,   243,   448,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,     0,   257,   258,
     259,     0,     0,   262,   263,   264,   265,   449,   450,   451,
     911,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   452,   453,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,   454,   455,   456,   457,   458,     0,   459,     0,
     460,   461,   462,   463,   464,   465,   466,   467,    58,     0,
      13,   468,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   469,   470,   471,
       0,    14,     0,     0,   472,   473,     0,     0,     0,     0,
       0,     0,     0,   474,     0,   475,     0,   476,   477,    16,
     478,   -95,   430,   431,     0,     0,     0,     0,     0,     0,
       0,     0,   432,   433,   434,   435,   436,     0,     0,     0,
       0,     0,   437,   900,   438,   901,     0,     0,   439,     0,
       0,     0,     0,     0,     0,     0,   440,     0,     0,     0,
       0,     0,   441,     0,     0,   442,     0,     0,   443,   906,
       0,     0,   444,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   445,     0,     0,   446,   447,     0,   235,   236,
     237,     0,   239,   240,   241,   242,   243,   448,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,     0,
     257,   258,   259,     0,     0,   262,   263,   264,   265,   449,
     450,   451,   911,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   452,   453,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,   454,   455,   456,   457,   458,     0,
     459,     0,   460,   461,   462,   463,   464,   465,   466,   467,
      58,     0,     0,   468,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   469,
     470,   471,     0,    14,     0,     0,   472,   473,     0,     0,
       0,     0,     0,   430,   431,   474,     0,   475,     0,   476,
     477,   624,   478,   432,   433,   434,   435,   436,     0,     0,
       0,     0,     0,   437,     0,   438,     0,     0,     0,   439,
       0,     0,     0,     0,     0,     0,     0,   440,     0,     0,
       0,     0,     0,   441,     0,     0,   442,   625,     0,   443,
       0,     0,     0,   444,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   445,     0,     0,   446,   447,     0,   235,
     236,   237,     0,   239,   240,   241,   242,   243,   448,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
       0,   257,   258,   259,     0,     0,   262,   263,   264,   265,
     449,   450,   451,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   452,   453,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,   454,   455,   456,   457,   458,
       0,   459,   627,   460,   461,   462,   463,   464,   465,   466,
     467,   628,     0,     0,   468,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     469,   470,   471,     0,    14,     0,     0,   472,   473,     0,
       0,     0,     0,     0,   430,   431,   474,     0,   475,     0,
     476,   477,   624,   478,   432,   433,   434,   435,   436,     0,
       0,     0,     0,     0,   437,     0,   438,     0,     0,     0,
     439,     0,     0,     0,     0,     0,     0,     0,   440,     0,
       0,     0,     0,     0,   441,     0,     0,   442,   625,     0,
     443,     0,     0,     0,   444,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   445,     0,     0,   446,   447,     0,
     235,   236,   237,     0,   239,   240,   241,   242,   243,   448,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,     0,   257,   258,   259,     0,     0,   262,   263,   264,
     265,   449,   450,   451,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   452,   453,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,   454,   455,   456,   457,
     458,     0,   459,     0,   460,   461,   462,   463,   464,   465,
     466,   467,    58,     0,     0,   468,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   469,   470,   471,     0,    14,     0,     0,   472,   473,
       0,     0,     0,     0,     0,   430,   431,   474,     0,   475,
       0,   476,   477,     0,   478,   432,   433,   434,   435,   436,
       0,     0,     0,     0,     0,   437,     0,   438,     0,     0,
       0,   439,     0,     0,     0,     0,     0,     0,     0,   440,
       0,     0,     0,     0,     0,   441,     0,     0,   442,     0,
       0,   443,     0,     0,     0,   444,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   445,     0,     0,   446,   447,
     957,   235,   236,   237,     0,   239,   240,   241,   242,   243,
     448,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,     0,   257,   258,   259,     0,     0,   262,   263,
     264,   265,   449,   450,   451,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   452,   453,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,   454,   455,   456,
     457,   458,     0,   459,   627,   460,   461,   462,   463,   464,
     465,   466,   467,   628,     0,     0,   468,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   469,   470,   471,     0,    14,     0,     0,   472,
     473,     0,     0,     0,     0,     0,   430,   431,   474,     0,
     475,     0,   476,   477,     0,   478,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   438,     0,
       0,     0,   439,     0,     0,     0,     0,     0,     0,     0,
     440,     0,     0,     0,     0,     0,   441,     0,     0,   442,
       0,     0,   443,     0,     0,     0,   444,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   445,     0,     0,   446,
     447,     0,   235,   236,   237,     0,   239,   240,   241,   242,
     243,   448,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,     0,   257,   258,   259,     0,     0,   262,
     263,   264,   265,   449,   450,   451,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   452,
     453,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,   454,   455,
     456,   457,   458,     0,   459,     0,   460,   461,   462,   463,
     464,   465,   466,   467,    58,     0,     0,   468,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   469,   470,   471,     0,    14,     0,     0,
     472,   473,     0,     0,     0,     0,     0,   430,   431,   474,
     531,   475,     0,   476,   477,     0,   478,   432,   433,   434,
     435,   436,     0,     0,     0,     0,     0,   437,     0,   438,
       0,     0,     0,   439,     0,     0,     0,     0,     0,     0,
       0,   440,     0,     0,     0,     0,     0,   441,     0,     0,
     442,     0,     0,   443,     0,     0,     0,   444,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   445,     0,     0,
     446,   447,     0,   235,   236,   237,     0,   239,   240,   241,
     242,   243,   448,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,     0,   257,   258,   259,     0,     0,
     262,   263,   264,   265,   449,   450,   451,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     452,   453,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,   454,
     455,   456,   457,   458,     0,   459,   627,   460,   461,   462,
     463,   464,   465,   466,   467,   628,     0,     0,   468,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   469,   470,   471,     0,    14,     0,
       0,   472,   473,     0,     0,     0,     0,     0,   430,   431,
     474,     0,   475,     0,   476,   477,     0,   478,   432,   433,
     434,   435,   436,     0,     0,     0,     0,     0,   437,     0,
     438,     0,     0,     0,   439,     0,     0,     0,     0,     0,
       0,     0,   440,     0,     0,     0,     0,     0,   441,     0,
       0,   442,     0,     0,   443,     0,     0,     0,   444,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   445,     0,
       0,   446,   447,     0,   235,   236,   237,     0,   239,   240,
     241,   242,   243,   448,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,     0,   257,   258,   259,     0,
       0,   262,   263,   264,   265,   449,   450,   451,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   452,   453,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
     454,   455,   456,   457,   458,     0,   459,     0,   460,   461,
     462,   463,   464,   465,   466,   467,    58,     0,     0,   468,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   469,   470,   471,     0,    14,
       0,     0,   472,   473,     0,     0,     0,     0,     0,   430,
     431,   474,   706,   475,     0,   476,   477,     0,   478,   432,
     433,   434,   435,   436,     0,     0,   840,     0,     0,   437,
       0,   438,     0,     0,     0,   439,     0,     0,     0,     0,
       0,     0,     0,   440,     0,     0,     0,     0,     0,   441,
       0,     0,   442,     0,     0,   443,     0,     0,     0,   444,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   445,
       0,     0,   446,   447,     0,   235,   236,   237,     0,   239,
     240,   241,   242,   243,   448,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,     0,   257,   258,   259,
       0,     0,   262,   263,   264,   265,   449,   450,   451,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   452,   453,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,   454,   455,   456,   457,   458,     0,   459,     0,   460,
     461,   462,   463,   464,   465,   466,   467,    58,     0,     0,
     468,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   469,   470,   471,     0,
      14,     0,     0,   472,   473,     0,     0,     0,     0,     0,
     430,   431,   474,     0,   475,     0,   476,   477,     0,   478,
     432,   433,   434,   435,   436,     0,     0,     0,     0,     0,
     437,     0,   438,     0,     0,     0,   439,     0,     0,     0,
       0,     0,     0,     0,   440,     0,     0,     0,     0,     0,
     441,     0,     0,   442,     0,     0,   443,     0,     0,     0,
     444,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     445,     0,     0,   446,   447,     0,   235,   236,   237,     0,
     239,   240,   241,   242,   243,   448,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,     0,   257,   258,
     259,     0,     0,   262,   263,   264,   265,   449,   450,   451,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   452,   453,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,   454,   455,   456,   457,   458,     0,   459,     0,
     460,   461,   462,   463,   464,   465,   466,   467,    58,     0,
       0,   468,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   469,   470,   471,
       0,    14,     0,     0,   472,   473,     0,     0,     0,     0,
       0,   430,   431,   474,     0,   475,   861,   476,   477,     0,
     478,   432,   433,   434,   435,   436,     0,     0,     0,     0,
       0,   437,     0,   438,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,     0,   440,     0,     0,     0,     0,
       0,   441,     0,     0,   442,     0,     0,   443,     0,     0,
       0,   444,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   445,     0,     0,   446,   447,     0,   235,   236,   237,
       0,   239,   240,   241,   242,   243,   448,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,     0,   257,
     258,   259,     0,     0,   262,   263,   264,   265,   449,   450,
     451,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   452,   453,     0,     0,     0,     0,
       0,     0,     0,  1083,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,   454,   455,   456,   457,   458,     0,   459,
       0,   460,   461,   462,   463,   464,   465,   466,   467,    58,
       0,     0,   468,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   469,   470,
     471,     0,    14,     0,     0,   472,   473,     0,     0,     0,
       0,     0,   430,   431,   474,     0,   475,     0,   476,   477,
       0,   478,   432,   433,   434,   435,   436,     0,     0,     0,
       0,     0,   437,     0,   438,     0,     0,     0,   439,     0,
       0,     0,     0,     0,     0,     0,   440,     0,     0,     0,
       0,     0,   441,     0,     0,   442,     0,     0,   443,     0,
       0,     0,   444,     0,     0,     0,     0,     0,  1086,     0,
       0,     0,   445,     0,     0,   446,   447,     0,   235,   236,
     237,     0,   239,   240,   241,   242,   243,   448,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,     0,
     257,   258,   259,     0,     0,   262,   263,   264,   265,   449,
     450,   451,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   452,   453,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,   454,   455,   456,   457,   458,     0,
     459,     0,   460,   461,   462,   463,   464,   465,   466,   467,
      58,     0,     0,   468,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   469,
     470,   471,     0,    14,     0,     0,   472,   473,     0,     0,
       0,     0,     0,   430,   431,   474,     0,   475,     0,   476,
     477,     0,   478,   432,   433,   434,   435,   436,     0,     0,
       0,     0,     0,   437,     0,   438,     0,     0,     0,   439,
       0,     0,     0,     0,     0,     0,     0,   440,     0,     0,
       0,     0,     0,   441,     0,     0,   442,     0,     0,   443,
       0,     0,     0,   444,     0,     0,  1092,     0,     0,     0,
       0,     0,     0,   445,     0,     0,   446,   447,     0,   235,
     236,   237,     0,   239,   240,   241,   242,   243,   448,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
       0,   257,   258,   259,     0,     0,   262,   263,   264,   265,
     449,   450,   451,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   452,   453,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,   454,   455,   456,   457,   458,
       0,   459,     0,   460,   461,   462,   463,   464,   465,   466,
     467,    58,     0,     0,   468,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     469,   470,   471,     0,    14,     0,     0,   472,   473,     0,
       0,     0,     0,     0,   430,   431,   474,     0,   475,     0,
     476,   477,     0,   478,   432,   433,   434,   435,   436,     0,
       0,     0,     0,     0,   437,     0,   438,     0,     0,     0,
     439,     0,     0,     0,     0,     0,     0,     0,   440,     0,
       0,     0,     0,     0,   441,     0,     0,   442,     0,     0,
     443,     0,     0,     0,   444,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   445,     0,     0,   446,   447,     0,
     235,   236,   237,     0,   239,   240,   241,   242,   243,   448,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,     0,   257,   258,   259,     0,     0,   262,   263,   264,
     265,   449,   450,   451,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   452,   453,     0,
       0,     0,     0,     0,     0,     0,  1095,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,   454,   455,   456,   457,
     458,     0,   459,     0,   460,   461,   462,   463,   464,   465,
     466,   467,    58,     0,     0,   468,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   469,   470,   471,     0,    14,     0,     0,   472,   473,
       0,     0,     0,     0,     0,   430,   431,   474,     0,   475,
       0,   476,   477,     0,   478,   432,   433,   434,   435,   436,
       0,     0,     0,     0,     0,   437,     0,   438,     0,     0,
       0,   439,     0,     0,     0,     0,     0,     0,     0,   440,
       0,     0,     0,     0,     0,   441,     0,     0,   442,     0,
       0,   443,     0,     0,     0,   444,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   445,     0,     0,   446,   447,
       0,   235,   236,   237,     0,   239,   240,   241,   242,   243,
     448,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,     0,   257,   258,   259,     0,     0,   262,   263,
     264,   265,   449,   450,   451,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   452,   453,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,   454,   455,   456,
     457,   458,     0,   459,     0,   460,   461,   462,   463,   464,
     465,   466,   467,    58,     0,     0,   468,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   469,   470,   471,     0,    14,     0,     0,   472,
     473,     0,     0,     0,     0,     0,   430,   431,   474,     0,
     475,  1217,   476,   477,     0,   478,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   438,     0,
       0,     0,   439,     0,     0,     0,     0,     0,     0,     0,
     440,     0,     0,     0,     0,     0,   441,     0,     0,   442,
       0,     0,   443,     0,     0,     0,   444,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   445,     0,     0,   446,
     447,     0,   235,   236,   237,     0,   239,   240,   241,   242,
     243,   448,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,     0,   257,   258,   259,     0,     0,   262,
     263,   264,   265,   449,   450,   451,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   452,
     453,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,   454,   455,
     456,   457,   458,     0,   459,     0,   460,   461,   462,   463,
     464,   465,   466,   467,    58,     0,     0,   468,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   469,   470,   471,     0,    14,     0,     0,
     472,   473,     0,     0,     0,     0,     0,   430,   431,  1226,
       0,   475,  1227,   476,   477,     0,   478,   432,   433,   434,
     435,   436,     0,     0,     0,     0,     0,   437,     0,   438,
       0,     0,     0,   439,     0,     0,     0,     0,     0,     0,
       0,   440,     0,     0,     0,     0,     0,   441,     0,     0,
     442,     0,     0,   443,     0,     0,     0,   444,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   445,     0,     0,
     446,   447,     0,   235,   236,   237,     0,   239,   240,   241,
     242,   243,   448,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,     0,   257,   258,   259,     0,     0,
     262,   263,   264,   265,   449,   450,   451,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     452,   453,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,   454,
     455,   456,   457,   458,     0,   459,     0,   460,   461,   462,
     463,   464,   465,   466,   467,    58,     0,     0,   468,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   469,   470,   471,     0,    14,     0,
       0,   472,   473,     0,     0,     0,     0,     0,   430,   431,
     474,     0,   475,  1232,   476,   477,     0,   478,   432,   433,
     434,   435,   436,     0,     0,     0,     0,     0,   437,     0,
     438,     0,     0,     0,   439,     0,     0,     0,     0,     0,
       0,     0,   440,     0,     0,     0,     0,     0,   441,     0,
       0,   442,     0,     0,   443,     0,     0,     0,   444,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   445,     0,
       0,   446,   447,     0,   235,   236,   237,     0,   239,   240,
     241,   242,   243,   448,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,     0,   257,   258,   259,     0,
       0,   262,   263,   264,   265,   449,   450,   451,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   452,   453,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
     454,   455,   456,   457,   458,     0,   459,     0,   460,   461,
     462,   463,   464,   465,   466,   467,    58,     0,     0,   468,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   469,   470,   471,     0,    14,
       0,     0,   472,   473,     0,     0,     0,     0,     0,   430,
     431,   474,     0,   475,  1319,   476,   477,     0,   478,   432,
     433,   434,   435,   436,     0,     0,     0,     0,     0,   437,
       0,   438,     0,     0,     0,   439,     0,     0,     0,     0,
       0,     0,     0,   440,     0,     0,     0,     0,     0,   441,
       0,     0,   442,     0,     0,   443,     0,     0,     0,   444,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   445,
       0,     0,   446,   447,     0,   235,   236,   237,     0,   239,
     240,   241,   242,   243,   448,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,     0,   257,   258,   259,
       0,     0,   262,   263,   264,   265,   449,   450,   451,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   452,   453,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,   454,   455,   456,   457,   458,     0,   459,     0,   460,
     461,   462,   463,   464,   465,   466,   467,    58,     0,     0,
     468,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   469,   470,   471,     0,
      14,     0,     0,   472,   473,     0,     0,     0,     0,     0,
     430,   431,   474,     0,   475,  1408,   476,   477,     0,   478,
     432,   433,   434,   435,   436,     0,     0,     0,     0,     0,
     437,     0,   438,     0,     0,     0,   439,     0,     0,     0,
       0,     0,     0,     0,   440,     0,     0,     0,     0,     0,
     441,     0,     0,   442,     0,     0,   443,     0,     0,     0,
     444,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     445,     0,     0,   446,   447,     0,   235,   236,   237,     0,
     239,   240,   241,   242,   243,   448,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,     0,   257,   258,
     259,     0,     0,   262,   263,   264,   265,   449,   450,   451,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   452,   453,     0,     0,     0,     0,     0,
       0,     0,  1481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,   454,   455,   456,   457,   458,     0,   459,     0,
     460,   461,   462,   463,   464,   465,   466,   467,    58,     0,
       0,   468,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   469,   470,   471,
       0,    14,     0,     0,   472,   473,     0,     0,     0,     0,
       0,   430,   431,   474,     0,   475,     0,   476,   477,     0,
     478,   432,   433,   434,   435,   436,     0,     0,     0,     0,
       0,   437,     0,   438,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,     0,   440,     0,     0,     0,     0,
       0,   441,     0,     0,   442,     0,     0,   443,     0,     0,
       0,   444,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   445,     0,     0,   446,   447,     0,   235,   236,   237,
       0,   239,   240,   241,   242,   243,   448,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,     0,   257,
     258,   259,     0,     0,   262,   263,   264,   265,   449,   450,
     451,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   452,   453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,   454,   455,   456,   457,   458,     0,   459,
       0,   460,   461,   462,   463,   464,   465,   466,   467,    58,
       0,     0,   468,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   469,   470,
     471,     0,    14,     0,     0,   472,   473,     0,     0,     0,
       0,     0,   430,   431,   474,     0,   475,     0,   476,   477,
       0,   478,   432,   433,   434,   435,   436,     0,     0,     0,
       0,     0,   437,     0,   438,     0,     0,     0,   439,     0,
       0,     0,     0,     0,     0,     0,   440,     0,     0,     0,
       0,     0,   441,     0,     0,   442,     0,     0,   443,     0,
       0,     0,   444,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   445,     0,     0,   446,   447,     0,   235,   236,
     237,     0,   239,   240,   241,   242,   243,   448,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,     0,
     257,   258,   259,     0,     0,   262,   263,   264,   265,   449,
     450,   451,     0,     0,     0,     0,   -80,     0,     0,     0,
       0,     0,     0,     0,     0,   452,   453,   642,   643,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,   454,   455,   456,   457,   458,     0,
     459,     0,   460,   461,   462,   463,   464,   465,   466,   467,
      58,     0,     0,   468,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   469,
     470,   471,     0,    14,     0,     0,   472,   473,     0,     0,
       0,     0,     0,     0,     0,  1203,     0,   475,     0,   476,
     477,     0,   478,     0,  1116,  1117,  1118,  1119,  1120,  1121,
    1122,  1123,   644,   645,   646,   647,   648,  1124,  1125,   649,
     650,   651,   652,  1126,   653,   654,   655,   656,   657,  1127,
     658,   659,  1128,  1129,     0,     0,   660,   661,   662,  1130,
    1131,  1132,   663,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1133,   664,     0,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   229,     0,     0,     0,     0,     0,   230,
       0,     0,     0,   675,   676,   231,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   232,     0,     0,     0,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   234,     0,
       0,     0,     0,     0,     0,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,     0,     0,
       0,     0,     0,     0,     0,     0,   229,     0,     0,     0,
       0,     0,   230,     0,     0,     0,     0,     0,   231,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   232,     0,
       0,     0,     0,    57,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   268,     0,     0,     0,
       0,   234,     0,     0,     0,     0,     0,    58,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,     0,     0,     0,     0,     0,   269,     0,     0,     0,
       0,   534,   229,     0,     0,     0,     0,     0,   230,     0,
       0,     0,     0,     0,   231,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   232,     0,    57,     0,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,     0,   268,
       0,     0,     0,     0,     0,     0,     0,   234,     0,     0,
     689,     0,    13,     0,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,     0,     0,   269,
       0,    16,   642,   643,     0,   229,     0,     0,     0,     0,
       0,   230,     0,     0,     0,     0,     0,   231,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   232,     0,     0,
       0,     0,    57,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   268,     0,     0,     0,     0,
     234,     0,     0,     0,     0,     0,    58,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
       0,     0,     0,     0,     0,   269,     0,   644,   645,   646,
     647,   648,   642,   643,   649,   650,   651,   652,     0,   653,
     654,   655,   656,   657,     0,   658,   659,     0,     0,     0,
       0,   660,   661,   662,     0,    57,     0,   663,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   268,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   689,
       0,     0,     0,     0,     0,     0,   664,     0,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,     0,     0,
       0,     0,   812,     0,     0,     0,     0,     0,   675,   676,
       0,     0,   677,     0,     0,     0,     0,     0,   269,   642,
     643,     0,     0,     0,     0,     0,     0,   644,   645,   646,
     647,   648,     0,     0,   649,   650,   651,   652,     0,   653,
     654,   655,   656,   657,     0,   658,   659,     0,     0,     0,
       0,   660,   661,   662,   235,   236,   237,   663,   239,   240,
     241,   242,   243,   448,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,     0,   257,   258,   259,     0,
       0,   262,   263,   264,   265,     0,   664,     0,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,     0,     0,
       0,     0,     0,     0,     0,   642,   643,     0,   675,   676,
       0,     0,   696,     0,   644,   645,   646,   647,   648,     0,
       0,   649,   650,   651,   652,     0,   653,   654,   655,   656,
     657,     0,   658,   659,     0,     0,     0,   813,   660,   661,
     662,     0,     0,     0,   663,     0,   814,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   664,     0,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,     0,     0,     0,     0,     0,
       0,   642,   643,     0,     0,   675,   676,     0,     0,   892,
     644,   645,   646,   647,   648,     0,     0,   649,   650,   651,
     652,     0,   653,   654,   655,   656,   657,     0,   658,   659,
       0,     0,     0,     0,   660,   661,   662,     0,     0,     0,
     663,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   664,
       0,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,     0,     0,     0,     0,     0,     0,   642,   643,     0,
       0,   675,   676,     0,     0,   974,   644,   645,   646,   647,
     648,     0,     0,   649,   650,   651,   652,     0,   653,   654,
     655,   656,   657,     0,   658,   659,     0,     0,     0,     0,
     660,   661,   662,     0,     0,     0,   663,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   664,     0,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,     0,     0,     0,
       0,     0,     0,   642,   643,     0,     0,   675,   676,     0,
       0,   977,   644,   645,   646,   647,   648,     0,     0,   649,
     650,   651,   652,     0,   653,   654,   655,   656,   657,     0,
     658,   659,     0,     0,     0,     0,   660,   661,   662,     0,
       0,     0,   663,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   664,     0,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,     0,     0,     0,     0,     0,     0,   642,
     643,     0,     0,   675,   676,     0,     0,   979,   644,   645,
     646,   647,   648,     0,     0,   649,   650,   651,   652,     0,
     653,   654,   655,   656,   657,     0,   658,   659,     0,     0,
       0,     0,   660,   661,   662,     0,     0,     0,   663,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   664,     0,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,     0,
       0,     0,     0,     0,     0,   642,   643,     0,     0,   675,
     676,     0,     0,   987,   644,   645,   646,   647,   648,     0,
       0,   649,   650,   651,   652,     0,   653,   654,   655,   656,
     657,     0,   658,   659,     0,     0,     0,     0,   660,   661,
     662,     0,     0,     0,   663,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   664,     0,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,     0,     0,     0,     0,     0,
       0,   642,   643,     0,     0,   675,   676,     0,     0,   988,
     644,   645,   646,   647,   648,     0,     0,   649,   650,   651,
     652,     0,   653,   654,   655,   656,   657,     0,   658,   659,
       0,     0,     0,     0,   660,   661,   662,     0,     0,     0,
     663,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   664,
       0,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,     0,     0,     0,     0,     0,     0,   642,   643,     0,
       0,   675,   676,     0,     0,   989,   644,   645,   646,   647,
     648,     0,     0,   649,   650,   651,   652,     0,   653,   654,
     655,   656,   657,     0,   658,   659,     0,     0,     0,     0,
     660,   661,   662,     0,     0,     0,   663,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   664,     0,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,     0,     0,     0,
       0,     0,     0,   642,   643,     0,     0,   675,   676,     0,
       0,   990,   644,   645,   646,   647,   648,     0,     0,   649,
     650,   651,   652,     0,   653,   654,   655,   656,   657,     0,
     658,   659,     0,     0,     0,     0,   660,   661,   662,     0,
       0,     0,   663,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   664,     0,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,     0,     0,     0,     0,     0,     0,   642,
     643,     0,     0,   675,   676,     0,     0,   991,   644,   645,
     646,   647,   648,     0,     0,   649,   650,   651,   652,     0,
     653,   654,   655,   656,   657,     0,   658,   659,     0,     0,
       0,     0,   660,   661,   662,     0,     0,     0,   663,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   664,     0,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,     0,
       0,     0,     0,     0,     0,   642,   643,     0,     0,   675,
     676,     0,     0,   992,   644,   645,   646,   647,   648,     0,
       0,   649,   650,   651,   652,     0,   653,   654,   655,   656,
     657,     0,   658,   659,     0,     0,     0,     0,   660,   661,
     662,     0,     0,     0,   663,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   664,     0,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,     0,     0,     0,     0,     0,
       0,   642,   643,     0,     0,   675,   676,     0,     0,  1142,
     644,   645,   646,   647,   648,     0,     0,   649,   650,   651,
     652,     0,   653,   654,   655,   656,   657,     0,   658,   659,
       0,     0,     0,     0,   660,   661,   662,     0,     0,     0,
     663,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   664,
       0,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,     0,     0,     0,     0,     0,     0,   642,   643,     0,
       0,   675,   676,     0,     0,  1157,   644,   645,   646,   647,
     648,     0,     0,   649,   650,   651,   652,     0,   653,   654,
     655,   656,   657,     0,   658,   659,     0,     0,     0,     0,
     660,   661,   662,     0,     0,     0,   663,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   664,     0,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,     0,     0,     0,
       0,     0,     0,   642,   643,     0,     0,   675,   676,     0,
       0,  1189,   644,   645,   646,   647,   648,     0,     0,   649,
     650,   651,   652,     0,   653,   654,   655,   656,   657,     0,
     658,   659,     0,     0,     0,     0,   660,   661,   662,     0,
       0,     0,   663,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   664,     0,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,     0,     0,     0,     0,     0,     0,   642,
     643,     0,     0,   675,   676,     0,     0,  1246,   644,   645,
     646,   647,   648,     0,     0,   649,   650,   651,   652,     0,
     653,   654,   655,   656,   657,     0,   658,   659,     0,     0,
       0,     0,   660,   661,   662,     0,     0,     0,   663,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   664,     0,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,     0,
       0,     0,     0,     0,     0,   642,   643,     0,     0,   675,
     676,     0,     0,  1336,   644,   645,   646,   647,   648,     0,
       0,   649,   650,   651,   652,     0,   653,   654,   655,   656,
     657,     0,   658,   659,     0,     0,     0,     0,   660,   661,
     662,     0,     0,     0,   663,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   664,     0,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,     0,     0,     0,     0,     0,
       0,   642,   643,     0,     0,   675,   676,     0,     0,  1337,
     644,   645,   646,   647,   648,     0,     0,   649,   650,   651,
     652,     0,   653,   654,   655,   656,   657,     0,   658,   659,
       0,     0,     0,     0,   660,   661,   662,     0,     0,     0,
     663,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   664,
       0,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,     0,     0,     0,     0,     0,     0,   642,   643,     0,
       0,   675,   676,     0,     0,  1353,   644,   645,   646,   647,
     648,     0,     0,   649,   650,   651,   652,     0,   653,   654,
     655,   656,   657,     0,   658,   659,     0,     0,     0,     0,
     660,   661,   662,     0,     0,     0,   663,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   664,     0,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,     0,     0,     0,
       0,     0,     0,   642,   643,     0,     0,   675,   676,     0,
       0,  1355,   644,   645,   646,   647,   648,     0,     0,   649,
     650,   651,   652,     0,   653,   654,   655,   656,   657,     0,
     658,   659,     0,     0,     0,     0,   660,   661,   662,     0,
       0,     0,   663,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   664,     0,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,     0,     0,     0,     0,     0,     0,   642,
     643,     0,     0,   675,   676,     0,     0,  1357,   644,   645,
     646,   647,   648,     0,     0,   649,   650,   651,   652,     0,
     653,   654,   655,   656,   657,     0,   658,   659,     0,     0,
       0,     0,   660,   661,   662,     0,     0,     0,   663,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   664,     0,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,     0,
       0,     0,     0,     0,     0,   642,   643,     0,     0,   675,
     676,     0,     0,  1361,   644,   645,   646,   647,   648,     0,
       0,   649,   650,   651,   652,     0,   653,   654,   655,   656,
     657,     0,   658,   659,     0,     0,     0,     0,   660,   661,
     662,     0,     0,     0,   663,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   664,     0,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,     0,     0,     0,     0,     0,
       0,   642,   643,     0,     0,   675,   676,     0,     0,  1413,
     644,   645,   646,   647,   648,     0,     0,   649,   650,   651,
     652,     0,   653,   654,   655,   656,   657,     0,   658,   659,
       0,     0,     0,     0,   660,   661,   662,     0,     0,     0,
     663,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   664,
       0,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,     0,     0,     0,     0,     0,     0,   642,   643,     0,
       0,   675,   676,     0,     0,  1421,   644,   645,   646,   647,
     648,     0,     0,   649,   650,   651,   652,     0,   653,   654,
     655,   656,   657,     0,   658,   659,     0,     0,     0,     0,
     660,   661,   662,     0,     0,     0,   663,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   664,     0,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,     0,     0,     0,
       0,     0,     0,   642,   643,     0,     0,   675,   676,     0,
       0,  1435,   644,   645,   646,   647,   648,     0,     0,   649,
     650,   651,   652,     0,   653,   654,   655,   656,   657,     0,
     658,   659,     0,     0,     0,     0,   660,   661,   662,     0,
       0,     0,   663,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   664,     0,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,     0,     0,     0,     0,     0,     0,   642,
     643,     0,     0,   675,   676,     0,     0,  1436,   644,   645,
     646,   647,   648,     0,     0,   649,   650,   651,   652,     0,
     653,   654,   655,   656,   657,     0,   658,   659,     0,     0,
       0,     0,   660,   661,   662,     0,     0,     0,   663,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   664,     0,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,     0,
       0,     0,     0,     0,     0,   642,   643,     0,     0,   675,
     676,     0,     0,  1438,   644,   645,   646,   647,   648,     0,
       0,   649,   650,   651,   652,     0,   653,   654,   655,   656,
     657,     0,   658,   659,     0,     0,     0,     0,   660,   661,
     662,     0,     0,     0,   663,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   664,     0,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,     0,     0,     0,     0,     0,
       0,   642,   643,     0,     0,   675,   676,     0,     0,  1440,
     644,   645,   646,   647,   648,     0,     0,   649,   650,   651,
     652,     0,   653,   654,   655,   656,   657,     0,   658,   659,
       0,     0,     0,     0,   660,   661,   662,     0,     0,     0,
     663,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   664,
       0,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,     0,     0,     0,     0,     0,     0,   642,   643,     0,
       0,   675,   676,     0,     0,  1441,   644,   645,   646,   647,
     648,     0,     0,   649,   650,   651,   652,     0,   653,   654,
     655,   656,   657,     0,   658,   659,     0,     0,     0,     0,
     660,   661,   662,     0,     0,     0,   663,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   664,     0,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,     0,     0,     0,
       0,     0,     0,   642,   643,     0,     0,   675,   676,     0,
       0,  1443,   644,   645,   646,   647,   648,     0,     0,   649,
     650,   651,   652,     0,   653,   654,   655,   656,   657,     0,
     658,   659,     0,     0,     0,     0,   660,   661,   662,     0,
       0,     0,   663,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   664,     0,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,     0,     0,     0,     0,     0,     0,   642,
     643,     0,     0,   675,   676,     0,     0,  1444,   644,   645,
     646,   647,   648,     0,     0,   649,   650,   651,   652,     0,
     653,   654,   655,   656,   657,     0,   658,   659,     0,     0,
       0,     0,   660,   661,   662,     0,     0,     0,   663,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   664,     0,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,     0,
       0,     0,     0,     0,     0,   642,   643,     0,     0,   675,
     676,     0,     0,  1458,   644,   645,   646,   647,   648,     0,
       0,   649,   650,   651,   652,     0,   653,   654,   655,   656,
     657,     0,   658,   659,     0,     0,     0,     0,   660,   661,
     662,     0,     0,     0,   663,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   664,     0,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,     0,     0,     0,     0,     0,
       0,   642,   643,     0,     0,   675,   676,     0,     0,  1468,
     644,   645,   646,   647,   648,     0,     0,   649,   650,   651,
     652,     0,   653,   654,   655,   656,   657,     0,   658,   659,
       0,     0,     0,     0,   660,   661,   662,     0,     0,     0,
     663,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   664,
       0,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,     0,     0,     0,     0,     0,     0,   642,   643,     0,
       0,   675,   676,     0,     0,  1552,   644,   645,   646,   647,
     648,     0,     0,   649,   650,   651,   652,     0,   653,   654,
     655,   656,   657,     0,   658,   659,     0,     0,     0,     0,
     660,   661,   662,     0,     0,     0,   663,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   664,     0,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,     0,     0,     0,
       0,   642,   643,     0,     0,     0,     0,   675,   676,     0,
       0,  1606,   644,   645,   646,   647,   648,     0,     0,   649,
     650,   651,   652,     0,   653,   654,   655,   656,   657,     0,
     658,   659,     0,     0,     0,     0,   660,   661,   662,     0,
       0,     0,   663,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   664,     0,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,     0,     0,   642,   643,     0,     0,     0,
       0,     0,     0,   675,   676,   705,   644,   645,   646,   647,
     648,     0,     0,   649,   650,   651,   652,     0,   653,   654,
     655,   656,   657,     0,   658,   659,     0,     0,     0,     0,
     660,   661,   662,     0,     0,     0,   663,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   664,     0,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,     0,     0,   642,
     643,     0,     0,     0,     0,     0,     0,   675,   676,   884,
     644,   645,   646,   647,   648,     0,     0,   649,   650,   651,
     652,     0,   653,   654,   655,   656,   657,     0,   658,   659,
       0,     0,     0,     0,   660,   661,   662,     0,     0,     0,
     663,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   664,
       0,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,     0,     0,   642,   643,     0,     0,     0,     0,     0,
       0,   675,   676,  1030,   644,   645,   646,   647,   648,     0,
       0,   649,   650,   651,   652,     0,   653,   654,   655,   656,
     657,     0,   658,   659,     0,     0,     0,     0,   660,   661,
     662,     0,     0,     0,   663,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   664,     0,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,     0,     0,   642,   643,   186,
       0,     0,     0,     0,     0,   675,   676,  1046,   644,   645,
     646,   647,   648,     0,     0,   649,   650,   651,   652,     0,
     653,   654,   655,   656,   657,     0,   658,   659,     0,     0,
       0,     0,   660,   661,   662,   187,     0,   188,   663,   189,
     190,   191,   192,   193,     0,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,     0,   205,   206,   207,
       0,     0,   208,   209,   210,   211,     0,   664,     0,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,     0,
       0,     0,   212,   213,     0,     0,     0,     0,     0,   675,
     676,  1224,   644,   645,   646,   647,   648,   288,   289,   649,
     650,   651,   652,     0,   653,   654,   655,   656,   657,     0,
     658,   659,     0,     0,   290,     0,   660,   661,   662,     0,
       0,     0,   663,     0,     0,     0,     0,   214,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   664,     0,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   675,   676,  1230,     0,     0,     0,     0,
     642,   643,     0,     0,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,     0,     0,   309,   310,   311,     0,     0,   312,
     313,   314,   315,   316,     0,     0,   317,   318,   319,   320,
     321,   322,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     816,   324,     0,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   642,   643,   335,   336,     0,     0,     0,
       0,     0,     0,   337,   338,   644,   645,   646,   647,   648,
       0,     0,   649,   650,   651,   652,     0,   653,   654,   655,
     656,   657,     0,   658,   659,     0,     0,   971,     0,   660,
     661,   662,   235,   236,   237,   663,   239,   240,   241,   242,
     243,   448,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,     0,   257,   258,   259,     0,     0,   262,
     263,   264,   265,     0,   664,     0,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   642,   643,     0,     0,
       0,     0,     0,     0,     0,     0,   675,   676,   644,   645,
     646,   647,   648,     0,     0,   649,   650,   651,   652,     0,
     653,   654,   655,   656,   657,     0,   658,   659,     0,     0,
       0,     0,   660,   661,   662,   817,     0,     0,   663,     0,
       0,     0,     0,     0,   818,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1031,   664,  1035,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   642,
     643,     0,     0,     0,     0,     0,     0,     0,     0,   675,
     676,   644,   645,   646,   647,   648,     0,     0,   649,   650,
     651,   652,     0,   653,   654,   655,   656,   657,     0,   658,
     659,     0,     0,     0,     0,   660,   661,   662,   235,   236,
     237,   663,   239,   240,   241,   242,   243,   448,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,     0,
     257,   258,   259,     0,     0,   262,   263,   264,   265,     0,
     664,  1180,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   642,   643,     0,     0,     0,     0,     0,     0,
       0,     0,   675,   676,   644,   645,   646,   647,   648,     0,
       0,   649,   650,   651,   652,     0,   653,   654,   655,   656,
     657,     0,   658,   659,   642,   643,     0,     0,   660,   661,
     662,  1032,     0,     0,   663,     0,     0,     0,     0,     0,
    1033,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   664,     0,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   675,   676,   644,   645,   646,
     647,   648,     0,     0,   649,   650,   651,   652,     0,   653,
     654,   655,   656,   657,     0,   658,   659,   642,   643,     0,
       0,   660,   661,   662,     0,     0,     0,  -795,     0,   644,
     645,   646,   647,   648,     0,     0,   649,   650,   651,   652,
       0,   653,   654,   655,   656,   657,     0,   658,   659,   642,
     643,     0,     0,   660,   661,   662,   664,     0,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   675,   676,
       0,     0,     0,     0,     0,     0,     0,     0,   664,     0,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     675,   676,   644,   645,   646,   647,   648,     0,     0,   649,
     650,   651,   652,     0,   653,   654,   655,   656,   657,     0,
     658,   659,   642,   643,     0,     0,   660,     0,   662,     0,
       0,     0,     0,     0,   644,   645,   646,   647,   648,     0,
       0,   649,   650,   651,   652,     0,   653,   654,   655,   656,
     657,     0,   658,   659,   642,   643,     0,     0,   660,     0,
       0,     0,     0,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   675,   676,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   675,   676,   644,   645,   646,
     647,   648,     0,     0,   649,   650,   651,   652,     0,   653,
     654,   655,   656,   657,     0,   658,   659,   642,   643,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   644,
     645,   646,   647,   648,     0,     0,   649,   650,   651,   652,
       0,   653,   654,   655,   656,   657,     0,   658,   659,   642,
     643,     0,     0,     0,     0,     0,     0,     0,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   675,   676,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   666,   667,   668,   669,   670,   671,   672,   673,   674,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     675,   676,   644,   645,   646,   647,   648,     0,     0,   649,
     650,   651,   652,     0,   653,   654,   655,   656,   657,     0,
     658,   659,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   644,   645,   646,   647,   648,     0,
       0,   649,     0,     0,   652,     0,   653,   654,   655,   656,
     657,     0,   658,   659,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   668,   669,   670,   671,
     672,   673,   674,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   675,   676,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   668,   669,
     670,   671,   672,   673,   674,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   675,   676,   235,   236,   237,
       0,   239,   240,   241,   242,   243,   448,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,     0,   257,
     258,   259,     0,     0,   262,   263,   264,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   852,   853,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     854,     0,     0,     0,     0,     0,     0,     0,     0,   855,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   856,   857
};

static const yytype_int16 yycheck[] =
{
       1,    14,    15,   138,   139,   374,   568,   718,   639,   572,
     162,   972,   799,   902,   580,   538,   635,   629,   630,    20,
      21,    22,  1114,   176,    80,  1164,   684,   510,   686,   512,
     688,   514,     8,   886,   537,   475,   771,   881,   769,     5,
       6,     5,    33,     5,     6,    22,  1265,    20,   811,    50,
      63,    64,    65,    20,    19,    20,  1422,    20,  1488,    25,
     154,  1488,    20,   176,    40,    31,    46,  1488,  1488,   163,
     148,  1488,   139,   180,   140,   141,   142,   156,   176,   519,
      57,   216,   180,   129,   130,   163,   698,   386,   941,   102,
     103,   104,   105,   200,    15,    16,   151,   148,    33,     7,
       0,   151,    68,    69,   148,  1535,   200,     7,  1535,   176,
    1329,  1477,   163,   802,  1535,  1535,    15,    16,  1535,   163,
     126,   165,   200,   129,   130,   424,   106,   205,   747,   695,
      30,   165,    32,   699,    34,   201,   102,   103,   102,   103,
      40,  1523,    50,   755,   163,   107,   201,   759,   203,   200,
      50,   197,   198,   203,   594,   767,    56,    33,   770,   163,
     204,  1542,   206,   176,   604,   221,   165,   607,  1598,   732,
     204,  1598,   206,   139,     5,     6,   173,  1598,  1598,  1561,
      80,  1598,   801,   202,    60,    61,   173,    57,   179,   165,
     756,   197,   198,    63,  1575,   635,   200,   163,   163,   351,
     173,   200,   102,   103,    47,  1094,   173,   204,   185,    62,
     173,   998,   199,   348,   180,   173,   180,   352,  1071,   220,
     197,  1065,   198,   986,    67,    21,    22,   163,  1457,   165,
     206,   966,   198,   386,   965,   156,   200,  1466,   678,   205,
     161,     8,   163,   127,   179,   166,   165,   127,   124,   165,
     173,   404,   128,   133,   174,   127,   269,   156,   177,   139,
    1399,   133,   161,   198,   163,   165,   173,   166,   204,    36,
     127,   424,   207,   176,   427,   428,   429,   127,   201,   200,
    1509,  1510,  1135,   133,   200,   204,   186,   422,  1380,   173,
     174,   404,   176,  1004,   174,   179,   595,   353,   198,   175,
     154,   165,   174,   179,   204,   181,   182,   747,    34,   163,
     609,   173,   127,   879,   427,   428,   429,   174,   133,   154,
     177,   173,   118,   119,   174,   808,   173,   173,   163,   173,
     126,   207,   128,   129,   130,   131,   132,    63,   957,   201,
     204,  1314,    21,    22,   163,   139,   173,   201,   163,   201,
     173,   173,   505,   506,   201,   878,   509,   201,   511,   174,
     513,  1250,   515,  1021,   179,   127,   173,   890,   521,   163,
    1081,   133,   875,   386,   201,   163,   127,   176,   201,   201,
     179,   107,   133,   946,   537,   888,   182,   183,   184,   185,
     186,   404,   505,   506,   201,   961,   509,   127,   511,  1011,
     513,   197,   198,   133,   173,   173,   132,   139,   521,   148,
     173,   424,   174,   139,   427,   428,   429,  1390,  1391,   985,
     173,   434,   435,   174,   163,   200,   164,   165,  1607,  1318,
     199,   199,   731,  1406,  1407,   173,  1615,   163,   201,   118,
     119,   156,   595,   181,   174,   744,   745,   126,   201,   128,
     129,   130,   131,   132,   173,   754,   609,   173,    21,    22,
     173,  1350,   761,   762,   190,   764,   204,   766,   173,   768,
      63,    64,    65,  1260,   200,   173,   197,   173,  1189,   140,
     176,   142,   201,   179,   167,   201,   199,  1460,  1461,   642,
     643,   165,   505,   506,   199,   173,   509,   163,   511,   634,
     513,   173,   515,   201,   657,   184,   185,   186,   521,   102,
     103,   104,   105,   173,  1203,   173,   181,  1478,   197,   198,
     186,   139,   675,   201,   537,  1008,   180,   176,    57,   201,
     148,    33,   181,    33,    63,   153,   173,  1226,  1196,   204,
    1023,   201,  1431,   201,   697,   163,   200,    57,    21,    22,
     563,   200,   176,    63,  1212,   118,   119,   181,    60,    61,
      60,    61,   180,   126,   201,   128,   129,   130,   131,   132,
     176,   572,   164,   165,   176,   181,   200,    57,   731,   181,
     154,   173,   595,    63,   697,   177,   180,   148,  1028,   163,
     743,   744,   745,   746,   200,  1161,   609,   750,   200,   163,
    1040,   754,   163,  1492,  1493,  1045,   200,   163,   761,   762,
     177,   764,   204,   766,   181,   768,   769,   204,   205,   632,
     180,   180,   124,   636,   124,   127,   128,   139,   128,   180,
     743,   133,   163,   746,   197,   198,   148,   750,   164,   165,
     200,   200,    57,   164,   165,   118,   119,   173,    63,   200,
      12,   163,   173,   126,    79,   181,   129,   130,   131,   132,
     181,    23,    24,   165,   173,   964,   163,   176,   163,    94,
     179,   970,   174,   175,    99,   175,   101,   179,   204,   179,
     182,  1570,   182,   204,   697,   167,   168,   840,   163,   164,
     154,   163,   164,   163,    21,    22,   177,   154,   173,   163,
     181,   173,   204,   164,   163,   207,   163,   207,  1007,   710,
    1421,   712,   173,  1602,   205,   177,    57,   154,   731,   181,
     873,   722,   875,   876,   197,   198,   163,    10,    11,   882,
     743,   744,   745,   746,   735,   888,    57,   750,    57,  1179,
     174,   754,    63,   896,    63,   190,  1186,   176,   761,   762,
     177,   764,    33,   766,   181,   768,   769,  1197,   177,   167,
     873,   177,   181,   876,   140,   181,   177,  1456,  1457,   882,
     181,   165,   177,  1462,   163,   910,   181,  1466,  1467,    60,
      61,   177,   177,   896,   177,   181,   181,    66,   181,   116,
     117,   118,   119,   167,   168,   169,   170,   203,   164,   126,
      35,   128,   129,   130,   131,   132,    35,   134,   135,   810,
     200,   964,   965,  1502,  1445,   202,   203,   970,   205,   176,
    1509,  1510,   106,   203,   164,   165,   180,   164,   165,   140,
     141,   142,   163,   173,   180,   180,   173,   167,   168,   169,
     200,   181,   180,   124,   181,   180,   127,   128,   556,   557,
     558,    33,   133,  1465,  1007,   182,   183,   184,   185,   186,
     873,   200,   875,   876,   204,   164,   165,   204,   180,   882,
     197,   198,   180,   180,   173,   888,   361,   180,    60,    61,
     180,   198,   181,   896,  1446,   200,   371,   888,  1577,   890,
    1330,   177,  1504,   174,   175,   167,   168,   169,   179,   384,
     164,   182,    54,    55,    56,   204,   907,   205,   205,   163,
      21,    22,   913,   914,  1213,   163,   163,   199,   164,   165,
      22,   163,   199,   176,   163,   926,   207,   173,   929,   930,
     931,   932,   933,  1496,   205,   181,   937,  1568,   198,   164,
     165,   163,   124,   180,   163,   946,   128,   948,   173,   200,
     205,   964,   965,   205,   204,   176,   181,   970,   204,   163,
     201,   164,   165,  1403,   180,   180,   180,   452,   453,   200,
     173,   180,   180,  1604,   200,   180,   200,  1540,   181,   204,
     180,   180,   164,   165,   469,   470,   471,   472,   473,  1141,
     180,   173,   203,   175,  1007,    13,   163,   179,   200,   181,
     182,   204,  1442,   200,   200,   116,   117,   118,   119,   120,
     198,   200,   123,   124,   125,   126,   200,   128,   129,   130,
     131,   132,   204,   134,   135,   207,    75,   200,  1397,   200,
      79,   200,   173,   201,   199,   201,   200,    43,   181,   200,
     180,   180,   180,  1178,    93,    94,   200,    13,   533,    98,
      99,   100,   101,   199,   173,   200,     4,   201,   200,   163,
    1213,    33,  1215,   163,   176,   163,    33,   552,   179,   180,
     181,   182,   183,   184,   185,   186,  1211,   203,   200,    43,
     163,   163,   163,   163,   173,   200,   197,   198,    60,    61,
     200,   576,   201,    60,    61,   200,   180,     1,   201,  1252,
     201,   200,  1215,   200,   180,   200,    33,   200,   200,   200,
     200,   596,   597,   200,   181,   600,   181,   602,   108,   109,
     110,   111,   112,   113,   114,   115,   181,   612,   613,   614,
     615,   616,   617,    60,    61,   174,   200,   163,   163,  1252,
     163,   163,   156,   133,    10,  1146,    13,     9,    42,   206,
      66,    43,   124,   143,   144,   145,   128,   124,   174,   644,
     645,   128,   200,   648,   649,   650,   651,   201,   653,   201,
     655,   656,   657,   658,   659,   660,   661,   662,   663,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     201,   676,    33,   201,   201,   200,  1495,   124,   201,   200,
    1213,   128,  1215,   175,    43,  1358,   163,   179,   175,   181,
     182,   200,   179,   206,   181,   182,  1351,   702,   201,    60,
      61,   180,   200,   200,   200,   163,   206,   206,    43,  1382,
      14,   174,   174,   156,    37,   207,   176,   722,   200,  1252,
     207,   163,   200,   728,   200,  1358,   200,   200,   175,   734,
       8,   163,   179,   200,   181,   182,   201,   163,   171,   200,
     163,   163,   201,   163,   749,   163,   163,   174,   163,  1382,
     206,   206,    67,   174,   200,   200,   181,   201,    21,    22,
     207,   200,   200,   124,    70,   200,   200,   128,   163,   200,
      43,   181,  1293,   163,  1295,   780,   138,   205,   167,   163,
     785,   200,   787,    12,   789,   201,  1441,   200,  1443,  1444,
      33,   200,   200,   204,   173,   800,   201,   204,   201,   201,
     201,    33,   173,   200,   204,   810,    33,   205,   201,   201,
     201,   205,    53,   201,   175,    79,   201,   200,   179,     1,
     181,   182,  1495,   201,   204,  1358,   201,   201,    60,    61,
     206,   199,   199,    60,    61,   618,   206,   206,    44,   205,
     205,   130,   206,    82,  1614,   712,   207,  1611,   853,  1382,
     712,   101,   857,   116,   117,   118,   119,   120,   636,  1380,
     123,   124,   125,   126,   227,   128,   129,   130,   131,   132,
     942,   134,   135,   697,     1,   640,  1348,   140,   141,   142,
     494,  1536,  1455,   146,  1452,  1500,  1385,    53,   893,  1422,
    1501,  1501,   124,   898,   941,   900,   128,   124,   903,   941,
     244,   128,   165,  1135,   909,   345,   911,  1467,  1563,    -1,
     434,    -1,   175,   596,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,   434,    -1,    -1,  1450,
      -1,    -1,    -1,    -1,   197,   198,    -1,  1592,    -1,    -1,
      -1,   204,   947,   175,  1477,    -1,    -1,   179,   175,   181,
     182,  1606,   179,    -1,   181,   182,    33,    -1,    -1,    -1,
      -1,    -1,  1495,    -1,    -1,    -1,   971,   972,  1489,    -1,
      -1,    -1,    -1,  1494,    -1,   207,    -1,    -1,    -1,    -1,
     207,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1005,  1006,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      -1,  1016,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1025,    -1,  1027,    -1,  1029,    -1,    -1,    -1,    -1,    -1,
    1035,    60,    61,    -1,  1039,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1047,    -1,    -1,    -1,    -1,   124,  1569,    -1,
      -1,   128,  1573,  1574,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1069,  1586,    -1,    -1,  1589,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1083,    -1,
      -1,  1086,    -1,    -1,  1605,    -1,    -1,    -1,  1609,    -1,
    1095,    -1,    -1,    -1,    -1,   124,    -1,    -1,   175,   128,
      -1,    -1,   179,    -1,   181,   182,    -1,    -1,    -1,    -1,
      -1,  1116,  1117,  1118,  1119,  1120,  1121,  1122,  1123,  1124,
    1125,  1126,  1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,
     207,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1145,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,
     179,    -1,   181,   182,    -1,    -1,    -1,    -1,    -1,  1164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1182,   207,    -1,
      -1,    -1,    -1,    -1,    -1,  1190,  1191,  1192,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     5,     6,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,  1221,    27,  1223,    -1,
      -1,    31,    33,    -1,  1229,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,  1248,    -1,    55,    -1,    -1,  1253,    60,
      61,    -1,    -1,  1258,  1259,    65,  1261,  1262,    68,    69,
      70,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    33,    98,    99,
     100,   101,   102,   103,   104,    -1,  1301,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1310,  1311,  1312,   118,   119,
      -1,    -1,  1317,   124,    60,    61,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1331,  1332,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,    -1,    -1,    -1,
      -1,  1366,    -1,    -1,   175,    -1,    -1,    -1,   179,    -1,
     181,   182,   182,   183,   184,    -1,   186,    33,   124,   189,
     190,    -1,   128,    21,    22,    -1,    -1,    -1,   198,    -1,
     200,   201,   202,   203,  1399,   205,   207,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1428,    -1,    -1,    -1,    -1,    -1,   175,
      -1,    -1,    -1,   179,    -1,   181,   182,    -1,    -1,    -1,
      -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1459,    -1,    -1,    -1,    -1,    -1,
      -1,   207,    -1,    -1,    -1,  1470,    -1,    -1,   124,  1474,
    1475,    -1,   128,  1478,    -1,    -1,  1481,    -1,   116,   117,
     118,   119,   120,  1488,    -1,   123,   124,   125,   126,  1494,
     128,   129,   130,   131,   132,    -1,   134,   135,    -1,    -1,
      -1,    -1,   140,   141,   142,    -1,   124,    -1,   146,    -1,
     128,    -1,    -1,    -1,  1519,    -1,    -1,    -1,    -1,   175,
      -1,    -1,    -1,   179,    -1,   181,   182,    -1,    -1,    -1,
    1535,    -1,    -1,    -1,    -1,    -1,  1541,   175,    -1,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    -1,
      -1,   207,    -1,    -1,    -1,    -1,    -1,   175,    -1,   197,
     198,   179,    -1,   181,   182,    -1,   204,  1572,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1580,    -1,    -1,    -1,    -1,
    1585,  1586,     1,    -1,    -1,    -1,     5,     6,     7,   207,
       9,    10,    11,  1598,    13,    -1,    15,    16,    17,    18,
      19,    -1,    -1,    -1,  1609,    -1,    25,    26,    27,    28,
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
       1,    -1,    -1,    -1,     5,     6,     7,    -1,     9,    10,
      11,    -1,    13,    -1,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,
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
     205,   206,     5,     6,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     182,   183,   184,    -1,   186,    -1,    -1,   189,   190,    -1,
      -1,    -1,    -1,    -1,     5,     6,   198,    -1,   200,    -1,
     202,   203,    13,   205,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
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
      70,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
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
     148,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
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
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    61,    -1,
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
      -1,    -1,    -1,     5,     6,   198,    -1,   200,    -1,   202,
     203,    -1,   205,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    58,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,     5,     6,   198,    -1,   200,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     103,   104,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,
     183,   184,    -1,   186,    -1,    -1,   189,   190,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   198,    -1,   200,    -1,   202,
     203,    -1,   205,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,    -1,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,   197,   198,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,   163,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,   202,    -1,    -1,    -1,
      -1,   207,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
     163,    -1,   165,    -1,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,   202,
      -1,   204,    21,    22,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,   163,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,   202,    -1,   116,   117,   118,
     119,   120,    21,    22,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,   139,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   197,   198,
      -1,    -1,   201,    -1,    -1,    -1,    -1,    -1,   202,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    71,    72,    73,   146,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,   197,   198,
      -1,    -1,   201,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,    -1,   134,   135,    -1,    -1,    -1,   154,   140,   141,
     142,    -1,    -1,    -1,   146,    -1,   163,    -1,    -1,    -1,
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
      -1,    21,    22,    -1,    -1,    -1,    -1,   197,   198,    -1,
      -1,   201,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,    -1,
     134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,   199,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    -1,    -1,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,
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
     182,   183,   184,   185,   186,    -1,    -1,    21,    22,    35,
      -1,    -1,    -1,    -1,    -1,   197,   198,   199,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,   132,    -1,   134,   135,    -1,    -1,
      -1,    -1,   140,   141,   142,    71,    -1,    73,   146,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,    -1,   175,    -1,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    -1,
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,   197,
     198,   199,   116,   117,   118,   119,   120,    21,    22,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,    -1,
     134,   135,    -1,    -1,    38,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,   163,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,   199,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,    -1,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    21,    22,   189,   190,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    -1,    -1,   138,    -1,   140,
     141,   142,    71,    72,    73,   146,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,   132,    -1,   134,   135,    -1,    -1,
      -1,    -1,   140,   141,   142,   154,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    71,    72,
      73,   146,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,    -1,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,    -1,   134,   135,    21,    22,    -1,    -1,   140,   141,
     142,   154,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    21,    22,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    21,
      22,    -1,    -1,   140,   141,   142,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,    -1,
     134,   135,    21,    22,    -1,    -1,   140,    -1,   142,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,    -1,   134,   135,    21,    22,    -1,    -1,   140,    -1,
      -1,    -1,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,   182,   183,   184,   185,   186,
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
     225,   227,   228,   232,   277,   281,   310,   391,   399,   406,
     416,   466,   471,   476,    19,    20,   163,   268,   269,   270,
     156,   233,   234,   163,   186,   229,   230,    57,    63,   396,
     397,   163,   202,   214,   477,   467,   472,   139,   163,   298,
      34,    63,   107,   132,   190,   200,   272,   273,   274,   275,
     298,   211,   211,   211,     8,    36,   417,    62,   387,   174,
     173,   176,   173,   229,    22,    57,   185,   197,   231,   163,
     211,   387,   396,   396,   396,   163,   139,   226,   274,   274,
     274,   200,   140,   141,   142,   173,   199,    57,    63,   282,
     284,   407,     5,     6,   107,   413,    57,    63,   388,    15,
      16,   156,   161,   163,   166,   200,   218,   269,   156,   234,
     163,   163,   163,   398,    57,    63,   213,   163,   163,   163,
     163,   167,   224,   201,   270,   274,   274,   274,   274,   165,
     238,   239,   285,    57,    63,   400,     5,     6,    57,    63,
     414,   205,   392,   167,   168,   169,   217,    15,    16,   156,
     161,   163,   218,   266,   267,   231,   174,   190,   215,   176,
     427,   239,   239,   167,   201,   165,    35,    71,    73,    75,
      76,    77,    78,    79,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    93,    94,    95,    98,    99,
     100,   101,   118,   119,   163,   280,   283,   163,   401,   418,
     389,   203,   271,   357,   167,   168,   169,   173,   201,    19,
      25,    31,    41,    49,    64,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   152,   202,
     298,   421,   423,   424,   428,   434,   436,   465,    66,    79,
      94,    99,   101,   164,   404,   405,   468,   473,    21,    22,
      38,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   128,
     129,   130,   133,   134,   135,   136,   137,   140,   141,   142,
     143,   144,   145,   146,   175,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   189,   190,   197,   198,    35,
      35,   200,   278,   239,   176,   402,   106,   411,   412,   206,
     211,   390,   269,   203,   163,   383,   386,   266,   180,   180,
     180,   200,   180,   180,   200,   427,   180,   180,   180,   180,
     180,   200,   298,   180,   200,    33,    60,    61,   124,   128,
     175,   179,   182,   207,   198,   433,   177,   164,   478,   205,
     205,   163,   163,   163,   199,    22,   163,   199,   151,   201,
     357,   367,   368,   369,   176,   279,   289,   290,    75,    79,
      93,    94,    98,    99,   100,   101,   422,   405,   163,   239,
     357,   239,   269,   173,   176,   179,   381,   437,   443,   445,
       5,     6,    15,    16,    17,    18,    19,    25,    27,    31,
      39,    45,    48,    51,    55,    65,    68,    69,    80,   102,
     103,   104,   118,   119,   147,   148,   149,   150,   151,   153,
     155,   156,   157,   158,   159,   160,   161,   162,   166,   182,
     183,   184,   189,   190,   198,   200,   202,   203,   205,   223,
     225,   292,   298,   303,   315,   322,   325,   328,   332,   334,
     336,   337,   339,   344,   347,   348,   355,   356,   421,   481,
     489,   499,   502,   514,   518,   447,   441,   163,   180,   449,
     451,   453,   455,   457,   459,   461,   463,   348,   180,   200,
     435,   439,   295,   326,   348,    33,   179,    33,   179,   198,
     207,   199,   348,   198,   207,   434,   205,   469,   474,   163,
     108,   109,   110,   111,   112,   113,   114,   115,   133,   143,
     144,   145,   200,     7,    50,   309,   204,   173,   204,   201,
     465,   205,   205,   176,   410,   419,   148,   163,   382,   385,
     239,   163,   434,   127,   133,   174,   380,   465,   465,   432,
     465,   180,   180,   180,   298,   300,   423,   481,   298,   180,
       5,   102,   103,   180,   200,   180,   200,   200,   180,   180,
     200,   180,   200,   180,   200,   180,   180,   200,   180,   180,
     348,   348,   200,   200,   200,   200,   200,   200,   222,   348,
     348,   348,   348,   348,    13,    49,   295,   154,   163,   326,
     482,   484,   203,    13,   515,   200,   198,   276,   203,   205,
     328,   333,    21,    22,   116,   117,   118,   119,   120,   123,
     124,   125,   126,   128,   129,   130,   131,   132,   134,   135,
     140,   141,   142,   146,   175,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   197,   198,   201,   200,   465,
     465,   201,   429,   465,   278,   465,   278,   465,   278,   163,
     370,   371,   465,   163,   373,   374,   201,   440,   326,   297,
     465,   201,   173,   517,   199,   199,   199,   348,   479,   370,
     372,   373,   375,   348,   148,   163,   377,   378,   420,   369,
     369,   369,   294,   408,   298,   204,   205,   415,   200,    43,
     173,   176,   381,   211,   380,   348,   181,   181,   181,   164,
     173,   210,   211,   431,   490,   492,   301,   200,   180,   200,
     323,   180,   180,   180,   509,   326,   434,   348,   498,   513,
     348,   316,   318,   348,   320,   348,   511,   326,   496,   500,
     326,   494,   434,   348,   348,   348,   348,   348,   348,   169,
     170,   217,   200,    13,   199,   200,   127,   133,   174,   376,
     517,   173,   517,   201,   148,   153,   180,   298,   338,   200,
     239,    70,   198,   201,   326,   484,   275,     4,   331,   203,
     294,   276,    19,   154,   163,   421,    19,   154,   163,   421,
     348,   348,   348,   348,   348,   348,   163,   348,   154,   163,
     348,   348,   348,   421,   348,   348,   348,   348,   348,   348,
      22,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   129,   130,   154,   163,   197,   198,   345,   421,
     348,   201,   326,   181,   181,   163,   425,   181,   279,   181,
     279,   181,   279,   176,   181,   431,   176,   181,   431,   297,
     517,   181,   431,   348,   199,   163,   426,   211,   244,   245,
     244,   245,   201,   200,    43,   173,   176,   179,   376,     1,
      26,    28,    29,    38,    40,    44,    52,    54,    58,    59,
      65,   105,   211,   235,   236,   243,   254,   255,   256,   258,
     259,   260,   261,   262,   263,   264,   265,   291,   293,   299,
     304,   305,   306,   307,   308,   310,   314,   335,   348,   163,
     393,   395,   363,   348,   163,   163,   434,   380,   348,   211,
     438,   444,   446,   465,   434,   434,   465,    70,   326,   484,
     488,   163,   348,   465,   503,   505,   507,   434,   517,   181,
     431,   138,   173,   517,   201,   434,   434,   201,   434,   201,
     434,   517,   434,   371,   517,   374,   181,   201,   201,   201,
     201,   201,   201,   348,   148,   163,   200,   257,   200,   348,
     348,   348,   201,   154,   163,   200,   200,   340,   342,   257,
     296,   513,   163,   201,   484,   482,   173,   201,   201,   199,
     200,   278,   206,   331,   180,   200,   180,   200,   200,   200,
     199,    19,   154,   163,   421,   176,   154,   163,   348,   200,
     200,   154,   163,   348,     1,   200,   199,   173,   201,   448,
     442,   181,   204,   450,   181,   454,   181,   458,   181,   465,
     462,   370,   465,   464,   373,   181,   201,   435,   465,   174,
     210,   394,   403,   211,   370,   470,   373,   475,   348,   163,
     163,   465,   348,   127,   348,   289,    61,   348,   163,   163,
     211,   156,    58,   348,   239,   127,   348,   211,   211,    10,
      10,    11,   241,    13,     9,    42,   211,   206,   211,   211,
     211,   211,   211,    66,   311,   211,   108,   109,   110,   111,
     112,   113,   114,   115,   121,   122,   127,   133,   136,   137,
     143,   144,   145,   174,   174,   394,   403,    40,   165,   206,
     277,   364,   201,    43,   211,   380,   348,   211,   181,   181,
     181,   484,   201,   201,   201,   181,   431,   201,   181,   434,
     371,   374,   181,   201,   200,   434,   348,   498,   201,   181,
     181,   181,   181,   201,   181,   181,   201,   181,   331,   200,
     176,   220,   200,    43,   163,   312,    20,   173,   257,   201,
     200,   133,   376,   348,   348,   434,   278,    20,   206,   517,
     201,   173,   199,   198,   127,   133,   163,   174,   179,   329,
     330,   279,   278,   349,   348,   351,   348,   201,   326,   348,
     180,   200,   348,   200,   199,   348,   198,   201,   326,   200,
     199,   346,   201,   326,   430,   163,   452,   456,   460,   435,
     348,   163,   210,   480,   206,   206,   201,    43,   376,   348,
      14,   348,   174,   174,   176,   156,   289,   348,   200,   200,
     200,   200,   200,    37,   286,   200,   205,   313,   385,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   393,   409,
     163,     8,   357,   362,   163,   348,   211,   491,   493,   302,
     201,   200,   163,   324,   181,   181,   181,   510,   296,   181,
     317,   319,   321,   512,   497,   501,   495,   200,   239,   201,
     326,   221,   171,   348,   163,   173,   201,   326,   163,   200,
      20,   133,   376,   348,   348,   348,   201,   201,   181,   279,
     326,   201,   482,   163,   163,   200,   163,   163,   173,   201,
     239,   279,   434,   201,   465,   201,   201,   201,   353,   348,
     348,   201,   482,   201,   348,   201,   174,   206,   163,   348,
     289,   465,   348,   348,   348,   257,   348,   348,   287,   312,
     384,   206,   174,    57,    63,   360,    67,   361,   211,   211,
     200,   200,   348,   181,   504,   506,   508,   200,   201,   200,
     348,   348,   348,   200,    70,   488,   200,   200,   201,   348,
     289,   201,   219,   201,   163,   201,    43,   312,   326,   348,
     348,   201,   341,   181,   201,   199,   163,   329,   138,   289,
     327,   239,   181,   181,   465,   201,   201,   199,   201,   348,
     201,   201,    20,   201,   201,   205,   201,   206,   211,   385,
     465,    47,   361,    46,   106,   358,   488,   488,   201,   200,
     200,   200,   200,   295,   296,   326,   488,   488,   201,   167,
     204,   163,   201,   201,   133,   376,   338,   343,   204,   201,
     201,   127,   348,   289,   350,   352,   181,   201,    12,   242,
     239,   326,   239,   239,   294,   176,   381,   211,    33,   359,
     358,   360,   200,   482,   485,   486,   487,   487,   348,   488,
     488,   482,   483,   201,   201,   517,   487,   483,   348,   204,
     348,   348,   338,   498,   348,   354,   243,   304,   305,   306,
     307,   348,   211,   246,   247,   249,   201,   289,   289,   288,
     434,   380,   365,   359,   377,   378,   379,   482,   173,   517,
     201,   201,   201,   487,   487,   201,   201,   201,   201,   204,
     516,   348,   516,    12,    23,    24,   237,   240,   205,   243,
     239,   206,   380,   348,   283,   366,   201,   200,   201,   201,
      53,   199,   516,   206,   239,   200,   294,   211,   289,   348,
     211,   211,   283,   482,   348,   199,   250,   251,   253,   348,
     248,   211,   239,   201,   205,   243,   201,   206,   289,   294,
     211,   239,   286,   252,   250,   206,   240,   286
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
     269,   269,   269,   269,   269,   269,   269,   270,   270,   271,
     271,   272,   272,   272,   272,   273,   273,   274,   274,   274,
     274,   274,   274,   274,   275,   275,   276,   276,   277,   277,
     278,   278,   278,   279,   279,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   281,   282,   282,   282,   283,   285,   284,   286,   287,
     288,   286,   290,   291,   289,   292,   293,   293,   293,   293,
     293,   293,   293,   293,   293,   293,   293,   293,   293,   293,
     293,   293,   293,   293,   293,   294,   294,   294,   295,   295,
     296,   296,   297,   297,   298,   298,   298,   299,   299,   301,
     302,   300,   300,   303,   303,   303,   303,   303,   303,   304,
     305,   306,   306,   306,   307,   307,   308,   309,   309,   309,
     310,   310,   311,   311,   312,   312,   313,   313,   314,   314,
     314,   316,   317,   315,   318,   319,   315,   320,   321,   315,
     323,   324,   322,   325,   325,   325,   326,   326,   327,   327,
     327,   328,   328,   328,   329,   329,   329,   329,   329,   330,
     330,   331,   331,   332,   333,   333,   334,   334,   334,   334,
     334,   334,   334,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   336,   336,   337,   337,   338,   338,   339,   340,
     341,   339,   342,   343,   339,   344,   344,   344,   344,   344,
     344,   344,   345,   346,   344,   347,   347,   347,   347,   347,
     347,   347,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   349,   350,
     348,   348,   348,   348,   351,   352,   348,   348,   348,   353,
     354,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   355,   355,   355,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   357,   357,   358,   358,   358,   359,
     359,   360,   360,   360,   361,   361,   362,   363,   363,   363,
     364,   363,   365,   363,   366,   363,   367,   368,   368,   369,
     369,   369,   369,   369,   370,   370,   371,   371,   372,   372,
     372,   373,   374,   374,   375,   375,   375,   376,   376,   377,
     377,   377,   378,   378,   379,   379,   380,   380,   380,   381,
     381,   382,   382,   382,   382,   382,   383,   383,   384,   384,
     384,   385,   385,   385,   386,   386,   386,   387,   387,   388,
     388,   388,   389,   389,   390,   389,   391,   392,   391,   393,
     393,   394,   394,   395,   395,   395,   396,   396,   396,   398,
     397,   399,   400,   400,   400,   401,   402,   402,   403,   403,
     404,   404,   405,   405,   407,   408,   409,   406,   410,   410,
     411,   411,   412,   413,   413,   413,   413,   414,   414,   414,
     415,   415,   417,   418,   419,   416,   420,   420,   420,   420,
     420,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   422,   422,
     422,   422,   422,   422,   422,   422,   423,   424,   424,   424,
     425,   425,   426,   426,   426,   426,   426,   427,   427,   427,
     427,   427,   429,   430,   428,   431,   431,   432,   432,   433,
     433,   433,   433,   434,   434,   435,   435,   436,   436,   436,
     436,   437,   438,   436,   436,   436,   436,   439,   436,   440,
     436,   436,   436,   436,   436,   436,   436,   436,   436,   436,
     436,   436,   436,   441,   442,   436,   436,   443,   444,   436,
     445,   446,   436,   447,   448,   436,   436,   449,   450,   436,
     451,   452,   436,   436,   453,   454,   436,   455,   456,   436,
     436,   457,   458,   436,   459,   460,   436,   461,   462,   436,
     463,   464,   436,   465,   465,   465,   467,   468,   469,   470,
     466,   472,   473,   474,   475,   471,   477,   478,   479,   480,
     476,   481,   481,   481,   481,   481,   482,   482,   482,   482,
     482,   482,   482,   482,   483,   483,   484,   485,   485,   486,
     486,   487,   487,   488,   488,   490,   491,   489,   492,   493,
     489,   494,   495,   489,   496,   497,   489,   498,   498,   499,
     500,   501,   499,   502,   503,   504,   502,   505,   506,   502,
     507,   508,   502,   502,   509,   510,   502,   502,   511,   512,
     502,   513,   513,   515,   514,   514,   514,   514,   516,   516,
     517,   517,   518,   518,   518
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
       3,     3,     3,     3,     3,     1,     5,     1,     3,     3,
       4,     1,     1,     1,     1,     1,     4,     1,     2,     3,
       3,     3,     3,     2,     1,     3,     0,     3,     0,     4,
       0,     2,     3,     0,     2,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     3,
       3,     2,     2,     3,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     3,     2,     2,     2,
       2,     2,     3,     3,     3,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     0,     1,     1,     3,     0,     5,     0,     0,
       0,     6,     0,     0,     6,     2,     1,     2,     2,     1,
       1,     1,     1,     2,     1,     2,     2,     2,     2,     1,
       1,     1,     2,     2,     2,     0,     2,     2,     0,     2,
       0,     2,     1,     3,     1,     3,     2,     2,     3,     0,
       0,     5,     1,     2,     5,     5,     5,     6,     2,     1,
       1,     1,     2,     3,     2,     3,     4,     1,     1,     0,
       1,     1,     1,     0,     1,     3,     8,     7,     3,     3,
       5,     0,     0,     7,     0,     0,     7,     0,     0,     7,
       0,     0,     6,     5,     8,    10,     1,     3,     1,     2,
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
       1,     3,     0,     1,     3,     3,     5,     0,     2,     2,
       2,     2,     0,     0,     7,     1,     1,     1,     3,     3,
       2,     4,     3,     1,     2,     0,     4,     1,     1,     1,
       1,     0,     0,     6,     4,     4,     3,     0,     6,     0,
       7,     4,     2,     2,     3,     2,     3,     2,     2,     3,
       3,     3,     2,     0,     0,     6,     2,     0,     0,     6,
       0,     0,     6,     0,     0,     6,     1,     0,     0,     6,
       0,     0,     7,     1,     0,     0,     6,     0,     0,     7,
       1,     0,     0,     6,     0,     0,     7,     0,     0,     6,
       0,     0,     6,     1,     3,     3,     0,     0,     0,     0,
      12,     0,     0,     0,     0,    12,     0,     0,     0,     0,
      13,     1,     1,     1,     1,     1,     3,     3,     5,     5,
       6,     6,     8,     8,     0,     1,     2,     3,     5,     1,
       2,     1,     0,     0,     1,     0,     0,    10,     0,     0,
      10,     0,     0,    10,     0,     0,     7,     3,     1,     5,
       0,     0,    10,     3,     0,     0,    11,     0,     0,    11,
       0,     0,    10,     5,     0,     0,     9,     5,     0,     0,
      10,     1,     3,     0,     5,     5,     7,     9,     0,     3,
       0,     1,    11,    12,    11
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

  case 131: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 132: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 133: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 134: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 135: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 136: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 137: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 138: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 139: /* metadata_argument_list: '@' annotation_argument optional_emit_semis  */
                                                         {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[-1].aa));
    }
    break;

  case 140: /* metadata_argument_list: metadata_argument_list '@' annotation_argument optional_emit_semis  */
                                                                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-3].aaList),(yyvsp[-1].aa));
    }
    break;

  case 141: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 142: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 143: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 144: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 145: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 146: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 147: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 148: /* annotation_declaration: '!' annotation_declaration  */
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

  case 149: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 150: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 151: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 152: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 153: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 154: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 155: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 156: /* optional_annotation_list: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 157: /* optional_annotation_list: '[' annotation_list ']'  */
                                       { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 158: /* optional_annotation_list_with_emit_semis: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 159: /* optional_annotation_list_with_emit_semis: '[' annotation_list ']' optional_emit_semis  */
                                                          { (yyval.faList) = (yyvsp[-2].faList); }
    break;

  case 160: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 161: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 162: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 163: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 164: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 165: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 166: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 167: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 168: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 169: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 170: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 171: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 172: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 173: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 174: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 175: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 176: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 177: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 178: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 179: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 180: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 181: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 182: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 183: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 184: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 185: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 186: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 187: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 188: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 189: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 190: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 191: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 192: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 193: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 194: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 195: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 196: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 197: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 198: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 199: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 200: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 201: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 202: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 203: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 204: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 205: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 206: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 207: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 208: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 209: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 210: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 211: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 212: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 213: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 214: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 215: /* function_name: "operator" '.' "name" "+="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`+="); delete (yyvsp[-1].s); }
    break;

  case 216: /* function_name: "operator" '.' "name" "-="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`-="); delete (yyvsp[-1].s); }
    break;

  case 217: /* function_name: "operator" '.' "name" "*="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`*="); delete (yyvsp[-1].s); }
    break;

  case 218: /* function_name: "operator" '.' "name" "/="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`/="); delete (yyvsp[-1].s); }
    break;

  case 219: /* function_name: "operator" '.' "name" "%="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`%="); delete (yyvsp[-1].s); }
    break;

  case 220: /* function_name: "operator" '.' "name" "&="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&="); delete (yyvsp[-1].s); }
    break;

  case 221: /* function_name: "operator" '.' "name" "|="  */
                                          { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`|="); delete (yyvsp[-1].s); }
    break;

  case 222: /* function_name: "operator" '.' "name" "^="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^="); delete (yyvsp[-1].s); }
    break;

  case 223: /* function_name: "operator" '.' "name" "&&="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&&="); delete (yyvsp[-1].s); }
    break;

  case 224: /* function_name: "operator" '.' "name" "||="  */
                                            { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`||="); delete (yyvsp[-1].s); }
    break;

  case 225: /* function_name: "operator" '.' "name" "^^="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^^="); delete (yyvsp[-1].s); }
    break;

  case 226: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 227: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 228: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 229: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 230: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 231: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 232: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 233: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 234: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 235: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 236: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 237: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 238: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 239: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 240: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 241: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 242: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 243: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 244: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 245: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 246: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 247: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 248: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 249: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 250: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 251: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 252: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 253: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 254: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 255: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 256: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 257: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 258: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 259: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 260: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 261: /* global_function_declaration: optional_annotation_list_with_emit_semis "def" function_declaration  */
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

  case 262: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 263: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 264: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 265: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 266: /* $@13: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 267: /* function_declaration: optional_public_or_private_function $@13 function_declaration_header optional_emit_semis expression_block  */
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

  case 268: /* expression_block_finally: %empty  */
        {
        (yyval.pExpression) = nullptr;
    }
    break;

  case 269: /* $@14: %empty  */
                  {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 270: /* $@15: %empty  */
                             {
        yyextra->pop_nesteds();
    }
    break;

  case 271: /* expression_block_finally: "finally" $@14 '{' expressions $@15 '}'  */
          {
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 272: /* $@16: %empty  */
        {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 273: /* $@17: %empty  */
                                      {
        yyextra->pop_nesteds();
    }
    break;

  case 274: /* expression_block: $@16 '{' expressions $@17 '}' expression_block_finally  */
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

  case 275: /* expr_call_pipe: expr_call expr_full_block_assumed_piped  */
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

  case 276: /* expression_any: SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 277: /* expression_any: expr_assign SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 278: /* expression_any: expression_delete SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 279: /* expression_any: expression_let  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 280: /* expression_any: expression_while_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 281: /* expression_any: expression_unsafe  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 282: /* expression_any: expression_with  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 283: /* expression_any: expression_with_alias SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 284: /* expression_any: expression_for_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 285: /* expression_any: expression_break SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 286: /* expression_any: expression_continue SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 287: /* expression_any: expression_return SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 288: /* expression_any: expression_yield SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 289: /* expression_any: expression_if_then_else  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 290: /* expression_any: expression_if_then_else_oneliner  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 291: /* expression_any: expression_try_catch  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 292: /* expression_any: expression_label SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 293: /* expression_any: expression_goto SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 294: /* expression_any: "pass" SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 295: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 296: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 297: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 298: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 299: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 300: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 301: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 302: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 303: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 304: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 305: /* name_in_namespace: "name" "::" "name"  */
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

  case 306: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 307: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 308: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 309: /* $@18: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 310: /* $@19: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 311: /* new_type_declaration: '<' $@18 type_declaration '>' $@19  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 312: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 313: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 314: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 315: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 316: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 317: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 318: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 319: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 320: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 321: /* expression_return: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 322: /* expression_return: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 323: /* expression_return: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 324: /* expression_yield: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 325: /* expression_yield: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 326: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 327: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 328: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 329: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 330: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 331: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 332: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 333: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 334: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 335: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 336: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 337: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr SEMICOLON  */
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

  case 338: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 339: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 340: /* expression_let: kwd_let optional_in_scope '{' variable_declaration_list '}'  */
                                                                               {
        (yyval.pExpression) = ast_LetList(scanner,(yyvsp[-4].b),(yyvsp[-3].b),*(yyvsp[-1].pVarDeclList),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 341: /* $@20: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 342: /* $@21: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 343: /* expr_cast: "cast" '<' $@20 type_declaration_no_options '>' $@21 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
    }
    break;

  case 344: /* $@22: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 345: /* $@23: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 346: /* expr_cast: "upcast" '<' $@22 type_declaration_no_options '>' $@23 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 347: /* $@24: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 348: /* $@25: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 349: /* expr_cast: "reinterpret" '<' $@24 type_declaration_no_options '>' $@25 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 350: /* $@26: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 351: /* $@27: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 352: /* expr_type_decl: "type" '<' $@26 type_declaration '>' $@27  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 353: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 354: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 355: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" c_or_s "name" '>' '(' expr ')'  */
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

  case 356: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 357: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 358: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 359: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 360: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 361: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 362: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 363: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 364: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 365: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 366: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 367: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 368: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 369: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 370: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 371: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 372: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 373: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis block_or_simple_block  */
                                                                                                                {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 374: /* expr_full_block_assumed_piped: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis expression_block  */
                                                                                                           {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 375: /* expr_full_block_assumed_piped: '{' expressions '}'  */
                                   {
        (yyval.pExpression) = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,new TypeDecl(Type::autoinfer),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 376: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 377: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 378: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 379: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 380: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 381: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 382: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 383: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 384: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 385: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 386: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 387: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 388: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 389: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 390: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 391: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 392: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 393: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 394: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 395: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 396: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 397: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 398: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 399: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 400: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 401: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 402: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 403: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 404: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 405: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 406: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 407: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 408: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 409: /* $@28: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 410: /* $@29: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 411: /* func_addr_expr: '@' '@' '<' $@28 type_declaration_no_options '>' $@29 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 412: /* $@30: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 413: /* $@31: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 414: /* func_addr_expr: '@' '@' '<' $@30 optional_function_argument_list optional_function_type '>' $@31 func_addr_name  */
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

  case 415: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 416: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 417: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 418: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 419: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
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

  case 420: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 421: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 422: /* $@32: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 423: /* $@33: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 424: /* expr_field: expr '.' $@32 error $@33  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 425: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 426: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 427: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 428: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-4])),*(yyvsp[-4].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 429: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 430: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 431: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 432: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 433: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 434: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 435: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 436: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 437: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 438: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 439: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 440: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 441: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 442: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",(yyvsp[0].pExpression)); }
    break;

  case 443: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",(yyvsp[0].pExpression)); }
    break;

  case 444: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",(yyvsp[0].pExpression)); }
    break;

  case 445: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",(yyvsp[0].pExpression)); }
    break;

  case 446: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 447: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 448: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 449: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 450: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 451: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 452: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 453: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 454: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 455: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 456: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 457: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 458: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 459: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 460: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 461: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 462: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 463: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 464: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 465: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 466: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 467: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 468: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", (yyvsp[0].pExpression)); }
    break;

  case 469: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", (yyvsp[0].pExpression)); }
    break;

  case 470: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 471: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 472: /* expr: '(' expr_list optional_comma ')'  */
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

  case 473: /* expr: '(' make_struct_single ')'  */
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

  case 474: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 475: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 476: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 477: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 478: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 479: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 480: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 481: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 482: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 483: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 484: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 485: /* expr: expr_generator  */
                                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 486: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 487: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 488: /* $@34: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 489: /* $@35: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 490: /* expr: expr "is" "type" '<' $@34 type_declaration_no_options '>' $@35  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 491: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 492: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 493: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 494: /* $@36: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 495: /* $@37: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 496: /* expr: expr "as" "type" '<' $@36 type_declaration '>' $@37  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 497: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 498: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 499: /* $@38: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 500: /* $@39: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 501: /* expr: expr '?' "as" "type" '<' $@38 type_declaration '>' $@39  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 502: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 503: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 504: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 505: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 506: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 507: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 508: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 509: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 510: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 511: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 512: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 513: /* expr: expr_call_pipe  */
                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 514: /* expr: "unsafe" '(' expr ')'  */
                                         {
            (yyvsp[-1].pExpression)->alwaysSafe = true;
            (yyvsp[-1].pExpression)->userSaidItsSafe = true;
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    break;

  case 515: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 516: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 517: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list optional_emit_semis expression_block  */
                                                                                                                                                  {
        auto closure = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),(yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->returnType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),closure,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 518: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 519: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 520: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 521: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 522: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 523: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 524: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 525: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 526: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 527: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 528: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 529: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 530: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 531: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 532: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 533: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 534: /* optional_field_annotation: %empty  */
                                      { (yyval.aaList) = nullptr; }
    break;

  case 535: /* optional_field_annotation: metadata_argument_list  */
                                      { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 536: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 537: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 538: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 539: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 540: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 541: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 542: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 543: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 544: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 545: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 546: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 547: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 548: /* struct_variable_declaration_list: struct_variable_declaration_list "new line, semicolon"  */
                                                                 { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 549: /* struct_variable_declaration_list: struct_variable_declaration_list "typedef" "name" '=' type_declaration SEMICOLON  */
                                                                                                                {
        (yyval.pVarDeclList) = (yyvsp[-5].pVarDeclList);
        ast_structureAlias(scanner,(yyvsp[-3].s),(yyvsp[-1].pTypeDecl),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 550: /* $@40: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 551: /* struct_variable_declaration_list: struct_variable_declaration_list $@40 structure_variable_declaration SEMICOLON  */
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

  case 552: /* $@41: %empty  */
                                                                                                                     {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 553: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable "abstract" optional_constant $@41 function_declaration_header SEMICOLON  */
                                                          {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 554: /* $@42: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 555: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@42 function_declaration_header optional_emit_semis expression_block  */
                                                                                            {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-10].pVarDeclList),(yyvsp[-9].faList),(yyvsp[-6].b),(yyvsp[-7].b),(yyvsp[-5].i),(yyvsp[-4].b),(yyvsp[-2].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-8]),(yylsp[0])),tokAt(scanner,(yylsp[-9])));
            }
    break;

  case 556: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
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

  case 557: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
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

  case 558: /* function_argument_declaration_type: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))));
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 559: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 560: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 561: /* function_argument_list: function_argument_declaration_no_type ';' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 562: /* function_argument_list: function_argument_declaration_type ';' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 563: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 564: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 565: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 566: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 567: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 568: /* tuple_alias_type_list: %empty  */
      {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 569: /* tuple_alias_type_list: tuple_type  */
                       {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 570: /* tuple_alias_type_list: tuple_alias_type_list semis tuple_type  */
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

  case 571: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 572: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 573: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 574: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 575: /* variant_alias_type_list: variant_type  */
                         {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 576: /* variant_alias_type_list: variant_alias_type_list semis variant_type  */
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

  case 577: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 578: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 579: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 580: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 581: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 582: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 583: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 584: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 585: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 586: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 587: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 588: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 589: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 590: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 591: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 592: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 593: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 594: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 595: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 596: /* global_let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 597: /* global_let_variable_name_with_pos_list: global_let_variable_name_with_pos_list ',' "name"  */
                                                                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 598: /* variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 599: /* variable_declaration_list: variable_declaration_list SEMICOLON  */
                                                  {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 600: /* variable_declaration_list: variable_declaration_list let_variable_declaration  */
                                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 601: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 602: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 603: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 604: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                         {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 605: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                               {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 606: /* global_let_variable_declaration: global_let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 607: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 608: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 609: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 610: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 611: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 612: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 613: /* global_variable_declaration_list: global_variable_declaration_list SEMICOLON  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 614: /* $@43: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 615: /* global_variable_declaration_list: global_variable_declaration_list $@43 optional_field_annotation let_variable_declaration  */
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

  case 616: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 617: /* $@44: %empty  */
                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 618: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@44 optional_field_annotation global_let_variable_declaration  */
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

  case 619: /* enum_expression: "name"  */
                   {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        delete (yyvsp[0].s);
    }
    break;

  case 620: /* enum_expression: "name" '=' expr  */
                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[-2].s),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-2])));
        delete (yyvsp[-2].s);
    }
    break;

  case 623: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 624: /* enum_list: enum_expression  */
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

  case 625: /* enum_list: enum_list commas enum_expression  */
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

  case 626: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 627: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 628: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 629: /* $@45: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 630: /* single_alias: optional_public_or_private_alias "name" $@45 '=' type_declaration  */
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

  case 632: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 633: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 634: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 635: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 636: /* optional_enum_basic_type_declaration: %empty  */
        {
        (yyval.type) = Type::tInt;
    }
    break;

  case 637: /* optional_enum_basic_type_declaration: ':' enum_basic_type_declaration  */
                                              {
        (yyval.type) = (yyvsp[0].type);
    }
    break;

  case 644: /* $@46: %empty  */
                                                                     {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 645: /* $@47: %empty  */
                                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 646: /* $@48: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 647: /* enum_declaration: optional_annotation_list_with_emit_semis "enum" $@46 optional_public_or_private_enum enum_name optional_enum_basic_type_declaration optional_emit_commas '{' $@47 enum_list optional_commas $@48 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-8].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-12].faList),tokAt(scanner,(yylsp[-12])),(yyvsp[-9].b),(yyvsp[-8].pEnum),(yyvsp[-3].pEnum),(yyvsp[-7].type));
    }
    break;

  case 648: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 649: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 650: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 651: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 652: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 653: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 654: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 655: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 656: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 657: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 658: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 659: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 660: /* optional_struct_variable_declaration_list: ';'  */
            {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 661: /* optional_struct_variable_declaration_list: '{' struct_variable_declaration_list '}'  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 662: /* $@49: %empty  */
                                                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 663: /* $@50: %empty  */
                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 664: /* $@51: %empty  */
                                             {
        if ( (yyvsp[-1].pStructure) ) {
            (yyvsp[-1].pStructure)->isClass = (yyvsp[-4].i)==CorS_Class || (yyvsp[-4].i)==CorS_ClassTemplate;
            (yyvsp[-1].pStructure)->isTemplate = (yyvsp[-4].i)==CorS_ClassTemplate || (yyvsp[-4].i)==CorS_StructTemplate;
            (yyvsp[-1].pStructure)->privateStructure = !(yyvsp[-3].b);
        }
    }
    break;

  case 665: /* structure_declaration: optional_annotation_list_with_emit_semis $@49 class_or_struct optional_public_or_private_structure $@50 structure_name optional_emit_semis $@51 optional_struct_variable_declaration_list  */
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

  case 666: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 667: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 668: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 669: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 670: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 671: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 672: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 673: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 674: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 675: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 676: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 677: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 678: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 679: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 680: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 681: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 682: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 683: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 684: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 685: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 686: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 687: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 688: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 689: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 690: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 691: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 692: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 693: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 694: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 695: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 696: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 697: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 698: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 699: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 700: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 701: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 702: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 703: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 704: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 705: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 706: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 707: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 708: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 709: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 710: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 711: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 712: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<tuple<string,Expression *>>();
        (yyval.pNameExprList) = pSL;

    }
    break;

  case 713: /* bitfield_alias_bits: "name"  */
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

  case 714: /* bitfield_alias_bits: "name" '=' expr  */
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

  case 715: /* bitfield_alias_bits: bitfield_alias_bits commas "name"  */
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

  case 716: /* bitfield_alias_bits: bitfield_alias_bits commas "name" '=' expr  */
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

  case 717: /* bitfield_basic_type_declaration: %empty  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 718: /* bitfield_basic_type_declaration: ':' "uint8"  */
                             { (yyval.type) = Type::tBitfield8; }
    break;

  case 719: /* bitfield_basic_type_declaration: ':' "uint16"  */
                             { (yyval.type) = Type::tBitfield16; }
    break;

  case 720: /* bitfield_basic_type_declaration: ':' "uint"  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 721: /* bitfield_basic_type_declaration: ':' "uint64"  */
                             { (yyval.type) = Type::tBitfield64; }
    break;

  case 722: /* $@52: %empty  */
                                                                     { yyextra->das_arrow_depth ++; }
    break;

  case 723: /* $@53: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 724: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' $@52 bitfield_bits '>' $@53  */
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

  case 727: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 728: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 729: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 730: /* dim_list: '[' ']'  */
                {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 731: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 732: /* dim_list: dim_list '[' ']'  */
                              {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 733: /* type_declaration_no_options: type_declaration_no_options_no_dim  */
                                                     {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 734: /* type_declaration_no_options: type_declaration_no_options_no_dim dim_list  */
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

  case 735: /* optional_expr_list_in_braces: %empty  */
            { (yyval.pExpression) = nullptr; }
    break;

  case 736: /* optional_expr_list_in_braces: '(' expr_list optional_comma ')'  */
                                                { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 737: /* type_declaration_no_options_no_dim: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 738: /* type_declaration_no_options_no_dim: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 739: /* type_declaration_no_options_no_dim: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 740: /* type_declaration_no_options_no_dim: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 741: /* $@54: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 742: /* $@55: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 743: /* type_declaration_no_options_no_dim: "type" '<' $@54 type_declaration '>' $@55  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 744: /* type_declaration_no_options_no_dim: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 745: /* type_declaration_no_options_no_dim: name_in_namespace '(' optional_expr_list ')'  */
                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 746: /* type_declaration_no_options_no_dim: '$' name_in_namespace optional_expr_list_in_braces  */
                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-1]), (yylsp[0]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[0].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-1])), *(yyvsp[-1].s)));
        delete (yyvsp[-1].s);
    }
    break;

  case 747: /* $@56: %empty  */
                                    { yyextra->das_arrow_depth ++; }
    break;

  case 748: /* type_declaration_no_options_no_dim: name_in_namespace '<' $@56 type_declaration_no_options_list '>' optional_expr_list_in_braces  */
                                                                                                                                                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-5]), (yylsp[0]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-2].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-5])), *(yyvsp[-5].s)));
        delete (yyvsp[-5].s);
    }
    break;

  case 749: /* $@57: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 750: /* type_declaration_no_options_no_dim: '$' name_in_namespace '<' $@57 type_declaration_no_options_list '>' optional_expr_list_in_braces  */
                                                                                                                                                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-5]), (yylsp[0]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-2].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-5])), *(yyvsp[-5].s)));
        delete (yyvsp[-5].s);
    }
    break;

  case 751: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 752: /* type_declaration_no_options_no_dim: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 753: /* type_declaration_no_options_no_dim: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 754: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 755: /* type_declaration_no_options_no_dim: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 756: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 757: /* type_declaration_no_options_no_dim: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 758: /* type_declaration_no_options_no_dim: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 759: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 760: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 761: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 762: /* type_declaration_no_options_no_dim: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 763: /* $@58: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 764: /* $@59: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 765: /* type_declaration_no_options_no_dim: "smart_ptr" '<' $@58 type_declaration '>' $@59  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 766: /* type_declaration_no_options_no_dim: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 767: /* $@60: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 768: /* $@61: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 769: /* type_declaration_no_options_no_dim: "array" '<' $@60 type_declaration '>' $@61  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 770: /* $@62: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 771: /* $@63: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 772: /* type_declaration_no_options_no_dim: "table" '<' $@62 table_type_pair '>' $@63  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 773: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 774: /* $@65: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 775: /* type_declaration_no_options_no_dim: "iterator" '<' $@64 type_declaration '>' $@65  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 776: /* type_declaration_no_options_no_dim: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 777: /* $@66: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 778: /* $@67: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 779: /* type_declaration_no_options_no_dim: "block" '<' $@66 type_declaration '>' $@67  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 780: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 781: /* $@69: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 782: /* type_declaration_no_options_no_dim: "block" '<' $@68 optional_function_argument_list optional_function_type '>' $@69  */
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

  case 783: /* type_declaration_no_options_no_dim: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 784: /* $@70: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 785: /* $@71: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 786: /* type_declaration_no_options_no_dim: "function" '<' $@70 type_declaration '>' $@71  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 787: /* $@72: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 788: /* $@73: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 789: /* type_declaration_no_options_no_dim: "function" '<' $@72 optional_function_argument_list optional_function_type '>' $@73  */
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

  case 790: /* type_declaration_no_options_no_dim: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 791: /* $@74: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 792: /* $@75: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 793: /* type_declaration_no_options_no_dim: "lambda" '<' $@74 type_declaration '>' $@75  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 794: /* $@76: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 795: /* $@77: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 796: /* type_declaration_no_options_no_dim: "lambda" '<' $@76 optional_function_argument_list optional_function_type '>' $@77  */
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

  case 797: /* $@78: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 798: /* $@79: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 799: /* type_declaration_no_options_no_dim: "tuple" '<' $@78 tuple_type_list '>' $@79  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 800: /* $@80: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 801: /* $@81: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 802: /* type_declaration_no_options_no_dim: "variant" '<' $@80 variant_type_list '>' $@81  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 803: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 804: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 805: /* type_declaration: type_declaration '|' '#'  */
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

  case 806: /* $@82: %empty  */
                   {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 807: /* $@83: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 808: /* $@84: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 809: /* $@85: %empty  */
                                                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 810: /* tuple_alias_declaration: "tuple" $@82 optional_public_or_private_alias "name" optional_emit_semis $@83 '{' $@84 tuple_alias_type_list optional_semis $@85 '}'  */
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

  case 811: /* $@86: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 812: /* $@87: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 813: /* $@88: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 814: /* $@89: %empty  */
                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 815: /* variant_alias_declaration: "variant" $@86 optional_public_or_private_alias "name" optional_emit_semis $@87 '{' $@88 variant_alias_type_list optional_semis $@89 '}'  */
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

  case 816: /* $@90: %empty  */
                      {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 817: /* $@91: %empty  */
                                                                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 818: /* $@92: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 819: /* $@93: %empty  */
                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 820: /* bitfield_alias_declaration: "bitfield" $@90 optional_public_or_private_alias "name" bitfield_basic_type_declaration optional_emit_commas $@91 '{' $@92 bitfield_alias_bits optional_commas $@93 '}'  */
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

  case 821: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 822: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 823: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 824: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 825: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 826: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 827: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 828: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 829: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 830: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 831: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 832: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 833: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 834: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 835: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 836: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 837: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 838: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 839: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 840: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 841: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 842: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 843: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 844: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 845: /* $@94: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 846: /* $@95: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 847: /* make_struct_decl: "struct" '<' $@94 type_declaration_no_options '>' $@95 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 848: /* $@96: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 849: /* $@97: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 850: /* make_struct_decl: "class" '<' $@96 type_declaration_no_options '>' $@97 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 851: /* $@98: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 852: /* $@99: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 853: /* make_struct_decl: "variant" '<' $@98 variant_type_list '>' $@99 '(' use_initializer make_variant_dim ')'  */
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

  case 854: /* $@100: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 855: /* $@101: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 856: /* make_struct_decl: "default" '<' $@100 type_declaration_no_options '>' $@101 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 857: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 858: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 859: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 860: /* $@102: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 861: /* $@103: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 862: /* make_tuple_call: "tuple" '<' $@102 tuple_type_list '>' $@103 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 863: /* make_dim_decl: '[' optional_expr_list ']'  */
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

  case 864: /* $@104: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 865: /* $@105: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 866: /* make_dim_decl: "array" "struct" '<' $@104 type_declaration_no_options '>' $@105 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 867: /* $@106: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 868: /* $@107: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 869: /* make_dim_decl: "array" "tuple" '<' $@106 tuple_type_list '>' $@107 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 870: /* $@108: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 871: /* $@109: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 872: /* make_dim_decl: "array" "variant" '<' $@108 variant_type_list '>' $@109 '(' make_variant_dim ')'  */
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

  case 873: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 874: /* $@110: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 875: /* $@111: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 876: /* make_dim_decl: "array" '<' $@110 type_declaration_no_options '>' $@111 '(' optional_expr_list ')'  */
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

  case 877: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 878: /* $@112: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 879: /* $@113: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 880: /* make_dim_decl: "fixed_array" '<' $@112 type_declaration_no_options '>' $@113 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 881: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 882: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 883: /* $@114: %empty  */
                  {
        yyextra->das_nested_parentheses ++;
    }
    break;

  case 884: /* make_table_decl: '{' $@114 optional_emit_semis optional_expr_map_tuple_list '}'  */
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

  case 885: /* make_table_decl: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 886: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 887: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 888: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 889: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 890: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 891: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 892: /* array_comprehension: '[' "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                               {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 893: /* array_comprehension: '[' "iterator" "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 894: /* array_comprehension: '{' "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' make_map_tuple array_comprehension_where '}'  */
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


