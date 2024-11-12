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
  YYSYMBOL_BEGIN_STRING = 163,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 164,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 165,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 166,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 167,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 168,          /* "}"  */
  YYSYMBOL_END_OF_READ = 169,              /* "end of failed eader macro"  */
  YYSYMBOL_170_begin_of_code_block_ = 170, /* "begin of code block"  */
  YYSYMBOL_171_end_of_code_block_ = 171,   /* "end of code block"  */
  YYSYMBOL_172_end_of_expression_ = 172,   /* "end of expression"  */
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
  YYSYMBOL_LLPIPE = 198,                   /* LLPIPE  */
  YYSYMBOL_POST_INC = 199,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 200,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 201,                    /* DEREF  */
  YYSYMBOL_202_ = 202,                     /* '.'  */
  YYSYMBOL_203_ = 203,                     /* '['  */
  YYSYMBOL_204_ = 204,                     /* ']'  */
  YYSYMBOL_205_ = 205,                     /* '('  */
  YYSYMBOL_206_ = 206,                     /* ')'  */
  YYSYMBOL_207_ = 207,                     /* '$'  */
  YYSYMBOL_208_ = 208,                     /* '@'  */
  YYSYMBOL_209_ = 209,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 210,                 /* $accept  */
  YYSYMBOL_program = 211,                  /* program  */
  YYSYMBOL_top_level_reader_macro = 212,   /* top_level_reader_macro  */
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
  YYSYMBOL_require_module_name = 225,      /* require_module_name  */
  YYSYMBOL_require_module = 226,           /* require_module  */
  YYSYMBOL_is_public_module = 227,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 228,       /* expect_declaration  */
  YYSYMBOL_expect_list = 229,              /* expect_list  */
  YYSYMBOL_expect_error = 230,             /* expect_error  */
  YYSYMBOL_expression_label = 231,         /* expression_label  */
  YYSYMBOL_expression_goto = 232,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 233,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 234,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 235,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 236, /* expression_else_one_liner  */
  YYSYMBOL_expression_if_one_liner = 237,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 238,  /* expression_if_then_else  */
  YYSYMBOL_expression_for_loop = 239,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 240,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 241,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 242,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 243,    /* expression_with_alias  */
  YYSYMBOL_annotation_argument_value = 244, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 245, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 246, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 247,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 248, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 249,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 250, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 251, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 252,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 253,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 254, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 255, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 256,   /* optional_function_type  */
  YYSYMBOL_function_name = 257,            /* function_name  */
  YYSYMBOL_global_function_declaration = 258, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 259, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 260, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 261,     /* function_declaration  */
  YYSYMBOL_262_2 = 262,                    /* $@2  */
  YYSYMBOL_expression_block = 263,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 264,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 265,           /* expression_any  */
  YYSYMBOL_expressions = 266,              /* expressions  */
  YYSYMBOL_optional_expr_list = 267,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_map_tuple_list = 268, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 269, /* type_declaration_no_options_list  */
  YYSYMBOL_name_in_namespace = 270,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 271,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 272,     /* new_type_declaration  */
  YYSYMBOL_273_3 = 273,                    /* $@3  */
  YYSYMBOL_274_4 = 274,                    /* $@4  */
  YYSYMBOL_expr_new = 275,                 /* expr_new  */
  YYSYMBOL_expression_break = 276,         /* expression_break  */
  YYSYMBOL_expression_continue = 277,      /* expression_continue  */
  YYSYMBOL_expression_return = 278,        /* expression_return  */
  YYSYMBOL_expression_yield = 279,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 280,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 281,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 282,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 283,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 284,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 285, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 286,           /* expression_let  */
  YYSYMBOL_expr_cast = 287,                /* expr_cast  */
  YYSYMBOL_288_5 = 288,                    /* $@5  */
  YYSYMBOL_289_6 = 289,                    /* $@6  */
  YYSYMBOL_290_7 = 290,                    /* $@7  */
  YYSYMBOL_291_8 = 291,                    /* $@8  */
  YYSYMBOL_292_9 = 292,                    /* $@9  */
  YYSYMBOL_293_10 = 293,                   /* $@10  */
  YYSYMBOL_expr_type_decl = 294,           /* expr_type_decl  */
  YYSYMBOL_295_11 = 295,                   /* $@11  */
  YYSYMBOL_296_12 = 296,                   /* $@12  */
  YYSYMBOL_expr_type_info = 297,           /* expr_type_info  */
  YYSYMBOL_expr_list = 298,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 299,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 300,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 301,            /* capture_entry  */
  YYSYMBOL_capture_list = 302,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 303,    /* optional_capture_list  */
  YYSYMBOL_expr_full_block = 304,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 305, /* expr_full_block_assumed_piped  */
  YYSYMBOL_expr_numeric_const = 306,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 307,              /* expr_assign  */
  YYSYMBOL_expr_named_call = 308,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 309,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 310,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 311,           /* func_addr_expr  */
  YYSYMBOL_312_13 = 312,                   /* $@13  */
  YYSYMBOL_313_14 = 313,                   /* $@14  */
  YYSYMBOL_314_15 = 314,                   /* $@15  */
  YYSYMBOL_315_16 = 315,                   /* $@16  */
  YYSYMBOL_expr_field = 316,               /* expr_field  */
  YYSYMBOL_317_17 = 317,                   /* $@17  */
  YYSYMBOL_318_18 = 318,                   /* $@18  */
  YYSYMBOL_expr_call = 319,                /* expr_call  */
  YYSYMBOL_expr = 320,                     /* expr  */
  YYSYMBOL_321_19 = 321,                   /* $@19  */
  YYSYMBOL_322_20 = 322,                   /* $@20  */
  YYSYMBOL_323_21 = 323,                   /* $@21  */
  YYSYMBOL_324_22 = 324,                   /* $@22  */
  YYSYMBOL_325_23 = 325,                   /* $@23  */
  YYSYMBOL_326_24 = 326,                   /* $@24  */
  YYSYMBOL_expr_mtag = 327,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 328, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 329,        /* optional_override  */
  YYSYMBOL_optional_constant = 330,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 331, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 332, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 333, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 334, /* struct_variable_declaration_list  */
  YYSYMBOL_335_25 = 335,                   /* $@25  */
  YYSYMBOL_336_26 = 336,                   /* $@26  */
  YYSYMBOL_337_27 = 337,                   /* $@27  */
  YYSYMBOL_function_argument_declaration = 338, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 339,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 340,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 341,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 342,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 343,             /* variant_type  */
  YYSYMBOL_variant_type_list = 344,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 345,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 346,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 347,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 348,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 349,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 350, /* let_variable_name_with_pos_list  */
  YYSYMBOL_global_let_variable_name_with_pos_list = 351, /* global_let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 352, /* let_variable_declaration  */
  YYSYMBOL_global_let_variable_declaration = 353, /* global_let_variable_declaration  */
  YYSYMBOL_optional_shared = 354,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 355, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 356,               /* global_let  */
  YYSYMBOL_357_28 = 357,                   /* $@28  */
  YYSYMBOL_enum_expression = 358,          /* enum_expression  */
  YYSYMBOL_enum_list = 359,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 360, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 361,             /* single_alias  */
  YYSYMBOL_362_29 = 362,                   /* $@29  */
  YYSYMBOL_alias_declaration = 363,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 364, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 365,                /* enum_name  */
  YYSYMBOL_optional_enum_basic_type_declaration = 366, /* optional_enum_basic_type_declaration  */
  YYSYMBOL_enum_declaration = 367,         /* enum_declaration  */
  YYSYMBOL_368_30 = 368,                   /* $@30  */
  YYSYMBOL_369_31 = 369,                   /* $@31  */
  YYSYMBOL_optional_structure_parent = 370, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 371,          /* optional_sealed  */
  YYSYMBOL_structure_name = 372,           /* structure_name  */
  YYSYMBOL_class_or_struct = 373,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 374, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 375, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 376,    /* structure_declaration  */
  YYSYMBOL_377_32 = 377,                   /* $@32  */
  YYSYMBOL_378_33 = 378,                   /* $@33  */
  YYSYMBOL_variable_name_with_pos_list = 379, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 380,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 381, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 382, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 383,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 384,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 385,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 386, /* bitfield_type_declaration  */
  YYSYMBOL_387_34 = 387,                   /* $@34  */
  YYSYMBOL_388_35 = 388,                   /* $@35  */
  YYSYMBOL_c_or_s = 389,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 390,          /* table_type_pair  */
  YYSYMBOL_dim_list = 391,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 392, /* type_declaration_no_options  */
  YYSYMBOL_type_declaration_no_options_no_dim = 393, /* type_declaration_no_options_no_dim  */
  YYSYMBOL_394_36 = 394,                   /* $@36  */
  YYSYMBOL_395_37 = 395,                   /* $@37  */
  YYSYMBOL_396_38 = 396,                   /* $@38  */
  YYSYMBOL_397_39 = 397,                   /* $@39  */
  YYSYMBOL_398_40 = 398,                   /* $@40  */
  YYSYMBOL_399_41 = 399,                   /* $@41  */
  YYSYMBOL_400_42 = 400,                   /* $@42  */
  YYSYMBOL_401_43 = 401,                   /* $@43  */
  YYSYMBOL_402_44 = 402,                   /* $@44  */
  YYSYMBOL_403_45 = 403,                   /* $@45  */
  YYSYMBOL_404_46 = 404,                   /* $@46  */
  YYSYMBOL_405_47 = 405,                   /* $@47  */
  YYSYMBOL_406_48 = 406,                   /* $@48  */
  YYSYMBOL_407_49 = 407,                   /* $@49  */
  YYSYMBOL_408_50 = 408,                   /* $@50  */
  YYSYMBOL_409_51 = 409,                   /* $@51  */
  YYSYMBOL_410_52 = 410,                   /* $@52  */
  YYSYMBOL_411_53 = 411,                   /* $@53  */
  YYSYMBOL_412_54 = 412,                   /* $@54  */
  YYSYMBOL_413_55 = 413,                   /* $@55  */
  YYSYMBOL_414_56 = 414,                   /* $@56  */
  YYSYMBOL_415_57 = 415,                   /* $@57  */
  YYSYMBOL_416_58 = 416,                   /* $@58  */
  YYSYMBOL_417_59 = 417,                   /* $@59  */
  YYSYMBOL_418_60 = 418,                   /* $@60  */
  YYSYMBOL_419_61 = 419,                   /* $@61  */
  YYSYMBOL_420_62 = 420,                   /* $@62  */
  YYSYMBOL_type_declaration = 421,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 422,  /* tuple_alias_declaration  */
  YYSYMBOL_423_63 = 423,                   /* $@63  */
  YYSYMBOL_424_64 = 424,                   /* $@64  */
  YYSYMBOL_425_65 = 425,                   /* $@65  */
  YYSYMBOL_variant_alias_declaration = 426, /* variant_alias_declaration  */
  YYSYMBOL_427_66 = 427,                   /* $@66  */
  YYSYMBOL_428_67 = 428,                   /* $@67  */
  YYSYMBOL_429_68 = 429,                   /* $@68  */
  YYSYMBOL_bitfield_alias_declaration = 430, /* bitfield_alias_declaration  */
  YYSYMBOL_431_69 = 431,                   /* $@69  */
  YYSYMBOL_432_70 = 432,                   /* $@70  */
  YYSYMBOL_433_71 = 433,                   /* $@71  */
  YYSYMBOL_make_decl = 434,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 435,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 436,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 437,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim_list = 438,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 439,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 440, /* optional_make_struct_dim_decl  */
  YYSYMBOL_use_initializer = 441,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 442,         /* make_struct_decl  */
  YYSYMBOL_443_72 = 443,                   /* $@72  */
  YYSYMBOL_444_73 = 444,                   /* $@73  */
  YYSYMBOL_445_74 = 445,                   /* $@74  */
  YYSYMBOL_446_75 = 446,                   /* $@75  */
  YYSYMBOL_447_76 = 447,                   /* $@76  */
  YYSYMBOL_448_77 = 448,                   /* $@77  */
  YYSYMBOL_449_78 = 449,                   /* $@78  */
  YYSYMBOL_450_79 = 450,                   /* $@79  */
  YYSYMBOL_make_map_tuple = 451,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 452,          /* make_tuple_call  */
  YYSYMBOL_453_80 = 453,                   /* $@80  */
  YYSYMBOL_454_81 = 454,                   /* $@81  */
  YYSYMBOL_make_dim_decl = 455,            /* make_dim_decl  */
  YYSYMBOL_456_82 = 456,                   /* $@82  */
  YYSYMBOL_457_83 = 457,                   /* $@83  */
  YYSYMBOL_458_84 = 458,                   /* $@84  */
  YYSYMBOL_459_85 = 459,                   /* $@85  */
  YYSYMBOL_460_86 = 460,                   /* $@86  */
  YYSYMBOL_461_87 = 461,                   /* $@87  */
  YYSYMBOL_462_88 = 462,                   /* $@88  */
  YYSYMBOL_463_89 = 463,                   /* $@89  */
  YYSYMBOL_464_90 = 464,                   /* $@90  */
  YYSYMBOL_465_91 = 465,                   /* $@91  */
  YYSYMBOL_expr_map_tuple_list = 466,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 467,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 468, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 469,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 470       /* array_comprehension  */
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
#define YYLAST   11020

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  210
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  261
/* YYNRULES -- Number of rules.  */
#define YYNRULES  781
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1435

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   437


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
       2,     2,     2,   195,     2,   209,   207,   191,   184,     2,
     205,   206,   189,   188,   178,   187,   202,   190,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   181,   172,
     185,   179,   186,   180,   208,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   203,     2,   204,   183,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   170,   182,   171,   194,     2,     2,     2,
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
     165,   166,   167,   168,   169,   173,   174,   175,   176,   177,
     192,   193,   196,   197,   198,   199,   200,   201
};

