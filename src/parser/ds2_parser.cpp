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
  YYSYMBOL_type_declaration_no_options_list = 268, /* type_declaration_no_options_list  */
  YYSYMBOL_name_in_namespace = 269,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 270,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 271,     /* new_type_declaration  */
  YYSYMBOL_272_3 = 272,                    /* $@3  */
  YYSYMBOL_273_4 = 273,                    /* $@4  */
  YYSYMBOL_expr_new = 274,                 /* expr_new  */
  YYSYMBOL_expression_break = 275,         /* expression_break  */
  YYSYMBOL_expression_continue = 276,      /* expression_continue  */
  YYSYMBOL_expression_return = 277,        /* expression_return  */
  YYSYMBOL_expression_yield = 278,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 279,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 280,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 281,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 282,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 283,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 284, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 285,           /* expression_let  */
  YYSYMBOL_expr_cast = 286,                /* expr_cast  */
  YYSYMBOL_287_5 = 287,                    /* $@5  */
  YYSYMBOL_288_6 = 288,                    /* $@6  */
  YYSYMBOL_289_7 = 289,                    /* $@7  */
  YYSYMBOL_290_8 = 290,                    /* $@8  */
  YYSYMBOL_291_9 = 291,                    /* $@9  */
  YYSYMBOL_292_10 = 292,                   /* $@10  */
  YYSYMBOL_expr_type_decl = 293,           /* expr_type_decl  */
  YYSYMBOL_294_11 = 294,                   /* $@11  */
  YYSYMBOL_295_12 = 295,                   /* $@12  */
  YYSYMBOL_expr_type_info = 296,           /* expr_type_info  */
  YYSYMBOL_expr_list = 297,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 298,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 299,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 300,            /* capture_entry  */
  YYSYMBOL_capture_list = 301,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 302,    /* optional_capture_list  */
  YYSYMBOL_expr_full_block = 303,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 304, /* expr_full_block_assumed_piped  */
  YYSYMBOL_expr_numeric_const = 305,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 306,              /* expr_assign  */
  YYSYMBOL_expr_named_call = 307,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 308,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 309,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 310,           /* func_addr_expr  */
  YYSYMBOL_311_13 = 311,                   /* $@13  */
  YYSYMBOL_312_14 = 312,                   /* $@14  */
  YYSYMBOL_313_15 = 313,                   /* $@15  */
  YYSYMBOL_314_16 = 314,                   /* $@16  */
  YYSYMBOL_expr_field = 315,               /* expr_field  */
  YYSYMBOL_316_17 = 316,                   /* $@17  */
  YYSYMBOL_317_18 = 317,                   /* $@18  */
  YYSYMBOL_expr_call = 318,                /* expr_call  */
  YYSYMBOL_expr = 319,                     /* expr  */
  YYSYMBOL_320_19 = 320,                   /* $@19  */
  YYSYMBOL_321_20 = 321,                   /* $@20  */
  YYSYMBOL_322_21 = 322,                   /* $@21  */
  YYSYMBOL_323_22 = 323,                   /* $@22  */
  YYSYMBOL_324_23 = 324,                   /* $@23  */
  YYSYMBOL_325_24 = 325,                   /* $@24  */
  YYSYMBOL_expr_mtag = 326,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 327, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 328,        /* optional_override  */
  YYSYMBOL_optional_constant = 329,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 330, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 331, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 332, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 333, /* struct_variable_declaration_list  */
  YYSYMBOL_334_25 = 334,                   /* $@25  */
  YYSYMBOL_335_26 = 335,                   /* $@26  */
  YYSYMBOL_336_27 = 336,                   /* $@27  */
  YYSYMBOL_function_argument_declaration = 337, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 338,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 339,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 340,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 341,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 342,             /* variant_type  */
  YYSYMBOL_variant_type_list = 343,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 344,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 345,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 346,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 347,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 348,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 349, /* let_variable_name_with_pos_list  */
  YYSYMBOL_global_let_variable_name_with_pos_list = 350, /* global_let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 351, /* let_variable_declaration  */
  YYSYMBOL_global_let_variable_declaration = 352, /* global_let_variable_declaration  */
  YYSYMBOL_optional_shared = 353,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 354, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 355,               /* global_let  */
  YYSYMBOL_356_28 = 356,                   /* $@28  */
  YYSYMBOL_enum_expression = 357,          /* enum_expression  */
  YYSYMBOL_enum_list = 358,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 359, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 360,             /* single_alias  */
  YYSYMBOL_361_29 = 361,                   /* $@29  */
  YYSYMBOL_alias_declaration = 362,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 363, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 364,                /* enum_name  */
  YYSYMBOL_optional_enum_basic_type_declaration = 365, /* optional_enum_basic_type_declaration  */
  YYSYMBOL_enum_declaration = 366,         /* enum_declaration  */
  YYSYMBOL_367_30 = 367,                   /* $@30  */
  YYSYMBOL_368_31 = 368,                   /* $@31  */
  YYSYMBOL_optional_structure_parent = 369, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 370,          /* optional_sealed  */
  YYSYMBOL_structure_name = 371,           /* structure_name  */
  YYSYMBOL_class_or_struct = 372,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 373, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 374, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 375,    /* structure_declaration  */
  YYSYMBOL_376_32 = 376,                   /* $@32  */
  YYSYMBOL_377_33 = 377,                   /* $@33  */
  YYSYMBOL_variable_name_with_pos_list = 378, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 379,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 380, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 381, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 382,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 383,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 384,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 385, /* bitfield_type_declaration  */
  YYSYMBOL_386_34 = 386,                   /* $@34  */
  YYSYMBOL_387_35 = 387,                   /* $@35  */
  YYSYMBOL_table_type_pair = 388,          /* table_type_pair  */
  YYSYMBOL_dim_list = 389,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 390, /* type_declaration_no_options  */
  YYSYMBOL_type_declaration_no_options_no_dim = 391, /* type_declaration_no_options_no_dim  */
  YYSYMBOL_392_36 = 392,                   /* $@36  */
  YYSYMBOL_393_37 = 393,                   /* $@37  */
  YYSYMBOL_394_38 = 394,                   /* $@38  */
  YYSYMBOL_395_39 = 395,                   /* $@39  */
  YYSYMBOL_396_40 = 396,                   /* $@40  */
  YYSYMBOL_397_41 = 397,                   /* $@41  */
  YYSYMBOL_398_42 = 398,                   /* $@42  */
  YYSYMBOL_399_43 = 399,                   /* $@43  */
  YYSYMBOL_400_44 = 400,                   /* $@44  */
  YYSYMBOL_401_45 = 401,                   /* $@45  */
  YYSYMBOL_402_46 = 402,                   /* $@46  */
  YYSYMBOL_403_47 = 403,                   /* $@47  */
  YYSYMBOL_404_48 = 404,                   /* $@48  */
  YYSYMBOL_405_49 = 405,                   /* $@49  */
  YYSYMBOL_406_50 = 406,                   /* $@50  */
  YYSYMBOL_407_51 = 407,                   /* $@51  */
  YYSYMBOL_408_52 = 408,                   /* $@52  */
  YYSYMBOL_409_53 = 409,                   /* $@53  */
  YYSYMBOL_410_54 = 410,                   /* $@54  */
  YYSYMBOL_411_55 = 411,                   /* $@55  */
  YYSYMBOL_412_56 = 412,                   /* $@56  */
  YYSYMBOL_413_57 = 413,                   /* $@57  */
  YYSYMBOL_414_58 = 414,                   /* $@58  */
  YYSYMBOL_415_59 = 415,                   /* $@59  */
  YYSYMBOL_416_60 = 416,                   /* $@60  */
  YYSYMBOL_417_61 = 417,                   /* $@61  */
  YYSYMBOL_418_62 = 418,                   /* $@62  */
  YYSYMBOL_type_declaration = 419,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 420,  /* tuple_alias_declaration  */
  YYSYMBOL_421_63 = 421,                   /* $@63  */
  YYSYMBOL_422_64 = 422,                   /* $@64  */
  YYSYMBOL_423_65 = 423,                   /* $@65  */
  YYSYMBOL_variant_alias_declaration = 424, /* variant_alias_declaration  */
  YYSYMBOL_425_66 = 425,                   /* $@66  */
  YYSYMBOL_426_67 = 426,                   /* $@67  */
  YYSYMBOL_427_68 = 427,                   /* $@68  */
  YYSYMBOL_bitfield_alias_declaration = 428, /* bitfield_alias_declaration  */
  YYSYMBOL_429_69 = 429,                   /* $@69  */
  YYSYMBOL_430_70 = 430,                   /* $@70  */
  YYSYMBOL_431_71 = 431,                   /* $@71  */
  YYSYMBOL_make_decl = 432,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 433,       /* make_struct_fields  */
  YYSYMBOL_make_struct_single = 434,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 435,          /* make_struct_dim  */
  YYSYMBOL_use_initializer = 436,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 437,         /* make_struct_decl  */
  YYSYMBOL_438_72 = 438,                   /* $@72  */
  YYSYMBOL_439_73 = 439,                   /* $@73  */
  YYSYMBOL_440_74 = 440,                   /* $@74  */
  YYSYMBOL_441_75 = 441,                   /* $@75  */
  YYSYMBOL_442_76 = 442,                   /* $@76  */
  YYSYMBOL_443_77 = 443,                   /* $@77  */
  YYSYMBOL_444_78 = 444,                   /* $@78  */
  YYSYMBOL_445_79 = 445,                   /* $@79  */
  YYSYMBOL_make_map_tuple = 446,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 447,          /* make_tuple_call  */
  YYSYMBOL_448_80 = 448,                   /* $@80  */
  YYSYMBOL_449_81 = 449,                   /* $@81  */
  YYSYMBOL_make_dim_decl = 450,            /* make_dim_decl  */
  YYSYMBOL_451_82 = 451,                   /* $@82  */
  YYSYMBOL_452_83 = 452,                   /* $@83  */
  YYSYMBOL_453_84 = 453,                   /* $@84  */
  YYSYMBOL_454_85 = 454,                   /* $@85  */
  YYSYMBOL_455_86 = 455,                   /* $@86  */
  YYSYMBOL_456_87 = 456,                   /* $@87  */
  YYSYMBOL_457_88 = 457,                   /* $@88  */
  YYSYMBOL_458_89 = 458,                   /* $@89  */
  YYSYMBOL_459_90 = 459,                   /* $@90  */
  YYSYMBOL_460_91 = 460,                   /* $@91  */
  YYSYMBOL_expr_map_tuple_list = 461,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 462,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 463, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 464,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 465       /* array_comprehension  */
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
#define YYLAST   10650

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  210
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  256
/* YYNRULES -- Number of rules.  */
#define YYNRULES  765
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1405

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
       0,   529,   529,   530,   535,   536,   537,   538,   539,   540,
     541,   542,   543,   544,   545,   546,   547,   551,   557,   558,
     559,   563,   564,   568,   586,   587,   588,   589,   593,   597,
     602,   611,   619,   635,   640,   648,   648,   687,   705,   709,
     712,   716,   722,   731,   734,   740,   741,   745,   749,   750,
     754,   757,   763,   769,   772,   778,   779,   783,   784,   785,
     794,   795,   799,   800,   806,   807,   808,   809,   810,   814,
     820,   826,   832,   840,   850,   859,   866,   867,   868,   869,
     870,   871,   875,   880,   888,   889,   890,   894,   895,   896,
     897,   898,   899,   900,   901,   907,   910,   916,   919,   925,
     926,   927,   931,   944,   962,   965,   973,   984,   995,  1006,
    1009,  1016,  1020,  1027,  1028,  1032,  1033,  1034,  1038,  1041,
    1048,  1052,  1053,  1054,  1055,  1056,  1057,  1058,  1059,  1060,
    1061,  1062,  1063,  1064,  1065,  1066,  1067,  1068,  1069,  1070,
    1071,  1072,  1073,  1074,  1075,  1076,  1077,  1078,  1079,  1080,
    1081,  1082,  1083,  1084,  1085,  1086,  1087,  1088,  1089,  1090,
    1091,  1092,  1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,
    1101,  1102,  1103,  1104,  1105,  1106,  1107,  1108,  1109,  1110,
    1111,  1112,  1113,  1114,  1115,  1116,  1117,  1118,  1119,  1120,
    1121,  1122,  1123,  1124,  1125,  1126,  1127,  1128,  1129,  1130,
    1131,  1132,  1133,  1134,  1135,  1140,  1158,  1159,  1160,  1164,
    1170,  1170,  1187,  1191,  1202,  1215,  1216,  1217,  1218,  1219,
    1220,  1221,  1222,  1223,  1224,  1225,  1226,  1227,  1228,  1229,
    1230,  1231,  1232,  1236,  1241,  1247,  1253,  1254,  1258,  1262,
    1269,  1270,  1281,  1285,  1288,  1296,  1296,  1296,  1302,  1305,
    1309,  1313,  1320,  1326,  1330,  1334,  1337,  1340,  1348,  1351,
    1359,  1365,  1366,  1367,  1371,  1372,  1376,  1377,  1381,  1386,
    1394,  1400,  1412,  1415,  1421,  1421,  1421,  1424,  1424,  1424,
    1429,  1429,  1429,  1437,  1437,  1437,  1443,  1453,  1464,  1479,
    1482,  1488,  1489,  1496,  1507,  1508,  1509,  1513,  1514,  1515,
    1516,  1520,  1525,  1533,  1534,  1538,  1545,  1549,  1555,  1556,
    1557,  1558,  1559,  1560,  1561,  1565,  1566,  1567,  1568,  1569,
    1570,  1571,  1572,  1573,  1574,  1575,  1576,  1577,  1578,  1579,
    1580,  1581,  1582,  1583,  1587,  1594,  1606,  1611,  1621,  1625,
    1632,  1635,  1635,  1635,  1640,  1640,  1640,  1653,  1657,  1661,
    1666,  1673,  1673,  1673,  1680,  1684,  1693,  1697,  1700,  1706,
    1707,  1708,  1709,  1710,  1711,  1712,  1713,  1714,  1715,  1716,
    1717,  1718,  1719,  1720,  1721,  1722,  1723,  1724,  1725,  1726,
    1727,  1728,  1729,  1730,  1731,  1732,  1733,  1734,  1735,  1736,
    1737,  1738,  1739,  1740,  1741,  1747,  1748,  1749,  1750,  1751,
    1764,  1765,  1766,  1767,  1768,  1769,  1770,  1771,  1772,  1773,
    1774,  1775,  1778,  1781,  1782,  1785,  1785,  1785,  1788,  1793,
    1797,  1801,  1801,  1801,  1806,  1809,  1813,  1813,  1813,  1818,
    1821,  1822,  1823,  1824,  1825,  1826,  1827,  1828,  1829,  1831,
    1835,  1839,  1840,  1841,  1842,  1843,  1844,  1845,  1849,  1853,
    1857,  1861,  1865,  1869,  1873,  1877,  1881,  1888,  1889,  1893,
    1894,  1895,  1899,  1900,  1904,  1905,  1906,  1910,  1911,  1915,
    1926,  1929,  1929,  1948,  1947,  1961,  1960,  1976,  1985,  1995,
    1996,  2000,  2003,  2012,  2013,  2017,  2020,  2023,  2039,  2048,
    2049,  2053,  2056,  2059,  2073,  2074,  2078,  2084,  2090,  2093,
    2097,  2106,  2107,  2108,  2112,  2113,  2117,  2124,  2129,  2138,
    2144,  2155,  2162,  2172,  2175,  2180,  2191,  2194,  2199,  2211,
    2212,  2216,  2217,  2218,  2222,  2222,  2240,  2244,  2251,  2254,
    2267,  2284,  2285,  2286,  2291,  2291,  2317,  2321,  2322,  2323,
    2327,  2337,  2340,  2346,  2351,  2346,  2366,  2367,  2371,  2372,
    2376,  2382,  2383,  2387,  2388,  2389,  2393,  2396,  2402,  2407,
    2402,  2421,  2428,  2433,  2442,  2448,  2459,  2460,  2461,  2462,
    2463,  2464,  2465,  2466,  2467,  2468,  2469,  2470,  2471,  2472,
    2473,  2474,  2475,  2476,  2477,  2478,  2479,  2480,  2481,  2482,
    2483,  2484,  2485,  2489,  2490,  2491,  2492,  2493,  2494,  2495,
    2496,  2500,  2511,  2515,  2522,  2534,  2541,  2550,  2555,  2565,
    2578,  2578,  2578,  2591,  2595,  2602,  2606,  2610,  2614,  2621,
    2624,  2642,  2643,  2644,  2645,  2646,  2646,  2646,  2650,  2655,
    2662,  2662,  2669,  2673,  2677,  2682,  2687,  2692,  2697,  2701,
    2705,  2710,  2714,  2718,  2723,  2723,  2723,  2729,  2736,  2736,
    2736,  2741,  2741,  2741,  2747,  2747,  2747,  2752,  2756,  2756,
    2756,  2761,  2761,  2761,  2770,  2774,  2774,  2774,  2779,  2779,
    2779,  2788,  2792,  2792,  2792,  2797,  2797,  2797,  2806,  2806,
    2806,  2812,  2812,  2812,  2821,  2824,  2835,  2851,  2856,  2861,
    2851,  2886,  2891,  2897,  2886,  2922,  2927,  2932,  2922,  2962,
    2963,  2964,  2965,  2966,  2970,  2977,  2984,  2990,  2996,  3003,
    3010,  3016,  3026,  3034,  3039,  3046,  3047,  3051,  3051,  3051,
    3059,  3059,  3059,  3066,  3066,  3066,  3073,  3073,  3073,  3084,
    3090,  3096,  3102,  3102,  3102,  3112,  3120,  3120,  3120,  3130,
    3130,  3130,  3140,  3140,  3140,  3150,  3158,  3158,  3158,  3166,
    3173,  3173,  3173,  3183,  3186,  3192,  3200,  3208,  3216,  3229,
    3230,  3234,  3235,  3240,  3243,  3246
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
  "type_declaration_no_options_list", "name_in_namespace",
  "expression_delete", "new_type_declaration", "$@3", "$@4", "expr_new",
  "expression_break", "expression_continue", "expression_return",
  "expression_yield", "expression_try_catch", "kwd_let_var_or_nothing",
  "kwd_let", "optional_in_scope", "tuple_expansion",
  "tuple_expansion_variable_declaration", "expression_let", "expr_cast",
  "$@5", "$@6", "$@7", "$@8", "$@9", "$@10", "expr_type_decl", "$@11",
  "$@12", "expr_type_info", "expr_list", "block_or_simple_block",
  "block_or_lambda", "capture_entry", "capture_list",
  "optional_capture_list", "expr_full_block",
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
  "bitfield_type_declaration", "$@34", "$@35", "table_type_pair",
  "dim_list", "type_declaration_no_options",
  "type_declaration_no_options_no_dim", "$@36", "$@37", "$@38", "$@39",
  "$@40", "$@41", "$@42", "$@43", "$@44", "$@45", "$@46", "$@47", "$@48",
  "$@49", "$@50", "$@51", "$@52", "$@53", "$@54", "$@55", "$@56", "$@57",
  "$@58", "$@59", "$@60", "$@61", "$@62", "type_declaration",
  "tuple_alias_declaration", "$@63", "$@64", "$@65",
  "variant_alias_declaration", "$@66", "$@67", "$@68",
  "bitfield_alias_declaration", "$@69", "$@70", "$@71", "make_decl",
  "make_struct_fields", "make_struct_single", "make_struct_dim",
  "use_initializer", "make_struct_decl", "$@72", "$@73", "$@74", "$@75",
  "$@76", "$@77", "$@78", "$@79", "make_map_tuple", "make_tuple_call",
  "$@80", "$@81", "make_dim_decl", "$@82", "$@83", "$@84", "$@85", "$@86",
  "$@87", "$@88", "$@89", "$@90", "$@91", "expr_map_tuple_list",
  "make_table_decl", "array_comprehension_where", "optional_comma",
  "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1265)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-676)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1265,    22, -1265, -1265,    48,   -89,    25,   208, -1265,   -36,
     208,   208,   208, -1265,   251,    26, -1265, -1265,   -75, -1265,
   -1265, -1265,   137, -1265,    61, -1265, -1265, -1265, -1265, -1265,
   -1265, -1265, -1265, -1265, -1265,   -74, -1265,    75,   -37,   155,
   -1265, -1265,    25,    17, -1265, -1265, -1265,     0,    28, -1265,
   -1265,    61,   206,   240,   319,   322,   170, -1265, -1265, -1265,
      26,    26,    26,   321, -1265,   519,    24, -1265, -1265, -1265,
   -1265,   221,   343,   451, -1265,   476,    21,    48,   387,   -89,
     375,   391, -1265,   431,   436, -1265, -1265, -1265,   481, -1265,
   -1265, -1265, -1265,   439,   423, -1265, -1265,   -48,    48,    26,
      26,    26,    26, -1265, -1265, -1265, -1265, -1265, -1265, -1265,
     446, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,
   -1265, -1265,   263,    92, -1265, -1265, -1265, -1265,   542, -1265,
   -1265,   432, -1265, -1265, -1265,   443,   444,   450, -1265, -1265,
     478, -1265,    40, -1265,   425,   496,   519, 10488, -1265,   464,
     540,   440, -1265, -1265,   498, -1265, -1265, -1265, -1265, -1265,
   -1265, -1265,   122, -1265,  1585, -1265, -1265, -1265, -1265, -1265,
    9241, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,
   -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,
   -1265, -1265, -1265, -1265, -1265, -1265,   614,   633, -1265,   466,
     502,   344,   503, -1265,   512, -1265,    48,   467,   514, -1265,
   -1265, -1265,    92, -1265,   492,   493,   495,   479,   500,   501,
   -1265, -1265, -1265,   482, -1265, -1265, -1265, -1265, -1265,   505,
   -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,
   -1265,   507, -1265, -1265, -1265,   511,   513, -1265, -1265, -1265,
   -1265,   515,   516,   483,   251, -1265, -1265, -1265, -1265, -1265,
   -1265,   534,   499,   521, -1265, -1265,   535,   537, -1265, -1265,
   -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,
   -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,   541,   506,
   -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,
   -1265, -1265,   680, -1265, -1265, -1265, -1265, -1265, -1265, -1265,
   -1265, -1265, -1265, -1265, -1265,   544,   508, -1265, -1265,   -97,
     523, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,
   -1265, -1265, -1265,   524,   539, -1265,    48, -1265,   328, -1265,
   -1265, -1265, -1265, -1265,  5959, -1265, -1265,   545, -1265,   -20,
     200,   230, -1265, -1265,  5959,    85, -1265, -1265, -1265,     8,
   -1265, -1265,     2, -1265,  3241, -1265,   510,  1301, -1265,   533,
    1440,   153, -1265, -1265, -1265, -1265,   552,   583, -1265,   517,
   -1265,    70, -1265,   -54,  1585, -1265,  1857,   554,   251, -1265,
   -1265, -1265, -1265,   555,  1585, -1265,    14,  1585,  1585,  1585,
     546,   547, -1265, -1265,    65,   251,   548,    29, -1265,   202,
     525,   549,   551,   536,   553,   229,   557, -1265,   271,   559,
     562,  5959,  5959,   556,   560,   561,   565,   568,   571, -1265,
   -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,  3437,  5959,
    5959,  5959,  5959,  5959,  2853,  5959, -1265,   518, -1265, -1265,
   -1265,   573, -1265, -1265, -1265, -1265,   526, -1265, -1265, -1265,
   -1265, -1265, -1265,    73,  6296, -1265,   574, -1265, -1265, -1265,
   -1265, -1265, -1265,  1585,  1585,   522,   558,  1585,   466,  1585,
     466,  1585,   466,  6302,   577,  6332, -1265,  5959, -1265, -1265,
   -1265, -1265,   576, -1265, -1265,  8753,  3631, -1265, -1265,   586,
   -1265,   -34, -1265,   578,   499,   585,   538, -1265,   588,   587,
   -1265, -1265,  5959, -1265, -1265,   174,   -91, -1265,   499, -1265,
     580, -1265, -1265,   582,  3825, -1265,   502,  4019,   584,   595,
   -1265,   590,   594,  4213,   502,  4407,   727, -1265,   609,   610,
     589,   778, -1265, -1265, -1265, -1265, -1265,   618, -1265,   619,
     620,   621,   623,   624, -1265,   731, -1265,   626,  9125,   622,
   -1265,   627, -1265,    19, -1265,   -31, -1265, -1265, -1265,  5959,
     243,   276,   613,   279, -1265, -1265, -1265,   597,   599, -1265,
     272, -1265,   615,   625,   628, -1265,  5959,  1585,  5959,  5959,
   -1265, -1265,  5959, -1265, -1265,  5959, -1265, -1265,  5959, -1265,
    1585,   453,   453,  5959,  5959,  5959,  5959,  5959,  5959,   487,
     174,  9272, -1265,   631,   453,   453,   -65,   453,   453,   174,
     799,   636,  9929,   636,   179,  3047,   811, -1265,   608,   526,
   -1265, 10310, 10398,  5959,  5959, -1265, -1265,  5959,  5959,  5959,
    5959,   656,  5959,   334,  5959,  5959,  5959,  5959,  5959,  5959,
    5959,  5959,  5959,  4601,  5959,  5959,  5959,  5959,  5959,  5959,
    5959,  5959,  5959,  5959,   -47,  5959, -1265,  4795,   369,   383,
   -1265, -1265,   268,   394,   523,   406,   523,   414,   523, -1265,
     292, -1265,   294, -1265,  1585,   611,   641, -1265, -1265, -1265,
    8784, -1265,   649,  1585, -1265, -1265,  1585, -1265, -1265,  6424,
     616,   779, -1265,   118, -1265,  5959,   174,  5959,  9929,   810,
    5959,  9929,  5959,   644, -1265,   645,   670,  9929, -1265,  5959,
    9929,   657, -1265, -1265,  5959,   629, -1265, -1265, -1265, -1265,
   -1265, -1265, -1265,   -78, -1265,  5959,  5959,  5959,  5959,  5959,
    5959,  5959,  5959,  5959,  5959,  5959,  5959,  5959,  5959,  5959,
    5959,  5959,  5959,  5959,   554, -1265, -1265,   820,   440, -1265,
    5959,  9371, -1265, -1265, -1265,  1585,  1585,  1585,  1585,   759,
    5959,   671,  5959,  1585, -1265, -1265, -1265,  1585,   636,   296,
     631,  6523,  1585,  1585,  6554,  1585,  1585,   636,  1585,  1585,
     636,  1585,   646,  6653,  6745,  6778,  6870,  6969,  7000, -1265,
    5959,   327,    13,  5959,  5959,   664,    18,   174,  5959,   632,
     634,   637,   638,   231, -1265, -1265,   642,   -16,  2657, -1265,
     145,   659,   635,   643,   466,  2062, -1265,   811,   653,   647,
   -1265, -1265,   654,   648, -1265, -1265,   372,   372,   158,   158,
    1584,  1584,   650,   176,   651, -1265,  8874,   -49,   -49,   574,
     372,   372, 10164,   899, 10132, 10018, 10431,  9460,   809,  1039,
   10246,   158,   158,   400,   400,   176,   176,   176,   345,  5959,
     652,   660,   351,  5959,   845,  8905, -1265,   146, -1265, -1265,
     687, -1265, -1265,   668, -1265,   672, -1265,   674,  6302, -1265,
     577, -1265,   300,   499, -1265,  5959, -1265, -1265,   499,   499,
   -1265,  5959,   688, -1265,   699, -1265,  1585, -1265,  5959,  7099,
      23,  9929,   502,  9929,  7191,  5959, -1265, -1265,  9929, -1265,
    7224,  5959,   661,   819,   702, -1265,   356, -1265,  9929,  9929,
    9929,  9929,  9929,  9929,  9929,  9929,  9929,  9929,  9929,  9929,
    9929,  9929,  9929,  9929,  9929,  9929,  9929, -1265,   696,   484,
     801,   697,  9492, -1265, -1265, -1265, -1265,   499,   684,   685,
     437, -1265,   -32,   666,   307,  7316,   455,  1585,  1585,  1585,
     689,   667,  1585,   673,   675, -1265,   690,   691, -1265,   693,
     694,   676,   698,   700,   677,   703,   811, -1265, -1265, -1265,
   -1265, -1265,   683,  9574,  5959,  9929, -1265, -1265,  5959,    30,
    9929, -1265, -1265,  5959,  5959,  1585,   466,  5959,  5959,  5959,
     165,  6153, -1265,   357, -1265,   -81,   523, -1265,   466, -1265,
    5959, -1265,  5959,  4989,  5959, -1265,   705,   686, -1265, -1265,
    5959,   695, -1265,  8995,  5959,  5183,   701, -1265,  9026, -1265,
   -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,
   -1265, -1265, -1265, -1265, -1265, -1265,  1585,   704,  7415, -1265,
     842,   -43,  9929,   502,  5959, -1265,   502,  9929,  2267,   502,
    7446,  5959,   732, -1265,   149,   734,  1585,    14, -1265, -1265,
   -1265,   283, -1265,    -6, -1265, -1265, -1265, -1265, -1265,    33,
   -1265,   692, -1265,   737,   706, -1265, -1265,   707,   715,   716,
   -1265, -1265,   717,  5959, -1265, -1265, -1265, -1265, -1265, -1265,
   -1265, -1265, -1265, -1265,   708,  5377, -1265,   378,   380,  5959,
    7545,  7637,   718,   523,  7670,  9929,  9929,   710,  2657,   709,
     130,   743,   746,   748,   750, -1265,   159,    87,   523,  1585,
    7762,  1585,  7861, -1265,   189,  7892, -1265,  5959, 10050,  5959,
   -1265,  7991, -1265,   195,  5959, -1265, -1265, -1265, -1265, -1265,
     499,  5959, -1265,   757,  5959, -1265,   197, -1265, -1265,   471,
     910,  8083, -1265,   761,   340,   887,     3,  5959,   902,    -6,
   -1265, -1265,   484,   736,   739, -1265, -1265,   760,  5959, -1265,
   -1265, -1265, -1265,   740,   742,   631,  5959,  5959,  5959,   744,
     759,   745,   747,  5571, -1265,   198,  5959,  5959,   407, -1265,
   -1265, -1265,   762,   135, -1265,   184,  5959,  5959,  5959, -1265,
   -1265, -1265, -1265,   -81, -1265,  5765, -1265, -1265,   502,   765,
   -1265,   457, -1265, -1265, -1265,  1585,  8116,  8208, -1265, -1265,
    8307,   751, -1265,  9929,   502,   502, -1265, -1265,   754, -1265,
    2461,   784, -1265, -1265,  1585,    14,   798, -1265,  5959,  9606,
   -1265, -1265,   902,   174,   759,   759,   756,  8338,   758,   763,
     764,  5959,  5959,   766,   158,   158,   158,  5959, -1265,   759,
     358, -1265,  8437, -1265,   790,  9688,  5959,   286, -1265,  5959,
    5959,   767,  8529,  9929,  9929, -1265,  5959, 10018, -1265, -1265,
   -1265,   458, -1265, -1265, -1265, -1265, -1265, -1265,  5959, -1265,
   -1265, -1265, -1265, -1265,  9929, -1265,    14,  5959, -1265,  9722,
   -1265, 10488, -1265, -1265,   358,   358,  5959, -1265,   759,   759,
     358,   636,   631, -1265,   636,   358,   659,    -2, -1265,   913,
     796,   770,  9688, -1265,   286,  9929,  9929, -1265,   140, 10050,
   -1265, -1265, -1265,  8562,  5959,  9804, -1265,   803, 10488,    43,
      57,  8654,   358,   358,    76,   771,   773,   781,   105,   358,
   -1265,  5959, -1265, -1265,   772, -1265,  5959,  5959, -1265,   502,
    9893, -1265, -1265,   502, -1265, -1265, -1265,   123,   126, -1265,
   -1265, -1265, -1265, -1265,   659,  9929, -1265,  9929,  9929,   471,
   -1265, -1265, -1265, -1265, -1265
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   113,     1,   264,     0,     0,     0,   531,   265,     0,
     531,   531,   531,    16,     0,     0,    15,     3,     0,    10,
       9,     8,     0,     7,   519,     6,    11,     5,     4,    13,
      12,    14,    85,    86,    84,    93,    95,    37,    50,    47,
      48,    39,     0,    45,    38,   533,   532,     0,     0,    22,
      21,   519,     0,     0,     0,     0,   240,    35,   100,   101,
       0,     0,     0,   102,   104,   111,     0,    99,    17,   552,
     551,   206,   537,   553,   520,   521,     0,     0,     0,     0,
      40,     0,    46,     0,     0,    43,   534,   536,    18,   695,
     687,   691,   242,     0,     0,   110,   105,     0,     0,     0,
       0,     0,     0,   114,   208,   207,   210,   205,   539,   538,
       0,   555,   554,   558,   523,   522,   524,    91,    92,    89,
      90,    88,     0,     0,    87,    96,    51,    49,    45,    42,
      41,     0,    19,    20,    23,     0,     0,     0,   241,    33,
      36,   109,     0,   106,   107,   108,   112,     0,   540,   541,
     548,   457,    24,    25,     0,    80,    81,    78,    79,    77,
      76,    82,     0,    44,     0,   696,   688,   692,    34,   103,
       0,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,     0,     0,   120,   115,
       0,     0,     0,   549,     0,   559,     0,   458,     0,    26,
      27,    28,     0,    94,     0,     0,     0,     0,     0,     0,
     566,   586,   567,   602,   568,   572,   573,   574,   575,   592,
     579,   580,   581,   582,   583,   584,   585,   587,   588,   589,
     590,   657,   571,   578,   591,   664,   671,   569,   576,   570,
     577,     0,     0,     0,     0,   601,   621,   624,   622,   623,
     684,   619,   535,   607,   485,   491,   174,   175,   172,   123,
     124,   126,   125,   127,   128,   129,   130,   156,   157,   154,
     155,   147,   158,   159,   148,   145,   146,   173,   167,     0,
     171,   160,   161,   162,   163,   134,   135,   136,   131,   132,
     133,   144,     0,   150,   151,   149,   142,   143,   138,   137,
     139,   140,   141,   122,   121,   166,     0,   152,   153,   457,
     118,   233,   211,   593,   596,   599,   600,   594,   597,   595,
     598,   542,   543,   546,   556,    97,     0,   511,   504,   525,
      83,   625,   648,   651,     0,   654,   644,     0,   610,   658,
     665,   672,   678,   681,     0,     0,   634,   639,   633,     0,
     647,   636,     0,   643,     0,   638,   620,     0,   608,   761,
     689,   693,   176,   177,   170,   165,   178,   168,   164,     0,
     116,   263,   479,     0,     0,   209,     0,   528,     0,   550,
     470,   560,    98,     0,     0,   505,     0,     0,     0,     0,
       0,     0,   365,   366,     0,     0,     0,     0,   359,     0,
       0,     0,     0,     0,     0,     0,     0,   592,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   446,
     308,   310,   309,   311,   312,   313,   314,    29,     0,     0,
       0,     0,     0,     0,     0,     0,   294,   295,   363,   362,
     440,   360,   433,   432,   431,   430,   113,   436,   361,   435,
     434,   406,   367,   407,     0,   368,     0,   364,   699,   703,
     700,   701,   702,     0,     0,     0,     0,     0,   115,     0,
     115,     0,   115,     0,     0,     0,   630,   236,   641,   642,
     635,   637,     0,   640,   616,     0,     0,   686,   685,   762,
     697,   240,   486,     0,   481,     0,     0,   492,     0,     0,
     179,   169,     0,   261,   262,     0,   457,   117,   119,   235,
       0,    60,    61,     0,   255,   253,     0,     0,     0,     0,
     254,     0,     0,     0,     0,     0,   212,   215,     0,     0,
       0,     0,   228,   223,   220,   219,   221,     0,   234,     0,
      67,    68,    65,    66,   229,   267,   218,     0,    64,   526,
     529,   761,   547,   471,   512,     0,   502,   503,   501,     0,
       0,     0,     0,   613,   717,   720,   245,     0,   248,   252,
       0,   283,     0,     0,     0,   746,     0,     0,     0,     0,
     274,   277,     0,   280,   750,     0,   726,   732,     0,   723,
       0,   395,   396,     0,     0,     0,     0,     0,     0,     0,
       0,   730,   753,   761,   372,   371,   408,   370,   369,     0,
       0,   761,   289,   761,   296,     0,   303,   233,   295,   113,
     214,     0,     0,     0,     0,   397,   398,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   351,     0,   628,     0,     0,     0,
     603,   605,     0,     0,   118,     0,   118,     0,   118,   483,
       0,   489,     0,   604,     0,     0,   237,   632,   615,   618,
       0,   609,     0,     0,   487,   690,     0,   493,   694,     0,
       0,   561,   477,   496,   480,     0,     0,     0,   256,     0,
       0,   243,     0,     0,   232,     0,     0,    54,    72,     0,
     258,     0,   230,   231,     0,     0,   222,   217,   224,   225,
     226,   227,   266,     0,   216,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   762,   544,   557,     0,   457,   516,
       0,     0,   626,   649,   652,     0,     0,     0,     0,   715,
     236,     0,     0,     0,   736,   739,   742,     0,   761,     0,
     761,     0,     0,     0,     0,     0,     0,   761,     0,     0,
     761,     0,     0,     0,     0,     0,     0,     0,     0,    32,
       0,    30,     0,     0,   762,     0,     0,     0,   762,     0,
       0,     0,     0,   341,   338,   340,     0,   240,     0,   354,
       0,   712,     0,     0,   115,     0,   296,   303,     0,     0,
     419,   418,     0,     0,   420,   424,   373,   374,   386,   387,
     384,   385,     0,   413,     0,   404,     0,   437,   438,   439,
     375,   376,   391,   392,   393,   394,     0,     0,   389,   390,
     388,   382,   383,   378,   377,   379,   380,   381,     0,     0,
       0,   347,     0,     0,     0,     0,   357,     0,   655,   645,
       0,   611,   659,     0,   666,     0,   673,     0,     0,   679,
       0,   682,     0,   238,   629,     0,   617,   698,   482,   488,
     478,     0,     0,   495,     0,   494,     0,   497,     0,     0,
       0,   257,     0,   244,     0,     0,    52,    53,   259,   233,
       0,     0,     0,   506,     0,   273,   504,   272,   325,   326,
     328,   327,   329,   319,   320,   321,   330,   331,   317,   318,
     332,   333,   322,   323,   324,   316,   527,   530,     0,   464,
     467,     0,     0,   518,   627,   650,   653,   614,     0,     0,
       0,   716,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   409,     0,     0,   410,     0,
       0,     0,     0,     0,     0,     0,   303,   441,   442,   443,
     444,   445,     0,     0,     0,   729,   754,   755,     0,     0,
     290,   735,   399,     0,     0,     0,   115,     0,     0,     0,
       0,     0,   356,     0,   355,     0,   118,   307,   115,   415,
       0,   421,     0,     0,     0,   402,     0,     0,   425,   429,
       0,     0,   405,     0,     0,     0,     0,   348,     0,   352,
     400,   358,   656,   646,   606,   612,   660,   662,   667,   669,
     674,   676,   484,   680,   490,   683,     0,     0,     0,   563,
     564,   498,   500,     0,     0,   260,     0,    75,     0,     0,
       0,     0,     0,   268,     0,     0,     0,     0,   545,   465,
     466,   467,   468,   459,   472,   517,   718,   721,   246,     0,
     250,     0,   249,     0,     0,   286,   284,     0,     0,     0,
     747,   745,     0,     0,   756,   275,   278,   281,   751,   749,
     727,   733,   731,   724,     0,     0,    31,     0,     0,     0,
       0,     0,     0,   118,     0,   705,   704,     0,     0,     0,
       0,     0,     0,     0,     0,   301,     0,     0,   118,     0,
       0,     0,     0,   336,     0,     0,   426,     0,   414,     0,
     403,     0,   349,     0,     0,   401,   353,   663,   670,   677,
     239,   236,   562,     0,     0,    73,     0,    74,   213,    57,
      62,     0,   508,     0,   504,   509,     0,     0,   462,   459,
     460,   461,   464,     0,     0,   247,   251,     0,     0,   285,
     737,   740,   743,     0,     0,   761,     0,     0,     0,     0,
     715,     0,     0,     0,   447,     0,     0,     0,     0,   339,
     456,   342,     0,     0,   334,     0,     0,     0,     0,   299,
     300,   298,   297,     0,   304,     0,   291,   305,     0,     0,
     455,     0,   453,   337,   450,     0,     0,     0,   449,   350,
       0,     0,   565,   499,     0,     0,    55,    56,     0,    69,
       0,     0,   507,   269,     0,     0,     0,   513,     0,     0,
     463,   473,   462,     0,   715,   715,     0,     0,     0,     0,
       0,     0,     0,     0,   276,   279,   282,     0,   728,   715,
       0,   411,     0,   448,   759,   759,     0,     0,   345,     0,
       0,     0,     0,   707,   706,   302,     0,   292,   306,   416,
     422,     0,   454,   452,   451,   631,    71,    58,     0,    63,
      67,    68,    65,    66,    64,    70,     0,     0,   510,     0,
     515,     0,   475,   469,     0,     0,     0,   287,   715,   715,
       0,   761,   761,   757,   761,     0,   713,     0,   412,     0,
       0,     0,   759,   343,     0,   709,   708,   335,     0,   293,
     417,   423,   427,     0,     0,     0,   514,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     725,     0,   765,   763,     0,   346,     0,     0,   428,     0,
       0,   271,   474,     0,   719,   722,   288,     0,     0,   744,
     748,   758,   752,   734,   714,   760,   764,   711,   710,    57,
     270,   476,   738,   741,    59
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1265, -1265, -1265, -1265, -1265, -1265,   361,   895, -1265, -1265,
   -1265,   987, -1265, -1265, -1265,   947, -1265,   862, -1265, -1265,
     922, -1265, -1265, -1265,  -396, -1265, -1265,  -246, -1265, -1265,
   -1265, -1265, -1265, -1265,   793, -1265, -1265,   -57,   908, -1265,
   -1265, -1265,   297, -1265,  -412,  -470,  -650, -1265, -1265, -1265,
   -1264, -1265, -1265,  -520, -1265, -1265,  -609,  -753, -1265,   -14,
   -1265, -1265, -1265, -1265, -1265,  -243,  -242,  -241,  -240, -1265,
   -1265,  1012, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,
   -1265, -1265, -1265, -1265, -1265, -1265, -1265,  -426, -1265,   572,
    -204, -1265,  -797, -1265, -1265, -1265, -1265, -1265, -1265, -1245,
   -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,   543,
   -1265, -1265, -1265, -1265, -1265, -1265, -1265,  -140,  -159,  -237,
    -151,   -45, -1265, -1265, -1265, -1265, -1265,   527, -1265,  -468,
   -1265, -1265,  -475, -1265, -1265,  -698,  -229,  -561,  -905, -1265,
   -1265, -1265, -1265,   986, -1265, -1265, -1265,   287, -1265,   655,
   -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,
   -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,  -587,  -161,
   -1265,   640, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,
    -336, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,
   -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,
   -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,  -139,
   -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,
   -1265, -1265,   662,  -623,    83,  -953, -1151, -1265, -1265, -1265,
   -1265, -1265, -1265, -1265, -1265, -1265,  -788, -1265, -1265, -1265,
   -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265, -1265,
   -1265,  -581, -1265, -1230,  -548, -1265
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    16,   134,    51,    17,   154,   160,   609,   448,
     140,   449,    94,    19,    20,    43,    44,    85,    21,    39,
      40,   538,   539,  1248,  1249,   540,  1251,   541,   542,   543,
     544,   545,   546,   547,   161,   162,    35,    36,    37,   207,
      63,    64,    65,    66,    22,   320,   385,   199,    23,   106,
     200,   107,   147,   322,   450,   548,   386,   685,   892,   451,
     549,   577,   768,  1185,   452,   550,   551,   552,   553,   554,
     515,   555,   733,  1074,   925,   556,   453,   782,  1196,   783,
    1197,   785,  1198,   454,   773,  1189,   455,   621,  1227,   456,
    1135,  1136,   824,   457,   630,   458,   557,   459,   460,   815,
     461,  1005,  1287,  1006,  1344,   462,   874,  1156,   463,   622,
    1139,  1350,  1141,  1351,  1235,  1378,   465,   381,  1182,  1261,
    1081,  1083,   951,   563,   758,  1321,  1358,   382,   383,   503,
     680,   370,   508,   682,   371,  1009,   702,   569,   396,   926,
     338,   927,   339,    75,   116,    25,   151,   560,   561,    47,
      48,   131,    26,   110,   149,   202,    27,   387,   948,   389,
     204,   205,    73,   113,   391,    28,   150,   334,   703,   466,
     331,   257,   258,   672,   369,   259,   476,  1045,   572,   366,
     260,   261,   397,   954,   684,   474,  1043,   398,   955,   399,
     956,   473,  1042,   477,  1046,   478,  1157,   479,  1048,   480,
    1158,   481,  1050,   482,  1159,   483,  1053,   484,  1055,   504,
      29,   136,   264,   505,    30,   137,   265,   509,    31,   135,
     263,   692,   467,  1336,   822,  1337,   962,   468,   766,  1183,
     767,  1184,   791,  1202,   788,  1200,   612,   469,   789,  1201,
     470,   967,  1268,   968,  1269,   969,  1270,   777,  1193,   786,
    1199,   613,   471,  1340,   500,   472
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      57,    67,   821,   256,   760,   908,   709,   780,   674,   681,
     676,   208,   678,   755,   718,   679,   996,   963,   825,   623,
     125,  1077,     2,   802,   883,   262,   885,  -113,   887,     3,
    1018,   498,   806,   994,   582,   490,   117,   118,   998,    81,
    1180,   488,  1343,  1064,   626,  1131,    67,    67,    67,  1278,
    1119,  1132,     4,   379,     5,  1341,     6,  1357,   565,   379,
      58,   686,     7,   643,   644,   805,    38,    32,    33,   922,
     400,   401,     8,   809,    82,   810,   641,   513,     9,   643,
     644,   868,   869,   903,   923,    67,    67,    67,    67,    59,
     407,    99,   100,   101,  1383,   566,   409,    68,  1133,  1375,
    1181,   567,    10,  1134,    93,    76,   870,   155,   156,   380,
     903,   206,  1374,  1324,  1325,   871,  1008,   206,   516,   910,
     514,   816,    93,    74,    11,    12,    49,   924,  1335,   566,
    1089,   583,   584,   415,   416,   567,   905,   664,   665,   367,
     566,   759,    69,    70,    78,    71,   567,   693,   568,   335,
     255,   757,   517,   664,   665,   872,   873,    60,   141,   903,
     778,  -661,    86,   905,    55,  1008,  -661,   418,   419,   787,
    1369,    50,   790,    72,  1090,  1257,   119,  1362,  1363,   631,
     632,   120,   568,   121,   122,  -661,   491,    41,    56,  1114,
     756,   904,   489,   568,    13,  1010,   904,   631,   632,   820,
      87,   904,   102,    55,  1370,   492,   256,    83,   904,   256,
      34,   493,   905,    14,   585,  1369,    42,   827,    77,    84,
     999,    61,    15,   256,  1225,    15,   123,    56,   103,  1369,
     971,    62,   974,   256,   586,   438,   256,   256,   256,   981,
     355,   877,   984,   627,   903,   518,   169,   157,  1369,  1384,
     576,   779,   158,    77,   159,   122,   903,   321,   570,   571,
     573,   903,  1217,  1385,   792,    45,   903,  1289,   444,  1255,
     486,    46,  1376,   633,   634,   635,   636,  1369,   104,   392,
     446,   628,  1389,   641,   105,   642,   643,   644,   645,   646,
     487,   647,   648,   635,   636,  1369,   904,   905,  1369,   906,
     212,   641,   907,   642,   643,   644,   645,   646,    93,   905,
    1068,  1393,   256,   256,   905,   506,   256,    55,   256,   905,
     256,   700,   256,  1011,   895,   507,   811,  1173,   213,  1402,
    1178,   812,  1403,    79,   668,   669,   701,  1223,   673,   821,
     675,    56,   677,  1013,   686,   659,   660,   661,   662,   663,
    1082,  1012,  1041,   255,  1016,  1174,   255,    95,    96,    97,
     664,   665,  1013,  1164,   813,  1224,  1137,   895,    89,  1127,
     255,  1359,  1360,   895,   562,   895,   895,  1364,   664,   665,
     255,  -668,  1368,   255,   255,   255,  -668,   587,  1291,    55,
     255,   580,  1065,   631,   632,  1233,   143,   144,   145,   146,
     108,  1239,    90,  1244,  1283,  -668,   109,   588,  1241,  1387,
    1388,  -675,  -344,    56,   594,  1054,  -675,  -344,  1284,   323,
    1052,   631,   632,   324,    55,   367,   256,   152,   153,   762,
     958,   959,  1218,   811,   595,  -675,  -344,   325,   326,   256,
     880,   970,   327,   328,   329,   330,   976,   977,    56,   979,
     980,   765,   982,   983,   881,   985,   597,   771,   367,   255,
     255,   367,   763,   255,   888,   255,   890,   255,   972,   255,
     831,   835,  1056,  1212,   631,   632,   598,   772,   889,  1093,
     891,    91,   973,  1245,    92,   849,  1057,   844,  1228,   635,
     636,   209,   210,  1094,  1246,  1247,   845,   641,  1031,   642,
     643,   644,   645,   646,  1036,  1215,   393,  1032,   111,   394,
    1129,   816,   395,  1037,   112,  1290,  1177,   635,   636,  1130,
    1089,  1254,  1195,   256,   395,   641,    98,   642,   643,   644,
     645,   646,   256,   114,  1075,   256,  1123,  1076,   132,   115,
     395,  1079,   126,  1165,   133,   893,  1167,  1080,  1138,  1169,
    1206,   367,  1207,   128,   898,   878,   895,   899,   895,   659,
     660,   661,   662,   663,    99,   367,   101,   356,  1117,   879,
     635,   636,  1118,   255,   664,   665,   367,    84,   641,  1286,
     882,   643,   644,   645,   646,   895,   255,   139,   367,   661,
     662,   663,   884,   129,   357,   358,   367,  1144,   130,    82,
     886,   138,   664,   665,   256,   256,   256,   256,   148,  1153,
     814,   164,   256,   165,   166,  1258,   256,  1226,   950,   367,
     167,   256,   256,  1088,   256,   256,   957,   256,   256,   960,
     256,  1097,  1098,  1099,   966,    99,  1102,   367,  1166,   367,
     367,  1096,   168,  1300,  1352,   201,   203,  1273,   206,   317,
    1377,   152,   153,   799,   800,   664,   665,   359,    99,   100,
     101,   360,   209,   210,   211,    52,    53,    54,   318,  1122,
     255,   319,   321,   332,   333,   336,   337,   341,   342,   255,
     343,   367,   255,   368,   344,   345,   346,   347,   354,  1205,
     348,  1332,   349,  1208,  1317,  1029,   350,   372,   351,   373,
     352,   353,   376,   374,   384,   388,   377,   475,  1298,   390,
     375,   499,   378,   496,   510,   511,   559,   564,   361,   696,
     671,   362,   512,   363,  1306,  1307,   624,   256,   670,    15,
     589,   574,   575,   581,   590,   686,   591,   364,   593,   506,
    1176,   592,   596,   365,   599,   256,  1394,   600,   691,   715,
     694,   255,   255,   255,   255,  1354,   695,   713,   698,   255,
     697,   603,   714,   255,   721,   604,   605,  1061,   255,   255,
     606,   255,   255,   607,   255,   255,   608,   255,   625,   667,
     687,   722,   723,  1365,  1366,   705,  1367,   706,   725,   712,
     726,   727,   728,   729,   724,   730,   731,   732,   734,   764,
     774,   753,   769,  1229,   770,   754,   256,   256,   256,   804,
     775,   256,   807,   776,   808,   823,   826,   894,   842,   895,
     897,   901,   902,   915,   912,   917,   916,   919,   949,   961,
     631,   632,   986,   964,   921,   997,  1001,  1013,  1019,  1021,
    1002,  1014,  1003,  1004,   256,  1331,  1039,  1007,  1015,  1044,
    1059,  1334,  1020,  1022,  1047,  1023,  1024,  1034,  1049,  1399,
    1051,  1060,  1072,  1401,  1073,  1035,  1071,  1078,  1082,  1084,
    1086,  1087,  1092,  1101,   255,  1100,  1105,  1106,  1103,  1107,
    1108,  1104,  1109,  1112,  1110,  1163,  1111,   464,  1115,  1113,
    1146,  1147,   255,  1190,  1172,   256,  1175,   485,  1186,  1187,
    1149,  1191,  1192,  1194,  1211,  1219,  1154,   495,  1220,  1161,
    1221,  1188,  1222,  1203,  1216,   256,  1214,  1160,  1316,  1242,
     631,   632,  1250,  1253,   633,   634,   635,   636,   637,   558,
    1256,   638,   639,   640,   641,  1260,   642,   643,   644,   645,
     646,  1264,   647,   648,  1265,  1271,  1266,  1272,  1288,  1277,
    1279,  1299,  1280,   255,   255,   255,  1315,  1305,   255,  1308,
    1318,  1326,  1339,  1328,   601,   602,  1371,  1372,  1329,  1330,
     801,   124,  1333,  1347,  1373,  1382,  1396,  1390,   256,  1391,
     256,   611,   614,   615,   616,   617,   618,  1392,    18,    80,
     163,   255,   655,   656,   657,   658,   659,   660,   661,   662,
     663,   127,  1231,  1404,  1309,   340,   142,  1310,  1311,  1312,
    1313,   664,   665,    24,   633,   634,   635,   636,   637,  1295,
    1262,   638,   639,   640,   641,  1322,   642,   643,   644,   645,
     646,  1263,   647,   648,  1323,   629,  1179,    88,   649,   690,
     651,   947,   255,   704,   578,  1091,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   699,     0,     0,     0,     0,
     631,   632,   255,     0,     0,     0,   579,   708,     0,     0,
     711,     0,     0,     0,   256,     0,   717,     0,   720,     0,
       0,   654,   655,   656,   657,   658,   659,   660,   661,   662,
     663,     0,     0,   256,     0,     0,  1301,     0,     0,     0,
       0,   664,   665,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   761,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   255,     0,   255,     0,     0,
       0,   611,   781,     0,     0,   784,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   793,   794,   795,   796,
     797,   798,     0,     0,   633,   634,   635,   636,   637,     0,
       0,   638,   639,   640,   641,     0,   642,   643,   644,   645,
     646,     0,   647,   648,     0,     0,   836,   837,     0,     0,
     838,   839,   840,   841,     0,   843,     0,   846,   847,   848,
     850,   851,   852,   853,   854,   855,   857,   858,   859,   860,
     861,   862,   863,   864,   865,   866,   867,     0,   875,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   255,     0,   656,   657,   658,   659,   660,   661,   662,
     663,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     255,   664,   665,     0,     0,     0,     0,     0,   909,     0,
     911,     0,     0,   913,     0,   914,     0,     0,     0,     0,
       0,     0,   918,     0,     0,     0,     0,   920,     0,     0,
       0,     0,     0,   814,     0,     0,     0,     0,   928,   929,
     930,   931,   932,   933,   934,   935,   936,   937,   938,   939,
     940,   941,   942,   943,   944,   945,   946,     0,     0,     0,
       0,     0,     0,   952,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   965,     0,     0,     0,     0,
     214,     0,     0,     0,     0,     0,   215,     0,     0,     0,
     814,     0,   216,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   217,   993,     0,     0,   995,   611,     0,     0,
     218,  1000,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   219,     0,     0,   558,     0,
       0,     0,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,     0,     0,     0,     0,     0,
       0,     0,  1033,     0,     0,     0,  1038,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1000,    55,
       0,     0,     0,     0,  1058,     0,     0,     0,     0,     0,
       0,  1062,   253,     0,     0,     0,     0,     0,  1067,   214,
       0,     0,     0,    56,  1070,   215,     0,     0,     0,     0,
       0,   216,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   217,     0,     0,     0,     0,     0,     0,     0,   218,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   219,     0,     0,     0,   254,     0,
     497,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,     0,     0,  1120,  1121,     0,     0,
    1124,  1125,  1126,     0,  1000,     0,     0,     0,     0,     0,
       0,     0,     0,  1140,     0,  1142,     0,  1145,     0,     0,
       0,     0,     0,  1148,     0,     0,     0,  1151,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   253,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   501,     0,   214,   631,   632,     0,     0,     0,
     215,   558,   502,     0,  1171,     0,   216,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   217,     0,     0,     0,
       0,     0,     0,     0,   218,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   611,   254,     0,   219,
       0,     0,     0,     0,     0,     0,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,     0,
    1236,     0,  1237,     0,     0,     0,     0,  1240,     0,   633,
     634,   635,   636,   637,     0,     0,   638,  1243,     0,   641,
       0,   642,   643,   644,   645,   646,     0,   647,   648,     0,
    1259,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       0,  1267,     0,     0,     0,     0,   253,     0,     0,  1274,
    1275,  1276,     0,     0,     0,     0,  1282,    56,     0,   611,
    1285,     0,     0,     0,     0,     0,     0,     0,     0,  1292,
    1293,  1294,     0,     0,     0,     0,     0,     0,  1297,   657,
     658,   659,   660,   661,   662,   663,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   664,   665,     0,     0,
       0,     0,   254,  1314,     0,     0,     0,     0,     0,     0,
       0,  1319,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   611,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1342,
       0,     0,  1345,  1346,     0,     0,     0,     0,     0,  1349,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1353,     0,     0,     0,     0,     0,     0,   519,     0,
    1355,     0,   400,   401,     3,     0,   520,   521,   522,  1361,
     523,     0,   402,   403,   404,   405,   406,     0,     0,     0,
       0,     0,   407,   524,   408,   525,   526,     0,   409,     0,
       0,     0,     0,     0,     0,   527,   410,  1380,     0,   528,
       0,   529,   411,     0,     0,   412,     0,     8,   413,   530,
       0,   531,   414,     0,  1395,   532,   533,     0,     0,  1397,
    1398,     0,   534,     0,     0,   415,   416,     0,   220,   221,
     222,     0,   224,   225,   226,   227,   228,   417,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,     0,
     242,   243,   244,     0,     0,   247,   248,   249,   250,   418,
     419,   420,   535,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   421,   422,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,   423,   424,   425,   426,   427,     0,   428,
       0,   429,   430,   431,   432,   433,   434,   435,   436,    56,
     437,     0,     0,     0,     0,     0,     0,   438,   536,   537,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,   440,   441,     0,    14,     0,
       0,   442,   443,     0,     0,     0,     0,     0,     0,     0,
     444,     0,   445,   519,   446,   447,     0,   400,   401,     3,
       0,   520,   521,   522,     0,   523,     0,   402,   403,   404,
     405,   406,     0,     0,     0,     0,     0,   407,   524,   408,
     525,   526,     0,   409,     0,     0,     0,     0,     0,     0,
     527,   410,     0,     0,   528,     0,   529,   411,     0,     0,
     412,     0,     8,   413,   530,     0,   531,   414,     0,     0,
     532,   533,     0,     0,     0,     0,     0,   534,     0,     0,
     415,   416,     0,   220,   221,   222,     0,   224,   225,   226,
     227,   228,   417,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,     0,   242,   243,   244,     0,     0,
     247,   248,   249,   250,   418,   419,   420,   535,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   421,
     422,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   423,   424,
     425,   426,   427,     0,   428,     0,   429,   430,   431,   432,
     433,   434,   435,   436,    56,   437,     0,     0,     0,     0,
       0,     0,   438,  1017,   537,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   439,
     440,   441,     0,    14,     0,     0,   442,   443,     0,     0,
       0,     0,     0,     0,     0,   444,     0,   445,   519,   446,
     447,     0,   400,   401,     3,     0,   520,   521,   522,     0,
     523,     0,   402,   403,   404,   405,   406,     0,     0,     0,
       0,     0,   407,   524,   408,   525,   526,     0,   409,     0,
       0,     0,     0,     0,     0,   527,   410,     0,     0,   528,
       0,   529,   411,     0,     0,   412,     0,     8,   413,   530,
       0,   531,   414,     0,     0,   532,   533,     0,     0,     0,
       0,     0,   534,     0,     0,   415,   416,     0,   220,   221,
     222,     0,   224,   225,   226,   227,   228,   417,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,     0,
     242,   243,   244,     0,     0,   247,   248,   249,   250,   418,
     419,   420,   535,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   421,   422,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,   423,   424,   425,   426,   427,     0,   428,
       0,   429,   430,   431,   432,   433,   434,   435,   436,    56,
     437,     0,     0,     0,     0,     0,     0,   438,  1168,   537,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,   440,   441,     0,    14,     0,
       0,   442,   443,     0,     0,     0,   400,   401,     0,     0,
     444,     0,   445,     0,   446,   447,   402,   403,   404,   405,
     406,     0,     0,     0,     0,     0,   407,   524,   408,   525,
       0,     0,   409,     0,     0,     0,     0,     0,     0,     0,
     410,     0,     0,     0,     0,     0,   411,     0,     0,   412,
       0,     0,   413,   530,     0,     0,   414,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   415,
     416,     0,   220,   221,   222,     0,   224,   225,   226,   227,
     228,   417,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   242,   243,   244,     0,     0,   247,
     248,   249,   250,   418,   419,   420,   535,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   421,   422,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,   423,   424,   425,
     426,   427,     0,   428,     0,   429,   430,   431,   432,   433,
     434,   435,   436,    56,   437,     0,     0,     0,     0,     0,
       0,   438,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   439,   440,
     441,     0,    14,     0,     0,   442,   443,     0,     0,     0,
       0,     0,   400,   401,   444,     0,   445,     0,   446,   447,
     619,     0,   402,   403,   404,   405,   406,     0,     0,     0,
       0,     0,   407,     0,   408,     0,     0,     0,   409,     0,
       0,     0,     0,     0,     0,     0,   410,     0,     0,     0,
       0,     0,   411,     0,     0,   412,   620,     0,   413,     0,
       0,     0,   414,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   415,   416,     0,   220,   221,
     222,     0,   224,   225,   226,   227,   228,   417,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,     0,
     242,   243,   244,     0,     0,   247,   248,   249,   250,   418,
     419,   420,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   421,   422,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,   423,   424,   425,   426,   427,     0,   428,
     816,   429,   430,   431,   432,   433,   434,   435,   436,   817,
     437,     0,     0,     0,     0,     0,     0,   438,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,   440,   441,     0,    14,     0,
       0,   442,   443,     0,     0,     0,     0,     0,   400,   401,
     444,     0,   445,     0,   446,   447,   619,     0,   402,   403,
     404,   405,   406,     0,     0,     0,     0,     0,   407,     0,
     408,     0,     0,     0,   409,     0,     0,     0,     0,     0,
       0,     0,   410,     0,     0,     0,     0,     0,   411,     0,
       0,   412,   620,     0,   413,     0,     0,     0,   414,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   415,   416,     0,   220,   221,   222,     0,   224,   225,
     226,   227,   228,   417,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,     0,   242,   243,   244,     0,
       0,   247,   248,   249,   250,   418,   419,   420,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     421,   422,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   423,
     424,   425,   426,   427,     0,   428,     0,   429,   430,   431,
     432,   433,   434,   435,   436,    56,   437,     0,     0,     0,
       0,     0,     0,   438,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     439,   440,   441,     0,    14,     0,     0,   442,   443,     0,
       0,     0,   400,   401,     0,     0,   444,     0,   445,     0,
     446,   447,   402,   403,   404,   405,   406,     0,     0,     0,
       0,     0,   407,     0,   408,     0,     0,     0,   409,     0,
       0,     0,     0,     0,     0,     0,   410,     0,     0,     0,
       0,     0,   411,     0,     0,   412,     0,     0,   413,     0,
       0,     0,   414,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   415,   416,     0,   220,   221,
     222,     0,   224,   225,   226,   227,   228,   417,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,     0,
     242,   243,   244,     0,     0,   247,   248,   249,   250,   418,
     419,   420,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   421,   422,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,   423,   424,   425,   426,   427,     0,   428,
     816,   429,   430,   431,   432,   433,   434,   435,   436,   817,
     437,     0,     0,     0,     0,     0,     0,   438,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,   440,   441,     0,    14,     0,
       0,   442,   443,     0,     0,     0,   400,   401,     0,     0,
     818,     0,   445,   819,   446,   447,   402,   403,   404,   405,
     406,     0,     0,     0,     0,     0,   407,     0,   408,     0,
       0,     0,   409,     0,     0,     0,     0,     0,     0,     0,
     410,     0,     0,     0,     0,     0,   411,     0,     0,   412,
       0,     0,   413,     0,     0,     0,   414,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   415,
     416,     0,   220,   221,   222,     0,   224,   225,   226,   227,
     228,   417,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   242,   243,   244,     0,     0,   247,
     248,   249,   250,   418,   419,   420,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   421,   422,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,   423,   424,   425,
     426,   427,     0,   428,     0,   429,   430,   431,   432,   433,
     434,   435,   436,    56,   437,     0,     0,     0,     0,     0,
       0,   438,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   439,   440,
     441,     0,    14,     0,     0,   442,   443,     0,     0,     0,
       0,     0,   400,   401,   444,   494,   445,     0,   446,   447,
     610,     0,   402,   403,   404,   405,   406,     0,     0,     0,
       0,     0,   407,     0,   408,     0,     0,     0,   409,     0,
       0,     0,     0,     0,     0,     0,   410,     0,     0,     0,
       0,     0,   411,     0,     0,   412,     0,     0,   413,     0,
       0,     0,   414,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   415,   416,     0,   220,   221,
     222,     0,   224,   225,   226,   227,   228,   417,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,     0,
     242,   243,   244,     0,     0,   247,   248,   249,   250,   418,
     419,   420,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   421,   422,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,   423,   424,   425,   426,   427,     0,   428,
       0,   429,   430,   431,   432,   433,   434,   435,   436,    56,
     437,     0,     0,     0,     0,     0,     0,   438,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,   440,   441,     0,    14,     0,
       0,   442,   443,     0,     0,     0,   400,   401,     0,     0,
     444,     0,   445,     0,   446,   447,   402,   403,   404,   405,
     406,     0,     0,     0,     0,     0,   407,     0,   408,     0,
       0,     0,   409,     0,     0,     0,     0,     0,     0,     0,
     410,     0,     0,     0,     0,     0,   411,     0,     0,   412,
       0,     0,   413,     0,     0,     0,   414,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   415,
     416,     0,   220,   221,   222,     0,   224,   225,   226,   227,
     228,   417,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   242,   243,   244,     0,     0,   247,
     248,   249,   250,   418,   419,   420,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   421,   422,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,   423,   424,   425,
     426,   427,     0,   428,     0,   429,   430,   431,   432,   433,
     434,   435,   436,    56,   437,     0,     0,     0,     0,     0,
       0,   438,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   439,   440,
     441,     0,    14,     0,     0,   442,   443,     0,     0,     0,
     400,   401,     0,     0,   444,   689,   445,     0,   446,   447,
     402,   403,   404,   405,   406,     0,     0,     0,     0,     0,
     407,     0,   408,     0,     0,     0,   409,     0,     0,     0,
       0,     0,     0,     0,   410,     0,     0,     0,     0,     0,
     411,     0,     0,   412,     0,     0,   413,     0,     0,     0,
     414,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   415,   416,     0,   220,   221,   222,     0,
     224,   225,   226,   227,   228,   417,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,     0,   242,   243,
     244,     0,     0,   247,   248,   249,   250,   418,   419,   420,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   421,   422,     0,     0,     0,     0,     0,     0,
       0,   707,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       0,   423,   424,   425,   426,   427,     0,   428,     0,   429,
     430,   431,   432,   433,   434,   435,   436,    56,   437,     0,
       0,     0,     0,     0,     0,   438,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   439,   440,   441,     0,    14,     0,     0,   442,
     443,     0,     0,     0,   400,   401,     0,     0,   444,     0,
     445,     0,   446,   447,   402,   403,   404,   405,   406,     0,
       0,     0,     0,     0,   407,     0,   408,     0,     0,     0,
     409,     0,     0,     0,     0,     0,     0,     0,   410,     0,
       0,     0,     0,     0,   411,     0,     0,   412,     0,     0,
     413,     0,     0,     0,   414,     0,     0,     0,     0,     0,
     710,     0,     0,     0,     0,     0,     0,   415,   416,     0,
     220,   221,   222,     0,   224,   225,   226,   227,   228,   417,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,     0,   242,   243,   244,     0,     0,   247,   248,   249,
     250,   418,   419,   420,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   421,   422,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,   423,   424,   425,   426,   427,
       0,   428,     0,   429,   430,   431,   432,   433,   434,   435,
     436,    56,   437,     0,     0,     0,     0,     0,     0,   438,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   439,   440,   441,     0,
      14,     0,     0,   442,   443,     0,     0,     0,   400,   401,
       0,     0,   444,     0,   445,     0,   446,   447,   402,   403,
     404,   405,   406,     0,     0,     0,     0,     0,   407,     0,
     408,     0,     0,     0,   409,     0,     0,     0,     0,     0,
       0,     0,   410,     0,     0,     0,     0,     0,   411,     0,
       0,   412,     0,     0,   413,     0,     0,     0,   414,     0,
       0,   716,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   415,   416,     0,   220,   221,   222,     0,   224,   225,
     226,   227,   228,   417,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,     0,   242,   243,   244,     0,
       0,   247,   248,   249,   250,   418,   419,   420,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     421,   422,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   423,
     424,   425,   426,   427,     0,   428,     0,   429,   430,   431,
     432,   433,   434,   435,   436,    56,   437,     0,     0,     0,
       0,     0,     0,   438,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     439,   440,   441,     0,    14,     0,     0,   442,   443,     0,
       0,     0,   400,   401,     0,     0,   444,     0,   445,     0,
     446,   447,   402,   403,   404,   405,   406,     0,     0,     0,
       0,     0,   407,     0,   408,     0,     0,     0,   409,     0,
       0,     0,     0,     0,     0,     0,   410,     0,     0,     0,
       0,     0,   411,     0,     0,   412,     0,     0,   413,     0,
       0,     0,   414,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   415,   416,     0,   220,   221,
     222,     0,   224,   225,   226,   227,   228,   417,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,     0,
     242,   243,   244,     0,     0,   247,   248,   249,   250,   418,
     419,   420,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   421,   422,     0,     0,     0,     0,
       0,     0,     0,   719,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,   423,   424,   425,   426,   427,     0,   428,
       0,   429,   430,   431,   432,   433,   434,   435,   436,    56,
     437,     0,     0,     0,     0,     0,     0,   438,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,   440,   441,     0,    14,     0,
       0,   442,   443,     0,     0,     0,   400,   401,     0,     0,
     444,     0,   445,     0,   446,   447,   402,   403,   404,   405,
     406,     0,     0,   856,     0,     0,   407,     0,   408,     0,
       0,     0,   409,     0,     0,     0,     0,     0,     0,     0,
     410,     0,     0,     0,     0,     0,   411,     0,     0,   412,
       0,     0,   413,     0,     0,     0,   414,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   415,
     416,     0,   220,   221,   222,     0,   224,   225,   226,   227,
     228,   417,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   242,   243,   244,     0,     0,   247,
     248,   249,   250,   418,   419,   420,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   421,   422,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,   423,   424,   425,
     426,   427,     0,   428,     0,   429,   430,   431,   432,   433,
     434,   435,   436,    56,   437,     0,     0,     0,     0,     0,
       0,   438,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   439,   440,
     441,     0,    14,     0,     0,   442,   443,     0,     0,     0,
     400,   401,     0,     0,   444,     0,   445,     0,   446,   447,
     402,   403,   404,   405,   406,     0,     0,     0,     0,     0,
     407,     0,   408,     0,     0,     0,   409,     0,     0,     0,
       0,     0,     0,     0,   410,     0,     0,     0,     0,     0,
     411,     0,     0,   412,     0,     0,   413,     0,     0,     0,
     414,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   415,   416,     0,   220,   221,   222,     0,
     224,   225,   226,   227,   228,   417,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,     0,   242,   243,
     244,     0,     0,   247,   248,   249,   250,   418,   419,   420,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   421,   422,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       0,   423,   424,   425,   426,   427,     0,   428,     0,   429,
     430,   431,   432,   433,   434,   435,   436,    56,   437,     0,
       0,     0,     0,     0,     0,   438,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   439,   440,   441,     0,    14,     0,     0,   442,
     443,     0,     0,     0,   400,   401,     0,     0,   444,     0,
     445,   876,   446,   447,   402,   403,   404,   405,   406,     0,
       0,     0,     0,     0,   407,     0,   408,     0,     0,     0,
     409,     0,     0,     0,     0,     0,     0,     0,   410,     0,
       0,     0,     0,     0,   411,     0,     0,   412,     0,     0,
     413,     0,     0,     0,   414,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   415,   416,     0,
     220,   221,   222,     0,   224,   225,   226,   227,   228,   417,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,     0,   242,   243,   244,     0,     0,   247,   248,   249,
     250,   418,   419,   420,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   421,   422,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,   423,   424,   425,   426,   427,
       0,   428,     0,   429,   430,   431,   432,   433,   434,   435,
     436,    56,   437,     0,     0,     0,     0,     0,     0,   438,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   439,   440,   441,     0,
      14,     0,     0,   442,   443,     0,     0,     0,   400,   401,
       0,     0,   444,     0,   445,  1143,   446,   447,   402,   403,
     404,   405,   406,     0,     0,     0,     0,     0,   407,     0,
     408,     0,     0,     0,   409,     0,     0,     0,     0,     0,
       0,     0,   410,     0,     0,     0,     0,     0,   411,     0,
       0,   412,     0,     0,   413,     0,     0,     0,   414,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   415,   416,     0,   220,   221,   222,     0,   224,   225,
     226,   227,   228,   417,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,     0,   242,   243,   244,     0,
       0,   247,   248,   249,   250,   418,   419,   420,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     421,   422,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   423,
     424,   425,   426,   427,     0,   428,     0,   429,   430,   431,
     432,   433,   434,   435,   436,    56,   437,     0,     0,     0,
       0,     0,     0,   438,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     439,   440,   441,     0,    14,     0,     0,   442,   443,     0,
       0,     0,   400,   401,     0,     0,   444,     0,   445,  1152,
     446,   447,   402,   403,   404,   405,   406,     0,     0,     0,
       0,     0,   407,     0,   408,     0,     0,     0,   409,     0,
       0,     0,     0,     0,     0,     0,   410,     0,     0,     0,
       0,     0,   411,     0,     0,   412,     0,     0,   413,     0,
       0,     0,   414,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   415,   416,     0,   220,   221,
     222,     0,   224,   225,   226,   227,   228,   417,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,     0,
     242,   243,   244,     0,     0,   247,   248,   249,   250,   418,
     419,   420,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   421,   422,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,   423,   424,   425,   426,   427,     0,   428,
       0,   429,   430,   431,   432,   433,   434,   435,   436,    56,
     437,     0,     0,     0,     0,     0,     0,   438,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,   440,   441,     0,    14,     0,
       0,   442,   443,     0,     0,     0,   400,   401,     0,     0,
     444,     0,   445,  1204,   446,   447,   402,   403,   404,   405,
     406,     0,     0,     0,     0,     0,   407,     0,   408,     0,
       0,     0,   409,     0,     0,     0,     0,     0,     0,     0,
     410,     0,     0,     0,     0,     0,   411,     0,     0,   412,
       0,     0,   413,     0,     0,     0,   414,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   415,
     416,     0,   220,   221,   222,     0,   224,   225,   226,   227,
     228,   417,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   242,   243,   244,     0,     0,   247,
     248,   249,   250,   418,   419,   420,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   421,   422,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,   423,   424,   425,
     426,   427,     0,   428,     0,   429,   430,   431,   432,   433,
     434,   435,   436,    56,   437,     0,     0,     0,     0,     0,
       0,   438,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   439,   440,
     441,     0,    14,     0,     0,   442,   443,     0,     0,     0,
     400,   401,     0,     0,   444,     0,   445,  1281,   446,   447,
     402,   403,   404,   405,   406,     0,     0,     0,     0,     0,
     407,     0,   408,     0,     0,     0,   409,     0,     0,     0,
       0,     0,     0,     0,   410,     0,     0,     0,     0,     0,
     411,     0,     0,   412,     0,     0,   413,     0,     0,     0,
     414,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   415,   416,     0,   220,   221,   222,     0,
     224,   225,   226,   227,   228,   417,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,     0,   242,   243,
     244,     0,     0,   247,   248,   249,   250,   418,   419,   420,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   421,   422,     0,     0,     0,     0,     0,     0,
       0,  1296,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       0,   423,   424,   425,   426,   427,     0,   428,     0,   429,
     430,   431,   432,   433,   434,   435,   436,    56,   437,     0,
       0,     0,     0,     0,     0,   438,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   439,   440,   441,     0,    14,     0,     0,   442,
     443,     0,     0,     0,   400,   401,     0,     0,   444,     0,
     445,     0,   446,   447,   402,   403,   404,   405,   406,     0,
       0,     0,     0,     0,   407,     0,   408,     0,     0,     0,
     409,     0,     0,     0,     0,     0,     0,     0,   410,     0,
       0,     0,     0,     0,   411,     0,     0,   412,     0,     0,
     413,     0,     0,     0,   414,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   415,   416,     0,
     220,   221,   222,     0,   224,   225,   226,   227,   228,   417,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,     0,   242,   243,   244,     0,     0,   247,   248,   249,
     250,   418,   419,   420,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   421,   422,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,   423,   424,   425,   426,   427,
       0,   428,     0,   429,   430,   431,   432,   433,   434,   435,
     436,    56,   437,     0,     0,     0,     0,     0,     0,   438,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   439,   440,   441,     0,
      14,     0,     0,   442,   443,     0,     0,     0,   400,   401,
       0,     0,   444,     0,   445,     0,   446,   447,   402,   403,
     404,   405,   406,     0,     0,     0,     0,     0,   407,     0,
     408,     0,     0,     0,   409,     0,     0,     0,     0,     0,
       0,     0,   410,     0,     0,     0,     0,     0,   411,     0,
       0,   412,     0,     0,   413,     0,     0,     0,   414,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   415,   416,     0,   220,   221,   222,     0,   224,   225,
     226,   227,   228,   417,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,     0,   242,   243,   244,     0,
       0,   247,   248,   249,   250,   418,   419,   420,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     421,   422,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   423,
     424,   425,   426,   427,     0,   428,     0,   429,   430,   431,
     432,   433,   434,   435,   436,    56,   437,   631,   632,     0,
       0,   214,     0,   438,     0,     0,     0,   215,     0,     0,
       0,     0,     0,   216,     0,     0,     0,     0,     0,     0,
     439,   440,   441,   217,    14,     0,     0,   442,   443,     0,
       0,   218,     0,   631,   632,     0,  1128,     0,   445,     0,
     446,   447,     0,     0,     0,     0,   219,     0,     0,     0,
       0,     0,     0,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,     0,     0,     0,     0,
       0,   633,   634,   635,   636,   637,     0,     0,   638,   639,
     640,   641,     0,   642,   643,   644,   645,   646,     0,   647,
     648,     0,     0,     0,     0,   649,   650,   651,     0,     0,
      55,   652,     0,     0,     0,   631,   632,   633,   634,   635,
     636,   637,     0,   253,   638,   639,   640,   641,     0,   642,
     643,   644,   645,   646,   501,   647,   648,     0,     0,     0,
       0,   649,   650,   651,     0,     0,   653,   652,   654,   655,
     656,   657,   658,   659,   660,   661,   662,   663,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   664,   665,
       0,     0,   666,     0,     0,     0,     0,     0,     0,   254,
       0,     0,   653,     0,   654,   655,   656,   657,   658,   659,
     660,   661,   662,   663,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   664,   665,     0,     0,   683,   633,
     634,   635,   636,   637,   631,   632,   638,   639,   640,   641,
       0,   642,   643,   644,   645,   646,     0,   647,   648,     0,
       0,     0,     0,   649,   650,   651,     0,     0,     0,   652,
       0,     0,     0,     0,     0,   631,   632,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   653,     0,   654,   655,   656,   657,
     658,   659,   660,   661,   662,   663,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   664,   665,     0,     0,
     900,     0,     0,     0,     0,     0,     0,     0,   633,   634,
     635,   636,   637,     0,     0,   638,   639,   640,   641,     0,
     642,   643,   644,   645,   646,     0,   647,   648,     0,     0,
       0,     0,   649,   650,   651,     0,     0,     0,   652,   633,
     634,   635,   636,   637,   631,   632,   638,   639,   640,   641,
       0,   642,   643,   644,   645,   646,     0,   647,   648,     0,
       0,     0,     0,   649,   650,   651,     0,     0,     0,   652,
       0,     0,     0,   653,     0,   654,   655,   656,   657,   658,
     659,   660,   661,   662,   663,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   664,   665,     0,     0,   975,
       0,     0,     0,     0,   653,     0,   654,   655,   656,   657,
     658,   659,   660,   661,   662,   663,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   664,   665,     0,     0,
     978,     0,     0,     0,     0,     0,   631,   632,   633,   634,
     635,   636,   637,     0,     0,   638,   639,   640,   641,     0,
     642,   643,   644,   645,   646,     0,   647,   648,     0,     0,
       0,     0,   649,   650,   651,     0,     0,     0,   652,   631,
     632,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   653,     0,   654,   655,   656,   657,   658,
     659,   660,   661,   662,   663,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   664,   665,     0,     0,   987,
     633,   634,   635,   636,   637,     0,     0,   638,   639,   640,
     641,     0,   642,   643,   644,   645,   646,     0,   647,   648,
       0,     0,     0,     0,   649,   650,   651,     0,     0,     0,
     652,   631,   632,   633,   634,   635,   636,   637,     0,     0,
     638,   639,   640,   641,     0,   642,   643,   644,   645,   646,
       0,   647,   648,     0,     0,     0,     0,   649,   650,   651,
       0,     0,     0,   652,     0,   653,     0,   654,   655,   656,
     657,   658,   659,   660,   661,   662,   663,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   664,   665,     0,
       0,   988,     0,     0,     0,     0,     0,     0,   653,     0,
     654,   655,   656,   657,   658,   659,   660,   661,   662,   663,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     664,   665,     0,     0,   989,   633,   634,   635,   636,   637,
     631,   632,   638,   639,   640,   641,     0,   642,   643,   644,
     645,   646,     0,   647,   648,     0,     0,     0,     0,   649,
     650,   651,     0,     0,     0,   652,     0,     0,     0,     0,
       0,   631,   632,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     653,     0,   654,   655,   656,   657,   658,   659,   660,   661,
     662,   663,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   664,   665,     0,     0,   990,     0,     0,     0,
       0,     0,     0,     0,   633,   634,   635,   636,   637,     0,
       0,   638,   639,   640,   641,     0,   642,   643,   644,   645,
     646,     0,   647,   648,     0,     0,     0,     0,   649,   650,
     651,     0,     0,     0,   652,   633,   634,   635,   636,   637,
     631,   632,   638,   639,   640,   641,     0,   642,   643,   644,
     645,   646,     0,   647,   648,     0,     0,     0,     0,   649,
     650,   651,     0,     0,     0,   652,     0,     0,     0,   653,
       0,   654,   655,   656,   657,   658,   659,   660,   661,   662,
     663,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   664,   665,     0,     0,   991,     0,     0,     0,     0,
     653,     0,   654,   655,   656,   657,   658,   659,   660,   661,
     662,   663,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   664,   665,     0,     0,   992,     0,     0,     0,
       0,     0,   631,   632,   633,   634,   635,   636,   637,     0,
       0,   638,   639,   640,   641,     0,   642,   643,   644,   645,
     646,     0,   647,   648,     0,     0,     0,     0,   649,   650,
     651,     0,     0,     0,   652,   631,   632,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   653,
       0,   654,   655,   656,   657,   658,   659,   660,   661,   662,
     663,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   664,   665,     0,     0,  1063,   633,   634,   635,   636,
     637,     0,     0,   638,   639,   640,   641,     0,   642,   643,
     644,   645,   646,     0,   647,   648,     0,     0,     0,     0,
     649,   650,   651,     0,     0,     0,   652,   631,   632,   633,
     634,   635,   636,   637,     0,     0,   638,   639,   640,   641,
       0,   642,   643,   644,   645,   646,     0,   647,   648,     0,
       0,     0,     0,   649,   650,   651,     0,     0,     0,   652,
       0,   653,     0,   654,   655,   656,   657,   658,   659,   660,
     661,   662,   663,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   664,   665,     0,     0,  1066,     0,     0,
       0,     0,     0,     0,   653,     0,   654,   655,   656,   657,
     658,   659,   660,   661,   662,   663,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   664,   665,     0,     0,
    1069,   633,   634,   635,   636,   637,   631,   632,   638,   639,
     640,   641,     0,   642,   643,   644,   645,   646,     0,   647,
     648,     0,     0,     0,     0,   649,   650,   651,     0,     0,
       0,   652,     0,     0,     0,     0,     0,   631,   632,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   653,     0,   654,   655,
     656,   657,   658,   659,   660,   661,   662,   663,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   664,   665,
       0,     0,  1095,     0,     0,     0,     0,     0,     0,     0,
     633,   634,   635,   636,   637,     0,     0,   638,   639,   640,
     641,     0,   642,   643,   644,   645,   646,     0,   647,   648,
       0,     0,     0,     0,   649,   650,   651,     0,     0,     0,
     652,   633,   634,   635,   636,   637,   631,   632,   638,   639,
     640,   641,     0,   642,   643,   644,   645,   646,     0,   647,
     648,     0,     0,     0,     0,   649,   650,   651,     0,     0,
       0,   652,     0,     0,     0,   653,     0,   654,   655,   656,
     657,   658,   659,   660,   661,   662,   663,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   664,   665,     0,
       0,  1162,     0,     0,     0,     0,   653,     0,   654,   655,
     656,   657,   658,   659,   660,   661,   662,   663,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   664,   665,
       0,     0,  1170,     0,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,     0,     0,   638,   639,   640,
     641,     0,   642,   643,   644,   645,   646,     0,   647,   648,
       0,     0,     0,     0,   649,   650,   651,     0,     0,     0,
     652,   631,   632,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   653,     0,   654,   655,   656,
     657,   658,   659,   660,   661,   662,   663,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   664,   665,     0,
       0,  1209,   633,   634,   635,   636,   637,     0,     0,   638,
     639,   640,   641,     0,   642,   643,   644,   645,   646,     0,
     647,   648,     0,     0,     0,     0,   649,   650,   651,     0,
       0,     0,   652,   631,   632,   633,   634,   635,   636,   637,
       0,     0,   638,   639,   640,   641,     0,   642,   643,   644,
     645,   646,     0,   647,   648,     0,     0,     0,     0,   649,
     650,   651,     0,     0,     0,   652,     0,   653,     0,   654,
     655,   656,   657,   658,   659,   660,   661,   662,   663,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   664,
     665,     0,     0,  1210,     0,     0,     0,     0,     0,     0,
     653,     0,   654,   655,   656,   657,   658,   659,   660,   661,
     662,   663,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   664,   665,     0,     0,  1213,   633,   634,   635,
     636,   637,   631,   632,   638,   639,   640,   641,     0,   642,
     643,   644,   645,   646,     0,   647,   648,     0,     0,     0,
       0,   649,   650,   651,     0,     0,     0,   652,     0,     0,
       0,     0,     0,   631,   632,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   653,     0,   654,   655,   656,   657,   658,   659,
     660,   661,   662,   663,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   664,   665,     0,     0,  1230,     0,
       0,     0,     0,     0,     0,     0,   633,   634,   635,   636,
     637,     0,     0,   638,   639,   640,   641,     0,   642,   643,
     644,   645,   646,     0,   647,   648,     0,     0,     0,     0,
     649,   650,   651,     0,     0,     0,   652,   633,   634,   635,
     636,   637,   631,   632,   638,   639,   640,   641,     0,   642,
     643,   644,   645,   646,     0,   647,   648,     0,     0,     0,
       0,   649,   650,   651,     0,     0,     0,   652,     0,     0,
       0,   653,     0,   654,   655,   656,   657,   658,   659,   660,
     661,   662,   663,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   664,   665,     0,     0,  1232,     0,     0,
       0,     0,   653,     0,   654,   655,   656,   657,   658,   659,
     660,   661,   662,   663,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   664,   665,     0,     0,  1234,     0,
       0,     0,     0,     0,   631,   632,   633,   634,   635,   636,
     637,     0,     0,   638,   639,   640,   641,     0,   642,   643,
     644,   645,   646,     0,   647,   648,     0,     0,     0,     0,
     649,   650,   651,     0,     0,     0,   652,   631,   632,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   653,     0,   654,   655,   656,   657,   658,   659,   660,
     661,   662,   663,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   664,   665,     0,     0,  1238,   633,   634,
     635,   636,   637,     0,     0,   638,   639,   640,   641,     0,
     642,   643,   644,   645,   646,     0,   647,   648,     0,     0,
       0,     0,   649,   650,   651,     0,     0,     0,   652,   631,
     632,   633,   634,   635,   636,   637,     0,     0,   638,   639,
     640,   641,     0,   642,   643,   644,   645,   646,     0,   647,
     648,     0,     0,     0,     0,   649,   650,   651,     0,     0,
       0,   652,     0,   653,     0,   654,   655,   656,   657,   658,
     659,   660,   661,   662,   663,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   664,   665,     0,     0,  1252,
       0,     0,     0,     0,     0,     0,   653,     0,   654,   655,
     656,   657,   658,   659,   660,   661,   662,   663,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   664,   665,
       0,     0,  1302,   633,   634,   635,   636,   637,   631,   632,
     638,   639,   640,   641,     0,   642,   643,   644,   645,   646,
       0,   647,   648,     0,     0,     0,     0,   649,   650,   651,
       0,     0,     0,   652,     0,     0,     0,     0,     0,   631,
     632,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   653,     0,
     654,   655,   656,   657,   658,   659,   660,   661,   662,   663,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     664,   665,     0,     0,  1303,     0,     0,     0,     0,     0,
       0,     0,   633,   634,   635,   636,   637,     0,     0,   638,
     639,   640,   641,     0,   642,   643,   644,   645,   646,     0,
     647,   648,     0,     0,     0,     0,   649,   650,   651,     0,
       0,     0,   652,   633,   634,   635,   636,   637,   631,   632,
     638,   639,   640,   641,     0,   642,   643,   644,   645,   646,
       0,   647,   648,     0,     0,     0,     0,   649,   650,   651,
       0,     0,     0,   652,     0,     0,     0,   653,     0,   654,
     655,   656,   657,   658,   659,   660,   661,   662,   663,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   664,
     665,     0,     0,  1304,     0,     0,     0,     0,   653,     0,
     654,   655,   656,   657,   658,   659,   660,   661,   662,   663,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     664,   665,     0,     0,  1327,     0,     0,     0,     0,     0,
     631,   632,   633,   634,   635,   636,   637,     0,     0,   638,
     639,   640,   641,     0,   642,   643,   644,   645,   646,     0,
     647,   648,     0,     0,     0,     0,   649,   650,   651,     0,
       0,     0,   652,   631,   632,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   653,     0,   654,
     655,   656,   657,   658,   659,   660,   661,   662,   663,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   664,
     665,     0,     0,  1338,   633,   634,   635,   636,   637,     0,
       0,   638,   639,   640,   641,     0,   642,   643,   644,   645,
     646,     0,   647,   648,     0,     0,     0,     0,   649,   650,
     651,     0,     0,     0,   652,   631,   632,   633,   634,   635,
     636,   637,     0,     0,   638,   639,   640,   641,     0,   642,
     643,   644,   645,   646,     0,   647,   648,     0,     0,     0,
       0,   649,   650,   651,     0,     0,     0,   652,     0,   653,
       0,   654,   655,   656,   657,   658,   659,   660,   661,   662,
     663,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   664,   665,     0,     0,  1348,     0,     0,     0,     0,
       0,     0,   653,     0,   654,   655,   656,   657,   658,   659,
     660,   661,   662,   663,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   664,   665,     0,     0,  1379,   633,
     634,   635,   636,   637,   631,   632,   638,   639,   640,   641,
       0,   642,   643,   644,   645,   646,     0,   647,   648,     0,
       0,     0,     0,   649,   650,   651,     0,     0,     0,   652,
       0,     0,     0,     0,     0,   631,   632,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   653,     0,   654,   655,   656,   657,
     658,   659,   660,   661,   662,   663,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   664,   665,     0,     0,
    1386,     0,     0,     0,     0,     0,     0,     0,   633,   634,
     635,   636,   637,     0,     0,   638,   639,   640,   641,     0,
     642,   643,   644,   645,   646,     0,   647,   648,     0,     0,
       0,     0,   649,   650,   651,   631,   632,     0,   652,   633,
     634,   635,   636,   637,     0,     0,   638,   639,   640,   641,
       0,   642,   643,   644,   645,   646,     0,   647,   648,     0,
       0,     0,     0,   649,   650,   651,   631,   632,     0,   652,
       0,     0,     0,   653,     0,   654,   655,   656,   657,   658,
     659,   660,   661,   662,   663,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   664,   665,   688,     0,     0,
       0,     0,     0,     0,   653,     0,   654,   655,   656,   657,
     658,   659,   660,   661,   662,   663,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   664,   665,   896,   633,
     634,   635,   636,   637,     0,     0,   638,   639,   640,   641,
       0,   642,   643,   644,   645,   646,     0,   647,   648,     0,
       0,     0,     0,   649,   650,   651,   631,   632,     0,   652,
     633,   634,   635,   636,   637,     0,     0,   638,   639,   640,
     641,     0,   642,   643,   644,   645,   646,     0,   647,   648,
       0,     0,     0,     0,   649,   650,   651,   631,   632,     0,
     652,     0,     0,     0,   653,     0,   654,   655,   656,   657,
     658,   659,   660,   661,   662,   663,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   664,   665,  1025,     0,
       0,     0,     0,     0,     0,   653,     0,   654,   655,   656,
     657,   658,   659,   660,   661,   662,   663,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   664,   665,  1040,
     633,   634,   635,   636,   637,     0,     0,   638,   639,   640,
     641,     0,   642,   643,   644,   645,   646,     0,   647,   648,
       0,     0,     0,     0,   649,   650,   651,     0,     0,     0,
     652,   633,   634,   635,   636,   637,   631,   632,   638,   639,
     640,   641,     0,   642,   643,   644,   645,   646,     0,   647,
     648,     0,     0,     0,     0,   649,   650,   651,     0,     0,
       0,   652,     0,     0,     0,   653,     0,   654,   655,   656,
     657,   658,   659,   660,   661,   662,   663,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   664,   665,  1150,
       0,     0,     0,     0,     0,     0,   653,     0,   654,   655,
     656,   657,   658,   659,   660,   661,   662,   663,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   664,   665,
    1155,     0,   735,   736,   737,   738,   739,   740,   741,   742,
     633,   634,   635,   636,   637,   743,   744,   638,   639,   640,
     641,   745,   642,   643,   644,   645,   646,   746,   647,   648,
     747,   748,   266,   267,   649,   650,   651,   749,   750,   751,
     652,     0,     0,     0,     0,     0,     0,     0,     0,   268,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   631,   632,     0,     0,  -315,     0,     0,
       0,     0,     0,     0,   752,   653,     0,   654,   655,   656,
     657,   658,   659,   660,   661,   662,   663,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   664,   665,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,     0,     0,   287,   288,
     289,     0,     0,   290,   291,   292,   293,   294,     0,     0,
     295,   296,   297,   298,   299,   300,   301,   633,   634,   635,
     636,   637,   631,   632,   638,   639,   640,   641,     0,   642,
     643,   644,   645,   646,     0,   647,   648,     0,     0,   803,
       0,   649,   650,   651,     0,     0,     0,   652,     0,     0,
       0,   302,     0,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,     0,     0,   313,   314,     0,     0,     0,
       0,     0,     0,   315,   316,     0,     0,     0,     0,     0,
       0,     0,   653,     0,   654,   655,   656,   657,   658,   659,
     660,   661,   662,   663,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   664,   665,     0,     0,     0,     0,
       0,   631,   632,     0,     0,     0,   633,   634,   635,   636,
     637,     0,     0,   638,   639,   640,   641,     0,   642,   643,
     644,   645,   646,     0,   647,   648,     0,     0,     0,     0,
     649,   650,   651,   631,   632,     0,   652,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   953,     0,     0,     0,     0,     0,     0,
       0,   653,     0,   654,   655,   656,   657,   658,   659,   660,
     661,   662,   663,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   664,   665,   633,   634,   635,   636,   637,
       0,     0,   638,   639,   640,   641,     0,   642,   643,   644,
     645,   646,     0,   647,   648,   631,   632,     0,     0,   649,
     650,   651,     0,     0,     0,   652,     0,   633,   634,   635,
     636,   637,     0,     0,   638,   639,   640,   641,     0,   642,
     643,   644,   645,   646,     0,   647,   648,   631,   632,     0,
       0,   649,   650,   651,     0,     0,     0,   652,     0,     0,
     653,  1030,   654,   655,   656,   657,   658,   659,   660,   661,
     662,   663,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   664,   665,  1085,     0,     0,     0,     0,     0,
       0,     0,   653,     0,   654,   655,   656,   657,   658,   659,
     660,   661,   662,   663,     0,     0,     0,     0,     0,   633,
     634,   635,   636,   637,   664,   665,   638,   639,   640,   641,
       0,   642,   643,   644,   645,   646,     0,   647,   648,   631,
     632,     0,     0,   649,   650,   651,     0,     0,     0,   652,
       0,   633,   634,   635,   636,   637,     0,     0,   638,   639,
     640,   641,     0,   642,   643,   644,   645,   646,     0,   647,
     648,     0,  1116,   631,   632,   649,   650,   651,     0,     0,
       0,   652,     0,     0,   653,     0,   654,   655,   656,   657,
     658,   659,   660,   661,   662,   663,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   664,   665,  1320,     0,
       0,     0,     0,     0,     0,     0,   653,     0,   654,   655,
     656,   657,   658,   659,   660,   661,   662,   663,     0,     0,
       0,     0,     0,   633,   634,   635,   636,   637,   664,   665,
     638,   639,   640,   641,     0,   642,   643,   644,   645,   646,
       0,   647,   648,     0,     0,   631,   632,   649,   650,   651,
       0,     0,     0,   652,     0,     0,     0,   633,   634,   635,
     636,   637,     0,     0,   638,   639,   640,   641,     0,   642,
     643,   644,   645,   646,     0,   647,   648,     0,     0,     0,
    1339,   649,   650,   651,     0,     0,     0,   652,   653,     0,
     654,   655,   656,   657,   658,   659,   660,   661,   662,   663,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     664,   665,     0,     0,  1356,     0,     0,     0,     0,     0,
       0,     0,   653,     0,   654,   655,   656,   657,   658,   659,
     660,   661,   662,   663,   631,   632,     0,     0,     0,   633,
     634,   635,   636,   637,   664,   665,   638,   639,   640,   641,
       0,   642,   643,   644,   645,   646,     0,   647,   648,     0,
       0,     0,     0,   649,   650,   651,     0,     0,     0,   652,
     631,   632,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1381,     0,     0,     0,
       0,     0,     0,     0,   653,     0,   654,   655,   656,   657,
     658,   659,   660,   661,   662,   663,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   664,   665,   633,   634,
     635,   636,   637,     0,     0,   638,   639,   640,   641,     0,
     642,   643,   644,   645,   646,     0,   647,   648,     0,     0,
       0,     0,   649,   650,   651,     0,     0,     0,   652,   631,
     632,     0,     0,     0,   633,   634,   635,   636,   637,     0,
       0,   638,   639,   640,   641,     0,   642,   643,   644,   645,
     646,     0,   647,   648,     0,  1400,     0,     0,   649,   650,
     651,   631,   632,   653,   652,   654,   655,   656,   657,   658,
     659,   660,   661,   662,   663,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   664,   665,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   653,
       0,   654,   655,   656,   657,   658,   659,   660,   661,   662,
     663,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   664,   665,   633,   634,   635,   636,   637,     0,     0,
     638,   639,   640,   641,     0,   642,   643,   644,   645,   646,
       0,   647,   648,   631,   632,     0,     0,   649,   650,   651,
       0,     0,     0,  -676,     0,   633,   634,   635,   636,   637,
       0,     0,   638,   639,   640,   641,     0,   642,   643,   644,
     645,   646,     0,   647,   648,   631,   632,     0,     0,   649,
     650,   651,     0,     0,     0,     0,     0,     0,   653,     0,
     654,   655,   656,   657,   658,   659,   660,   661,   662,   663,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     664,   665,     0,     0,     0,     0,     0,     0,     0,     0,
     653,     0,   654,   655,   656,   657,   658,   659,   660,   661,
     662,   663,     0,     0,     0,     0,     0,   633,   634,   635,
     636,   637,   664,   665,   638,   639,   640,   641,     0,   642,
     643,   644,   645,   646,     0,   647,   648,   631,   632,     0,
       0,   649,     0,     0,     0,     0,     0,     0,     0,   633,
     634,   635,   636,   637,     0,     0,   638,   639,   640,   641,
       0,   642,   643,   644,   645,   646,     0,   647,   648,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   654,   655,   656,   657,   658,   659,
     660,   661,   662,   663,     0,     0,     0,     0,     0,   828,
       0,     0,     0,     0,   664,   665,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   654,   655,   656,   657,
     658,   659,   660,   661,   662,   663,     0,     0,     0,     0,
       0,   633,   634,   635,   636,   637,   664,   665,   638,   639,
     640,   641,     0,   642,   643,   644,   645,   646,     0,   647,
     648,   220,   221,   222,     0,   224,   225,   226,   227,   228,
     417,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,     0,   242,   243,   244,     0,     0,   247,   248,
     249,   250,     0,     0,     0,     0,     0,   832,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   657,   658,   659,   660,   661,   662,   663,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   664,   665,
    1026,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   829,     0,     0,     0,     0,     0,   220,
     221,   222,   830,   224,   225,   226,   227,   228,   417,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
       0,   242,   243,   244,     0,     0,   247,   248,   249,   250,
       0,     0,   220,   221,   222,     0,   224,   225,   226,   227,
     228,   417,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   170,   242,   243,   244,     0,     0,   247,
     248,   249,   250,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   833,     0,     0,     0,     0,     0,     0,     0,   171,
     834,   172,     0,   173,   174,   175,   176,   177,     0,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
       0,   189,   190,   191,  1027,     0,   192,   193,   194,   195,
       0,     0,     0,  1028,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   196,   197,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     198
};

static const yytype_int16 yycheck[] =
{
      14,    15,   625,   164,   565,   703,   526,   588,   478,   484,
     480,   151,   482,   561,   534,   483,   804,   770,   627,   445,
      77,   926,     0,   610,   674,   164,   676,     8,   678,     7,
     827,   367,   619,    20,     5,    33,    15,    16,    20,    22,
      46,    33,  1287,    20,   456,   126,    60,    61,    62,  1200,
      20,   132,    30,   150,    32,  1285,    34,  1321,   394,   150,
      34,   487,    40,   128,   129,   613,   155,    19,    20,   147,
       5,     6,    50,   621,    57,   623,   125,     7,    56,   128,
     129,   128,   129,   126,   162,    99,   100,   101,   102,    63,
      25,   139,   140,   141,  1358,   126,    31,   172,   179,  1344,
     106,   132,    80,   184,   138,   179,   153,    15,    16,   206,
     126,   208,  1342,  1264,  1265,   162,   132,   208,   172,   706,
      50,   153,   138,    62,   102,   103,   162,   205,  1279,   126,
     162,   102,   103,    68,    69,   132,   179,   202,   203,   182,
     126,   172,     5,     6,   181,     8,   132,   181,   179,   206,
     164,   563,   206,   202,   203,   202,   203,   131,   206,   126,
     586,   181,   162,   179,   138,   132,   186,   102,   103,   595,
     172,   207,   598,    36,   206,   172,   155,  1328,  1329,    21,
      22,   160,   179,   162,   163,   205,   184,   162,   162,   986,
     171,   178,   184,   179,   172,   818,   178,    21,    22,   625,
     172,   178,   178,   138,   206,   203,   367,   190,   178,   370,
     162,   209,   179,   191,   185,   172,   191,   629,   178,   202,
     807,   195,   203,   384,   137,   203,   205,   162,   204,   172,
     778,   205,   780,   394,   205,   170,   397,   398,   399,   787,
     254,   667,   790,   170,   126,   384,   206,   155,   172,   206,
     185,   587,   160,   178,   162,   163,   126,   170,   397,   398,
     399,   126,   132,   206,   600,    57,   126,   132,   203,  1174,
     185,    63,   132,   115,   116,   117,   118,   172,    57,   336,
     207,   208,   206,   125,    63,   127,   128,   129,   130,   131,
     205,   133,   134,   117,   118,   172,   178,   179,   172,   181,
     178,   125,   184,   127,   128,   129,   130,   131,   138,   179,
     919,   206,   473,   474,   179,   162,   477,   138,   479,   179,
     481,   147,   483,   178,   178,   172,   147,   178,   206,   206,
      47,   152,   206,   178,   473,   474,   162,   178,   477,   962,
     479,   162,   481,   178,   770,   187,   188,   189,   190,   191,
      67,   206,   206,   367,   824,   206,   370,    60,    61,    62,
     202,   203,   178,  1061,   185,   206,  1016,   178,   162,   204,
     384,  1324,  1325,   178,   388,   178,   178,  1330,   202,   203,
     394,   181,  1335,   397,   398,   399,   186,   185,   204,   138,
     404,   405,   912,    21,    22,   206,    99,   100,   101,   102,
      57,   206,   162,   206,   206,   205,    63,   205,  1161,  1362,
    1363,   181,   181,   162,   185,   890,   186,   186,  1206,    75,
     888,    21,    22,    79,   138,   182,   587,   164,   165,   186,
     766,   767,  1130,   147,   205,   205,   205,    93,    94,   600,
     172,   777,    98,    99,   100,   101,   782,   783,   162,   785,
     786,   172,   788,   789,   186,   791,   185,   185,   182,   473,
     474,   182,   186,   477,   172,   479,   172,   481,   172,   483,
     631,   632,   172,  1123,    21,    22,   205,   205,   186,   172,
     186,   162,   186,    12,   162,   646,   186,   153,  1138,   117,
     118,   164,   165,   186,    23,    24,   162,   125,   153,   127,
     128,   129,   130,   131,   153,  1128,   178,   162,    57,   181,
     153,   153,   184,   162,    63,  1213,  1077,   117,   118,   162,
     162,   181,  1103,   684,   184,   125,   205,   127,   128,   129,
     130,   131,   693,    57,   178,   696,  1006,   181,    57,    63,
     184,    57,   155,  1063,    63,   684,  1066,    63,  1018,  1069,
     172,   182,   172,   162,   693,   186,   178,   696,   178,   187,
     188,   189,   190,   191,   139,   182,   141,    33,   994,   186,
     117,   118,   998,   587,   202,   203,   182,   202,   125,   172,
     186,   128,   129,   130,   131,   178,   600,   164,   182,   189,
     190,   191,   186,   162,    60,    61,   182,  1023,   162,    57,
     186,   162,   202,   203,   765,   766,   767,   768,   162,  1035,
     624,   179,   773,   170,   170,  1176,   777,  1137,   758,   182,
     170,   782,   783,   186,   785,   786,   765,   788,   789,   768,
     791,   967,   968,   969,   773,   139,   972,   182,  1064,   182,
     182,   186,   164,   186,   186,   181,   106,  1195,   208,    35,
    1348,   164,   165,   166,   167,   202,   203,   123,   139,   140,
     141,   127,   164,   165,   166,    10,    11,    12,    35,  1005,
     684,   205,   170,   170,   162,   208,   162,   185,   185,   693,
     185,   182,   696,   162,   205,   185,   185,   205,   205,  1115,
     185,  1272,   185,  1119,  1255,   856,   185,   162,   185,   162,
     185,   185,    22,   162,   181,   181,   162,   162,  1228,   170,
     204,   178,   204,   203,   162,   132,   162,   162,   184,   181,
     162,   187,   205,   189,  1244,  1245,   208,   888,   206,   203,
     205,   185,   185,   185,   185,  1161,   185,   203,   185,   162,
    1076,   205,   185,   209,   185,   906,  1369,   185,   162,   155,
     172,   765,   766,   767,   768,  1316,   171,   162,   171,   773,
     172,   205,   172,   777,    37,   205,   205,   906,   782,   783,
     205,   785,   786,   205,   788,   789,   205,   791,   205,   205,
     204,   172,   172,  1331,  1332,   205,  1334,   205,    10,   205,
     172,   172,   172,   172,   205,   172,   172,    66,   172,   186,
     185,   179,   205,  1139,   205,   178,   967,   968,   969,   178,
     185,   972,    13,   185,   178,     4,   208,   206,   162,   178,
     171,   205,    43,   179,    14,   155,   181,   170,     8,    70,
      21,    22,   186,   162,   205,   171,   204,   178,   185,   185,
     206,   206,   205,   205,  1005,  1271,     1,   205,   205,   162,
     162,  1277,   205,   205,   186,   205,   205,   205,   186,  1379,
     186,   162,    43,  1383,   162,   205,   205,   171,    67,   172,
     186,   186,   206,   206,   888,   186,   186,   186,   205,   186,
     186,   206,   206,   206,   186,    43,   186,   344,   205,   186,
     185,   205,   906,   186,   162,  1056,   162,   354,   206,   162,
     205,   186,   186,   186,   186,   162,   205,   364,   162,   205,
     162,   205,   162,   205,   205,  1076,   206,  1056,  1254,   162,
      21,    22,    12,   162,   115,   116,   117,   118,   119,   386,
      43,   122,   123,   124,   125,    33,   127,   128,   129,   130,
     131,   205,   133,   134,   205,   205,   186,   205,   186,   205,
     205,   186,   205,   967,   968,   969,   172,   206,   972,   205,
     162,   205,   172,   205,   421,   422,    53,   171,   205,   205,
     609,    76,   206,   206,   204,   172,   204,   206,  1139,   206,
    1141,   438,   439,   440,   441,   442,   443,   206,     1,    42,
     128,  1005,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    79,  1141,  1399,  1250,   212,    98,  1250,  1250,  1250,
    1250,   202,   203,     1,   115,   116,   117,   118,   119,  1223,
    1179,   122,   123,   124,   125,  1262,   127,   128,   129,   130,
     131,  1182,   133,   134,  1263,   463,  1081,    51,   139,   496,
     141,   754,  1056,   516,   404,   962,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   512,    -1,    -1,    -1,    -1,
      21,    22,  1076,    -1,    -1,    -1,   404,   524,    -1,    -1,
     527,    -1,    -1,    -1,  1235,    -1,   533,    -1,   535,    -1,
      -1,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,  1254,    -1,    -1,  1235,    -1,    -1,    -1,
      -1,   202,   203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   569,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1139,    -1,  1141,    -1,    -1,
      -1,   588,   589,    -1,    -1,   592,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   603,   604,   605,   606,
     607,   608,    -1,    -1,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    -1,    -1,   633,   634,    -1,    -1,
     637,   638,   639,   640,    -1,   642,    -1,   644,   645,   646,
     647,   648,   649,   650,   651,   652,   653,   654,   655,   656,
     657,   658,   659,   660,   661,   662,   663,    -1,   665,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1235,    -1,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1254,   202,   203,    -1,    -1,    -1,    -1,    -1,   705,    -1,
     707,    -1,    -1,   710,    -1,   712,    -1,    -1,    -1,    -1,
      -1,    -1,   719,    -1,    -1,    -1,    -1,   724,    -1,    -1,
      -1,    -1,    -1,  1287,    -1,    -1,    -1,    -1,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   749,   750,   751,   752,   753,    -1,    -1,    -1,
      -1,    -1,    -1,   760,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   772,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
    1344,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    41,   800,    -1,    -1,   803,   804,    -1,    -1,
      49,   808,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,   825,    -1,
      -1,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   869,    -1,    -1,    -1,   873,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   895,   138,
      -1,    -1,    -1,    -1,   901,    -1,    -1,    -1,    -1,    -1,
      -1,   908,   151,    -1,    -1,    -1,    -1,    -1,   915,    19,
      -1,    -1,    -1,   162,   921,    25,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,   207,    -1,
     209,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,  1003,  1004,    -1,    -1,
    1007,  1008,  1009,    -1,  1011,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1020,    -1,  1022,    -1,  1024,    -1,    -1,
      -1,    -1,    -1,  1030,    -1,    -1,    -1,  1034,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   162,    -1,    19,    21,    22,    -1,    -1,    -1,
      25,  1068,   172,    -1,  1071,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1103,   207,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
    1147,    -1,  1149,    -1,    -1,    -1,    -1,  1154,    -1,   115,
     116,   117,   118,   119,    -1,    -1,   122,  1164,    -1,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
    1177,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1188,    -1,    -1,    -1,    -1,   151,    -1,    -1,  1196,
    1197,  1198,    -1,    -1,    -1,    -1,  1203,   162,    -1,  1206,
    1207,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1216,
    1217,  1218,    -1,    -1,    -1,    -1,    -1,    -1,  1225,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,
      -1,    -1,   207,  1250,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1272,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1286,
      -1,    -1,  1289,  1290,    -1,    -1,    -1,    -1,    -1,  1296,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1308,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
    1317,    -1,     5,     6,     7,    -1,     9,    10,    11,  1326,
      13,    -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    29,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    39,  1354,    -1,    42,
      -1,    44,    45,    -1,    -1,    48,    -1,    50,    51,    52,
      -1,    54,    55,    -1,  1371,    58,    59,    -1,    -1,  1376,
    1377,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
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
      -1,   194,   195,    -1,    -1,    -1,     5,     6,    -1,    -1,
     203,    -1,   205,    -1,   207,   208,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    52,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,
     149,   150,    -1,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,
     189,    -1,   191,    -1,    -1,   194,   195,    -1,    -1,    -1,
      -1,    -1,     5,     6,   203,    -1,   205,    -1,   207,   208,
      13,    -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,   188,   189,    -1,   191,    -1,
      -1,   194,   195,    -1,    -1,    -1,    -1,    -1,     5,     6,
     203,    -1,   205,    -1,   207,   208,    13,    -1,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,   188,   189,    -1,   191,    -1,    -1,   194,   195,    -1,
      -1,    -1,     5,     6,    -1,    -1,   203,    -1,   205,    -1,
     207,   208,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,   188,   189,    -1,   191,    -1,
      -1,   194,   195,    -1,    -1,    -1,     5,     6,    -1,    -1,
     203,    -1,   205,   206,   207,   208,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,
     149,   150,    -1,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,
     189,    -1,   191,    -1,    -1,   194,   195,    -1,    -1,    -1,
      -1,    -1,     5,     6,   203,   204,   205,    -1,   207,   208,
      13,    -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,   188,   189,    -1,   191,    -1,
      -1,   194,   195,    -1,    -1,    -1,     5,     6,    -1,    -1,
     203,    -1,   205,    -1,   207,   208,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,
     149,   150,    -1,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,
     189,    -1,   191,    -1,    -1,   194,   195,    -1,    -1,    -1,
       5,     6,    -1,    -1,   203,   204,   205,    -1,   207,   208,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   146,   147,   148,   149,   150,    -1,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,
     195,    -1,    -1,    -1,     5,     6,    -1,    -1,   203,    -1,
     205,    -1,   207,   208,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   146,   147,   148,   149,   150,
      -1,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   187,   188,   189,    -1,
     191,    -1,    -1,   194,   195,    -1,    -1,    -1,     5,     6,
      -1,    -1,   203,    -1,   205,    -1,   207,   208,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,   188,   189,    -1,   191,    -1,    -1,   194,   195,    -1,
      -1,    -1,     5,     6,    -1,    -1,   203,    -1,   205,    -1,
     207,   208,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,   188,   189,    -1,   191,    -1,
      -1,   194,   195,    -1,    -1,    -1,     5,     6,    -1,    -1,
     203,    -1,   205,    -1,   207,   208,    15,    16,    17,    18,
      19,    -1,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,
     149,   150,    -1,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,
     189,    -1,   191,    -1,    -1,   194,   195,    -1,    -1,    -1,
       5,     6,    -1,    -1,   203,    -1,   205,    -1,   207,   208,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   146,   147,   148,   149,   150,    -1,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,
     195,    -1,    -1,    -1,     5,     6,    -1,    -1,   203,    -1,
     205,   206,   207,   208,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   146,   147,   148,   149,   150,
      -1,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   187,   188,   189,    -1,
     191,    -1,    -1,   194,   195,    -1,    -1,    -1,     5,     6,
      -1,    -1,   203,    -1,   205,   206,   207,   208,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,   188,   189,    -1,   191,    -1,    -1,   194,   195,    -1,
      -1,    -1,     5,     6,    -1,    -1,   203,    -1,   205,   206,
     207,   208,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,   188,   189,    -1,   191,    -1,
      -1,   194,   195,    -1,    -1,    -1,     5,     6,    -1,    -1,
     203,    -1,   205,   206,   207,   208,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,
     149,   150,    -1,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,
     189,    -1,   191,    -1,    -1,   194,   195,    -1,    -1,    -1,
       5,     6,    -1,    -1,   203,    -1,   205,   206,   207,   208,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   146,   147,   148,   149,   150,    -1,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,
     195,    -1,    -1,    -1,     5,     6,    -1,    -1,   203,    -1,
     205,    -1,   207,   208,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   146,   147,   148,   149,   150,
      -1,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   187,   188,   189,    -1,
     191,    -1,    -1,   194,   195,    -1,    -1,    -1,     5,     6,
      -1,    -1,   203,    -1,   205,    -1,   207,   208,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    21,    22,    -1,
      -1,    19,    -1,   170,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
     187,   188,   189,    41,   191,    -1,    -1,   194,   195,    -1,
      -1,    49,    -1,    21,    22,    -1,   203,    -1,   205,    -1,
     207,   208,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
     138,   145,    -1,    -1,    -1,    21,    22,   115,   116,   117,
     118,   119,    -1,   151,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,   162,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,   180,   145,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,   207,
      -1,    -1,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,   115,
     116,   117,   118,   119,    21,    22,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,
     206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,   115,
     116,   117,   118,   119,    21,    22,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,
      -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,
     206,    -1,    -1,    -1,    -1,    -1,    21,    22,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,
     145,    21,    22,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,
      -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     202,   203,    -1,    -1,   206,   115,   116,   117,   118,   119,
      21,    22,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,    -1,    -1,   206,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,   145,   115,   116,   117,   118,   119,
      21,    22,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,   180,
      -1,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,    -1,    -1,   206,    -1,    -1,    -1,    -1,
     180,    -1,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,    -1,    -1,   206,    -1,    -1,    -1,
      -1,    -1,    21,    22,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,   145,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,    -1,    -1,   206,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,   141,    -1,    -1,    -1,   145,    21,    22,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,    -1,    -1,   206,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,
     206,   115,   116,   117,   118,   119,    21,    22,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,
     145,   115,   116,   117,   118,   119,    21,    22,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,
      -1,   206,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    21,    22,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,
     145,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,
      -1,   206,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,
      -1,    -1,   145,    21,    22,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,   180,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,    -1,    -1,   206,   115,   116,   117,
     118,   119,    21,    22,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,   141,    -1,    -1,    -1,   145,   115,   116,   117,
     118,   119,    21,    22,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,    -1,    -1,   206,    -1,    -1,
      -1,    -1,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,    -1,
      -1,    -1,    -1,    -1,    21,    22,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,   141,    -1,    -1,    -1,   145,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,    -1,    -1,   206,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,    21,
      22,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
      -1,    -1,   206,   115,   116,   117,   118,   119,    21,    22,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     202,   203,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,
      -1,    -1,   145,   115,   116,   117,   118,   119,    21,    22,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,   180,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,    -1,    -1,   206,    -1,    -1,    -1,    -1,   180,    -1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     202,   203,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,
      21,    22,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,
      -1,    -1,   145,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,    -1,    -1,   206,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,   145,    21,    22,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,   180,
      -1,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,    -1,    -1,   206,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,   115,
     116,   117,   118,   119,    21,    22,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,
     206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
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
      -1,    -1,    -1,   139,   140,   141,    21,    22,    -1,   145,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,   139,   140,   141,    21,    22,    -1,
     145,    -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   204,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,
     145,   115,   116,   117,   118,   119,    21,    22,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
     204,    -1,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,    21,    22,   139,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,   172,    -1,    -1,
      -1,    -1,    -1,    -1,   179,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    -1,    -1,   127,   128,
     129,    -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,
     139,   140,   141,   142,   143,   144,   145,   115,   116,   117,
     118,   119,    21,    22,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,   137,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,   194,   195,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,   141,    21,    22,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    21,    22,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    21,    22,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,   115,
     116,   117,   118,   119,   202,   203,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    21,
      22,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,   168,    21,    22,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,   115,   116,   117,   118,   119,   202,   203,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    21,    22,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
     172,   139,   140,   141,    -1,    -1,    -1,   145,   180,    -1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     202,   203,    -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    21,    22,    -1,    -1,    -1,   115,
     116,   117,   118,   119,   202,   203,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,    21,
      22,    -1,    -1,    -1,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    -1,   172,    -1,    -1,   139,   140,
     141,    21,    22,   180,   145,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    21,    22,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    21,    22,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     202,   203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,   202,   203,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    21,    22,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,   202,   203,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,   115,   116,   117,   118,   119,   202,   203,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,   162,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
      -1,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    35,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
     162,    73,    -1,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,   153,    -1,    98,    99,   100,   101,
      -1,    -1,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
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
     224,   228,   254,   258,   281,   355,   362,   366,   375,   420,
     424,   428,    19,    20,   162,   246,   247,   248,   155,   229,
     230,   162,   191,   225,   226,    57,    63,   359,   360,   162,
     207,   214,   359,   359,   359,   138,   162,   269,    34,    63,
     131,   195,   205,   250,   251,   252,   253,   269,   172,     5,
       6,     8,    36,   372,    62,   353,   179,   178,   181,   178,
     225,    22,    57,   190,   202,   227,   162,   172,   353,   162,
     162,   162,   162,   138,   222,   252,   252,   252,   205,   139,
     140,   141,   178,   204,    57,    63,   259,   261,    57,    63,
     363,    57,    63,   373,    57,    63,   354,    15,    16,   155,
     160,   162,   163,   205,   217,   247,   155,   230,   162,   162,
     162,   361,    57,    63,   213,   429,   421,   425,   162,   164,
     220,   206,   248,   252,   252,   252,   252,   262,   162,   364,
     376,   356,   164,   165,   216,    15,    16,   155,   160,   162,
     217,   244,   245,   227,   179,   170,   170,   170,   164,   206,
      35,    71,    73,    75,    76,    77,    78,    79,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    93,
      94,    95,    98,    99,   100,   101,   117,   118,   162,   257,
     260,   181,   365,   106,   370,   371,   208,   249,   327,   164,
     165,   166,   178,   206,    19,    25,    31,    41,    49,    64,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   151,   207,   269,   379,   381,   382,   385,
     390,   391,   419,   430,   422,   426,    21,    22,    38,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   127,   128,   129,
     132,   133,   134,   135,   136,   139,   140,   141,   142,   143,
     144,   145,   180,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   194,   195,   202,   203,    35,    35,   205,
     255,   170,   263,    75,    79,    93,    94,    98,    99,   100,
     101,   380,   170,   162,   377,   247,   208,   162,   350,   352,
     244,   185,   185,   185,   205,   185,   185,   205,   185,   185,
     185,   185,   185,   185,   205,   269,    33,    60,    61,   123,
     127,   184,   187,   189,   203,   209,   389,   182,   162,   384,
     341,   344,   162,   162,   162,   204,    22,   162,   204,   150,
     206,   327,   337,   338,   181,   256,   266,   367,   181,   369,
     170,   374,   247,   178,   181,   184,   348,   392,   397,   399,
       5,     6,    15,    16,    17,    18,    19,    25,    27,    31,
      39,    45,    48,    51,    55,    68,    69,    80,   102,   103,
     104,   117,   118,   146,   147,   148,   149,   150,   152,   154,
     155,   156,   157,   158,   159,   160,   161,   163,   170,   187,
     188,   189,   194,   195,   203,   205,   207,   208,   219,   221,
     264,   269,   274,   286,   293,   296,   299,   303,   305,   307,
     308,   310,   315,   318,   319,   326,   379,   432,   437,   447,
     450,   462,   465,   401,   395,   162,   386,   403,   405,   407,
     409,   411,   413,   415,   417,   319,   185,   205,    33,   184,
      33,   184,   203,   209,   204,   319,   203,   209,   390,   178,
     464,   162,   172,   339,   419,   423,   162,   172,   342,   427,
     162,   132,   205,     7,    50,   280,   172,   206,   419,     1,
       9,    10,    11,    13,    26,    28,    29,    38,    42,    44,
      52,    54,    58,    59,    65,   105,   171,   172,   231,   232,
     235,   237,   238,   239,   240,   241,   242,   243,   265,   270,
     275,   276,   277,   278,   279,   281,   285,   306,   319,   162,
     357,   358,   269,   333,   162,   390,   126,   132,   179,   347,
     419,   419,   388,   419,   185,   185,   185,   271,   381,   432,
     269,   185,     5,   102,   103,   185,   205,   185,   205,   205,
     185,   185,   205,   185,   185,   205,   185,   185,   205,   185,
     185,   319,   319,   205,   205,   205,   205,   205,   205,   218,
      13,   319,   446,   461,   319,   319,   319,   319,   319,    13,
      49,   297,   319,   297,   208,   205,   254,   170,   208,   299,
     304,    21,    22,   115,   116,   117,   118,   119,   122,   123,
     124,   125,   127,   128,   129,   130,   131,   133,   134,   139,
     140,   141,   145,   180,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   202,   203,   206,   205,   419,   419,
     206,   162,   383,   419,   255,   419,   255,   419,   255,   339,
     340,   342,   343,   206,   394,   267,   297,   204,   204,   204,
     319,   162,   431,   181,   172,   171,   181,   172,   171,   319,
     147,   162,   346,   378,   337,   205,   205,   126,   319,   263,
      61,   319,   205,   162,   172,   155,    58,   319,   263,   126,
     319,    37,   172,   172,   205,    10,   172,   172,   172,   172,
     172,   172,    66,   282,   172,   107,   108,   109,   110,   111,
     112,   113,   114,   120,   121,   126,   132,   135,   136,   142,
     143,   144,   179,   179,   178,   464,   171,   254,   334,   172,
     347,   319,   186,   186,   186,   172,   438,   440,   272,   205,
     205,   185,   205,   294,   185,   185,   185,   457,   297,   390,
     461,   319,   287,   289,   319,   291,   459,   297,   444,   448,
     297,   442,   390,   319,   319,   319,   319,   319,   319,   166,
     167,   216,   378,   137,   178,   464,   378,    13,   178,   464,
     464,   147,   152,   185,   269,   309,   153,   162,   203,   206,
     297,   433,   434,     4,   302,   266,   208,   254,    19,   153,
     162,   379,    19,   153,   162,   379,   319,   319,   319,   319,
     319,   319,   162,   319,   153,   162,   319,   319,   319,   379,
     319,   319,   319,   319,   319,   319,    22,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   128,   129,
     153,   162,   202,   203,   316,   319,   206,   297,   186,   186,
     172,   186,   186,   256,   186,   256,   186,   256,   172,   186,
     172,   186,   268,   419,   206,   178,   204,   171,   419,   419,
     206,   205,    43,   126,   178,   179,   181,   184,   345,   319,
     378,   319,    14,   319,   319,   179,   181,   155,   319,   170,
     319,   205,   147,   162,   205,   284,   349,   351,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   357,   368,     8,
     327,   332,   319,   172,   393,   398,   400,   419,   390,   390,
     419,    70,   436,   267,   162,   319,   419,   451,   453,   455,
     390,   464,   172,   186,   464,   206,   390,   390,   206,   390,
     390,   464,   390,   390,   464,   390,   186,   206,   206,   206,
     206,   206,   206,   319,    20,   319,   446,   171,    20,   378,
     319,   204,   206,   205,   205,   311,   313,   205,   132,   345,
     433,   178,   206,   178,   206,   205,   255,   171,   302,   185,
     205,   185,   205,   205,   205,   204,    19,   153,   162,   379,
     181,   153,   162,   319,   205,   205,   153,   162,   319,     1,
     204,   206,   402,   396,   162,   387,   404,   186,   408,   186,
     412,   186,   339,   416,   342,   418,   172,   186,   319,   162,
     162,   419,   319,   206,    20,   263,   206,   319,   266,   206,
     319,   205,    43,   162,   283,   178,   181,   348,   171,    57,
      63,   330,    67,   331,   172,   172,   186,   186,   186,   162,
     206,   434,   206,   172,   186,   206,   186,   390,   390,   390,
     186,   206,   390,   205,   206,   186,   186,   186,   186,   206,
     186,   186,   206,   186,   302,   205,   168,   297,   297,    20,
     319,   319,   390,   255,   319,   319,   319,   204,   203,   153,
     162,   126,   132,   179,   184,   300,   301,   256,   255,   320,
     319,   322,   319,   206,   297,   319,   185,   205,   319,   205,
     204,   319,   206,   297,   205,   204,   317,   406,   410,   414,
     419,   205,   206,    43,   345,   263,   297,   263,   171,   263,
     206,   319,   162,   178,   206,   162,   390,   347,    47,   331,
      46,   106,   328,   439,   441,   273,   206,   162,   205,   295,
     186,   186,   186,   458,   186,   461,   288,   290,   292,   460,
     445,   449,   443,   205,   206,   297,   172,   172,   297,   206,
     206,   186,   256,   206,   206,   433,   205,   132,   345,   162,
     162,   162,   162,   178,   206,   137,   263,   298,   256,   390,
     206,   419,   206,   206,   206,   324,   319,   319,   206,   206,
     319,   267,   162,   319,   206,    12,    23,    24,   233,   234,
      12,   236,   206,   162,   181,   348,    43,   172,   347,   319,
      33,   329,   328,   330,   205,   205,   186,   319,   452,   454,
     456,   205,   205,   464,   319,   319,   319,   205,   436,   205,
     205,   206,   319,   206,   446,   319,   172,   312,   186,   132,
     345,   204,   319,   319,   319,   300,   126,   319,   263,   186,
     186,   419,   206,   206,   206,   206,   263,   263,   205,   237,
     275,   276,   277,   278,   319,   172,   390,   347,   162,   319,
     172,   335,   329,   346,   436,   436,   205,   206,   205,   205,
     205,   297,   461,   206,   297,   436,   433,   435,   206,   172,
     463,   463,   319,   309,   314,   319,   319,   206,   206,   319,
     321,   323,   186,   319,   347,   319,   172,   260,   336,   435,
     435,   319,   436,   436,   435,   464,   464,   464,   435,   172,
     206,    53,   171,   204,   463,   309,   132,   345,   325,   206,
     319,   172,   172,   260,   206,   206,   206,   435,   435,   206,
     206,   206,   206,   206,   433,   319,   204,   319,   319,   263,
     172,   263,   206,   206,   234
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
     269,   269,   269,   270,   270,   272,   273,   271,   274,   274,
     274,   274,   274,   275,   276,   277,   277,   277,   278,   278,
     279,   280,   280,   280,   281,   281,   282,   282,   283,   283,
     284,   284,   285,   285,   287,   288,   286,   289,   290,   286,
     291,   292,   286,   294,   295,   293,   296,   296,   296,   297,
     297,   298,   298,   298,   299,   299,   299,   300,   300,   300,
     300,   301,   301,   302,   302,   303,   304,   304,   305,   305,
     305,   305,   305,   305,   305,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   307,   307,   308,   308,   309,   309,
     310,   311,   312,   310,   313,   314,   310,   315,   315,   315,
     315,   316,   317,   315,   318,   318,   318,   318,   318,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   320,   321,   319,   319,   319,
     319,   322,   323,   319,   319,   319,   324,   325,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   327,   327,   328,
     328,   328,   329,   329,   330,   330,   330,   331,   331,   332,
     333,   334,   333,   335,   333,   336,   333,   337,   337,   338,
     338,   339,   339,   340,   340,   341,   341,   341,   342,   343,
     343,   344,   344,   344,   345,   345,   346,   346,   346,   346,
     346,   347,   347,   347,   348,   348,   349,   349,   349,   349,
     349,   350,   350,   351,   351,   351,   352,   352,   352,   353,
     353,   354,   354,   354,   356,   355,   357,   357,   358,   358,
     358,   359,   359,   359,   361,   360,   362,   363,   363,   363,
     364,   365,   365,   367,   368,   366,   369,   369,   370,   370,
     371,   372,   372,   373,   373,   373,   374,   374,   376,   377,
     375,   378,   378,   378,   378,   378,   379,   379,   379,   379,
     379,   379,   379,   379,   379,   379,   379,   379,   379,   379,
     379,   379,   379,   379,   379,   379,   379,   379,   379,   379,
     379,   379,   379,   380,   380,   380,   380,   380,   380,   380,
     380,   381,   382,   382,   382,   383,   383,   384,   384,   384,
     386,   387,   385,   388,   388,   389,   389,   389,   389,   390,
     390,   391,   391,   391,   391,   392,   393,   391,   391,   391,
     394,   391,   391,   391,   391,   391,   391,   391,   391,   391,
     391,   391,   391,   391,   395,   396,   391,   391,   397,   398,
     391,   399,   400,   391,   401,   402,   391,   391,   403,   404,
     391,   405,   406,   391,   391,   407,   408,   391,   409,   410,
     391,   391,   411,   412,   391,   413,   414,   391,   415,   416,
     391,   417,   418,   391,   419,   419,   419,   421,   422,   423,
     420,   425,   426,   427,   424,   429,   430,   431,   428,   432,
     432,   432,   432,   432,   433,   433,   433,   433,   433,   433,
     433,   433,   434,   435,   435,   436,   436,   438,   439,   437,
     440,   441,   437,   442,   443,   437,   444,   445,   437,   446,
     446,   447,   448,   449,   447,   450,   451,   452,   450,   453,
     454,   450,   455,   456,   450,   450,   457,   458,   450,   450,
     459,   460,   450,   461,   461,   462,   462,   462,   462,   463,
     463,   464,   464,   465,   465,   465
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     0,     1,
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
       2,     2,     2,     0,     2,     2,     0,     1,     1,     3,
       1,     3,     2,     2,     3,     0,     0,     5,     2,     5,
       5,     6,     2,     1,     1,     1,     2,     3,     2,     3,
       4,     1,     1,     0,     1,     1,     1,     0,     1,     3,
       8,     7,     3,     3,     0,     0,     7,     0,     0,     7,
       0,     0,     7,     0,     0,     6,     5,     8,    10,     1,
       3,     1,     2,     3,     1,     1,     2,     2,     2,     2,
       2,     1,     3,     0,     4,     6,     6,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     6,     8,     5,     6,     1,     4,
       3,     0,     0,     8,     0,     0,     9,     3,     4,     5,
       6,     0,     0,     5,     3,     4,     4,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     4,
       4,     5,     4,     5,     3,     4,     1,     1,     2,     4,
       4,     7,     8,     3,     5,     0,     0,     8,     3,     3,
       3,     0,     0,     8,     3,     4,     0,     0,     9,     4,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       1,     4,     4,     4,     4,     4,     1,     6,     7,     6,
       6,     7,     7,     6,     7,     6,     6,     0,     1,     0,
       1,     1,     0,     1,     0,     1,     1,     0,     1,     5,
       0,     0,     4,     0,     9,     0,    10,     3,     4,     1,
       3,     1,     3,     1,     3,     0,     2,     3,     3,     1,
       3,     0,     2,     3,     1,     1,     1,     2,     3,     5,
       3,     1,     1,     1,     0,     1,     1,     4,     3,     3,
       5,     1,     3,     4,     6,     5,     4,     6,     5,     0,
       1,     0,     1,     1,     0,     6,     1,     3,     0,     1,
       3,     0,     1,     1,     0,     5,     3,     0,     1,     1,
       1,     0,     2,     0,     0,    11,     0,     2,     0,     1,
       3,     1,     1,     0,     1,     1,     0,     3,     0,     0,
       7,     1,     4,     3,     3,     5,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     4,     1,     3,     0,     1,     3,
       0,     0,     6,     1,     3,     3,     2,     4,     3,     1,
       2,     1,     1,     1,     1,     0,     0,     6,     4,     5,
       0,     9,     4,     2,     2,     3,     2,     3,     2,     2,
       3,     3,     3,     2,     0,     0,     6,     2,     0,     0,
       6,     0,     0,     6,     0,     0,     6,     1,     0,     0,
       6,     0,     0,     7,     1,     0,     0,     6,     0,     0,
       7,     1,     0,     0,     6,     0,     0,     7,     0,     0,
       6,     0,     0,     6,     1,     3,     3,     0,     0,     0,
       9,     0,     0,     0,     9,     0,     0,     0,    10,     1,
       1,     1,     1,     1,     3,     3,     5,     5,     6,     6,
       8,     8,     1,     1,     3,     0,     1,     0,     0,    10,
       0,     0,    10,     0,     0,     9,     0,     0,     7,     3,
       1,     5,     0,     0,    10,     4,     0,     0,    11,     0,
       0,    11,     0,     0,    10,     5,     0,     0,    10,     5,
       0,     0,    10,     1,     3,     4,     5,     8,    10,     0,
       3,     0,     1,     9,    10,     9
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

    case YYSYMBOL_make_struct_single: /* make_struct_single  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_struct_dim: /* make_struct_dim  */
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

  case 237: /* optional_expr_list: expr_list  */
                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 238: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 239: /* type_declaration_no_options_list: type_declaration_no_options_list "end of expression" type_declaration  */
                                                                           {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 240: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 241: /* name_in_namespace: "name" "::" "name"  */
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

  case 242: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 243: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 244: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 245: /* $@3: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 246: /* $@4: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 247: /* new_type_declaration: '<' $@3 type_declaration '>' $@4  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 248: /* expr_new: "new" structure_type_declaration  */
                                                            {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),true);
    }
    break;

  case 249: /* expr_new: "new" structure_type_declaration '(' optional_expr_list ')'  */
                                                                                                   {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 250: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 251: /* expr_new: "new" new_type_declaration '(' use_initializer make_struct_single ')'  */
                                                                                                            {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 252: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 253: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 254: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 255: /* expression_return: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 256: /* expression_return: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 257: /* expression_return: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 258: /* expression_yield: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 259: /* expression_yield: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 260: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 261: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 262: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 263: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 264: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 265: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 266: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 267: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 268: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 269: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 270: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 271: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 272: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 273: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 274: /* $@5: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 275: /* $@6: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 276: /* expr_cast: "cast" '<' $@5 type_declaration_no_options '>' $@6 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 277: /* $@7: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 278: /* $@8: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 279: /* expr_cast: "upcast" '<' $@7 type_declaration_no_options '>' $@8 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 280: /* $@9: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 281: /* $@10: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 282: /* expr_cast: "reinterpret" '<' $@9 type_declaration_no_options '>' $@10 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 283: /* $@11: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 284: /* $@12: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 285: /* expr_type_decl: "type" '<' $@11 type_declaration '>' $@12  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 286: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 287: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 288: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" "end of expression" "name" '>' '(' expr ')'  */
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

  case 289: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 290: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 291: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 292: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 293: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 294: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 295: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 296: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 297: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 298: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 299: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 300: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 301: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 302: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 303: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 304: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 305: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 306: /* expr_full_block_assumed_piped: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 307: /* expr_full_block_assumed_piped: "begin of code block" expressions "end of code block"  */
                                   {
        (yyval.pExpression) = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,new TypeDecl(Type::autoinfer),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 308: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 309: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 310: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 311: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 312: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 313: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 314: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 315: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 316: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 317: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 318: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 319: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 320: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 321: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 322: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 323: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 324: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 325: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 326: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 327: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 328: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 329: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 330: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 331: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 332: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 333: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 334: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 335: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 336: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 337: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 338: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 339: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 340: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 341: /* $@13: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 342: /* $@14: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 343: /* func_addr_expr: '@' '@' '<' $@13 type_declaration_no_options '>' $@14 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 344: /* $@15: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 345: /* $@16: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 346: /* func_addr_expr: '@' '@' '<' $@15 optional_function_argument_list optional_function_type '>' $@16 func_addr_name  */
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

  case 347: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 348: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 349: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 350: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 351: /* $@17: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 352: /* $@18: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 353: /* expr_field: expr '.' $@17 error $@18  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 354: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 355: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 356: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 357: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 358: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 359: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 360: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 361: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 362: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 363: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 364: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 365: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 366: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 367: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 368: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 369: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 370: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 371: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 372: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 392: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 393: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 394: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 395: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 396: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 397: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 398: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 399: /* expr: '(' expr_list optional_comma ')'  */
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

  case 400: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 401: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 402: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 403: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 404: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 405: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 406: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 407: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 408: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 409: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 410: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 411: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 412: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 413: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 414: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 415: /* $@19: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 416: /* $@20: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 417: /* expr: expr "is" "type" '<' $@19 type_declaration_no_options '>' $@20  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 418: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 419: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 420: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 421: /* $@21: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 422: /* $@22: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 423: /* expr: expr "as" "type" '<' $@21 type_declaration '>' $@22  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 424: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 425: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 426: /* $@23: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 427: /* $@24: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 428: /* expr: expr '?' "as" "type" '<' $@23 type_declaration '>' $@24  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 429: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 430: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 431: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 432: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 433: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 434: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 435: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 436: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 437: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 438: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 439: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 440: /* expr: expr_call_pipe  */
                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 441: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 442: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 443: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 444: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 445: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 446: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 447: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 448: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 449: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 450: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 451: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 452: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 453: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 454: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 455: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 456: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 457: /* optional_field_annotation: %empty  */
                                      { (yyval.aaList) = nullptr; }
    break;

  case 458: /* optional_field_annotation: metadata_argument_list  */
                                      { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 459: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 460: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 461: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 462: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 463: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 464: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 465: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 466: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 467: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 468: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 469: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 470: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 471: /* $@25: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 472: /* struct_variable_declaration_list: struct_variable_declaration_list $@25 structure_variable_declaration "end of expression"  */
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

  case 473: /* $@26: %empty  */
                                                                                                                     {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 474: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@26 function_declaration_header "end of expression"  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 475: /* $@27: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 476: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@27 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 477: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 478: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 479: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 480: /* function_argument_list: function_argument_list "end of expression" function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 481: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 482: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 483: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 484: /* tuple_type_list: tuple_type_list "end of expression" tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 485: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 486: /* tuple_alias_type_list: tuple_alias_type_list "end of expression"  */
                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 487: /* tuple_alias_type_list: tuple_alias_type_list tuple_type "end of expression"  */
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

  case 488: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 489: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 490: /* variant_type_list: variant_type_list "end of expression" variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 491: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 492: /* variant_alias_type_list: variant_alias_type_list "end of expression"  */
                                        {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 493: /* variant_alias_type_list: variant_alias_type_list variant_type "end of expression"  */
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

  case 501: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 502: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 503: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 504: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 505: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 506: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 507: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 508: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 509: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 510: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 511: /* global_let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 512: /* global_let_variable_name_with_pos_list: global_let_variable_name_with_pos_list ',' "name"  */
                                                                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 513: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options "end of expression"  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 514: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 515: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 516: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options "end of expression"  */
                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 517: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                         {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 518: /* global_let_variable_declaration: global_let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                                 {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 519: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 520: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 521: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 522: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 523: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 524: /* $@28: %empty  */
                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 525: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@28 optional_field_annotation global_let_variable_declaration  */
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

  case 526: /* enum_expression: "name"  */
                   {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 527: /* enum_expression: "name" '=' expr  */
                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[-2].s),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 528: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 529: /* enum_list: enum_expression  */
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

  case 530: /* enum_list: enum_list ',' enum_expression  */
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

  case 531: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 532: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 533: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 534: /* $@29: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 535: /* single_alias: optional_public_or_private_alias "name" $@29 '=' type_declaration  */
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

  case 537: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 538: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 539: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 540: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 541: /* optional_enum_basic_type_declaration: %empty  */
        {
        (yyval.type) = Type::tInt;
    }
    break;

  case 542: /* optional_enum_basic_type_declaration: ':' enum_basic_type_declaration  */
                                              {
        (yyval.type) = (yyvsp[0].type);
    }
    break;

  case 543: /* $@30: %empty  */
                                                                                                                                                          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 544: /* $@31: %empty  */
                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 545: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name optional_enum_basic_type_declaration "begin of code block" $@30 enum_list optional_comma $@31 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-3].pEnum),(yyvsp[-6].type));
    }
    break;

  case 546: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 547: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 548: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 549: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 550: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 551: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 552: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 553: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 554: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 555: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 556: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 557: /* optional_struct_variable_declaration_list: "begin of code block" struct_variable_declaration_list "end of code block"  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 558: /* $@32: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 559: /* $@33: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 560: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@32 structure_name $@33 optional_struct_variable_declaration_list  */
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

  case 561: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 562: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 563: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 564: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 565: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 566: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 567: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 568: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 569: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 570: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 571: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 572: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 573: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 574: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 575: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 576: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 577: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 578: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 579: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 580: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 581: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 582: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 583: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 584: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 585: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 586: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 587: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 588: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 589: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 590: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 591: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 592: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 593: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 594: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 595: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 596: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 597: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 598: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 599: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 600: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 601: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 602: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 603: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 604: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 605: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 606: /* bitfield_bits: bitfield_bits "end of expression" "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 607: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 608: /* bitfield_alias_bits: "name"  */
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

  case 609: /* bitfield_alias_bits: bitfield_alias_bits ',' "name"  */
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

  case 610: /* $@34: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 611: /* $@35: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 612: /* bitfield_type_declaration: "bitfield" '<' $@34 bitfield_bits '>' $@35  */
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

  case 613: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 614: /* table_type_pair: type_declaration "end of expression" type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 615: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 616: /* dim_list: '[' ']'  */
                {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 617: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 618: /* dim_list: dim_list '[' ']'  */
                              {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 619: /* type_declaration_no_options: type_declaration_no_options_no_dim  */
                                                     {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 620: /* type_declaration_no_options: type_declaration_no_options_no_dim dim_list  */
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

  case 621: /* type_declaration_no_options_no_dim: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 622: /* type_declaration_no_options_no_dim: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 623: /* type_declaration_no_options_no_dim: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 624: /* type_declaration_no_options_no_dim: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 625: /* $@36: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 626: /* $@37: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 627: /* type_declaration_no_options_no_dim: "type" '<' $@36 type_declaration '>' $@37  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 628: /* type_declaration_no_options_no_dim: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 629: /* type_declaration_no_options_no_dim: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 630: /* $@38: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 631: /* type_declaration_no_options_no_dim: '$' name_in_namespace '<' $@38 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 632: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' '[' ']'  */
                                                                 {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 633: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "explicit"  */
                                                                  {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 634: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "const"  */
                                                               {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 635: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' "const"  */
                                                                   {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 636: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '&'  */
                                                         {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 637: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' '&'  */
                                                             {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 638: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '#'  */
                                                         {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 639: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "implicit"  */
                                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 640: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' '#'  */
                                                             {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 641: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "==" "const"  */
                                                                      {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 642: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "==" '&'  */
                                                                {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 643: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '*'  */
                                                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 644: /* $@39: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 645: /* $@40: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 646: /* type_declaration_no_options_no_dim: "smart_ptr" '<' $@39 type_declaration '>' $@40  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 647: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "??"  */
                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 648: /* $@41: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 649: /* $@42: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 650: /* type_declaration_no_options_no_dim: "array" '<' $@41 type_declaration '>' $@42  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 651: /* $@43: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 652: /* $@44: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 653: /* type_declaration_no_options_no_dim: "table" '<' $@43 table_type_pair '>' $@44  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 654: /* $@45: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 655: /* $@46: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 656: /* type_declaration_no_options_no_dim: "iterator" '<' $@45 type_declaration '>' $@46  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 657: /* type_declaration_no_options_no_dim: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 658: /* $@47: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 659: /* $@48: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 660: /* type_declaration_no_options_no_dim: "block" '<' $@47 type_declaration '>' $@48  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 661: /* $@49: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 662: /* $@50: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 663: /* type_declaration_no_options_no_dim: "block" '<' $@49 optional_function_argument_list optional_function_type '>' $@50  */
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

  case 664: /* type_declaration_no_options_no_dim: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 665: /* $@51: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 666: /* $@52: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 667: /* type_declaration_no_options_no_dim: "function" '<' $@51 type_declaration '>' $@52  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 668: /* $@53: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 669: /* $@54: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 670: /* type_declaration_no_options_no_dim: "function" '<' $@53 optional_function_argument_list optional_function_type '>' $@54  */
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

  case 671: /* type_declaration_no_options_no_dim: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 672: /* $@55: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 673: /* $@56: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 674: /* type_declaration_no_options_no_dim: "lambda" '<' $@55 type_declaration '>' $@56  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 675: /* $@57: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 676: /* $@58: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 677: /* type_declaration_no_options_no_dim: "lambda" '<' $@57 optional_function_argument_list optional_function_type '>' $@58  */
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

  case 678: /* $@59: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 679: /* $@60: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 680: /* type_declaration_no_options_no_dim: "tuple" '<' $@59 tuple_type_list '>' $@60  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 681: /* $@61: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 682: /* $@62: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 683: /* type_declaration_no_options_no_dim: "variant" '<' $@61 variant_type_list '>' $@62  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 684: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 685: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 686: /* type_declaration: type_declaration '|' '#'  */
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

  case 687: /* $@63: %empty  */
                                                                      {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 688: /* $@64: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 689: /* $@65: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 690: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias "name" $@63 "begin of code block" $@64 tuple_alias_type_list $@65 "end of code block"  */
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

  case 691: /* $@66: %empty  */
                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 692: /* $@67: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 693: /* $@68: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 694: /* variant_alias_declaration: "variant" optional_public_or_private_alias "name" $@66 "begin of code block" $@67 variant_alias_type_list $@68 "end of code block"  */
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

  case 695: /* $@69: %empty  */
                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 696: /* $@70: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 697: /* $@71: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 698: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias "name" $@69 "begin of code block" $@70 bitfield_alias_bits optional_comma $@71 "end of code block"  */
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

  case 699: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 700: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 701: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 702: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 703: /* make_decl: make_tuple_call  */
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

  case 712: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 713: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 714: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 715: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 716: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 717: /* $@72: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 718: /* $@73: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 719: /* make_struct_decl: "struct" '<' $@72 type_declaration_no_options '>' $@73 '(' use_initializer make_struct_dim ')'  */
                                                                                                                                                                                        {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 720: /* $@74: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 721: /* $@75: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 722: /* make_struct_decl: "class" '<' $@74 type_declaration_no_options '>' $@75 '(' use_initializer make_struct_dim ')'  */
                                                                                                                                                                                       {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 723: /* $@76: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 724: /* $@77: %empty  */
                                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 725: /* make_struct_decl: "variant" '<' $@76 type_declaration_no_options '>' $@77 '(' make_struct_dim ')'  */
                                                                                                                                                                    {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 726: /* $@78: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 727: /* $@79: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 728: /* make_struct_decl: "default" '<' $@78 type_declaration_no_options '>' $@79 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 729: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 730: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 731: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 732: /* $@80: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 733: /* $@81: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 734: /* make_tuple_call: "tuple" '<' $@80 type_declaration_no_options '>' $@81 '(' use_initializer make_struct_dim ')'  */
                                                                                                                                                                                        {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 735: /* make_dim_decl: '[' expr_list optional_comma ']'  */
                                                          {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 736: /* $@82: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 737: /* $@83: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 738: /* make_dim_decl: "array" "struct" '<' $@82 type_declaration_no_options '>' $@83 '(' use_initializer make_struct_dim ')'  */
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

  case 739: /* $@84: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 740: /* $@85: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 741: /* make_dim_decl: "array" "tuple" '<' $@84 type_declaration_no_options '>' $@85 '(' use_initializer make_struct_dim ')'  */
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

  case 742: /* $@86: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 743: /* $@87: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 744: /* make_dim_decl: "array" "variant" '<' $@86 type_declaration_no_options '>' $@87 '(' make_struct_dim ')'  */
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

  case 745: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 746: /* $@88: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 747: /* $@89: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 748: /* make_dim_decl: "array" '<' $@88 type_declaration_no_options '>' $@89 '(' expr_list optional_comma ')'  */
                                                                                                                                                                              {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-9])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 749: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 750: /* $@90: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 751: /* $@91: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 752: /* make_dim_decl: "fixed_array" '<' $@90 type_declaration_no_options '>' $@91 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        if ( !mka->makeType->dim.size() ) mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 753: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 754: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 755: /* make_table_decl: "begin of code block" expr_map_tuple_list optional_comma "end of code block"  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 756: /* make_table_decl: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 757: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
                                                                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-7])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-7])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 758: /* make_table_decl: "table" '<' type_declaration_no_options "end of expression" type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
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

  case 759: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 760: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 761: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 762: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 763: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 764: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 765: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
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