#if DAS2_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   533,   533,   534,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   551,   555,   561,   562,
     563,   567,   568,   572,   590,   591,   592,   593,   597,   601,
     606,   615,   623,   639,   644,   652,   652,   691,   709,   713,
     716,   720,   726,   735,   738,   744,   745,   749,   753,   754,
     758,   761,   767,   773,   776,   782,   783,   787,   788,   789,
     798,   799,   803,   804,   810,   811,   812,   813,   814,   818,
     824,   830,   836,   844,   854,   863,   870,   871,   872,   873,
     874,   875,   879,   884,   892,   893,   894,   898,   899,   900,
     901,   902,   903,   904,   905,   911,   914,   920,   923,   929,
     930,   931,   935,   948,   966,   969,   977,   988,   999,  1010,
    1013,  1020,  1024,  1031,  1032,  1036,  1037,  1038,  1042,  1045,
    1052,  1056,  1057,  1058,  1059,  1060,  1061,  1062,  1063,  1064,
    1065,  1066,  1067,  1068,  1069,  1070,  1071,  1072,  1073,  1074,
    1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,  1084,
    1085,  1086,  1087,  1088,  1089,  1090,  1091,  1092,  1093,  1094,
    1095,  1096,  1097,  1098,  1099,  1100,  1101,  1102,  1103,  1104,
    1105,  1106,  1107,  1108,  1109,  1110,  1111,  1112,  1113,  1114,
    1115,  1116,  1117,  1118,  1119,  1120,  1121,  1122,  1123,  1124,
    1125,  1126,  1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,
    1135,  1136,  1137,  1138,  1139,  1144,  1162,  1163,  1164,  1168,
    1174,  1174,  1191,  1195,  1206,  1219,  1220,  1221,  1222,  1223,
    1224,  1225,  1226,  1227,  1228,  1229,  1230,  1231,  1232,  1233,
    1234,  1235,  1236,  1240,  1245,  1251,  1257,  1258,  1262,  1263,
    1267,  1271,  1278,  1279,  1290,  1294,  1297,  1305,  1305,  1305,
    1311,  1314,  1318,  1322,  1329,  1335,  1339,  1343,  1346,  1349,
    1357,  1360,  1368,  1374,  1375,  1376,  1380,  1381,  1385,  1386,
    1390,  1395,  1403,  1409,  1421,  1424,  1430,  1430,  1430,  1433,
    1433,  1433,  1438,  1438,  1438,  1446,  1446,  1446,  1452,  1462,
    1473,  1488,  1491,  1497,  1498,  1505,  1516,  1517,  1518,  1522,
    1523,  1524,  1525,  1526,  1530,  1535,  1543,  1544,  1548,  1555,
    1559,  1565,  1566,  1567,  1568,  1569,  1570,  1571,  1575,  1576,
    1577,  1578,  1579,  1580,  1581,  1582,  1583,  1584,  1585,  1586,
    1587,  1588,  1589,  1590,  1591,  1592,  1593,  1597,  1604,  1616,
    1621,  1631,  1635,  1642,  1645,  1645,  1645,  1650,  1650,  1650,
    1663,  1667,  1671,  1676,  1683,  1688,  1695,  1695,  1695,  1702,
    1706,  1716,  1725,  1734,  1738,  1741,  1747,  1748,  1749,  1750,
    1751,  1752,  1753,  1754,  1755,  1756,  1757,  1758,  1759,  1760,
    1761,  1762,  1763,  1764,  1765,  1766,  1767,  1768,  1769,  1770,
    1771,  1772,  1773,  1774,  1775,  1776,  1777,  1778,  1779,  1780,
    1781,  1782,  1788,  1789,  1790,  1791,  1792,  1805,  1806,  1807,
    1808,  1809,  1810,  1811,  1812,  1813,  1814,  1815,  1816,  1819,
    1822,  1827,  1828,  1831,  1831,  1831,  1834,  1839,  1843,  1847,
    1847,  1847,  1852,  1855,  1859,  1859,  1859,  1864,  1867,  1868,
    1869,  1870,  1871,  1872,  1873,  1874,  1875,  1877,  1881,  1882,
    1890,  1891,  1892,  1893,  1894,  1895,  1896,  1900,  1904,  1908,
    1912,  1916,  1920,  1924,  1928,  1932,  1939,  1940,  1944,  1945,
    1946,  1950,  1951,  1955,  1956,  1957,  1961,  1962,  1966,  1977,
    1980,  1980,  1999,  1998,  2012,  2011,  2027,  2036,  2046,  2047,
    2051,  2054,  2063,  2064,  2068,  2071,  2074,  2090,  2099,  2100,
    2104,  2107,  2110,  2124,  2125,  2129,  2135,  2141,  2144,  2148,
    2157,  2158,  2159,  2163,  2164,  2168,  2175,  2180,  2189,  2195,
    2206,  2213,  2223,  2226,  2231,  2242,  2245,  2250,  2262,  2263,
    2267,  2268,  2269,  2273,  2273,  2291,  2295,  2302,  2305,  2318,
    2335,  2336,  2337,  2342,  2342,  2368,  2372,  2373,  2374,  2378,
    2388,  2391,  2397,  2402,  2397,  2417,  2418,  2422,  2423,  2427,
    2433,  2434,  2438,  2439,  2440,  2444,  2447,  2453,  2458,  2453,
    2472,  2479,  2484,  2493,  2499,  2510,  2511,  2512,  2513,  2514,
    2515,  2516,  2517,  2518,  2519,  2520,  2521,  2522,  2523,  2524,
    2525,  2526,  2527,  2528,  2529,  2530,  2531,  2532,  2533,  2534,
    2535,  2536,  2540,  2541,  2542,  2543,  2544,  2545,  2546,  2547,
    2551,  2562,  2566,  2573,  2585,  2592,  2601,  2606,  2616,  2629,
    2629,  2629,  2642,  2643,  2647,  2651,  2658,  2662,  2666,  2670,
    2677,  2680,  2698,  2699,  2700,  2701,  2702,  2702,  2702,  2706,
    2711,  2718,  2718,  2725,  2729,  2733,  2738,  2743,  2748,  2753,
    2757,  2761,  2766,  2770,  2774,  2779,  2779,  2779,  2785,  2792,
    2792,  2792,  2797,  2797,  2797,  2803,  2803,  2803,  2808,  2812,
    2812,  2812,  2817,  2817,  2817,  2826,  2830,  2830,  2830,  2835,
    2835,  2835,  2844,  2848,  2848,  2848,  2853,  2853,  2853,  2862,
    2862,  2862,  2868,  2868,  2868,  2877,  2880,  2891,  2907,  2912,
    2917,  2907,  2942,  2947,  2953,  2942,  2978,  2983,  2988,  2978,
    3018,  3019,  3020,  3021,  3022,  3026,  3033,  3040,  3046,  3052,
    3059,  3066,  3072,  3081,  3087,  3095,  3100,  3107,  3112,  3118,
    3119,  3123,  3124,  3128,  3128,  3128,  3136,  3136,  3136,  3143,
    3143,  3143,  3150,  3150,  3150,  3161,  3167,  3173,  3179,  3179,
    3179,  3189,  3197,  3197,  3197,  3207,  3207,  3207,  3217,  3217,
    3217,  3227,  3235,  3235,  3235,  3254,  3261,  3261,  3261,  3271,
    3274,  3280,  3288,  3296,  3316,  3341,  3342,  3346,  3347,  3352,
    3355,  3358
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
  "\"double constant\"", "\"name\"", "\"start of the string\"",
  "STRING_CHARACTER", "STRING_CHARACTER_ESC", "\"end of the string\"",
  "\"{\"", "\"}\"", "\"end of failed eader macro\"",
  "\"begin of code block\"", "\"end of code block\"",
  "\"end of expression\"", "\";}}\"", "\";}]\"", "\";]]\"", "\",]]\"",
  "\",}]\"", "','", "'='", "'?'", "':'", "'|'", "'^'", "'&'", "'<'", "'>'",
  "'-'", "'+'", "'*'", "'/'", "'%'", "UNARY_MINUS", "UNARY_PLUS", "'~'",
  "'!'", "PRE_INC", "PRE_DEC", "LLPIPE", "POST_INC", "POST_DEC", "DEREF",
  "'.'", "'['", "']'", "'('", "')'", "'$'", "'@'", "'#'", "$accept",
  "program", "top_level_reader_macro", "optional_public_or_private_module",
  "module_name", "module_declaration", "character_sequence",
  "string_constant", "string_builder_body", "string_builder",
  "reader_character_sequence", "expr_reader", "$@1", "options_declaration",
  "require_declaration", "require_module_name", "require_module",
  "is_public_module", "expect_declaration", "expect_list", "expect_error",
  "expression_label", "expression_goto", "elif_or_static_elif",
  "expression_else", "if_or_static_if", "expression_else_one_liner",
  "expression_if_one_liner", "expression_if_then_else",
  "expression_for_loop", "expression_unsafe", "expression_while_loop",
  "expression_with", "expression_with_alias", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "metadata_argument_list", "annotation_declaration_name",
  "annotation_declaration_basic", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@2", "expression_block", "expr_call_pipe",
  "expression_any", "expressions", "optional_expr_list",
  "optional_expr_map_tuple_list", "type_declaration_no_options_list",
  "name_in_namespace", "expression_delete", "new_type_declaration", "$@3",
  "$@4", "expr_new", "expression_break", "expression_continue",
  "expression_return", "expression_yield", "expression_try_catch",
  "kwd_let_var_or_nothing", "kwd_let", "optional_in_scope",
  "tuple_expansion", "tuple_expansion_variable_declaration",
  "expression_let", "expr_cast", "$@5", "$@6", "$@7", "$@8", "$@9", "$@10",
  "expr_type_decl", "$@11", "$@12", "expr_type_info", "expr_list",
  "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_full_block",
  "expr_full_block_assumed_piped", "expr_numeric_const", "expr_assign",
  "expr_named_call", "expr_method_call", "func_addr_name",
  "func_addr_expr", "$@13", "$@14", "$@15", "$@16", "expr_field", "$@17",
  "$@18", "expr_call", "expr", "$@19", "$@20", "$@21", "$@22", "$@23",
  "$@24", "expr_mtag", "optional_field_annotation", "optional_override",
  "optional_constant", "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@25", "$@26", "$@27",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list",
  "global_let_variable_name_with_pos_list", "let_variable_declaration",
  "global_let_variable_declaration", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@28",
  "enum_expression", "enum_list", "optional_public_or_private_alias",
  "single_alias", "$@29", "alias_declaration",
  "optional_public_or_private_enum", "enum_name",
  "optional_enum_basic_type_declaration", "enum_declaration", "$@30",
  "$@31", "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@32", "$@33", "variable_name_with_pos_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "bitfield_bits", "bitfield_alias_bits",
  "bitfield_type_declaration", "$@34", "$@35", "c_or_s", "table_type_pair",
  "dim_list", "type_declaration_no_options",
  "type_declaration_no_options_no_dim", "$@36", "$@37", "$@38", "$@39",
  "$@40", "$@41", "$@42", "$@43", "$@44", "$@45", "$@46", "$@47", "$@48",
  "$@49", "$@50", "$@51", "$@52", "$@53", "$@54", "$@55", "$@56", "$@57",
  "$@58", "$@59", "$@60", "$@61", "$@62", "type_declaration",
  "tuple_alias_declaration", "$@63", "$@64", "$@65",
  "variant_alias_declaration", "$@66", "$@67", "$@68",
  "bitfield_alias_declaration", "$@69", "$@70", "$@71", "make_decl",
  "make_struct_fields", "make_variant_dim", "make_struct_single",
  "make_struct_dim_list", "make_struct_dim_decl",
  "optional_make_struct_dim_decl", "use_initializer", "make_struct_decl",
  "$@72", "$@73", "$@74", "$@75", "$@76", "$@77", "$@78", "$@79",
  "make_map_tuple", "make_tuple_call", "$@80", "$@81", "make_dim_decl",
  "$@82", "$@83", "$@84", "$@85", "$@86", "$@87", "$@88", "$@89", "$@90",
  "$@91", "expr_map_tuple_list", "make_table_decl",
  "array_comprehension_where", "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1253)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-687)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1253,    70, -1253, -1253,    25,   -79,   183,   522, -1253,   -82,
     522,   522,   522, -1253,    78,    22, -1253, -1253,   -89,     9,
   -1253, -1253,   408, -1253,    31, -1253, -1253, -1253, -1253, -1253,
   -1253, -1253, -1253, -1253, -1253,    36, -1253,    26,    48,   100,
   -1253, -1253,   183,    12, -1253, -1253, -1253,   123,   138, -1253,
   -1253,    31,   204,   245,   277,   278,   250, -1253, -1253, -1253,
      22,    22,    22,   184, -1253,   569,  -126, -1253, -1253, -1253,
   -1253, -1253,   538,   550,   584, -1253,   585,    20,    25,   326,
     -79,   293,   363, -1253,   368,   399, -1253, -1253, -1253,   589,
   -1253, -1253, -1253, -1253,   428,   429, -1253, -1253,   -66,    25,
      22,    22,    22,    22, -1253, -1253, -1253, -1253, -1253, -1253,
   -1253,   440, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253,
   -1253, -1253, -1253,     0,   393, -1253, -1253, -1253, -1253,   546,
   -1253, -1253,   442, -1253, -1253, -1253,   454,   475,   496, -1253,
   -1253,   479, -1253,   -29, -1253,   473,   514,   569, 10858, -1253,
     484,   588,   482, -1253, -1253,   551, -1253, -1253, -1253, -1253,
   -1253, -1253, -1253,   -21, -1253,  1582, -1253, -1253, -1253, -1253,
   -1253,  9450, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253,
   -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253,
   -1253, -1253, -1253, -1253, -1253, -1253, -1253,   656,   660, -1253,
     493,   530,   348,   532, -1253,   549, -1253,    25,   504,   552,
   -1253, -1253, -1253,   393, -1253,   533,   535,   539,   516,   547,
     548, -1253, -1253, -1253,   518, -1253, -1253, -1253, -1253, -1253,
     553, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253,
   -1253, -1253,   554, -1253, -1253, -1253,   555,   556, -1253, -1253,
   -1253, -1253,   559,   560,   529,    78, -1253, -1253, -1253, -1253,
   -1253, -1253,   689,   564,   574, -1253, -1253,   575,   586, -1253,
   -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253,
   -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253,   592,
     543, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253,
   -1253, -1253, -1253,   729, -1253, -1253, -1253, -1253, -1253, -1253,
   -1253, -1253, -1253, -1253, -1253, -1253,   593,   557, -1253, -1253,
     -99,   581, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253,
   -1253, -1253, -1253, -1253,   590,   595, -1253,    25, -1253,   172,
   -1253, -1253, -1253, -1253, -1253,  6168, -1253, -1253,   604, -1253,
     282,   296,   297, -1253, -1253,  6168,    89, -1253, -1253, -1253,
      27, -1253, -1253, -1253,    73,  3256, -1253,   566,  1298, -1253,
     594,  1399,   294, -1253, -1253, -1253, -1253,   608,   635, -1253,
     570, -1253,    98, -1253,  -109,  1582, -1253,  1872,   615,    78,
   -1253, -1253, -1253, -1253,   617,  1582, -1253,   -10,  1582,  1582,
    1582,   599,   600, -1253, -1253,    90,    78,   601,    34, -1253,
     110,   582,   603,   611,   596,   625,   606,   111,   633, -1253,
     146,   636,   637,  6168,  6168,   619,   622,   623,   624,   626,
     627, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253,
    3452,  6168,  6168,  6168,  6168,  6168,  3062,  6168, -1253,   612,
   -1253, -1253, -1253,   629, -1253, -1253, -1253, -1253,   638, -1253,
   -1253, -1253, -1253, -1253, -1253,   -52,   897, -1253,   639, -1253,
   -1253, -1253, -1253, -1253, -1253,  1582,  1582,   583,   668,  1582,
     493,  1582,   493,  1582,   493,  6511,   678,  6505, -1253,  6168,
   -1253, -1253, -1253, -1253,   641, -1253, -1253,  8962,  3646, -1253,
   -1253,   680, -1253,    32, -1253, -1253,   483, -1253,   564,   672,
     665,   483, -1253,   676, -1253, -1253,  6168, -1253, -1253,   -68,
    -110, -1253,   564, -1253,   643, -1253, -1253,   644,  3840, -1253,
     530,  4034,   645,   691, -1253,   679,   699,  4228,   -37,  4422,
     818, -1253,   684,   685,   662,   858, -1253, -1253, -1253, -1253,
   -1253,   700, -1253,   702,   703,   707,   712,   713, -1253,   805,
   -1253,   715,  9334,   701, -1253,   710, -1253,    18, -1253,   180,
   -1253, -1253, -1253,  6168,   306,   359,   704,   380, -1253, -1253,
   -1253,   688,   690, -1253,   188, -1253,   709,   711,   714, -1253,
    6168,  1582,  6168,  6168, -1253, -1253,  6168, -1253,  6168, -1253,
    6168, -1253, -1253,  6168, -1253,  1582,   141,   141,  6168,  6168,
    6168,  6168,  6168,  6168,   540,   -68,  9481, -1253,   719,   141,
     141,   -57,   141,   141,   -68,   876,   724, 10138,   724,   115,
    2670,   899, -1253,   696,   638, -1253, 10620, 10651,  6168,  6168,
   -1253, -1253,  6168,  6168,  6168,  6168,   743,  6168,   317,  6168,
    6168,  6168,  6168,  6168,  6168,  6168,  6168,  6168,  4616,  6168,
    6168,  6168,  6168,  6168,  6168,  6168,  6168,  6168,  6168, 10770,
    6168, -1253,  4810,   394,   406, -1253, -1253,   -20,   468,   581,
     492,   581,   497,   581, -1253,   308, -1253,   345, -1253,  1582,
     706,   724, -1253, -1253, -1253,  8993, -1253,   735,  1582, -1253,
   -1253,  1582, -1253, -1253,  6541,   717,   864, -1253,   -67, -1253,
    6168,   -68,  6168, 10138,   894,  6168, 10138,  6168,   734, -1253,
     733,   760, 10138, -1253,  6168, 10138,   746, -1253, -1253,  6168,
     720, -1253, -1253, -1253, -1253, -1253, -1253, -1253,   -63, -1253,
    6168,  6168,  6168,  6168,  6168,  6168,  6168,  6168,  6168,  6168,
    6168,  6168,  6168,  6168,  6168,  6168,  6168,  6168,  6168,   615,
   -1253, -1253,   909,   482, -1253,  6168,  9580, -1253, -1253, -1253,
    1582,  1582,  1582,  1582,   851,  6168,   762,  6168,  1582, -1253,
   -1253, -1253,  1582,   724,   346,   719,  6633,  1582,  1582,  6732,
    1582,  6763,  1582,   724,  1582,  1582,   724,  1582,   740,  6862,
    6954,  6987,  7079,  7178,  7209, -1253,  6168,    99,    10,  6168,
    6168,   756,    13,   -68,  6168,   725,   726,   728,   730,   335,
   -1253, -1253,   -72,   731,   118,  2866, -1253,   103,   752,   736,
     732,   493,  2077, -1253,   899,   749,   738, -1253, -1253,   754,
     739, -1253, -1253,   469,   469,   174,   174, 10487, 10487,   741,
     708,   742, -1253,  9083,   -75,   -75,   639,   469,   469,   381,
   10341,  1037, 10227, 10739,  9669,   675, 10373, 10455,   174,   174,
     209,   209,   708,   708,   708,   360,  6168,   744,   745,   366,
    6168,   930,   747,  9114, -1253,   105, -1253, -1253,   778, -1253,
   -1253,   755, -1253,   759, -1253,   765, -1253,  6511, -1253,   678,
     357,   564, -1253, -1253, -1253, -1253,   564,   564, -1253,  6168,
     786, -1253,   791, -1253,  1582, -1253,  6168,  7308,    21, 10138,
     530, 10138,  7400,  6168, -1253, -1253, 10138, -1253,  7433,  6168,
     750,   911,   794, -1253,   453, -1253, 10138, 10138, 10138, 10138,
   10138, 10138, 10138, 10138, 10138, 10138, 10138, 10138, 10138, 10138,
   10138, 10138, 10138, 10138, 10138, -1253,   787,   605,   890,   792,
    9701, -1253, -1253, -1253, -1253,   564,   779,   780,   499, -1253,
      14,   763,   403,  7525,   500,  1582,  1582,  1582,   782,   764,
     766,  1582,   767, -1253,   788,   789, -1253,   796, -1253,   797,
     781,   798,   799,   783,   800,   899, -1253, -1253, -1253, -1253,
   -1253,   785,  9783,  6168, 10138, -1253, -1253,  6168,    29, 10138,
   -1253, -1253,  6168,  6168,  1582,   493,    86, -1253,   790,  6168,
    6168,  6168,     1,  6362, -1253,   389, -1253,   -71,   581, -1253,
     493, -1253,  6168, -1253,  6168,  5004,  6168, -1253,   803,   793,
   -1253, -1253,  6168,   801, -1253,  9204,  6168,  5198,   802, -1253,
    9235, -1253,  5392, -1253,  6168, -1253, -1253, -1253, -1253, -1253,
   -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253,
     804,  1582,  7624, -1253,   929,   -44, 10138,   530,  6168, -1253,
     530, 10138,  2282,   530,  7655,  6168,   829, -1253,   164,   831,
    1582,   -10, -1253, -1253, -1253,   343, -1253,    -3, -1253, -1253,
   -1253, -1253, -1253, -1253,   795, -1253,   806,   833, -1253, -1253,
     811,   813,   816, -1253, -1253,  6168,   817, -1253, -1253, -1253,
   -1253, -1253, -1253, -1253, -1253, -1253, -1253,   -27,  5586, -1253,
     491,   495,  6168,  7754,  7846,   822,   581, -1253,  7879, 10138,
   10138,   812,  2866,   824,   119,   842,   848,   827,   855,   861,
   -1253,   177,   -69,   581,  1582,  7971,  1582,  8070, -1253,   189,
    8101, -1253,  6168, 10259,  6168, -1253,  8200, -1253,   226,  6168,
   -1253, -1253, -1253,   227, -1253, -1253, -1253,  6168,   564, -1253,
     871,  6168, -1253,   246, -1253, -1253,   461,  1023,  8292, -1253,
     877,   365,   997,   215,  6168,  1008,    -3, -1253, -1253,   605,
     838,   839, -1253, -1253,  6168,   859, -1253, -1253, -1253, -1253,
     841,   843,   719,   845,  6168,  6168,  6168,   846,   851,   849,
     850,  5780, -1253, -1253,   248,  6168,  6168,   498, -1253, -1253,
   -1253,   862,   154, -1253,   168,  6168,  6168,  6168, -1253, -1253,
     885, -1253, -1253,   -71, -1253,  5974, -1253, -1253,   530,   867,
   -1253,   506, -1253, -1253, -1253,  1582,  8325,  8417, -1253, -1253,
    8516, -1253,   854, -1253, 10138,   530,   530, -1253, -1253,   856,
   -1253,  2476,   884, -1253, -1253,  1582,   -10,   900, -1253,  6168,
    9815, -1253, -1253,  1008,   -68,   851,   851,  8547,   860,   863,
     865,   866,  6168, -1253, -1253,  6168,   174,   174,   174,  6168,
   -1253,   851,   420, -1253,  8646, -1253,   891,  9897,  6168,   268,
   -1253,  6168,  6168,   868,  8738, 10138, 10138,   872, -1253,  6168,
   10227, -1253, -1253, -1253,   507, -1253, -1253, -1253, -1253, -1253,
   -1253,  6168, -1253, -1253, -1253, -1253, -1253, 10138, -1253,   -10,
    6168, -1253,  9931, -1253, 10858, -1253, -1253,   196,   196, -1253,
    6168,   851,   851,   420,   883,   886,   724,   196,   752,   888,
   -1253,  1013,   898,   887,  9897, -1253,   268, 10138, 10138, -1253,
     203, -1253, 10259, -1253, -1253, -1253,  8771,  6168, 10013, -1253,
     918, 10858,   420,   752,   917, -1253,   892,   895,  8863,   196,
     196,   896, -1253, -1253,   901,   902, -1253,  6168, -1253, -1253,
     893, -1253,  6168,  6168, -1253,   530, 10102, -1253, -1253,   530,
     253,   905, -1253, -1253, -1253, -1253,   906,   908, -1253, -1253,
   -1253, 10138, -1253, 10138, 10138,   461, -1253, -1253, -1253,   420,
   -1253, -1253, -1253,   258, -1253
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   113,     1,   266,     0,     0,     0,   540,   267,     0,
     540,   540,   540,    16,     0,     0,    15,     3,     0,     0,
       9,     8,     0,     7,   528,     6,    11,     5,     4,    13,
      12,    14,    85,    86,    84,    93,    95,    37,    50,    47,
      48,    39,     0,    45,    38,   542,   541,     0,     0,    22,
      21,   528,     0,     0,     0,     0,   242,    35,   100,   101,
       0,     0,     0,   102,   104,   111,     0,    99,    17,    10,
     561,   560,   206,   546,   562,   529,   530,     0,     0,     0,
       0,    40,     0,    46,     0,     0,    43,   543,   545,    18,
     706,   698,   702,   244,     0,     0,   110,   105,     0,     0,
       0,     0,     0,     0,   114,   208,   207,   210,   205,   548,
     547,     0,   564,   563,   567,   532,   531,   533,    91,    92,
      89,    90,    88,     0,     0,    87,    96,    51,    49,    45,
      42,    41,     0,    19,    20,    23,     0,     0,     0,   243,
      33,    36,   109,     0,   106,   107,   108,   112,     0,   549,
     550,   557,   466,    24,    25,     0,    80,    81,    78,    79,
      77,    76,    82,     0,    44,     0,   707,   699,   703,    34,
     103,     0,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,     0,     0,   120,
     115,     0,     0,     0,   558,     0,   568,     0,   467,     0,
      26,    27,    28,     0,    94,     0,     0,     0,     0,     0,
       0,   575,   595,   576,   611,   577,   581,   582,   583,   584,
     601,   588,   589,   590,   591,   592,   593,   594,   596,   597,
     598,   599,   668,   580,   587,   600,   675,   682,   578,   585,
     579,   586,     0,     0,     0,     0,   610,   632,   635,   633,
     634,   695,   630,   544,   616,   494,   500,   174,   175,   172,
     123,   124,   126,   125,   127,   128,   129,   130,   156,   157,
     154,   155,   147,   158,   159,   148,   145,   146,   173,   167,
       0,   171,   160,   161,   162,   163,   134,   135,   136,   131,
     132,   133,   144,     0,   150,   151,   149,   142,   143,   138,
     137,   139,   140,   141,   122,   121,   166,     0,   152,   153,
     466,   118,   233,   211,   602,   605,   608,   609,   603,   606,
     604,   607,   551,   552,   555,   565,    97,     0,   520,   513,
     534,    83,   636,   659,   662,     0,   665,   655,     0,   619,
     669,   676,   683,   689,   692,     0,     0,   645,   650,   644,
       0,   658,   654,   647,     0,     0,   649,   631,     0,   617,
     777,   700,   704,   176,   177,   170,   165,   178,   168,   164,
       0,   116,   265,   488,     0,     0,   209,     0,   537,     0,
     559,   479,   569,    98,     0,     0,   514,     0,     0,     0,
       0,     0,     0,   372,   373,     0,     0,     0,     0,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   601,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   455,   311,   313,   312,   314,   315,   316,   317,    29,
       0,     0,     0,     0,     0,     0,     0,     0,   296,   297,
     370,   369,   448,   367,   441,   440,   439,   438,   113,   444,
     368,   443,   442,   413,   374,   414,     0,   375,     0,   371,
     710,   714,   711,   712,   713,     0,     0,     0,     0,     0,
     115,     0,   115,     0,   115,     0,     0,     0,   641,   236,
     652,   653,   646,   648,     0,   651,   627,     0,     0,   697,
     696,   778,   708,   242,   623,   622,     0,   495,   490,     0,
       0,     0,   501,     0,   179,   169,     0,   263,   264,     0,
     466,   117,   119,   235,     0,    60,    61,     0,   257,   255,
       0,     0,     0,     0,   256,     0,     0,     0,     0,     0,
     212,   215,     0,     0,     0,     0,   228,   223,   220,   219,
     221,     0,   234,     0,    67,    68,    65,    66,   229,   269,
     218,     0,    64,   535,   538,   777,   556,   480,   521,     0,
     511,   512,   510,     0,     0,     0,     0,   624,   733,   736,
     247,     0,   250,   254,     0,   285,     0,     0,     0,   762,
       0,     0,     0,     0,   276,   279,     0,   282,     0,   766,
       0,   742,   748,     0,   739,     0,   402,   403,     0,     0,
       0,     0,     0,     0,     0,     0,   746,   769,   777,   379,
     378,   415,   377,   376,     0,     0,   777,   291,   777,   298,
       0,   306,   233,   297,   113,   214,     0,     0,     0,     0,
     404,   405,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
       0,   639,     0,     0,     0,   612,   614,     0,     0,   118,
       0,   118,     0,   118,   492,     0,   498,     0,   613,     0,
       0,   777,   643,   626,   629,     0,   618,     0,     0,   496,
     701,     0,   502,   705,     0,     0,   570,   486,   505,   489,
       0,     0,     0,   258,     0,     0,   245,     0,     0,   232,
       0,     0,    54,    72,     0,   260,     0,   230,   231,     0,
       0,   222,   217,   224,   225,   226,   227,   268,     0,   216,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   778,
     553,   566,     0,   466,   525,     0,     0,   637,   660,   663,
       0,     0,     0,     0,   731,   236,     0,     0,     0,   752,
     755,   758,     0,   777,     0,   777,     0,     0,     0,     0,
       0,     0,     0,   777,     0,     0,   777,     0,     0,     0,
       0,     0,     0,     0,     0,    32,     0,    30,     0,     0,
     778,     0,     0,     0,   778,     0,     0,     0,     0,   344,
     341,   343,     0,     0,   242,     0,   359,     0,   724,     0,
       0,   115,     0,   298,   306,     0,     0,   427,   426,     0,
       0,   428,   432,   380,   381,   393,   394,   391,   392,     0,
     421,     0,   411,     0,   445,   446,   447,   382,   383,   398,
     399,   400,   401,     0,     0,   396,   397,   395,   389,   390,
     385,   384,   386,   387,   388,     0,     0,     0,   350,     0,
       0,     0,     0,     0,   364,     0,   666,   656,     0,   620,
     670,     0,   677,     0,   684,     0,   690,     0,   693,     0,
       0,   240,   640,   237,   628,   709,   491,   497,   487,     0,
       0,   504,     0,   503,     0,   506,     0,     0,     0,   259,
       0,   246,     0,     0,    52,    53,   261,   233,     0,     0,
       0,   515,     0,   275,   513,   274,   328,   329,   331,   330,
     332,   322,   323,   324,   333,   334,   320,   321,   335,   336,
     325,   326,   327,   319,   536,   539,     0,   473,   476,     0,
       0,   527,   638,   661,   664,   625,     0,     0,     0,   732,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   416,     0,     0,   417,     0,   449,     0,
       0,     0,     0,     0,     0,   306,   450,   451,   452,   453,
     454,     0,     0,     0,   745,   770,   771,     0,     0,   292,
     751,   406,     0,     0,     0,   115,     0,   360,     0,     0,
       0,     0,     0,     0,   363,     0,   361,     0,   118,   310,
     115,   423,     0,   429,     0,     0,     0,   409,     0,     0,
     433,   437,     0,     0,   412,     0,     0,     0,     0,   351,
       0,   357,     0,   407,     0,   365,   667,   657,   615,   621,
     671,   673,   678,   680,   685,   687,   691,   493,   694,   499,
       0,     0,     0,   572,   573,   507,   509,     0,     0,   262,
       0,    75,     0,     0,     0,     0,     0,   270,     0,     0,
       0,     0,   554,   474,   475,   476,   477,   468,   481,   526,
     734,   737,   248,   252,     0,   251,     0,     0,   288,   286,
       0,     0,     0,   763,   761,   238,     0,   772,   277,   280,
     283,   767,   765,   743,   749,   747,   740,     0,     0,    31,
       0,     0,     0,     0,     0,     0,   118,   362,     0,   716,
     715,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     304,     0,     0,   118,     0,     0,     0,     0,   339,     0,
       0,   434,     0,   422,     0,   410,     0,   352,     0,     0,
     408,   358,   354,     0,   674,   681,   688,   236,   241,   571,
       0,     0,    73,     0,    74,   213,    57,    62,     0,   517,
       0,   513,   518,     0,     0,   471,   468,   469,   470,   473,
       0,     0,   249,   253,     0,     0,   287,   753,   756,   759,
       0,     0,   777,     0,     0,     0,     0,     0,   731,     0,
       0,     0,   420,   456,     0,     0,     0,     0,   342,   465,
     345,     0,     0,   337,     0,     0,     0,     0,   301,   302,
       0,   300,   299,     0,   307,     0,   293,   308,     0,     0,
     464,     0,   462,   340,   459,     0,     0,     0,   458,   353,
       0,   355,     0,   574,   508,     0,     0,    55,    56,     0,
      69,     0,     0,   516,   271,     0,     0,     0,   522,     0,
       0,   472,   482,   471,     0,   731,   731,     0,     0,     0,
       0,     0,   236,   773,   239,   238,   278,   281,   284,     0,
     744,   731,     0,   418,     0,   457,   775,   775,     0,     0,
     348,     0,     0,     0,     0,   718,   717,     0,   305,     0,
     294,   309,   424,   430,     0,   463,   461,   460,   642,    71,
      58,     0,    63,    67,    68,    65,    66,    64,    70,     0,
       0,   519,     0,   524,     0,   484,   478,   730,   730,   289,
       0,   731,   731,     0,     0,     0,   777,   730,   723,     0,
     419,     0,     0,     0,   775,   346,     0,   720,   719,   338,
       0,   303,   295,   425,   431,   435,     0,     0,     0,   523,
       0,     0,     0,   727,   777,   729,     0,     0,     0,   730,
     730,     0,   764,   774,     0,     0,   741,     0,   781,   779,
       0,   349,     0,     0,   436,     0,     0,   273,   483,     0,
       0,   778,   728,   735,   738,   290,     0,     0,   760,   768,
     750,   776,   780,   722,   721,    57,   272,   485,   725,     0,
     754,   757,    59,     0,   726
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1253, -1253, -1253, -1253, -1253, -1253,   458,  1019, -1253, -1253,
   -1253,  1104, -1253, -1253, -1253,  1064, -1253,   982, -1253, -1253,
    1035, -1253, -1253, -1253,  -309, -1253, -1253,  -154, -1253, -1253,
   -1253, -1253, -1253, -1253,   907, -1253, -1253,   -53,  1020, -1253,
   -1253, -1253,   318, -1253,  -426,  -471,  -660, -1253, -1253, -1253,
   -1252, -1253, -1253,  -523, -1253, -1253,  -605,  -755,  -177, -1253,
     -14, -1253, -1253, -1253, -1253, -1253,  -150,  -149,  -148,  -147,
   -1253, -1253,  1124, -1253, -1253, -1253, -1253, -1253, -1253, -1253,
   -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253,  -429, -1253,
     661,  -116, -1253,  -805, -1253, -1253, -1253, -1253, -1253, -1253,
   -1247, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253,
     536, -1253, -1253, -1253, -1253, -1253, -1253, -1253,  -146,   -65,
    -153,   -64,    38, -1253, -1253, -1253, -1253, -1253,   616, -1253,
    -469, -1253, -1253,  -462, -1253, -1253,  -706,  -145,  -557,  -912,
   -1253, -1253, -1253, -1253,  1086, -1253, -1253, -1253,   379, -1253,
     716, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253,
   -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253,  -587,
    -162, -1253,   753, -1253, -1253, -1253, -1253, -1253, -1253,  -367,
   -1253, -1253,  -337, -1253, -1253, -1253, -1253, -1253, -1253, -1253,
   -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253,
   -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253,
   -1253,  -151, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253,
   -1253, -1253, -1253, -1253,   758,  -622,  -212,  -784, -1253, -1253,
   -1102, -1154, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253,
   -1253,  -800, -1253, -1253, -1253, -1253, -1253, -1253, -1253, -1253,
   -1253, -1253, -1253, -1253, -1253, -1253,  -398, -1253, -1241,  -561,
   -1253
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    16,   135,    51,    17,   155,   161,   614,   450,
     141,   451,    95,    19,    20,    43,    44,    86,    21,    39,
      40,   542,   543,  1269,  1270,   544,  1272,   545,   546,   547,
     548,   549,   550,   551,   162,   163,    35,    36,    37,   208,
      63,    64,    65,    66,    22,   321,   386,   200,    23,   107,
     201,   108,   148,   323,   452,   552,   387,   690,  1211,   900,
     453,   553,   581,   773,  1202,   454,   554,   555,   556,   557,
     558,   519,   559,   738,  1088,   933,   560,   455,   787,  1214,
     788,  1215,   790,  1216,   456,   778,  1206,   457,   691,  1247,
     458,  1150,  1151,   831,   459,   635,   460,   561,   461,   462,
     821,   463,  1014,  1309,  1015,  1366,   464,   881,  1171,   465,
     627,  1154,  1373,  1156,  1374,  1255,  1404,   467,   382,  1199,
    1282,  1095,  1097,   959,   567,   763,  1344,  1381,   383,   384,
     506,   685,   371,   511,   687,   372,  1021,   707,   573,   397,
     934,   339,   935,   340,    76,   117,    25,   152,   564,   565,
      47,    48,   132,    26,   111,   150,   203,    27,   388,   956,
     390,   205,   206,    74,   114,   392,    28,   151,   335,   708,
     468,   332,   258,   259,   677,   370,   260,   478,  1059,   507,
     576,   367,   261,   262,   398,   962,   689,   476,  1057,   399,
     963,   400,   964,   475,  1056,   479,  1060,   480,  1174,   481,
    1062,   482,  1175,   483,  1064,   484,  1176,   485,  1066,   486,
    1068,   508,    29,   137,   265,   509,    30,   138,   266,   513,
      31,   136,   264,   697,   469,  1383,  1359,   829,  1384,  1385,
    1386,   970,   470,   771,  1200,   772,  1201,   797,  1220,   794,
    1218,   617,   471,   795,  1219,   472,   975,  1289,   976,  1290,
     977,  1291,   782,  1210,   792,  1217,  1212,   473,  1362,   502,
     474
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      57,    67,   916,   257,   760,   512,   209,   714,   828,   679,
    1005,   681,   765,   683,   263,   723,   684,   626,   628,   891,
     971,   893,  1091,   895,   686,   126,  -113,   832,   808,  1030,
    1003,   500,   631,  1007,    82,   118,   119,   812,  1018,   586,
     380,  1078,   618,  1197,    32,    33,    67,    67,    67,  1132,
     646,   380,   103,   648,   649,  1145,    58,   811,   569,   911,
     490,  1146,  1365,   520,  1300,   815,  1363,   816,  1245,    83,
       2,   648,   649,   100,   101,   102,    38,     3,   104,   705,
      49,   823,   911,    68,   930,    59,    67,    67,    67,    67,
    1016,  1147,  1380,    75,   706,   401,   402,   521,   207,   931,
       4,   322,     5,  1198,     6,   517,   492,   381,  1148,   207,
       7,   912,   913,  1149,   914,   408,   570,   915,   632,  1401,
       8,   410,   571,  1400,   918,    50,     9,   669,   670,  1409,
     903,  1347,  1348,   322,  1017,   913,   587,   588,   368,   699,
     142,   762,   932,   322,   702,   669,   670,  1357,   518,    78,
      10,   256,   888,    60,   336,   448,   633,   213,   417,   418,
      55,   783,   636,   637,   153,   154,   889,   823,   598,   572,
      94,   793,    11,    12,   796,   120,  1016,   170,  1221,  1025,
     121,    69,   122,   123,    56,   214,  1104,    34,   912,   761,
    1127,   912,   420,   421,   785,   636,   637,  1389,  1390,   912,
     828,   827,    84,  1022,    78,  1141,   257,   912,   834,   257,
     770,   491,   911,   698,    85,    77,    55,    61,  1020,   589,
    1103,    15,   979,   257,   982,   124,  1008,    62,    55,    79,
     636,   637,   990,   257,   522,   993,   257,   257,   257,   590,
      56,   356,    13,   885,   911,   911,  1387,   574,   575,   577,
    1020,  1236,    56,    55,   784,  1395,    94,   493,   640,   641,
     440,    14,   817,   210,   211,   913,   646,   818,   798,   648,
     649,   650,   651,    15,   488,   580,   494,    56,    80,  1276,
     911,  1023,   495,  1054,   393,    87,  1311,  1416,  1417,   638,
     639,   640,   641,   446,   489,   591,   599,   913,   913,   646,
     819,   647,   648,   649,   650,   651,   570,   652,   653,  1024,
      88,  1055,   571,   257,   257,   592,   600,   257,   897,   257,
     899,   257,  1082,   257,   673,   674,   640,   641,   678,   911,
     680,   602,   682,   913,   646,  1402,   647,   648,   649,   650,
     651,   570,  1190,   669,   670,    41,  1025,   571,   828,   823,
     394,   603,   764,   395,   256,  1243,   396,   256,  1016,   572,
    1028,   664,   665,   666,   667,   668,    90,  1054,  1152,  1181,
    1191,   256,  1313,   776,    42,   566,   669,   670,    96,    97,
      98,   256,   913,  1244,   256,   256,   256,  1278,    94,    99,
    1195,   256,   584,   777,   572,  1253,   626,  1079,   666,   667,
     668,  1382,   636,   637,  1054,  1054,    55,    91,   156,   157,
    1096,   669,   670,    70,    71,   817,    72,   981,   144,   145,
     146,   147,  1262,   324,  1054,  1306,  1054,   325,  1067,   257,
      56,  1025,  1259,  1261,   966,   967,  1025,  1069,  1237,    92,
      93,   326,   327,   257,    73,   978,   328,   329,   330,   331,
     984,   985,  1265,   987,  1305,   989,   510,   991,   992,  1428,
     994,   256,   256,  -672,  1434,   256,   504,   256,  -672,   256,
     851,   256,   505,  1266,   838,   842,  1231,  -679,  -686,   852,
     504,   127,  -679,  -686,  1267,  1268,   505,  -672,   368,   856,
     636,   637,   767,  1248,   896,    85,   638,   639,   640,   641,
     642,  -679,  -686,   643,   644,   645,   646,   882,   647,   648,
     649,   650,   651,  1043,   652,   653,  -347,   504,   504,  1048,
    1234,  -347,  1044,   505,   505,   129,  1312,   257,  1049,   504,
     130,   898,   980,  1071,  1194,   505,   257,  1354,   901,   257,
    -347,   368,  1143,  1070,  1136,   768,  1275,   906,   158,   396,
     907,  1144,   504,   159,  1182,   160,   123,  1184,   505,  1153,
    1186,   131,   368,   659,   660,   661,   662,   663,   664,   665,
     666,   667,   668,   823,  1130,   504,   368,   256,  1131,    45,
     886,   505,  1016,   669,   670,    46,   640,   641,   368,  1106,
     139,   256,   887,   140,   646,   105,   647,   648,   649,   650,
     651,   106,   149,    83,  1222,  1107,  1159,   109,   257,   257,
     257,   257,   100,   110,   102,   820,   257,   958,  1168,   965,
     257,   165,   968,  1173,   166,   257,   257,   974,   257,  1246,
     257,  1089,   257,   257,  1090,   257,  1279,   396,  1110,  1111,
    1112,   112,   115,   169,  1116,   167,   133,   113,   116,  1183,
     368,  1294,   134,   100,   890,   504,   664,   665,   666,   667,
     668,   505,  1093,  1225,  1403,   202,   168,  1226,  1094,  1054,
    1308,   669,   670,  1054,   368,   256,  1054,  1135,   892,   368,
    1358,   368,   368,   894,   256,  1102,  1109,   256,   368,   368,
     207,   318,  1323,  1375,   204,   319,   636,   637,   320,  1224,
     322,  1041,   333,  1227,   153,   154,   805,   806,   100,   101,
     102,   334,   337,   626,   338,   210,   211,   212,   342,  1340,
     343,   345,   357,   348,   344,  1321,    52,    53,    54,   636,
     637,  1358,   346,   347,   355,   257,   369,   373,   349,   350,
     351,   352,  1329,  1330,   353,   354,   368,   376,   374,   358,
     359,   377,   257,  1193,   375,   378,   256,   256,   256,   256,
    1410,   379,   385,  1075,   256,   391,   477,   515,   256,   498,
     514,   389,   501,   256,   256,   516,   256,   563,   256,   568,
     256,   256,  1377,   256,   578,   579,   585,   593,   594,   675,
     638,   639,   640,   641,   642,  1394,   595,   643,   644,   645,
     646,   596,   647,   648,   649,   650,   651,  1433,   652,   653,
     597,   598,   360,   257,   257,   257,   361,  1249,   601,   257,
     629,   604,   605,  1412,   608,   640,   641,   609,   610,   611,
     676,   612,   613,   646,   630,   647,   648,   649,   650,   651,
     510,    15,   696,   700,   672,   692,   701,   703,   710,   711,
     717,   719,   257,   718,   720,   726,   727,   728,   660,   661,
     662,   663,   664,   665,   666,   667,   668,   729,   730,   362,
    1356,   737,   731,   363,   732,   733,   364,   669,   670,   734,
     758,   466,  1425,   256,   735,   736,  1427,   739,   759,   813,
     769,   487,   365,   774,   779,   775,   780,   810,   366,   781,
     256,   497,   814,   830,   833,   849,   905,   910,   920,   257,
     669,   670,   902,   923,   924,   925,   927,   957,   636,   637,
    1178,   969,   909,   562,   972,   929,   995,  1006,   257,  1010,
    1025,  1051,  1011,  1012,  1031,  1013,  1019,  1027,  1339,  1033,
    1058,  1061,  1026,  1032,  1034,  1063,  1035,  1036,  1073,  1046,
    1047,  1065,  1052,  1074,  1086,  1085,  1087,  1096,  1092,   606,
     607,   256,   256,   256,  1098,  1100,  1101,   256,  1113,  1105,
    1114,  1115,  1180,  1117,  1118,  1119,   616,   619,   620,   621,
     622,   623,  1120,  1121,  1123,  1124,  1126,  1122,  1161,  1125,
    1128,  1189,   257,  1192,   257,  1205,  1137,  1207,  1162,  1208,
     256,  1203,  1209,  1213,  1238,  1251,  1164,  1169,  1230,  1177,
    1239,  1204,   638,   639,   640,   641,   642,  1241,  1233,   643,
     644,   645,   646,  1242,   647,   648,   649,   650,   651,  1235,
     652,   653,  1240,  1263,   695,  1271,   654,   655,   656,  1274,
    1277,  1281,   657,  1285,  1286,  1288,  1292,  1317,  1310,  1293,
    1295,  1299,   704,  1322,  1301,  1302,  1338,   256,   636,   637,
    1328,  1331,  1341,  1361,   713,  1350,  1397,   716,  1351,  1398,
    1352,  1353,   807,   722,  1369,   725,   256,   658,  1371,   659,
     660,   661,   662,   663,   664,   665,   666,   667,   668,  1392,
    1408,  1399,  1393,   257,  1396,  1411,   125,  1422,  1413,   669,
     670,  1414,  1418,   671,  1324,    18,    81,  1419,  1420,   766,
    1429,   164,  1430,   257,  1431,   128,  1432,  1332,  1355,   143,
     341,  1333,  1334,  1335,  1336,    24,   634,  1318,   616,   786,
    1345,  1283,   789,  1196,   791,  1284,   709,    89,   955,  1346,
     256,  1391,   256,     0,   799,   800,   801,   802,   803,   804,
       0,     0,   638,   639,   640,   641,   642,     0,   582,   643,
     644,   645,   646,   583,   647,   648,   649,   650,   651,     0,
     652,   653,     0,     0,   843,   844,   654,     0,   845,   846,
     847,   848,     0,   850,     0,   853,   854,   855,   857,   858,
     859,   860,   861,   862,   864,   865,   866,   867,   868,   869,
     870,   871,   872,   873,   874,     0,   883,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   659,
     660,   661,   662,   663,   664,   665,   666,   667,   668,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   669,
     670,   256,     0,     0,     0,     0,   917,     0,   919,     0,
       0,   921,     0,   922,     0,     0,     0,     0,     0,     0,
     926,   256,     0,     0,     0,   928,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   936,   937,   938,   939,
     940,   941,   942,   943,   944,   945,   946,   947,   948,   949,
     950,   951,   952,   953,   954,   820,     0,     0,     0,     0,
       0,   960,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   973,     0,     0,     0,   215,     0,     0,
       0,     0,     0,   216,     0,     0,     0,     0,     0,   217,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   218,
       0,     0,  1002,     0,     0,  1004,   616,   219,     0,     0,
    1009,     0,   820,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   220,     0,     0,     0,     0,     0,   562,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1045,     0,     0,     0,  1050,     0,   215,     0,
       0,     0,     0,     0,   216,     0,     0,     0,     0,     0,
     217,     0,     0,     0,     0,     0,    55,     0,     0,     0,
     218,     0,     0,     0,     0,  1072,     0,     0,   219,   254,
       0,     0,  1076,     0,     0,     0,     0,     0,     0,  1081,
      56,     0,     0,   220,     0,  1084,     0,     0,     0,     0,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,     0,   255,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1133,  1134,
     254,     0,     0,     0,     0,  1138,  1139,  1140,     0,  1009,
       0,   503,     0,     0,     0,     0,     0,     0,  1155,     0,
    1157,   504,  1160,     0,     0,     0,     0,   505,  1163,     0,
       0,     0,  1166,     0,     0,     0,     0,     0,     0,     0,
    1009,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   215,     0,     0,     0,     0,   255,   216,     0,     0,
       0,     0,     0,   217,     0,     0,     0,     0,   562,     0,
       0,  1188,     0,   218,     0,     0,     0,     0,     0,     0,
       0,   219,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   220,     0,     0,     0,
       0,   616,     0,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1256,     0,
    1257,     0,     0,     0,     0,  1260,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1264,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1280,     0,     0,   254,     0,     0,     0,     0,     0,     0,
    1287,     0,     0,     0,    56,     0,     0,     0,     0,     0,
    1296,  1297,  1298,     0,     0,     0,     0,  1304,     0,     0,
       0,   616,  1307,     0,     0,     0,     0,     0,     0,     0,
       0,  1314,  1315,  1316,     0,     0,     0,     0,     0,     0,
       0,  1320,     0,     0,     0,     0,     0,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1337,     0,     0,
       0,     0,     0,     0,     0,  1342,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   616,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1364,     0,     0,  1367,  1368,     0,
       0,     0,     0,     0,     0,  1372,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1376,     0,     0,
       0,     0,     0,   523,     0,     0,  1378,   401,   402,     3,
       0,   524,   525,   526,     0,   527,  1388,   403,   404,   405,
     406,   407,     0,     0,     0,     0,     0,   408,   528,   409,
     529,   530,     0,   410,     0,     0,     0,     0,     0,     0,
     531,   411,     0,  1406,   532,     0,   533,   412,     0,     0,
     413,     0,     8,   414,   534,     0,   535,   415,     0,     0,
     536,   537,     0,  1421,     0,     0,     0,   538,  1423,  1424,
     417,   418,     0,   221,   222,   223,     0,   225,   226,   227,
     228,   229,   419,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,     0,   243,   244,   245,     0,     0,
     248,   249,   250,   251,   420,   421,   422,   539,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   423,
     424,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   425,   426,
     427,   428,   429,     0,   430,     0,   431,   432,   433,   434,
     435,   436,   437,   438,    56,   439,     0,     0,     0,     0,
       0,     0,   440,   540,   541,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
     442,   443,     0,    14,     0,     0,   444,   445,     0,     0,
       0,     0,     0,     0,     0,   446,     0,   447,   523,   448,
     449,     0,   401,   402,     3,     0,   524,   525,   526,     0,
     527,     0,   403,   404,   405,   406,   407,     0,     0,     0,
       0,     0,   408,   528,   409,   529,   530,     0,   410,     0,
       0,     0,     0,     0,     0,   531,   411,     0,     0,   532,
       0,   533,   412,     0,     0,   413,     0,     8,   414,   534,
       0,   535,   415,     0,     0,   536,   537,     0,     0,     0,
       0,     0,   538,     0,     0,   417,   418,     0,   221,   222,
     223,     0,   225,   226,   227,   228,   229,   419,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,     0,
     243,   244,   245,     0,     0,   248,   249,   250,   251,   420,
     421,   422,   539,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   423,   424,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,   425,   426,   427,   428,   429,     0,   430,
       0,   431,   432,   433,   434,   435,   436,   437,   438,    56,
     439,     0,     0,     0,     0,     0,     0,   440,  1029,   541,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   441,   442,   443,     0,    14,     0,
       0,   444,   445,     0,     0,     0,     0,     0,     0,     0,
     446,     0,   447,   523,   448,   449,     0,   401,   402,     3,
       0,   524,   525,   526,     0,   527,     0,   403,   404,   405,
     406,   407,     0,     0,     0,     0,     0,   408,   528,   409,
     529,   530,     0,   410,     0,     0,     0,     0,     0,     0,
     531,   411,     0,     0,   532,     0,   533,   412,     0,     0,
     413,     0,     8,   414,   534,     0,   535,   415,     0,     0,
     536,   537,     0,     0,     0,     0,     0,   538,     0,     0,
     417,   418,     0,   221,   222,   223,     0,   225,   226,   227,
     228,   229,   419,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,     0,   243,   244,   245,     0,     0,
     248,   249,   250,   251,   420,   421,   422,   539,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   423,
     424,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   425,   426,
     427,   428,   429,     0,   430,     0,   431,   432,   433,   434,
     435,   436,   437,   438,    56,   439,     0,     0,     0,     0,
       0,     0,   440,  1185,   541,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
     442,   443,     0,    14,     0,     0,   444,   445,     0,     0,
       0,   401,   402,     0,     0,   446,     0,   447,     0,   448,
     449,   403,   404,   405,   406,   407,     0,     0,     0,     0,
       0,   408,   528,   409,   529,     0,     0,   410,     0,     0,
       0,     0,     0,     0,     0,   411,     0,     0,     0,     0,
       0,   412,     0,     0,   413,     0,     0,   414,   534,     0,
       0,   415,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   416,     0,     0,   417,   418,     0,   221,   222,   223,
       0,   225,   226,   227,   228,   229,   419,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,     0,   243,
     244,   245,     0,     0,   248,   249,   250,   251,   420,   421,
     422,   539,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   423,   424,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,   425,   426,   427,   428,   429,     0,   430,     0,
     431,   432,   433,   434,   435,   436,   437,   438,    56,   439,
       0,     0,     0,     0,     0,     0,   440,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,   442,   443,     0,    14,     0,     0,
     444,   445,     0,     0,     0,   401,   402,     0,     0,   446,
       0,   447,     0,   448,   449,   403,   404,   405,   406,   407,
       0,     0,     0,     0,     0,   408,     0,   409,     0,     0,
       0,   410,     0,     0,     0,     0,     0,     0,     0,   411,
       0,     0,     0,     0,     0,   412,     0,     0,   413,     0,
       0,   414,     0,     0,     0,   415,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   416,     0,     0,   417,   418,
     822,   221,   222,   223,     0,   225,   226,   227,   228,   229,
     419,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,     0,   243,   244,   245,     0,     0,   248,   249,
     250,   251,   420,   421,   422,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   423,   424,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,   425,   426,   427,   428,
     429,     0,   430,   823,   431,   432,   433,   434,   435,   436,
     437,   438,   824,   439,     0,     0,     0,     0,     0,     0,
     440,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   441,   442,   443,
       0,    14,     0,     0,   444,   445,     0,     0,     0,     0,
       0,   401,   402,   825,     0,   447,   826,   448,   449,   624,
       0,   403,   404,   405,   406,   407,     0,     0,     0,     0,
       0,   408,     0,   409,     0,     0,     0,   410,     0,     0,
       0,     0,     0,     0,     0,   411,     0,     0,     0,     0,
       0,   412,     0,     0,   413,   625,     0,   414,     0,     0,
       0,   415,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   416,     0,     0,   417,   418,     0,   221,   222,   223,
       0,   225,   226,   227,   228,   229,   419,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,     0,   243,
     244,   245,     0,     0,   248,   249,   250,   251,   420,   421,
     422,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   423,   424,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,   425,   426,   427,   428,   429,     0,   430,   823,
     431,   432,   433,   434,   435,   436,   437,   438,   824,   439,
       0,     0,     0,     0,     0,     0,   440,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,   442,   443,     0,    14,     0,     0,
     444,   445,     0,     0,     0,     0,     0,   401,   402,   446,
       0,   447,     0,   448,   449,   624,     0,   403,   404,   405,
     406,   407,     0,     0,     0,     0,     0,   408,     0,   409,
       0,     0,     0,   410,     0,     0,     0,     0,     0,     0,
       0,   411,     0,     0,     0,     0,     0,   412,     0,     0,
     413,   625,     0,   414,     0,     0,     0,   415,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   416,     0,     0,
     417,   418,     0,   221,   222,   223,     0,   225,   226,   227,
     228,   229,   419,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,     0,   243,   244,   245,     0,     0,
     248,   249,   250,   251,   420,   421,   422,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   423,
     424,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   425,   426,
     427,   428,   429,     0,   430,     0,   431,   432,   433,   434,
     435,   436,   437,   438,    56,   439,     0,     0,     0,     0,
       0,     0,   440,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
     442,   443,     0,    14,     0,     0,   444,   445,     0,     0,
       0,   401,   402,     0,     0,   446,     0,   447,     0,   448,
     449,   403,   404,   405,   406,   407,     0,     0,     0,     0,
       0,   408,     0,   409,     0,     0,     0,   410,     0,     0,
       0,     0,     0,     0,     0,   411,     0,     0,     0,     0,
       0,   412,     0,     0,   413,     0,     0,   414,     0,     0,
       0,   415,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   416,     0,     0,   417,   418,     0,   221,   222,   223,
       0,   225,   226,   227,   228,   229,   419,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,     0,   243,
     244,   245,     0,     0,   248,   249,   250,   251,   420,   421,
     422,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   423,   424,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,   425,   426,   427,   428,   429,     0,   430,     0,
     431,   432,   433,   434,   435,   436,   437,   438,    56,   439,
       0,     0,     0,     0,     0,     0,   440,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,   442,   443,     0,    14,     0,     0,
     444,   445,     0,     0,     0,     0,     0,   401,   402,   446,
     496,   447,     0,   448,   449,   615,     0,   403,   404,   405,
     406,   407,     0,     0,     0,     0,     0,   408,     0,   409,
       0,     0,     0,   410,     0,     0,     0,     0,     0,     0,
       0,   411,     0,     0,     0,     0,     0,   412,     0,     0,
     413,     0,     0,   414,     0,     0,     0,   415,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   416,     0,     0,
     417,   418,     0,   221,   222,   223,     0,   225,   226,   227,
     228,   229,   419,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,     0,   243,   244,   245,     0,     0,
     248,   249,   250,   251,   420,   421,   422,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   423,
     424,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   425,   426,
     427,   428,   429,     0,   430,     0,   431,   432,   433,   434,
     435,   436,   437,   438,    56,   439,     0,     0,     0,     0,
       0,     0,   440,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
     442,   443,     0,    14,     0,     0,   444,   445,     0,     0,
       0,   401,   402,     0,     0,   446,     0,   447,     0,   448,
     449,   403,   404,   405,   406,   407,     0,     0,     0,     0,
       0,   408,     0,   409,     0,     0,     0,   410,     0,     0,
       0,     0,     0,     0,     0,   411,     0,     0,     0,     0,
       0,   412,     0,     0,   413,     0,     0,   414,     0,     0,
       0,   415,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   416,     0,     0,   417,   418,     0,   221,   222,   223,
       0,   225,   226,   227,   228,   229,   419,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,     0,   243,
     244,   245,     0,     0,   248,   249,   250,   251,   420,   421,
     422,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   423,   424,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,   425,   426,   427,   428,   429,     0,   430,     0,
     431,   432,   433,   434,   435,   436,   437,   438,    56,   439,
       0,     0,     0,     0,     0,     0,   440,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,   442,   443,     0,    14,     0,     0,
     444,   445,     0,     0,     0,   401,   402,     0,     0,   446,
     694,   447,     0,   448,   449,   403,   404,   405,   406,   407,
       0,     0,     0,     0,     0,   408,     0,   409,     0,     0,
       0,   410,     0,     0,     0,     0,     0,     0,     0,   411,
       0,     0,     0,     0,     0,   412,     0,     0,   413,     0,
       0,   414,     0,     0,     0,   415,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   416,     0,     0,   417,   418,
       0,   221,   222,   223,     0,   225,   226,   227,   228,   229,
     419,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,     0,   243,   244,   245,     0,     0,   248,   249,
     250,   251,   420,   421,   422,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   423,   424,     0,
       0,     0,     0,     0,     0,     0,   712,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,   425,   426,   427,   428,
     429,     0,   430,     0,   431,   432,   433,   434,   435,   436,
     437,   438,    56,   439,     0,     0,     0,     0,     0,     0,
     440,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   441,   442,   443,
       0,    14,     0,     0,   444,   445,     0,     0,     0,   401,
     402,     0,     0,   446,     0,   447,     0,   448,   449,   403,
     404,   405,   406,   407,     0,     0,     0,     0,     0,   408,
       0,   409,     0,     0,     0,   410,     0,     0,     0,     0,
       0,     0,     0,   411,     0,     0,     0,     0,     0,   412,
       0,     0,   413,     0,     0,   414,     0,     0,     0,   415,
       0,     0,     0,     0,     0,   715,     0,     0,     0,   416,
       0,     0,   417,   418,     0,   221,   222,   223,     0,   225,
     226,   227,   228,   229,   419,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,     0,   243,   244,   245,
       0,     0,   248,   249,   250,   251,   420,   421,   422,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   423,   424,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
     425,   426,   427,   428,   429,     0,   430,     0,   431,   432,
     433,   434,   435,   436,   437,   438,    56,   439,     0,     0,
       0,     0,     0,     0,   440,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   441,   442,   443,     0,    14,     0,     0,   444,   445,
       0,     0,     0,   401,   402,     0,     0,   446,     0,   447,
       0,   448,   449,   403,   404,   405,   406,   407,     0,     0,
       0,     0,     0,   408,     0,   409,     0,     0,     0,   410,
       0,     0,     0,     0,     0,     0,     0,   411,     0,     0,
       0,     0,     0,   412,     0,     0,   413,     0,     0,   414,
       0,     0,     0,   415,     0,     0,   721,     0,     0,     0,
       0,     0,     0,   416,     0,     0,   417,   418,     0,   221,
     222,   223,     0,   225,   226,   227,   228,   229,   419,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
       0,   243,   244,   245,     0,     0,   248,   249,   250,   251,
     420,   421,   422,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   423,   424,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,   425,   426,   427,   428,   429,     0,
     430,     0,   431,   432,   433,   434,   435,   436,   437,   438,
      56,   439,     0,     0,     0,     0,     0,     0,   440,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   441,   442,   443,     0,    14,
       0,     0,   444,   445,     0,     0,     0,   401,   402,     0,
       0,   446,     0,   447,     0,   448,   449,   403,   404,   405,
     406,   407,     0,     0,     0,     0,     0,   408,     0,   409,
       0,     0,     0,   410,     0,     0,     0,     0,     0,     0,
       0,   411,     0,     0,     0,     0,     0,   412,     0,     0,
     413,     0,     0,   414,     0,     0,     0,   415,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   416,     0,     0,
     417,   418,     0,   221,   222,   223,     0,   225,   226,   227,
     228,   229,   419,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,     0,   243,   244,   245,     0,     0,
     248,   249,   250,   251,   420,   421,   422,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   423,
     424,     0,     0,     0,     0,     0,     0,     0,   724,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   425,   426,
     427,   428,   429,     0,   430,     0,   431,   432,   433,   434,
     435,   436,   437,   438,    56,   439,     0,     0,     0,     0,
       0,     0,   440,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
     442,   443,     0,    14,     0,     0,   444,   445,     0,     0,
       0,   401,   402,     0,     0,   446,     0,   447,     0,   448,
     449,   403,   404,   405,   406,   407,     0,     0,   863,     0,
       0,   408,     0,   409,     0,     0,     0,   410,     0,     0,
       0,     0,     0,     0,     0,   411,     0,     0,     0,     0,
       0,   412,     0,     0,   413,     0,     0,   414,     0,     0,
       0,   415,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   416,     0,     0,   417,   418,     0,   221,   222,   223,
       0,   225,   226,   227,   228,   229,   419,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,     0,   243,
     244,   245,     0,     0,   248,   249,   250,   251,   420,   421,
     422,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   423,   424,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,   425,   426,   427,   428,   429,     0,   430,     0,
     431,   432,   433,   434,   435,   436,   437,   438,    56,   439,
       0,     0,     0,     0,     0,     0,   440,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,   442,   443,     0,    14,     0,     0,
     444,   445,     0,     0,     0,   401,   402,     0,     0,   446,
       0,   447,     0,   448,   449,   403,   404,   405,   406,   407,
       0,     0,     0,     0,     0,   408,     0,   409,     0,     0,
       0,   410,     0,     0,     0,     0,     0,     0,     0,   411,
       0,     0,     0,     0,     0,   412,     0,     0,   413,     0,
       0,   414,     0,     0,     0,   415,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   416,     0,     0,   417,   418,
       0,   221,   222,   223,     0,   225,   226,   227,   228,   229,
     419,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,     0,   243,   244,   245,     0,     0,   248,   249,
     250,   251,   420,   421,   422,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   423,   424,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,   425,   426,   427,   428,
     429,     0,   430,     0,   431,   432,   433,   434,   435,   436,
     437,   438,    56,   439,     0,     0,     0,     0,     0,     0,
     440,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   441,   442,   443,
       0,    14,     0,     0,   444,   445,     0,     0,     0,   401,
     402,     0,     0,   446,     0,   447,   884,   448,   449,   403,
     404,   405,   406,   407,     0,     0,     0,     0,     0,   408,
       0,   409,     0,     0,     0,   410,     0,     0,     0,     0,
       0,     0,     0,   411,     0,     0,     0,     0,     0,   412,
       0,     0,   413,     0,     0,   414,     0,     0,     0,   415,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   416,
       0,     0,   417,   418,     0,   221,   222,   223,     0,   225,
     226,   227,   228,   229,   419,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,     0,   243,   244,   245,
       0,     0,   248,   249,   250,   251,   420,   421,   422,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   423,   424,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
     425,   426,   427,   428,   429,     0,   430,     0,   431,   432,
     433,   434,   435,   436,   437,   438,    56,   439,     0,     0,
       0,     0,     0,     0,   440,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   441,   442,   443,     0,    14,     0,     0,   444,   445,
       0,     0,     0,   401,   402,     0,     0,   446,     0,   447,
    1158,   448,   449,   403,   404,   405,   406,   407,     0,     0,
       0,     0,     0,   408,     0,   409,     0,     0,     0,   410,
       0,     0,     0,     0,     0,     0,     0,   411,     0,     0,
       0,     0,     0,   412,     0,     0,   413,     0,     0,   414,
       0,     0,     0,   415,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   416,     0,     0,   417,   418,     0,   221,
     222,   223,     0,   225,   226,   227,   228,   229,   419,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
       0,   243,   244,   245,     0,     0,   248,   249,   250,   251,
     420,   421,   422,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   423,   424,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,   425,   426,   427,   428,   429,     0,
     430,     0,   431,   432,   433,   434,   435,   436,   437,   438,
      56,   439,     0,     0,     0,     0,     0,     0,   440,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   441,   442,   443,     0,    14,
       0,     0,   444,   445,     0,     0,     0,   401,   402,     0,
       0,   446,     0,   447,  1167,   448,   449,   403,   404,   405,
     406,   407,     0,     0,     0,     0,     0,   408,     0,   409,
       0,     0,     0,   410,     0,     0,     0,     0,     0,     0,
       0,   411,     0,     0,     0,     0,     0,   412,     0,     0,
     413,     0,     0,   414,     0,     0,     0,   415,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   416,     0,     0,
     417,   418,     0,   221,   222,   223,     0,   225,   226,   227,
     228,   229,   419,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,     0,   243,   244,   245,     0,     0,
     248,   249,   250,   251,   420,   421,   422,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   423,
     424,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   425,   426,
     427,   428,   429,     0,   430,     0,   431,   432,   433,   434,
     435,   436,   437,   438,    56,   439,     0,     0,     0,     0,
       0,     0,   440,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
     442,   443,     0,    14,     0,     0,   444,   445,     0,     0,
       0,   401,   402,     0,     0,   446,     0,   447,  1172,   448,
     449,   403,   404,   405,   406,   407,     0,     0,     0,     0,
       0,   408,     0,   409,     0,     0,     0,   410,     0,     0,
       0,     0,     0,     0,     0,   411,     0,     0,     0,     0,
       0,   412,     0,     0,   413,     0,     0,   414,     0,     0,
       0,   415,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   416,     0,     0,   417,   418,     0,   221,   222,   223,
       0,   225,   226,   227,   228,   229,   419,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,     0,   243,
     244,   245,     0,     0,   248,   249,   250,   251,   420,   421,
     422,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   423,   424,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,   425,   426,   427,   428,   429,     0,   430,     0,
     431,   432,   433,   434,   435,   436,   437,   438,    56,   439,
       0,     0,     0,     0,     0,     0,   440,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,   442,   443,     0,    14,     0,     0,
     444,   445,     0,     0,     0,   401,   402,     0,     0,   446,
       0,   447,  1223,   448,   449,   403,   404,   405,   406,   407,
       0,     0,     0,     0,     0,   408,     0,   409,     0,     0,
       0,   410,     0,     0,     0,     0,     0,     0,     0,   411,
       0,     0,     0,     0,     0,   412,     0,     0,   413,     0,
       0,   414,     0,     0,     0,   415,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   416,     0,     0,   417,   418,
       0,   221,   222,   223,     0,   225,   226,   227,   228,   229,
     419,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,     0,   243,   244,   245,     0,     0,   248,   249,
     250,   251,   420,   421,   422,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   423,   424,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,   425,   426,   427,   428,
     429,     0,   430,     0,   431,   432,   433,   434,   435,   436,
     437,   438,    56,   439,     0,     0,     0,     0,     0,     0,
     440,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   441,   442,   443,
       0,    14,     0,     0,   444,   445,     0,     0,     0,   401,
     402,     0,     0,   446,     0,   447,  1303,   448,   449,   403,
     404,   405,   406,   407,     0,     0,     0,     0,     0,   408,
       0,   409,     0,     0,     0,   410,     0,     0,     0,     0,
       0,     0,     0,   411,     0,     0,     0,     0,     0,   412,
       0,     0,   413,     0,     0,   414,     0,     0,     0,   415,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   416,
       0,     0,   417,   418,     0,   221,   222,   223,     0,   225,
     226,   227,   228,   229,   419,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,     0,   243,   244,   245,
       0,     0,   248,   249,   250,   251,   420,   421,   422,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   423,   424,     0,     0,     0,     0,     0,     0,     0,
    1319,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
     425,   426,   427,   428,   429,     0,   430,     0,   431,   432,
     433,   434,   435,   436,   437,   438,    56,   439,     0,     0,
       0,     0,     0,     0,   440,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   441,   442,   443,     0,    14,     0,     0,   444,   445,
       0,     0,     0,   401,   402,     0,     0,   446,     0,   447,
       0,   448,   449,   403,   404,   405,   406,   407,     0,     0,
       0,     0,     0,   408,     0,   409,     0,     0,     0,   410,
       0,     0,     0,     0,     0,     0,     0,   411,     0,     0,
       0,     0,     0,   412,     0,     0,   413,     0,     0,   414,
       0,     0,     0,   415,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   416,     0,     0,   417,   418,     0,   221,
     222,   223,     0,   225,   226,   227,   228,   229,   419,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
       0,   243,   244,   245,     0,     0,   248,   249,   250,   251,
     420,   421,   422,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   423,   424,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,   425,   426,   427,   428,   429,     0,
     430,     0,   431,   432,   433,   434,   435,   436,   437,   438,
      56,   439,     0,     0,     0,     0,     0,     0,   440,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   441,   442,   443,     0,    14,
       0,     0,   444,   445,     0,     0,     0,   401,   402,     0,
       0,   446,     0,   447,     0,   448,   449,   403,   404,   405,
     406,   407,     0,     0,     0,     0,     0,   408,     0,   409,
       0,     0,     0,   410,     0,     0,     0,     0,     0,     0,
       0,   411,     0,     0,     0,     0,     0,   412,     0,     0,
     413,     0,     0,   414,     0,     0,     0,   415,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   416,     0,     0,
     417,   418,     0,   221,   222,   223,     0,   225,   226,   227,
     228,   229,   419,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,     0,   243,   244,   245,     0,     0,
     248,   249,   250,   251,   420,   421,   422,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   423,
     424,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   425,   426,
     427,   428,   429,     0,   430,     0,   431,   432,   433,   434,
     435,   436,   437,   438,    56,   439,   636,   637,     0,     0,
     215,     0,   440,     0,     0,     0,   216,     0,     0,     0,
       0,     0,   217,     0,     0,     0,     0,     0,     0,   441,
     442,   443,   218,    14,     0,     0,   444,   445,     0,     0,
     219,     0,   636,   637,     0,  1142,     0,   447,     0,   448,
     449,     0,     0,     0,     0,   220,     0,     0,     0,     0,
       0,     0,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,     0,     0,     0,     0,
     638,   639,   640,   641,   642,     0,     0,   643,   644,   645,
     646,     0,   647,   648,   649,   650,   651,     0,   652,   653,
       0,     0,     0,     0,   654,   655,   656,     0,     0,    55,
     657,     0,     0,     0,   636,   637,   638,   639,   640,   641,
     642,     0,   254,   643,   644,   645,   646,     0,   647,   648,
     649,   650,   651,   503,   652,   653,     0,     0,     0,     0,
     654,   655,   656,     0,     0,   658,   657,   659,   660,   661,
     662,   663,   664,   665,   666,   667,   668,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   669,   670,     0,
       0,   688,     0,     0,     0,     0,     0,     0,   255,     0,
       0,   658,     0,   659,   660,   661,   662,   663,   664,   665,
     666,   667,   668,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   669,   670,     0,     0,   908,   638,   639,
     640,   641,   642,   636,   637,   643,   644,   645,   646,     0,
     647,   648,   649,   650,   651,     0,   652,   653,     0,     0,
       0,     0,   654,   655,   656,     0,     0,     0,   657,     0,
       0,     0,     0,     0,   636,   637,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   658,     0,   659,   660,   661,   662,   663,
     664,   665,   666,   667,   668,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   669,   670,     0,     0,   983,
       0,     0,     0,     0,     0,     0,     0,   638,   639,   640,
     641,   642,     0,     0,   643,   644,   645,   646,     0,   647,
     648,   649,   650,   651,     0,   652,   653,     0,     0,     0,
       0,   654,   655,   656,     0,     0,     0,   657,   638,   639,
     640,   641,   642,   636,   637,   643,   644,   645,   646,     0,
     647,   648,   649,   650,   651,     0,   652,   653,     0,     0,
       0,     0,   654,   655,   656,     0,     0,     0,   657,     0,
       0,     0,   658,     0,   659,   660,   661,   662,   663,   664,
     665,   666,   667,   668,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   669,   670,     0,     0,   986,     0,
       0,     0,     0,   658,     0,   659,   660,   661,   662,   663,
     664,   665,   666,   667,   668,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   669,   670,     0,     0,   988,
       0,     0,     0,     0,     0,   636,   637,   638,   639,   640,
     641,   642,     0,     0,   643,   644,   645,   646,     0,   647,
     648,   649,   650,   651,     0,   652,   653,     0,     0,     0,
       0,   654,   655,   656,     0,     0,     0,   657,   636,   637,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   658,     0,   659,   660,   661,   662,   663,   664,
     665,   666,   667,   668,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   669,   670,     0,     0,   996,   638,
     639,   640,   641,   642,     0,     0,   643,   644,   645,   646,
       0,   647,   648,   649,   650,   651,     0,   652,   653,     0,
       0,     0,     0,   654,   655,   656,     0,     0,     0,   657,
     636,   637,   638,   639,   640,   641,   642,     0,     0,   643,
     644,   645,   646,     0,   647,   648,   649,   650,   651,     0,
     652,   653,     0,     0,     0,     0,   654,   655,   656,     0,
       0,     0,   657,     0,   658,     0,   659,   660,   661,   662,
     663,   664,   665,   666,   667,   668,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   669,   670,     0,     0,
     997,     0,     0,     0,     0,     0,     0,   658,     0,   659,
     660,   661,   662,   663,   664,   665,   666,   667,   668,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   669,
     670,     0,     0,   998,   638,   639,   640,   641,   642,   636,
     637,   643,   644,   645,   646,     0,   647,   648,   649,   650,
     651,     0,   652,   653,     0,     0,     0,     0,   654,   655,
     656,     0,     0,     0,   657,     0,     0,     0,     0,     0,
     636,   637,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   658,
       0,   659,   660,   661,   662,   663,   664,   665,   666,   667,
     668,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   669,   670,     0,     0,   999,     0,     0,     0,     0,
       0,     0,     0,   638,   639,   640,   641,   642,     0,     0,
     643,   644,   645,   646,     0,   647,   648,   649,   650,   651,
       0,   652,   653,     0,     0,     0,     0,   654,   655,   656,
       0,     0,     0,   657,   638,   639,   640,   641,   642,   636,
     637,   643,   644,   645,   646,     0,   647,   648,   649,   650,
     651,     0,   652,   653,     0,     0,     0,     0,   654,   655,
     656,     0,     0,     0,   657,     0,     0,     0,   658,     0,
     659,   660,   661,   662,   663,   664,   665,   666,   667,   668,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     669,   670,     0,     0,  1000,     0,     0,     0,     0,   658,
       0,   659,   660,   661,   662,   663,   664,   665,   666,   667,
     668,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   669,   670,     0,     0,  1001,     0,     0,     0,     0,
       0,   636,   637,   638,   639,   640,   641,   642,     0,     0,
     643,   644,   645,   646,     0,   647,   648,   649,   650,   651,
       0,   652,   653,     0,     0,     0,     0,   654,   655,   656,
       0,     0,     0,   657,   636,   637,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   658,     0,
     659,   660,   661,   662,   663,   664,   665,   666,   667,   668,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     669,   670,     0,     0,  1077,   638,   639,   640,   641,   642,
       0,     0,   643,   644,   645,   646,     0,   647,   648,   649,
     650,   651,     0,   652,   653,     0,     0,     0,     0,   654,
     655,   656,     0,     0,     0,   657,   636,   637,   638,   639,
     640,   641,   642,     0,     0,   643,   644,   645,   646,     0,
     647,   648,   649,   650,   651,     0,   652,   653,     0,     0,
       0,     0,   654,   655,   656,     0,     0,     0,   657,     0,
     658,     0,   659,   660,   661,   662,   663,   664,   665,   666,
     667,   668,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   669,   670,     0,     0,  1080,     0,     0,     0,
       0,     0,     0,   658,     0,   659,   660,   661,   662,   663,
     664,   665,   666,   667,   668,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   669,   670,     0,     0,  1083,
     638,   639,   640,   641,   642,   636,   637,   643,   644,   645,
     646,     0,   647,   648,   649,   650,   651,     0,   652,   653,
       0,     0,     0,     0,   654,   655,   656,     0,     0,     0,
     657,     0,     0,     0,     0,     0,   636,   637,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   658,     0,   659,   660,   661,
     662,   663,   664,   665,   666,   667,   668,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   669,   670,     0,
       0,  1108,     0,     0,     0,     0,     0,     0,     0,   638,
     639,   640,   641,   642,     0,     0,   643,   644,   645,   646,
       0,   647,   648,   649,   650,   651,     0,   652,   653,     0,
       0,     0,     0,   654,   655,   656,     0,     0,     0,   657,
     638,   639,   640,   641,   642,   636,   637,   643,   644,   645,
     646,     0,   647,   648,   649,   650,   651,     0,   652,   653,
       0,     0,     0,     0,   654,   655,   656,     0,     0,     0,
     657,     0,     0,     0,   658,     0,   659,   660,   661,   662,
     663,   664,   665,   666,   667,   668,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   669,   670,     0,     0,
    1179,     0,     0,     0,     0,   658,     0,   659,   660,   661,
     662,   663,   664,   665,   666,   667,   668,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   669,   670,     0,
       0,  1187,     0,     0,     0,     0,     0,   636,   637,   638,
     639,   640,   641,   642,     0,     0,   643,   644,   645,   646,
       0,   647,   648,   649,   650,   651,     0,   652,   653,     0,
       0,     0,     0,   654,   655,   656,     0,     0,     0,   657,
     636,   637,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   658,     0,   659,   660,   661,   662,
     663,   664,   665,   666,   667,   668,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   669,   670,     0,     0,
    1228,   638,   639,   640,   641,   642,     0,     0,   643,   644,
     645,   646,     0,   647,   648,   649,   650,   651,     0,   652,
     653,     0,     0,     0,     0,   654,   655,   656,     0,     0,
       0,   657,   636,   637,   638,   639,   640,   641,   642,     0,
       0,   643,   644,   645,   646,     0,   647,   648,   649,   650,
     651,     0,   652,   653,     0,     0,     0,     0,   654,   655,
     656,     0,     0,     0,   657,     0,   658,     0,   659,   660,
     661,   662,   663,   664,   665,   666,   667,   668,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   669,   670,
       0,     0,  1229,     0,     0,     0,     0,     0,     0,   658,
       0,   659,   660,   661,   662,   663,   664,   665,   666,   667,
     668,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   669,   670,     0,     0,  1232,   638,   639,   640,   641,
     642,   636,   637,   643,   644,   645,   646,     0,   647,   648,
     649,   650,   651,     0,   652,   653,     0,     0,     0,     0,
     654,   655,   656,     0,     0,     0,   657,     0,     0,     0,
       0,     0,   636,   637,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   658,     0,   659,   660,   661,   662,   663,   664,   665,
     666,   667,   668,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   669,   670,     0,     0,  1250,     0,     0,
       0,     0,     0,     0,     0,   638,   639,   640,   641,   642,
       0,     0,   643,   644,   645,   646,     0,   647,   648,   649,
     650,   651,     0,   652,   653,     0,     0,     0,     0,   654,
     655,   656,     0,     0,     0,   657,   638,   639,   640,   641,
     642,   636,   637,   643,   644,   645,   646,     0,   647,   648,
     649,   650,   651,     0,   652,   653,     0,     0,     0,     0,
     654,   655,   656,     0,     0,     0,   657,     0,     0,     0,
     658,     0,   659,   660,   661,   662,   663,   664,   665,   666,
     667,   668,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   669,   670,     0,     0,  1252,     0,     0,     0,
       0,   658,     0,   659,   660,   661,   662,   663,   664,   665,
     666,   667,   668,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   669,   670,     0,     0,  1254,     0,     0,
       0,     0,     0,   636,   637,   638,   639,   640,   641,   642,
       0,     0,   643,   644,   645,   646,     0,   647,   648,   649,
     650,   651,     0,   652,   653,     0,     0,     0,     0,   654,
     655,   656,     0,     0,     0,   657,   636,   637,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     658,     0,   659,   660,   661,   662,   663,   664,   665,   666,
     667,   668,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   669,   670,     0,     0,  1258,   638,   639,   640,
     641,   642,     0,     0,   643,   644,   645,   646,     0,   647,
     648,   649,   650,   651,     0,   652,   653,     0,     0,     0,
       0,   654,   655,   656,     0,     0,     0,   657,   636,   637,
     638,   639,   640,   641,   642,     0,     0,   643,   644,   645,
     646,     0,   647,   648,   649,   650,   651,     0,   652,   653,
       0,     0,     0,     0,   654,   655,   656,     0,     0,     0,
     657,     0,   658,     0,   659,   660,   661,   662,   663,   664,
     665,   666,   667,   668,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   669,   670,     0,     0,  1273,     0,
       0,     0,     0,     0,     0,   658,     0,   659,   660,   661,
     662,   663,   664,   665,   666,   667,   668,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   669,   670,     0,
       0,  1325,   638,   639,   640,   641,   642,   636,   637,   643,
     644,   645,   646,     0,   647,   648,   649,   650,   651,     0,
     652,   653,     0,     0,     0,     0,   654,   655,   656,     0,
       0,     0,   657,     0,     0,     0,     0,     0,   636,   637,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   658,     0,   659,
     660,   661,   662,   663,   664,   665,   666,   667,   668,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   669,
     670,     0,     0,  1326,     0,     0,     0,     0,     0,     0,
       0,   638,   639,   640,   641,   642,     0,     0,   643,   644,
     645,   646,     0,   647,   648,   649,   650,   651,     0,   652,
     653,     0,     0,     0,     0,   654,   655,   656,     0,     0,
       0,   657,   638,   639,   640,   641,   642,   636,   637,   643,
     644,   645,   646,     0,   647,   648,   649,   650,   651,     0,
     652,   653,     0,     0,     0,     0,   654,   655,   656,     0,
       0,     0,   657,     0,     0,     0,   658,     0,   659,   660,
     661,   662,   663,   664,   665,   666,   667,   668,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   669,   670,
       0,     0,  1327,     0,     0,     0,     0,   658,     0,   659,
     660,   661,   662,   663,   664,   665,   666,   667,   668,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   669,
     670,     0,     0,  1349,     0,     0,     0,     0,     0,   636,
     637,   638,   639,   640,   641,   642,     0,     0,   643,   644,
     645,   646,     0,   647,   648,   649,   650,   651,     0,   652,
     653,     0,     0,     0,     0,   654,   655,   656,     0,     0,
       0,   657,   636,   637,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   658,     0,   659,   660,
     661,   662,   663,   664,   665,   666,   667,   668,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   669,   670,
       0,     0,  1360,   638,   639,   640,   641,   642,     0,     0,
     643,   644,   645,   646,     0,   647,   648,   649,   650,   651,
       0,   652,   653,     0,     0,     0,     0,   654,   655,   656,
       0,     0,     0,   657,   636,   637,   638,   639,   640,   641,
     642,     0,     0,   643,   644,   645,   646,     0,   647,   648,
     649,   650,   651,     0,   652,   653,     0,     0,     0,     0,
     654,   655,   656,     0,     0,     0,   657,     0,   658,     0,
     659,   660,   661,   662,   663,   664,   665,   666,   667,   668,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     669,   670,     0,     0,  1370,     0,     0,     0,     0,     0,
       0,   658,     0,   659,   660,   661,   662,   663,   664,   665,
     666,   667,   668,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   669,   670,     0,     0,  1405,   638,   639,
     640,   641,   642,   636,   637,   643,   644,   645,   646,     0,
     647,   648,   649,   650,   651,     0,   652,   653,     0,     0,
       0,     0,   654,   655,   656,     0,     0,     0,   657,     0,
       0,     0,     0,     0,   636,   637,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   658,     0,   659,   660,   661,   662,   663,
     664,   665,   666,   667,   668,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   669,   670,     0,     0,  1415,
       0,     0,     0,     0,     0,     0,     0,   638,   639,   640,
     641,   642,     0,     0,   643,   644,   645,   646,     0,   647,
     648,   649,   650,   651,     0,   652,   653,     0,     0,     0,
       0,   654,   655,   656,   636,   637,     0,   657,   638,   639,
     640,   641,   642,     0,     0,   643,   644,   645,   646,     0,
     647,   648,   649,   650,   651,     0,   652,   653,     0,     0,
       0,     0,   654,   655,   656,   636,   637,     0,   657,     0,
       0,     0,   658,     0,   659,   660,   661,   662,   663,   664,
     665,   666,   667,   668,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   669,   670,   693,     0,     0,     0,
       0,     0,     0,   658,     0,   659,   660,   661,   662,   663,
     664,   665,   666,   667,   668,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   669,   670,   904,   638,   639,
     640,   641,   642,     0,     0,   643,   644,   645,   646,     0,
     647,   648,   649,   650,   651,     0,   652,   653,     0,     0,
       0,     0,   654,   655,   656,   636,   637,     0,   657,   638,
     639,   640,   641,   642,     0,     0,   643,   644,   645,   646,
       0,   647,   648,   649,   650,   651,     0,   652,   653,     0,
       0,     0,     0,   654,   655,   656,   636,   637,     0,   657,
       0,     0,     0,   658,     0,   659,   660,   661,   662,   663,
     664,   665,   666,   667,   668,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   669,   670,  1037,     0,     0,
       0,     0,     0,     0,   658,     0,   659,   660,   661,   662,
     663,   664,   665,   666,   667,   668,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   669,   670,  1053,   638,
     639,   640,   641,   642,     0,     0,   643,   644,   645,   646,
       0,   647,   648,   649,   650,   651,     0,   652,   653,     0,
       0,     0,     0,   654,   655,   656,     0,     0,     0,   657,
     638,   639,   640,   641,   642,   636,   637,   643,   644,   645,
     646,     0,   647,   648,   649,   650,   651,     0,   652,   653,
       0,     0,     0,     0,   654,   655,   656,     0,     0,     0,
     657,     0,     0,     0,   658,     0,   659,   660,   661,   662,
     663,   664,   665,   666,   667,   668,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   669,   670,  1165,     0,
       0,     0,     0,     0,     0,   658,     0,   659,   660,   661,
     662,   663,   664,   665,   666,   667,   668,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   669,   670,  1170,
       0,   740,   741,   742,   743,   744,   745,   746,   747,   638,
     639,   640,   641,   642,   748,   749,   643,   644,   645,   646,
     750,   647,   648,   649,   650,   651,   751,   652,   653,   752,
     753,   267,   268,   654,   655,   656,   754,   755,   756,   657,
       0,     0,     0,     0,     0,     0,     0,     0,   269,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   636,   637,     0,     0,  -318,     0,     0,     0,
       0,     0,     0,   757,   658,     0,   659,   660,   661,   662,
     663,   664,   665,   666,   667,   668,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   669,   670,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,     0,     0,   288,   289,   290,
       0,     0,   291,   292,   293,   294,   295,     0,     0,   296,
     297,   298,   299,   300,   301,   302,   638,   639,   640,   641,
     642,   636,   637,   643,   644,   645,   646,     0,   647,   648,
     649,   650,   651,     0,   652,   653,     0,     0,   809,     0,
     654,   655,   656,     0,     0,     0,   657,     0,     0,     0,
     303,     0,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,     0,     0,   314,   315,     0,     0,     0,     0,
       0,     0,   316,   317,     0,     0,     0,     0,     0,     0,
       0,   658,     0,   659,   660,   661,   662,   663,   664,   665,
     666,   667,   668,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   669,   670,     0,     0,     0,     0,     0,
     636,   637,     0,     0,     0,   638,   639,   640,   641,   642,
       0,     0,   643,   644,   645,   646,     0,   647,   648,   649,
     650,   651,     0,   652,   653,     0,     0,     0,     0,   654,
     655,   656,   636,   637,     0,   657,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   961,     0,     0,     0,     0,     0,     0,     0,
     658,     0,   659,   660,   661,   662,   663,   664,   665,   666,
     667,   668,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   669,   670,   638,   639,   640,   641,   642,     0,
       0,   643,   644,   645,   646,     0,   647,   648,   649,   650,
     651,     0,   652,   653,   636,   637,     0,     0,   654,   655,
     656,     0,     0,     0,   657,     0,   638,   639,   640,   641,
     642,     0,     0,   643,   644,   645,   646,     0,   647,   648,
     649,   650,   651,     0,   652,   653,   636,   637,     0,     0,
     654,   655,   656,     0,     0,     0,   657,     0,     0,   658,
    1042,   659,   660,   661,   662,   663,   664,   665,   666,   667,
     668,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   669,   670,  1099,     0,     0,     0,     0,     0,     0,
       0,   658,     0,   659,   660,   661,   662,   663,   664,   665,
     666,   667,   668,     0,     0,     0,     0,     0,   638,   639,
     640,   641,   642,   669,   670,   643,   644,   645,   646,     0,
     647,   648,   649,   650,   651,     0,   652,   653,   636,   637,
       0,     0,   654,   655,   656,     0,     0,     0,   657,     0,
     638,   639,   640,   641,   642,     0,     0,   643,   644,   645,
     646,     0,   647,   648,   649,   650,   651,     0,   652,   653,
       0,  1129,   636,   637,   654,   655,   656,     0,     0,     0,
     657,     0,     0,   658,     0,   659,   660,   661,   662,   663,
     664,   665,   666,   667,   668,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   669,   670,  1343,     0,     0,
       0,     0,     0,     0,     0,   658,     0,   659,   660,   661,
     662,   663,   664,   665,   666,   667,   668,     0,     0,     0,
       0,     0,   638,   639,   640,   641,   642,   669,   670,   643,
     644,   645,   646,     0,   647,   648,   649,   650,   651,     0,
     652,   653,     0,     0,   636,   637,   654,   655,   656,     0,
       0,     0,   657,     0,     0,     0,   638,   639,   640,   641,
     642,     0,     0,   643,   644,   645,   646,     0,   647,   648,
     649,   650,   651,     0,   652,   653,     0,     0,     0,  1361,
     654,   655,   656,     0,     0,     0,   657,   658,     0,   659,
     660,   661,   662,   663,   664,   665,   666,   667,   668,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   669,
     670,     0,     0,  1379,     0,     0,     0,     0,     0,     0,
       0,   658,     0,   659,   660,   661,   662,   663,   664,   665,
     666,   667,   668,   636,   637,     0,     0,     0,   638,   639,
     640,   641,   642,   669,   670,   643,   644,   645,   646,     0,
     647,   648,   649,   650,   651,     0,   652,   653,     0,     0,
       0,     0,   654,   655,   656,     0,     0,     0,   657,   636,
     637,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1407,     0,     0,     0,     0,
       0,     0,     0,   658,     0,   659,   660,   661,   662,   663,
     664,   665,   666,   667,   668,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   669,   670,   638,   639,   640,
     641,   642,     0,     0,   643,   644,   645,   646,     0,   647,
     648,   649,   650,   651,     0,   652,   653,     0,     0,     0,
       0,   654,   655,   656,     0,     0,     0,   657,   636,   637,
       0,     0,     0,   638,   639,   640,   641,   642,     0,     0,
     643,   644,   645,   646,     0,   647,   648,   649,   650,   651,
       0,   652,   653,     0,  1426,     0,     0,   654,   655,   656,
     636,   637,   658,   657,   659,   660,   661,   662,   663,   664,
     665,   666,   667,   668,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   669,   670,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   658,     0,
     659,   660,   661,   662,   663,   664,   665,   666,   667,   668,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     669,   670,   638,   639,   640,   641,   642,     0,     0,   643,
     644,   645,   646,     0,   647,   648,   649,   650,   651,     0,
     652,   653,   636,   637,     0,     0,   654,   655,   656,     0,
       0,     0,  -687,     0,   638,   639,   640,   641,   642,     0,
       0,   643,   644,   645,   646,     0,   647,   648,   649,   650,
     651,     0,   652,   653,   636,   637,     0,     0,   654,   655,
     656,     0,     0,     0,     0,     0,     0,   658,     0,   659,
     660,   661,   662,   663,   664,   665,   666,   667,   668,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   669,
     670,     0,     0,     0,     0,     0,     0,     0,     0,   658,
       0,   659,   660,   661,   662,   663,   664,   665,   666,   667,
     668,     0,     0,     0,     0,     0,   638,   639,   640,   641,
     642,   669,   670,   643,   644,   645,   646,     0,   647,   648,
     649,   650,   651,     0,   652,   653,   636,   637,     0,     0,
     654,     0,   656,     0,     0,     0,     0,     0,   638,   639,
     640,   641,   642,     0,     0,   643,   644,   645,   646,     0,
     647,   648,   649,   650,   651,     0,   652,   653,   636,   637,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   659,   660,   661,   662,   663,   664,   665,
     666,   667,   668,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   669,   670,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   661,   662,   663,
     664,   665,   666,   667,   668,     0,     0,     0,     0,     0,
     638,   639,   640,   641,   642,   669,   670,   643,   644,   645,
     646,     0,   647,   648,   649,   650,   651,     0,   652,   653,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   638,   639,   640,   641,   642,     0,     0,   643,
       0,     0,   646,     0,   647,   648,   649,   650,   651,     0,
     652,   653,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   835,
     662,   663,   664,   665,   666,   667,   668,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   669,   670,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     839,     0,   662,   663,   664,   665,   666,   667,   668,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   669,
     670,   221,   222,   223,     0,   225,   226,   227,   228,   229,
     419,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,     0,   243,   244,   245,     0,     0,   248,   249,
     250,   251,   221,   222,   223,     0,   225,   226,   227,   228,
     229,   419,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,     0,   243,   244,   245,     0,     0,   248,
     249,   250,   251,     0,     0,     0,     0,     0,  1038,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   836,     0,     0,     0,     0,     0,     0,
       0,     0,   837,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   840,     0,     0,     0,     0,     0,
     221,   222,   223,   841,   225,   226,   227,   228,   229,   419,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,     0,   243,   244,   245,     0,     0,   248,   249,   250,
     251,   221,   222,   223,     0,   225,   226,   227,   228,   229,
     419,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,     0,   243,   244,   245,     0,     0,   248,   249,
     250,   251,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1039,   171,     0,     0,     0,     0,   875,   876,
       0,  1040,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   877,     0,     0,     0,     0,     0,   172,
       0,   173,   878,   174,   175,   176,   177,   178,     0,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,   190,   191,   192,     0,     0,   193,   194,   195,   196,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   879,   880,     0,   197,   198,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     199
};

static const yytype_int16 yycheck[] =
{
      14,    15,   708,   165,   565,   372,   152,   530,   630,   480,
     810,   482,   569,   484,   165,   538,   485,   446,   447,   679,
     775,   681,   934,   683,   486,    78,     8,   632,   615,   834,
      20,   368,   458,    20,    22,    15,    16,   624,   822,     5,
     150,    20,   440,    46,    19,    20,    60,    61,    62,    20,
     125,   150,   178,   128,   129,   126,    34,   618,   395,   126,
      33,   132,  1309,   172,  1218,   626,  1307,   628,   137,    57,
       0,   128,   129,   139,   140,   141,   155,     7,   204,   147,
     162,   153,   126,   172,   147,    63,   100,   101,   102,   103,
     162,   162,  1344,    62,   162,     5,     6,   206,   208,   162,
      30,   170,    32,   106,    34,     7,    33,   206,   179,   208,
      40,   178,   179,   184,   181,    25,   126,   184,   170,  1366,
      50,    31,   132,  1364,   711,   207,    56,   202,   203,  1381,
     691,  1285,  1286,   170,   206,   179,   102,   103,   182,   506,
     206,   567,   205,   170,   511,   202,   203,  1301,    50,   178,
      80,   165,   172,   131,   207,   207,   208,   178,    68,    69,
     138,   590,    21,    22,   164,   165,   186,   153,   205,   179,
     138,   600,   102,   103,   603,   155,   162,   206,   205,   178,
     160,   172,   162,   163,   162,   206,   970,   162,   178,   171,
     995,   178,   102,   103,   592,    21,    22,  1351,  1352,   178,
     822,   630,   190,   825,   178,   204,   368,   178,   634,   371,
     577,   184,   126,   181,   202,   179,   138,   195,   132,   185,
     206,   203,   783,   385,   785,   205,   813,   205,   138,   181,
      21,    22,   793,   395,   385,   796,   398,   399,   400,   205,
     162,   255,   172,   672,   126,   126,  1348,   398,   399,   400,
     132,   132,   162,   138,   591,  1357,   138,   184,   117,   118,
     170,   191,   147,   164,   165,   179,   125,   152,   605,   128,
     129,   130,   131,   203,   185,   185,   203,   162,   178,  1191,
     126,   178,   209,   178,   337,   162,   132,  1389,  1390,   115,
     116,   117,   118,   203,   205,   185,   185,   179,   179,   125,
     185,   127,   128,   129,   130,   131,   126,   133,   134,   206,
     172,   206,   132,   475,   476,   205,   205,   479,   685,   481,
     687,   483,   927,   485,   475,   476,   117,   118,   479,   126,
     481,   185,   483,   179,   125,   132,   127,   128,   129,   130,
     131,   126,   178,   202,   203,   162,   178,   132,   970,   153,
     178,   205,   172,   181,   368,   178,   184,   371,   162,   179,
     831,   187,   188,   189,   190,   191,   162,   178,  1028,  1075,
     206,   385,   204,   185,   191,   389,   202,   203,    60,    61,
      62,   395,   179,   206,   398,   399,   400,   172,   138,   205,
      47,   405,   406,   205,   179,   206,   825,   920,   189,   190,
     191,   205,    21,    22,   178,   178,   138,   162,    15,    16,
      67,   202,   203,     5,     6,   147,     8,   784,   100,   101,
     102,   103,  1177,    75,   178,  1225,   178,    79,   897,   591,
     162,   178,   206,   206,   771,   772,   178,   899,  1144,   162,
     162,    93,    94,   605,    36,   782,    98,    99,   100,   101,
     787,   788,   206,   790,   206,   792,   162,   794,   795,   206,
     797,   475,   476,   181,   206,   479,   172,   481,   186,   483,
     153,   485,   178,    12,   636,   637,  1136,   181,   181,   162,
     172,   155,   186,   186,    23,    24,   178,   205,   182,   651,
      21,    22,   186,  1153,   186,   202,   115,   116,   117,   118,
     119,   205,   205,   122,   123,   124,   125,   669,   127,   128,
     129,   130,   131,   153,   133,   134,   181,   172,   172,   153,
    1142,   186,   162,   178,   178,   162,  1232,   689,   162,   172,
     162,   186,   186,   900,  1091,   178,   698,  1292,   689,   701,
     205,   182,   153,   186,  1015,   186,   181,   698,   155,   184,
     701,   162,   172,   160,  1077,   162,   163,  1080,   178,  1030,
    1083,   162,   182,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   153,  1003,   172,   182,   591,  1007,    57,
     186,   178,   162,   202,   203,    63,   117,   118,   182,   186,
     162,   605,   186,   164,   125,    57,   127,   128,   129,   130,
     131,    63,   162,    57,  1127,   972,  1035,    57,   770,   771,
     772,   773,   139,    63,   141,   629,   778,   763,  1047,   770,
     782,   179,   773,  1052,   170,   787,   788,   778,   790,  1152,
     792,   178,   794,   795,   181,   797,  1193,   184,   975,   976,
     977,    57,    57,   164,   981,   170,    57,    63,    63,  1078,
     182,  1212,    63,   139,   186,   172,   187,   188,   189,   190,
     191,   178,    57,   172,  1370,   181,   170,   172,    63,   178,
     172,   202,   203,   178,   182,   689,   178,  1014,   186,   182,
    1302,   182,   182,   186,   698,   186,   186,   701,   182,   182,
     208,    35,   186,   186,   106,    35,    21,    22,   205,  1128,
     170,   863,   170,  1132,   164,   165,   166,   167,   139,   140,
     141,   162,   208,  1142,   162,   164,   165,   166,   185,  1276,
     185,   205,    33,   205,   185,  1248,    10,    11,    12,    21,
      22,  1353,   185,   185,   205,   897,   162,   162,   185,   185,
     185,   185,  1265,  1266,   185,   185,   182,   204,   162,    60,
      61,    22,   914,  1090,   162,   162,   770,   771,   772,   773,
    1382,   204,   181,   914,   778,   170,   162,   132,   782,   203,
     162,   181,   178,   787,   788,   205,   790,   162,   792,   162,
     794,   795,  1339,   797,   185,   185,   185,   205,   185,   206,
     115,   116,   117,   118,   119,  1356,   185,   122,   123,   124,
     125,   205,   127,   128,   129,   130,   131,  1429,   133,   134,
     185,   205,   123,   975,   976,   977,   127,  1154,   185,   981,
     208,   185,   185,  1384,   205,   117,   118,   205,   205,   205,
     162,   205,   205,   125,   205,   127,   128,   129,   130,   131,
     162,   203,   162,   171,   205,   204,   181,   171,   205,   205,
     205,   172,  1014,   162,   155,    37,   172,   172,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   205,    10,   180,
    1299,    66,   172,   184,   172,   172,   187,   202,   203,   172,
     179,   345,  1405,   897,   172,   172,  1409,   172,   178,    13,
     186,   355,   203,   205,   185,   205,   185,   178,   209,   185,
     914,   365,   178,     4,   208,   162,   171,    43,    14,  1071,
     202,   203,   206,   179,   181,   155,   170,     8,    21,    22,
    1071,    70,   205,   387,   162,   205,   186,   171,  1090,   204,
     178,     1,   206,   205,   185,   205,   205,   205,  1275,   185,
     162,   186,   206,   205,   205,   186,   205,   205,   162,   205,
     205,   186,   205,   162,    43,   205,   162,    67,   171,   423,
     424,   975,   976,   977,   172,   186,   186,   981,   186,   206,
     206,   205,    43,   206,   186,   186,   440,   441,   442,   443,
     444,   445,   186,   186,   186,   186,   186,   206,   185,   206,
     205,   162,  1154,   162,  1156,   162,   206,   186,   205,   186,
    1014,   206,   186,   186,   162,  1156,   205,   205,   186,   205,
     162,   205,   115,   116,   117,   118,   119,   162,   206,   122,
     123,   124,   125,   162,   127,   128,   129,   130,   131,   205,
     133,   134,   205,   162,   498,    12,   139,   140,   141,   162,
      43,    33,   145,   205,   205,   186,   205,   162,   186,   206,
     205,   205,   516,   186,   205,   205,   172,  1071,    21,    22,
     206,   205,   162,   172,   528,   205,    53,   531,   205,   171,
     205,   205,   614,   537,   206,   539,  1090,   180,   206,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   206,
     172,   204,   206,  1255,   206,   178,    77,   204,   206,   202,
     203,   206,   206,   206,  1255,     1,    42,   206,   206,   573,
     205,   129,   206,  1275,   206,    80,  1425,  1271,  1295,    99,
     213,  1271,  1271,  1271,  1271,     1,   465,  1243,   592,   593,
    1283,  1196,   596,  1095,   598,  1199,   520,    51,   759,  1284,
    1154,  1353,  1156,    -1,   608,   609,   610,   611,   612,   613,
      -1,    -1,   115,   116,   117,   118,   119,    -1,   405,   122,
     123,   124,   125,   405,   127,   128,   129,   130,   131,    -1,
     133,   134,    -1,    -1,   638,   639,   139,    -1,   642,   643,
     644,   645,    -1,   647,    -1,   649,   650,   651,   652,   653,
     654,   655,   656,   657,   658,   659,   660,   661,   662,   663,
     664,   665,   666,   667,   668,    -1,   670,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,  1255,    -1,    -1,    -1,    -1,   710,    -1,   712,    -1,
      -1,   715,    -1,   717,    -1,    -1,    -1,    -1,    -1,    -1,
     724,  1275,    -1,    -1,    -1,   729,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,   750,   751,   752,   753,
     754,   755,   756,   757,   758,  1309,    -1,    -1,    -1,    -1,
      -1,   765,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   777,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,
      -1,    -1,   806,    -1,    -1,   809,   810,    49,    -1,    -1,
     814,    -1,  1366,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   832,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   876,    -1,    -1,    -1,   880,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    -1,   909,    -1,    -1,    49,   151,
      -1,    -1,   916,    -1,    -1,    -1,    -1,    -1,    -1,   923,
     162,    -1,    -1,    64,    -1,   929,    -1,    -1,    -1,    -1,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,   207,    -1,   209,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1012,  1013,
     151,    -1,    -1,    -1,    -1,  1019,  1020,  1021,    -1,  1023,
      -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,  1032,    -1,
    1034,   172,  1036,    -1,    -1,    -1,    -1,   178,  1042,    -1,
      -1,    -1,  1046,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1054,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,   207,    25,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,  1082,    -1,
      -1,  1085,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,  1115,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1162,    -1,
    1164,    -1,    -1,    -1,    -1,  1169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1181,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1194,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
    1204,    -1,    -1,    -1,   162,    -1,    -1,    -1,    -1,    -1,
    1214,  1215,  1216,    -1,    -1,    -1,    -1,  1221,    -1,    -1,
      -1,  1225,  1226,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1235,  1236,  1237,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1245,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   207,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1271,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1279,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1295,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1308,    -1,    -1,  1311,  1312,    -1,
      -1,    -1,    -1,    -1,    -1,  1319,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1331,    -1,    -1,
      -1,    -1,    -1,     1,    -1,    -1,  1340,     5,     6,     7,
      -1,     9,    10,    11,    -1,    13,  1350,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    29,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    39,    -1,  1377,    42,    -1,    44,    45,    -1,    -1,
      48,    -1,    50,    51,    52,    -1,    54,    55,    -1,    -1,
      58,    59,    -1,  1397,    -1,    -1,    -1,    65,  1402,  1403,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,    -1,    -1,
      -1,    -1,   170,   171,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
     188,   189,    -1,   191,    -1,    -1,   194,   195,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   203,    -1,   205,     1,   207,
     208,    -1,     5,     6,     7,    -1,     9,    10,    11,    -1,
      13,    -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    29,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    42,
      -1,    44,    45,    -1,    -1,    48,    -1,    50,    51,    52,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,    -1,    -1,    -1,    -1,   170,   171,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,   188,   189,    -1,   191,    -1,
      -1,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     203,    -1,   205,     1,   207,   208,    -1,     5,     6,     7,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,    -1,    -1,
      -1,    -1,   170,   171,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
     188,   189,    -1,   191,    -1,    -1,   194,   195,    -1,    -1,
      -1,     5,     6,    -1,    -1,   203,    -1,   205,    -1,   207,
     208,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    52,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,   149,   150,    -1,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,   188,   189,    -1,   191,    -1,    -1,
     194,   195,    -1,    -1,    -1,     5,     6,    -1,    -1,   203,
      -1,   205,    -1,   207,   208,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      70,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,   189,
      -1,   191,    -1,    -1,   194,   195,    -1,    -1,    -1,    -1,
      -1,     5,     6,   203,    -1,   205,   206,   207,   208,    13,
      -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,   188,   189,    -1,   191,    -1,    -1,
     194,   195,    -1,    -1,    -1,    -1,    -1,     5,     6,   203,
      -1,   205,    -1,   207,   208,    13,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,    -1,    -1,
      -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
     188,   189,    -1,   191,    -1,    -1,   194,   195,    -1,    -1,
      -1,     5,     6,    -1,    -1,   203,    -1,   205,    -1,   207,
     208,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,   149,   150,    -1,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,   188,   189,    -1,   191,    -1,    -1,
     194,   195,    -1,    -1,    -1,    -1,    -1,     5,     6,   203,
     204,   205,    -1,   207,   208,    13,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,    -1,    -1,
      -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
     188,   189,    -1,   191,    -1,    -1,   194,   195,    -1,    -1,
      -1,     5,     6,    -1,    -1,   203,    -1,   205,    -1,   207,
     208,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,   149,   150,    -1,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,   188,   189,    -1,   191,    -1,    -1,
     194,   195,    -1,    -1,    -1,     5,     6,    -1,    -1,   203,
     204,   205,    -1,   207,   208,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,   149,
     150,    -1,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,   189,
      -1,   191,    -1,    -1,   194,   195,    -1,    -1,    -1,     5,
       6,    -1,    -1,   203,    -1,   205,    -1,   207,   208,    15,
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
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,   195,
      -1,    -1,    -1,     5,     6,    -1,    -1,   203,    -1,   205,
      -1,   207,   208,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   187,   188,   189,    -1,   191,
      -1,    -1,   194,   195,    -1,    -1,    -1,     5,     6,    -1,
      -1,   203,    -1,   205,    -1,   207,   208,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,    -1,    -1,
      -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
     188,   189,    -1,   191,    -1,    -1,   194,   195,    -1,    -1,
      -1,     5,     6,    -1,    -1,   203,    -1,   205,    -1,   207,
     208,    15,    16,    17,    18,    19,    -1,    -1,    22,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,   149,   150,    -1,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,   188,   189,    -1,   191,    -1,    -1,
     194,   195,    -1,    -1,    -1,     5,     6,    -1,    -1,   203,
      -1,   205,    -1,   207,   208,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,   149,
     150,    -1,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,   189,
      -1,   191,    -1,    -1,   194,   195,    -1,    -1,    -1,     5,
       6,    -1,    -1,   203,    -1,   205,   206,   207,   208,    15,
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
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,   195,
      -1,    -1,    -1,     5,     6,    -1,    -1,   203,    -1,   205,
     206,   207,   208,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   187,   188,   189,    -1,   191,
      -1,    -1,   194,   195,    -1,    -1,    -1,     5,     6,    -1,
      -1,   203,    -1,   205,   206,   207,   208,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,    -1,    -1,
      -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
     188,   189,    -1,   191,    -1,    -1,   194,   195,    -1,    -1,
      -1,     5,     6,    -1,    -1,   203,    -1,   205,   206,   207,
     208,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,   149,   150,    -1,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,   188,   189,    -1,   191,    -1,    -1,
     194,   195,    -1,    -1,    -1,     5,     6,    -1,    -1,   203,
      -1,   205,   206,   207,   208,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,   149,
     150,    -1,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,   189,
      -1,   191,    -1,    -1,   194,   195,    -1,    -1,    -1,     5,
       6,    -1,    -1,   203,    -1,   205,   206,   207,   208,    15,
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
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,   195,
      -1,    -1,    -1,     5,     6,    -1,    -1,   203,    -1,   205,
      -1,   207,   208,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   187,   188,   189,    -1,   191,
      -1,    -1,   194,   195,    -1,    -1,    -1,     5,     6,    -1,
      -1,   203,    -1,   205,    -1,   207,   208,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    21,    22,    -1,    -1,
      19,    -1,   170,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   187,
     188,   189,    41,   191,    -1,    -1,   194,   195,    -1,    -1,
      49,    -1,    21,    22,    -1,   203,    -1,   205,    -1,   207,
     208,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,   138,
     145,    -1,    -1,    -1,    21,    22,   115,   116,   117,   118,
     119,    -1,   151,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,   162,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,   141,    -1,    -1,   180,   145,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,
      -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,
      -1,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,    -1,    -1,   206,   115,   116,
     117,   118,   119,    21,    22,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,   115,   116,
     117,   118,   119,    21,    22,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,
      -1,    -1,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,    -1,
      -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    21,    22,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      21,    22,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,
      -1,    -1,   145,    -1,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,
     206,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,    -1,    -1,   206,   115,   116,   117,   118,   119,    21,
      22,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,    -1,    -1,   206,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,   115,   116,   117,   118,   119,    21,
      22,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,   145,    -1,    -1,    -1,   180,    -1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     202,   203,    -1,    -1,   206,    -1,    -1,    -1,    -1,   180,
      -1,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,    -1,    -1,   206,    -1,    -1,    -1,    -1,
      -1,    21,    22,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     202,   203,    -1,    -1,   206,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    21,    22,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,
     180,    -1,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,    -1,    -1,   206,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,
     115,   116,   117,   118,   119,    21,    22,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,
      -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
     115,   116,   117,   118,   119,    21,    22,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,
     145,    -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,
     206,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,
      -1,   206,    -1,    -1,    -1,    -1,    -1,    21,    22,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,
     206,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    21,    22,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,   145,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,    -1,    -1,   206,   115,   116,   117,   118,
     119,    21,    22,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,    -1,    -1,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,   115,   116,   117,   118,
     119,    21,    22,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,
     180,    -1,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,    -1,    -1,   206,    -1,    -1,    -1,
      -1,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,    -1,    -1,   206,    -1,    -1,
      -1,    -1,    -1,    21,    22,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,    -1,    -1,   206,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    21,    22,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,
     145,    -1,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,
      -1,   206,   115,   116,   117,   118,   119,    21,    22,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,   115,   116,   117,   118,   119,    21,    22,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,
      -1,    -1,   145,    -1,    -1,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
      -1,    -1,   206,    -1,    -1,    -1,    -1,   180,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    21,
      22,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
      -1,    -1,   206,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    21,    22,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,   141,    -1,    -1,    -1,   145,    -1,   180,    -1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     202,   203,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,    -1,    -1,   206,   115,   116,
     117,   118,   119,    21,    22,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    21,    22,    -1,   145,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    21,    22,    -1,   145,    -1,
      -1,    -1,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,   204,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    21,    22,    -1,   145,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    21,    22,    -1,   145,
      -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   204,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
     115,   116,   117,   118,   119,    21,    22,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,
     145,    -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   204,
      -1,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,    21,    22,   139,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,   172,    -1,    -1,    -1,
      -1,    -1,    -1,   179,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    -1,    -1,   127,   128,   129,
      -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   115,   116,   117,   118,
     119,    21,    22,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    -1,    -1,   137,    -1,
     139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,
     180,    -1,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,   194,   195,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    21,    22,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    21,    22,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,   145,    -1,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    21,    22,    -1,    -1,
     139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,   115,   116,
     117,   118,   119,   202,   203,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    21,    22,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,    -1,   133,   134,
      -1,   168,    21,    22,   139,   140,   141,    -1,    -1,    -1,
     145,    -1,    -1,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,   172,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,   202,   203,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,    -1,
     133,   134,    -1,    -1,    21,    22,   139,   140,   141,    -1,
      -1,    -1,   145,    -1,    -1,    -1,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,   172,
     139,   140,   141,    -1,    -1,    -1,   145,   180,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,    -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    21,    22,    -1,    -1,    -1,   115,   116,
     117,   118,   119,   202,   203,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    21,    22,
      -1,    -1,    -1,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,   172,    -1,    -1,   139,   140,   141,
      21,    22,   180,   145,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     202,   203,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,    -1,
     133,   134,    21,    22,    -1,    -1,   139,   140,   141,    -1,
      -1,    -1,   145,    -1,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    21,    22,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,
     119,   202,   203,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    21,    22,    -1,    -1,
     139,    -1,   141,    -1,    -1,    -1,    -1,    -1,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,
     115,   116,   117,   118,   119,   202,   203,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,    -1,    -1,   122,
      -1,    -1,   125,    -1,   127,   128,   129,   130,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,
      71,    72,    73,   162,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    35,    -1,    -1,    -1,    -1,   128,   129,
      -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,    71,
      -1,    73,   162,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     162
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   211,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   172,   191,   203,   212,   215,   221,   223,
     224,   228,   254,   258,   282,   356,   363,   367,   376,   422,
     426,   430,    19,    20,   162,   246,   247,   248,   155,   229,
     230,   162,   191,   225,   226,    57,    63,   360,   361,   162,
     207,   214,   360,   360,   360,   138,   162,   270,    34,    63,
     131,   195,   205,   250,   251,   252,   253,   270,   172,   172,
       5,     6,     8,    36,   373,    62,   354,   179,   178,   181,
     178,   225,    22,    57,   190,   202,   227,   162,   172,   354,
     162,   162,   162,   162,   138,   222,   252,   252,   252,   205,
     139,   140,   141,   178,   204,    57,    63,   259,   261,    57,
      63,   364,    57,    63,   374,    57,    63,   355,    15,    16,
     155,   160,   162,   163,   205,   217,   247,   155,   230,   162,
     162,   162,   362,    57,    63,   213,   431,   423,   427,   162,
     164,   220,   206,   248,   252,   252,   252,   252,   262,   162,
     365,   377,   357,   164,   165,   216,    15,    16,   155,   160,
     162,   217,   244,   245,   227,   179,   170,   170,   170,   164,
     206,    35,    71,    73,    75,    76,    77,    78,    79,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      93,    94,    95,    98,    99,   100,   101,   117,   118,   162,
     257,   260,   181,   366,   106,   371,   372,   208,   249,   328,
     164,   165,   166,   178,   206,    19,    25,    31,    41,    49,
      64,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   151,   207,   270,   380,   382,   383,
     386,   392,   393,   421,   432,   424,   428,    21,    22,    38,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   127,   128,
     129,   132,   133,   134,   135,   136,   139,   140,   141,   142,
     143,   144,   145,   180,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   194,   195,   202,   203,    35,    35,
     205,   255,   170,   263,    75,    79,    93,    94,    98,    99,
     100,   101,   381,   170,   162,   378,   247,   208,   162,   351,
     353,   244,   185,   185,   185,   205,   185,   185,   205,   185,
     185,   185,   185,   185,   185,   205,   270,    33,    60,    61,
     123,   127,   180,   184,   187,   203,   209,   391,   182,   162,
     385,   342,   345,   162,   162,   162,   204,    22,   162,   204,
     150,   206,   328,   338,   339,   181,   256,   266,   368,   181,
     370,   170,   375,   247,   178,   181,   184,   349,   394,   399,
     401,     5,     6,    15,    16,    17,    18,    19,    25,    27,
      31,    39,    45,    48,    51,    55,    65,    68,    69,    80,
     102,   103,   104,   117,   118,   146,   147,   148,   149,   150,
     152,   154,   155,   156,   157,   158,   159,   160,   161,   163,
     170,   187,   188,   189,   194,   195,   203,   205,   207,   208,
     219,   221,   264,   270,   275,   287,   294,   297,   300,   304,
     306,   308,   309,   311,   316,   319,   320,   327,   380,   434,
     442,   452,   455,   467,   470,   403,   397,   162,   387,   405,
     407,   409,   411,   413,   415,   417,   419,   320,   185,   205,
      33,   184,    33,   184,   203,   209,   204,   320,   203,   209,
     392,   178,   469,   162,   172,   178,   340,   389,   421,   425,
     162,   343,   389,   429,   162,   132,   205,     7,    50,   281,
     172,   206,   421,     1,     9,    10,    11,    13,    26,    28,
      29,    38,    42,    44,    52,    54,    58,    59,    65,   105,
     171,   172,   231,   232,   235,   237,   238,   239,   240,   241,
     242,   243,   265,   271,   276,   277,   278,   279,   280,   282,
     286,   307,   320,   162,   358,   359,   270,   334,   162,   392,
     126,   132,   179,   348,   421,   421,   390,   421,   185,   185,
     185,   272,   382,   434,   270,   185,     5,   102,   103,   185,
     205,   185,   205,   205,   185,   185,   205,   185,   205,   185,
     205,   185,   185,   205,   185,   185,   320,   320,   205,   205,
     205,   205,   205,   205,   218,    13,   320,   451,   466,   320,
     320,   320,   320,   320,    13,    49,   298,   320,   298,   208,
     205,   254,   170,   208,   300,   305,    21,    22,   115,   116,
     117,   118,   119,   122,   123,   124,   125,   127,   128,   129,
     130,   131,   133,   134,   139,   140,   141,   145,   180,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   202,
     203,   206,   205,   421,   421,   206,   162,   384,   421,   255,
     421,   255,   421,   255,   340,   341,   343,   344,   206,   396,
     267,   298,   204,   204,   204,   320,   162,   433,   181,   389,
     171,   181,   389,   171,   320,   147,   162,   347,   379,   338,
     205,   205,   126,   320,   263,    61,   320,   205,   162,   172,
     155,    58,   320,   263,   126,   320,    37,   172,   172,   205,
      10,   172,   172,   172,   172,   172,   172,    66,   283,   172,
     107,   108,   109,   110,   111,   112,   113,   114,   120,   121,
     126,   132,   135,   136,   142,   143,   144,   179,   179,   178,
     469,   171,   254,   335,   172,   348,   320,   186,   186,   186,
     389,   443,   445,   273,   205,   205,   185,   205,   295,   185,
     185,   185,   462,   298,   392,   466,   320,   288,   290,   320,
     292,   320,   464,   298,   449,   453,   298,   447,   392,   320,
     320,   320,   320,   320,   320,   166,   167,   216,   379,   137,
     178,   469,   379,    13,   178,   469,   469,   147,   152,   185,
     270,   310,    70,   153,   162,   203,   206,   298,   435,   437,
       4,   303,   266,   208,   254,    19,   153,   162,   380,    19,
     153,   162,   380,   320,   320,   320,   320,   320,   320,   162,
     320,   153,   162,   320,   320,   320,   380,   320,   320,   320,
     320,   320,   320,    22,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   128,   129,   153,   162,   202,
     203,   317,   380,   320,   206,   298,   186,   186,   172,   186,
     186,   256,   186,   256,   186,   256,   186,   389,   186,   389,
     269,   421,   206,   469,   204,   171,   421,   421,   206,   205,
      43,   126,   178,   179,   181,   184,   346,   320,   379,   320,
      14,   320,   320,   179,   181,   155,   320,   170,   320,   205,
     147,   162,   205,   285,   350,   352,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   358,   369,     8,   328,   333,
     320,   172,   395,   400,   402,   421,   392,   392,   421,    70,
     441,   267,   162,   320,   421,   456,   458,   460,   392,   469,
     186,   389,   469,   206,   392,   392,   206,   392,   206,   392,
     469,   392,   392,   469,   392,   186,   206,   206,   206,   206,
     206,   206,   320,    20,   320,   451,   171,    20,   379,   320,
     204,   206,   205,   205,   312,   314,   162,   206,   437,   205,
     132,   346,   435,   178,   206,   178,   206,   205,   255,   171,
     303,   185,   205,   185,   205,   205,   205,   204,    19,   153,
     162,   380,   181,   153,   162,   320,   205,   205,   153,   162,
     320,     1,   205,   204,   178,   206,   404,   398,   162,   388,
     406,   186,   410,   186,   414,   186,   418,   340,   420,   343,
     186,   389,   320,   162,   162,   421,   320,   206,    20,   263,
     206,   320,   266,   206,   320,   205,    43,   162,   284,   178,
     181,   349,   171,    57,    63,   331,    67,   332,   172,   172,
     186,   186,   186,   206,   437,   206,   186,   389,   206,   186,
     392,   392,   392,   186,   206,   205,   392,   206,   186,   186,
     186,   186,   206,   186,   186,   206,   186,   303,   205,   168,
     298,   298,    20,   320,   320,   392,   255,   206,   320,   320,
     320,   204,   203,   153,   162,   126,   132,   162,   179,   184,
     301,   302,   256,   255,   321,   320,   323,   320,   206,   298,
     320,   185,   205,   320,   205,   204,   320,   206,   298,   205,
     204,   318,   206,   298,   408,   412,   416,   205,   421,   206,
      43,   346,   263,   298,   263,   171,   263,   206,   320,   162,
     178,   206,   162,   392,   348,    47,   332,    46,   106,   329,
     444,   446,   274,   206,   205,   162,   296,   186,   186,   186,
     463,   268,   466,   186,   289,   291,   293,   465,   450,   454,
     448,   205,   263,   206,   298,   172,   172,   298,   206,   206,
     186,   256,   206,   206,   435,   205,   132,   346,   162,   162,
     205,   162,   162,   178,   206,   137,   263,   299,   256,   392,
     206,   421,   206,   206,   206,   325,   320,   320,   206,   206,
     320,   206,   267,   162,   320,   206,    12,    23,    24,   233,
     234,    12,   236,   206,   162,   181,   349,    43,   172,   348,
     320,    33,   330,   329,   331,   205,   205,   320,   186,   457,
     459,   461,   205,   206,   469,   205,   320,   320,   320,   205,
     441,   205,   205,   206,   320,   206,   451,   320,   172,   313,
     186,   132,   346,   204,   320,   320,   320,   162,   301,   126,
     320,   263,   186,   186,   421,   206,   206,   206,   206,   263,
     263,   205,   237,   276,   277,   278,   279,   320,   172,   392,
     348,   162,   320,   172,   336,   330,   347,   441,   441,   206,
     205,   205,   205,   205,   267,   268,   298,   441,   435,   436,
     206,   172,   468,   468,   320,   310,   315,   320,   320,   206,
     206,   206,   320,   322,   324,   186,   320,   348,   320,   172,
     260,   337,   205,   435,   438,   439,   440,   440,   320,   441,
     441,   436,   206,   206,   469,   440,   206,    53,   171,   204,
     468,   310,   132,   346,   326,   206,   320,   172,   172,   260,
     435,   178,   469,   206,   206,   206,   440,   440,   206,   206,
     206,   320,   204,   320,   320,   263,   172,   263,   206,   205,
     206,   206,   234,   435,   206
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   210,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   212,   213,   213,
     213,   214,   214,   215,   216,   216,   216,   216,   217,   218,
     218,   218,   219,   220,   220,   222,   221,   223,   224,   225,
     225,   225,   225,   226,   226,   227,   227,   228,   229,   229,
     230,   230,   231,   232,   232,   233,   233,   234,   234,   234,
     235,   235,   236,   236,   237,   237,   237,   237,   237,   238,
     238,   239,   240,   241,   242,   243,   244,   244,   244,   244,
     244,   244,   245,   245,   246,   246,   246,   247,   247,   247,
     247,   247,   247,   247,   247,   248,   248,   249,   249,   250,
     250,   250,   251,   251,   252,   252,   252,   252,   252,   252,
     252,   253,   253,   254,   254,   255,   255,   255,   256,   256,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   258,   259,   259,   259,   260,
     262,   261,   263,   263,   264,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   266,   266,   266,   267,   267,   268,   268,
     269,   269,   270,   270,   270,   271,   271,   273,   274,   272,
     275,   275,   275,   275,   275,   276,   277,   278,   278,   278,
     279,   279,   280,   281,   281,   281,   282,   282,   283,   283,
     284,   284,   285,   285,   286,   286,   288,   289,   287,   290,
     291,   287,   292,   293,   287,   295,   296,   294,   297,   297,
     297,   298,   298,   299,   299,   299,   300,   300,   300,   301,
     301,   301,   301,   301,   302,   302,   303,   303,   304,   305,
     305,   306,   306,   306,   306,   306,   306,   306,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   308,   308,   309,
     309,   310,   310,   311,   312,   313,   311,   314,   315,   311,
     316,   316,   316,   316,   316,   316,   317,   318,   316,   319,
     319,   319,   319,   319,   319,   319,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   321,   322,   320,   320,   320,   320,   323,
     324,   320,   320,   320,   325,   326,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   328,   328,   329,   329,
     329,   330,   330,   331,   331,   331,   332,   332,   333,   334,
     335,   334,   336,   334,   337,   334,   338,   338,   339,   339,
     340,   340,   341,   341,   342,   342,   342,   343,   344,   344,
     345,   345,   345,   346,   346,   347,   347,   347,   347,   347,
     348,   348,   348,   349,   349,   350,   350,   350,   350,   350,
     351,   351,   352,   352,   352,   353,   353,   353,   354,   354,
     355,   355,   355,   357,   356,   358,   358,   359,   359,   359,
     360,   360,   360,   362,   361,   363,   364,   364,   364,   365,
     366,   366,   368,   369,   367,   370,   370,   371,   371,   372,
     373,   373,   374,   374,   374,   375,   375,   377,   378,   376,
     379,   379,   379,   379,   379,   380,   380,   380,   380,   380,
     380,   380,   380,   380,   380,   380,   380,   380,   380,   380,
     380,   380,   380,   380,   380,   380,   380,   380,   380,   380,
     380,   380,   381,   381,   381,   381,   381,   381,   381,   381,
     382,   383,   383,   383,   384,   384,   385,   385,   385,   387,
     388,   386,   389,   389,   390,   390,   391,   391,   391,   391,
     392,   392,   393,   393,   393,   393,   394,   395,   393,   393,
     393,   396,   393,   393,   393,   393,   393,   393,   393,   393,
     393,   393,   393,   393,   393,   397,   398,   393,   393,   399,
     400,   393,   401,   402,   393,   403,   404,   393,   393,   405,
     406,   393,   407,   408,   393,   393,   409,   410,   393,   411,
     412,   393,   393,   413,   414,   393,   415,   416,   393,   417,
     418,   393,   419,   420,   393,   421,   421,   421,   423,   424,
     425,   422,   427,   428,   429,   426,   431,   432,   433,   430,
     434,   434,   434,   434,   434,   435,   435,   435,   435,   435,
     435,   435,   435,   436,   437,   438,   438,   439,   439,   440,
     440,   441,   441,   443,   444,   442,   445,   446,   442,   447,
     448,   442,   449,   450,   442,   451,   451,   452,   453,   454,
     452,   455,   456,   457,   455,   458,   459,   455,   460,   461,
     455,   455,   462,   463,   455,   455,   464,   465,   455,   466,
     466,   467,   467,   467,   467,   468,   468,   469,   469,   470,
     470,   470
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       3,     2,     2,     2,     2,     2,     2,     2,     0,     1,
       1,     1,     1,     4,     1,     1,     2,     2,     3,     0,
       2,     4,     3,     1,     2,     0,     4,     2,     2,     1,
       2,     3,     3,     2,     4,     0,     1,     2,     1,     3,
       1,     3,     3,     3,     2,     1,     1,     0,     2,     6,
       1,     1,     0,     2,     1,     1,     1,     1,     1,     6,
       7,     7,     2,     5,     5,     4,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     1,     5,     1,     3,     2,     3,     1,
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
       0,     4,     3,     7,     2,     1,     2,     2,     1,     1,
       1,     1,     2,     1,     2,     2,     2,     2,     1,     1,
       2,     2,     2,     0,     2,     2,     0,     2,     0,     2,
       1,     3,     1,     3,     2,     2,     3,     0,     0,     5,
       2,     5,     5,     6,     2,     1,     1,     1,     2,     3,
       2,     3,     4,     1,     1,     0,     1,     1,     1,     0,
       1,     3,     8,     7,     3,     3,     0,     0,     7,     0,
       0,     7,     0,     0,     7,     0,     0,     6,     5,     8,
      10,     1,     3,     1,     2,     3,     1,     1,     2,     2,
       2,     2,     2,     4,     1,     3,     0,     4,     6,     6,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     6,     8,     5,
       6,     1,     4,     3,     0,     0,     8,     0,     0,     9,
       3,     4,     5,     6,     5,     6,     0,     0,     5,     3,
       4,     4,     5,     4,     3,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     2,     4,     4,     5,     4,
       5,     3,     4,     1,     1,     2,     4,     4,     7,     8,
       6,     3,     5,     0,     0,     8,     3,     3,     3,     0,
       0,     8,     3,     4,     0,     0,     9,     4,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     1,     4,
       4,     4,     4,     4,     4,     1,     6,     7,     6,     6,
       7,     7,     6,     7,     6,     6,     0,     1,     0,     1,
       1,     0,     1,     0,     1,     1,     0,     1,     5,     0,
       0,     4,     0,     9,     0,    10,     3,     4,     1,     3,
       1,     3,     1,     3,     0,     2,     3,     3,     1,     3,
       0,     2,     3,     1,     1,     1,     2,     3,     5,     3,
       1,     1,     1,     0,     1,     1,     4,     3,     3,     5,
       1,     3,     4,     6,     5,     4,     6,     5,     0,     1,
       0,     1,     1,     0,     6,     1,     3,     0,     1,     3,
       0,     1,     1,     0,     5,     3,     0,     1,     1,     1,
       0,     2,     0,     0,    11,     0,     2,     0,     1,     3,
       1,     1,     0,     1,     1,     0,     3,     0,     0,     7,
       1,     4,     3,     3,     5,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     4,     1,     3,     0,     1,     3,     0,
       0,     6,     1,     1,     1,     3,     3,     2,     4,     3,
       1,     2,     1,     1,     1,     1,     0,     0,     6,     4,
       5,     0,     9,     4,     2,     2,     3,     2,     3,     2,
       2,     3,     3,     3,     2,     0,     0,     6,     2,     0,
       0,     6,     0,     0,     6,     0,     0,     6,     1,     0,
       0,     6,     0,     0,     7,     1,     0,     0,     6,     0,
       0,     7,     1,     0,     0,     6,     0,     0,     7,     0,
       0,     6,     0,     0,     6,     1,     3,     3,     0,     0,
       0,     9,     0,     0,     0,     9,     0,     0,     0,    10,
       1,     1,     1,     1,     1,     3,     3,     5,     5,     6,
       6,     8,     8,     1,     1,     3,     5,     1,     2,     1,
       0,     0,     1,     0,     0,    10,     0,     0,    10,     0,
       0,     9,     0,     0,     7,     3,     1,     5,     0,     0,
      10,     4,     0,     0,    11,     0,     0,    11,     0,     0,
      10,     5,     0,     0,     9,     5,     0,     0,    10,     1,
       3,     4,     5,     7,     9,     0,     3,     0,     1,     9,
      10,     9
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

    case YYSYMBOL_let_variable_declaration: /* let_variable_declaration  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_global_let_variable_declaration: /* global_let_variable_declaration  */
            { delete ((*yyvaluep).pVarDecl); }
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
            das2_yyerror(scanner,"this module already has a name " + yyextra->g_Program->thisModule->name,tokAt(scanner,(yylsp[-2])),
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
        if ( err ) das2_yyerror(scanner,"invalid escape sequence",tokAt(scanner,(yylsp[-1])), CompilationError::invalid_escape_sequence);
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
            das2_yyend_reader(scanner);
        }
    }
    break;

  case 34: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das2_yyend_reader(scanner);
        }
    }
    break;

  case 35: /* $@1: %empty  */
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

  case 36: /* expr_reader: '%' name_in_namespace $@1 reader_character_sequence  */
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

  case 37: /* options_declaration: "options" annotation_argument_list  */
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

  case 39: /* require_module_name: "name"  */
                   {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 40: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 41: /* require_module_name: require_module_name '.' "name"  */
                                                {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 42: /* require_module_name: require_module_name '/' "name"  */
                                                {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 43: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 44: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 45: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 46: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 50: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 51: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 52: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 53: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 54: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 55: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 56: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 57: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 58: /* expression_else: "else" expression_block  */
                                                           { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 59: /* expression_else: elif_or_static_elif '(' expr ')' expression_block expression_else  */
                                                                                                  {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-3].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-5].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 60: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 61: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 62: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 63: /* expression_else_one_liner: "else" expression_if_one_liner  */
                                                      {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 64: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 65: /* expression_if_one_liner: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 66: /* expression_if_one_liner: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 67: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 68: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 69: /* expression_if_then_else: if_or_static_if '(' expr ')' expression_block expression_else  */
                                                                                              {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-3].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-5].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 70: /* expression_if_then_else: expression_if_one_liner "if" '(' expr ')' expression_else_one_liner "end of expression"  */
                                                                                                                {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr(ast_wrapInBlock((yyvsp[-6].pExpression))),(yyvsp[-1].pExpression) ? ExpressionPtr(ast_wrapInBlock((yyvsp[-1].pExpression))) : nullptr);
    }
    break;

  case 71: /* expression_for_loop: "for" '(' variable_name_with_pos_list "in" expr_list ')' expression_block  */
                                                                                                               {
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-4].pNameWithPosList),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 72: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 73: /* expression_while_loop: "while" '(' expr ')' expression_block  */
                                                                       {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-4])));
        pWhile->cond = ExpressionPtr((yyvsp[-2].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 74: /* expression_with: "with" '(' expr ')' expression_block  */
                                                                 {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-4])));
        pWith->with = ExpressionPtr((yyvsp[-2].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
    break;

  case 75: /* expression_with_alias: "assume" "name" '=' expr  */
                                                      {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-3])), *(yyvsp[-2].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-2].s);
    }
    break;

  case 76: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 77: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 78: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 79: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 80: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 81: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 82: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 83: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 84: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 85: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 86: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 87: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 88: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 89: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 90: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 91: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 92: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 93: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 94: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 95: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 96: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 97: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 98: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
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

  case 103: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 104: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 105: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 106: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 107: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 108: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 210: /* $@2: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 211: /* function_declaration: optional_public_or_private_function $@2 function_declaration_header expression_block  */
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

  case 212: /* expression_block: "begin of code block" expressions "end of code block"  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 213: /* expression_block: "begin of code block" expressions "end of code block" "finally" "begin of code block" expressions "end of code block"  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 214: /* expr_call_pipe: expr_call expr_full_block_assumed_piped  */
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

  case 215: /* expression_any: "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 216: /* expression_any: expr_assign "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 217: /* expression_any: expression_delete "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 218: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 219: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 220: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 221: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 222: /* expression_any: expression_with_alias "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 223: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 224: /* expression_any: expression_break "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 225: /* expression_any: expression_continue "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 226: /* expression_any: expression_return "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 227: /* expression_any: expression_yield "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 228: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 229: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 230: /* expression_any: expression_label "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 231: /* expression_any: expression_goto "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 232: /* expression_any: "pass" "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 233: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 234: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 235: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 236: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 237: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 238: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 239: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 240: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 241: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 242: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 243: /* name_in_namespace: "name" "::" "name"  */
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

  case 244: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 245: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 246: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 247: /* $@3: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 248: /* $@4: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 249: /* new_type_declaration: '<' $@3 type_declaration '>' $@4  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 250: /* expr_new: "new" structure_type_declaration  */
                                                            {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),true);
    }
    break;

  case 251: /* expr_new: "new" structure_type_declaration '(' optional_expr_list ')'  */
                                                                                                   {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 252: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 253: /* expr_new: "new" new_type_declaration '(' use_initializer make_struct_single ')'  */
                                                                                                            {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 254: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 255: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 256: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 257: /* expression_return: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 258: /* expression_return: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 259: /* expression_return: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 260: /* expression_yield: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 261: /* expression_yield: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 262: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 263: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 264: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 265: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 266: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 267: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 268: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 269: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 270: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 271: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 272: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 273: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 274: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 275: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 276: /* $@5: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 277: /* $@6: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 278: /* expr_cast: "cast" '<' $@5 type_declaration_no_options '>' $@6 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 279: /* $@7: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 280: /* $@8: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 281: /* expr_cast: "upcast" '<' $@7 type_declaration_no_options '>' $@8 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 282: /* $@9: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 283: /* $@10: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 284: /* expr_cast: "reinterpret" '<' $@9 type_declaration_no_options '>' $@10 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 285: /* $@11: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 286: /* $@12: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 287: /* expr_type_decl: "type" '<' $@11 type_declaration '>' $@12  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 288: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 289: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 290: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" c_or_s "name" '>' '(' expr ')'  */
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

  case 291: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 292: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 293: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 294: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 295: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 296: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 297: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 298: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 299: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 300: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 301: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 302: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 303: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 304: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 305: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 306: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 307: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 308: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 309: /* expr_full_block_assumed_piped: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 310: /* expr_full_block_assumed_piped: "begin of code block" expressions "end of code block"  */
                                   {
        (yyval.pExpression) = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,new TypeDecl(Type::autoinfer),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[-1])));
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

  case 337: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 338: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 339: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 340: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 341: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 342: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 343: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 344: /* $@13: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 345: /* $@14: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 346: /* func_addr_expr: '@' '@' '<' $@13 type_declaration_no_options '>' $@14 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 347: /* $@15: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 348: /* $@16: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 349: /* func_addr_expr: '@' '@' '<' $@15 optional_function_argument_list optional_function_type '>' $@16 func_addr_name  */
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

  case 350: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 351: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 352: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 353: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 354: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 355: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 356: /* $@17: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 357: /* $@18: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 358: /* expr_field: expr '.' $@17 error $@18  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 359: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 360: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 361: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 362: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
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

  case 363: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 364: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 365: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 366: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 367: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 368: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 369: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 370: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 371: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 372: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 373: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 374: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 375: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 376: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 392: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 393: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 394: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 395: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 396: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 397: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 398: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 399: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 400: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 401: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 402: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 403: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 404: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 405: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 406: /* expr: '(' expr_list optional_comma ')'  */
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

  case 407: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 408: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 409: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 410: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 411: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 412: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 413: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 414: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 415: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 416: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 417: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 418: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 419: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 420: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list expression_block  */
                                                                                                                              {
        auto closure = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        ((ExprBlock *)(yyvsp[0].pExpression))->returnType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),closure,tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 421: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 422: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 423: /* $@19: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 424: /* $@20: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 425: /* expr: expr "is" "type" '<' $@19 type_declaration_no_options '>' $@20  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 426: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 427: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 428: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 429: /* $@21: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 430: /* $@22: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 431: /* expr: expr "as" "type" '<' $@21 type_declaration '>' $@22  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 432: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 433: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 434: /* $@23: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 435: /* $@24: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 436: /* expr: expr '?' "as" "type" '<' $@23 type_declaration '>' $@24  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 437: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 438: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 439: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 440: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 441: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 442: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 443: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 444: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 445: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 446: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 447: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 448: /* expr: expr_call_pipe  */
                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 449: /* expr: "unsafe" '(' expr ')'  */
                                         {
            (yyvsp[-1].pExpression)->alwaysSafe = true;
            (yyvsp[-1].pExpression)->userSaidItsSafe = true;
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    break;

  case 450: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 451: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 452: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 453: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 454: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 455: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 456: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 457: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 458: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 459: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 460: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 461: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 462: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 463: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 464: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 465: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 466: /* optional_field_annotation: %empty  */
                                      { (yyval.aaList) = nullptr; }
    break;

  case 467: /* optional_field_annotation: metadata_argument_list  */
                                      { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 468: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 469: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 470: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 471: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 472: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 473: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 474: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 475: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 476: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 477: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 478: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 479: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 480: /* $@25: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 481: /* struct_variable_declaration_list: struct_variable_declaration_list $@25 structure_variable_declaration "end of expression"  */
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

  case 482: /* $@26: %empty  */
                                                                                                                     {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 483: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@26 function_declaration_header "end of expression"  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 484: /* $@27: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 485: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@27 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 486: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 487: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 488: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 489: /* function_argument_list: function_argument_list "end of expression" function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 490: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 491: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 492: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 493: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 494: /* tuple_alias_type_list: %empty  */
      {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 495: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 496: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 497: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 498: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 499: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 500: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 501: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 502: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 503: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 504: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 505: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 506: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 507: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 508: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 509: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 510: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 511: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 512: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 513: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 514: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 515: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 516: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 517: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 518: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 519: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 520: /* global_let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 521: /* global_let_variable_name_with_pos_list: global_let_variable_name_with_pos_list ',' "name"  */
                                                                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 522: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options "end of expression"  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 523: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 524: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 525: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options "end of expression"  */
                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 526: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                         {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 527: /* global_let_variable_declaration: global_let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                                 {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 528: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 529: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 530: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 531: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 532: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 533: /* $@28: %empty  */
                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 534: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@28 optional_field_annotation global_let_variable_declaration  */
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

  case 535: /* enum_expression: "name"  */
                   {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 536: /* enum_expression: "name" '=' expr  */
                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[-2].s),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 537: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 538: /* enum_list: enum_expression  */
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

  case 539: /* enum_list: enum_list ',' enum_expression  */
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

  case 540: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 541: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 542: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 543: /* $@29: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 544: /* single_alias: optional_public_or_private_alias "name" $@29 '=' type_declaration  */
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

  case 546: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 547: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 548: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 549: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 550: /* optional_enum_basic_type_declaration: %empty  */
        {
        (yyval.type) = Type::tInt;
    }
    break;

  case 551: /* optional_enum_basic_type_declaration: ':' enum_basic_type_declaration  */
                                              {
        (yyval.type) = (yyvsp[0].type);
    }
    break;

  case 552: /* $@30: %empty  */
                                                                                                                                                          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 553: /* $@31: %empty  */
                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 554: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name optional_enum_basic_type_declaration "begin of code block" $@30 enum_list optional_comma $@31 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-3].pEnum),(yyvsp[-6].type));
    }
    break;

  case 555: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 556: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 557: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 558: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 559: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 560: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 561: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 562: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 563: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 564: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 565: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 566: /* optional_struct_variable_declaration_list: "begin of code block" struct_variable_declaration_list "end of code block"  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 567: /* $@32: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 568: /* $@33: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 569: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@32 structure_name $@33 optional_struct_variable_declaration_list  */
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

  case 570: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 571: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 572: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 573: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 574: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 575: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 576: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 577: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 578: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 579: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 580: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 581: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 582: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 583: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 584: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 585: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 586: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 587: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 588: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 589: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 590: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 591: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 592: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 593: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 594: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 595: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 596: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 597: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 598: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 599: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 600: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 601: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 602: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 603: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 604: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 605: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 606: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 607: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 608: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 609: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 610: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 611: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 612: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 613: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 614: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 615: /* bitfield_bits: bitfield_bits "end of expression" "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 616: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 617: /* bitfield_alias_bits: "name"  */
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

  case 618: /* bitfield_alias_bits: bitfield_alias_bits ',' "name"  */
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

  case 619: /* $@34: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 620: /* $@35: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 621: /* bitfield_type_declaration: "bitfield" '<' $@34 bitfield_bits '>' $@35  */
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

  case 624: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 625: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 626: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 627: /* dim_list: '[' ']'  */
                {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 628: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 629: /* dim_list: dim_list '[' ']'  */
                              {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 630: /* type_declaration_no_options: type_declaration_no_options_no_dim  */
                                                     {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 631: /* type_declaration_no_options: type_declaration_no_options_no_dim dim_list  */
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

  case 632: /* type_declaration_no_options_no_dim: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 633: /* type_declaration_no_options_no_dim: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 634: /* type_declaration_no_options_no_dim: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 635: /* type_declaration_no_options_no_dim: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 636: /* $@36: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 637: /* $@37: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 638: /* type_declaration_no_options_no_dim: "type" '<' $@36 type_declaration '>' $@37  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 639: /* type_declaration_no_options_no_dim: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 640: /* type_declaration_no_options_no_dim: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 641: /* $@38: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 642: /* type_declaration_no_options_no_dim: '$' name_in_namespace '<' $@38 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 643: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' '[' ']'  */
                                                                 {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 644: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "explicit"  */
                                                                  {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 645: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "const"  */
                                                               {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 646: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' "const"  */
                                                                   {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 647: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '&'  */
                                                         {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 648: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' '&'  */
                                                             {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 649: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '#'  */
                                                         {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 650: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "implicit"  */
                                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 651: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' '#'  */
                                                             {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 652: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "==" "const"  */
                                                                      {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 653: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "==" '&'  */
                                                                {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 654: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '?'  */
                                                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 655: /* $@39: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 656: /* $@40: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 657: /* type_declaration_no_options_no_dim: "smart_ptr" '<' $@39 type_declaration '>' $@40  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 658: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "??"  */
                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 659: /* $@41: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 660: /* $@42: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 661: /* type_declaration_no_options_no_dim: "array" '<' $@41 type_declaration '>' $@42  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 662: /* $@43: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 663: /* $@44: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 664: /* type_declaration_no_options_no_dim: "table" '<' $@43 table_type_pair '>' $@44  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 665: /* $@45: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 666: /* $@46: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 667: /* type_declaration_no_options_no_dim: "iterator" '<' $@45 type_declaration '>' $@46  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 668: /* type_declaration_no_options_no_dim: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 669: /* $@47: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 670: /* $@48: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 671: /* type_declaration_no_options_no_dim: "block" '<' $@47 type_declaration '>' $@48  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 672: /* $@49: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 673: /* $@50: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 674: /* type_declaration_no_options_no_dim: "block" '<' $@49 optional_function_argument_list optional_function_type '>' $@50  */
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

  case 675: /* type_declaration_no_options_no_dim: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 676: /* $@51: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 677: /* $@52: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 678: /* type_declaration_no_options_no_dim: "function" '<' $@51 type_declaration '>' $@52  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 679: /* $@53: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 680: /* $@54: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 681: /* type_declaration_no_options_no_dim: "function" '<' $@53 optional_function_argument_list optional_function_type '>' $@54  */
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

  case 682: /* type_declaration_no_options_no_dim: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 683: /* $@55: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 684: /* $@56: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 685: /* type_declaration_no_options_no_dim: "lambda" '<' $@55 type_declaration '>' $@56  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 686: /* $@57: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 687: /* $@58: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 688: /* type_declaration_no_options_no_dim: "lambda" '<' $@57 optional_function_argument_list optional_function_type '>' $@58  */
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

  case 689: /* $@59: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 690: /* $@60: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 691: /* type_declaration_no_options_no_dim: "tuple" '<' $@59 tuple_type_list '>' $@60  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 692: /* $@61: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 693: /* $@62: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 694: /* type_declaration_no_options_no_dim: "variant" '<' $@61 variant_type_list '>' $@62  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 695: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 696: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 697: /* type_declaration: type_declaration '|' '#'  */
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

  case 698: /* $@63: %empty  */
                                                                      {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 699: /* $@64: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 700: /* $@65: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 701: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias "name" $@63 "begin of code block" $@64 tuple_alias_type_list $@65 "end of code block"  */
          {
        auto vtype = make_smart<TypeDecl>(Type::tTuple);
        vtype->alias = *(yyvsp[-6].s);
        vtype->at = tokAt(scanner,(yylsp[-6]));
        vtype->isPrivateAlias = !(yyvsp[-7].b);
        varDeclToTypeDecl(scanner, vtype.get(), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-6].s),tokAt(scanner,(yylsp[-6])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTuple((yyvsp[-6].s)->c_str(),atvname);
        }
        delete (yyvsp[-6].s);
    }
    break;

  case 702: /* $@66: %empty  */
                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 703: /* $@67: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 704: /* $@68: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 705: /* variant_alias_declaration: "variant" optional_public_or_private_alias "name" $@66 "begin of code block" $@67 variant_alias_type_list $@68 "end of code block"  */
          {
        auto vtype = make_smart<TypeDecl>(Type::tVariant);
        vtype->alias = *(yyvsp[-6].s);
        vtype->at = tokAt(scanner,(yylsp[-6]));
        vtype->isPrivateAlias = !(yyvsp[-7].b);
        varDeclToTypeDecl(scanner, vtype.get(), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-6].s),tokAt(scanner,(yylsp[-6])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariant((yyvsp[-6].s)->c_str(),atvname);
        }
        delete (yyvsp[-6].s);
    }
    break;

  case 706: /* $@69: %empty  */
                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 707: /* $@70: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 708: /* $@71: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 709: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias "name" $@69 "begin of code block" $@70 bitfield_alias_bits optional_comma $@71 "end of code block"  */
          {
        auto btype = make_smart<TypeDecl>(Type::tBitfield);
        btype->alias = *(yyvsp[-7].s);
        btype->at = tokAt(scanner,(yylsp[-7]));
        btype->argNames = *(yyvsp[-3].pNameList);
        btype->isPrivateAlias = !(yyvsp[-8].b);
        if ( btype->argNames.size()>32 ) {
            das2_yyerror(scanner,"only 32 different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-7])),
                CompilationError::invalid_type);
        }
        if ( !yyextra->g_Program->addAlias(btype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-7].s),tokAt(scanner,(yylsp[-7])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfield((yyvsp[-7].s)->c_str(),atvname);
        }
        delete (yyvsp[-7].s);
        delete (yyvsp[-3].pNameList);
    }
    break;

  case 710: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 711: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 712: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 713: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 714: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 715: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 716: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 717: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 718: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 719: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 720: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 721: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 722: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 723: /* make_variant_dim: make_struct_fields  */
                                {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 724: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 725: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 726: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 727: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 728: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 729: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 730: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 731: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 732: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 733: /* $@72: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 734: /* $@73: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 735: /* make_struct_decl: "struct" '<' $@72 type_declaration_no_options '>' $@73 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 736: /* $@74: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 737: /* $@75: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 738: /* make_struct_decl: "class" '<' $@74 type_declaration_no_options '>' $@75 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 739: /* $@76: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 740: /* $@77: %empty  */
                                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 741: /* make_struct_decl: "variant" '<' $@76 type_declaration_no_options '>' $@77 '(' make_variant_dim ')'  */
                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 742: /* $@78: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 743: /* $@79: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 744: /* make_struct_decl: "default" '<' $@78 type_declaration_no_options '>' $@79 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 745: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 746: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 747: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 748: /* $@80: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 749: /* $@81: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 750: /* make_tuple_call: "tuple" '<' $@80 type_declaration_no_options '>' $@81 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 751: /* make_dim_decl: '[' expr_list optional_comma ']'  */
                                                          {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 752: /* $@82: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 753: /* $@83: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 754: /* make_dim_decl: "array" "struct" '<' $@82 type_declaration_no_options '>' $@83 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 755: /* $@84: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 756: /* $@85: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 757: /* make_dim_decl: "array" "tuple" '<' $@84 type_declaration_no_options '>' $@85 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 758: /* $@86: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 759: /* $@87: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 760: /* make_dim_decl: "array" "variant" '<' $@86 type_declaration_no_options '>' $@87 '(' make_variant_dim ')'  */
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

  case 761: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 762: /* $@88: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 763: /* $@89: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 764: /* make_dim_decl: "array" '<' $@88 type_declaration_no_options '>' $@89 '(' optional_expr_list ')'  */
                                                                                                                                                                        {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-8])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
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

  case 765: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 766: /* $@90: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 767: /* $@91: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 768: /* make_dim_decl: "fixed_array" '<' $@90 type_declaration_no_options '>' $@91 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        if ( !mka->makeType->dim.size() ) mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 769: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 770: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 771: /* make_table_decl: "begin of code block" expr_map_tuple_list optional_comma "end of code block"  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 772: /* make_table_decl: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 773: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 774: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 775: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 776: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 777: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 778: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 779: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 780: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 781: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
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


